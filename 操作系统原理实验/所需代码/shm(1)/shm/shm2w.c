/*write.c*/
#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>

#define SHMSZ     27
int main()
{
    char c;
    int shmid;
    key_t key;
    char *shm, *s;

    key = 5679;

   if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        		perror("shmget");
        		exit(1);
    	}

    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        		perror("shmat");
        		exit(1);
    	}
    s = shm;
    for (c = 'a'; c <= 'z'; c++)
        		*s++ = c;
			//{*s++ = c;sleep(1);}
    *s = '\0';   
   c = getchar();
    
}

