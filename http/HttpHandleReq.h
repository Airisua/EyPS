#ifndef EYPS_HTTPHANDLEREQ_H
#define EYPS_HTTPHANDLEREQ_H

#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <mutex>
#include <vector>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>

#include "../resources/proto/QueryRegionListHttpRsp.pb.h"
#include "../resources/proto/RegionSimpleInfo.pb.h"
#include "../resources/proto/RegionInfo.pb.h"
#include "../resources/proto/QueryCurrRegionHttpRsp.pb.h"
#include "../util/Crypto.h"
#include "../util/psConfig.h"
#include "../util/dynamicBuffer.h"
#include "../util/log.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace asio = boost::asio;
namespace beast = boost::beast;


template <class Body, class Allocator, class Send>
void handle_request(beast::http::request<Body, beast::http::basic_fields<Allocator>>&& req, Send&& send) {
    // bad request response
    auto const bad_request = [&req](beast::string_view why) {
        beast::http::response<beast::http::string_body> res {
                beast::http::status::bad_request,req.version()
        };
        res.set(beast::http::field::server,BOOST_BEAST_VERSION_STRING);
        res.set(beast::http::field::content_type,"text/html");
        res.keep_alive(req.keep_alive());
        res.body() = std::string(why);
        res.prepare_payload();
        return res;
    };

    // not found response
    auto const not_found =[&req](beast::string_view target) {
        beast::http::response<beast::http::string_body> res {
                beast::http::status::not_found,req.version()
        };
        res.set(beast::http::field::server,BOOST_BEAST_VERSION_STRING);
        res.set(beast::http::field::content_type,"text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "the resource " + std::string(target) + " was not found";
        res.prepare_payload();
        return res;
    };

    // server error resp
    auto const server_error = [&req](beast::string_view what) {
        beast::http::response<beast::http::string_body> res {
                beast::http::status::internal_server_error,req.version()
        };
        res.set(boost::beast::http::field::server,BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type,"text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "an error: " + std::string(what);
        res.prepare_payload();
        return res;
    };

    // test

    LOG_DEBUG("[target] " << req.target());

    const auto creat_response = [&req]() {
        beast::http::response<beast::http::string_body> res {
                beast::http::status::ok, req.version()
        };
        res.set(beast::http::field::server, "EyPS");
        res.set(beast::http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        return res;
    };


    auto const do_query_region_list = [&]() {
        auto res_ = creat_response();
        auto Config = Config::getInst();
        QueryRegionListHttpRsp rsp{};
        std::string_view clientCustomConfig_ = R"({"sdkenv":"2","checkdevice":"false","loadPatch":"false","showexception":"false","regionConfig":"pm|fk|add","downloadMode":"0"})";

        auto patchKey = Xor(clientCustomConfig_,Config->getDispatchKey_());  //

        auto addInfo = rsp.add_region_list();
        addInfo->template set_type("DEV_PUBLIC");
        addInfo->template set_name(Config->regionName);
        addInfo->template set_title(Config->regionTitle);
        addInfo->template set_dispatch_url("https://" + Config->dispatchAddr + ":" + std::to_string(Config->dispatchPort) + "/query_cur_region/" + Config->regionName);


        rsp.set_retcode(0);
        rsp.set_enable_login_pc(true);
        rsp.template set_client_secret_key((const std::string &)std::move(Config->getDispatchSeed_()));
        rsp.template set_client_custom_config_encrypted((const std::string&)patchKey);


        std::string fuB{};
        Base64Encode(rsp.SerializeAsString(),fuB);
        res_.body() = fuB;
        res_.prepare_payload();
        // bt 2284
        // regin_list      75     bt 2209
        // secret_key  2079   bt 205
        // patchKey  2156     bt 128

        LOG_DEBUG("do_region_list");
        return res_;

    };

    auto const do_query_cur_region = [&]() {
        auto res_ = creat_response();
        auto Config = Config::getInst();

        QueryCurrRegionHttpRsp updatedQuery{};
        updatedQuery.set_retcode(0);
        RegionInfo rInfo;
        rInfo.set_gateserver_port(Config->gamePort);
        rInfo.template set_gateserver_ip(Config->gameAddr);

        rInfo.template set_secret_key((std::string&)std::move(Config->getDispatchSeed_()));

        updatedQuery.mutable_region_info()->CopyFrom(rInfo);



        std::string buf {};
        Base64Encode(updatedQuery.SerializeAsString(),buf);



        std::string bf{"CAESGE5vdCBGb3VuZCB2ZXJzaW9uIGNvbmZpZw=="}; // not found version config
        res_.body() = buf;
        res_.prepare_payload();

        LOG_DEBUG("do_cur_region");
        return res_;



        // 反
//        std::string buf_ = "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";

//        std::cout << '\n' << std::endl;
//        std::cout << "cur_list:  bt" << updatedQuery.region_info().ByteSizeLong() << std::endl;
//        std::cout << "cur_list res_url: " << updatedQuery.region_info().resource_url() << std::endl;
//        QueryCurrRegionHttpRsp psr;
//        std::string buff{};
//        Base64Decode(buf_,buff);
//        psr.ParseFromString(buff);
//        std::cout << "buff: " << buff << std::endl;

    };

    if(req.target().starts_with("/region_list"))
        return send(do_query_region_list());
    if(req.target().starts_with("/cur_region"))
        return send(do_query_cur_region());


    {

    }


    if(req.target()[1] == '?') {
        auto res_ = creat_response();
        res_.body() = "Hello from EyPS!" ;
        res_.prepare_payload();
        return send(std::move(res_));
    }
}

#endif //EYPS_HTTPHANDLEREQ_H
