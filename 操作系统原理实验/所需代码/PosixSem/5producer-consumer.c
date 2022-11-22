#include <stdlib.h>  
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>  
#include <semaphore.h>  
#include <errno.h>  
#include <fcntl.h>  

#define NBUFF      5      //槽位的个数  
   
int nitems;  //条目的个数 
  
int buff[NBUFF];
sem_t mutex,empty,full;

void *produce(void *arg); 
void *consume(void *arg); 
   
int main(int argc,char *argv[]) 
{ 
	pthread_t   tid_produce,tid_consume; 
	if(argc != 2) 
	{ 
		printf("usage: prodcons <#itmes>"); 
		exit(0); 
	} 
	nitems = atoi(argv[1]);  //获取条目数目 
	//创建信号量
	sem_init(&mutex,0,1);
	sem_init(&empty,0,NBUFF);
	sem_init(&full,0,0); 
	
	pthread_setconcurrency(2); //设置并发级别
	pthread_create(&tid_produce,NULL,produce,NULL); //生产者线程
	pthread_create(&tid_consume,NULL,consume,NULL); //消费者线程
	pthread_join(tid_produce,NULL); 
	pthread_join(tid_consume,NULL); 
	exit(0); 
} 
   
void *produce(void *arg) 
{ 
	int i; 
	printf("\nproduce is called.\n"); 
	for(i=0;i<nitems;i++) 
	{ 		
		sem_wait(&empty); //判断是否有空槽，有的将其减少1		
		sem_wait(&mutex);  //锁住槽位，对于多个生产者的时候有必要，单个生产者没有必要 
		printf("\nproduced a new item %d:",i); 
		scanf("%d",&buff[i%NBUFF]);
		sleep(1);
		sem_post(&mutex);  //释放锁 
		sem_post(&full);  //缓冲区中条目数加1 
	} 
	return NULL; 
} 
   
void *consume(void *arg) 
{ 
	int   i; 
	printf("\nconsumer is called.\n"); 
	for(i=0;i<nitems;i++) 
	{		
		sem_wait(&full);  //判断缓冲区中是否有条目，有的话将条目数减少1 
		sem_wait(&mutex); 	//锁住缓冲区，对多个消费者有必要，对单个消费者没必要
		if(buff[i % NBUFF] != i) 
			printf("\nremoved %d item %d.",i,buff[i% NBUFF]); 
		sleep(1);
		sem_post(&mutex);  //释放锁 
		sem_post(&empty); //将缓冲区中的空槽数目加1 
	} 
	return NULL;
}
