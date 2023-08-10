#include "Account.h"
#include "../util/Crypto.h"
#include "databaseManager.h"
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>

void Account::setId(std::string_view id_) {
    this->id = id_;
}

void Account::setUsername(std::string_view name) {
    this->username = name;
}

void Account::setPwd(std::string_view pwd) {
    this->password = pwd;
}

void Account::setreservedPlayerId(int playId) {
    this->reservedPlayerId = playId;
}

void Account::setToken(std::string_view token_) {
    this->token = token_;
}


void Account::setSessionKey(std::string_view SessionKey_) {
    this->sessionKey = SessionKey_;
}

void Account::setBanStartTime(int banStartTime_) {
    this->banStartTime = banStartTime_;
}

void Account::setBanEndTime(int banEndTime_) {
    this->banEndTime = banEndTime_;
}

void Account::setBan(bool isBan) {
    this->isBanned = isBan;
}

bool Account::isBan() {
    return false;
}



std::string_view Account::getId() {
    return id;
}

std::string_view Account::getUsername() {
    return username;
}

std::string_view Account::getPwd() {
    return password;
}

int Account::getReservedPlayerId() const {
    return reservedPlayerId;
}

std::string_view Account::getToken() {
    return token;
}

std::string_view Account::getSessionKey() {
    return sessionKey;
}

int Account::getBanStartTime() {
    return banStartTime;
}

int Account::getBanEndTime() {
    return banEndTime;
}

// sessionKey
std::string_view Account::generateSessionKey() {
    // 每次响应的时候更新 token
    auto oldSessionKey = this->sessionKey;
    std::cout << "oldSessionKey: " << oldSessionKey << std::endl;  //
    this->sessionKey = createToken();
    auto dbManager =  databaseManager::getInst();

    auto db = dbManager->createClient();
    auto coll = dbManager->threadfunc(*db,"account");
//    mongocxx::client client{mongocxx::uri{}};
//    auto coll = client["EyPS"]["account"];

    coll.find_one_and_update(make_document(kvp("sessionKey",oldSessionKey)),
                             make_document(kvp("$set", make_document(kvp("sessionKey", this->sessionKey)))));

    std::cout << "new seesion key" << this->sessionKey << '\n';
    return sessionKey;
}

// token
std::string_view Account::generateLoginToken() {
    auto oldToken = this->token;
    std::cout << "old token: " << oldToken << std::endl;  //
    this->token = createToken().c_str();
    auto dbManager =  databaseManager::getInst();

    auto db = dbManager->createClient();
    auto coll = dbManager->threadfunc(*db,"account");

//    mongocxx::client client{mongocxx::uri{}};
//    auto coll = client["EyPS"]["account"];

    coll.find_one_and_update(make_document(kvp("token",oldToken)),
                             make_document(kvp("$set", make_document(kvp("token", this->token)))));

    std::cout << "new token" << this->token << '\n';
    return this->token;
}


