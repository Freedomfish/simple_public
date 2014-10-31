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
        errcode_ = mysql_errno(&sql_);
        errmsg_ = mysql_error(&sql_);
        return 1;
    }
    is_connected_ = true;
    errcode_ = 0;
    errmsg_.clear();
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
    errcode_ = 0;
    errmsg_="not connect";
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
    errcode_ = mysql_real_query(&sql_, sql.c_str(), sql.length());
    if (errcode_) errmsg_ = mysql_error(&sql_);
    return errcode_;
}

MYSQL_RES* SimSql::QueryRes(const std::string& sql)
{
    MYSQL_RES* result = NULL;
    if (Query(sql))    return NULL;
    result = mysql_store_result(&sql_);
    if (!result)
    {
        errcode_ = mysql_errno(&sql_);
        errmsg_ = mysql_error(&sql_);
        return NULL;
    }
    return result;
}

}//end of namespace sim
