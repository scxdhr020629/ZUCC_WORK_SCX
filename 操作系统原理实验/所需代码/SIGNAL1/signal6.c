#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include <stdio.h>

main()
{
	pid_t pid;
	int status;
	if(!(pid= fork())){
		//子进程
		printf("child process start!\n");
		sleep(6);
		printf("child process end !\n");
		exit(0);
	}
	else{
		//父进程
		printf("send signal to child process (%d) \n",pid);
		sleep(2);
		//kill(pid ,SIGABRT);
		wait(&status);
		if(WIFSIGNALED(status))
		printf("chile process receive signal %d\n",WTERMSIG(status));
	}
}
