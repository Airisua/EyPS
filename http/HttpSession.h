#ifndef EYPS_HTTPSESSION_H
#define EYPS_HTTPSESSION_H


#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <mutex>
#include <vector>
#include <map>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include "../util/log.h"
#include "HttpHandleReq.h"
#include "GenericHandler.h"
#include "LoginHandler.h"


namespace asio = boost::asio;
namespace beast = boost::beast;


template<class Derived>
class HttpSession{
public:
    // 返回子类
     Derived& derived(){
        return static_cast<Derived&>(*this);
    }

    // 用于发送http消息

    struct send_lambda {
        Derived& self_ ;
//
        explicit send_lambda(Derived& self) :self_(self) {}

        template <bool isRequest, class Body, class Fields>
        void operator()(boost::beast::http::message<isRequest, Body, Fields>&& msg) const {

            bool keep_alive =  msg.keep_alive();

            LOG_DEBUG("async_write");
            // 异步写
            beast::http::async_write(self_.stream(),std::move(msg),
                                     beast::bind_front_handler(&HttpSession::on_write,
                                                               self_.shared_from_this(),
                                                               keep_alive));
        }

    };


    beast::flat_buffer buffer_;
    beast::http::request<beast::http::string_body> req_;
//    std::shared_ptr<void> res_;
    send_lambda sLambda;



public:;
    explicit HttpSession(beast::flat_buffer buffer) :  buffer_(std::move(buffer)), sLambda(derived()){};

    virtual ~HttpSession() = default ;



    void do_read();

    void on_read(beast::error_code ec,std::size_t bt_transferred);
    void on_write(bool close,beast::error_code ec,std::size_t bt_transferred);
    void run();
//    void do_close();


};

template<class Derived>
void HttpSession<Derived>::do_read() {

    req_ = {};  //


    //
    beast::get_lowest_layer(
            derived().stream()).expires_after(std::chrono::seconds(30));

    beast::http::async_read(
            derived().stream(),buffer_,req_,beast::bind_front_handler(
                    &HttpSession::on_read,derived().shared_from_this()
            )
    );

}

template<class Derived>
void HttpSession<Derived>::on_read(beast::error_code ec, std::size_t bt_transferred) {
    boost::ignore_unused(bt_transferred);
    if(ec == beast::http::error::end_of_stream)
        return derived().do_close();

    if(ec) {
        LOG_WARN("read: " << ec.message());
        return;
    }

    handle_request(std::move(req_),sLambda);    // gate
    GenericHandler(std::move(req_),sLambda);
    LoginHandler(std::move(req_),sLambda);

    // test
}


template<class Derived>
void HttpSession<Derived>::on_write(bool close, beast::error_code ec, std::size_t bt_transferred) {
    boost::ignore_unused(bt_transferred);
    if(ec) {
        LOG_WARN("write: " << ec.message());
        return;
    }

    // Connection: close
    if(!close)   //
        return derived().do_close();

    // 已经完成了 response
//    derived().res_ = nullptr;

    do_read();  // 继续读新的请求
}

template<class Derived>
void HttpSession<Derived>::run() {
    asio::dispatch(
            derived().stream().get_executor(),
            beast::bind_front_handler(&HttpSession::do_read, derived().shared_from_this()));
}


#endif //EYPS_HTTPSESSION_H



