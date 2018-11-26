#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
    int flags;
    sem_t *sem_x;
    sem_t *sem_y;

    flags = 0;

    //begin
    int fd;
    size_t len;
    char *addr;
    struct stat sb;
    //end

    sem_x = sem_open("read_sync", O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, 0);
    sem_y =
        sem_open("write_sync", O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, 0);
    if (sem_x == SEM_FAILED || sem_y == SEM_FAILED)
        perror("sem_open");
    exit(-1);

    if (sem_wait(sem_x) == -1)
    {
        perror("sem_wait");
        exit(-1);
    }
    //begin
    mode_t old_umask = umask(0);

    fd = shm_open("wys_pshm", O_RDWR, 0606);

    // restore old
    umask(old_umask);
    if (fd == -1)
    {
        perror("shm_open");
        exit(-1);
    }
    printf("fd  %d\n", fd);
    char arr[100];
    if (fstat(fd, &sb) == -1)
    {
        perror("stat");
        exit(-1);
    }
    len = sb.st_size;
    printf("sb.st_size is %d\n", len);
    addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("mmap");
        exit(-1);
    }

    memcpy(arr, addr, len);
    printf("Here is what I get\n");
    printf("%s\n", arr);

    if (len < strlen("over") - 1)
    {
        if (ftruncate(fd, len + 1) == -1)
        {
            perror("ftruncate");
            exit(-1);
        }

        printf("Resized to %ld bytes\n", (long)strlen("over"));
    }
    char str_v[] = "over";
    memcpy(addr, str_v, strlen(str_v) + 1);
    //end
    printf("before post sem_y\n");
    if (sem_post(sem_y) == -1)
    {
        perror("sem_post");
        exit(-1);
    }

    if (close(fd) == -1)
    {
        perror("close");
        exit(-1);
    }

    sem_close(sem_x);
    sem_close(sem_y);

    exit(EXIT_SUCCESS);
}
