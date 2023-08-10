#include "ssl_session.h"
#include "../util/log.h"

ssl_session::ssl_session(asio::ip::tcp::socket &&socket, asio::ssl::context &ssl_context, beast::flat_buffer buffer)
            : HttpSession<ssl_session>(std::move(buffer)),
                    stream_(std::move(socket),ssl_context){
}


beast::ssl_stream<beast::tcp_stream> &ssl_session::stream() {
    return stream_;
}

void ssl_session::run() {
    auto self = shared_from_this();
    asio::dispatch(stream_.get_executor(),[self]() {
        beast::get_lowest_layer(self->stream_).expires_after(
                std::chrono::seconds(30));

        self->stream_.async_handshake(
                asio::ssl::stream_base::server,
                self->buffer_.data(),
                beast::bind_front_handler(&ssl_session::on_handshake, self)
                );
    });
}

void ssl_session::on_handshake(beast::error_code ec, std::size_t bytes_used) {
    if(ec) {
        LOG_ERROR("handshake: " << ec.message());
        return;
    };

    buffer_.consume(bytes_used);

    HttpSession<ssl_session>::run();

}

void ssl_session::do_close() {
    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));
    stream_.async_shutdown(
            beast::bind_front_handler(&ssl_session::on_shutdown,shared_from_this())
            );
}

void ssl_session::on_shutdown(beast::error_code ec) {
    if(ec) {
        LOG_ERROR("shutdown: " << ec.message());
        return;
    };
}
