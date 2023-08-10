#ifndef EYPS_TOKENAUTHENTICATE_H
#define EYPS_TOKENAUTHENTICATE_H

#include "../objects/loginTokenReq.h"
#include "../objects/LoginRet.h"

class TokenAuthenticate {
public:
    TokenAuthenticate() = default;
    ~TokenAuthenticate() = default;

    void authenticate(std::shared_ptr<LoginTokenReq>& req, std::shared_ptr<loginRet>& rsp);
};


#endif //EYPS_TOKENAUTHENTICATE_H
