#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "threadpool.h"

//定义一个：当每次要销毁（创建）线程时，销毁（创建）的个数
#define NUMBER 2

// 线程池中的任务
//线程池中的任务实际上可以理解为一串代码（函数），但是我们存储的是这串代码的地址（指针），然后线程池中的线程去执行这串代码
typedef struct Task
{
    void (*function)(void* arg); //函数指针，指向函数的地址，这个函数的参数是void* arg（这个其实就是c语言的泛型，兼容任意数据类型）
    void* arg;                   //用于存储任务函数的参数
}Task;

//先定义线程池结构
typedef struct ThreadPool
{
    // 任务队列
    Task* taskQ;
    int queueCapacity;  // 容量
    int queueSize;      // 当前任务个数
    int queueFront;     // 队头 -> 取数据
    int queueRear;      // 队尾 -> 放数据

    pthread_t managerID;    // 管理者线程ID,因为管理者线程只有一个
    pthread_t *threadIDs;   // 工作的线程ID，因为有多个，所以用数组，threadIDs定义为指针，指向数组的首地址
    int minNum;             // 最小线程数量
    int maxNum;             // 最大线程数量
    int busyNum;            // 忙的线程的个数，因为这个会被多个线程同时修改，所以需要加锁，而且因为这个变量使用频繁，
                            // 所以需要单独拿一个锁，不使用大的线程池锁,提升效率
    int liveNum;            // 存活的线程的个数 ，因为这个变量和下面的变量只是管理者进程（只有一个）修改，所以不需要加锁
    int exitNum;            // 要销毁的线程个数
    pthread_mutex_t mutexPool;  // 锁整个的线程池
    pthread_mutex_t mutexBusy;  // 锁busyNum变量
    pthread_cond_t notFull;     // 任务队列是不是满了
    pthread_cond_t notEmpty;    // 任务队列是不是空了

    int shutdown;           // 是不是要销毁线程池, 销毁为1, 不销毁为0
}ThreadPool;

//函数声明
void* manager(void* arg);
void* worker(void* arg);
void threadExit(ThreadPool* pool);

ThreadPool* threadPoolCreate(int minNum, int maxNum, int queueCapacity){
    ThreadPool* pool = (ThreadPool*)malloc (sizeof(ThreadPool));
    do{
        if(pool ==NULL){
            printf("malloc threadpool fail\n");
            break;
        }
        
        //初始化线程池的一些变量
        pool->minNum = minNum;
        pool->maxNum = maxNum;
        pool->busyNum = 0;
        pool->liveNum = minNum;
        pool->exitNum = 0;
        pool->shutdown = 0;
        if(pthread_mutex_init(&pool->mutexPool,NULL)!=0||
        pthread_mutex_init(&pool->mutexBusy,NULL)!=0||
        pthread_cond_init(&pool->notEmpty,NULL)!=0||
        pthread_cond_init(&pool->notFull,NULL)!=0){
            printf("init the lock or cond fail\n");
            break;
        }

        //接下来初始化线程池中的任务队列
        pool->taskQ = (Task*)malloc(sizeof(Task)*queueCapacity);
        pool->queueCapacity = queueCapacity;
        pool->queueSize = 0;
        pool->queueFront = 0;
        pool->queueRear = 0;

        //因为pthread_create函数的第一个参数一个传出参数，是无符号长整形数，
        //线程创建成功，会将线程 ID 写入到这个指针指向的内存中，所以我们给他一个指针（指向一个地址）即可
        //这里初始化使得所有指针指向的内存地址数据都为0，这是为了区分,后面创建线程时，创建成功会写入线程id到指针指向的地址
        //所以如果指针指向的值是未知数（一个线程id）就代表线程已经创建，如果是0代表线程未创建，所以我们创建新线程是找到id为0的位置创建
        pool->threadIDs = (pthread_t*)malloc(sizeof(pthread_t)*maxNum);
        if(pool->threadIDs == NULL){
            printf("malloc threadIDs fail\n");
            break;
        }
        memset(pool->threadIDs,0,sizeof(pthread_t)*maxNum);

        //创建线程
        //创建管理者线程
        pthread_create(&pool->managerID,NULL,manager,pool);
        //创建工作线程
        //先创建最少数量个线程
        for(int i=0;i<minNum;i++){
            pthread_create(&pool->threadIDs[i],NULL,worker,pool);
        }
        //创建线程池完毕return
        return pool;
    }while(0);
    //当走到这，一定是上面走到了break（走到break的都是报错的）
    //所以我们需要释放资源
    if(pool&&pool->threadIDs){
        free(pool->threadIDs);
    }
    if(pool&&pool->taskQ){
        free(pool->taskQ);
    }
    if(pool){
        free(pool);
    }

    return NULL;
}

