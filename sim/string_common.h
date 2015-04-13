#ifndef STRING_COMMON_H
#define STRING_COMMON_H

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <sstream>
namespace sim {
inline bool CheckMask(int mask, int off_set) { return mask & (1<<(off_set-1));}
std::string StrLoadFile(const std::string& file);
std::string StrReplace(const std::string& s, 
    const std::string& str, 
    const std::string& dst, 
    size_t count = -1);

std::string StrToLow(char* s, const size_t len);
std::string StrToUp(char* s, const size_t len);
std::string url_decode(const std::string& path);

inline int strnfind(const std::string& s, char c, int n, int pos=0)
{
	pos = s.find(c, pos);
	if (n <= 1) return pos;
    return strnfind(s, c, --n, pos+1);
}

int split(const std::string &src, const char ch, std::vector<std::string> &dest);

template<typename T>
T strtoa(const char* src, 
    const std::string& sDefault="")
{
    std::string sStr;
    if (!src) sStr = src;
    const std::string *s;
    if(!sStr.empty()) s = &sStr;
    else s = &sDefault;
    std::istringstream sBuffer(*s);
    T t;
    sBuffer >> t;
    return t;
}
template<typename T>
T strtoa(const std::string& sStr, 
    const std::string& sDefault="")
{
    const std::string *s;
    if(!sStr.empty()) s = &sStr;
    else s = &sDefault;
    std::istringstream sBuffer(*s);
    T t;
    sBuffer >> t;
    return t;
}

template<typename T>
std::string atostr(const T& t)
{
    std::ostringstream sbuf;
    sbuf<<t;
    return sbuf.str();
}


inline std::string AtoStr(uint64_t i)
{
#ifndef MAX_INT_LENGH    
#define MAX_INT_LENGH = 21
#endif
    char data_[MAX_INT_LENGH]={0};
    sprintf(data, "%llu", i);
    return data;
}

}
#endif
