#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

main( )
{
	int p1,p2;

	while ((p1=fork())==-1);             /*父进程创建第一个进程，直到成功*/
	if(p1==0)                         /*0返回给子进程 1*/
		{
			putchar('b');/*P1的处理过程*/ 
			}
	else   
	{                             /*正数返回给父进程(子进程号)*/
		while ((p2=fork())==-1);         /*父进程创建第二个进程，直到成功*/
		if(p2==0)                      /*0返回给子进程2*/
			{
			putchar('c');/*P2的处理过程*/
			  }            
		else
			{
			putchar('a');/*P2创建完成后，父进程的处理过程*/
			}            
	}
}
