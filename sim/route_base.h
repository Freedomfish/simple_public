#ifndef ROUTE_BASE_H
#define ROUTE_BASE_H
#include "simpool.h"
#include <map>
#include <vector>
#include "typedef.h"
#include <iostream>
#include "log4cplus_common.h"

namespace sim {

#define SimPoolHandler SimPoolHandler

template<class T>
class RouteBaseHandler
{
public:
    virtual ~RouteBaseHandler(){}
    virtual T* handler() = 0;
    virtual SharedPtr<T> get(){return NULL;}
};

template<class T>
class RouteObj
{
public:
    RouteObj():handler_(NULL){}
    virtual ~RouteObj(){}
    void set_handler(SharedPtr<RouteBaseHandler<T> > h){handler_ = h;}
    T* operator -> (){return handler_->handler();}
    SharedPtr<T> get (){return handler_->get();}
    operator const bool () {return handler_!=NULL && handler_->handler()!=NULL;}
private:
    SharedPtr<RouteBaseHandler<T> > handler_;
};


template<class OBJ>
class RouteHandler : public RouteBaseHandler<OBJ>
{
public:
    RouteHandler()
    {
        obj_ = NULL;
        ppool_ = NULL;
    }
    ~RouteHandler()
    {
        if (ppool_) ppool_->push_back(obj_);
        ppool_ = NULL;
    }
    OBJ* handler() {return obj_.get();}
    SharedPtr<OBJ> get() {return obj_;}
public:
    void set_obj(SharedPtr<OBJ> o){obj_ = o;}
    void set_pool(SimPool<SharedPtr<OBJ>>& p){ppool_ = &p;}

public:
    operator const bool (){return obj_ != NULL;}
    OBJ* operator -> () {return obj_.get();}

private:
    SharedPtr<OBJ> obj_;
    SimPool<SharedPtr<OBJ>>* ppool_;
};

template<class KEY, class OBJ>
class RouteBase
{
public:
    void AddObj(const KEY& k, SharedPtr<OBJ> obj);
    SharedPtr<RouteHandler<OBJ>> get(const KEY& k);
private:
    std::map<KEY, SimPool<SharedPtr<OBJ> > > pools_;
};

template<class KEY, class OBJ>
SharedPtr<RouteHandler<OBJ>> RouteBase<KEY, OBJ>::get(const KEY& k)
{
    typename std::map<KEY, SimPool<SharedPtr<OBJ> > >::iterator it;
    it = pools_.find(k);
    if (it == pools_.end()) 
        return NULL;
    SharedPtr<RouteHandler<OBJ> > objptr(new RouteHandler<OBJ>);
    objptr->set_obj(it->second.get());
    objptr->set_pool(it->second);
    return objptr;
}



template<class KEY, class OBJ>
void RouteBase<KEY, OBJ>::AddObj(
    const KEY& k, SharedPtr<OBJ> obj)
{
    pools_[k].push_back(obj);
}
}
#endif
