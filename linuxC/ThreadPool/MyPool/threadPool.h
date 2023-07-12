#ifndef _THREADPOOL_H
#define _THREADPOOL_H

typedef struct Task{
    void(*func)(void* arg);
    void* arg;
}Task;

typedef struct ThreadPool{
    Task* taskQueue;
    int Tcapacity;
    int taskNumber;
    int headPoint;
    int rearPoint;

    pthread_t managerId;
    pthread_t* workerId;
    int minT;
    int maxT;
    int busyNumber;
    int liveNumber;
    int destroyNumber;

    pthread_mutex_t busyThreadLock;
    pthread_mutex_t poolLock;
    pthread_cond_t isEmpty;
    pthread_cond_t isFull;

    int isShutdown;
}ThreadPool;

// typedef struct ThreadPool ThreadPool;

ThreadPool* ThreadPoolCreate(int min, int max, int capacity);

void* worker(void* arg);

void* manager(void* arg);

void threadExit(ThreadPool* pool);

int destroyPool(ThreadPool* pool);

void TaskAdd(ThreadPool* pool,void(*func)(void*),void *arg);

#endif  // _THREADPOOL_H