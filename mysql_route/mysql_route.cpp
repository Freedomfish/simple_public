#include "mysql_route.h"
#include <iostream>
#include <sim/log4cplus_common.h>
#include <sim/simroute.h>
using namespace std;
using namespace sim;

static void LoadDataFromIni(
                SimIni& ini, 
                const std::vector<std::string>& types, 
                MysqlInfoUtil& info_util);
static void LoadDataFromDb(
                SimSql& sql,
                const std::vector<std::string>& types,
                MysqlInfoUtil& info_util);

namespace sim {

void MysqlRouteInfo::Init(SimIni& ini,
    const std::vector<std::string>& db_types)
{
    LoadDataFromIni(ini, db_types, info_util_);
}

void MysqlRouteInfo::Init(SimSql& sql,
    const std::vector<std::string>& db_types)
{
    LoadDataFromDb(sql, db_types, info_util_);
}

void *MysqlRouteInfo::Infos()
{
    return (void*)&info_util_;
}

void MysqlRoute::InitData(MysqlInfoManager* infos)
{
    if (!is_started_)
    {
         Start(infos);
    }
}

MysqlObj MysqlRoute::get(int index,
    const std::string& db_key, int r_w)
{
    map<string, SharedPtr<MysqlBase> >::iterator it;
    it = routes_[r_w].find(db_key);
    if (it != routes_[r_w].end()) 
        return it->second->get(index);
    return MysqlObj();
}

void MysqlRoute::Start(MysqlInfoManager *pinfo)
{
    MysqlInfoUtil* infos = (MysqlInfoUtil*)pinfo->Infos();
    MysqlInfoSet::iterator sit;
    std::map<MysqlInfo, SharedPtr<AbstractObjStore<SimSql>>> obj_map;
    //根据info生成对象
    for (sit=infos->infos.begin(); sit!=infos->infos.end(); ++sit)
    {
        SharedPtr<AbstractObjStore<SimSql>> os;
        if (!is_pool_) os.reset(new SimConnObjStore<SimSql>);
        else os.reset(new SimConnPoolObjStore<SimSql>);
        obj_map[*sit] = os;

        for (int i=0; i<copy_num_; ++i)
            os->AddObj(SharedPtr<SimSql>(new SimSql(*sit)));
    }
    //根据type和index建立对象关联
    map<string, MysqlInfoSetMap[2]>::iterator mit;
    for (mit=infos->type_infos.begin(); mit!=infos->type_infos.end(); ++mit)
    {
        MysqlInfoSetMap::iterator it;
        for (size_t i=0; i<2; ++i)
        {
            SharedPtr<MysqlBase> ptr(new SimRoute<int, SimSql>);
            routes_[i][mit->first] = ptr;
            for (it=mit->second[i].begin(); it!=mit->second[i].end(); ++it)
            {
                for (sit=it->second.begin(); sit!=it->second.end(); ++sit)
                    ptr->AddObj(it->first, obj_map[*sit]);
            }
        }
    }
}

}

static void LoadDataFromIni(
                SimIni& ini, 
                const std::vector<std::string>& types, 
                MysqlInfoUtil& info_util)
{
    vector<string>::const_iterator cit;
    for (cit=types.begin(); cit!=types.end(); ++cit)
    {
        string sess = "mysql_route_" + *cit;
        map<string, string> kvm = ini.GetSession(sess);
        if (kvm.empty()) 
            throw SimException(
                "read config failed|mysql_route not found|sess="+sess);

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
            if (3 != k.size()) continue;
            split(it->second, ';', vs);
            int b = atoi(k[0].c_str());
            int e = atoi(k[1].c_str());
            int r_w = k[2]=="rw"?0:k[2]=="r"?1:k[2]=="w"?2:-1;
            if (-1 == r_w) continue;
            for (vit=vs.begin(); vit!=vs.end(); ++vit)
            {
                split(*vit, '|', v);
                MysqlInfo info;
                info.host = v[0];
                info.port = atoi(v[1].c_str());
                info.user = v[2];
                info.passwd = v[3];
                info.db = v[4];
                info_util.infos.insert(info);
                for (int i=b; i<=e; ++i)
                {
                    if (0 == r_w)
                    {
                        info_util.type_infos[*cit][0][i].insert(info);
                        info_util.type_infos[*cit][1][i].insert(info);
                    }
                    else
                    {
                        info_util.type_infos[*cit][r_w-1][i].insert(info);
                    }
                }
            }
        }
    }
}

static void LoadDataFromDb(
    SimSql& sql, 
    const std::vector<std::string>& db_types,
    MysqlInfoUtil& info_util)
{
    if (db_types.size() == 0) 
        throw SimException("no db_type");
    string query = "select rw_flag, host, port, username, password,"
        " db, db_type, start_index, end_index from route.mysql_route ";
    query += " where db_type in (";
    vector<string>::const_iterator it;
    for (it=db_types.begin(); it!=db_types.end(); ++it)
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
        if (!row[6] || !row[0] || !row[7] || !row[8]) continue;
        int r_w = atoi(row[0]);
        if (r_w <0 || r_w > 2) continue;
        string db_type = row[6];
        int start_index = atoi(row[7]);
        int end_index = atoi(row[8]);
        MysqlInfo info;
        if (row[1]) info.host = row[1];
        if (row[2]) info.port = atoi(row[2]);
        if (row[3]) info.user = row[3];
        if (row[4]) info.passwd = row[4];
        if (row[5]) info.db = row[5];

        info_util.infos.insert(info);
        for (int j=start_index; j<=end_index; ++j)
        {
            if (0 == r_w)
            {
                info_util.type_infos[db_type][0][j].insert(info);
                info_util.type_infos[db_type][1][j].insert(info);
            }
            else
            {
                info_util.type_infos[db_type][r_w-1][j].insert(info);
            }
        }
    }
    //InitData(infos);
} 

