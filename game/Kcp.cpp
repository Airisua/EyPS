#include "Kcp.h"
#include <memory>
#include <utility>
#include <chrono>
#include <ctime>

extern std::unordered_map<int,std::shared_ptr<PacketHandler>> m_Packets;
extern boost::unordered_map<unsigned long, std::shared_ptr<Kcp>> g_Clients;

Kcp::Kcp(asio::ip::udp::endpoint ep, int conv, int token, void *ctx) :
    _Conv(conv),_Token(token),startTime(time(0)),_recvBuf((char*)malloc(65534)),endpoint(std::move(ep)) {

}

Kcp::~Kcp() {
    _State = Kcp::connectionState::DISCONNECTED;
    ikcp_release(kcp);
    free(_recvBuf);
    free(_ctx);
}

void Kcp::Background() {
    for (;;) {
        if (_State != Kcp::connectionState::CONNECTED || (time(0) - lastPacketTime) > 20) {
            _State = Kcp::connectionState::DISCONNECTED;
            delete this;
            return;
        }

        auto current = clock();
        ikcp_update(this->kcp, current);
        auto ts = ikcp_check(this->kcp, current);


        std::this_thread::sleep_for(std::chrono::milliseconds(ts - current));
    }
}

// token req
// login req
// ping req
// PlayerSetPauseReq
// EnterSceneReadyReq

void Kcp::PacketQueueHandler() {
    for(;;) {
        if (_State != Kcp::connectionState::CONNECTED) {
            return;
        }
        std::this_thread::sleep_for(std::chrono::nanoseconds(5));
        while (!this->packet_queue.empty()) {
            BasePacket packet = this->packet_queue.back();
            this->packet_queue.pop_back();
//            std::cout << "packet id: " << packet.m_PacketId << '\n';

            if(packet.m_PacketId == PacketIds::PingReq) {
//                std::cout << "packet id:  " << packet.m_PacketId << "\n";
            } else if(packet.m_PacketId == PacketIds::GetPlayerTokenReq) {
                if(sessionState != SessionState::WAITING_FOR_TOKEN)
                    return;
            } else if(packet.m_PacketId == PacketIds::PlayerLoginReq) {
                if(sessionState != SessionState::WAITING_FOR_LOGIN)
                    return;
            } else if(packet.m_PacketId == PacketIds::SetPlayerBornDataReq){
                if(sessionState != SessionState::PICKING_CHARACTER)
                    return;
            } else {
                if(sessionState != SessionState::ACTIVE)
                    return;
            }

            m_Packets[packet.m_PacketId]->handler(this,packet);
        }
    }
}

void Kcp::SetCallback(int (*callback_ptr)(const char *, int, struct IKCPCB *, void *)) {
    this->_callback_ptr = callback_ptr;
}

void Kcp::SetContext(Context *ctx) {
    this->_ctx = ctx;
}

[[maybe_unused]] std::span<uint8_t> Kcp::Recv() {
    int sz = ikcp_peeksize(kcp);
    if (sz && sz != -1) {
        int result = ikcp_recv(kcp, _recvBuf, sz);
        if (result > 0) {
            return { std::move((uint8_t*)_recvBuf), (size_t)result };
        }
    }
    return {(uint8_t*)nullptr, 0};
}

int Kcp::Input(char *buffer, long len) {
    int status = 0;
    Handshake handshake;
    std::span<uint8_t> handshake_result;
    LOG_DEBUG("kcp recv:  state" << _State);        // 首次 3 wait
    
    lastPacketTime = time(0);

    switch (_State){
        case Kcp::connectionState::DISCONNECTED:
        case Kcp::connectionState::CLOSED:
            //do something
            break;
        case Kcp::connectionState::CONNECTED:
//            if (len <= 20) {
//                //do something
//            }

            status = ikcp_input(this->kcp, buffer, len);
            if (status == -1) {
                //_State = Kcp::ConnectionState::CLOSED;
                //abort connection
            }
            break;
        case Kcp::connectionState::HANDSHAKE_WAIT:
            _Conv = (time(0) & 0xFFFFFFFF);
            _Token = 0xFFCCEEBB ^ ((time(0) >> 32) & 0xFFFFFFFF);

            handshake.Magic1 = handshake.MAGIC_SEND_BACK_CONV[0];   // start
            handshake.Magic2 = handshake.MAGIC_SEND_BACK_CONV[1];   // end

            handshake.Conv = _Conv;
            handshake.Token = _Token;
            handshake_result = handshake.Encode();

            std::cout << "handshake2 result: " << handshake_result.size() << std::endl;

            _ctx->socket->send_to(asio::const_buffer(handshake_result.data(), handshake_result.size()), _ctx->endpoint);
            Initialize();
            status = 0;

        case Kcp::connectionState::HANDSHAKE_CONNECT:
            break;
    }
    return status;
}

