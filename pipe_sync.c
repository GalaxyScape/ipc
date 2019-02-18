#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

//使用管道同步 思路来自TLPI

/**
 * 当管道为空且存在写端口时，管道的读端会被阻塞
 * 当管道为空且所有的写端口都被关闭的时候，对管道的读写会出错返回-1
 * 使用阻塞来实现同步
 */ 

int main(int argc, char *argv[])
{
	int pfd[2];
	int j, dummy;

	setbuf(stdout, NULL);

	if (pipe(pfd) == -1)
	{
		perror("pipe");
	}

	for (j = 0; j < 2; j++)
	{
		switch (fork())
		{
		case -1:
			perror("fork");
		case 0:
			if (close(pfd[0]) == -1)
			{
				perror("close");
			}

			sleep(j + 1);

			printf("Child %d (PID=%ld) closing pipe\n", j,
				   (long)getpid());
			if (close(pfd[1]) == -1)
			{
				perror("close");
			}

			_exit(EXIT_SUCCESS);

		default:
			break;
		}
	}

	if (close(pfd[1]) == -1)
		perror("close");

	if (read(pfd[0], &dummy, 1) != 0)
		perror("parent didn't get EOF");

	printf("  Parent ready to go\n");

	exit(EXIT_SUCCESS);
}
