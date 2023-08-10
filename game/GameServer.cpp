#include "GameServer.h"
#include "../util/Crypto.h"
#include "../util/BasePacket.h"
#include "../util/log.h"
#include "../util/psConfig.h"
#include "../util/PacketIds.h"
#include "packet/PacketManager.h"
#include "packet/PacketHandler.h"

#include <boost/asio/buffer.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/write.hpp>

#include <thread>
#include <array>
#include <iostream>
#include <vector>
#include <fstream>


boost::unordered_map<unsigned long, std::shared_ptr<Kcp>> g_Clients;

int udp_output(const char* buf, int len, ikcpcb* kcp, void* user) {
    if(user) {
        auto ctx = (Context*)user;
        ctx->socket->send_to(asio::const_buffer(buf, len), ctx->endpoint);
        LOG_INFO("[GameServer::UdpOutput] Sending data to " << ctx->endpoint.address().to_string().c_str() << ":" << ctx->endpoint.port());
//        boost::asio::write(ctx->socket, asio::const_buffer(buf, len), ctx->endpoint);
    }
    return 0;
}


// 处理请求s
GameServer::GameServer(asio::io_context& io_context, const asio::ip::udp::endpoint &ep) :
        m_socket(io_context,ep){
    auto pkgs = packetManager::getInst();
    pkgs->init();

    asio::co_spawn(
        this->m_socket.get_executor(),
        [this] {
         return this->AsyncRec();
        }, asio::detached
            );

    LOG_INFO("[GameServer::GameServer] listening on " << ep.address().to_string().c_str() << ":" << ep.port());
};

GameServer::~GameServer() = default;


asio::awaitable<void> GameServer::AsyncRec() {
    try {
        std::array<std::uint8_t, 65534> packetBuffer{};
        for(;;) {
            std::size_t bufSize = co_await this->m_socket.async_receive_from(asio::buffer(packetBuffer), this->m_ep, asio::use_awaitable);
            co_await this->ParsePacket({ packetBuffer.data(), bufSize });
        }
    }   catch (const std::exception& e){
        LOG_ERROR("rec failed " << e.what());
    }

}

asio::awaitable<void> GameServer::ParsePacket(std::span<uint8_t> buffer) {
    bufferView Packet(buffer);
    std::cout << "packet size(): " << Packet.m_DataView.size_bytes() << '\n';
    auto keys = Config::getInst();
    try {
        unsigned long ip_port_num = this->m_ep.address().to_v4().to_uint() + this->m_ep.port();
        LOG_DEBUG("[GameServer::ParsePacket] Recv data from " << this->m_ep.address().to_string().c_str() << ":" << this->m_ep.port());
        if (g_Clients.find(ip_port_num) == g_Clients.end()) { // not found
            Handshake handshake;
            if (handshake.Decode(Packet, true)) { //
                LOG_DEBUG("[GameServer::ParsePacket] New connection from " << this->m_ep.address().to_string().c_str() << ":" << this->m_ep.port());
                auto* ctx = (Context*)malloc(sizeof(Context));
                ctx->endpoint = m_ep;
                ctx->socket = &m_socket;

                auto kcp = std::make_shared<Kcp>(m_ep);
                kcp->SetCallback(udp_output);
                kcp->SetContext(ctx);
                kcp->AcceptNonblock();
                kcp->Input((char*)buffer.data(), buffer.size());   //
                kcp->ip_port_num = ip_port_num;

                g_Clients[ip_port_num] = kcp;
            }
        } else {
            auto client = g_Clients[ip_port_num];
            int ret = client->Input((char*)buffer.data(), buffer.size());
            auto data = client->Recv();
            printf("have udp connection..... \n");
            std::span<uint8_t> dec_data;

            if (!data.empty()) {
                if(client->ShouldUseMT())
                    dec_data = Xor(data, client->mt_key);
                else
                    dec_data = Xor(data, keys->getDispatchKey_());
                auto DecPacket = BasePacket(dec_data);
                client->packet_queue.push_front(DecPacket);
                LOG_DEBUG("[GameServer::ParsePacket] KCP Message from " << this->m_ep.address().to_string().c_str() << ":" << this->m_ep.port() << "Packet ID: " << DecPacket.m_PacketId);
            }
        }
    }
    catch (const std::exception& e) {
        LOG_WARN(e.what());
    }

    co_return;
}


