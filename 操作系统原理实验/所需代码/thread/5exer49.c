#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

int myglobal=0;
void *thread_function(void *arg)
{
	int i,j;
	for(i = 0; i <20; i++){
		j = myglobal;
		j++;
		printf(".");
		fflush(stdout);
		usleep(1000);
		myglobal = j;
	}
	return NULL;
}

int main(){
	pthread_t mythread;
	int i;
	if( pthread_create(&mythread,NULL,thread_function,NULL))
	{
		printf("error creating thread!\n");
		abort();
	}
	for(i = 0; i <20; i++)
	{
		myglobal++;
		printf("O");
		fflush(stdout);
		usleep(1000);
	}
	if( pthread_join(mythread,NULL))
	{
		printf("error joining thread!\n");
		abort();
	}
	printf("\nmyglobal equald %d\n",myglobal);
	exit(0);
}
