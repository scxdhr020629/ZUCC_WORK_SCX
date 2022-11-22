/**** reader_writer1.c 一个利用共享内存进行进程通信的程序 ****/
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

/* The union for semctl may or may not be defined for us.This code,defined
 in linux's semctl() manpage,is the proper way to attain it if necessary */
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
/* union semun is defined by including <sys/sem.h> */
#else
/* according to X/OPEN we have to define it ourselves */
union semun
{
	int val;				   /* value for SETVAL */
	struct semid_ds *buf;	   /* buffer for IPC_STAT,IPC_SET */
	unsigned short int *array; /* array for GETALL,SETALL */
	struct seminfo *__buf;	   /* buffer for IPC_INFO */
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

int main(int argc, char *argv[])
{
	/* 没有其它的参数，则为 reader */

	if (argc < 2)
	{
		reader();
	}
	else
	{
		writer(atoi(argv[1]));
	}
	return 0;
}
void reader(void)
{
	union semun sunion;
	int semid, shmid;
	void *shmdata;
	char *buffer;

	/* 首先：我们要创建信号量 */
	semid = mysemget(IPC_PRIVATE, 2, SHM_R | SHM_W);
	Semid = semid;

	/* 在进程离开时，删除信号量 */
	atexit(&delete);
	signal(SIGINT, &sigdelete);

	/* 信号量 SN_READ 初始化为 1(锁定)，SN_WRITE 初始化为 0（未锁定）*/
	sunion.val = 1;
	mysemctl(semid, SN_READ, SETVAL, sunion);

	sunion.val = 0;
	mysemctl(semid, SN_WRITE, SETVAL, sunion);

	/* 现在创建一块共享内存 */
	shmid = myshmget(IPC_PRIVATE, SHMDATASIZE, IPC_CREAT | SHM_R | SHM_W);

	/* 将该共享内存映射到进程的虚存空间 */
	shmdata = shmat(shmid, 0, 0);

	/* 将该共享内存标志为已销毁的，这样在使用完毕后，将被自动销毁*/
	shmctl(shmid, IPC_RMID, NULL);

	/* 将信号量的标识符写入共享内存，以通知其它的进程 */

	*(int *)shmdata = semid;

	buffer = shmdata + sizeof(int);

	printf("\n reader begin to run，and the id of share memory is %d ** \n", shmid);

	/***********************************************************
	reader 的主循环
	************************************************************/

	while (1)
	{
		// printf(" \n wait for the writer’s output information ...");
		// fflush(stdout);

		// locksem(semid, SN_WRITE);

		// printf(" finish \n");
		// printf(" received information: %s \n", buffer);

		// unlocksem(semid, SN_READ);
		locksem(semid, SN_WRITE);
		printf("%s", buffer);
		sleep(rand() % 4);
		unlocksem(semid, SN_READ);
	}
}
void writer(int shmid)
{
	int semid;
	void *shmdata;
	char *buffer;

	/* 将该共享内存映射到进程的虚存空间 */
	shmdata = myshmat(shmid, 0, 0);

	semid = *(int *)shmdata;
	buffer = shmdata + sizeof(int);

	printf(" \n writer begin to run，the id of share memory is %d, the semaphore is %d \n", shmid, semid);
	/***********************************************************
	writer 的主循环
	************************************************************/
	while (1)
	{
		char input[3];

		printf("\n menu \n 1.send a message \n");
		printf(" 2.quit \n");
		printf("input your choice（1-2）:");

		fgets(input, sizeof(input), stdin);

		switch (input[0])
		{
		case '1':
			write(shmid, semid, buffer);
			break;
		case '2':
			exit(0);
			break;
		}
	}
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
	// printf("\n wait for reader to read in information ...");
	// fflush(stdout);

	// locksem(semid, SN_READ);

	// printf("finish \n");
	// printf("please input information: ");
	// fgets(buffer, BUFFERSIZE, stdin);

	// unlocksem(semid, SN_WRITE);

	locksem(semid, SN_READ);
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
