#ifndef SIM_CLASS_FUNC_H
#define SIM_CLASS_FUNC_H
#include <map>

template<class K, class C>
class ClassFunc
{
public:
    typedef int (C::*func)();
    typedef std::map<K, func> FuncMap;
public:
    virtual void Init() = 0;
    int ExecFunc(const K& k)
    {
        if (!init_flag_)
        {
            Init();
            init_flag_ = true;
        }
        typename FuncMap::iterator it;
        it = func_map_.find(k);
        if (it == func_map_.end()) return -1;
        return (((C*)this)->*it->second)();
    }
    void AddFunc(const K& k, func f)
    {
        func_map_[k] = f;
    }
protected:
    
private:
    static FuncMap func_map_;
    static bool init_flag_;
};
template<class K, class C>
typename ClassFunc<K,C>::FuncMap ClassFunc<K,C>::func_map_;
template<class K, class C>
bool ClassFunc<K, C>::init_flag_ = false;

#endif
