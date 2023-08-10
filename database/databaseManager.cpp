#include "databaseManager.h"


void databaseManager::initDataBase() {
    static mongocxx::instance instance{};
    auto Config = Config::getInst();
    this->connectionUri_ = Config->connectionUri;
    this->collection_ = Config->collection;

    std::cout << "init database\n";
}

mongocxx::pool::entry databaseManager::createClient() {
    static mongocxx::uri uri_(this->connectionUri_);
    static mongocxx::pool pool{uri_};
    auto entry = pool.acquire();
    return entry;
}


mongocxx::collection databaseManager::threadfunc(mongocxx::client &client, const char *collName) {

    auto coll = client[this->collection_][collName];
    return std::move(coll);
}

