#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define KEY 1234                              /*键*/
#define SIZE 1024                             /*欲建立的共享内存的大小*/

int main()
{
	int shmid;
	char *shmaddr;
	struct shmid_ds buf;
	shmid=shmget(KEY,SIZE,IPC_CREAT|0600);         /*建立共享内存*/
	if(shmid==-1)
	{
		printf("create share memory failed:%s",strerror(errno));
		return 0;
	}
	if(fork( )==0)
	{                                   /*子进程*/
		sleep(2);	
		shmaddr=(char*)shmat(shmid,NULL,0);   /*系统自动选择一个地址连接*/
		if(shmaddr==(void*)-1)
		{
			printf("connect to the share memory failed:%s",strerror(errno));
			return 0;
		}
		/*向共享内存内写数据*/
		strcpy(shmaddr,"hello,i am huj\n");
		shmdt(shmaddr);                            /*断开共享内存*/
		exit(0);

	}else
	{                                          /*父进程*/
		wait(0);		
		shmctl(shmid,IPC_STAT,&buf);     /*取得共享内存的相关信息*/
		printf("size of the share memory: shm_segsz=%dbytes\n",buf.shm_segsz);
		printf("process id of the creator:shm_cpid=%d\n",buf.shm_cpid);
		printf("process id of the last operator:shm_lpid=%d\n\n",buf.shm_lpid);
		shmaddr=(char*)shmat(shmid,NULL,0);      /*系统自动选择一个地址连接*/
		if(shmaddr==(void*)-1)
		{
			printf("connect the share memory failed:%s",strerror(errno));
			return 0;
		}
		printf("print the content of the share memory:\t");
		printf("%s\n",shmaddr);
		shmdt(shmaddr);                           /*断开共享内存*/
		shmctl(shmid,IPC_STAT,&buf);     /*取得共享内存的相关信息*/
		printf("process id of the last operator:shm_lpid=%d\n",buf.shm_lpid);
		/*当不再有任何其它进程使用该共享内存时系统将自动销毁它*/
		//shmctl(shmid,IPC_RMID,NULL);
	}
}
