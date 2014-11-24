#ifndef HTTP_INTERFACE_H
#define HTTP_INTERFACE_H

#include "goc_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef goc_string_t goc_string;
enum http_return{
    HTTP_OK = 0,
    HTTP_ERROR,
    HTTP_NO_HANDLER,
    HTTP_FILTED //被过滤器过滤
};

enum http_method{
    GET = 0,
    POST,
    OTHER
};

typedef enum http_method http_method_t;
struct request_arg{
    char* name;
    goc_string value;
};

typedef void (*set_sess_int_pt)(void* sess, char* name, int value);
typedef void (*set_sess_str_pt)(void* sess, char* name, char* value);
typedef int (*get_sess_int_pt)(void* sess, char* name);
typedef char* (*get_sess_str_pt)(void* sess, char* name);
typedef void (*del_sess_pt)(void* sess, char* name);
typedef void (*flush_sess_pt)(void* sess);
struct http_sess{
    void* sess;
    set_sess_int_pt set_int;
    set_sess_str_pt set_str;
    get_sess_int_pt get_int;
    get_sess_str_pt get_str;
    del_sess_pt     del;
    flush_sess_pt   flush;
};
typedef struct http_sess http_sess_t;

typedef struct request_arg request_arg_t;
typedef void (*send_data_pt)(void* ctx, char* data, int length);
typedef void (*finalize_request_pt)(void* ctx, int result);
struct http_request{
    void* ctx;
    http_sess_t sess;
    send_data_pt sendfunc;
    finalize_request_pt finalizefunc;
    char* uri;
    http_method_t method;
    request_arg_t* args;
    size_t arglen;
    goc_string_t body;
};
typedef struct http_request http_request_t;

#ifdef __cplusplus
}
#endif

#endif
