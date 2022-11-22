#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main( )
{
	int data_processed;
	int file_pipes[2];
	char buffer[100];
	pid_t fork_result;

	
	if ( pipe(file_pipes) == 0){
		fork_result = fork();
		if (fork_result == -1) { 
			fprintf(stderr, "Fork failure");
			exit(EXIT_FAILURE);
		}

		if (fork_result == 0) {                             //子进程
			//sleep(1);
			data_processed = read(file_pipes[0], buffer, BUFSIZ);
			printf("child:Read %d bytes: %s\n", data_processed, buffer);
			sleep(1);
			exit(EXIT_SUCCESS);
		}
		else {                                         //父进程
			printf("input a string:");
			scanf("%s",buffer);
			data_processed = write(file_pipes[1], buffer,strlen(buffer));
			wait(0);
			printf("parent:Wrote %d bytes\n", data_processed);
		}
	}
}
