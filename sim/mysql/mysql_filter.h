#ifndef MYSQL_FILTER_H
#define MYSQL_FILTER_H
#include <sim/mysql/filter_template.h>
#include <stdint.h>
#include <string>
namespace sim {

namespace mysql {

class MysqlFilter : public SqlFilter<MysqlFilter>
{
public:
    MysqlFilter(Mysql* s=NULL):SqlFilter<MysqlFilter>(*this, s)
    {
    }
};

}//endof namespace mysql
}//endof namespace sim

#endif
