#ifndef EYPS_LOGINACCOUNTREQ_H
#define EYPS_LOGINACCOUNTREQ_H

#include <string>
#include <string_view>
#include <memory>

class LoginAccountReq{
public:
    using ptr = std::shared_ptr<LoginAccountReq>;
    LoginAccountReq() = default;
    ~LoginAccountReq() = default;

    std::string_view account{};
    std::string_view password{};
    bool is_crypto{};

};

#endif //EYPS_LOGINACCOUNTREQ_H
