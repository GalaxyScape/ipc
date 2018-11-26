#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAXMSG 100
int
main()
{
  char msg[][MAXMSG] = { "This is an awesome attempt to go far "
                         "Go go go this is the light of the dawn",
                         "The Hobbit is something amazing" };
  sem_t sem;
  sem_init(&sem, 0, 1);
  // pipe
  int pfd[2];
  int dummy;
  setbuf(stdout, NULL);
  if (pipe(pfd) == -1)
    perror("pipe");

  for (int i = 0; i < 3; ++i) {
    switch (fork()) {
      case -1:
        perror("Fork Error\n");
      case 0: // child
        sem_wait(&sem);
        
	// generate messages

        if (close(pfd[0]) == -1) // read end not used
          perror("close");
        write(pfd[1], msg[i], MAXMSG);
        sem_post(&sem);
        _exit(EXIT_SUCCESS);
      default:
        break;
    }
  }


  printf("Father\n");
  char buffer[MAXMSG];
  if (close(pfd[1]) == -1) /* Write end is unused */
    perror("close");
  for (int i = 0; i < 3; ++i) {
    printf("%d\n", wait(NULL));
    read(pfd[0], buffer, MAXMSG);
    printf("%s\n", buffer);
  }

  exit(EXIT_SUCCESS);
}
