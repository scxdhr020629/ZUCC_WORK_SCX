#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/syscall.h>
/*全局变量*/
int gnum = 0;
/*互斥量 */
pthread_mutex_t mutex;
/*声明线程运行服务程序*/
static void pthread_add2 (void);
static void pthread_add3 (void);

pid_t gettid() {
	return syscall(SYS_gettid);
}

int main (void)
{
 /*线程的标识符*/
  pthread_t pt_1 = 0;
  pthread_t pt_2 = 0;
  int ret = 0;
  printf ("main programme start,pid = %d,num = %d!\n",getpid(),gnum);
  /*互斥初始化*/
  pthread_mutex_init (&mutex, NULL);
  /*分别创建线程1、2*/
  ret = pthread_create( &pt_1, NULL,(void *)pthread_add2, NULL);  
  ret = pthread_create( &pt_2, NULL,(void *)pthread_add3, NULL); 
  /*等待线程1、2的结束*/
  pthread_join (pt_1, NULL);
  pthread_join (pt_2, NULL);

  printf ("main programme exit!\n");
  return 0;
}

/*线程1的服务程序*/
static void pthread_add2 (void)
{
  int i = 0;
  printf ("This is pthread_1!PID = %d,LWP = %d,tid = %lu\n",getpid(),gettid(),pthread_self());
  for( i=0; i<3; i++ ){
    pthread_mutex_lock(&mutex); /*获取互斥锁*/
    gnum++; sleep (1); gnum++;/*临界资源*/
    printf ("Thread_1 add 2 to num:%d\n",gnum);
    pthread_mutex_unlock(&mutex); /*释放互斥锁*/
    usleep(1000);
    }
  pthread_exit ( NULL );
}
/*线程2的服务程序*/
static void pthread_add3 (void)
{
  int i = 0;
  printf ("This is pthread_2!PID = %d,LWP = %d,tid = %lu\n",getpid(),gettid(),pthread_self());
  for( i=0; i<4; i++ )  {
    pthread_mutex_lock(&mutex); /*获取互斥锁*/
    gnum++; sleep (1);gnum++; sleep (1);gnum++; /*临界资源*/
    printf ("Thread_2 add 3 to num:%d\n",gnum);
    pthread_mutex_unlock(&mutex); /*释放互斥锁*/
    usleep(1000);
     }
  pthread_exit ( NULL );
} 
