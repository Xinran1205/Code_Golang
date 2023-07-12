#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "threadPool.h"

#define Number 2

void* manager(void* arg);
void* worker(void* arg);
void threadExit(ThreadPool* pool);

ThreadPool* ThreadPoolCreate(int min, int max, int capacity){
    ThreadPool * pool = (ThreadPool *)malloc(sizeof(ThreadPool));
    do{
        if(pool==NULL){
            break;
        }
        pool->Tcapacity = capacity;
        pool->minT = min;
        pool->maxT = max;
        pool->busyNumber = 0;
        pool->liveNumber = min;
        pool->destroyNumber = 0;
        pool ->isShutdown = 0;
        if(pthread_mutex_init(&pool->busyThreadLock,NULL)!=0||
        pthread_mutex_init(&pool->poolLock,NULL)!=0||
        pthread_cond_init(&pool->isEmpty,NULL)!=0||
        pthread_cond_init(&pool->isFull,NULL)!=0){
            break;
        }
        pool->workerId = (pthread_t*)malloc(sizeof(pthread_t)*max);
        if(pool->workerId==NULL){
            break;
        }
        memset(pool->workerId,0,max);
        pthread_create(&pool->managerId,NULL,manager,pool);
        for(int i=0;i<min;i++){
            pthread_create(&pool->workerId[i],NULL,worker,pool);
        }
        pool->taskNumber=0;
        pool->taskQueue = (Task*)malloc(sizeof(Task)*capacity);
        pool->headPoint=0;
        pool->rearPoint=0;
        return pool;
    }while(1);

    if(pool->workerId!=NULL){
        free(pool->workerId);
        pool->workerId = NULL;
    }
    if(pool->taskQueue!=NULL){
        free(pool->taskQueue);
        pool->taskQueue = NULL;
    }
    free(pool);
    pool = NULL;
    return NULL;
}

void* manager(void* arg){
    ThreadPool* pool = (ThreadPool*)arg;
    while(!pool->isShutdown){
        pthread_mutex_lock(&pool->poolLock);
        int liveNumber = pool->liveNumber;
        int taskNumber = pool->taskNumber;
        int busyNumber = pool->busyNumber;
        // pthread_mutex_unlock(&pool->poolLock);

        if (liveNumber<taskNumber&&liveNumber<pool->maxT){
            int counter =0;
            for(int i=0;i<pool->maxT&&counter<Number&&pool->liveNumber<pool->maxT;i++){
                if(pool->workerId[i]==0){
                    pool->liveNumber++;
                    counter++;
                    pthread_create(&pool->workerId[i],NULL,worker,pool);
                }
            }
        }
        pthread_mutex_unlock(&pool->poolLock);

        if(busyNumber+2<pool->liveNumber&&pool->liveNumber>pool->minT){
            pthread_mutex_lock(&pool->poolLock);
            pool->destroyNumber = Number;
            pthread_mutex_unlock(&pool->poolLock);
            for(int i=0;i<Number;i++){
                pthread_cond_signal(&pool->isEmpty);
            }
        }
    }
    return NULL;
}

void* worker(void* arg){
    ThreadPool* pool = (ThreadPool*)arg;
    while(1){
        pthread_mutex_lock(&pool->poolLock);
        while(pool->taskNumber==0&&!pool->isShutdown){
            pthread_cond_wait(&pool->isEmpty,&pool->poolLock);

            if(pool->destroyNumber>0){
                pool->destroyNumber--;
                if(pool->liveNumber>pool->minT){
                    pool->liveNumber--;
                    pthread_mutex_unlock(&pool->poolLock);
                    threadExit(pool);
                }
            }
        }
        if(pool->isShutdown){
            pthread_mutex_unlock(&pool->poolLock);
            threadExit(pool);
        }
        Task task;
        task.func = pool->taskQueue[pool->headPoint].func;
        task.arg = pool->taskQueue[pool->headPoint].arg;
        pool->headPoint = (pool->headPoint+1)%pool->Tcapacity;
        pool->taskNumber--;
        pthread_cond_signal(&pool->isFull);
        pthread_mutex_unlock(&pool->poolLock);

        printf("thread %ld start working...\n", pthread_self());
        pthread_mutex_lock(&pool->busyThreadLock);
        pool->busyNumber++;
        pthread_mutex_unlock(&pool->busyThreadLock);

        task.func(task.arg);
        free(task.arg);
        task.arg = NULL;

        printf("thread %ld end working...\n", pthread_self());
        pthread_mutex_lock(&pool->busyThreadLock);
        pool->busyNumber--;
        pthread_mutex_unlock(&pool->busyThreadLock);
    }

return NULL;
}

void threadExit(ThreadPool* pool){
    for(int i=0;i<pool->maxT;i++){
        if(pool->workerId[i]==pthread_self()){
            pool->workerId[i]=0;
            break;
        }
    }
    pthread_exit(NULL);
}

int destroyPool(ThreadPool* pool){
    if (pool == NULL)
    {
        return -1;
    }
    pool->isShutdown = 1;
    pthread_join(pool->managerId,NULL);

    for (int i=0;i<pool->liveNumber;i++){
        pthread_cond_signal(&pool->isEmpty);
    }

   if(pool->workerId!=NULL){
        free(pool->workerId);
        pool->workerId = NULL;
    }
    if(pool->taskQueue!=NULL){
        free(pool->taskQueue);
        pool->taskQueue = NULL;
    }

    pthread_mutex_destroy(&pool->poolLock);
    pthread_mutex_destroy(&pool->busyThreadLock);
    pthread_cond_destroy(&pool->isEmpty);
    pthread_cond_destroy(&pool->isFull);
    free(pool);
    pool = NULL;

    return 1;
}

void TaskAdd(ThreadPool* pool,void(*func)(void*),void *arg){
    pthread_mutex_lock(&pool->poolLock);
    while(pool->taskNumber==pool->Tcapacity&&!pool->isShutdown){
        pthread_cond_wait(&pool->isFull,&pool->poolLock);
    }
    if (pool->isShutdown)
    {
        pthread_mutex_unlock(&pool->poolLock);
        return;
    }
    pool->taskQueue[pool->rearPoint].func=func;
    pool->taskQueue[pool->rearPoint].arg = arg;
    pool->rearPoint = (pool->rearPoint+1)%pool->Tcapacity;
    pool->taskNumber++;
    pthread_mutex_unlock(&pool->poolLock);
    pthread_cond_signal(&pool->isEmpty);
}