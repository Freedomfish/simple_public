#include "so_interface.hpp"
#include "http_interface.hpp"
#include "http_entry.hpp"
#include <sim/so_handler.h>
using namespace std;
using namespace sim;


static SoLoad soload;
static void init_handler(void* );
static void exit_handler(void* );
extern HttpHandlerManager g_httphandler;
static int init_dummy = g_httphandler.AddInitHandler(init_handler);
static map<string, vector<void*>> sofuncs;

static void init_handler(void* paramer)
{
	LOG_ERROR("%s|%s|%d\n", __FILE__, __FUNCTION__, __LINE__);
	g_httphandler.AddExitHandler(exit_handler);
	const char* path = ((SoFuncParamer*)paramer)->path().c_str();
	vector<string> symbols;
	symbols.push_back(SO_LOAD_FUNC);
	symbols.push_back(SO_UNLOAD_FUNC);
        LOG_ERROR("path=%s\n", path);
	if (soload.FuncLoadFromPath(path, symbols, sofuncs))
	{
		LOG_ERROR("errcode:%d|errmsg:%s\n", 
				soload.errcode(),soload.errmsg().c_str());
		return;
	}
	LOG_ERROR("%s|%s|%d\n", __FILE__, __FUNCTION__, __LINE__);
        vector<void*>& loadfuncs = sofuncs[SO_LOAD_FUNC];
	LOG_ERROR("%s|%s|%d|loadfuncs.size:%ld\n", 
			__FILE__, __FUNCTION__, __LINE__, loadfuncs.size());
	vector<void*>::iterator it;
	for (it=loadfuncs.begin(); it!=loadfuncs.end(); ++it)
        ((sofunc_load_pt)(*it))(paramer);
}

static void exit_handler(void* paramer)
{
    vector<void*>& loadfuncs = sofuncs[SO_UNLOAD_FUNC];
	vector<void*>::iterator it;
	for (it=loadfuncs.begin(); it!=loadfuncs.end(); ++it)
        ((sofunc_unload_pt)(*it))(paramer);
}

