/*read.c*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define SHMSZ     27
int main()
{
    int shmid;
    key_t key;
    char *shm, *s;
char c;
   	key = 5679;
	if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
        		perror("shmget");
        		exit(1);
    	}

	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        		perror("shmat");
        		exit(1);
   	}
	for (s = shm; *s != '\0'; s++)
        		putchar(*s);
    	putchar('\n');

    	//exit(0);
      c = getchar();
	shmctl(shmid,IPC_RMID,NULL);
}
