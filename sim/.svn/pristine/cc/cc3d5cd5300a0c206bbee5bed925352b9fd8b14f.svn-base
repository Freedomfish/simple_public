#ifndef SIMSQL_H
#define SIMSQL_H

#include <mysql/mysql.h>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <sim/string_common.h>

namespace sim
{
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

class SimSqlRes
{
friend class SimSql;
public:
    SimSqlRes(MYSQL_RES* r=NULL):res_(r){}
    ~SimSqlRes(){Free();}
    void swap(SimSqlRes& r){std::swap(res_, r.res_);}
    SimSqlRes* ptr(void){return this;}
    operator const bool () const {return res_!=NULL;}
    int RowNum(void)
    { 
        if (!res_) return 0;
        return mysql_num_rows(res_);
    }
    unsigned long* FetchLengths(void)
    {
        if (!res_) return 0;
        return mysql_fetch_lengths(res_);
    }
    int FieldNum(void)
    {
        if (!res_) return 0;
        return mysql_num_fields(res_);
    }
    char *FieldName(int i)
    {
        if (!res_) return NULL;
        return mysql_fetch_field_direct(res_, i)->name;
    }
    MYSQL_ROW FetchRow(void)
    {
        if (!res_) return NULL;
        return mysql_fetch_row(res_);
    }
private:
    //SimSqlRes(SimSqlRes& simres):res_(simres.res_), m_is_valid(true){simres.m_is_valid = false;}
    void Free(void){if (res_) mysql_free_result(res_);res_=NULL;}
    void Reset(MYSQL_RES* res){Free(); res_ = res;};
    SimSqlRes& operator=(const SimSqlRes& simres){return *this;}
private:
    MYSQL_RES* res_;
};

class SimSql
{
public:
    int errcode(){return mysql_errno(&sql_);}
    std::string errmsg(){return mysql_error(&sql_);}
    SimSql():is_connected_(false){}
    SimSql(const MysqlInfo& in):is_connected_(false){
        Reset(in);
    }
    ~SimSql(){DisConnect();}
    bool Ping(){
        if (is_connected_ && !mysql_ping(&sql_)) 
            return true; return false;
    }
    
    int Connect(const char* host, const char *user, const char*passwd, const char*db, unsigned int port);
    int Connect(void);
    void DisConnect(void);
    int Update(void);
    void Reset(const char* host, const char* user, const char* passwd, const char*db, unsigned int port);
    void Reset(const MysqlInfo& _info);
    int Query(const std::string& sql);
    MYSQL_RES* QueryRes(const std::string& sql);

    long LastInsertId(void)
    {
        return mysql_insert_id(&sql_);
    }

    int TryQuery(const std::string& sql) 
    {
        if (!Query(sql)) return 0;
        if (Update()) return errcode();
        return Query(sql);
    }

    MYSQL_RES* TryQueryRes(const std::string& sql)
    {
        MYSQL_RES* res = QueryRes(sql);
        if (!res && !Update()) res = QueryRes(sql);
        return res;
    }
    //针对网络
    int TryNetQuery(const std::string& sql)
    {
        return TryQuery(sql);
        int ret = Query(sql);
        int err = errcode();
        if (1 == err || (err>= 1158 && err <=1161))
        {
            if (!Update()) return Query(sql);
        }

        return ret;

    }
    MYSQL_RES* TryNetQueryRes(const std::string& sql)
    {
        return TryQueryRes(sql);
        MYSQL_RES* res = NULL;
        res = QueryRes(sql);
        int err = errcode();
        if (1 == err|| (err>= 1158 && err<=1161))
        {
            if (!Update())
                res = QueryRes(sql);
        }
        return res;
    }
    
    bool is_connected(){return is_connected_;}

    static inline void FreeRes(MYSQL_RES* res) 
    {
        if (res) mysql_free_result(res);
    }
    static inline const std::string Escape(const std::string& str) 
    {
        static const int tolen=1024*1024;
        char to[tolen]={0};
        size_t len = mysql_escape_string(to, str.c_str(), str.length());
        return std::string(to, len);
    }
    inline const std::string RealEscape(const std::string& str)
    {
        if (!is_connected_ && Update()) return "";
        static const int tolen=1024*1024;
        char to[tolen]={0};
        size_t len = mysql_real_escape_string(&sql_, to, str.c_str(), str.length());
        return std::string(to, len);
    }
private:
    bool is_connected_;
    MYSQL sql_;
    MysqlInfo info_;
    
};

#define CHECK_MYSQL_ERROR_RET(ptr, err_ret) do{\
    if (ptr->errcode()){\
        LOG_APP_ERROR("mysql error|errcode:%d|errmsg:%s",\
            ptr->errcode(), ptr->errmsg().c_str());\
        return err_ret;\
    }}while(0)

};

#endif
