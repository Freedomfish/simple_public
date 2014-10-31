package simini

import (
	"bufio"
	"io"
	"os"
	"strconv"
	"strings"
)

type StrMap map[string]string
type SimIni struct {
	sess_map_ map[string]StrMap
	loaded_   bool
	errmsg_   string
}

func (p *SimIni) IsLoaded() bool {
	return p.loaded_
}

func (p *SimIni) ErrMsg() string {
	return p.errmsg_
}

func (p *SimIni) LoadFile(filename string) int {
	fd, err := os.Open(filename)
	if nil != err {
		p.errmsg_ = err.Error()
		return 1
	}
	defer fd.Close()
	p.sess_map_ = make(map[string]StrMap)
	buf := bufio.NewReader(fd)
	curkey := ""
	for {
		line, err := buf.ReadString('\n')
		if io.EOF == err {
			break
		}
		line = strings.TrimLeft(line, " ")
		if 0 == len(line) || '#' == line[0] {
			continue
		}
		length := len(line)
		if '[' == line[0] && ']' == line[length-2] {
			curkey = line[1 : length-2]
			p.sess_map_[curkey] = make(StrMap)
			continue
		}
		if curkey == "" {
			p.errmsg_ = "lack of []"
			return 1
		}
		val := strings.SplitN(line, "=", 2)
		if 0 == len(val) || 0 == len(val[0]) {
			continue
		}
		v := strings.TrimLeft(val[1], " ")
		p.sess_map_[curkey][strings.TrimRight(val[0], " ")] = v[0 : len(v)-1]
	}
	p.loaded_ = true
	return 0
}

func (p *SimIni) GetStringVal(sess, key string) string {
	sv, sok := p.sess_map_[sess]
	if sok {
		v, ok := sv[key]
		if ok {
			return v
		}
	}
	return ""
}

func (p *SimIni) GetStringValWithDefault(sess, key, default_v string) string {
	str := p.GetStringVal(sess, key)
	if str == "" {
		str = default_v
	}
	return str
}

func (p *SimIni) GetIntVal(sess, key string) (int, error) {
	str := p.GetStringVal(sess, key)
	if str == "" {
		return 0, nil
	}
	return strconv.Atoi(str)
}

func (p *SimIni) GetIntValWithDefault(sess, key string, default_v int) (int, error) {
	str := p.GetStringVal(sess, key)
	if str == "" {
		return default_v, nil
	}
	return strconv.Atoi(str)
}

func (p *SimIni) GetSession(sess string) StrMap {
	strmap := make(StrMap)
	sv, sok := p.sess_map_[sess]
	if sok {
		for k, v := range sv {
			strmap[k] = v
		}
	}
	return strmap
}
