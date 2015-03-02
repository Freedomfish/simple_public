#ifndef QUERY_SECTER_H
#define QUERY_SECTER_H
#include <simsql/mysql_select.h>
#include <simsql/mysql_insert.h>
#include <simsql/mysql_update.h>

namespace sim
{
namespace mysql
{

class QuerySecter : public Mysql
{
public:
    QuerySecter():select_(this),insert_(this),update_(this){}
    MysqlSelect& Select(){return select_;}
    MysqlInsert& Insert(){return insert_;}
    MysqlUpdate& Update(){return update_;}
private:
    MysqlSelecter select_;
    MysqlInserter insert_;
    MysqlUpdateer update_;
};

}//endof namespace mysql
}//endof namespace sim

#endif
