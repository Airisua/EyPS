#include "detect_session.h"
#include "ssl_session.h"
#include "plain_session.h"
#include "../util/log.h"


detect_session::detect_session(asio::ip::tcp::socket &&socket, asio::ssl::context &ssl_context) :
                tcp_stream(std::move(socket)),ssl_context(ssl_context){};

void detect_session::run() {
    // 设置超时
    beast::get_lowest_layer(tcp_stream).expires_after(std::chrono::seconds(30));


    async_detect_ssl(
            tcp_stream,buffer_,beast::bind_front_handler(
                    &detect_session::on_detect,
                    shared_from_this()));

}

void detect_session::on_detect(beast::error_code ec, bool result) {
    if(ec){
        LOG_ERROR(ec.message());
        return;
    }

    // 调用 ssl session
    if(result) {
        std::make_shared<ssl_session>(tcp_stream.release_socket(),ssl_context,
                std::move(buffer_))->run();
        return;
    }
    // 调用普通 session
    std::make_shared<plain_session>(tcp_stream.release_socket(),std::move(buffer_))
            ->run();
}