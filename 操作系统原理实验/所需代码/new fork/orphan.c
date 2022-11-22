#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
  
  int main()
  {
      pid_t pid;
      //创建一个进程
     pid = fork();
     //创建失败
     if (pid < 0)
     {
         perror("fork error:");
         exit(0);
     }
     //子进程
     if (pid == 0)
     {
         printf("I am the child process.\n");
         //输出进程ID和父进程ID
         printf("pid: %d\tppid:%d\n", getpid(), getppid());
         printf("I will sleep for five seconds.\n");
         //睡眠5s，保证父进程先退出
         sleep(5);
         printf("pid: %d\tppid:%d\n", getpid(), getppid());
         printf("Child process exited.\n");
     }
     //父进程
     else
     {
         printf("I am the father process.\n");
         //父进程睡眠1s，保证子进程输出进程id
         sleep(1);
         printf("Father process exited.\n");
     }
     exit(0);
 }