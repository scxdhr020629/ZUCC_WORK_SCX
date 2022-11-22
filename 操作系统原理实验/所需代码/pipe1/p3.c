#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main()
{
		pid_t p1;
		pid_t p2;
		char *argv1[]={"echo","good good morning",0};
	//	char *argv2[]={"sed","s/good/hi/",0};
		char *argv2[]={"sed","s/good/hi/g",0};

		int fd[2];

		if(pipe(fd)<0)printf("create pipe erorr");

		if(p1=fork()==0)
		{
			close(fd[0]);
			close(1);
			dup(fd[1]);
			close(fd[1]);//sleep(3);
			if(execvp("echo",argv1)==-1)
				printf("come on");
			sleep(1);
		}

		if(p2=fork()==0)
		{
			close(fd[1]);
			close(0);
			dup(fd[0]);
			close(fd[0]);//sleep(3);
			execvp("sed",argv2);
			sleep(1);
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

