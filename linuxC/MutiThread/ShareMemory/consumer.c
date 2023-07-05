#include <stdio.h>
#include <sys/shm.h>
#include <string.h>

int main(){

    int shmid = shmget(1000,0,0);
    if(shmid == -1)
    {
        perror("shmget error");
        return -1;
    }

    void* ptr = shmat(shmid, NULL, 0);
    if(ptr == (void *) -1)
    {
        perror("shmat error");
        return -1;
    }


    printf("共享内存数据: %s\n", (char*)ptr);


    printf("按任意键继续, 删除共享内存\n");
    getchar();

    shmdt(ptr);


    shmctl(shmid, IPC_RMID, NULL);
    printf("共享内存已经被删除...\n");

    return 0;
}