int Kcp::Send(std::span<uint8_t> buffer) {
    int ret = ikcp_send(kcp, (char*)buffer.data(), buffer.size());
    ikcp_flush(kcp);

    return ret;
}

int Kcp::Send(std::vector<uint8_t> buffer) {
    int ret = ikcp_send(kcp, (char*)buffer.data(), buffer.size());
    ikcp_flush(kcp);

    return ret;
}

int Kcp::Send(BasePacket data, bool use_dispatch_key) {
    std::lock_guard<std::mutex> locker(kcplock);
    auto cif = Config::getInst();
    auto encrypted = Xor(data.GetDataOwnership(), use_dispatch_key ? cif->getDispatchKey_() : this->mt_key);
    int ret = ikcp_send(kcp, (char*)encrypted.data(), encrypted.size());
    ikcp_flush(kcp);
    //kcplock.unlock();

    printf("send packet.....\n");
//    free(encrypted.data());   //
    return ret;
}

void Kcp::AcceptNonblock() {
    _State = Kcp::connectionState::HANDSHAKE_WAIT;
    sessionState = Kcp::SessionState::WAITING_FOR_TOKEN;
}

void Kcp::Initialize() {
    LOG_DEBUG("init kcp");
    kcp = ikcp_create(_Conv, _Token, _ctx);
    ikcp_nodelay(kcp, 1, 10, 2, 1);
    ikcp_wndsize(kcp, 4096, 4096);
    ikcp_setoutput(kcp, _callback_ptr);
    _State = Kcp::connectionState::CONNECTED;
    background_thread = std::thread(&Kcp::Background, this);
    background_thread.detach();
    packet_handler_thread = std::thread(&Kcp::PacketQueueHandler, this);
    packet_handler_thread.detach();
}

bool Kcp::ShouldUseMT() {
    return _ShouldUseMt;
}

void Kcp::SetUseMT(bool state) {
    _ShouldUseMt = state;
}

void Kcp::GenerateMTKey(unsigned long long seed) {
    mt_key = generateKey(seed);
}


Handshake::Handshake(int *magic, int conv, int data, int token) {
    Magic1 = magic[0];
    Magic2 = magic[1];

    Conv = conv;
    Data = data;
    Token = token;
}

Handshake::Handshake()
{
    Magic1 = 0;
    Magic2 = 0;

    Conv = 1;
    Data = 1234567890;
    Token = 1;
}

bool Handshake::Decode(bufferView buffer, bool verifyMagic) {
    if (buffer.m_DataView.size_bytes() < Handshake::LEN)
        return false;
    std::cout << "decode: " << buffer.m_DataView.size_bytes() << '\n';
    Magic1 = buffer.Read<std::uint32_t>(true);
    Conv = buffer.Read<std::uint32_t>(true);
    Token = buffer.Read<std::uint32_t>(true);
    Data = buffer.Read<std::uint32_t>(true);
    Magic2 = buffer.Read<std::uint32_t>(true);

    if (verifyMagic)
        return (Magic1 == Handshake::MAGIC_CONNECT[0] &&
                Magic2 == Handshake::MAGIC_CONNECT[1]);
}

std::span<uint8_t> Handshake::Encode() {
    auto& bufRef = this->GetBuffer();
    bufRef.Write<std::uint32_t>(Magic1, true);
    bufRef.Write<std::uint32_t>(Conv, true);
    bufRef.Write<std::uint32_t>(Token, true);
    bufRef.Write<std::uint32_t>(Data, true);
    bufRef.Write<std::uint32_t>(Magic2, true);
    return { bufRef.GetDataOwnership().data(), 20};      //
}