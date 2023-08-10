#include "plain_session.h"

plain_session::plain_session(asio::ip::tcp::socket &&socket, beast::flat_buffer buffer)
        : HttpSession<plain_session>(std::move(buffer)),
                stream_(std::move(socket)){
}

beast::tcp_stream &plain_session::stream() {
    return stream_;
}

void plain_session::run() {
    asio::dispatch(
        stream_.get_executor(),
        beast::bind_front_handler(
                &HttpSession::do_read,
                shared_from_this()));
}

void plain_session::do_close() {
    beast::error_code ec;
    stream_.socket().shutdown(asio::ip::tcp::socket::shutdown_send, ec);
}