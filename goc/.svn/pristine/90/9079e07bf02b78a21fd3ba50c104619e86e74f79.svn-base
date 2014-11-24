package main

//import proto "code.google.com/p/goprotobuf/proto"

import (
	"cwarea"
	"fmt"
)

func main() {
	fmt.Printf("test program\n")
	cwarea.Init("a.txt", 3373, 1)
	//	cwarea.Test()
	i, areainfo := cwarea.GetAreainfoByCode("8207")
	j, nameinfo := cwarea.GetAreainfoByName("湖北省黄冈市")
	k, areainfos := cwarea.GetSubAreaByCode("4211", 2)
	l, areainfos2 := cwarea.GetSubAreaByName("湖北省仙桃市", 0)
	m, areainfos3 := cwarea.GetSubAreaByName("湖北省", 0)
	n, areainfos4 := cwarea.GetAllAreaByName("湖北省", 0)
	fmt.Println("i=", i, areainfo.GetFullname())
	fmt.Println("j=", j, nameinfo.GetCode())
	fmt.Printf("k=%d ", k)
	for _, areainfo := range areainfos.Subareas {
		fmt.Println(areainfo.GetFullname())
	}
	fmt.Printf("l=%d ", l)
	for _, areainfo := range areainfos2.Subareas {
		fmt.Println(areainfo.GetFullname())
	}
	fmt.Println()
	fmt.Printf("m=%d ", m)
	for _, areainfo := range areainfos3.Subareas {
		fmt.Println(areainfo.GetFullname())
	}
	fmt.Println()
	fmt.Printf("n=%d ", n)
	fmt.Println("len(Subareas)=", len(areainfos4.Subareas))
	for _, areainfos := range areainfos4.Subareas {
		fmt.Println(areainfos.ParentArea.GetFullname())
		for _, areainfo := range areainfos.Subareas {
			fmt.Println(areainfo.GetFullname())
		}
	}
	fmt.Println()
}
