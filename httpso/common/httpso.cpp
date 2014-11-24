#include "httpso.h"
#include <stdio.h>
//#define LOG_ERROR(fmt,...) printf(fmt, __VA_ARGS__);
//
#include <iostream>
#include "simple_array.h"
using namespace std;
simple_array_t SoHandlerManager::init_arr_;
simple_array_t SoHandlerManager::exit_arr_;
int SoHandlerManager::SoInitFuncAdd(sofunc_init_pt func)
{
    simple_array_add(&init_arr_, (void*)func, 10);
    return 0;
}

int SoHandlerManager::SoExitFuncAdd(sofunc_exit_pt func)
{
    simple_array_add(&exit_arr_, (void*)func, 10);
    return 0;
}

int SoHandlerManager::Init(SoFuncParamer& paramer)
{
    LOG_ERROR("size=%ld\n", init_arr_.size);
    size_t i = 0;
    for (i=0; i<init_arr_.size; ++i)
        ((sofunc_init_pt)init_arr_.data[i])(paramer);
    return 0;
}

void SoHandlerManager::Exit(SoFuncParamer& paramer)
{
    size_t i = 0;
    for (i=0; i<exit_arr_.size; ++i)
        ((sofunc_exit_pt)exit_arr_.data[i])(paramer);
}

#ifdef __cpluscplus
extern "C" {
#endif

int so_load_func(void* paramer)
{
    LOG_ERROR("%s|%s|%d\n", __FILE__, __FUNCTION__, __LINE__);
    //return 0;
    return SoHandlerManager::Init(*(SoFuncParamer*)paramer);
}

void so_unload_func(void* paramer)
{
    SoHandlerManager::Exit(*(SoFuncParamer*)paramer);
}

#ifdef __cpluscplus
}
#endif
