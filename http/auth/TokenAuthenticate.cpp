#include "TokenAuthenticate.h"
#include "../../database/databaseHelper.h"
#include <mutex>
static std::mutex mtx;

void TokenAuthenticate::authenticate(std::shared_ptr<LoginTokenReq> &req, std::shared_ptr<loginRet>& rsp) {

    bool successfulLogin = false;
    std::string rspMsg;

    Account account;
    databaseHelper::ptr dbHelper = std::make_shared<databaseHelper>();

    // 根据uid获得account
    dbHelper->getAccountById(req->uid, account);
    std::cout << "cache sessionKey: " << account.getSessionKey() <<std::endl;
    successfulLogin = !account.getId().empty() && account.getSessionKey() == req->token;

    if(successfulLogin) {
        rsp->setMsg("OK");
        rsp->setUid(account.getId());
        rsp->setToken(account.getSessionKey());
    } else {
        rsp->setRetcode(-201);
        rsp->setMsg("account_cache_error") ;
    }

}
