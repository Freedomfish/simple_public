#ifndef REDIS_ROUTE_COMMON_H
#define REDIS_ROUTE_COMMON_H

#include "redis_route_base_factory.h"

#include <string>
namespace sim {
template<class T>

class RedisRouteCommon
{
public:
    RedisRouteCommon():route_(NULL){}
    static void Init(
        RedisRouteBaseFactory *f
        )
    {
        factory_.reset(f);
        info_manager_.reset(f->NewInfoManager());
    }
    static void InitData(SimIni& ini, 
        const std::vector<std::string>& db_types)
    {
        info_manager_->Init(ini, db_types);
    }
    static void InitData(SimSql& sql, 
        const std::vector<std::string>& db_types)
    {
        info_manager_->Init(sql, db_types);
    }

    void Start()
    {
        if (factory_ && !route_) 
        {
            route_.reset(factory_->NewRoute());
        }
        route_->InitData(info_manager_.get());
    }
    SharedPtr<RedisRouteBase> route()
    {
        Start();
        return route_;
    }
protected:
    SharedPtr<RedisRouteBase> route_;
    static SharedPtr<RedisRouteBaseFactory> factory_;
    static SharedPtr<RedisInfoManager> info_manager_;
};

template<class T>
SharedPtr<RedisRouteBaseFactory> RedisRouteCommon<T>::factory_= NULL;
template<class T>
SharedPtr<RedisInfoManager> RedisRouteCommon<T>::info_manager_ = NULL;

}
#endif

