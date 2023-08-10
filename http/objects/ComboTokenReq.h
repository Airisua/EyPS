#ifndef EYPS_COMBOTOKENREQ_H
#define EYPS_COMBOTOKENREQ_H
#include <memory>
#include <string_view>

class ComboTokenReq{
public:
    using ptr = std::shared_ptr<ComboTokenReq>;
    ComboTokenReq() = default;
    ~ComboTokenReq() = default;

//    const char* data{};
    std::string_view device{};      //
    std::string_view sign{};        //
    std::string_view uid{};
    std::string_view token{};
    int app_id{};              //
    int channel_id{};
    bool guest{};              //
};

#endif//EYPS_COMBOTOKENREQ_H
