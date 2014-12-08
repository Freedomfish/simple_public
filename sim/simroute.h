#ifndef SIMROUTE_H
#define SIMROUTE_H
#include "route_base.h"
#include "simpool_with_lock.h"
#include "route_base.h"
#include "typedef.h"
#include <map>
#include <vector>
#include "typedef.h"
#include <iostream>
#include <atomic>


namespace sim {

//一个主机一个对象，status标记主机状态
template<class T>
class SimConnObjStore : public AbstractObjStore<T>
{
public:
    void AddObj(SharedPtr<T> obj){pools_.push_back(obj);}
    SharedPtr<T> get(){return pools_.get();}
    int status(){return status_;}
    void set_status(int s){status_ = s;};
private:
    std::atomic_int status_;
    SimPool<SharedPtr<T>> pools_;
};

//一个主机一个对象，status标记主机状态
template<class T>
class SimConnPoolObjStore : public AbstractObjStore<T>
{
public:
    void AddObj(SharedPtr<T> obj){pools_.push_back(obj);}
    SharedPtr<T> get(){return pools_.get();}
    int status(){return status_;}
    void set_status(int s){status_ = s;};
private:
    std::atomic_int status_;
    SimLockPool<SharedPtr<T>> pools_;
};

template<class T>
class SimRouteHandler : public AbstractRouteHandler<T>
{
public:
    SimRouteHandler(AbstractObjStore<T>* p)
        :pool_(p), obj_(p->get()){}
    ~SimRouteHandler(){if (obj_) pool_->AddObj(obj_);}
    T* handler()
    {
        return obj_.get();
    }
    SharedPtr<T> get()
    {
        return obj_;
    };
    int status(){return pool_->status();}
    void set_status(int s){pool_->set_status(s);}
private:
    AbstractObjStore<T>* pool_;
    SharedPtr<T> obj_;
};

template<class KEY, class OBJ>
class SimRoute : public AbstractRoute<KEY, OBJ>
{
public:
    void AddObj(const KEY& key, SharedPtr<AbstractObjStore<OBJ>> objs);
    RouteObj<OBJ> get(const KEY& k);
private:
    std::map<KEY, std::vector<SharedPtr<AbstractObjStore<OBJ>>>> pools_; 
};

template<class KEY, class OBJ>
void SimRoute<KEY, OBJ>::AddObj(const KEY& k, SharedPtr<AbstractObjStore<OBJ>> obj)
{
    pools_[k].push_back(obj);
};

template<class KEY, class OBJ>
RouteObj<OBJ> SimRoute<KEY, OBJ>::get(const KEY& k)
{
    RouteObj<OBJ> o;
    typename std::map<KEY, 
         std::vector<SharedPtr<AbstractObjStore<OBJ>>>
         >::iterator it;
    it = pools_.find(k);
    if (it != pools_.end())
    {
        typename std::vector<SharedPtr<AbstractObjStore<OBJ>>>::iterator vit;
        for (vit=it->second.begin(); vit!=it->second.end(); ++vit)
        {
            if (!(*vit)->status())
            {
                o.set_handler(SharedPtr<AbstractRouteHandler<OBJ>>(new SimRouteHandler<OBJ>(vit->get())));
            }
        }
    }
    return o;
};



}

#endif
