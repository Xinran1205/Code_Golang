package main

import "fmt"

func main() {
	EmpInter(10)
	//EmpInter("1")
	//TestInterface()
	//TestInheritance()
}

// 练习写一个万能接口 也叫empty interface， int,float,stuct,string等都实现了interface{}接口
// 可以回忆发现在自己写的HashMapImpl中就采用了interface{}写法
func EmpInter(arg interface{}) {
	fmt.Println(arg)
	//假如我们还需要根据arg的类型来做不同的操作
	//我们可以这样校验arg的类型
	value, ok := arg.(string)
	if ok {
		fmt.Println("value is String type", value)
	} else {
		//如果走到这里value的值其实为""空字符串，注意一下
		fmt.Printf("value is not String type")
	}
}

func TestInheritance() {
	//定义父类
	father := Hero{"WXR", "23"}

	//定义子类
	hero := Son{Hero{"wxr", "2"}, 1}
	//还有一种写法定义方法，我其实觉得上面这种好看一点
	var hero2 Son
	hero2.Age = "1"
	//hero2.Name = "wx"
	//hero2.Eat = 2
	if hero2.Name == "" && hero2.Eat == 0 {
		fmt.Println("没赋值应该就是当前类型默认值 字符串是空字符串，int是0")
	}

	father.Hello()
	hero.Hello()
	hero.HelloSon()
}

func TestInterface() {
	//接口本身他是一个指针，所以需要传地址
	var ani Animal
	ani = &Cat{"kittie"}
	ani.Eat()
}
