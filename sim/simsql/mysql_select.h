#ifndef MYSQL_SELECTOR_H
#define MYSQL_SELECTOR_H

namespace sim {
namespace mysql {

class MysqlSelect : public SqlFilter<MysqlSelect>
{
public:
    MysqlSelect(Mysql* s=NULL):SqlFilter<MysqlSelect>(*this, s),sql_(s)
    {
    } 
    MysqlRes Query();
    MysqlSelect& Select(const std::string& s);
    MysqlSelect& Table(const std::string& table){table_=table; return *this;}
    MysqlSelect& Use(const std::string& db){sql_->Query("use " + db); return *this;}
    MysqlSelect& operator (const std::string& s){return Select(s);}
    void Clear();
private:
    Mysql* sql_;
    std::string selector_;
    std::string table_;
};

inline MysqlRes MysqlSelect::Query()
{
    std::string s = "select " + selector_ + " from " + table_ + ToString();
    return sql_->QueryRes(s);
}

inline MysqlSelect& Select(const std::string& s)
{
    selector_ = s;
    return *this;
}

inline void MysqlSelect::Clear()
{
    selector_.clear();
}

}
}

#endif
