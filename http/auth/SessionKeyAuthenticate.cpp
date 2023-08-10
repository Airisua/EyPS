#include "SessionKeyAuthenticate.h"
#include "../../database/databaseHelper.h"

void SessionKeyAuthenticate::authenticate(std::shared_ptr<ComboTokenReq> &req, std::shared_ptr<ComboTokenRes>& rsp) {

    bool successfulLogin = false;

    Account account;
    databaseHelper::ptr dbHelper = std::make_shared<databaseHelper>();
    // 更新uid获取account
    dbHelper->getAccountById(req->uid, account);

    std::cout << "v2_login: sessionKey: " << account.getSessionKey() << std::endl;
    std::cout << "v2_loin: token: " << req->token << std::endl;

    successfulLogin = !account.getId().empty() && account.getSessionKey() == req->token;
    if(successfulLogin) {
        rsp->message = "OK";
        rsp->open_id = account.getId();
        rsp->combo_id = "123456789";
        rsp->combo_token = account.generateLoginToken();
    }  else {
        rsp->retcode = -201;
        rsp->message = "account_sessionKey_error";
    }

}

