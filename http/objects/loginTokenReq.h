#ifndef EYPS_LOGINTOKENREQ_H
#define EYPS_LOGINTOKENREQ_H
#include <memory>
#include <string_view>

class LoginTokenReq{
public:
    using ptr = std::shared_ptr<LoginTokenReq>;
    LoginTokenReq() = default;
    ~LoginTokenReq() = default;

    std::string_view uid{};
    std::string_view token{};
};

#endif //EYPS_LOGINTOKENREQ_H
