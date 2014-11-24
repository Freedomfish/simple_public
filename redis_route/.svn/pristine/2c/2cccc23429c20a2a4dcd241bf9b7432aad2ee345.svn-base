#ifndef SIMREDIS_ROUTE_H
#define SIMREDIS_ROUTE_H
#include <sim/simexception.h>
#include <sim/simini.h>
#include <sim/simpool.h>
#include <sim/route_base.h>
#include "redis_route_base.h"
#include <set>
#include <map>
namespace sim {

typedef std::set<RedisInfo> RedisInfoSet;
//index-info
typedef std::map<int, RedisInfoSet> RedisInfoSetMap;

struct RedisInfoUtil
{
    //type-infos
    std::map<std::string, RedisInfoSetMap> type_infos; 
    //all infos
    RedisInfoSet infos;
};

class RedisRouteInfo : public RedisInfoManager
{
public:
    void Init(SimIni& ini, 
        const std::vector<std::string>& types);
    void Init(SimSql& sql, 
        const std::vector<std::string>& types);
    void *Infos();
private:
    RedisInfoUtil info_util_;
};

typedef RouteBase<int,SimRedis> RedisBase;

class RedisRoute : public RedisRouteBase
{
public:
    RedisRoute():is_started_(false){}
    virtual ~RedisRoute(){}
    virtual RedisObj get(const int index=0, 
        const std::string& key="");
    void InitData(RedisInfoManager* infos);
private:
    void Start(RedisInfoManager*);
private:
    bool is_started_;
    std::map<std::string, SharedPtr<RedisBase> > routes_;//0-r 1-w
};

}

#endif
