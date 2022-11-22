#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int pid1, pid2;
int i;
char ch;

void scanfALine(char result[]);

int main(void)
{
    int fd0[2], fd1[2];
    char readpipe[200], movepipe[200];

    pipe(fd0);
    pipe(fd1);

    if (pid1 = fork() > 0)
    { // yeye
        wait(0);
        wait(0);
        read(fd1[0], movepipe, 50);
        printf("Print: %s\n", movepipe);

        exit(0);
    }
    else
    {
        if (pid2 = fork() > 0)
        { // erzi
            wait(0);

            read(fd0[0], movepipe, 50);
            printf("Move: %s\n", movepipe);

            printf("Move:Please modify it:");
            scanfALine(movepipe);
            write(fd1[1], movepipe, 50);

            exit(0);
        }
        else
        {
            // sunzi
            printf("Read:Please enter:");
            scanfALine(readpipe);
            write(fd0[1], readpipe, 50);

            exit(0);
        }
    }
}

//输入一行的代码为网上的
void scanfALine(char result[])
{
    while (1)
    {
        scanf("%c", &ch);
        if (ch == '\n')
            break;
        result[i++] = ch;
    }
    result[i] = '\0';
}