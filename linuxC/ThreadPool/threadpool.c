#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>


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
    int busyNum;            // 忙的线程的个数，因为这个会被多个线程同时修改，所以需要加锁
    int liveNum;            // 存活的线程的个数 ，因为这个变量和下面的变量只是管理者进程（只有一个）修改，所以不需要加锁
    int exitNum;            // 要销毁的线程个数
    pthread_mutex_t mutexPool;  // 锁整个的线程池
    pthread_mutex_t mutexBusy;  // 锁busyNum变量
    pthread_cond_t notFull;     // 任务队列是不是满了
    pthread_cond_t notEmpty;    // 任务队列是不是空了

    int shutdown;           // 是不是要销毁线程池, 销毁为1, 不销毁为0
}ThreadPool;

//有一个注意的，pthread_create函数的第一个参数一个传出参数，是无符号长整形数，
//线程创建成功，会将线程 ID 写入到这个指针指向的内存中，所以我们给他一个指针（指向一个地址）即可

ThreadPool* ThreadPoolCreate(int minNum, int maxNum, int Capacity){
    ThreadPool* pool = (ThreadPool*)malloc (sizeof(ThreadPool));
    do{
        if(pool ==NULL){
            printf("malloc threadpool fail\n");
            break;
        }
        //这里初始化线程id是为了区分，后面创建线程会用到，可用的线程id为未知数，未初始化的为0，所以我们创建新线程是找到id为0的位置创建
        pool->threadIDs = (pthread_t*)malloc(sizeof(pthread_t)*maxNum);
        if(pool->threadIDs == NULL){
            printf("malloc threadIDs fail\n");
            break;
        }
        memset(pool->threadIDs,0,sizeof(pthread_t)*maxNum);
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
        pool->taskQ = (Task*)malloc(sizeof(Task)*Capacity);
        pool->queueCapacity = Capacity;
        pool->queueSize = 0;
        pool->queueFront = 0;
        pool->queueRear = 0;

        //初始化创建线程
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

void* worker(void* arg){
    ThreadPool* pool = (ThreadPool*)arg;
    while(1){
        pthread_mutex_lock(&pool->mutexPool);
        while(pool->queueSize==0&&!pool->shutdown){
            pthread_cond_wait(&pool->notEmpty,&pool->mutexPool);
        }
        if (pool->shutdown){
            pthread_mutex_unlock(&pool->mutexPool);
            threadExit(pool);
        }
        //这里直接定义局部变量，因为处理完就甩了，不需要定义成指针，指针还要malloc内存
        Task task;
        task.function = pool->taskQ[pool->queueFront].function;
        task.arg = pool->taskQ[pool->queueFront].arg;

        pool->queueFront = (pool->queueFront+1)%pool->queueCapacity;
        pool->queueSize--;
        pthread_mutex_unlock(&pool->mutexPool);

        printf("thread %ld start working...\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexBusy);
        task.function(task.arg);
        //这里task.arg我们传入的时候要传入堆内存，因为栈不好保存，
        //所以如果传入堆内存，那用完当然要把free掉
        free(task.arg);
        task.arg = NULL;

        printf("thread %ld end working...\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexBusy);
    }
    return NULL;
}

void threadExit(ThreadPool* pool){
    pthread_t tid = pthread_self();
    for(int i=0;i<pool->maxNum;i++){
        if(pool->threadIDs[i]==tid){
            pool->threadIDs[i]=0;
            break;
        }
    }
    pthread_exit(NULL);
}