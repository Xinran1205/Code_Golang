package main

import "fmt"

// 这个就是接口的两个方法，
// 那么一个类想要实现这个接口需要Implement这个接口的所有方法
// 不继承所有方法就会报错，这是一种多态的思想
type Animal interface {
	Cry()
	Eat()
}

// 结构体就把理解成类的概念
// 类1
type Cat struct {
	Name string
}

// 类2
type Dog struct {
	Name string
}

func (cat *Cat) Cry() {
	fmt.Println(cat.Name + " " + "cat is crying")
}

func (cat *Cat) Eat() {
	fmt.Println(cat.Name + " " + "cat is eating")
}

func (dog *Dog) Cry() {
	fmt.Println(dog.Name + " " + "dog is crying")
}

func (dog *Dog) Eat() {
	fmt.Println(dog.Name + " " + "dog is eating")
}
