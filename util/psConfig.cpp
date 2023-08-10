#include "psConfig.h"
#include <iostream>
#include "log.h"

void Config::loadKeys() {
    DispatchKey_ = ReadFile_("../resources/keys/dispatchKey.bin");
    DispatchSeed_ = ReadFile_("../resources/keys/dispatchSeed.bin");
    SecretKeyBuffer_ = ReadFile_("../resources/keys/secretKeyBuffer.bin");
    LOG_INFO("loadKey...");
}

std::span<uint8_t> Config::getDispatchKey_() {
    return DispatchKey_;
}

std::span<uint8_t> Config::getDispatchSeed_() {
    return DispatchSeed_;
}

std::span<uint8_t> Config::getSecretKeyBuffer_() {
    return SecretKeyBuffer_;
}

void Config::loadFile(const char *file) {
    int ret = luaL_dofile(L, file);
    if (ret)
        printf("luaL_dofile error!\n");
}

void Config::getStringVal(const char *key, std::string &val) {
    lua_getglobal(L, key);
    if(!lua_isstring(L,-1)) return;
    val = lua_tostring(L, -1);
}

void Config::getIntVal(const char *key, int &val) {
    lua_getglobal(L, key);
    if(!lua_isinteger(L, -1)) return;
    val = lua_tointeger(L, -1);
}

void Config::getBooleanVal(const char *key, bool &val) {
    lua_getglobal(L, key);
    if(!lua_isboolean(L, -1)) return;
    val = lua_toboolean(L, -1);
}

void Config::getValFromTable(const char *table, std::string &val, std::initializer_list<const char *> keys) {
    lua_getglobal(L, table);
    for(auto &key : keys)
        lua_getfield(L,-1,key);
    if(!lua_isstring(L,-1)) return;
    val = lua_tostring(L, -1);
    lua_pop(L,keys.size());
}

void Config::getValFromTable(const char *table, int &val, std::initializer_list<const char *> keys) {
    lua_getglobal(L, table);
    for(auto &key : keys)
        lua_getfield(L,-1,key);
    if(!lua_isinteger(L, -1)) return ;
    val = lua_tointeger(L, -1);
    lua_pop(L,keys.size());
}

void Config::getValFromTable(const char *table, bool &val, std::initializer_list<const char *> keys) {
    lua_getglobal(L, table);
    for(auto &key : keys)
        lua_getfield(L,-1,key);
    if(!lua_isboolean(L, -1)) return;
    val = lua_toboolean(L, -1);
    lua_pop(L, keys.size());
}

void Config::getValFromTable(const char *table, size_t &val, std::initializer_list<const char *> keys) {
    lua_getglobal(L, table);
    for(auto &key : keys)
        lua_getfield(L,-1,key);
    if(!lua_isinteger(L, -1)) return ;
    val = (size_t)lua_tointeger(L,-1);
    lua_pop(L,keys.size());
}

void Config::getValFromTable(const char *table, unsigned short &val, std::initializer_list<const char *> keys) {
    lua_getglobal(L, table);
    for(auto &key : keys)
        lua_getfield(L,-1,key);
    if(!lua_isinteger(L, -1)) return ;
    val = (unsigned short )lua_tointeger(L,-1);
    lua_pop(L,keys.size());
}

void Config::initCig() {
    L = luaL_newstate();
    loadFile("../psconfig.lua");
    getValFromTable("server",dispatchAddr,{"dispatch","bindAddress"});
    getValFromTable("server",dispatchPort,{"dispatch","bindPort"});
    getValFromTable("server",gameAddr,{"game","bindAddress"});
    getValFromTable("server",gamePort,{"game","bindPort"});
    getValFromTable("databaseInfo",connectionUri,{"connectionUri"});
    getValFromTable("databaseInfo",collection,{"collection"});
    getValFromTable("RegionInfo",regionName,{"name"});
    getValFromTable("RegionInfo",regionTitle,{"title"});
    getValFromTable("RegionInfo",regionIp,{"ip"});
}

Config::~Config() {
    lua_close(L);
}

std::string Config::getDispatchAddr(){
    return dispatchAddr;
}

std::string Config::getGameAddr() {
    return gameAddr;
}

unsigned short Config::getDispatchPort(){
    return dispatchPort;
}

unsigned short Config::getGamePort(){
    return gamePort;
}













