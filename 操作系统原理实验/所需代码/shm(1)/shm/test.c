// #include <unistd.h>
// #include <sys/ipc.h>
// #include <sys/shm.h>
// #include <errno.h>
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>

// #include <signal.h>

// int wait_mark;
// void waiting(), stop();
// void intToString(int a, char b[]);

// #define KEY 1234 /*��*/
// #define SIZE 1024

// int main()
// {
//     int shmid;
//     char *shmaddr;
//     struct shmid_ds buf;
//     char string1[16] = {0};
//     char string2[16] = {0};
//     int t;

//     shmid = shmget(KEY, SIZE, IPC_CREAT | 0600);
//     if (shmid == -1)
//     {
//         printf("create share memory failed:%s", strerror(errno));
//         return 0;
//     }

//     if ((t = fork()) == 0)
//     { /*�ӽ���*/
//         sleep(2);
//         shmaddr = (char *)shmat(shmid, NULL, 0); /*ϵͳ�Զ�ѡ��һ����ַ����*/
//         if (shmaddr == (void *)-1)
//         {
//             printf("connect to the share memory failed:%s", strerror(errno));
//             return 0;
//         }
//         /*�����ڴ���д����*/
//         int pid_son = getpid();
//         intToString(pid_son, string1);
//         // itoa(pid_son, string1, 10);
//         strcpy(shmaddr, string1);
//         printf("子进程读入\n");
//         printf("%s\n", shmaddr);

//         /*�Ͽ������ڴ�*/
//         // strcpy(shmaddr, ' ');

//         kill(getppid(), 17);
//         wait_mark = 1;
//         signal(16, stop);
//         waiting();
//         printf("END\n");

//         printf("%s", shmaddr);
//         shmdt(shmaddr);
//         exit(0);
//     }
//     else
//     { /*������*/
//         wait_mark = 1;
//         signal(17, stop);
//         waiting();
//         shmctl(shmid, IPC_STAT, &buf); /*ȡ�ù����ڴ�������Ϣ*/

//         shmaddr = (char *)shmat(shmid, NULL, 0); /*ϵͳ�Զ�ѡ��һ����ַ����*/
//         if (shmaddr == (void *)-1)
//         {
//             printf("connect the share memory failed:%s", strerror(errno));
//             return 0;
//         }
//         int pid_father = getpid();
//         intToString(pid_father, string2);
//         // itoa(pid_father, string2, 10);
//         strcpy(shmaddr, string2);
//         printf("父进程读入\n");
//         printf("%s\n", shmaddr);
//         kill(t, 16);
//         wait(0);
//         shmdt(shmaddr);                /*�Ͽ������ڴ�*/
//         shmctl(shmid, IPC_STAT, &buf); /*ȡ�ù����ڴ�������Ϣ*/

//         shmctl(shmid, IPC_RMID, NULL);
//     }
// }
// void waiting()
// {
//     while (wait_mark != 0)
//         ;
// }
// void stop()
// {
//     wait_mark = 0;
// }
// void intToString(int a, char b[])
// {
//     int i, m;
//     char c[20];
//     for (i = 1; a / i != 0; i *= 10)
//         ;
//     m = i / 10;

//     for (i = 0; m != 0; i++, m /= 10)
//     {
//         c[i] = (char)(48 + a / m);
//         a -= a / m * m;
//     }
//     c[i + 1] = '\0';

//     for (i = 0; c[i] != '\0'; i++)
//         b[i] = c[i];
// }

#include <unistd.h>
#include <sys/ipc.h>
#include <
sys / shm.h >
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define KEY 1234  /*键*/
#define SIZE 1024 /*欲建立的共享内存的大小*/

    int main()
{
    int shmid;
    char *shmaddr;
    struct shmid_ds buf;
    shmid =
        shmget(KEY, SIZE, IPC_CREAT | 0600); /*建立共享内存*/
    if (shmid == -1)
    {
        printf("create share memory failed:%s", strerror(errno));
        return 0;
    }
    if (fork() == 0)
    { /*子进程*/
        sleep(1);
        shmaddr = (char *)
            shmat(shmid, NULL, 0); /*系统自动选择一个地址连接*/
        if (shmaddr == (void *)-1)
        {
            printf("connect to the share memory failed:%s", strerror(errno));
            return 0;
        }
        /*向共享内存内写数据*/
        strcpy(
            shmaddr, "hello world！\n");

        shmdt(shmaddr); /*断开共享内存*/
        exit(0);
    }
    else
    { /*父进程*/
        wait(0);
        shmctl(shmid,
               IPC_STAT, &buf); /*取得共享内存的相关信息*/
        printf("size of the share memory: shm_segsz=%dbytes\n", buf.shm_segsz);
        printf("process id of the creator:shm_cpid=%d\n", buf.shm_cpid);
        printf("process id of the last operator:shm_lpid=%d\n\n", buf.shm_lpid);
        shmaddr = (char *)shmat(shmid, NULL, 0);
        if (shmaddr == (void *)-1)
        {
            printf("connect the share memory failed:%s", strerror(errno));
            return 0;
        }
        printf("print the content of the share memory:\t");
        printf("%s\n", shmaddr);
        shmdt(shmaddr);
        /*当不再有任何其它进程使用该共享内存时系统将自动销毁它*/

        shmctl(shmid,
               IPC_RMID, NULL);
    }
}