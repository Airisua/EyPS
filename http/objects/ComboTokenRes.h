#ifndef EYPS_COMBOTOKENRES_H
#define EYPS_COMBOTOKENRES_H

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "../../util/Singleton.h"

class ComboTokenRes {
public:
    using ptr = std::shared_ptr<ComboTokenRes>;
    ComboTokenRes() = default;
    ~ComboTokenRes()  = default;
    rapidjson::StringBuffer ComboTokenJson() const;

public:
    std::string message{};
    std::string combo_id{};
    std::string combo_token {};
    std::string open_id {};
    int retcode=0;
};

inline rapidjson::StringBuffer ComboTokenRes::ComboTokenJson() const {
    rapidjson::StringBuffer buffer_;
    rapidjson::Writer<rapidjson::StringBuffer> writer_(buffer_);
    writer_.StartObject();
    writer_.Key("retcode"); writer_.Int(retcode);
    writer_.Key("message"); writer_.String(message.data());
    writer_.Key("data");
    writer_.StartObject();
    writer_.Key("combo_id"); writer_.String(combo_id.data());
    writer_.Key("open_id"); writer_.String(open_id.data());
    writer_.Key("combo_token"); writer_.String(combo_token.data());
    writer_.Key("data");
    writer_.StartObject();
    writer_.Key("guest"); writer_.Bool(false);
    writer_.EndObject();
    writer_.Key("heartbeat"); writer_.Bool(false);
    writer_.Key("account_type"); writer_.Int(1);
    writer_.EndObject();
    writer_.EndObject();

    return buffer_;
}

#endif //EYPS_COMBOTOKENRES_H
