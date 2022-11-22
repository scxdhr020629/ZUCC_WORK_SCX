
#include <stdio.h>
……

#define MSGKEY1 66
#define MSGKEY2 68

struct msgform
{
	long mtype;
	char mtext[1000];
}msg;

int msgqid;

void server( )
{
	msgqid=msgget(MSGKEY2,0777|IPC_CREAT); 
	……
}
void client()
{	int i; 
	/*打开消息队列*/
	msgqid=msgget(MSGKEY1,0777|IPC_CREAT); 
	……
}
main( )
{
	if(fork()!=0)
		client();
	else
		server();
}