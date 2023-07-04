#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>

struct Node{
    int data;
    struct Node* next;
};

sem_t proSem;
sem_t conSem;

pthread_mutex_t mutex;

struct Node* head = NULL;

void* producer(void* argv){
    while(1){
        sem_wait(&proSem);
        pthread_mutex_lock(&mutex);
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->data = rand() % 1000;
        newNode->next = head;
        head = newNode;
        printf("+++producer, number = %d, tid = %ld\n", head->data, pthread_self());
        pthread_mutex_unlock(&mutex);
        sem_post(&conSem);
        sleep(10);
    }
    return NULL;
}

void* consumer(void* argv){
    while(1){
        sem_wait(&conSem);
        pthread_mutex_lock(&mutex);
        struct Node* delNode = head;
        printf("--consumer: number: %d, tid = %ld\n", delNode->data, pthread_self());
        head = head->next;
        free(delNode);
        pthread_mutex_unlock(&mutex);
        sem_post(&proSem);
        sleep(10);
    }
    return NULL;
}

int main(){
    sem_init(&proSem, 0, 5);  // 生成者线程一共有5个信号灯
    sem_init(&conSem, 0, 0);  // 消费者线程一共有0个信号灯
    // 初始化互斥锁
    pthread_mutex_init(&mutex, NULL);

    // 创建5个生产者, 5个消费者
    pthread_t ptid[5];
    pthread_t ctid[5];
    for(int i=0; i<5; ++i)
    {
        pthread_create(&ptid[i], NULL, producer, NULL);
    }

    for(int i=0; i<5; ++i)
    {
        pthread_create(&ctid[i], NULL, consumer, NULL);
    }

    // 释放资源
    for(int i=0; i<5; ++i)
    {
        pthread_join(ptid[i], NULL);
    }

    for(int i=0; i<5; ++i)
    {
        pthread_join(ctid[i], NULL);
    }

    sem_destroy(&proSem);
    sem_destroy(&conSem);
    pthread_mutex_destroy(&mutex);

    return 0;
}