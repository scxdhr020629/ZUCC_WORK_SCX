#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define KEY 1234                              /*��*/
#define SIZE 1024                             /*�������Ĺ����ڴ�Ĵ�С*/

int main()
{
	int shmid;
	char *shmaddr;
	struct shmid_ds buf;
	shmid=shmget(KEY,SIZE,IPC_CREAT|0600);         /*���������ڴ�*/
	if(shmid==-1)
	{
		printf("create share memory failed:%s",strerror(errno));
		return 0;
	}
	if(fork( )==0)
	{                                   /*�ӽ���*/
		sleep(2);	
		shmaddr=(char*)shmat(shmid,NULL,0);   /*ϵͳ�Զ�ѡ��һ����ַ����*/
		if(shmaddr==(void*)-1)
		{
			printf("connect to the share memory failed:%s",strerror(errno));
			return 0;
		}
		/*�����ڴ���д����*/
		strcpy(shmaddr,"hello,i am huj\n");
		shmdt(shmaddr);                            /*�Ͽ������ڴ�*/
		exit(0);

	}else
	{                                          /*������*/
		wait(0);		
		shmctl(shmid,IPC_STAT,&buf);     /*ȡ�ù����ڴ�������Ϣ*/
		printf("size of the share memory: shm_segsz=%dbytes\n",buf.shm_segsz);
		printf("process id of the creator:shm_cpid=%d\n",buf.shm_cpid);
		printf("process id of the last operator:shm_lpid=%d\n\n",buf.shm_lpid);
		shmaddr=(char*)shmat(shmid,NULL,0);      /*ϵͳ�Զ�ѡ��һ����ַ����*/
		if(shmaddr==(void*)-1)
		{
			printf("connect the share memory failed:%s",strerror(errno));
			return 0;
		}
		printf("print the content of the share memory:\t");
		printf("%s\n",shmaddr);
		shmdt(shmaddr);                           /*�Ͽ������ڴ�*/
		shmctl(shmid,IPC_STAT,&buf);     /*ȡ�ù����ڴ�������Ϣ*/
		printf("process id of the last operator:shm_lpid=%d\n",buf.shm_lpid);
		/*���������κ���������ʹ�øù����ڴ�ʱϵͳ���Զ�������*/
		//shmctl(shmid,IPC_RMID,NULL);
	}
}
