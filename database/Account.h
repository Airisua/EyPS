#ifndef EYPS_ACCOUNT_H
#define EYPS_ACCOUNT_H
#include <iostream>
#include <string>
#include <memory>
#include <string_view>

class Account {
public:
    using ptr = std::shared_ptr<Account>;
    Account() = default;
    ~Account()  = default;

private:
    std::string id{};
    std::string sessionKey{};
    std::string username{};
    std::string password{};
    std::string token{};
    int reservedPlayerId {};
    int banStartTime {};
    int banEndTime {};
    bool isBanned = false;

public:
    void setId(std::string_view id_);
    void setUsername(std::string_view name);
    void setPwd(std::string_view pwd);
    void setreservedPlayerId(int playId);
    void setToken(std::string_view token_);
    void setSessionKey(std::string_view SessionKey_);
    void setBanStartTime(int banStartTime_);
    void setBanEndTime(int banEndTime_);
    void setBan(bool isBan);
    bool isBan();


    std::string_view getId();
    std::string_view getUsername();
    std::string_view getPwd();
    int getReservedPlayerId() const;
    std::string_view getToken();
    std::string_view getSessionKey();
    std::string_view generateSessionKey(); //
    std::string_view generateLoginToken();
    int getBanStartTime();
    int getBanEndTime();
};


#endif //EYPS_ACCOUNT_H
