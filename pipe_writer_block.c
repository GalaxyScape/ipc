#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int count = 0;
int read_cnt = 0;
sem_t sem;

void my_alarm_handler(int signo)
{
    printf("Write blocked after %d characters\n", count);
    sem_post(&sem);
    exit(0);
}
/**
 * This is program for block test.
 */ 
int main()
{
    int p[2];
    char c = 'x';

    signal(SIGALRM, my_alarm_handler);
    if (sem_init(&sem, 0, 0) == -1)
    {
        perror("sem_init");
        exit(-1);
    }

    if (pipe(p) == -1)
        exit(1);
    switch (fork())
    {
    case -1:
        perror("fork");

    case 0:
        if (close(p[0]) == -1)
        {
            perror("close");
            exit(0);
        }
        while (1)
        {
            alarm(2);

            //write a char until blocked
            write(p[1], &c, 1);

            ++count;
            alarm(0);
        }
        if (close(p[1]) == -1)
        {
            perror("close");
        }
        exit(0);
    default:
        break;
    }
    char d;
    //father
    sem_wait(&sem);
    if (close(p[1]) == -1)
        perror("close");
    while (1)
    {
        //read a char until blocked
        if (read(p[0], &d, 1) == -1)
        {
            break;
        }
        ++read_cnt;
    }
    printf("PIPE size is %d",read_cnt);
    return 0;
}
