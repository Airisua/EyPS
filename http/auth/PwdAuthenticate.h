#ifndef EYPS_PWDAUTHENTICATE_H
#define EYPS_PWDAUTHENTICATE_H

//#include "Authenticator.h"
#include "../objects/LoginRet.h"
#include "../objects/loginAccountReq.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


class PwdAuthenticate {
public:
    PwdAuthenticate() = default;
    ~PwdAuthenticate() = default;

    void authenticate(const std::shared_ptr<LoginAccountReq>&  req, std::shared_ptr<loginRet>& rsp);
};


#endif //EYPS_PWDAUTHENTICATE_H
