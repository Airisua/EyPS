#include "HttpServer.h"
#include "detect_session.h"
#include "../util/log.h"

#include <boost/asio/strand.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

void fail_(boost::system::error_code ec, char const* what) {
    LOG_FATAL(what << " : " << ec.message());
}


HttpServer::HttpServer(asio::ssl::context& ssl_context,asio::io_context &io_context, const asio::ip::tcp::endpoint& ep) :
            ssl_context(ssl_context),io_context_(io_context), acceptor_(asio::make_strand(io_context)){
    boost::system::error_code ec;

    acceptor_.open(ep.protocol(),ec);
    if(ec){
        fail_(ec,"open");
        return;
    }

    // 地址复用
    acceptor_.set_option(asio::socket_base::reuse_address(true),ec);
    if(ec) {
        fail_(ec,"set_option");
        return;
    }

    // bind
    acceptor_.bind(ep,ec);
    if(ec) {
        fail_(ec,"bind");
        return;
    }

    // listening
    acceptor_.listen(asio::socket_base::max_listen_connections,ec);
    if(ec) {
        fail_(ec,"listen");
        return;
    }

    LOG_INFO("[HttpServer::HttpServer] listening on " << ep.address().to_string().c_str() << ":" << ep.port());

}

void HttpServer::run() {
    do_accept();

}

void HttpServer::do_accept() {
    acceptor_.async_accept(asio::make_strand(io_context_),
                           boost::beast::bind_front_handler(&HttpServer::on_accept,shared_from_this()));
}

void HttpServer::on_accept(boost::system::error_code ec, asio::ip::tcp::socket socket) {
    if(ec) {
        fail_(ec,"accept");
        return;
    }  else {
        // 创建一个 session 并执行
        std::make_shared<detect_session>(std::move(socket),ssl_context)->run();
    }

    // 继续接受连接
    do_accept();
}

