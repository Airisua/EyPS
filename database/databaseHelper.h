#ifndef EYPS_DATABASEHELPER_H
#define EYPS_DATABASEHELPER_H

#include <string_view>
#include "Account.h"
#include "databaseManager.h"
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;


class databaseHelper {
public:
    using ptr = std::shared_ptr<databaseHelper>;
    databaseHelper() = default;
    ~databaseHelper() = default;

    void getAccountByName(std::string_view& name,Account& account);
    void createAccountWithUid(std::string_view& name,int reservedUid,Account& account);
    bool checkIfAccountExists(std::string_view& name);

    void getAccountInfo(Account& account,std::string_view& name);
    void saveAccount(Account& account);
    int getNextId();

    void getAccountById(std::string_view& uid, Account& account);
    bool checkAccountById(std::string_view& uid);
    void getAccountInfoById(Account& account, std::string_view& uid);
};


#endif //EYPS_DATABASEHELPER_H
