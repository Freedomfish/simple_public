#ifndef MYSQL_ESCAPE_H
#define MYSQL_ESCAPE_H
#include <simsql/mysql.h>

namespace sim {

namespace mysql {

class Escape
{
public:
    Escape(Mysql* s=NULL):sql_(s){}
    std::string operator() (const std::string& str)
    {
        if (sql_) return s->RealEscape(str);
        else return Mysql::Escape(str);
    }
private:
    Mysql* sql_;
};

}

}

#endif
