#include "mysql_route_factory.h"
#include "mysql_route.h"
using namespace std;
using namespace sim;
namespace sim {
MysqlRouteBase* MysqlRouteFactory::NewRoute()
{
    return new MysqlRoute;
}

MysqlInfoManager* MysqlRouteFactory::NewInfoManager()
{
    return new MysqlRouteInfo;
}

}
