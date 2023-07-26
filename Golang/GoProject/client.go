package main

import (
	"flag"
	"fmt"
	"io"
	"net"
	"os"
)

type client struct {
	SeverIp   string
	SeverPort string
	Name      string
	conn      net.Conn
	flag      int
}

func NewClient(Ip string, Port string) *client {
	cli := &client{
		SeverIp:   Ip,
		SeverPort: Port,
		flag:      999,
	}
	conn, err := net.Dial("tcp", fmt.Sprintf("%s:%s", Ip, Port))
	if err != nil {
		fmt.Println("net.Dial err:", err)
		return nil
	}
	cli.conn = conn
	return cli
}

func (c *client) menu() bool {
	var flag int
	fmt.Println("Type 1 open public chat mode")
	fmt.Println("Type 2 open private chat mode")
	fmt.Println("Type 3 update username")
	fmt.Println("Type 0 exit")
	//当使用 fmt.Scanln(&flag) 从标准输入中读取数据时，如果输入的数据无法解析为 int 类型，那么 flag 变量将会被赋值为 0。
	//这是因为在 Go 中，如果无法将一个字符串解析为一个有效的数值类型（比如 int、float 等），
	//那么对应的变量将会被赋值为该类型的默认值。对于 int 类型来说，其默认值是 0。
	fmt.Scanln(&flag)
	if flag >= 0 && flag <= 3 {
		c.flag = flag
		return true
	} else {
		fmt.Println(">>>>>>Please enter correct type<<<<<<")
		return false
	}
}

func (c *client) ChooseMod() {
	for c.flag != 0 {
		for c.menu() != true {
		}
		switch c.flag {
		case 1:
			fmt.Println("public chat mode")
			c.PublicChat()
		case 2:
			fmt.Println("private chat mode")
			c.PrivateChat()
		case 3:
			fmt.Println("update username")
			c.UpdateName()
		}
	}
}

func (c *client) UpdateName() bool {
	fmt.Println(">>>>>>Please enter username<<<<<<")
	fmt.Scanln(&c.Name)
	sendMsg := "rename " + c.Name + "\n"
	_, err := c.conn.Write([]byte(sendMsg))
	if err != nil {
		fmt.Println("conn.Write err:", err)
		return false
	}
	return true
}

func (c *client) PublicChat() {
	var chatMsg string
	fmt.Println(">>>>>>Please enter chat content,exit type exit<<<<<<")
	fmt.Scanln(&chatMsg)
	for chatMsg != "exit" {
		if len(chatMsg) != 0 {
			sendMsg := chatMsg + "\n"
			_, err := c.conn.Write([]byte(sendMsg))
			if err != nil {
				fmt.Println("conn.Write err:", err)
				break
			}
		}
		chatMsg = ""
		fmt.Println(">>>>>>Please enter chat content,exit type exit<<<<<<")
		fmt.Scanln(&chatMsg)
	}
}

func (c *client) PrivateChat() {
	var chatMsg string
	var remoteName string
	c.SelectUsers()
	fmt.Println(">>>>>>Please enter remote username,exit type exit<<<<<<")
	fmt.Scanln(&remoteName)
	//这里其实不校验remoteName也可以，因为后面发给server端，server端会校验
	for remoteName != "exit" {
		fmt.Println(">>>>>>Please enter chat content,exit type exit<<<<<<")
		fmt.Scanln(&chatMsg)
		for chatMsg != "exit" {
			if len(chatMsg) != 0 {
				sendMsg := "to " + remoteName + " " + chatMsg + "\n"
				_, err := c.conn.Write([]byte(sendMsg))
				if err != nil {
					fmt.Println("conn.Write err:", err)
					break
				}
			}
			chatMsg = ""
			fmt.Println(">>>>>>Please enter chat content,exit type exit<<<<<<")
			fmt.Scanln(&chatMsg)
		}
		c.SelectUsers()
		fmt.Println(">>>>>>Please enter remote username,exit type exit<<<<<<")
		fmt.Scanln(&remoteName)
	}
}

// 查询在线用户
func (c *client) SelectUsers() {
	sendMsg := "who is online\n"
	_, err := c.conn.Write([]byte(sendMsg))
	if err != nil {
		fmt.Println("conn.Write err:", err)
		return
	}
}

// 这个函数需要开一个go程永久阻塞监听
func (c *client) dealServerResponse() {
	//一旦client.conn有数据，就直接copy到stdout标准输出上
	io.Copy(os.Stdout, c.conn)
}

var serverIp string
var serverPort string

// init函数在main函数执行前执行
// ./client -ip 127.0.0.1 -port 8888   这个是flag.StringVar的格式
func init() {
	flag.StringVar(&serverIp, "ip", "127.0.0.1", "set server Ip address(default 127.0.0.1)")
	flag.StringVar(&serverPort, "port", "8888", "set server port number(default 8888)")
}

func main() {
	client := NewClient(serverIp, serverPort)
	if client == nil {
		fmt.Println("error>>>>>>")
		return
	}
	fmt.Println("connection success")

	go client.dealServerResponse()

	client.ChooseMod()
}
