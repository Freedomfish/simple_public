#include "redis_route.h"
#include <iostream>
#include <sim/simroute.h>
using namespace std;
using namespace sim;

static void LoadDataFromIni(
                SimIni& ini, 
                const std::vector<std::string>& types, 
                RedisInfoUtil& info_util);
static void LoadDataFromDb(
                SimSql& sql,
                const std::vector<std::string>& types,
                RedisInfoUtil& info_util);

namespace sim {

void RedisRouteInfo::Init(SimIni& ini,
    const std::vector<std::string>& types)
{
    LoadDataFromIni(ini, types, info_util_);
}

void RedisRouteInfo::Init(SimSql& sql,
    const std::vector<std::string>& types)
{
    LoadDataFromDb(sql, types, info_util_);
}

void *RedisRouteInfo::Infos()
{
    return (void*)&info_util_;
}

void RedisRoute::InitData(RedisInfoManager* infos)
{
    if (!is_started_)
    {
         Start(infos);
    }
}

RedisObj RedisRoute::get(const int index,
    const std::string& key)
{
    map<string, SharedPtr<RedisBase> >::iterator it;
    it = routes_.find(key);
    if (it != routes_.end()) return it->second->get(index);
    return RedisObj();
}

void RedisRoute::Start(RedisInfoManager *pinfo)
{
    RedisInfoUtil* infos = (RedisInfoUtil*)pinfo->Infos();
    RedisInfoSet::iterator sit;
    std::map<RedisInfo, SharedPtr<AbstractObjMaster<SimRedis>>> master_map;
    //根据info生成对象
    for (sit=infos->infos.begin(); sit!=infos->infos.end(); ++sit)
    {
        SharedPtr<AbstractObjMaster<SimRedis>> os;
        if (!is_pool_) os.reset(new SimConnObjMaster<SimRedis>);
        else os.reset(new SimConnPoolObjMaster<SimRedis>);
        master_map[*sit] = os;
        for (int i=0; i<copy_num_; ++i)
            os->AddObj(SharedPtr<SimRedis>(new SimRedis(*sit)));
    }
    //根据type和index建立对象关联
    map<string, RedisInfoSetMap>::iterator mit;
    for (mit=infos->type_infos.begin(); mit!=infos->type_infos.end(); ++mit)
    {
        SharedPtr<RedisBase> ptr(new SimRoute<int, SimRedis>);
        routes_[mit->first] = ptr;
        RedisInfoSetMap::iterator it;
        for (it=mit->second.begin(); it!=mit->second.end(); ++it)
        {
            for (sit=it->second.begin(); sit!=it->second.end(); ++sit)
             {
                ptr->AddMaster(it->first, master_map[*sit]);
             }
        }
    }
}

}

static void LoadDataFromIni(
                SimIni& ini, 
                const std::vector<std::string>& types, 
                RedisInfoUtil& info_util)
{
    vector<string>::const_iterator cit;
    for (cit=types.begin(); cit!=types.end(); ++cit)
    {
        string sess = "redis_route_" + *cit;
        map<string, string> kvm = ini.GetSession(sess);
        if (kvm.empty()) 
            throw SimException(
                "read config failed|redis_route not found|sess="+sess);

        map<string, string>::iterator it;
        vector<string> k;
        vector<string> vs;
        vector<string> v;
        vector<string>::iterator vit;
        //int maxdbnum = ini.GetIntValWithDefault(sess, "maxdbnum", 1);
        //starti-endi=ip|port|username|password|dbname|r_w
        for (it=kvm.begin(); it!=kvm.end(); ++it)
        {
            k.clear();
            vs.clear();
            v.clear();
            split(it->first, '-', k);
            if (2 != k.size()) continue;
            split(it->second, ';', vs);
            int b = atoi(k[0].c_str());
            int e = atoi(k[1].c_str());
            for (vit=vs.begin(); vit!=vs.end(); ++vit)
            {
                split(*vit, '|', v);
                RedisInfo info;
                info.host = v[0];
                info.port = atoi(v[1].c_str());
                info.time_out= {1, 1000*atoi(v[2].c_str())};
                info_util.infos.insert(info);
                for (int i=b; i<=e; ++i)
                {
                    info_util.type_infos[*cit][i].insert(info);
                }
            }
        }
    }
}

static void LoadDataFromDb(
    SimSql& sql, 
    const std::vector<std::string>& types,
    RedisInfoUtil& info_util)
{
    if (types.size() == 0) 
        throw SimException("no db_type");

    string query = "select host, port, time_out "
        " types, start_index, end_index from route.redis_route ";
    query += " where types in (";
    vector<string>::const_iterator it;
    for (it=types.begin(); it!=types.end(); ++it)
        query += "'" + *it + "',";
    query.erase(query.length() - 1);
    query += ")";
    cout<<query<<endl;
    SimSqlRes res = sql.TryQueryRes(query);
    if (!res) 
        throw SimException("mysql error:"+sql.errmsg());    
    int cnt = res.RowNum();
    for (int i=0; i<cnt; ++i)
    {
        MYSQL_ROW row = res.FetchRow();
        if (!row[3] || !row[4] || !row[5]) continue;
        string type_s = row[3];
        int start_index = atoi(row[4]);
        int end_index = atoi(row[5]);
        RedisInfo info;
        if (row[1]) info.host = row[0];
        if (row[2]) info.port = atoi(row[1]);
        if (row[3]) info.time_out = {1, 1000*atoi(row[2])};


        for (int j=start_index; j<=end_index; ++j)
        {
            info_util.type_infos[type_s][j].insert(info);
        }
    }
    //InitData(infos);
} 


