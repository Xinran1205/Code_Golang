//写一个基于多进程（fork）的服务器
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
  

int worker(int clifd);

//子进程先于父进程退出，同时父进程太忙了，无瑕回收子进程的资源，
//子进程残留资源（PCB）存放于内核中，变成僵尸（Zombie）进程，

//这个函数和后面信号捕捉搭配可以避免子进程成为僵尸进程
//为了后面用于处理子进程退出时发送的SIGCHLD信号。
void callback(int num)
{
    while(1)
    {
        pid_t pid = waitpid(-1, NULL, WNOHANG);
        if(pid <= 0)
        {
            printf("子进程正在运行, 或者子进程被回收完毕了\n");
            break;
        }
        printf("child die, pid = %d\n", pid);
    }
}

int main(){

    int fatherfd = socket(AF_INET, SOCK_STREAM, 0);
    if(fatherfd == -1){
        perror("socket");
        exit(0);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY; 
    int ret = bind(fatherfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1){
        perror("bind");
        exit(0);
    }

    ret = listen(fatherfd, 128);
    if(ret == -1){
        perror("listen");
        exit(0);
    }

    //注册信号的捕捉
    //通过注册SIGCHLD信号的捕捉函数，可以在子进程退出时及时地处理其退出信息，避免子进程成为僵尸进程。
    //具体来说，当子进程退出时，内核会向父进程发送一个SIGCHLD信号，
    //父进程会调用注册的信号捕捉函数进行处理，该函数可以通过waitpid或wait等函数来获得子进程的退出状态并进行善后处理，
    //例如释放子进程占用的资源等。
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = callback;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, NULL);

    while(1){
        struct sockaddr_in cliaddr;
        int clilen = sizeof(cliaddr);
        int clifd = accept(fatherfd, (struct sockaddr*)&cliaddr, &clilen);

        if(clifd == -1){
            //多写一个这个也是为了配合信号的捕捉，不写直接服务器退出了
            if(errno == EINTR)
            {
                printf("errno = %d\n", errno);
                // accept调用被信号中断了, 解除阻塞, 返回了-1
                // 重新调用一次accept
                continue;
            }
            perror("accept");
            exit(0);
 
        }

        char ip[24] = {0};
        printf("客户端的IP地址: %s, 端口: %d\n",
           inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip)),
           ntohs(cliaddr.sin_port));

        pid_t pid = fork();
        //当pid是子进程
        if (pid==0){
            close(fatherfd);
            int ret = worker(clifd);
            // 退出子进程
            close(clifd);
            exit(0);
        }
        if(pid==1){
            close(clifd);
        } 
    }
    return 0;
}

int worker(int clifd){
    while(1){
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        int len = recv(clifd, buf, sizeof(buf), 0);
        if(len > 0)
        {
            printf("客户端say: %s\n", buf);
            send(clifd, buf, len, 0);
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
    return -1;
}
