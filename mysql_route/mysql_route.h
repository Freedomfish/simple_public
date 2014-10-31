#ifndef SIMMYSQL_ROUTE_H
#define SIMMYSQL_ROUTE_H
#include <sim/simexception.h>
#include <sim/simini.h>
#include <sim/simpool.h>
#include <sim/route_base.h>
#include "mysql_route_base.h"
#include <set>
#include <map>
namespace sim {

typedef std::set<MysqlInfo> MysqlInfoSet;
//index-info
typedef std::map<int, MysqlInfoSet> MysqlInfoSetMap;

struct MysqlInfoUtil
{
    //type-infos
    std::map<std::string, MysqlInfoSetMap[2]> type_infos; 
    //all infos
    MysqlInfoSet infos;
};

class MysqlRouteInfo : public MysqlInfoManager
{
public:
    void Init(SimIni& ini, 
        const std::vector<std::string>& db_types);
    void Init(SimSql& sql, 
        const std::vector<std::string>& db_types);
    void *Infos();
private:
    MysqlInfoUtil info_util_;
};

typedef RouteBase<int,SimSql> MysqlBase;

class MysqlRoute : public MysqlRouteBase
{
public:
    MysqlRoute():is_started_(false){}
    virtual ~MysqlRoute(){}
    virtual MysqlObj get(const int index=0, 
        const std::string& db_key="",
        const int r_w=1);
    void InitData(MysqlInfoManager* infos);
private:
    void Start(MysqlInfoManager*);
private:
    bool is_started_;
    std::map<std::string, SharedPtr<MysqlBase> > routes_[2];//0-r 1-w
};

}

#endif
