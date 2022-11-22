#include <stdio.h>
#include <stdlib.h>

#define MAXSTRS 5
int  main( )
{
	int cntr;
	FILE *pipe_fp;
	char *strings[MAXSTRS] = { "echo", "bravo", "alpha","charlie", "delta"};

	if (( pipe_fp = popen("sort", "w")) == NULL){          //打开管道写数据
		perror("popen");
		exit(1);
	}

	for(cntr=0; cntr<MAXSTRS; cntr++) {
		fputs(strings[cntr], pipe_fp);
		fputc('\n', pipe_fp);
	}

	pclose(pipe_fp);                                  //关闭管道
	return(0);
}
