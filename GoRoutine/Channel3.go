package main

import (
	"fmt"
)

// 关于channel的关闭有一些注意事项
// 1.channel不像文件一样需要经常去关闭，只有当你确实没有任何发送数据了，或者你想显式的结束range循环之类的，才去关闭channel
// 2.关闭channel后，无法再发送数据，但是可以继续接收数据
// 3.对于nil channel，无论收发都会阻塞 这个一般用不到
func main() {
	c := make(chan int)

	go func() {
		for i := 0; i < 5; i++ {
			c <- i
		}
		//close可以关闭一个channel
		//如果没有这个close会报一个error：fatal error: all goroutines are asleep - deadlock!
		close(c)
	}()

	//for {
	//	//如果ok为true表示channel没有关闭，如果为false表示channel已经关闭
	//	//这个表达式写法可以注意一下,data和ok是局部变量
	//	if data, ok := <-c; ok == true {
	//		println(data)
	//	} else {
	//		break
	//	}
	//}

	//用这个替代上述代码，这个更简单易懂，使用range来迭代不断操作channel
	for data := range c {
		println(data)
	}

	fmt.Println("main结束")
}
