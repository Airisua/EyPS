#include "PwdAuthenticate.h"
#include "../../database/Account.h"
#include "../../database/databaseHelper.h"
#include <mutex>
static std::mutex mtx;

#include <iostream>

void PwdAuthenticate::authenticate(const std::shared_ptr<LoginAccountReq>&  req, std::shared_ptr<loginRet>& rsp) {

    bool successfulLogin = false;
    std::string rspMsg;

//    Account::ptr account = std::make_shared<Account>();
    databaseHelper::ptr dbHelper = std::make_shared<databaseHelper>();
    // 检查是否存在账户
    Account account;

    dbHelper->getAccountByName(req->account, account);
    // 不存在则创建
    if(account.getUsername().empty()) {
        // creat account
        dbHelper->createAccountWithUid(req->account,0,account);
        // 创建失败
        if(account.getUsername().empty())
            rspMsg = "account_create_error";
        else {
            // 创建成功
            successfulLogin = true;
        }
    } else if(!account.getUsername().empty()){
        // 存在account
        successfulLogin = true;
    }


    if(successfulLogin) {
        rsp->setMsg("OK");
        rsp->setUid(account.getId());
        rsp->setName(account.getUsername());    // const char*有时乱码
        rsp->setToken(account.generateSessionKey());

    } else {
        rsp->setRetcode(-201);
        rsp->setMsg(rspMsg);
    }

}
