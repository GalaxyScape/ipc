#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
    int cnt = 0;
    int p[2];
    char c = 'c';

    if (pipe(p) == -1)
	exit(1);

    if (fcntl(p[1], F_SETFL, O_NONBLOCK) < 0) {
	exit(-3);
    }
    while (1) {

	if (write(p[1], &c, 1) == -1) {
	    perror("write");
	    break;
	}
	++cnt;
    }
    printf("the size of pipe is %d\n", cnt);
    if (close(p[0]) == -1) {
	perror("close p[0]");
    }
    if (close(p[1]) == -1) {
	perror("close p[1]");
    }
    return 0;
}
