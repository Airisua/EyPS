#ifndef EYPS_SSL_SESSION_H
#define EYPS_SSL_SESSION_H

#include <boost/beast/ssl.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include "HttpSession.h"

class ssl_session : public  HttpSession<ssl_session>,
        public std::enable_shared_from_this<ssl_session>{
public:

    beast::ssl_stream<beast::tcp_stream> stream_;
    std::shared_ptr<void> res_;

public:
    ssl_session(asio::ip::tcp::socket&& socket,asio::ssl::context& ssl_context,beast::flat_buffer buffer);
    void run();
    void do_close();
    void on_shutdown(beast::error_code ec);
    void on_handshake(beast::error_code ec,std::size_t bytes_used);
    beast::ssl_stream<beast::tcp_stream>&stream();
};


#endif //EYPS_SSL_SESSION_H
