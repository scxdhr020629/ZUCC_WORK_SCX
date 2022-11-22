#include<stdio.h>
#include<unistd.h>
#include <stdlib.h>

int main()
{
		pid_t pid1,pid2;
		char *argv1[]={"pstree","-a",0};
		char *argv2[]={"grep","login",0};

		int fd[2];

		if(pipe(fd)<0)printf("create pipe erorr");

		if(pid1=fork()==0)
		{
			close(fd[0]);
			close(1);
			dup(fd[1]);
			close(fd[1]);
			if(execvp("pstree",argv1)==-1)
				printf("come on");
		}

		if(pid2=fork() ==0)
		{
			close(fd[1]);
			close(0);
			dup(fd[0]);
			close(fd[0]);
			execvp("grep",argv2);
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

