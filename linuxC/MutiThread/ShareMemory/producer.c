#include <stdio.h>
#include <sys/shm.h>
#include <string.h>

int main(){
    // 1. 创建共享内存, 大小为4k
    int shmid = shmget(1000, 4096, IPC_CREAT|0664);
    if(shmid == -1)
    {
        perror("shmget error");
        return -1;
    }

    // 2. 当前进程和共享内存关联
    // 返回值共享内存的起始地址
    void* ptr = shmat(shmid, NULL, 0);
    if(ptr == (void *) -1)
    {
        perror("shmat error");
        return -1;
    }

    // 3. 写共享内存
    const char* p = "hello, world, 共享内存真香,我是wxr...";
    memcpy(ptr, p, strlen(p)+1);

    // 阻塞程序
    printf("按任意键继续, 删除共享内存\n");
    getchar();

    //解除关联
    shmdt(ptr);

    // 删除共享内存
    shmctl(shmid, IPC_RMID, NULL);
    printf("共享内存已经被删除...\n");

    return 0;
}