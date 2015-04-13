#ifndef SIM_UTILS_H
#define SIM_UTILS_H

#include <string>
#include <stdio.h>
#include <stdint.h>

namespace sim
{

#define MAX_INT_LENGH = 21

inline std::string AtoStr(uint64_t i)
{
    char data_[MAX_INT_LENGH]={0};
    sprintf(data, "%llu", i);
    return data;
}

}//endof namespace sim
#endif
