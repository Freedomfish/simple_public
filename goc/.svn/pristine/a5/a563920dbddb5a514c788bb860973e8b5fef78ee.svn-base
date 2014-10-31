package main

import (
	"fmt"
	"net/http"
	"session"
	"strconv"
)

var GlobalSessions *session.Manager

func main() {
	GlobalSessions, _ = session.NewManager("memory", `{"cookieName":"gosessionid", "enableSetCookie,omitempty": true, "gclifetime":3600, "maxLifetime": -1, "secure": false, "sessionIDHashFunc": "sha1", "sessionIDHashKey": "", "cookieLifeTime": 0, "providerConfig": "127.0.0.1:6379"}`)
	if GlobalSessions == nil {
		fmt.Println("sess is nil")
		return
	}
	go GlobalSessions.GC()
	gs = make(map[int]string)
	HandlerStart(":8017")
}

func HandlerStart(port string) {
	http.HandleFunc("/login", login)
	http.HandleFunc("/count", count)
	err := http.ListenAndServe(port, nil)
	if nil != err {
		fmt.Println(err.Error())
	}
}

var gs map[int]string
var i int = 0

func login(w http.ResponseWriter, r *http.Request) {
	sess := GlobalSessions.SessionStart(w, r)
	defer sess.SessionRelease(w)
	str := ""
	uid := r.FormValue("uid")
	if uid == "" {
		str = "Login failed"
	}
	id, _ := strconv.Atoi(uid)
	sess.Set("uid", id)
	sess.Set("sessionid", fmt.Sprintf("%d", i))
	i++
	str = "Login success"
	gs[id] = sess.Get("sessionid").(string)
	fmt.Fprintf(w, str)
}

func count(w http.ResponseWriter, r *http.Request) {
	sess := GlobalSessions.SessionStart(w, r)
	defer sess.SessionRelease(w)
	ct := sess.Get("countnum")
	suid := sess.Get("uid")
	ssid := sess.Get("sessionid")
	if suid == nil || ssid == nil {
		fmt.Fprintf(w, "session check failed, please login again")
		return
	} else if sid, ok := gs[suid.(int)]; !ok || ssid != sid {
		fmt.Fprintf(w, "session check failed, please login again")
		return
	}
	if ct == nil {
		sess.Set("countnum", 1)
	} else {
		sess.Set("countnum", (ct.(int) + 1))
	}
	fmt.Fprintf(w, "countnum:", sess.Get("countnum"))
}
