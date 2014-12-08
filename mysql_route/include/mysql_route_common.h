#ifndef MYSQL_ROUTE_COMMON_H
#define MYSQL_ROUTE_COMMON_H

#include "mysql_route_base_factory.h"

#include <string>
#include <sim/string_common.h>

namespace sim {
class SplitDbTableRule
{
public:
    virtual int Index(int i, int type_i = 0) const
    {
        return 0;
    }
    virtual int DbIndexMax(int type_i = 0) const
    {
        return 0;
    }
    virtual int TableIndexMax(int type_i = 0) const
    {
        return 0;
    }
    virtual std::string DbName(int i,
        int type_i=0) const
    {
        return "";
    }
    virtual std::string TableName(int i,
        int type_i=0) const
    {
        return "";
    }
    virtual std::string DbNamePrefix(int type_i) const
    {
        return "";
    }
    virtual std::string TableNamePrefix(int type_i) const
    {
        return "";
    }

    // 年库
    int IndexYear(int year) const
    {
        return year;
    }
    std::string DbNameYear(int year, const std::string &prefix) const
    {
        return (prefix + atostr(year));
    }

    // 100库100表
    int IndexHundred(int i) const
    {
        return i/100%100;
    }
    std::string DbNameHundred(int i, const std::string &prefix) const
    {
        return (prefix + atostr(i/100%100));
    }
    std::string TableNameHundred(int i, const std::string &prefix) const
    {
        return (prefix + atostr(i%100));
    }

    // 10库10表
    int IndexTen(int i) const
    {
        return i/10%10;
    }
    std::string DbNameTen(int i, const std::string &prefix) const
    {
        return (prefix + atostr(i/10%10));
    }
    std::string TableNameTen(int i, const std::string &prefix) const
    {
        return (prefix + atostr(i%10));
    }
};

template<class T>
class MysqlRouteCommon
{
public:
    //is_pool表示是否实用连接池，copy_num是每个对象的副本数量(实用线程池时建议>=线程数)
    static void Init(SplitDbTableRule* h, MysqlRouteBaseFactory *f,
                     bool is_pool=false, int copy_num=1)
    {
        baseh_.reset(h);
        factory_.reset(f);
        info_manager_.reset(f->NewInfoManager());
        is_pool_=is_pool;
        copy_num_=copy_num;
    }
    static void InitData(SimIni& ini, const std::vector<std::string>& db_types)
    {
        info_manager_->Init(ini, db_types);
    }
    static void InitData(SimSql& sql, const std::vector<std::string>& db_types)
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
    SharedPtr<MysqlRouteBase> route()
    {
        Start();
        return route_;
    }
    SharedPtr<SplitDbTableRule> split_handler()
    {
        return baseh_;
    }
protected:
    SharedPtr<MysqlRouteBase> route_;
    static SharedPtr<SplitDbTableRule> baseh_;
    static SharedPtr<MysqlRouteBaseFactory> factory_;
    static SharedPtr<MysqlInfoManager> info_manager_;
    static bool is_pool_;
    static int copy_num_;
};

template<class T>
SharedPtr<SplitDbTableRule> MysqlRouteCommon<T>::baseh_ = NULL;
template<class T>
SharedPtr<MysqlRouteBaseFactory> MysqlRouteCommon<T>::factory_ = NULL;
template<class T>
SharedPtr<MysqlInfoManager> MysqlRouteCommon<T>::info_manager_ = NULL;
template<class T>
bool MysqlRouteCommon<T>::is_pool_=false;
template<class T>
int MysqlRouteCommon<T>::copy_num_=1;

}
#endif

