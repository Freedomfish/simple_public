#ifndef ROUTE_BASE_H
#define ROUTE_BASE_H
#include <map>
#include <vector>
#include "typedef.h"
#include <iostream>

namespace sim {

//以主机为单位的连接对象集合
//status标记主机状态(0正常，1异常)
template<class T>
class AbstractObjMaster
{
public:
    virtual void AddObj(SharedPtr<T> o) = 0;
    virtual SharedPtr<T> get() = 0;
    virtual int status()=0;
    virtual void set_status(int s)=0;
};

//关联主机和连接，连接实用完后自动回收到主机中
template<class T>
class AbstractRouteHandler
{
public:
    virtual ~AbstractRouteHandler(){}
    virtual T* obj() = 0;
    virtual SharedPtr<T> get(){return NULL;}
    virtual void set_status(int s){}
    virtual int status(){return 0;}
};

//调用者获取到的路由对象
template<class T>
class RouteObj
{
public:
    RouteObj():handler_(NULL){}
    virtual ~RouteObj(){}
    void set_handler(SharedPtr<AbstractRouteHandler<T> > h){handler_ = h;}
    SharedPtr<AbstractRouteHandler<T>> handler(){return handler_;}
    T* operator -> (){return handler_->obj();}
    SharedPtr<T> get (){return handler_->get();}
    operator const bool () {return handler_!=NULL && handler_->obj()!=NULL;}
private:
    SharedPtr<AbstractRouteHandler<T> > handler_;
};

//路由
template<class KEY, class OBJ>
class AbstractRoute
{
public:
    virtual void AddMaster(const KEY& key, SharedPtr<AbstractObjMaster<OBJ>> m) = 0;
    virtual RouteObj<OBJ> get(const KEY& k) = 0;
};

}
#endif
