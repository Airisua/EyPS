#ifndef EYPS_PSCONFIG_H
#define EYPS_PSCONFIG_H
#include <span>
#include <string>
#include <initializer_list>
#include "lua.hpp"
#include "Crypto.h"
#include "Singleton.h"


class Config : public Singleton<Config>{
    friend class Singleton<Config>;
    Config() = default;
public:
    ~Config() override;


public:
    void loadKeys();
    std::span<uint8_t> getDispatchKey_();
    std::span<uint8_t> getDispatchSeed_();
    std::span<uint8_t> getSecretKeyBuffer_ ();
    void loadFile(const char * file);
    void getStringVal(const char* key, std::string& val);   // 获取全局
    void getIntVal(const char* key, int& val);
    void getBooleanVal(const char* key, bool& val);

    void getValFromTable(const char* table,std::string& val,std::initializer_list<const char*> keys);  // table
    void getValFromTable(const char* table,int& val, std::initializer_list<const char*> keys);
    void getValFromTable(const char* table, bool& val,std::initializer_list<const char*> keys);
    void getValFromTable(const char* table, size_t& val,std::initializer_list<const char*> keys);
    void getValFromTable(const char* table, unsigned short& val,std::initializer_list<const char*> keys);

    void initCig();
     std::string getDispatchAddr() ;
     std::string getGameAddr() ;
     unsigned short getDispatchPort() ;
     unsigned short getGamePort();

private:
    std::span<uint8_t> DispatchKey_;
    std::span<uint8_t> DispatchSeed_;
    std::span<uint8_t> SecretKeyBuffer_;
    lua_State *L{};

public:
    [[maybe_unused]] std::string connectionUri;
    [[maybe_unused]] std::string collection;
    std::string dispatchAddr;
    std::string gameAddr;
    unsigned short dispatchPort{};
    unsigned short gamePort{};

    std::string regionName;
    std::string regionTitle;
    std::string regionIp;
};

#endif //EYPS_PSCONFIG_H
