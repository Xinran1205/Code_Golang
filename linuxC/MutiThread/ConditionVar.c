#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

struct Node{
    int data;
    struct Node* next;
};

struct Node* head = NULL;

pthread_cond_t cond;
pthread_mutex_t mutex;

void* producer(void* arg){
    int i =0;
    while(i<10){
        pthread_mutex_lock(&mutex);
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->data = i;
        newNode->next = head;
        head = newNode;
        i++;
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&cond);

        // 生产慢一点
        sleep(3);
    }
    return NULL;
}

void* consumer(void* arg){
    while(1){
        pthread_mutex_lock(&mutex);
        //head等于null说明没有资源
        while(head==NULL){
            pthread_cond_wait(&cond,&mutex);
        }
        struct Node* DelNode = head;
        printf("--consumer: number: %d, tid = %ld\n", head->data, pthread_self());
        head = head->next;
        free(DelNode);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(){
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex,NULL);

    pthread_t proctid[5];
    pthread_t consutid[5];

    for(int i=0;i<5;i++){
        pthread_create(&proctid[i],NULL,producer,NULL);
    }

    for(int i=0;i<5;i++){
        pthread_create(&consutid[i],NULL,consumer,NULL);
    }

    for (int i=0;i<5;i++){
        pthread_join(proctid[i],NULL);
    }
    for (int i=0;i<5;i++){
        pthread_join(consutid[i],NULL);
    }

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);

    return 0;
}