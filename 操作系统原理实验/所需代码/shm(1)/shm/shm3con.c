/* Our first program is a consumer. After the headers the shared memory segment
 (the size of our shared memory structure) is created with a call to shmget,
 with the IPC_CREAT bit specified. */
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define TEXT_SZ 2048
/* The union for semctl may or may not be defined for us.This code,defined
 in linux's semctl() manpage,is the proper way to attain it if necessary */
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
/* union semun is defined by including <sys/sem.h> */
#else
/* according to X/OPEN we have to define it ourselves */
union semun
{
    int val;                   /* value for SETVAL */
    struct semid_ds *buf;      /* buffer for IPC_STAT,IPC_SET */
    unsigned short int *array; /* array for GETALL,SETALL */
    struct seminfo *__buf;     /* buffer for IPC_INFO */
};
#endif
#define SHMDATASIZE 1000
#define BUFFERSIZE (SHMDATASIZE - sizeof(int))
#define SN_READ 0
#define SN_WRITE 1
int Semid = 0; /* 用于最后删除这个信号量 */
void reader(void);
void writer(int shmid);
void delete (void);
void sigdelete(int signum);
void locksem(int semid, int semnum);
void unlocksem(int semid, int semnum);
void waitzero(int semid, int semnum);
void write(int shmid, int semid, char *buffer);
int mysemget(key_t key, int nsems, int semflg);
int mysemctl(int semid, int semnum, int cmd, union semun arg);
int mysemop(int semid, struct sembuf *sops, unsigned nsops);
int myshmget(key_t key, int size, int shmflg);
void *myshmat(int shmid, const void *shmaddr, int shmflg);
int myshmctl(int shmid, int cmd, struct shmid_ds *buf);

/**
 * @brief
 * 上面的代码是信号量的处理代码
 */
struct shared_use_st
{
    int written_by_you;
    char some_text[TEXT_SZ];
};

int main()
{
    union semun sunion;
	int semid;
	void *shmdata;
	char *buffer;
    
    /* 首先：我们要创建信号量 */
	semid = mysemget(IPC_PRIVATE, 2, SHM_R | SHM_W);
	Semid = semid;

	/* 在进程离开时，删除信号量 */
	atexit(&delete);
	signal(SIGINT, &sigdelete);
    
    int running = 1;
    void *shared_memory = (void *)0;
    struct shared_use_st *shared_stuff;
    int shmid;

    srand((unsigned int)getpid());

    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);

    if (shmid == -1)
    {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    /* We now make the shared memory accessible to the program. */

    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1)
    {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    /* The next portion of the program assigns the shared_memory segment to shared_stuff,
     which then prints out any text in written_by_you. The loop continues until end is found
     in written_by_you. The call to sleep forces the consumer to sit in its critical section,
     which makes the producer wait. */

    shared_stuff = (struct shared_use_st *)shared_memory;
    shared_stuff->written_by_you = 0;
    while (running)
    {
        
        
        if (shared_stuff->written_by_you)
        {
            printf("You wrote: %s", shared_stuff->some_text);
            sleep(rand() % 4); /* make the other process wait for us ! */
            shared_stuff->written_by_you = 0;
            if (strncmp(shared_stuff->some_text, "end", 3) == 0)
            {
                running = 0;
            }
        }


    }

    /* Lastly, the shared memory is detached and then deleted. */

    if (shmdt(shared_memory) == -1)
    {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}


void delete (void)
{
    printf("\n quit; delete the semaphore %d \n", Semid);

    /* 删除信号量 */
    if (semctl(Semid, 0, IPC_RMID, 0) == -1)
    {
        printf("Error releasing semaphore.\n");
    }
}
void sigdelete(int signum)
{
    /* Calling exit will conveniently trigger the normal delete item. */
    exit(0);
}
void locksem(int semid, int semnum)
{
    struct sembuf sb;

    sb.sem_num = semnum;
    sb.sem_op = -1;
    sb.sem_flg = SEM_UNDO;

    mysemop(semid, &sb, 1);
}
void unlocksem(int semid, int semnum)
{
    struct sembuf sb;

    sb.sem_num = semnum;
    sb.sem_op = 1;
    sb.sem_flg = SEM_UNDO;

    mysemop(semid, &sb, 1);
}
void waitzero(int semid, int semnum)
{
    struct sembuf sb;

    sb.sem_num = semnum;
    sb.sem_op = 0;
    sb.sem_flg = 0; /* No modification so no need to undo */
    mysemop(semid, &sb, 1);
}
void write(int shmid, int semid, char *buffer)
{
    printf("\n wait for reader to read in information ...");
    fflush(stdout);

    locksem(semid, SN_READ);

    printf("finish \n");
    printf("please input information: ");
    fgets(buffer, BUFFERSIZE, stdin);

    unlocksem(semid, SN_WRITE);
}

int mysemget(key_t key, int nsems, int semflg)
{
    int retval;

    retval = semget(key, nsems, semflg);
    if (retval == -1)
    {
        printf("semget key %d,nsems %d failed: %s ", key, nsems, strerror(errno));
        exit(255);
    }
    return retval;
}
int mysemctl(int semid, int semnum, int cmd, union semun arg)
{
    int retval;

    retval = semctl(semid, semnum, cmd, arg);
    if (retval == -1)
    {
        printf("semctl semid %d,semnum %d,cmd %d failed: %s", semid, semnum, cmd, strerror(errno));
        exit(255);
    }
    return retval;
}

int mysemop(int semid, struct sembuf *sops, unsigned nsops)
{
    int retval;

    retval = semop(semid, sops, nsops);
    if (retval == -1)
    {
        printf("semop semid %d (%d operations) failed: %s", semid, nsops, strerror(errno));
        exit(255);
    }
    return retval;
}
int myshmget(key_t key, int size, int shmflg)
{
    int retval;

    retval = shmget(key, size, shmflg);
    if (retval == -1)
    {
        printf("shmget key %d,size %d failed: %s", key, size, strerror(errno));
        exit(255);
    }
    return retval;
}
void *myshmat(int shmid, const void *shmaddr, int shmflg)
{
    void *retval;

    retval = shmat(shmid, shmaddr, shmflg);
    if (retval == (void *)-1)
    {
        printf("shmat shmid %d failed: %s", shmid, strerror(errno));
        exit(255);
    }
    return retval;
}
int myshmctl(int shmid, int cmd, struct shmid_ds *buf)
{
    int retval;

    retval = shmctl(shmid, cmd, buf);
    if (retval == -1)
    {
        printf("shmctl shmid %d,cmd %d failed: %s", shmid, cmd, strerror(errno));
        exit(255);
    }
    return retval;
}