#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char buf[100];
	pid_t cld_pid;
	int fd;

	if((fd=open("temp",O_CREAT|O_TRUNC|O_RDWR,S_IRWXU))==-1)
	{
		printf("open error%d",errno);
		exit(1);
	}
	strcpy(buf,"This is parent process write\n");

	if((cld_pid=fork())==0)
	{                                         /*这里是子进程执行的代码*/
		
		strcpy(buf,"This is child process write\n");
		printf("This is child process\n");
		usleep(100);
		printf("My PID (child) is%d\n",getpid());        /*打印出本进程的ID*/
		usleep(200); 
		printf("My parent PID is %d\n",getppid());       /*打印出父进程的ID*/
		usleep(300);
		write(fd,buf,strlen(buf));
		close(fd);
		exit(0);
	}
	else
	{                                         /*这里是父进程执行的代码*/
		//wait(0);                            /*如果此处没有这一句会如何？*/
		printf("This is parent process\n");
		usleep(100);
		printf("My PID (parent) is %d\n",getpid());       /*打印出本进程的ID*/
		usleep(100);
		printf("My child PID is %d\n",cld_pid);          /*打印出子进程的ID*/
		usleep(100);
		write(fd,buf,strlen(buf));
		close(fd);
	}
	return 0;
}
