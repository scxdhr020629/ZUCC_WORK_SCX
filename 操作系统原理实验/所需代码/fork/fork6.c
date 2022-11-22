#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main()
{
pid_t pid;

	/* fork a child process */
	pid = fork();

	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed\n");
		exit(-1);
	}
	else if (pid == 0) { /* child process */
		printf("I am the child,my pid =  %d,my parent's pid = %d\n",getpid(),getppid());
		execlp("/bin/ls","ls",NULL);
		//execlp("/bin/ps","ps",NULL);
		sleep(2);
	}
	else { /* parent process */
		/* parent will wait for the child to complete */	
		wait(NULL);
		printf("I am the parent,my pid = %d,my child's pid = %d\n",getpid(),pid);	
		printf("Child Complete\n");
		exit(0);
	}
}
