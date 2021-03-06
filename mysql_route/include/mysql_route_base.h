#ifndef MYSQL_ROUTE_BASE_H
#define MYSQL_ROUTE_BASE_H
#include <sim/typedef.h>
#include <sim/simsql.h>
#include <sim/simini.h>
#include <sim/route_base.h>
#include <string>
namespace sim {
enum ReadWrite {
    READ = 1,
    WRITE
};
typedef SharedPtr<SimSql> SimSqlPtr;
typedef RouteObj<SimSql> MysqlObj;

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
    virtual MysqlObj get(int index=0,
        const std::string& db_type="",
        int r_w=1) = 0;
    virtual void InitData(MysqlInfoManager*)=0;
};

}

#endif
