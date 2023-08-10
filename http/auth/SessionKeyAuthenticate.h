#ifndef EYPS_SESSIONKEYAUTHENTICATE_H
#define EYPS_SESSIONKEYAUTHENTICATE_H

#include "../objects/ComboTokenReq.h"
#include "../objects/ComboTokenRes.h"

class SessionKeyAuthenticate{
public:
    SessionKeyAuthenticate() = default;
    ~SessionKeyAuthenticate() = default;

    void authenticate(std::shared_ptr<ComboTokenReq>& req,std::shared_ptr<ComboTokenRes>& rsp);
};

#endif //EYPS_SESSIONKEYAUTHENTICATE_H
