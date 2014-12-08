#ifndef ROUTE_BASE_H
#define ROUTE_BASE_H
#include <map>
#include <vector>
#include "typedef.h"
#include <iostream>

namespace sim {

template<class T>
class AbstractRouteHandler
{
public:
    virtual ~AbstractRouteHandler(){}
    virtual T* handler() = 0;
    virtual SharedPtr<T> get(){return NULL;}
    virtual void set_status(int s){}
    virtual int status(){return 0;}
};

template<class T>
class RouteObj
{
public:
    RouteObj():handler_(NULL){}
    virtual ~RouteObj(){}
    void set_handler(SharedPtr<AbstractRouteHandler<T> > h){handler_ = h;}
    T* operator -> (){return handler_->handler();}
    SharedPtr<T> get (){return handler_->get();}
    operator const bool () {return handler_!=NULL && handler_->handler()!=NULL;}
    int status(){return handler_->status();}
    void set_status(int s){handler_->set_status(s);}
private:
    SharedPtr<AbstractRouteHandler<T> > handler_;
};

//以主机为单位的连接对象集合
template<class T>
class AbstractObjStore
{
public:
    virtual void AddObj(SharedPtr<T> obj) = 0;
    virtual SharedPtr<T> get() = 0;
    virtual int status()=0;
    virtual void set_status(int s)=0;
};

template<class KEY, class OBJ>
class AbstractRoute
{
public:
    virtual void AddObj(const KEY& key, SharedPtr<AbstractObjStore<OBJ>> objs) = 0;
    virtual RouteObj<OBJ> get(const KEY& k) = 0;
};

}
#endif
