#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdlib.h>

#define MSGKEY 75

struct msgform
{
	long mtype;
	char mtext[1000];
}msg;

int msgqid;

void server( )
{
	/*创建75#消息队列*/
	msgqid=msgget(MSGKEY,0777|IPC_CREAT); 
	do 
	{
		/*接收消息*/
		msgrcv(msgqid,&msg,1030,0,0);
		printf("(server)received:%s\n",msg.mtext);
	}while(msg.mtype!=1);
	/*删除消息队列，归还资源*/
	msgctl(msgqid,IPC_RMID,0);
	exit(0);
}
main( )
{
	server( );
}
