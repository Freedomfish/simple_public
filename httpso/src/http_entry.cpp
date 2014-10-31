#include "http_entry.hpp"
#include "http_entry.h"
#include <sim/simini.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
using namespace sim;

HttpHandlerManager g_httphandler;
static std::string g_ignore_url;

HttpHandlerUtils* HttpHandlerManager::Get(const std::string& uri)
{
    LOG_ERROR("hander.size:%ld\n", httphandlers_.size());
    HttpHandlerFuncMap::reverse_iterator rmit = httphandlers_.rbegin();
    for (; rmit != httphandlers_.rend(); ++rmit)
    {
        if (!rmit->first.empty() && 
                 rmit->first.compare(0,rmit->first.length(),
                     uri, 0, rmit->first.length()) == 0)
        {  
            return &rmit->second;
        }
    }
    return NULL;
}

void HttpHandlerManager::Init(void* paramer)
{
    LOG_ERROR("%s|%s|%d\n", __FILE__, __FUNCTION__, __LINE__);
    /*load config*/
    const char* cfg = (char*)paramer;
    LOG_ERROR("cfg:%s\n", cfg);
    SimIni& ini = SingleSimIni::get();
    if (!ini.is_loaded()){
        int ret = ini.LoadFile(cfg);
        if (ret)
        {
            LOG_ERROR("load config file failed,ret=%d\n", ret);
            return;
        }
    }
    sofunc_paramer_.set_hander_manager(this);
    sofunc_paramer_.set_path(ini.GetStringVal("so", "path"));
    g_ignore_url = ini.GetStringVal("http-server", "ignore_url");
    if (sofunc_paramer_.path().empty())
    {
        LOG_ERROR("%s\n", "get so path failed\n");
        return;
    }
    size_t i;
    for (i=0; i<init_arr_.size; ++i)
        ((http_init_handler_pt)init_arr_.data[i])(&sofunc_paramer_);
}

void HttpHandlerManager::Exit(void* paramer)
{
    size_t i;
    for (i=0; i<exit_arr_.size; ++i)
        ((http_exit_handler_pt)exit_arr_.data[i])(&sofunc_paramer_);
}

void Init(int argc, char** argv)
{
    LOG_ERROR("%s|%s|%d|argc:%d\n", __FILE__, __FUNCTION__, __LINE__, argc);
    g_httphandler.Init((void*)argv[1]);
}

void Exit(int argc, char** argv)
{
    g_httphandler.Exit((void*)argv[1]);
}

int HttpEntry(http_request_t* req)
{
    printf("%s|%s\n", __FILE__, __FUNCTION__);
    if (!req) return HTTP_ERROR;
    http_handler_pt h = NULL;
    string url = req->uri;
    if (!g_ignore_url.empty()
        && 0 == url.compare(0, g_ignore_url.length(), g_ignore_url))
        url = url.substr(g_ignore_url.length());
    cout<<"g_ignore_url="<<g_ignore_url<<endl;
    cout<<"url="<<url<<endl;
    HttpHandlerUtils* utils = g_httphandler.Get(url);
    vector<http_handler_pt>::iterator it;
    if (utils && utils->handler)
        h = utils->handler;
    if (!h) 
    {
        req->finalizefunc(req->ctx, HTTP_NO_HANDLER);
        return HTTP_NO_HANDLER;
    }
    HttpRequest httpreq;
    httpreq.req = req;
    httpreq.handler_utils = utils;
    httpreq.uri = url;
    httpreq.method = req->method;
    httpreq.session.sess = &req->sess;
    size_t size = req->arglen;
    for (size_t i=0; i<size; ++i)
    {
        httpreq.args[req->args[i].name]=string(req->args[i].value.data, 
                req->args[i].value.length);
    }

    httpreq.body = &req->body;

    if (!g_httphandler.InFilter().Exec(httpreq.uri, &httpreq, utils->is_excat_filted))
        return 0;

    return h(httpreq);
}
