#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int number; // 被保护的全局变量
sem_t sem_id1, sem_id2;

void *thread_white_fun(void *arg)
{
	int i;

	for (i = 0; i < 3; i++)
	{
		sem_wait(&sem_id1);
		printf("thread_white have the semaphore\n");
		number++;
		usleep(100);
		printf("white number = %d\n", number);
		sem_post(&sem_id2);
		usleep(100);
	}
}
void *thread_black_fun(void *arg)
{
	int i;

	for (i = 0; i < 3; i++)
	{
		sem_wait(&sem_id2);
		printf("thread_black have the semaphore \n");
		number--;
		usleep(100);
		printf("black number = %d\n", number);
		sem_post(&sem_id1);
		usleep(100);
	}
}
int main(int argc, char *argv[])
{
	number = 0;
	pthread_t id1, id2;
	sem_init(&sem_id1, 0, 0); // 空闲的
	sem_init(&sem_id2, 0, 1); // 忙的
	pthread_create(&id1, NULL, thread_white_fun, NULL);
	pthread_create(&id2, NULL, thread_black_fun, NULL);
	pthread_join(id1, NULL);
	pthread_join(id2, NULL);
	printf("main,,,\n");
	return 0;
}
