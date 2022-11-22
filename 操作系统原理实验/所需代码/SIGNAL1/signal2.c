#include <signal.h>
#include <unistd.h>
#include <stdio.h>
void sigroutine(int sig)
{ 
	switch (sig) {
		case 2:
			printf("Get a signal -- SIGINT\n");break;
		case 3:
			printf("Get a signal -- SIGQUIT\n");break;
	}
	return;
}

int main() {
	printf("process id is %d\n ",getpid());	
	(void) signal(SIGINT, sigroutine);	//ctr+c
	(void) signal(SIGQUIT, sigroutine);  //ctr+\
	for (;;);
}
