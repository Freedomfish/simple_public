#ifndef HTTPSO_H
#define HTTPSO_H

#include "simple_array.h"
#include "so_interface.hpp"
typedef void (*sofunc_init_pt)(SoFuncParamer&);
typedef void (*sofunc_exit_pt)(SoFuncParamer&);

class SoHandlerManager{
public:
    static int SoInitFuncAdd(sofunc_init_pt func);
    static int SoExitFuncAdd(sofunc_exit_pt func);
    static int Init(SoFuncParamer&);
    static void Exit(SoFuncParamer&);
private:
    static simple_array_t init_arr_;
    static simple_array_t exit_arr_;
};


#ifdef __cplusplus
extern "C" {
#endif

int so_load_func(void* paramer);
void so_unload_func(void* paramer);

#ifdef __cplusplus
}
#endif

#endif
