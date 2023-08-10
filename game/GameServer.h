#ifndef EYPS_GAMESERVER_H
#define EYPS_GAMESERVER_H
#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/unordered_map.hpp>

#include <span>
#include <cstdint>
#include <memory>
#include "../util/BufferView.h"
#include "Kcp.h"

namespace asio = boost::asio;

extern boost::unordered_map<unsigned long, std::shared_ptr<Kcp>> g_Clients;

class GameServer {
public:
    GameServer(asio::io_context& io_context, const asio::ip::udp::endpoint& ep);
    [[nodiscard]] uint16_t GetPort() const;
    ~GameServer();

public:
    asio::awaitable<void> AsyncRec();   //
    asio::awaitable<void> ParsePacket(std::span<uint8_t> buffer);
private:
    asio::ip::udp::socket m_socket;
    asio::ip::udp::endpoint m_ep;
};


#endif //EYPS_GAMESERVER_H
