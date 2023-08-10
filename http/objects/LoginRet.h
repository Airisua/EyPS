#ifndef EYPS_LOGINRET_H
#define EYPS_LOGINRET_H

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <memory>
#include <utility>

class loginRet {
public:
    using ptr = std::shared_ptr<loginRet>;
    loginRet() = default;
    ~loginRet() = default;
    rapidjson::StringBuffer accountJson() const ;

private:
    std::string message{};
    std::string uid{};
    std::string name{};
    std::string email{};
    std::string token{};
    int retcode = 0;

public:
    void setMsg(std::string_view msg);
    void setUid(std::string_view uid);
    void setName(std::string_view name);
    void setToken(std::string_view token);
    void setRetcode(int code);
};

inline rapidjson::StringBuffer loginRet::accountJson() const{
    rapidjson::StringBuffer buffer_;
    rapidjson::Writer<rapidjson::StringBuffer> writer_(buffer_);
    writer_.StartObject();
    writer_.Key("retcode"); writer_.Int(retcode);
    writer_.Key("message"); writer_.String(message.data());
    writer_.Key("data");
    writer_.StartObject();
    writer_.Key("account");

    writer_.StartObject();
    writer_.Key("uid"); writer_.String(uid.data());
    writer_.Key("name"); writer_.String(name.data());
    writer_.Key("email"); writer_.String("EyPSEmail");
    writer_.Key("mobile"); writer_.String(" ");
    writer_.Key("is_email_verify"); writer_.String("0");
    writer_.Key("realname"); writer_.String(" ");
    writer_.Key("identity_card"); writer_.String(" ");
    writer_.Key("token"); writer_.String(token.data());
    writer_.Key("safe_mobile"); writer_.String("");
    writer_.Key("facebook_name"); writer_.String("");
    writer_.Key("google_name"); writer_.String("");
    writer_.Key("twitter_name"); writer_.String("");
    writer_.Key("game_center_name"); writer_.String("");
    writer_.Key("apple_name"); writer_.String("");
    writer_.Key("sony_name"); writer_.String("");
    writer_.Key("tap_name"); writer_.String("");
    writer_.Key("country"); writer_.String("US");
    writer_.Key("reactivate_ticket"); writer_.String("");
    writer_.Key("area_code"); writer_.String("**");
    writer_.EndObject();
    writer_.EndObject();

    writer_.Key("device_grant_required"); writer_.Bool(false);
    writer_.Key("safe_mobile_required"); writer_.Bool(false);
    writer_.Key("realperson_required"); writer_.Bool(false);
    writer_.Key("reactivate_required"); writer_.Bool(false);
    writer_.Key("realname_operation"); writer_.String("None");
    writer_.EndObject();

    return buffer_;
}

inline void loginRet::setMsg(std::string_view msg) {
    this->message = msg;
}

inline void loginRet::setUid(std::string_view uid_) {
    this->uid = uid_;
}

inline void loginRet::setName(std::string_view name_) {
    this->name = name_;
}

inline void loginRet::setToken(std::string_view  token_) {
    this->token = token_;
}

inline void loginRet::setRetcode(int code) {
    this->retcode = code;
}

#endif //EYPS_LOGINRET_H
