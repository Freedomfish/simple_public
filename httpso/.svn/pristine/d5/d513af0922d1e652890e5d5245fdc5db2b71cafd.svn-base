#ifndef HTTP_ENTRY_HPP
#define HTTP_ENTRY_HPP

#include "http_interface.hpp"
#include "http_entry.h"
#include "so_interface.hpp"

class HttpHandlerManager : public HandlerManager
{
public:
    HttpHandlerUtils* Get(const std::string& uri);
    void Init(void *p);
    void Exit(void *p);
private:
    SoFuncParamer sofunc_paramer_;
};

#endif

