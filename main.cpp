#include <iostream>
#include <thread>
#include <string_view>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl.hpp>

#include "game/GameServer.h"
#include "http/HttpServer.h"
#include "util/psConfig.h"
#include "util/log.h"
#include "database/databaseManager.h"
#include "pool/IOServicePool.h"
#include <boost/asio/signal_set.hpp>

int main() {
    auto logger = logger::getInst();
    logger->init("test.log",0,0,1,1);

    auto Config = Config::getInst();
    Config->loadKeys();
    Config->initCig();

    auto dataInit = databaseManager::getInst();
    dataInit->initDataBase();

    auto pool = IOServicePool::getInst();

    auto const m_address = asio::ip::make_address(Config->dispatchAddr);
    auto const m_address_ = asio::ip::make_address(Config->gameAddr);

    asio::io_context io_context;
    boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
    signals.async_wait([&io_context,pool](auto,auto) {
        io_context.stop();
        pool->stop();
    });

    asio::ssl::context ctx{asio::ssl::context::tlsv12};

    load_certificate(ctx);

    // 处理tcp请求
    std::make_shared<HttpServer>(ctx,pool->getIOService(),asio::ip::tcp::endpoint{m_address,Config->dispatchPort})
            ->run();
    // 处理udp请求
    GameServer KcpGameServer(pool->getIOService(),asio::ip::udp::endpoint{m_address_,Config->gamePort});

   io_context.run();

    return 0;
}