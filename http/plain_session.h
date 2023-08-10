#ifndef EYPS_PLAIN_SESSION_H
#define EYPS_PLAIN_SESSION_H

#include "HttpSession.h"

class plain_session : public HttpSession<plain_session>,
                      public std::enable_shared_from_this<plain_session>{
public:
    beast::tcp_stream stream_;


public:
    plain_session(asio::ip::tcp::socket&& socket,beast::flat_buffer buffer);
    beast::tcp_stream &stream();
    void run();
    void do_close();
};


#endif //EYPS_PLAIN_SESSION_H
