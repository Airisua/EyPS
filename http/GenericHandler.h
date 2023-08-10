#ifndef EYPS_GENERICHANDLER_H
#define EYPS_GENERICHANDLER_H

#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace asio = boost::asio;
namespace beast = boost::beast;

template <class Body, class Allocator, class Send>
void GenericHandler(beast::http::request<Body, beast::http::basic_fields<Allocator>>&& req, Send&& send) {

    const auto creat_response = [&req]() {
        beast::http::response<beast::http::string_body> res {
                beast::http::status::ok, req.version()
        };
        res.set(beast::http::field::server, "EyPS");
        res.set(beast::http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        return res;
    };

    auto const proc_ver = [&]() {
        auto res_ = creat_response();

        rapidjson::StringBuffer buffer_;
        rapidjson::Writer<rapidjson::StringBuffer> writer_(buffer_);
        writer_.StartObject();
        writer_.Key("retcode"); writer_.Int(0);
        writer_.Key("message"); writer_.String("OK");
        writer_.Key("data");
        writer_.StartObject();
        writer_.Key("modified"); writer_.Bool(true);
        writer_.Key("protocol");
        writer_.StartObject();
        writer_.Key("id"); writer_.Int(0);
        writer_.Key("app_id"); writer_.Int(4);
        writer_.Key("language");   writer_.String("en");
        writer_.Key("user_proto");  writer_.String(" ");
        writer_.Key("priv_proto"); writer_.String(" ");
        writer_.Key("major"); writer_.Int(7);
        writer_.Key("minimum"); writer_.Int(0);
        writer_.Key("create_time"); writer_.String("0");
        writer_.Key("teenager_proto"); writer_.String(" ");
        writer_.Key("third_proto" );    writer_.String(" ");
        writer_.EndObject();
        writer_.EndObject();
        writer_.EndObject();

        res_.body() = buffer_.GetString();
        res_.prepare_payload();
        return res_;
    };

    const auto getAgreementInfos_ = [&]() {
        auto res_ = creat_response();
        rapidjson::StringBuffer buffer_;
        rapidjson::Writer<rapidjson::StringBuffer> writer_(buffer_);
        writer_.StartObject();

        writer_.Key("retcode"); writer_.Int(0);
        writer_.Key("message"); writer_.String("OK");
        writer_.Key("data");
        writer_.StartObject();
        writer_.Key("marketing_agreements");
        writer_.StartArray();
        writer_.EndArray();

        writer_.EndObject();
        writer_.EndObject();
        res_.body() = buffer_.GetString();
        res_.prepare_payload();
        return res_;
    };

    auto const account_ = [&]() {
        auto res_ = creat_response();

        rapidjson::StringBuffer buffer_;
        rapidjson::Writer<rapidjson::StringBuffer> writer_(buffer_);
        writer_.StartObject();
        writer_.Key("retcode");writer_.Int(0);
        writer_.Key("message");writer_.String("OK");
        writer_.Key("data");
        writer_.StartObject();
        writer_.Key("id"); writer_.String("none");
        writer_.Key("action"); writer_.String("ACTION_NONE");
        writer_.Key("geetest"); writer_.Null();
        writer_.EndObject();
        writer_.EndObject();

        res_.body() = buffer_.GetString();
        res_.prepare_payload();
        return res_;
    };

    const auto combo_ = [&]() {
        auto res_ = creat_response();
        rapidjson::StringBuffer buffer_;
        rapidjson::Writer<rapidjson::StringBuffer> writer_(buffer_);
        writer_.StartObject();
        writer_.Key("data");
        writer_.StartObject();
        writer_.Key("vals");
        writer_.StartObject();
        writer_.Key("disable_email_bind_skip"); writer_.String("false");
        writer_.Key("email_bind_remind"); writer_.String("true");
        writer_.Key("email_bind_remind_interval"); writer_.String("7");
        writer_.EndObject();
        writer_.EndObject();
        writer_.Key("message"); writer_.String("OK");
        writer_.Key("retcode"); writer_.Uint(0);
        writer_.EndObject();
        res_.body() = buffer_.GetString();
        res_.prepare_payload();
        return res_;
    };

    auto const getConfig = [&]() {
        auto res_ = creat_response();

        rapidjson::StringBuffer buffer_;
        rapidjson::Writer<rapidjson::StringBuffer> writer_(buffer_);
        writer_.StartObject();
        writer_.Key("retcode"); writer_.Int(0);
        writer_.Key("message"); writer_.String("OK");
        writer_.StartObject();
        writer_.Key("announce_url"); writer_.String("www.baidu.com");
        writer_.Key("disable_ysdk_guard"); writer_.Bool(false);
        writer_.Key("enable_announce_pic_popup"); writer_.Bool(true);
        writer_.Key("log_level"); writer_.String("INFO");
        writer_.Key("protocol"); writer_.Bool(true);
        writer_.Key("push_alias_type"); writer_.Int(2);
        writer_.Key("qr_enabled"); writer_.Bool(false);
        writer_.EndObject();
        writer_.EndObject();

        res_.body() = buffer_.GetString();
        res_.prepare_payload();
        return res_;
    };

    const auto loadConfig_ = [&](){
        auto res_ = creat_response();
        rapidjson::StringBuffer buffer_;
        rapidjson::Writer<rapidjson::StringBuffer> writer_(buffer_);
        writer_.StartObject();
        writer_.Key("client"); writer_.String("PC");
        writer_.Key("disable_mmt"); writer_.Bool(false);
        writer_.Key("enable_email_captcha"); writer_.Bool(false);
        writer_.Key("enable_ps_bind_account"); writer_.Bool(false);
        writer_.Key("disable_regist"); writer_.Bool(false);
        writer_.Key("game_key"); writer_.String("game_key");
        writer_.Key("guest"); writer_.Bool(false);
        writer_.Key("id"); writer_.Int(6);
        writer_.Key("identity"); writer_.String("I_IDENTITY");
        writer_.Key("ignore_versions"); writer_.String("");
        writer_.Key("name"); writer_.String("ps");
        writer_.Key("scene"); writer_.String("S_NORMAL");
        writer_.Key("server_guest"); writer_.Bool(false);

        writer_.Key("thirdparty");
        writer_.StartArray();
        writer_.String("fb"); writer_.String("tw");
        writer_.EndArray();

        writer_.Key("thirdparty_ignore");
        writer_.StartObject();
        writer_.Key("tw"); writer_.String("");
        writer_.Key("fb"); writer_.String("");
        writer_.EndObject();

        writer_.Key("thirdparty_login_configs");
        writer_.StartObject();

        writer_.Key("tw");
        writer_.StartObject();
        writer_.Key("token_type"); writer_.String("TK_GAME_TOKEN");
        writer_.Key("game_token_expires_in"); writer_.Int(604800);
        writer_.EndObject();
        writer_.Key("fb");
        writer_.StartObject();
        writer_.Key("token_type"); writer_.String("TK_GAME_TOKEN");
        writer_.Key("game_token_expires_in"); writer_.Int(604800);
        writer_.EndObject();

        writer_.EndObject();

        writer_.EndObject();
        res_.body() = buffer_.GetString();
        res_.prepare_payload();
        return res_;
    };

    const auto sdk_dataUpload = [&]() {
        auto res_ = creat_response();
        rapidjson::StringBuffer buffer_;
        rapidjson::Writer<rapidjson::StringBuffer> writer_(buffer_);
        writer_.StartObject();
        writer_.Key("code"); writer_.Uint(0);
        writer_.EndObject();
        res_.body() = buffer_.GetString();
        res_.prepare_payload();
        return res_;
    };


    if(req.target().starts_with("/compareProtocolVersion"))
        return send(proc_ver());
    if(req.target().starts_with("/getAgreementInfos"))
        return send(getAgreementInfos_());
    if(req.target().starts_with("/account/check"))
        return send(account_());
    if(req.target().starts_with("/config/sdk/combo"))
        return send(combo_());
    if(req.target().starts_with("/getConfig"))
        return send(getConfig());
    if(req.target().starts_with("/loadConfig"))
        return send(loadConfig_());
    if(req.target().starts_with("/dataUpload"))
        return send(sdk_dataUpload());
}

#endif //EYPS_GENERICHANDLER_H
