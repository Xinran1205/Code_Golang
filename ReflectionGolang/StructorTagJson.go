package main

import (
	"encoding/json"
	"fmt"
)

type Computer struct {
	//所以我们转成json它就是小写的
	Brand  string   `json:"brand"`
	Id     int      `json:"id"`
	People []string `json:"people"`
}

func Convert(arg interface{}) {
	//把结构体转成json
	jsonStr, err := json.Marshal(arg)
	if err != nil {
		fmt.Println("json marshal failed")
		return
	}
	fmt.Println(string(jsonStr))

	//解码，把json转成结构体
	// jsonStr := `{"brand":"apple","id":123,"people":["王欣然","乔布斯"]}`
	myCom := Computer{}
	err = json.Unmarshal([]byte(jsonStr), &myCom)
	if err != nil {
		fmt.Println("json unmarshal failed")
		return
	}
	fmt.Println(myCom)
}
