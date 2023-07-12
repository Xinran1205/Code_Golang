#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "../ThreadPool/MyPool/threadPool.h"

//这个结构体是为了给worker传参
struct SockInfo
{
    int fd;                      // 通信
    struct sockaddr_in addr;     // 地址信息
    pthread_t tid;               // 线程ID,方便后面创建线程和关闭线程
};

typedef struct poolInfo{
    ThreadPool *pool;
    int fd;
}poolInfo;


void handler(void* arg);
void acceptConn(void *arg);

int main(){
    //1.创建监听的套接字，socket函数
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket");
        exit(0);
    }

    //2.把这个监听的套接字和本地的IP和端口绑定，bind函数
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY; 
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1){
        perror("bind");
        exit(0);
    }

    //3.开始监听，listen函数
    ret = listen(fd, 128);
    if(ret == -1){
        perror("listen");
        exit(0);
    }

    //创建线程池
    ThreadPool *pool = ThreadPoolCreate(3, 8, 100);
    poolInfo *polInfo = (poolInfo*)malloc(sizeof(poolInfo));
    polInfo->pool = pool;
    polInfo->fd = fd;
    //向线程池中添加任务（监听）
    //poolInfo这个在线程池中执行完会自动被free掉
    TaskAdd(pool, acceptConn, polInfo);

    //可以直接让主线程退出，因为主线程任务已经完成了，剩下的任务交给线程池
    pthread_exit(NULL);
    return 0;
}

void acceptConn(void *arg){
    poolInfo *polInfo = (poolInfo*)arg;

    int len = sizeof(struct sockaddr_in);

    while(1){
        //pinfo这里我们使用堆内存，不用使用数组了，线程池用完会自动free
        struct SockInfo* pinfo;
        pinfo = (struct SockInfo*)malloc(sizeof(struct SockInfo));
        
        pinfo->fd = accept(polInfo->fd, (struct sockaddr*)&pinfo->addr, &len);

        if(pinfo->fd == -1){
            perror("accept");
            exit(0);
        }
        //把任务放到线程池里面
        TaskAdd(polInfo->pool, handler, pinfo);
        printf(" livethreadNumber = %d, taskNumber = %d,busyNumber = %d\n",
         polInfo->pool->liveNumber,polInfo->pool->taskNumber,polInfo->pool->busyNumber);
    }

    //关闭文件描述符
    close(polInfo->fd);
}

void handler(void* arg){
    struct SockInfo* pinfo = (struct SockInfo*)arg;
    char ip[24] = {0};
    printf("客户端的IP地址: %s, 端口: %d\n",
           inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, ip, sizeof(ip)),
           ntohs(pinfo->addr.sin_port));

    while(1){
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        int len = recv(pinfo->fd, buf, sizeof(buf), 0);
        if(len > 0)
        {
            printf("客户端say: %s\n", buf);
            send(pinfo->fd, buf, len, 0);
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
    close(pinfo->fd);
}
