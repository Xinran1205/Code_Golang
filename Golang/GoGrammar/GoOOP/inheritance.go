package main

import "fmt"

type Hero struct {
	Name string
	Age  string
}

type Son struct {
	//这个就是继承的关键！
	Hero //子类，继承了父类的属性和方法
	Eat  int
}

func (son *Son) HelloSon() {
	fmt.Println("This is son method")
}

func (hero *Hero) Hello() {
	fmt.Println(hero.Name + " " + hero.Age + " " + "says Hello world")
}
