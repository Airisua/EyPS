#ifndef EYPS_KCP_H
#define EYPS_KCP_H

#include <ikcp.h>
#include <mutex>
#include <thread>
#include <span>
#include <deque>
#include <unordered_map>

#include <boost/asio/buffer.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/unordered_map.hpp>

#include "../util/bufferView.h"
#include "../util/dynamicBuffer.h"
#include "../util/Crypto.h"
#include "../util/BasePacket.h"
#include "../util/log.h"
#include "../util/PacketIds.h"
#include "../util/psConfig.h"
#include "packet/PacketHandler.h"

namespace asio = boost::asio;

struct Context {
    asio::ip::udp::endpoint endpoint;
    asio::ip::udp::socket* socket;
};

class Kcp {
public:
    enum connectionState {
        CLOSED, DISCONNECTED, HANDSHAKE_CONNECT, HANDSHAKE_WAIT, CONNECTED
    };

    enum SessionState{
        INACTIVE,
        WAITING_FOR_TOKEN,
        WAITING_FOR_LOGIN,
        PICKING_CHARACTER,
        ACTIVE
    };

    Kcp(asio::ip::udp::endpoint ep, int conv = 0, int token = 0, void* ctx = nullptr);
    ~Kcp();
    void SetCallback(int (*_callback_ptr)(const char* buf, int len, struct IKCPCB* kcp, void* user));
    void SetContext(Context* ctx);

    void AcceptNonblock();

    int Input(char* buffer, long len);

    [[maybe_unused]] std::span<uint8_t> Recv();
    int Send(std::span<uint8_t> buffer);
    int Send(std::vector<uint8_t> buffer);
    int Send(BasePacket data,bool use_dispatch_key = false);

    bool ShouldUseMT();
    void SetUseMT(bool state);
    void GenerateMTKey(unsigned long long seed);

    void PacketQueueHandler();
    void Initialize();
    void Background();
    unsigned long ip_port_num{};
    asio::ip::udp::endpoint endpoint;
    std::vector<uint8_t> mt_key;
    std::deque<BasePacket> packet_queue;
    std::mutex kcplock;
    SessionState sessionState = SessionState::WAITING_FOR_TOKEN;

private:
    unsigned int _Conv;
    unsigned int _Token;
    long lastPacketTime{};
    long startTime;
    int (*_callback_ptr) (const char* buf, int len, struct IKCPCB* kcp, void* user){};
    connectionState _State;
    ikcpcb* kcp{};
    Context* _ctx{};
    char* _recvBuf;
    std::thread background_thread;
    std::thread packet_handler_thread;
    bool _ShouldUseMt = false;
};

class Handshake {
public:
    Handshake(int* magic, int conv, int data, int token);
    Handshake();

    const unsigned int MAGIC_CONNECT[2] = {0xFF, 0xFFFFFFFF};
    const int MAGIC_SEND_BACK_CONV[2] = {0x145, 0x14514545};
    const int MAGIC_DISCONNECT[2] = {0x194, 0x19419494};
    const int LEN = 20;

    int Magic1;
    unsigned int Conv;
    unsigned int Token;
    int Data;
    int Magic2;

    bool Decode(bufferView buffer, bool verifyMagic = false);
    std::span<uint8_t> Encode();
    [[nodiscard]] inline dynamicBuffer& GetBuffer() { return this->m_Buffer; }
private:
    dynamicBuffer m_Buffer;
};


#endif //EYPS_KCP_H
