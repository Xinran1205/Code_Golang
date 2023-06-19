package main

import (
	"fmt"
	"reflect"
)

//结构体标签的应用：json编解码，orm映射关系
//对象关系映射（Object Relational Mapping，简称ORM）模式是一种为了解决面向对象与关系数据库存在的互不匹配的现象的技术。
//    简单的说，ORM是通过使用描述对象和数据库之间映射的元数据，将程序中的对象自动持久化到关系数据库中。
//    ORM在业务逻辑层和数据库层之间充当了桥梁的作用。

// 要非常注意，结构体标签的`是1旁边的符号，不是单引号
type User2 struct {
	Id string `json:"id"` //这个json是结构体标签，用来给字段打标签的，id是标签的名字，
	// 这个标签因为我们还没用，所以可以随便写什么名字，但是假如我们要把数据转换成json的时候，标签需要写json，因为要调用json库
	//标签的名字就是转换成json时的名字
	Name string `json:"name"`
}

func GetTag(arg interface{}) {
	t := reflect.TypeOf(arg).Elem()
	for i := 0; i < t.NumField(); i++ {
		tag := t.Field(i).Tag.Get("json")
		fmt.Println(tag)
	}
}
