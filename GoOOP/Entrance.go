package main

import "fmt"

func main() {
	TestInterface()
	//TestInheritance()
}

func TestInheritance() {
	//定义父类
	father := Hero{"WXR", "23"}

	//定义子类
	hero := Human{Hero{"wxr", "2"}, 1}
	//还有一种写法定义方法，我其实觉得上面这种好看一点
	var hero2 Human
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
