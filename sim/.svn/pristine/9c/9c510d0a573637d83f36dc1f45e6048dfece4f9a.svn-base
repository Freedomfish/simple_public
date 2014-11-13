#include "simsql.h"
using namespace std;

namespace sim{
int SimSql::Connect(const char* host, const char *user, const char*passwd, const char*db, unsigned int port)
{
    mysql_init(&sql_);
    if (info_.host.empty())
        Reset(host, user, passwd, db, port);
    if (!mysql_real_connect(&sql_, host, user, passwd, db, port, NULL, 0))
    {
        return 1;
    }
    is_connected_ = true;
    const char *sql = "SET character_set_connection='utf8', character_set_results='utf8', character_set_client='utf8'";
    return Query(sql);
}

int SimSql::Connect(void)
{
    if (info_.host.empty()) return 1;
    return Connect(info_.host.c_str(), info_.user.c_str(), info_.passwd.c_str(), info_.db.c_str(), info_.port);
}
void SimSql::DisConnect(void)
{
    if (is_connected_) mysql_close(&sql_);
    is_connected_ = false;
}
int SimSql::Update()
{
    DisConnect();
    return Connect();
}
void SimSql::Reset(const char* host, const char *user, const char*passwd, const char*db, unsigned int port)
{
    info_.host = host;
    info_.user = user;
    info_.passwd = passwd;
    info_.db = db;
    info_.port = port;
}
void SimSql::Reset(const MysqlInfo& _info)
{
    info_.host = _info.host;
    info_.user = _info.user;
    info_.passwd = _info.passwd;
    info_.db = _info.db;
    info_.port = _info.port;
    DisConnect();
}

int SimSql::Query(const std::string& sql)
{
    if (!is_connected_ && Update()) return 1;
//    if (errcode_) return errcode_;
    return mysql_real_query(&sql_, sql.c_str(), sql.length());
}

MYSQL_RES* SimSql::QueryRes(const std::string& sql)
{
    MYSQL_RES* result = NULL;
    if (Query(sql))    return NULL;
    result = mysql_store_result(&sql_);
    return result;
}

}//end of namespace sim
