#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//这个头文件是为了使用socket函数
#include <arpa/inet.h>

int main(){
    //1.创建监听的套接字，socket函数
    // 参数1：使用的协议族，AF_INET表示使用ipv4协议族(AF_INET6: 使用IPv6格式的ip地址) 
    // 参数2:使用的套接字类型，SOCK_STREAM表示使用的是tcp协议
    // 参数3:使用的协议，0表示使用默认的协议
    // 返回值：可用于套接字通信的文件描述符

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket");
        exit(0);
    }

    //2.把这个监听的套接字和本地的IP和端口绑定，bind函数
    // 参数1:sockfd: 监听的文件描述符, 通过socket()调用得到的返回值
    // 参数2:addr: 传入参数, 要绑定的IP和端口信息需要初始化到这个结构体中，IP和端口要转换为网络字节序
    // 参数3:addrlen: 参数addr指向的内存大小, sizeof(struct sockaddr)
    // 返回值：成功返回0，失败返回-1
    //这里要注意，当我们使用bind函数时，肯定要先给addr结构体赋值，
    //这个结构体是const struct sockaddr *类型，但是我们为了方便，给struct sockaddr_in *类型赋值，再把它进行强转

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    // INADDR_ANY,它的值是0，代表本机的所有IP, 假设有三个网卡就有三个IP地址
    // 这个宏可以代表任意一个IP地址
    // 这个宏一般用于本地的绑定操作
    addr.sin_addr.s_addr = INADDR_ANY; 
    //这里用了强转
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1){
        perror("bind");
        exit(0);
    }

    //3.开始监听，listen函数
    // 参数1:sockfd: 文件描述符, 可以通过调用socket()得到，在监听之前必须要绑定 bind()
    // 参数2:backlog: 同时能处理的最大连接要求，最大值为128
    // 返回值：函数调用成功返回0，调用失败返回 -1

    ret = listen(fd, 128);
    if(ret == -1){
        perror("listen");
        exit(0);
    }

    //4.阻塞并等待客户端连接，accept函数
    // 参数1:sockfd: 监听的文件描述符
    // 参数2:addr: 传出参数, 里边存储了建立连接的客户端的地址信息
    // 参数3:addrlen: 传入传出参数，用于存储addr指向的内存大小
    // 返回值：函数调用成功，得到一个文件描述符, 用于和建立连接的这个客户端通信，调用失败返回 -1
    // 这个函数是一个阻塞函数，当没有新的客户端连接请求的时候，该函数阻塞；当检测到有新的客户端连接请求时，
    // 阻塞解除，新连接就建立了，得到的返回值也是一个文件描述符，基于这个文件描述符就可以和客户端通信了。
    // 回忆communication.png！这里创建用来通信的套接字，每来一个客户端就创建一个，上面的监听套接字还是只有一个

    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int com_fd = accept(fd, (struct sockaddr*)&client_addr, &len);
    if(com_fd == -1){
        perror("accept");
        exit(0);
    }

    //5.连接建立成功，开始通信
    //先打印一下客户端信息
    //这是定义一个 char 类型的数组 ip，大小为 24 字节，并将数组中的每个元素都初始化为 0
    char ip[24] = {0};
    printf("客户端的IP地址: %s, 端口: %d\n",
           inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip, sizeof(ip)),
           ntohs(client_addr.sin_port));

    while(1){
        char buf[1024];
        //在 C 语言中，如果定义数组时没有为其赋初值，那么数组中的元素将会是未定义的（uninitialized），
        //也就是说，其值是不确定的，可能是任何值（包括 0）。
        //这些未初始化的值可能是之前存储在该内存位置上的值，也可能是随机的垃圾值。
        //memset 函数对该数组进行了初始化，将其所有元素都设置为 0。
        memset(buf, 0, sizeof(buf));
        //这里用read也可以
        int len = recv(com_fd, buf, sizeof(buf), 0);
        if(len > 0)
        {
            printf("客户端say: %s\n", buf);
            //同样这里也可以写成write，这里就是将客户端发来的数据原样发回去
            send(com_fd, buf, len, 0);
        }
        else if(len  == 0)
        {
            printf("客户端断开了连接...\n");
            break;
        }
        else
        {
            perror("read");
            break;
        }
    }

    //6.关闭文件描述符 监听的+通信的（可能很多个），close函数

    close(fd);
    close(com_fd);
    return 0;
}
