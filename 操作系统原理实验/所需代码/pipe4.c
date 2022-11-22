#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
int pid1,pid2; 
main( )
{ 
	int fd[2];
	int i;
	char outpipe[200],inpipe[200];
	pipe(fd);                          /*创建一个管道*/
	while ((pid1=fork( )) == -1);
	if(pid1 == 0)
	{   
		lockf(fd[1],1,0);
		/*把输出串放入数组outpipe中*/
		for(i=0;i<3;i++){
			sprintf(outpipe,"child 1 %d %d process is sending message!" , i,getpid()); 
			write(fd[1],outpipe,50);     /*向管道写长为50字节的串*/
			sleep(1); 
		}                /*自我阻塞1秒*/
		lockf(fd[1],0,0);
		exit(0);
	}
	else
	{
		while((pid2=fork( ))==-1);
		if(pid2==0)
		{   
			lockf(fd[1],1,0);           /*互斥*/
			for(i=0;i<3;i++){
				sprintf(outpipe,"child 2 %d %d process is sending message!" , i ,getpid());
				write(fd[1],outpipe,50);
				sleep(1);
			}
			lockf(fd[1],0,0);
			exit(0);
		}
		else
		{   	
			//wait(0);              /*同步*/
			//wait(0);
			printf("father %d\n",getpid());
			for( i=0; i<6; i++){
				read(fd[0],inpipe,50);/*从管道中读长为50字节的串*/
				printf("%s\n",inpipe);
			}
		}
	}
}
