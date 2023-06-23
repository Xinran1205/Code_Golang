package main

import (
	"fmt"
	"reflect"
)

// 测试函数
func main() {
	//Go语言的变量是一个pair，pair中有两个值，type和value
	//pair<type:, value:>
	//反射的意义，动态的获取出未知变量的type和value

	//var num int = 10
	//num := 10
	//relectNum(num)

	//user := User{123, "wxr"}
	//DoFieldAndMethod(user)

	//测试结构体标签
	//var a User2
	//GetTag(&a)

	//测试结构体标签转换成json 和 json转换成结构体
	com := Computer{"apple", 123, []string{"王欣然", "乔布斯"}}
	Convert(com)
}

// go语言反射包
// valueof用来动态获取输入参数接口中的数据的值，如果接口为空则返回0
// typeof用来动态获取输入参数接口中的值的类型，如果接口为空则返回nil
// 简单的例子
func relectNum(arg interface{}) {
	fmt.Println("type:", reflect.TypeOf(arg))
	fmt.Println("value:", reflect.ValueOf(arg))
}

// 复杂的例子
type User struct {
	Id   int
	Name string
}

func (user User) Hello() {
	fmt.Printf("user is %v\n", user)
}

//这句话非常重要，非常重要，非常重要 重要的事情说三遍
//为什么要用反射，就是因为我们的入参是interface，我们不能系统的处理它
//所以需要使用反射来处理

func DoFieldAndMethod(arg interface{}) {
	//获取input的type和value
	inputType := reflect.TypeOf(arg)
	fmt.Println("type:", inputType.Name(), inputType.Kind(), inputType)
	inputValue := reflect.ValueOf(arg)
	fmt.Println("value:", inputValue)

	//通过type获取input的字段
	//NumField有多少个字段
	for i := 0; i < inputType.NumField(); i++ {
		field := inputType.Field(i)
		fmt.Println("field:", field)
		value := inputValue.Field(i).Interface()
		fmt.Println("value:", value)
	}

	//通过type获取input的方法，调用
	for i := 0; i < inputType.NumMethod(); i++ {
		method := inputType.Method(i)
		fmt.Println("method:", method)
		//调用方法
		//方法的参数也是通过反射获取
		args := []reflect.Value{reflect.ValueOf(arg)}
		method.Func.Call(args)
	}
}
