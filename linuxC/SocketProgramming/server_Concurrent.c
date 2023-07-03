#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

//现在我们要实现N个客户端和1个服务器进行通信

//这个结构体是为了给worker传参
struct SockInfo
{
    int fd;                      // 通信
    struct sockaddr_in addr;     // 地址信息
    pthread_t tid;               // 线程ID,方便后面创建线程和关闭线程
};

struct SockInfo info[512];       // 保存所有的客户端信息,先设置个最大值512

void* worker(void* arg);

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

    //初始化客户端的地址信息
    int max = sizeof(info)/sizeof(info[0]);
    for(int i = 0; i < max; ++i){
        memset(&info[i], 0, sizeof(info[i]))。
        info[i].fd = -1;
    }
    int len = sizeof(struct sockaddr_in);

    while(1){
        struct SockInfo* pinfo;
        //找到一个空闲的位置用来保存客户端的地址信息
        //让pinfo指向这个空闲的位置
        for(int i = 0; i < max; ++i){
            if(info[i].fd == -1){
                pinfo = &info[i];
                break;
            }
        }
        
        int com_fd = accept(fd, (struct sockaddr*)&pinfo->addr, &len);
        //保存通信的文件描述符
        pinfo->fd = com_fd;

        if(com_fd == -1){
            perror("accept");
            exit(0);
        }
        //第四个变量是传给线程函数的参数
        pthread_create(&pinfo->tid, NULL, worker, pinfo);

        //pthread_detach用于将指定的线程标记为“可分离（detached）”状态。
        //在 POSIX 线程库中，线程可以被标记为“可分离”或“不可分离”两种状态。如果一个线程被标记为“可分离”，
        //那么它在结束时会自动释放它所占用的资源，包括内存和其他系统资源。
        //如果一个线程被标记为“不可分离”，那么它在结束时不会自动释放资源，
        //需要等待其他线程通过 pthread_join 函数等方式来回收它的资源。
        pthread_detach(pinfo->tid);
    }

    //关闭文件描述符

    close(fd);
    return 0;
}

//void* 就是一个任意类型的指针，和void恰恰相反，void* 它并不是空，也不是什么都不返回，
//而是可以返回一个任意类型的指针，这里需要谨记的有两个点，一就是任意类型，二是指针。
void* worker(void* arg){
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
    pinfo->fd = -1;
    return NULL;
}
