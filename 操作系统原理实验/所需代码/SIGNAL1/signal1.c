#include <signal.h>
#include <stdio.h>
#include <unistd.h>
void ouch(int sig)
{
    int i =0;
	for( i=0;i<5;i++)
	{
		printf("OUCH! - I got signal %d\n", sig);
	}
  //恢复SIGINT信号的处理动作
   (void) signal(SIGINT, SIG_DFL); 
}

int main()
{
	//设置SIGINT信号的处理动作为响应ouch函数
	//(void) signal(SIGINT, SIG_IGN);
	(void) signal(SIGINT, ouch);  
	//主函数每隔1秒输出字符串
	while(1) {
		printf("Hello World!\n");
		sleep(1);
	}
}
