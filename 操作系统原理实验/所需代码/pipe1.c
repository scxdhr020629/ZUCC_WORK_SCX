#include <stdio.h>
#include <unistd.h>
#include <errno.h>                                /* perror function */
 
int hPipe[2];                                        /* Handle hPipe[0]=input, hPipe[1]=output */
 
main() 
{
	int count;
	char buff[200];
 
	char msg[] = "Hello World\n";
 
	if ( pipe( hPipe) <0)                                      /* create an anonymous pipe */
        perror("pipe creation");
 
	printf(" msg size is %d, read fd = %d, write = %d \n", sizeof(msg), hPipe[0], hPipe[1]);
 
	count = write( hPipe[1], msg, sizeof(msg) );  /* send message to self via pipe */
	printf(" characters Written to pipe: %d\n", count);
 
	count = read( hPipe[0], buff, sizeof(buff) );
	printf(" characters Read back from pipe: %d\n", count);
 
	write(1, buff, count);                      /* printf( buff) or the non-buffered write */
}
