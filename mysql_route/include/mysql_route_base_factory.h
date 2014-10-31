#ifndef MYSQL_ROUTE_BASE_FACTORY_H
#define MYSQL_ROUTE_BASE_FACTORY_H

#include "mysql_route_base.h"
namespace sim {
class MysqlRouteBaseFactory
{
public:
    virtual ~MysqlRouteBaseFactory(){}
    virtual MysqlRouteBase* NewRoute() = 0;
    virtual MysqlInfoManager* NewInfoManager(){return NULL;}
};
}
#endif
