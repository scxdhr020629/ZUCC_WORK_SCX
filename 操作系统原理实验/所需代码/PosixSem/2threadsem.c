#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define MAXSTACK 100

int stack[MAXSTACK];
int size = 0;
sem_t sem1;
sem_t sem2;

/* 从文件1.dat读取数据，每读一次，信号量加一*/
void ReadData1(void)
{
	printf("ReadData1 begin:**********************\n");
	FILE *fp = fopen("1.dat", "r");
	while (!feof(fp))
	{
		fscanf(fp, "%d %d", &stack[0], &stack[1]);
		sem_getvalue(&sem1, &size);
		printf("ReadData1 stack[0] =%d stack[1]=%d sem1.value = %d\n", stack[0], stack[1], size);
		sem_post(&sem1);
		// sleep(1);
	}
	fclose(fp);
}

/*从文件2.dat读取数据*/
void ReadData2(void)
{
	printf("ReadData2 begin:**********************\n");
	FILE *fp = fopen("2.dat", "r");
	while (!feof(fp))
	{
		fscanf(fp, "%d %d", &stack[0], &stack[1]);
		sem_getvalue(&sem2, &size);
		printf("ReadData2 stack[0] =%d stack[1]=%d sem2.value = %d\n", stack[0], stack[1], size);
		sem_post(&sem2);
		// sleep(1);
	}
	fclose(fp);
}

/*阻塞等待缓冲区有数据，读取数据后，释放空间，继续等待*/
void HandleData1(void)
{
	printf("Plus begin:**********************\n");
	while (1)
	{
		sem_getvalue(&sem1, &size);
		sem_wait(&sem1);
		printf("Plus:%d+%d=%d sem1.value = %d \n", stack[0], stack[1], stack[0] + stack[1], size);
		// sleep(1);
	}
}

void HandleData2(void)
{
	printf("Multiply begin:**********************\n");
	while (1)
	{
		sem_getvalue(&sem2, &size);
		sem_wait(&sem2);
		printf("Multiply:%d*%d=%d sem2.value = %d\n", stack[0], stack[1], stack[0] * stack[1], size);
		// sleep(1);
	}
}

int main(void)
{
	pthread_t t1, t2, t3, t4;
	sem_init(&sem1, 0, 0);
	sem_init(&sem2, 0, 0);
	pthread_create(&t1, NULL, (void *)HandleData1, NULL);
	pthread_create(&t2, NULL, (void *)HandleData2, NULL);
	pthread_create(&t3, NULL, (void *)ReadData1, NULL);
	pthread_create(&t4, NULL, (void *)ReadData2, NULL);
	/* 防止程序过早退出，让它在此无限期等待*/
	getchar();
}
