// pthread_join.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

//写这个之前想明白.和->

//pthread_exit() 函数用于终止当前线程。它可以传递一个指向线程返回值的指针作为参数，该返回值可以被其他线程通过 pthread_join() 函数获取。
//如果不需要向其他线程返回值，则可以将 pthread_exit() 函数的参数设置为 NULL。

// pthread_join() 函数用于等待指定的线程结束并获取其返回值。
//它接受两个参数：要等待的线程的标识符，以及一个指向线程返回值的指针。当被等待的线程结束时，它的返回值会被写入指针所指向的内存中。
//如果不需要返回值，可以将第二个参数设置为 NULL。如果指定的线程已经结束，或者从来没有被创建，
//pthread_join() 函数会立即返回，并且不会产生任何效果。
//这个函数是一个阻塞函数，如果还有子线程在运行，调用该函数就会阻塞，
//子线程退出函数解除阻塞进行资源的回收，函数被调用一次，只能回收一个子线程，如果有多个子线程则需要循环进行回收。

// 一般来说，pthread_exit() 用于在线程内部终止线程，并返回一个值；
//而 pthread_join() 用于等待另一个线程结束，并获取它的返回值。可以通过这两个函数来实现线程之间的同步和通信。
// 需要注意的是，在使用 pthread_join() 函数时，要确保被等待的线程不会一直处于阻塞状态，否则会导致死锁。
//因此，一般建议在等待线程之前，先将其设置为分离线程（detached thread），这样可以确保线程结束时自动释放资源，
//不需要显式地调用 pthread_join() 函数。



// 定义结构
struct Persion
{
    int id;
    char name[36];
    int age;
};
// struct Persion p;
// 子线程的处理代码
void* working(void* arg)
{
    struct Persion* p = (struct Persion*)arg;
    printf("我是子线程, 线程ID: %ld\n", pthread_self());
    for(int i=0; i<9; ++i)
    {
        printf("child == i: = %d\n", i);
        if(i == 6)
        {
            //这么写会有问题，发现最后打印的数据不一样，因为我们子线程结束后，他申请的栈区内存就被释放了，数据也被释放了，
            //解决方法：1.使用全局变量
            //2.使用主线程的栈空间，在main里面创建Persion，然后在pthread_create传递给子线程，子线程就可以使用了
            // struct Persion p;
            p->age  =12;
            strcpy(p->name, "tom");
            p->id = 100;
            // 该函数的参数将这个地址传递给了主线程的pthread_join()
            //代码执行到这就会退出了
            pthread_exit(&p);
        }
    }
    return NULL;	// 代码执行不到这个位置就退出了
}

int main()
{
    // 1. 创建一个子线程
    pthread_t tid;
    struct Persion p;
    pthread_create(&tid, NULL, working, &p);

    printf("子线程创建成功, 线程ID: %ld\n", tid);
    // 2. 子线程不会执行下边的代码, 主线程执行
    printf("我是主线程, 线程ID: %ld\n", pthread_self());
    for(int i=0; i<3; ++i)
    {
        printf("i = %d\n", i);
    }

    // 阻塞等待子线程退出，我认为：pthread_join好像不能接收子线程创建的地址类的返回值，只能接收一些值变量
    // void* ptr = NULL;
    // ptr是一个传出参数, 在函数内部让这个指针指向一块有效内存
    // 这个内存地址就是pthread_exit(&p)中p的地址
    pthread_join(tid, NULL);
    // 打印信息

    printf("子线程返回数据: name: %s, age: %d, id: %d\n", p.name, p.age, p.id);
    printf("子线程资源被成功回收...\n");
    
    return 0;
}

//如何使用detach
//在某些情况下，程序中的主线程有属于自己的业务处理流程，如果让主线程负责子线程的资源回收，
//调用 pthread_join() 只要子线程不退出主线程就会一直被阻塞，主要线程的任务也就不能被执行了。
//在线程库函数中为我们提供了线程分离函数 pthread_detach()，调用这个函数之后指定的子线程就可以和主线程分离，
//当子线程退出的时候，其占用的内核资源就被系统的其他进程接管并回收了。线程分离之后在主线程中使用 pthread_join() 就回收不到子线程资源了。

// int main()
// {
//     // 1. 创建一个子线程
//     pthread_t tid;
//     pthread_create(&tid, NULL, working, NULL);

//     printf("子线程创建成功, 线程ID: %ld\n", tid);
//     // 2. 子线程不会执行下边的代码, 主线程执行
//     printf("我是主线程, 线程ID: %ld\n", pthread_self());
//     for(int i=0; i<3; ++i)
//     {
//         printf("i = %d\n", i);
//     }

//     // 设置子线程和主线程分离
//     pthread_detach(tid);

//     // 让主线程自己退出即可
//     pthread_exit(NULL);
    
//     return 0;
// }