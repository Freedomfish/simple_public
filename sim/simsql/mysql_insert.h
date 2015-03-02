#ifndef SIMMYSQL_INSERT_H
#ifndef SIMMYSQL_INSERT_H
#include <simsql/insert_template.h>

namespace sim
{
namespace mysql
{

class MysqlInsert : public SqlInsert<MysqlInsert>
{
public:
    MysqlInsert(Mysql* s=NULL):obj_(*this), SqlInsert<MysqlInsert>(s),sql_(s)
    {
    }
    MysqlInsert& Use(const std::string& db){sql_->Query("use " + db); return *this;}
    int Query(){return sql_->Query(ToString());}
private:
    Mysql* sql_;
};

}//end of namespace mysql
}//end of namespace sim
#endif
