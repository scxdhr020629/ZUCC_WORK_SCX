#include<stdio.h>
#include<unistd.h>
#include <stdlib.h>

int main()
{
		pid_t pid1,pid2;
		char *argv1[]={"ls"," ",0};
		char *argv2[]={"more"," ",0};

		int fd[2];

		if(pipe(fd)<0)printf("create pipe erorr");

		if(pid1=fork() ==0)//管道上游进程
		{
			close(fd[0]);//关闭管道读取端口
			close(1);//关闭最初的标准输出
			dup(fd[1]);//管道的写入端改为文件描述符1
			close(fd[1]);//关闭打开文件描述符的一个副本
			if(execvp("ls",argv1)==-1)//启动
				printf("come on");
		}

		if(pid2=fork()==0)//管道下游进程
		{
			close(fd[1]);//关闭管道写入端口
			close(0);//关闭最初的标准输入
			dup(fd[0]);//管道的读取端改为文件描述符0
			close(fd[0]);//关闭打开文件描述符的一个副本
			execvp("more",argv2);//启动运行程序
		}

		else 
		{
			close(fd[0]);
			close(fd[1]);

			wait(NULL);
			wait(NULL);
		}

		return 0;

		}

