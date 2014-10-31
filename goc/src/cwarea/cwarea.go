package cwarea

import (
	"bufio"
	"code.google.com/p/goprotobuf/proto"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"
	"sync"
)

const (
	RetSuccess   = 0
	RetNoInitErr = 1
	RetNoDataErr = 2
	RetFileErr   = 3
	RetParamErr  = 4
	RetLevelErr  = 5
)

type indexArr []int
type CodeAreaMap map[string]*AreaInfo
type NameAreaMap map[string]*AreaInfo

type CodeSubAreaIndexArrPtrMap map[string]*indexArr
type NameSubAreaIndexArrPtrMap map[string]*indexArr

var area_arr []AreaInfo

var code_area_map CodeAreaMap
var name_area_map NameAreaMap
var code_subarea_map_ [2]CodeSubAreaIndexArrPtrMap //code_subarea_map_[level-1]
var name_subarea_map_ [2]NameSubAreaIndexArrPtrMap

var initflag bool
var rlock *sync.RWMutex = new(sync.RWMutex)

//文件名， 地区数量(0表示最大数量)， 起始行号
func Init(filename string, areanum, startline int) bool {
	//如果当前正在操作过程中或在查询过程中
	if initflag {
		return true
	}
	rlock.Lock()
	defer rlock.Unlock()

	if !initflag {
		areanum = areanum
		if 0 != areanum {
			area_arr = make([]AreaInfo, areanum)
		}
		code_area_map = make(CodeAreaMap, areanum)
		name_area_map = make(NameAreaMap, areanum)

		code_subarea_map_[0] = make(CodeSubAreaIndexArrPtrMap, areanum)
		name_subarea_map_[0] = make(NameSubAreaIndexArrPtrMap, areanum)
		code_subarea_map_[1] = make(CodeSubAreaIndexArrPtrMap, areanum)
		name_subarea_map_[1] = make(NameSubAreaIndexArrPtrMap, areanum)
		loadfile(filename, areanum, startline)
		initflag = true
	}

	return true
}

func Destroy() bool {
	if !initflag {
		return true
	}
	rlock.Lock()
	defer rlock.Unlock()

	if initflag {
		area_arr = nil
		code_area_map = nil
		name_area_map = nil
		initflag = false
	}
	return true
}

func loadfile(filename string, areanum, startline int) error {
	CodeFile := filename
	fd, err := os.Open(CodeFile)
	if nil != err {
		fmt.Printf("open codefile error!\n")
		os.Exit(-1)
		return err
	}
	defer fd.Close()
	read := bufio.NewReader(fd)
	for i := 0; 0 == areanum || i < areanum; i++ {
		line, err := read.ReadString('\n')
		if io.EOF == err {
			break
		}
		if i < startline {
			continue
		}
		if strings.Contains(line, "|") {
			s := strings.Split(line, "|")

			code := s[0]
			name := s[2]

			var tmp *AreaInfo
			if 0 == areanum {
				var areainfo AreaInfo
				area_arr = append(area_arr, areainfo)
			}
			tmp = &area_arr[i]
			code_area_map[code] = tmp
			name_area_map[name] = tmp

			tmp.Code = proto.String(s[0])
			tmp.Name = proto.String(s[1])
			tmp.Fullname = proto.String(s[2])
			tmp.Code1 = proto.String(s[3])
			tmp.Code2 = proto.String(s[4])
			tmp.Code3 = proto.String(s[5])
			tmp.Code4 = proto.String(s[6])
			tmp.Code5 = proto.String(s[7])
			tmp.Level = proto.String(s[8])
			tmp.Levelname = proto.String(s[9])
			if "1" == s[8] {
				iarr := new(indexArr)
				code_subarea_map_[0][code] = iarr
				name_subarea_map_[0][name] = iarr
			} else if "2" == s[8] {
				//防止错误数据导致程序崩溃，所以所有map均做一次判断
				indexarr, ok := code_subarea_map_[0][code[0:2]]
				if ok {
					*indexarr = append(*indexarr, i)
				}
				iarr := new(indexArr)
				code_subarea_map_[1][code] = iarr
				name_subarea_map_[1][name] = iarr
			} else if "3" == s[8] {
				//做判断是因为含有省直辖县级行政单位或错误数据,如429004的仙桃市
				indexarr, ok := code_subarea_map_[1][code[0:4]]
				if ok {
					*indexarr = append(*indexarr, i)
				} else {
					specialAreaDeal(&s, i)
				}
			}

		}
	}
	return nil
}

func specialAreaDeal(s *[]string, index int) {
	//先根据code找到该省，
	//然后再根据该省的名字和该地区的名字组好全名
	//根据全名在name_area_map中查找关联
	areainfo, ok := code_area_map[(*s)[0][0:2]]
	if ok {
		//省名
		firstname := areainfo.GetFullname()
		//要查找对象全名
		fullname := firstname + (*s)[1]
		//查找上级记录
		indexarr, ok := name_subarea_map_[1][fullname]
		//将该条记录添加到上级记录中
		if ok {
			println("special area: ", fullname)
			*indexarr = append(*indexarr, index)
		}
	}
}

