#ifndef REIDS_ROUTE_BASE_H 
#define REIDS_ROUTE_BASE_H 

#include <sim/simredis.h>
#include <sim/simsql.h>
#include <sim/typedef.h>
#include <sim/simini.h>
#include <sim/log4cplus_common.h>
#include <sim/route_base.h>

using namespace sim;

#define CHECK_REDIS(sr, r, ret) do {\
    if (sr->errcode()){\
        LOG_APP_ERROR("redis err|errcode=%d|errmsg=%s",\
            sr->errcode(), sr->errmsg().c_str());\
    }else if (!r){\
        LOG_APP_ERROR("redis err|reply is nil");\
    }else{\
        LOG_APP_ERROR("redis err|r->integer=%lld|r->str=%s",\
            r->integer, r->str);\
    }\
    return ret;\
}while(0)

typedef RouteObj<SimRedis> RedisObj;

class RedisInfoManager
{
public:
    virtual ~RedisInfoManager(){}
    virtual void Init(SimIni& ini, 
        const std::vector<std::string>& types){}
    virtual void Init(SimSql& sql, 
        const std::vector<std::string>& types){}
    virtual void *Infos() {return NULL;}
};

class RedisRouteBase
{
public:
    virtual ~RedisRouteBase(){}
    virtual RedisObj get(int index = 0, 
        const std::string& type_s=""){return RedisObj();}

    virtual void InitData(RedisInfoManager*){}

};

#endif
