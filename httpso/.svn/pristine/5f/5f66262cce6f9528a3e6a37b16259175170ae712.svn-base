#ifndef HTTP_INTERFACE_HPP
#define HTTP_INTERFACE_HPP
#include "http_interface.h"
#include <string>
#include <vector>
#include <map>

#define LOG_ERROR(fmt, ...) printf(fmt, __VA_ARGS__)
struct HttpRequest;
typedef int (*http_handler_pt)(HttpRequest&);
typedef void (*http_init_handler_pt)(void*);
typedef void (*http_exit_handler_pt)(void*);
typedef bool(*add_handler_pt)(const std::string&, http_handler_pt);

struct HttpHandlerUtils{
    HttpHandlerUtils():handler(NULL){}
    http_handler_pt handler;
    bool is_excat_filted;
};

struct HttpSession{
    http_sess_t* sess;
    void SetStr(const std::string& name, const std::string& value);
    void SetInt(const std::string& name, int value);
    std::string GetStr(const std::string& name);
    int GetInt(const std::string& name);
    void Delete(const std::string& name);
    void Flush();
};

struct HttpRequest{
    http_request_t* req;
    HttpHandlerUtils* handler_utils;
    HttpSession session;
    std::string uri;
    http_method_t method;
    std::map<std::string, std::string> args;
    goc_string_t* body;

    void Finalize(int ret){
            req->finalizefunc(req->ctx, ret);
        }
    const std::string& FormValue(const std::string& key){
        return args[key];
    }
    void SendData(const std::string& data){
        req->sendfunc(req->ctx, (char*)data.c_str(), data.length());
    }
};

inline void HttpSession::SetStr(const std::string& name, const std::string& value){
            sess->set_str(sess->sess, (char*)name.c_str(), (char*)value.c_str());
}
inline void HttpSession::SetInt(const std::string& name, int value){
            sess->set_int(sess->sess, (char*)name.c_str(), value);
}
inline std::string HttpSession::GetStr(const std::string& name){
       char* s = sess->get_str(sess->sess, (char*)name.c_str());
       if (s) return s;
       return "";
}
inline int HttpSession::GetInt(const std::string& name){
       return sess->get_int(sess->sess, (char*)name.c_str());
}
inline void HttpSession::Delete(const std::string& name){
       sess->del(sess->sess, (char*)name.c_str());
}
inline void HttpSession::Flush(){
       sess->flush(sess->sess);
}


#endif
