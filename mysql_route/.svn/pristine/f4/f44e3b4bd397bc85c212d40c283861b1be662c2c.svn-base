#ifndef MYSQL_COMMON_STR_H
#define MYSQL_COMMON_STR_H
#include <string>
#include "simsql.h"
#include "string_common.h"

class SqlFilterStr
{
public:
    SqlFilterStr()flag_(false){}
    void add_str(const std::string& name, const std::string& value)
    {
        if (flag_) filter_str_ += " and ";
        filter_str_ += " " + name + "=" + sim::SimSql::Escape(value) + "'";
    }
    template<typename T>
    void add_num(const std::string& name, const T value)
    {
        if (flag_) filter_str_ += " and ";
        filter_str_ += " " + name + "=" + sim::atostr(value);
    }
    const std::string& ToStr(){return filter_str_;}
private:
    std::string filter_str_;
    bool flag_;
};

class SqlInsertStr
{
public:
    SqlInsertStr()
    {
        sqlhead_ = "insert into ";
        sqlbegin_ = "(";
        sqlend_ = ")values(";
    }
    void set_table(const std::string& tablename)
    {
       sqlhead_ += tablename;
    }
    void add_str(const std::string& name, const std::string& value)
    {
        sqlbegin_ += name + ",";
        sqlend_ += "'" + sim::SimSql::Escape(value) + "',";
    }
    template<typename T>
    void add_num(const std::string& name, const T value)
    {
        sqlbegin_ += name + ",";
        sqlend_ += atostr(value) + ",";
    }
    const std::string& ToStr(void)
    {
        if (sql_.empty())
        {
            sqlbegin_.erase(sqlbegin_.length()-1);
            sqlend_.erase(sqlend_.length()-1);
            sql_ = sqlhead_ + sqlbegin_ + sqlend_ + ")";
        }
        return sql_;
    }
private:
    std::string sql_;
    std::string sqlhead_;
    std::string sqlbegin_;
    std::string sqlend_;
};

class SqlUpdateStr
{
public:
    SqlUpdateStr()
    {
        sqlhead_ = "update ";
        sqlbegin_ = " set ";
        sqlend_ = " where ";
    }
    const std::string& ToStr(void)
    {
        if (sql_.empty())
        {
            sqlbegin_.erase(sqlbegin_.length()-1);
            sql_ = sqlhead_ + sqlbegin_+ sqlend_;
        }
        return sql_;
    }
    void set_table(const std::string& tablename)
    {
        sqlhead_ += tablename;
    }
    void add_str(const std::string& name, const std::string& value)
    {
        sqlbegin_ += name + "='" + sim::SimSql::Escape(value) + "',"; 
    }
    template<typename T>
    void add_num(const std::string& name, const T value)
    {
        sqlbegin_ += name + "=" + atostr(value) + ",";
    }
    void set_filter(const std::string& str)
    {
        sqlend_ += str; 
    }

private:
    std::string sql_;
    std::string sqlhead_;
    std::string sqlbegin_;
    std::string sqlend_;
};


#endif
