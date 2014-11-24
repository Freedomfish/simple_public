#ifndef LOG_CONFIG_H
#define LOG_CONFIG_H

#if 0
#ifndef LOG_INIT
#define LOG_INIT
#endif
#ifndef LOG_APP_DEBUG
#define LOG_APP_DEBUG(fmt, ...) do {printf("DEBUG:" fmt, __VA_ARGS__);printf("\n");}while(0)
#endif

#ifndef LOG_APP_INFO_
#define LOG_APP_INFO(fmt, ...) do {printf("INFO:" fmt, __VA_ARGS__);printf("\n");}while(0)
#endif

#ifndef LOG_APP_ERROR
#define LOG_APP_ERROR(fmt, ...) do {printf("ERROR:" fmt, __VA_ARGS__);printf("\n");}while(0)
#endif

#endif

#if 1
#include <log4cplus/configurator.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <stdio.h>

/*
 * NOT_SET_LOG_LEVEL  (   -1)   ：接受缺省的LogLevel，如果有父logger则继承它的LogLevel
 * ALL_LOG_LEVEL      (    0)    ：开放所有log信息输出
 * TRACE_LOG_LEVEL    (    0)    ：开放trace信息输出(即ALL_LOG_LEVEL)
 * DEBUG_LOG_LEVEL    (10000) ：开放debug信息输出
 * INFO_LOG_LEVEL     (20000) ：开放info信息输出
 * WARN_LOG_LEVEL     (30000) ：开放warning信息输出
 * ERROR_LOG_LEVEL    (40000) ：开放error信息输出
 * FATAL_LOG_LEVEL    (50000)  ：开放fatal信息输出
 * OFF_LOG_LEVEL      (60000)  ：关闭所有log信息输出
 */

#define LOG_APP_DEBUG_ROOT log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("APP_DEBUG"))
#define LOG_APP_INFO_ROOT log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("APP_INFO"))
#define LOG_APP_ERR_ROOT log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("APP_ERR"))

inline bool isdebug(int l);
inline bool isinfo(int l);
inline bool iserror(int l);

inline int LOG_INIT(const char* fname)
{
    static bool flag = false;
    if (!flag)
    {
        log4cplus::PropertyConfigurator::doConfigure(fname);
        int dl = LOG_APP_DEBUG_ROOT.getLogLevel();
        isdebug(dl);
        int il = LOG_APP_INFO_ROOT.getLogLevel();
        isinfo(il);
        int el = LOG_APP_ERR_ROOT.getLogLevel();
        iserror(el);
    }
    flag = true;
    return 0;
}

inline bool isdebug(int l = 0)
{
    static int level = 0;
    if (l)
        level = l;
    return level >= log4cplus::DEBUG_LOG_LEVEL;
}
inline bool isinfo(int l = 0)
{
    static int level = 0;
    if (l)
        level = l;
    return level >= log4cplus::INFO_LOG_LEVEL;
}
inline bool iserror(int l = 0)
{
    static int level = 0;
    if (l)
        level = l;
    return level >= log4cplus::WARN_LOG_LEVEL;
}

//APP共用了一个 logger配置
//SYS,ACC 是独立的 logger 配置,这样 SYS,ACC,APP(APP_DAILY) 分别属于不同的logger
//它们可以独立的输出文件,相互不干扰

//#define LOG_ACC_ROOT log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("ACC.ACC"))
//#define LOG_SYS_ROOT log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("SYS"))

#define TMP_BUF_MAX 4096

//#ifdef WIN32
//# ifdef UNICODE
//#   define snprintf _snwprintf_s
//# else
//#   define snprintf _snprintf_s
//# endif
//#endif

#ifdef WIN32
#define LOG4CPLUS_C_STR_TO_TCSTR(str) LOG4CPLUS_C_STR_TO_TSTRING(str).c_str()
#define snprintf _snprintf_s
#else
#define LOG4CPLUS_C_STR_TO_TCSTR(str) str
#endif

#define OPENLOG 1

// app
#define LOG_APP_DEBUG(fmt, ...) \
    do{ \
        if (isdebug()){\
        char tmp_buf[TMP_BUF_MAX + 1]; \
        snprintf(tmp_buf, TMP_BUF_MAX, "%s|%d|%s|" fmt, __FILE__, __LINE__, __FUNCTION__ , ## __VA_ARGS__); \
        LOG4CPLUS_DEBUG(LOG_APP_DEBUG_ROOT, LOG4CPLUS_C_STR_TO_TCSTR(tmp_buf)); \
        }\
    } while (0)

#define LOG_APP_INFO(fmt, ...) \
    do { \
        if (isinfo()){\
        char tmp_buf[TMP_BUF_MAX + 1];\
        snprintf(tmp_buf, TMP_BUF_MAX, "%s|%d|%s|" fmt, __FILE__, __LINE__, __FUNCTION__ , ## __VA_ARGS__); \
        LOG4CPLUS_INFO(LOG_APP_INFO_ROOT, LOG4CPLUS_C_STR_TO_TCSTR(tmp_buf)); \
        }\
    } while (0)

#define LOG_APP_WARN(fmt, ...) \
    do { \
        if (isinfo())    {\
        char tmp_buf[TMP_BUF_MAX + 1]; \
        snprintf(tmp_buf, TMP_BUF_MAX, "%s|%d|%s|" fmt, __FILE__, __LINE__, __FUNCTION__ , ## __VA_ARGS__); \
        LOG4CPLUS_WARN(LOG_APP_ERR_ROOT, LOG4CPLUS_C_STR_TO_TCSTR(tmp_buf)); \
        }\
    } while (0)

#define LOG_APP_ERROR(fmt, ...) \
    do { \
        if (iserror()) {\
        char tmp_buf[TMP_BUF_MAX + 1]; \
        snprintf(tmp_buf, TMP_BUF_MAX, "%s|%d|%s|" fmt, __FILE__, __LINE__, __FUNCTION__ , ## __VA_ARGS__); \
        LOG4CPLUS_ERROR(LOG_APP_ERR_ROOT, LOG4CPLUS_C_STR_TO_TCSTR(tmp_buf)); \
        }\
    } while (0)

#define LOG_APP_FATAL(fmt, ...) \
    do { \
        if (iserror()) {\
        char tmp_buf[TMP_BUF_MAX + 1]; \
        snprintf(tmp_buf, TMP_BUF_MAX, "%s|%d|%s|" fmt, __FILE__, __LINE__, __FUNCTION__ , ## __VA_ARGS__); \
        LOG4CPLUS_FATAL(LOG_APP_ERR_ROOT, LOG4CPLUS_C_STR_TO_TCSTR(tmp_buf)); \
        }\
    } while (0)
#endif

#endif
