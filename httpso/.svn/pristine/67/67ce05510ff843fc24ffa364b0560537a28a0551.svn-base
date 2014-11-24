#include <httpso.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
using namespace std;

static int handler(HttpRequest& req);
static void init_handler(SoFuncParamer&);

static int init_dummy = SoHandlerManager::SoInitFuncAdd(init_handler);

static void init_handler(SoFuncParamer& paramer)
{
    paramer.AddHandler("/test", handler);
}

static int handler(HttpRequest& req)
{
    req.SendData("hello world");
    return 0;
}