func GetAreainfoByCode(code string) (int, *AreaInfo) {
	if !initflag {
		return RetNoInitErr, nil
	}
	rlock.RLock()
	defer rlock.RUnlock()
	value, ok := code_area_map[code]
	if !ok {
		return RetNoDataErr, nil
	}
	areainfo := *value
	return 0, &areainfo
}

func GetAreainfoByName(name string) (int, *AreaInfo) {
	if !initflag {
		return RetNoInitErr, nil
	}
	rlock.RLock()
	defer rlock.RUnlock()
	value, ok := name_area_map[name]
	if !ok {
		return RetNoDataErr, nil
	}
	areainfo := *value
	return 0, &areainfo
}

func GetAreaArrsByIndexs(indexarr *indexArr) []*AreaInfo {
	resparr := make([]*AreaInfo, len(*indexarr))
	for i, index := range *indexarr {
		area := area_arr[index]
		resparr[i] = &area
	}
	return resparr
}

func getSubAreaByCode(code string, level int) (int, []*AreaInfo) {
	if !initflag {
		return RetNoInitErr, nil
	}
	rlock.RLock()
	defer rlock.RUnlock()
	if 0 == level {
		level = len(code) / 2
	}
	switch level {
	case 1:
	case 2:
	default:
		return RetLevelErr, nil
	}
	ia, ok := name_subarea_map_[level-1][code]
	if !ok || 0 == len(*ia) {
		return RetNoDataErr, nil
	}
	return 0, GetAreaArrsByIndexs(ia)
}

func getSubAreaByName(name string, level int) (int, []*AreaInfo) {
	if !initflag {
		return RetNoInitErr, nil
	}
	rlock.RLock()
	defer rlock.RUnlock()
	if 0 == level {
		_, area := GetAreainfoByName(name)
		if nil == area {
			return RetNoDataErr, nil
		}
		level, _ = strconv.Atoi(area.GetLevel())
	}
	switch level {
	case 1:
	case 2:
	default:
		return RetLevelErr, nil
	}
	ia, ok := name_subarea_map_[level-1][name]
	if !ok || 0 == len(*ia) {
		return RetNoDataErr, nil
	}
	return 0, GetAreaArrsByIndexs(ia)
}

func GetSubAreaByCode(code string, level int) (int, *SubAreaInfo) {
	var subareainfo SubAreaInfo
	var ret int
	ret, subareainfo.Subareas = getSubAreaByCode(code, level)
	return ret, &subareainfo
}

func GetSubAreaByName(name string, level int) (int, *SubAreaInfo) {
	var subareainfo SubAreaInfo
	var ret int
	ret, subareainfo.Subareas = getSubAreaByName(name, level)
	return ret, &subareainfo
}

func getAllAreaBySubAreas(subareas []*AreaInfo, level int) (int, *AllAreaInfo) {
	var allarea AllAreaInfo
	var ret int
	if 2 == level {
		allarea.Subareas = make([]*SubAreaInfo, 1)
		allarea.Subareas[0].Subareas = subareas
	} else if 1 == level {
		if nil != subareas {
			allarea.Subareas = make([]*SubAreaInfo, len(subareas))
			for i, areaptr := range subareas {
				var subarea SubAreaInfo
				subarea.ParentArea = areaptr
				ret, subarea.Subareas = getSubAreaByCode(areaptr.GetCode(), 2)
				allarea.Subareas[i] = &subarea
			}
		} else {
			return RetNoDataErr, nil
		}
	} else {
		return RetNoDataErr, nil
	}

	return ret, &allarea
}

func GetAllAreaByCode(code string, level int) (int, *AllAreaInfo) {
	if !initflag {
		return RetNoInitErr, nil
	}
	rlock.RLock()
	defer rlock.RUnlock()
	if 0 == level {
		level = len(code) / 2
	}
	ret, subareas := getSubAreaByCode(code, level)

	if RetSuccess != ret {
		return ret, nil
	}

	return getAllAreaBySubAreas(subareas, level)
}

func GetAllAreaByName(name string, level int) (int, *AllAreaInfo) {
	if !initflag {
		return RetNoInitErr, nil
	}
	rlock.RLock()
	defer rlock.RUnlock()
	if 0 == level {
		_, area := GetAreainfoByName(name)
		if nil == area {
			return RetNoDataErr, nil
		}
		level, _ = strconv.Atoi(area.GetLevel())
	}
	ret, subareas := getSubAreaByName(name, level)

	if RetSuccess != ret {
		return ret, nil
	}

	return getAllAreaBySubAreas(subareas, level)
}
