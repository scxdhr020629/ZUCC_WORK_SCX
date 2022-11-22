//"hello world"单线程程序
# include <stdio.h>
const NUM = 5;

int main()
{
   void p_msg(char *);

	p_msg("hello");
	p_msg("world\n");
}
void p_msg(char *m)
{
	int i;
	for(i = 0 ; i < NUM ; i ++)
	{
		printf("%s",m);
		fflush(stdout);
		sleep(1);
	}
}
