#ifndef SIMPOOL_H
#define SIMPOOL_H
#include <deque>
#include <stdio.h>

namespace sim{

template<typename T>
void simswap(T a, T b){T t=a; a=b; b=t;}

template<typename T>class SimPoolHandler;

template<typename T>
class SimPool
{
public:
    SimPool(){m_max_size = 1024;}
    inline void set_max_size(size_t n){m_max_size = n;};
    size_t max_size() const {return m_max_size;};
    size_t size() const 
    {
        return pool_.size();
    }
    void clear(void (*free_func)(T)=NULL);
    int push_back(T t);
    
    T get(void);
    
private:
    std::deque<T> pool_;
    size_t m_max_size;
};

template<typename T>
inline int SimPool<T>::push_back(T t)
{   
    if (pool_.size() >  m_max_size) return 1;
    pool_.push_back(t);
    return 0;
}

template<typename T>
inline T SimPool<T>::get(void)
{
    if (pool_.empty()) return (T)0;
    T t1 = pool_.back();
    pool_.pop_back();
    return t1;
}
template<typename T>
inline void SimPool<T>::clear(void (*free_func)(T t))
{
    if (pool_.empty()) return;
    if (free_func)
    {
        typename std::deque<T>::iterator it;
        for (it=pool_.begin(); it!=pool_.end(); ++it)
            if (*it) free_func(*it); 
    }
    pool_.clear();
}

}//end of namespace
#endif