//工作线程（消费者）
void* worker(void* arg){
    ThreadPool* pool = (ThreadPool*)arg;
    while(1){
        pthread_mutex_lock(&pool->mutexPool);
        while(pool->queueSize==0&&!pool->shutdown){
            pthread_cond_wait(&pool->notEmpty,&pool->mutexPool);

            // 判断是不是要销毁线程
            if (pool->exitNum > 0)
            {
                //这里减减得写在if外面，因为无论如何我们都要减（即使实际上没有销毁）
                //因为有最少线程数限制
                pool->exitNum--;
                if (pool->liveNum > pool->minNum)
                {
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->mutexPool);
                    threadExit(pool);
                }
            }
        }
        if (pool->shutdown){
            pthread_mutex_unlock(&pool->mutexPool);
            threadExit(pool);
        }
        //取出任务，这里直接定义局部变量，因为任务处理完就甩了，不需要定义成指针，指针还要malloc内存
        Task task;
        task.function = pool->taskQ[pool->queueFront].function;
        task.arg = pool->taskQ[pool->queueFront].arg;
        //头指针后移，任务总数减1
        pool->queueFront = (pool->queueFront+1)%pool->queueCapacity;
        pool->queueSize--;
        pthread_cond_signal(&pool->notFull); //唤醒阻塞的生产者
        pthread_mutex_unlock(&pool->mutexPool);

        printf("thread %ld start working...\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexBusy);

        task.function(task.arg);
        //这里task.arg我们传入的时候要传入堆内存，因为栈不好保存，
        //所以如果传入堆内存，那用完当然要把free掉,free完还要记得设置NULL，防止“野指针”
        free(task.arg);
        task.arg = NULL;

        printf("thread %ld end working...\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexBusy);
    }
    return NULL;
}

void* manager(void* arg)
{
    ThreadPool* pool = (ThreadPool*)arg;
    while (!pool->shutdown)
    {
        // 每隔3s检测一次
        sleep(3);

        // 取出线程池中任务的数量和当前线程的数量
        pthread_mutex_lock(&pool->mutexPool);
        int queueSize = pool->queueSize;
        int liveNum = pool->liveNum;
        pthread_mutex_unlock(&pool->mutexPool);

        // 取出忙的线程的数量
        pthread_mutex_lock(&pool->mutexBusy);
        int busyNum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutexBusy);

        // 添加线程
        // 任务的个数>存活的线程个数 && 存活的线程数<最大线程数
        if (queueSize > liveNum && liveNum < pool->maxNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            int counter = 0;
            //pool->liveNum < pool->maxNum这个条件判断很重要，防止多加了，以至于超过最大线程数
            for (int i = 0; i < pool->maxNum && counter < NUMBER
                && pool->liveNum < pool->maxNum; ++i)
            {
                if (pool->threadIDs[i] == 0)
                {
                    pthread_create(&pool->threadIDs[i], NULL, worker, pool);
                    counter++;
                    pool->liveNum++;
                }
            }
            pthread_mutex_unlock(&pool->mutexPool);
        }
        // 销毁线程
        // 忙的线程*2 < 存活的线程数 && 存活的线程>最小线程数
        if (busyNum * 2 < liveNum && liveNum > pool->minNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum = NUMBER;
            pthread_mutex_unlock(&pool->mutexPool);
            // 让工作的线程自杀
            for (int i = 0; i < NUMBER; ++i)
            {
                pthread_cond_signal(&pool->notEmpty);
            }
        }
    }
    return NULL;
}

void threadExit(ThreadPool* pool){
    //这个作用主要是把指向的地址的值（线程id）改过来
    pthread_t tid = pthread_self();
    for(int i=0;i<pool->maxNum;i++){
        if(pool->threadIDs[i]==tid){
            pool->threadIDs[i]=0;
            break;
        }
    }
    //只要调用该函数当前线程就马上退出了，并且不会影响到其他线程的正常运行，不管是在子线程或者主线程中都可以使用。
    pthread_exit(NULL);
}


int threadPoolDestroy(ThreadPool* pool)
{
    if (pool == NULL)
    {
        return -1;
    }

    // 关闭线程池
    pool->shutdown = 1;
    // 阻塞回收管理者线程
    pthread_join(pool->managerID, NULL);
    // 唤醒阻塞的消费者线程
    for (int i = 0; i < pool->liveNum; ++i)
    {
        pthread_cond_signal(&pool->notEmpty);
    }
    // 释放堆内存
    if (pool->taskQ)
    {
        free(pool->taskQ);
    }
    if (pool->threadIDs)
    {
        free(pool->threadIDs);
    }

    pthread_mutex_destroy(&pool->mutexPool);
    pthread_mutex_destroy(&pool->mutexBusy);
    pthread_cond_destroy(&pool->notEmpty);
    pthread_cond_destroy(&pool->notFull);

    free(pool);
    pool = NULL;

    return 0;
}

//这是生产者，这是一个对外的接口，我们用户调用这个函数
void threadPoolAdd(ThreadPool* pool, void(*func)(void*), void* arg)
{
    pthread_mutex_lock(&pool->mutexPool);
    while (pool->queueSize == pool->queueCapacity && !pool->shutdown)
    {
        // 阻塞生产者线程
        pthread_cond_wait(&pool->notFull, &pool->mutexPool);
    }
    if (pool->shutdown)
    {
        pthread_mutex_unlock(&pool->mutexPool);
        return;
    }
    // 添加任务
    pool->taskQ[pool->queueRear].function = func;
    pool->taskQ[pool->queueRear].arg = arg;
    pool->queueRear = (pool->queueRear + 1) % pool->queueCapacity;
    pool->queueSize++;

    pthread_cond_signal(&pool->notEmpty);
    pthread_mutex_unlock(&pool->mutexPool);
}

//下面这两个函数是对外的接口，没啥用

int threadPoolBusyNum(ThreadPool* pool)
{
    pthread_mutex_lock(&pool->mutexBusy);
    int busyNum = pool->busyNum;
    pthread_mutex_unlock(&pool->mutexBusy);
    return busyNum;
}

int threadPoolAliveNum(ThreadPool* pool)
{
    pthread_mutex_lock(&pool->mutexPool);
    int aliveNum = pool->liveNum;
    pthread_mutex_unlock(&pool->mutexPool);
    return aliveNum;
}
