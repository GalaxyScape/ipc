#include <semaphore.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
    int flags, opt;
    mode_t perms;
    unsigned int value;
    sem_t *sem_x;
    sem_t *sem_y;

    flags = 0;

    sem_x = sem_open("read_sync", O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, 0);
    sem_y =
	sem_open("write_sync", O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, 0);

    if (sem_x == SEM_FAILED || sem_y == SEM_FAILED)
	perror("sem_open");

    printf("This should be 1\n");

    //begin
    int fd;
    size_t len;
    char *addr;

    mode_t old_umask = umask(0);

    fd = shm_open("wys_pshm", O_RDWR | O_CREAT | O_EXCL, 0606);

    // restore old
    umask(old_umask);

    if (fd == -1)
	perror("shm_open");
    char arr[100] = { };
    scanf("%s", arr);
    len = strlen(arr) + 1;
    if (ftruncate(fd, len) == -1) {
	perror("ftruncate");
    }
    printf("Resized to %ld bytes\n", (long) len);


    struct stat sb;

    if (fstat(fd, &sb) == -1) {
	perror("stat");
	//exit(EXIT_SUCCESS);
    }

    printf("sb.st_size is %d\n", sb.st_size);

    addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
	perror("mmap");

    memcpy(addr, arr, len);
    //end


    if (sem_post(sem_x) == -1) {
	perror("sem_post");
    }
    printf("After sem_x receiver to go\n");

    if (sem_wait(sem_y) == -1) {
	perror("sem_wait");
    }
    //begin
    memcpy(arr, addr, 5);
    printf("%s from writer\n", arr);
    //end
    if (close(fd) == -1)
	perror("close");

    sem_close(sem_x);
    sem_close(sem_y);
    sem_unlink("read_sync");
    sem_unlink("write_sync");
    shm_unlink("wys_pshm");
    exit(EXIT_SUCCESS);
}
