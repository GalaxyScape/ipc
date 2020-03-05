#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

/// \brief 另一种测试管道大小的思路，借助将管道置为NON_BLOCK

int main()
{
    int cnt = 0;
    int p[2];
    char c = 'c';

    if (pipe(p) == -1)
        exit(1);

    if (fcntl(p[1], F_SETFL, O_NONBLOCK) < 0)
    {
        goto failed;
    }
    while (1)
    {

        if (write(p[1], &c, 1) == -1)
        {
            printf("write fail with errno: %d\n", errno);
            printf("errno : 11 => EAGAIN\n");
            break;
        }
        ++cnt;
    }
    printf("the size of pipe is %d\n", cnt);
    
    close(p[0]);
    close(p[1]);

    return 0;

failed:
    close(p[0]);
    close(p[1]);
    exit(-1);
}
