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
private:
    SharedPtr<AbstractRouteHandler<T> > handler_;
};

template<class KEY, class OBJ>
class AbstractRoute
{
public:
    virtual void AddObj(const KEY& key, SharedPtr<OBJ> obj, int copy_num=1) = 0;
    virtual RouteObj<OBJ> get(const KEY& k) = 0;
};

}
#endif
