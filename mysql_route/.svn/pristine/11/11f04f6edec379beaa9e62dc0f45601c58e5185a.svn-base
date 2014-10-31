#ifndef MYSQL_ROUTE_BASE_H
#define MYSQL_ROUTE_BASE_H
#include <sim/typedef.h>
#include <sim/simsql.h>
#include <sim/simini.h>
#include <sim/route_base.h>
#include <string>
namespace sim {
//map<db_type, map<index, MysqlInfoVecMap[r_w]>

typedef SharedPtr<SimSql> SimSqlPtr;
typedef RouteObj<SimSql> MysqlObj;
//typedef SharedPtr<MysqlObj> MysqlObjPtr;

class MysqlInfoManager
{
public:
    virtual ~MysqlInfoManager(){}
    virtual void Init(SimIni& ini, 
        const std::vector<std::string>& db_types){};
    virtual void Init(SimSql& sql, 
        const std::vector<std::string>& db_types){};
    virtual void *Infos() =0;
};

class MysqlRouteBase
{
public:
    virtual ~MysqlRouteBase(){}
    virtual MysqlObj get(const int index=0,
        const std::string& db_type="",
        const int r_w=1) = 0;
    virtual void InitData(MysqlInfoManager*)=0;
};

}

#endif
