#include <unistd.h>  
#include <stdio.h>  
  
int main( void )  
{  
	int pp[2];  
	char buf[80];  
	pid_t pid;  
	  
	pipe( pp );  
	pid=fork();          
	if (pid > 0)  
	{  
		printf( "This is in the father process,here write a string to the pipe.\n" );  
		char s[] = "Hello world , this is write by pipe.\n";  
		sleep(3);
		write( pp[1], s, sizeof(s) );  
		close( pp[0] );  
		close( pp[1] );  
	}  
	else if(pid == 0)  
	{  
		printf( "This is in the child process,here read a string from the pipe.\n" );  
		read( pp[0], buf, sizeof(buf) );  
		printf( "%s\n", buf );  
		close( pp[0] );  
		close( pp[1] );  
	}  
	  
	waitpid( pid, NULL, 0 );  	  
	return 0;  
}  
