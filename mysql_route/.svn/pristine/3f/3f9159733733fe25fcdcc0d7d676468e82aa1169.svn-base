#ifndef MYSQL_ROUTE_COMMON_H
#define MYSQL_ROUTE_COMMON_H

#include "mysql_route_base_factory.h"

#include <string>
#include <sim/string_common.h>

namespace sim {
class SplitDbTableRule
{
public:
    virtual int Index(const int i, const int type_i = 0) const
    {
        return 0;
    }
    virtual int DbIndexMax(const int type_i = 0) const
    {
        return 0;
    }
    virtual int TableIndexMax(const int type_i = 0) const
    {
        return 0;
    }
    virtual std::string DbName(const int i,
        const int type_i=0) const
    {
        return "";
    }
    virtual std::string TableName(const int i,
        const int type_i=0) const
    {
        return "";
    }
    virtual std::string DbNamePrefix(const int type_i) const
    {
        return "";
    }
    virtual std::string TableNamePrefix(const int type_i) const
    {
        return "";
    }

    // 年库
    int IndexYear(const int year) const
    {
        return year;
    }
    std::string DbNameYear(const int year, const std::string &prefix) const
    {
        return (prefix + atostr(year));
    }

    // 100库100表
    int IndexHundred(const int i) const
    {
        return i/100%100;
    }
    std::string DbNameHundred(const int i, const std::string &prefix) const
    {
        return (prefix + atostr(i/100%100));
    }
    std::string TableNameHundred(const int i, const std::string &prefix) const
    {
        return (prefix + atostr(i%100));
    }

    // 10库10表
    int IndexTen(const int i) const
    {
        return i/10%10;
    }
    std::string DbNameTen(const int i, const std::string &prefix) const
    {
        return (prefix + atostr(i/10%10));
    }
    std::string TableNameTen(const int i, const std::string &prefix) const
    {
        return (prefix + atostr(i%10));
    }
};

template<class T>
class MysqlRouteCommon
{
public:
    static void Init(SplitDbTableRule* h, MysqlRouteBaseFactory *f)
    {
        baseh_.reset(h);
        factory_.reset(f);
        info_manager_.reset(f->NewInfoManager());
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
protected:
    SharedPtr<MysqlRouteBase> route_;
    static SharedPtr<SplitDbTableRule> baseh_;
    static SharedPtr<MysqlRouteBaseFactory> factory_;
    static SharedPtr<MysqlInfoManager> info_manager_;
};

template<class T>
SharedPtr<SplitDbTableRule> MysqlRouteCommon<T>::baseh_ = NULL;
template<class T>
SharedPtr<MysqlRouteBaseFactory> MysqlRouteCommon<T>::factory_ = NULL;
template<class T>
SharedPtr<MysqlInfoManager> MysqlRouteCommon<T>::info_manager_ = NULL;

}
#endif

