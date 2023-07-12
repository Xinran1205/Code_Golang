#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "threadpool.h"
// #include "threadpool.c"

typedef struct argg{
    int num;
    ThreadPool* pool;
}argg;

void taskFunc(void* arg);

int main()
{
    ThreadPool* pool = ThreadPoolCreate(5, 10, 100);
    for (int i = 0; i < 100; ++i)
    {
        argg* arg = (argg*)malloc(sizeof(argg));
        arg->num = i + 100;
        arg->pool = pool;
        TaskAdd(pool, taskFunc, arg);
    }

    sleep(10);

    destroyPool(pool);
    return 0;
}

void taskFunc(void* arg)
{
    argg* ret= (argg*)arg;
    printf("thread %ld is working, number = %d, livethreadNumber = %d, taskNumber = %d,busyNumber = %d\n",
        pthread_self(), ret->num, ret->pool->liveNumber,ret->pool->taskNumber,ret->pool->busyNumber);
    sleep(1);
}
