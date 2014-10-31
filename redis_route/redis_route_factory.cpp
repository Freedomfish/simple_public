#include "redis_route_factory.h"
#include "redis_route.h"

RedisRouteBase* RedisRouteFactory::NewRoute()
{
    return new RedisRoute;
}

RedisInfoManager* RedisRouteFactory::NewInfoManager()
{
    return new RedisRouteInfo;
}
