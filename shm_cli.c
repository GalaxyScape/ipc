#include <semaphore.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//共享内存的发送端

int main(int argc, char *argv[])
{
    sem_t *sem_x;
    sem_t *sem_y;

    sem_x = sem_open("read_sync", O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, 0);
    sem_y =
        sem_open("write_sync", O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, 0);

    if (sem_x == SEM_FAILED || sem_y == SEM_FAILED)
        perror("sem_open");

    //begin
    int fd;
    size_t len;
    char *addr;

    mode_t old_umask = umask(0);

    fd = shm_open("wys_pshm", O_RDWR | O_CREAT | O_EXCL, 0606);

    // restore old
    umask(old_umask);

    if (fd == -1)
    {
        perror("shm_open");
        exit(-1)
    }
    char arr[100] = {};
    scanf("%s", arr);
    len = strlen(arr) + 1;
    if (ftruncate(fd, len) == -1)
    {
        perror("ftruncate");
        exit(-1);
    }
    printf("Resized to %ld bytes\n", (long)len);

    struct stat sb;

    if (fstat(fd, &sb) == -1)
    {
        perror("stat");
        exit(-1);
    }

    printf("sb.st_size is %ld\n", sb.st_size);

    addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("mmap");
        exit(-1);
    }

    memcpy(addr, arr, len);
    //end

    if (sem_post(sem_x) == -1)
    {
        perror("sem_post");
        exit(-1);
    }
    printf("After sem_x receiver to go\n");

    if (sem_wait(sem_y) == -1)
    {
        perror("sem_wait");
        exit(-1);
    }
    //begin
    memcpy(arr, addr, 5);
    printf("%s from writer\n", arr);
    //end
    if (close(fd) == -1)
    {
        perror("close");
        exit(-1);
    }

    sem_close(sem_x);
    sem_close(sem_y);
    sem_unlink("read_sync");
    sem_unlink("write_sync");
    shm_unlink("wys_pshm");
    exit(EXIT_SUCCESS);
}
