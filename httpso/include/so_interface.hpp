#ifndef SO_INTERFACE_HPP
#define SO_INTERFACE_HPP

#define SO_LOAD_FUNC "so_load_func"
#define SO_UNLOAD_FUNC "so_unload_func"

#include "http_interface.hpp"
#include "simple_array.h"
#include <map>
#include <string>
#include <sim/simfilter.h>

typedef int (*sofunc_load_pt)(void*);
typedef void (*sofunc_unload_pt)(void*);

typedef std::map<std::string, HttpHandlerUtils> HttpHandlerFuncMap; 

class HandlerManager{
public:
    inline int AddInitHandler(http_init_handler_pt); 
    inline int AddExitHandler(http_exit_handler_pt);
    inline bool AddHandler(const std::string& name, http_handler_pt func, bool is_excat_filted = false);
    inline sim::UrlFilterManager& InFilter(){return in_filter_;}
    inline sim::UrlFilterManager& OutFilter(){return out_filter_;}
protected:
    HttpHandlerFuncMap httphandlers_;
    simple_array_t init_arr_;
    simple_array_t exit_arr_;
    sim::UrlFilterManager in_filter_;
    sim::UrlFilterManager out_filter_;
};

class SoFuncParamer{
public:
    inline HandlerManager* handler_manager(){
        return handler_manager_;
    }
    inline const std::string& path()const{
        return path_;
    }
    inline void set_path(const std::string& p){
        path_ = p;
    }
    inline void set_hander_manager(HandlerManager* p){
        handler_manager_ = p;
    }
private:
    HandlerManager* handler_manager_;
    std::string path_;
};

inline int HandlerManager::AddInitHandler(http_init_handler_pt h)
{
    simple_array_add(&init_arr_, (void*)h, 10);
    return 0;
}

inline int HandlerManager::AddExitHandler(http_init_handler_pt h)
{
    simple_array_add(&exit_arr_, (void*)h, 10);
    return 0;
}

inline bool HandlerManager::AddHandler(const std::string& name, http_handler_pt func, bool is_excat_filted)
{
    HttpHandlerFuncMap::iterator it = httphandlers_.find(name);
    if (it == httphandlers_.end())
    {
        httphandlers_[name] = HttpHandlerUtils();
    }
    HttpHandlerUtils& u = httphandlers_[name];
    if (u.handler) return false;
    u.handler = func;
    u.is_excat_filted = is_excat_filted;
    return true;
}

#endif
