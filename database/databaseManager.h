#ifndef EYPS_DATABASEMANAGER_H
#define EYPS_DATABASEMANAGER_H
#include "../util/Singleton.h"
#include "../util/psConfig.h"
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/pool.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;


class databaseManager : public Singleton<databaseManager>{
    databaseManager() = default;
    friend class Singleton<databaseManager>;
public:
    ~databaseManager() override = default;
    void initDataBase();

    mongocxx::pool::entry createClient();
    mongocxx::collection threadfunc(mongocxx::client& client,const char * collName);

    std::string connectionUri_;
    std::string collection_;

public:

};


#endif //EYPS_DATABASEMANAGER_H
