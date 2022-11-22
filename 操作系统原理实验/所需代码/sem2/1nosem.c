#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char message = '.';
	int i = 0;
	if (argc > 1)
		message = 'O';
	
	for (i = 0; i < 10; ++i)
	{
		printf("%c", message);// 向屏幕中输出数据
		fflush(stdout);// 清理缓冲区，然后休眠随机时间
		sleep(rand()%3);
		printf("%c", message);// 离开临界区前再一次向屏幕输出数据
		fflush(stdout);
		sleep(rand()%2);// 离开临界区，休眠随机时间后继续循环
	}
	sleep(3);
	printf("\n%dfinished\n",getpid());
	exit(EXIT_SUCCESS);
}
