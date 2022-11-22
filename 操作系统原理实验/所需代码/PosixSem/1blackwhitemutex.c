#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int number; // 被保护的全局变量
sem_t sem_id;

void* thread_white_fun(void *arg)
{
	int i;
	
	for(i = 0;i < 3;i++)
	{
		sem_wait(&sem_id);
		printf("w%d:thread_white have the semaphore\n",i);
		number++;usleep(100);
		printf("w%d:white number = %d\n",i,number);
		sem_post(&sem_id);
	}
	
}
void* thread_black_fun(void *arg)
{
	int i;

	for(i = 0;i < 3;i++)
	{
		sem_wait(&sem_id);
		printf("b%d:thread_black have the semaphore \n",i);
		number--;usleep(100);
		printf("b%d:black number = %d\n",i,number);
		sem_post(&sem_id);
	}

}

int main(int argc,char *argv[])
{
	number = 0;
	pthread_t id1, id2;
	sem_init(&sem_id, 0, 1);
	pthread_create(&id1,NULL,thread_white_fun, NULL);
	pthread_create(&id2,NULL,thread_black_fun, NULL);
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	printf("main,,,\n");
	return 0;
}
