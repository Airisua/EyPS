#ifndef EYPS_LOGINHANDLER_H
#define EYPS_LOGINHANDLER_H

#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <iostream>
#include <string_view>
#include "../util/util.h"
#include "../util/Crypto.h"
#include "objects/loginAccountReq.h"
#include "objects/loginTokenReq.h"
#include "auth/Authenticator.h"
#include "auth/PwdAuthenticate.h"
#include "auth/TokenAuthenticate.h"
#include "auth/SessionKeyAuthenticate.h"
#include "objects/LoginRet.h"
#include "objects/ComboTokenReq.h"
#include "objects/ComboTokenRes.h"

namespace asio = boost::asio;
namespace beast = boost::beast;


static rapidjson::StringBuffer handleLogin(std::string_view& body) {

        LoginAccountReq::ptr accountReq = std::make_shared<LoginAccountReq>();

        rapidjson::Document doc;
//        jsonParse(doc,body);
        doc.Parse(body.data());

//        if(doc.Empty()) return;

        accountReq->account = doc.FindMember("account")->value.GetString();
        accountReq->password = doc.FindMember("password")->value.GetString();
        accountReq->is_crypto = doc.FindMember("is_crypto")->value.GetBool();


        std::shared_ptr<PwdAuthenticate> auth = std::make_shared<PwdAuthenticate>();

        loginRet::ptr rsp  = std::make_shared<loginRet>();
        auth->authenticate(accountReq,rsp);

        return rsp->accountJson();
}

static rapidjson::StringBuffer handleTokenV(std::string_view & body) {
        LoginTokenReq::ptr tokeReq = std::make_shared<LoginTokenReq>();
        rapidjson::Document doc;
//        jsonParse(doc,body);
        doc.Parse(body.data());

        tokeReq->uid = doc.FindMember("uid")->value.GetString();
        tokeReq->token = doc.FindMember("token")->value.GetString();

        std::shared_ptr<TokenAuthenticate> auth = std::make_shared<TokenAuthenticate>();
        loginRet::ptr rsp  = std::make_shared<loginRet>();

        auth->authenticate(tokeReq,rsp);

        return rsp->accountJson();
}

static rapidjson::StringBuffer handleSessionKey(std::string_view & body) {
        // token + session key 组合验证
        ComboTokenReq::ptr comboTokenReq = std::make_shared<ComboTokenReq>();
        rapidjson::Document doc;
//        jsonParse(doc,body);    //
        doc.Parse(body.data());
        rapidjson::Document data_;
        data_.Parse(doc.FindMember("data")->value.GetString());

        comboTokenReq->uid = data_.FindMember("uid")->value.GetString();
        comboTokenReq->token = data_.FindMember("token")->value.GetString();
        comboTokenReq->guest = data_.FindMember("guest")->value.GetBool();
        comboTokenReq->app_id = doc.FindMember("app_id")->value.GetInt();
        comboTokenReq->channel_id = doc.FindMember("channel_id")->value.GetInt();
        comboTokenReq->device = doc.FindMember("device")->value.GetString();


        std::shared_ptr<SessionKeyAuthenticate> auth = std::make_shared<SessionKeyAuthenticate>();
        ComboTokenRes::ptr rsp = std::make_shared<ComboTokenRes>();
        auth->authenticate(comboTokenReq,rsp);

        return rsp->ComboTokenJson();
}

template <class Body, class Allocator, class Send>
void LoginHandler(beast::http::request<Body, beast::http::basic_fields<Allocator>>&& req, Send&& send) {

    const auto creat_response = [&req]() {
        beast::http::response<beast::http::string_body> res {
                beast::http::status::ok, req.version()
        };
        res.set(beast::http::field::server, "EyPS");
        res.set(beast::http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        return res;
    };

    const auto clientLogin = [&]() {
        // body :  account    password    is_crypto
        std::cout << "login_ body: "<< req.body() << '\n';
        std::string_view bodyData = req.body();

        auto bodyJson = handleLogin(bodyData);

        auto res_ = creat_response();
        res_.body() = bodyJson.GetString();
        res_.prepare_payload();
        return res_;

    };

    const auto tokenLogin = [&]() {
        // body toke uid
        std::cout << "token_ body: "<< req.body() << '\n';
        std::string_view bodyData = req.body();

        auto bodyJson = handleTokenV(bodyData);


        auto res_ = creat_response();
        res_.body() = bodyJson.GetString();
        res_.prepare_payload();
        return res_;
    };

    const auto sessionKeyLogin = [&]() {
        std::cout << "login_v2_body: "<< req.body() << '\n';
        std::string_view bodyData = req.body();

        auto bodyJson = handleSessionKey(bodyData);

        auto res_ = creat_response();
        res_.body() = bodyJson.GetString();
//        res_.body() = "";
        res_.prepare_payload();
        return res_;
    };

    // cache    login -> sessionKsy & token
    if(req.target().starts_with("login"))   // login
        return send(clientLogin());
    if(req.target().starts_with("verify"))  // cache
        return send(tokenLogin());
    if(req.target().starts_with("v2/login"))   // sessionKey & token
        return send(sessionKeyLogin());
}

#endif //EYPS_LOGINHANDLER_H
