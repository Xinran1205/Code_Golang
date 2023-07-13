//参考课件
//https://subingwen.cn/linux/select/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main(){

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket");
        exit(0);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY; 
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1){
        perror("bind");
        exit(0);
    }

    ret = listen(fd, 128);
    if(ret == -1){
        perror("listen");
        exit(0);
    }

    // 将监听的fd的状态检测委托给内核检测
    int maxfd = fd;
    // 初始化检测的读集合
    fd_set rdset;
    fd_set rdtemp;
    // 清零
    FD_ZERO(&rdset);
    // 将监听的fd设置到检测的读集合中
    FD_SET(fd, &rdset);
    // 通过select委托内核检测读集合中的文件描述符状态, 检测read缓冲区有没有数据
    // 如果有数据, select解除阻塞返回
    // 应该让内核持续检测

    while(1){
        // 默认阻塞
        // rdset 中是委托内核检测的所有的文件描述符
        rdtemp = rdset;
        int num = select(maxfd+1, &rdtemp, NULL, NULL, NULL);
        // rdset中的数据被内核改写了, 只保留了发生变化的文件描述的标志位上的1, 没变化的改为0
        // 只要rdset中的fd对应的标志位为1 -> 缓冲区有数据了
        // 判断
        // 有没有新连接
        if(FD_ISSET(fd, &rdtemp)){
            // 有新连接
            struct sockaddr_in client_addr;
            socklen_t len = sizeof(client_addr);
            //这里不会阻塞，因为select
            int com_fd = accept(fd, (struct sockaddr*)&client_addr, &len);

            char ip[24] = {0};
            printf("客户端的IP地址: %s, 端口: %d\n",
                   inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip, sizeof(ip)),
                   ntohs(client_addr.sin_port));

            // 将新的文件描述符设置到检测的读集合中
            FD_SET(com_fd, &rdset);
            // 更新最大的文件描述符
            maxfd = maxfd > com_fd ? maxfd : com_fd;
        }
        for(int i=0; i<maxfd+1; ++i)
        {
			// 判断从监听的文件描述符之后到maxfd这个范围内的文件描述符是否读缓冲区有数据
            if(i != fd && FD_ISSET(i, &rdtemp)){
                char buf[1024];
                memset(buf, 0, sizeof(buf));
                int len = recv(i, buf, sizeof(buf), 0);
                //这里我们确实不需要检测写缓冲区是否有空间, 因为我们是回射服务器, 走进去len一定大于0
                if(len > 0)
                {
                    printf("客户端say: %s\n", buf);
                    send(i, buf, len, 0);
                }
                else if(len  == 0)
                {
                    printf("客户端断开了连接...\n");
                    FD_CLR(i, &rdset);
                    close(i);
                }
                else
                {
                    perror("read");
                }
            }
        }
    }
    return 0;
}
