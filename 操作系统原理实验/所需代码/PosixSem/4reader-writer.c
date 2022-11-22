#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>

#define READERCOUNT  5		//读者数
#define WRITERCOUNT  3		//写者数
#define PAPERSIZE  26     	//文件长度

static char paper[PAPERSIZE];        //文件
unsigned short int write_index = 0; //写者需要写入的位置
char ch = 'A';  					//写者需要写入的字母
sem_t rmutex,wmutex; 
int nReader = 0;

//读者线程
void *reader(void *args)
{
    int number =*((int *)args);
    int i;
    for(i = 0;i <10;i++){
		sem_wait(&rmutex);
        //如果是第一个读者, 则锁定wmutex
        if (nReader == 0)
            sem_wait(&wmutex);
        ++ nReader;
        sem_post(&rmutex);
 
        //开始读
        printf("## reader %d was reading...\n", number);
        printf("text: %s\n", paper);sleep(1);
        printf("   reader %d end reading...\n\n", number);
 
        sem_wait(&rmutex);
        -- nReader;
        //如果是最后一个读者, 则解锁wmutex
        if (nReader == 0)
            sem_post(&wmutex);
        sem_post(&rmutex);
        sleep(1);
    }
    pthread_exit(NULL);
}
 
//写者线程
void *writer(void *args)
{
    int number =*((int *)args);
    int i;
    for(i = 0;i <9;i++){
        sem_wait(&wmutex);//获取写锁
        //start writing
        printf("++ writer %d was writing...%c\n", number,ch);
        paper[write_index] = ch;
		sleep(1);
        write_index = (write_index+1)%26;
        ch = ch+1;
        if (ch > 'Z') 		ch = 'A';
        printf("   writer %d end writing...\n\n", number);
        //end writing
        sem_post(&wmutex);//释放写锁
        sleep(1);
	} 
    pthread_exit(NULL);
}
 
int main()
{
	int i;    
	int rThdNum[READERCOUNT],wThdNum[WRITERCOUNT];
	pthread_t wthread[WRITERCOUNT];  //写者线程
	pthread_t rthread[READERCOUNT];  //读者线程	
	sem_init(&rmutex, 0, 1);
	sem_init(&wmutex, 0, 1);

	for (i = 0; i < READERCOUNT; i++)
		rThdNum[i] = i;
	for (i = 0; i < WRITERCOUNT; i++)
		wThdNum[i] = i;

	for ( i = 0; i < READERCOUNT; ++i)
		pthread_create(&rthread[i], NULL, reader,(void *)&rThdNum[i]);
	for (i = 0; i <WRITERCOUNT; ++i)
		pthread_create(&wthread[i], NULL, writer,(void *)&wThdNum[i]);

	for (i = 0; i < READERCOUNT; ++i)
		pthread_join(rthread[i], NULL);
	for (i = 0; i < WRITERCOUNT; ++i)
        	pthread_join(wthread[i], NULL);
 }
