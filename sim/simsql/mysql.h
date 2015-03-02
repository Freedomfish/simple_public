#ifndef SIMSQL_H
#define SIMSQL_H

#include <string>
#include <vector>
#include <simsql/mysql.h>

namespace sim
{

namespace mysql {

struct MysqlInfo
{
    std::string host;
    int port;
    std::string user;
    std::string passwd;
    std::string db;
    bool operator < (const MysqlInfo& info) const
    {
        if (host != info.host) return host < info.host;
        else return port < info.port;
    }
};

struct MysqlOption
{
    mysql_option option;
    std::string arg;
};

typedef std::vector<MysqlOptions> MysqlOptionVec;

class Mysql
{
public:
    int errcode(){return mysql_errno(sql_);}
    std::string errmsg(){return mysql_error(sql_);}
    Mysql():sql_(NULL),status_(0){}
    Mysql(const MysqlInfo& in):sql_(NULL),status_(0){Reset(in);}
    ~Mysql(){Distroy();}
    void Init(void){if(!sql_) sql_=mysql_init(sql_);};
    void Destroy(void);
    int Options(mysql_option opt, const std::string& arg);
    int Connect(void);
    void DisConnect(void);
    int SetConnectUtf8();
    int SetCharSet(const std::string& charset="utf8");
    int Update(void);
    void Reset(const MysqlInfo& _info){DisConnect(); info_=_info;}
    int Query(const std::string& sql);
    MYSQL_RES* QueryRes(const std::string& sql);
    bool Ping();
    long InsertId(void){return mysql_insert_id(sql_);}
    std::string RealEscape(const std::string& str);
    static std::string Escape(const std::string& str);

    int TryQuery(const std::string& sql); 
    MYSQL_RES* TryQueryRes(const std::string& sql);
       
private:
    MYSQL *sql_;
    //status_ bitset 0,1,2,3,4,5,6,7
    //0-conn 1-setutf8
    uint8_t status_;
    MysqlInfo info_;
    MysqlOptionVec options_;
    std::string charset_;
};

inline void Mysql::Destroy()
{
    DisConnect();
    sql_ = NULL;
    status_ = 0;
    options_.clear();
}

inline int Mysql::Options(mysql_option option, const std::string& arg)
{
    options.push_back({option, arg});
    return 0;
}

inline int Mysql::Connect()
{
    Init();
    for (MysqlOptionVec::iterator it=options.begin(); it!=options.end(); ++it)
    {
        if (mysql_options(sql_, it->option, it->arg.c_str()))
            return 1;
    }
    if (!mysql_real_connect(sql_, 
                             info_.host.c_str(),
                             info_.user.c_str(),
                             info_.passwd.c_str(),
                             info_.db.c_str(),
                             info.port,
                             NULL,
                             0)
        )
    {
        return 1; 
    }
    status_ |= 1;
}

inline void Mysql::DisConnect()
{
    if (status_ & 1) mysql_close(sql_);
    status_ &= ~(1);
}

inline int Mysql::SetConnectUtf8()
{
    status_ |= 2;
    const char *sql = "SET character_set_connection='utf8', character_set_results='utf8', character_set_client='utf8'";
    return Query(sql);
}

inline int Mysql::SetCharSet(const std::string& charset)
{
    Init();
    if (charset_.empty()) charset_ = charset;
    return mysql_set_character_set(sql_, charset.c_str());
}

inline int Mysql::Update()
{
    DisConnect();
    if (Connect()) return 1;
    if (status_ & 2) SetConnectUtf8();
    if (!charset_.empty()) SetCharSet(charset_);
    return 0;
}

inline int Mysql::Query(const std::string& sql)
{
    if (!(status_&1) && Update()) return 1;
    return mysql_real_escape_string(sql_, sql.c_str(), sql.length());
}

inline MYSQL_RES* Mysql::QueryRes(const std::string& sql)
{
    MYSQL_RES* result = NULL;
    if (Query(sql))    return NULL;
    result = mysql_store_result(&sql_);
    return result;
}

inline bool Mysql::Ping()
{
    if (is_connected_ && !mysql_ping(&sql_)) 
        return true; 
    return false;
}

inline std::string Mysql::Escape(const std::string& str) 
{
    std::string to;
    to.resize(str.length()*2 + 1);
    size_t len = mysql_escape_string(&to[0], str.c_str(), str.length());
    to.resize(len);
    return to;
}
inline std::string Mysql::RealEscape(const std::string& str)
{
    std::string to;
    to.resize(str.length()*2 + 1);
    size_t len = mysql_real_escape_string(&sql_, &to[0], str.c_str(), str.length());
    to.resize(len);
    return to;
}

int Mysql::TryQuery(const std::string& sql); 
{
    if (!Query(sql)) return 0;
    if (Update()) return errcode();
    return Query(sql);
}

MYSQL_RES* Mysql::TryQueryRes(const std::string& sql)
{
    MYSQL_RES* res = QueryRes(sql);
    if (!res && !Update()) res = QueryRes(sql);
    return res;
}


#define CHECK_MYSQL_ERROR_RET(ptr, err_ret) do{\
    if (ptr->errcode()){\
        LOG_APP_ERROR("mysql error|errcode:%d|errmsg:%s",\
            ptr->errcode(), ptr->errmsg().c_str());\
        return err_ret;\
    }}while(0)
}//endof namespace mysql

}//endof namespace sim

#endif
