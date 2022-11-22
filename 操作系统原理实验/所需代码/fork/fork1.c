#include<stdio.h>

main()
{
	int pid;
	pid = fork();
	printf("pid= %d\n",pid);
	//printf("pid= %d\n",getpid());
	sleep(1);	
}
