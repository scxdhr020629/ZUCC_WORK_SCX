#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <fcntl.h>

#define NBUFF 5 //槽位的个数

int nitems; //条目的个数

int buff[NBUFF];
sem_t mutex, fullB, fullC, fullD;
sem_t emptyB, emptyC, emptyD;
sem_t B, C, D;
void *produceA(void *arg);
void *consumeB(void *arg);
void *consumeC(void *arg);
void *consumeD(void *arg);

int main(int argc, char *argv[])
{
    pthread_t tid_produceA, tid_consumeB, tid_consumeC, tid_consumeD;
    if (argc != 2)
    {
        printf("usage: prodcons <#itmes>");
        exit(0);
    }
    nitems = atoi(argv[1]); //获取条目数目
    //创建信号量
    sem_init(&mutex, 0, 1);

    sem_init(&emptyB, 0, NBUFF);
    sem_init(&emptyC, 0, NBUFF);
    sem_init(&emptyD, 0, NBUFF);

    sem_init(&fullB, 0, 0);
    sem_init(&fullC, 0, 0);
    sem_init(&fullD, 0, 0);
    sem_init(&B, 0, 1);
    sem_init(&C, 0, 1);
    sem_init(&D, 0, 1);

    pthread_setconcurrency(2);                           //设置并发级别
    pthread_create(&tid_produceA, NULL, produceA, NULL); //生产者A线程
    pthread_create(&tid_consumeB, NULL, consumeB, NULL); //消费者B线程
    pthread_create(&tid_consumeC, NULL, consumeC, NULL); //消费者B线程
    pthread_create(&tid_consumeD, NULL, consumeD, NULL); //消费者B线程
    pthread_join(tid_produceA, NULL);
    pthread_join(tid_consumeB, NULL);
    pthread_join(tid_consumeC, NULL);
    pthread_join(tid_consumeD, NULL);
    exit(0);
}

void *produceA(void *arg)
{
    int i;
    printf("\nproduceA is called.\n");
    for (i = 0; i < nitems; i++)
    {
        //判断是否有空槽，有的将其减少1
        sem_wait(&emptyB);
        sem_wait(&emptyC);
        sem_wait(&emptyD);

        sem_wait(&B);
        sem_wait(&C);
        sem_wait(&D);

        sem_wait(&mutex); //锁住槽位，对于多个生产者的时候有必要，单个生产者没有必要
        printf("\nA produced a new item %d:", i);
        scanf("%d", &buff[i % NBUFF]);
        sleep(1);
        sem_post(&mutex); //释放锁

        sem_post(&fullB); //缓冲区中条目数加1
        sem_post(&fullC); //缓冲区中条目数加1
        sem_post(&fullD); //缓冲区中条目数加1
    }
    return NULL;
}

void *consumeB(void *arg)
{
    int i;
    printf("\nconsumer is called.\n");
    for (i = 0; i < nitems; i++)
    {
        sem_wait(&fullB); //判断缓冲区中是否有条目，有的话将条目数减少1
        sem_wait(&mutex); //锁住缓冲区，对多个消费者有必要，对单个消费者没必要
        if (buff[i % NBUFF] != i)
            printf("\nB removed %d item %d.", i, buff[i % NBUFF]);
        sleep(1);
        sem_post(&mutex); //释放锁
        sem_post(&B);     //将缓冲区中的空槽数目加1
        sem_post(&emptyB);
    }
    return NULL;
}
void *consumeC(void *arg)
{
    int i;
    printf("\nconsumer is called.\n");
    for (i = 0; i < nitems; i++)
    {
        sem_wait(&fullC); //判断缓冲区中是否有条目，有的话将条目数减少1
        sem_wait(&mutex); //锁住缓冲区，对多个消费者有必要，对单个消费者没必要
        if (buff[i % NBUFF] != i)
            printf("\nC removed %d item %d.", i, buff[i % NBUFF]);
        sleep(1);
        sem_post(&mutex); //释放锁
        sem_post(&C);     //将缓冲区中的空槽数目加1
        sem_post(&emptyC);
    }
    return NULL;
}
void *consumeD(void *arg)
{
    int i;
    printf("\nconsumer is called.\n");
    for (i = 0; i < nitems; i++)
    {
        sem_wait(&fullD); //判断缓冲区中是否有条目，有的话将条目数减少1
        sem_wait(&mutex); //锁住缓冲区，对多个消费者有必要，对单个消费者没必要
        if (buff[i % NBUFF] != i)
            printf("\nD removed %d item %d.", i, buff[i % NBUFF]);
        sleep(1);
        sem_post(&mutex); //释放锁
        sem_post(&D);     //将缓冲区中的空槽数目加1
        sem_post(&emptyD);
    }
    return NULL;
}
