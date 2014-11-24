package main

import (
	"fmt"
	"log"
	"net/http"
	"net/http/httputil"
	"net/url"
)

var proxymap map[string]*ProxyInfo
var init_flag bool

type ProxyInfo struct {
	url      string
	realAddr string
	handler  func(*http.Request)
}

func Init() {
	if !init_flag {
		proxymap = make(map[string]ProxyInfo)
		init_flag = true
	}
}

func NewProxyInfo(uri, realaddr string, handler func(*http.Request)) *ProxyInfo {
	return &ProxyInfo{uri, realaddr, handler}
}

func (p *ProxyInfo) Init() {
	Init()
	addr := url.Parse(p.RealAddr)
	proxymap[p.uri] = p
}

func init() {
	Init()
}

func ListenAndProxy(host string) {
	for uri, p := range proxymap {
		addr := url.Parse(p.RealAddr)
		ph := httputil.NewSingleHostReverseProxy(addr)
		http.HandleFunc(p.Url, handler(ph, p.handler))
	}
	err := http.ListenAndServe(host, nil)
	if nil != err {
		panic(err)
	}
}
func handler(p *httputil.ReverseProxy, h func(*http.Request)) func(http.ResponseWriter, *http.Request) {
	return func(w http.ResponseWriter, r *http.Request) {
		log.Println(r.URL)
		h(r)
		p.ServeHTTP(w, r)
	}
}

/*
func main() {
    remote, err := url.Parse("http://127.0.0.1:8011")
    if err != nil {
            panic(err)
    }

    proxy := httputil.NewSingleHostReverseProxy(remote)
    //http.HandleFunc("/", handler(proxy))
	h := http.HandlerFunc(handler(proxy))
    err = http.ListenAndServe(":8012", h)
    if err != nil {
            panic(err)
    }
}
*/
