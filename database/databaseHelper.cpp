#include "databaseHelper.h"
#include <exception>
#include <string>
#include <mutex>
static std::mutex mtx;


void databaseHelper::getAccountByName(std::string_view &name,Account& account) {
    std::cout << "get account by name\n";

    // 如果存在account则取得该account的信息
    if(checkIfAccountExists(name)) {
        getAccountInfo(account, name);
        std::cout << "has account\n";
//        return account;
    }

//    return account;
}

void databaseHelper::getAccountInfo(Account& account, std::string_view& name) {
    std::cout << "get account info\n";
    auto dbManager =  databaseManager::getInst();

    auto db = dbManager->createClient();
    auto coll = dbManager->threadfunc(*db,"account");

    auto findRes = coll.find(make_document(kvp("username",name.data())));
    for(auto &&doc : findRes) {
        account.setId(doc.find("_id")->get_value().get_string().value.data());
        account.setSessionKey(doc.find("sessionKey")->get_value().get_string().value.data());
        account.setUsername(doc.find("username")->get_value().get_string().value.data());
        account.setreservedPlayerId(doc.find("reservedPlayerId")->get_value().get_int32().value);
        account.setToken(doc.find("token")->get_value().get_string().value.data());
    }
}

void databaseHelper::createAccountWithUid(std::string_view& name,int reservedUid,Account& account) {
    std::cout << "create account with uid\n";

    // 存在 同名

    // create account
    if(!checkIfAccountExists(name)) {
        account.setUsername(name.data());
        std::string createdUid = std::to_string(getNextId());
        account.setId(createdUid);
//        account.setToken(account.generateLoginToken().c_str());
        if(reservedUid > 0) account.setreservedPlayerId(reservedUid);
        // create
        saveAccount(account);
    }

}

bool databaseHelper::checkIfAccountExists(std::string_view& name) {
    std::cout << "check if account exists\n";
    auto dbManager =  databaseManager::getInst();

    auto db = dbManager->createClient();
    auto coll = dbManager->threadfunc(*db,"account");

//    || !coll.find_one(make_document(kvp("username",name)))
    if(!coll.find_one({}) || !coll.find_one(make_document(kvp("username",name.data())))) {
        return false;
    }

    return true;
}

void databaseHelper::saveAccount(Account& account) {
    std::cout << "save account\n";

        auto dbManager =  databaseManager::getInst();

        auto db = dbManager->createClient();
        auto coll = dbManager->threadfunc(*db,"account");
        coll.insert_one(make_document(
                kvp("_id",account.getId().data()),
                kvp("username",account.getUsername().data()),
                kvp("reservedPlayerId",account.getReservedPlayerId()),
                kvp("token",account.generateLoginToken().data()),  // 先设置个默认 token
                kvp("sessionKey",account.getSessionKey().data()),
                kvp("banEndTime",account.getBanEndTime()),
                kvp("banStartTime",account.getBanStartTime()),
                kvp("isBanned", account.isBan())
        ));
}

int databaseHelper::getNextId() {
    auto dbManager =  databaseManager::getInst();
    auto db = dbManager->createClient();
    auto coll = dbManager->threadfunc(*db,"counters");
    int count_ =  100;

    auto res = coll.find_one({});
    if(!coll.find_one({})) {
        coll.insert_one(make_document(
                kvp("_id","Account"),
                kvp("count",count_)
        ));
        return count_;
    }

    count_ = coll.find_one({})->view()["count"].get_value().get_int32().value;
    int newCount_ = count_+1;
    coll.find_one_and_update(make_document(kvp("count",count_)),
                             make_document(kvp("$set", make_document(kvp("count",newCount_)))));

    std::cout << "id: "  << newCount_ << std::endl;
    return newCount_;

}

void databaseHelper::getAccountById(std::string_view& uid, Account& account) {
    if(checkAccountById(uid)) {
        getAccountInfoById(account, uid);
    }
}

bool databaseHelper::checkAccountById(std::string_view& uid) {
    auto dbManager =  databaseManager::getInst();
    auto db = dbManager->createClient();
    auto coll = dbManager->threadfunc(*db,"account");

    if(!coll.find_one(make_document(kvp("_id",uid.data()))))
        return false;

    return true;
}

void databaseHelper::getAccountInfoById(Account &account, std::string_view& uid) {
    auto dbManager =  databaseManager::getInst();

    auto db = dbManager->createClient();
    auto coll = dbManager->threadfunc(*db,"account");

    auto findRes = coll.find(make_document(kvp("_id",uid.data())));
    for(auto &&doc : findRes) {
        std::string t = doc.find("_id")->get_value().get_string().value.data();
        std::string tSkey = doc.find("sessionKey")->get_value().get_string().value.data();
        account.setId(t);
        account.setUsername(doc.find("username")->get_value().get_string().value.data());
        account.setSessionKey(tSkey);
        account.setreservedPlayerId(doc.find("reservedPlayerId")->get_value().get_int32().value);
        account.setToken(doc.find("token")->get_value().get_string().value.data());
    }
}

