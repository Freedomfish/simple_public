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

template<class OBJ>
class RouteObj
{
public:
    RouteObj()
    {
        obj = NULL;
        ppool = NULL;
    }
    ~RouteObj()
    {
        if (ppool) ppool->push_back(obj);
        ppool = NULL;
    }
    operator const bool (){return obj != NULL;}
    OBJ* operator -> () {return obj.get();}
    SharedPtr<OBJ> get() {return obj;}
public:
    SharedPtr<OBJ> obj;
    SimPool<SharedPtr<OBJ>>* ppool;
};

template<class KEY, class OBJ>
class RouteBase
{
public:
    void AddObj(const KEY& k, SharedPtr<OBJ> obj);
    SimPoolHandler<SharedPtr<OBJ> > Handler(const KEY& k);
    RouteObj<OBJ> get(const KEY& k);
private:
    std::map<KEY, SimPool<SharedPtr<OBJ> > > pools_;
};

template<class KEY, class OBJ>
SimPoolHandler<SharedPtr<OBJ> > RouteBase<KEY, OBJ>::Handler(const KEY& k)
{
    typename std::map<KEY, SimPool<SharedPtr<OBJ> > >::iterator it;
    it = pools_.find(k);
    if (it == pools_.end()) 
        return SimPoolHandler<SharedPtr<OBJ> >(NULL, (SharedPtr<OBJ>)0);
    return it->second.GetHandler();
}

template<class KEY, class OBJ>
RouteObj<OBJ> RouteBase<KEY, OBJ>::get(const KEY& k)
{
    RouteObj<OBJ> obj;
    typename std::map<KEY, SimPool<SharedPtr<OBJ> > >::iterator it;
    it = pools_.find(k);
    if (it == pools_.end()) 
        return obj;
    obj.obj = it->second.get();
    obj.ppool = &it->second;
    return obj;
}



template<class KEY, class OBJ>
void RouteBase<KEY, OBJ>::AddObj(
    const KEY& k, SharedPtr<OBJ> obj)
{
    pools_[k].push_back(obj);
}
}
#endif
