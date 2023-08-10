#ifndef EYPS_HTTPSERVER_H
#define EYPS_HTTPSERVER_H

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/error.hpp>
#include <memory>
#include <iostream>
#include <boost/asio/ssl.hpp>

namespace asio = boost::asio;



// 接收http连接
class HttpServer : public std::enable_shared_from_this<HttpServer> {
public:
    HttpServer(asio::ssl::context& ssl_context,asio::io_context& io_context, const asio::ip::tcp::endpoint& ep);


    void run();

private:
    void do_accept();
    void on_accept(boost::beast::error_code ec, asio::ip::tcp::socket socket);

private:
    asio::io_context& io_context_;
    asio::ssl::context& ssl_context;
    asio::ip::tcp::acceptor acceptor_;
};


#endif //EYPS_HTTPSERVER_H
