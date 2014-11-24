#ifndef UTIL_TOOL_H
#define UTIL_TOOL_H
#include <sys/time.h>

namespace sim {
class SimTimeGet
{
public:
    SimTimeGet()
    {
        gettimeofday(&m_st, NULL);
    }
    inline int64_t msec()
    {
        return (m_st.tv_sec*1000 + m_st.tv_usec/1000);
    }
    //时间差
    inline int  msec_interval()
    {
        gettimeofday(&m_et, NULL);
        return (m_et.tv_sec*1000 + m_et.tv_usec/1000) - (m_st.tv_sec*1000 + m_st.tv_usec/1000);
    }
    inline void reset()
    {
        gettimeofday(&m_st, NULL);
    }
private:
    struct timeval m_st;
    struct timeval m_et;
};
}
#endif
