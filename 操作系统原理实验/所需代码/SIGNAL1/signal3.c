#include<unistd.h>
#include<signal.h>
#include <stdio.h>

void handler() {
    printf("hello\n");
}

main()
{
     int i;
     signal(SIGALRM,handler);
     alarm(3);
     for(i=1;i<7;i++){
           printf("sleep %d ...\n",i);
           sleep(1);
     }
}
