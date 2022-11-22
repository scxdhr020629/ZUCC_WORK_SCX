/**
 * @file        main.c
 * @author      Arcana
 * @date        2018.11.12
 * @brief       Children process communicate with parent by pipe.
 */

#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_MAX_SIZE 8192
#define CHECK(x)                                            \
    do {                                                    \
        if (!(x)) {                                         \
            fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
            perror(#x);                                     \
            exit(-1);                                       \
        }                                                   \
    } while (0)

/**
 * Create three children processes to test pipe communication.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return status code.
 */
int main(int argc, char **argv) {
    int pipefd[2], pid, i = 0;
    int flag = 0;
    ssize_t n;
    char buf[BUF_MAX_SIZE];
    char str[BUF_MAX_SIZE];
    // 创建有名信号量，若不存在则创建，若存在则直接打开，初始值为0
	sem_t *write_mutex;
    sem_t *read_mutex1;
    sem_t *read_mutex2;
    write_mutex = sem_open("pipe_test_wm", O_CREAT | O_RDWR, 0666, 0);
    read_mutex1 = sem_open("pipe_test_rm_1", O_CREAT | O_RDWR, 0666, 0);
    read_mutex2 = sem_open("pipe_test_rm_2", O_CREAT | O_RDWR, 0666, 0);

    memset(buf, 0, BUF_MAX_SIZE);
    memset(str, 0, BUF_MAX_SIZE);

    CHECK(pipe(pipefd) >= 0);
    CHECK((pid = fork()) >= 0);

    if (pid == 0) {// 第一个子进程，利用非阻塞写测试管道大小 
        int count = 0;
        close(pipefd[0]);
        int flags = fcntl(pipefd[1], F_GETFL);
        fcntl(pipefd[1], F_SETFL, flags | O_NONBLOCK);
		//管道默认是阻塞写，通过`fcntl`设置成非阻塞写，在管道满无法继续写入时返回-EAGAIN，作为循环终止条件
        while (!flag) {
            n = write(pipefd[1], buf, BUF_MAX_SIZE);// 写入管道
            if (n == -1) {
                flag = 1;
            } else {
                count++;
                printf("children 1 write %dB\n", n);
            }
        }
        printf("space = %dKB\n", (count * BUF_MAX_SIZE) / 1024);
        exit(0);
    }

    CHECK((pid = fork()) >= 0);
    if (pid == 0) {//child2
        sem_wait(write_mutex);
        close(pipefd[0]);
        n = write(pipefd[1], "This is the second children.\n", 29);
        printf("children 2 write %dB\n", n);
        sem_post(write_mutex);
        sem_post(read_mutex1);
        exit(0);
    }

    CHECK((pid = fork()) >= 0);
    if (pid == 0) {//child3
        sem_wait(write_mutex);
        close(pipefd[0]);
        n = write(pipefd[1], "This is the third children.\n", 28);
        printf("children 3 write %dB\n", n);
        sem_post(write_mutex);
        sem_post(read_mutex2);
        exit(0);
    }

    wait(0);// 等待第一个子进程运行完成，父进程继续运行
    close(pipefd[1]);
    int flags = fcntl(pipefd[0], F_GETFL);
    fcntl(pipefd[0], F_SETFL, flags | O_NONBLOCK);// 设置非阻塞性读，作为循环结束标志
    while (!flag) {
        n = read(pipefd[0], str, BUF_MAX_SIZE);
        if (n == -1) {
            flag = 1;
        } else {
            printf("%dB read\n", n);
        }
    }
    sem_post(write_mutex);

    
    sem_wait(read_mutex2);sem_wait(read_mutex1);// 等待子进程二、三写入完毕
    n = read(pipefd[0], str, BUF_MAX_SIZE);
    printf("%dB read\n", n);
    for (i = 0; i < n; i++) {
        printf("%c", str[i]);
    }

    sem_close(write_mutex);
    sem_close(read_mutex1);
    sem_close(read_mutex2);
    sem_unlink("pipe_test_wm");
    sem_unlink("pipe_test_rm_1");
    sem_unlink("pipe_test_rm_2");
    return 0;
}
