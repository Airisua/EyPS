#ifndef EYPS_DETECT_SESSION_H
#define EYPS_DETECT_SESSION_H

#include <memory>
#include <boost/beast/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>

namespace beast = boost::beast;
namespace asio = boost::asio;

class detect_session : public std::enable_shared_from_this<detect_session>{
    beast::tcp_stream tcp_stream;
    asio::ssl::context& ssl_context;
    beast::flat_buffer buffer_;

public:
    detect_session(asio::ip::tcp::socket&& socket,asio::ssl::context& ssl_context);

    void run();
    void on_detect(beast::error_code ec, bool result);
};


#endif //EYPS_DETECT_SESSION_H
