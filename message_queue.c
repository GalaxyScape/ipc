#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

sem_t semx, semy, sem_rec, sem_1, sem_2;
const char name[] = "/queue_for_ipc";

static void *Sender1()
{

    char tr[] = "exit";
    mqd_t mqs;
    mode_t old_umask = umask(0);

    mqs = mq_open(name, O_CREAT | O_RDWR, 0606, NULL);

    // restore old
    umask(old_umask);
    if (mqs == -1) {
	perror("mq_open sender1");
	exit(-1);
    }

    struct mq_attr attr;
    if (mq_getattr(mqs, &attr) == -1) {
	perror("mq_getattr sender1");
	exit(-1);
    }

    char store[100] = { };
    while (scanf("%s", store) && strcmp(store, tr) != 0) {
	if (sem_wait(&sem_1) == -1) {
	    perror("sem_wait sem_1");
	    exit(-1);
	}
	printf("\n%s input of Sender 1\n", store);
	if (mq_send(mqs, store, strlen(store) + 1, 0)) {
	    perror("mq_send");
	    exit(-1);
	}

	if (sem_post(&sem_1) == -1) {
	    perror("sem_post sem_1");
	    exit(-1);
	}
    }
    char end1[] = "end1";
    if (sem_wait(&sem_2)) {
	perror("sem_wait sem_2");
	exit(-1);
    }
    if (mq_send(mqs, end1, strlen(end1) + 1, 0)) {
	perror("mq_send");
	exit(-1);
    }
    char from_main[100];
    printf("pre waiting\n");

    if (sem_wait(&semx) == -1) {
	perror("sem_wait semx");
	exit(-1);
    }
    printf("after waiting\n");
    if (mq_receive(mqs, from_main, attr.mq_msgsize, NULL) == -1) {
	perror("mq_receive");
	exit(-1);
    }
    printf(ANSI_COLOR_GREEN "%s from Sender1" ANSI_COLOR_RESET "\n",
	   from_main);
    if (sem_post(&sem_rec) == -1) {
	perror("sem_post sem_rec");
	exit(-1);
    }

    printf("post sem_rec\n");
    if (sem_post(&sem_2) == -1) {
	perror("sem_post sem_2");
	exit(-1);
    }
    printf("post sem_2\n");
    return (void *) 0;
}

static void *Sender2()
{

    char tr[] = "exit";
    mqd_t mqs;
    mode_t old_umask = umask(0);

    mqs = mq_open(name, O_CREAT | O_RDWR, 0606, NULL);

    // restore old
    umask(old_umask);
    if (mqs == -1) {
	perror("mq_open sender2");
	exit(-1);
    }

    struct mq_attr attr;
    if (mq_getattr(mqs, &attr) == -1) {
	perror("mq_getattr sender2");
	exit(-1);
    }

    char store[100] = { };
    while (scanf("%s", store) && strcmp(store, tr) != 0) {
	if (sem_wait(&sem_2) == -1) {
	    perror("sem_wait sem_2");
	    exit(-1);
	}

	printf("\n%s input of Sender 2\n", store);
	if (mq_send(mqs, store, strlen(store) + 1, 0) == -1) {
	    perror("mq_send");
	    exit(-1);
	}
	//sem_post(&sem_rec);
	if (sem_post(&sem_2) == -1) {
	    perror("sem_post sem_2");
	    exit(-1);
	}
    }
    char end2[] = "end2";
    if (sem_wait(&sem_1) == -1) {
	perror("sem_wait sem_1");
	exit(-1);
    }
    if (mq_send(mqs, end2, strlen(end2) + 1, 0) == -1) {
	perror("mq_send");
	exit(-1);
    }
    char from_main[100];
    printf("pre waiting\n");
    if (sem_wait(&semy) == -1) {
	perror("sem_wait semy");
	exit(-1);
    }
    printf("after waiting\n");
    if (mq_receive(mqs, from_main, attr.mq_msgsize, NULL) == -1) {
	perror("mq_receive");
	exit(-1);
    }
    printf(ANSI_COLOR_GREEN "%s from Sender2" ANSI_COLOR_RESET "\n",
	   from_main);

    if (sem_post(&sem_rec) == -1) {
	perror("sem_post sem_rec");
    }
    printf("post sem_rec\n");
    if (sem_post(&sem_1) == -1) {
	perror("");
	exit(-1);
    }
    printf("post sem_1\n");
    return (void *) 0;
}

int main(void)
{
    pthread_t t1, t2;
    void *res, *ret;
    int s, t;

    s = pthread_create(&t1, NULL, Sender1, NULL);
    t = pthread_create(&t2, NULL, Sender2, NULL);
    sem_init(&semx, 0, 0);
    sem_init(&semy, 0, 0);
    sem_init(&sem_rec, 0, 0);
    sem_init(&sem_1, 0, 1);
    sem_init(&sem_2, 0, 1);
    if (s != 0)
	perror("pthread_create");
    if (t != 0)
	perror("pthread_create");

    //sleep(3); 
    printf("Message from main()\n");


    char end1[] = "end1";
    char end2[] = "end2";
    char over1[] = "over1";
    char over2[] = "over2";

    mqd_t handler;
    struct mq_attr attr;

    mode_t old_umask = umask(0);

    handler = mq_open(name, O_CREAT | O_RDWR, 0666, NULL);

    // restore old
    umask(old_umask);
    //handler = mq_open (name, O_CREAT | O_RDWR, 0666);
    if (handler == -1) {
	perror("mq_open");
    }
    if (mq_getattr(handler, &attr) == -1) {
	perror("mq_getattr main");
	exit(-1);
    }

    int cnt = 0;		//num of end thread
    char echo[100] = { };
    while (1) {
	//sem_wait(&sem_rec);
	mq_receive(handler, echo, attr.mq_msgsize, NULL);
	printf("%s from Receiver\n", echo);
	if (strcmp(echo, end1) == 0) {
	    sem_post(&semx);
	    printf("Post semx\n");
	    mq_send(handler, over1, strlen(over1), 0);
	    sem_wait(&sem_rec);
	    printf("send over1\n");
	    ++cnt;
	} else if (strcmp(echo, end2) == 0) {
	    sem_post(&semy);
	    printf("Post semy\n");
	    mq_send(handler, over2, strlen(over2), 0);
	    printf("send over2\n");
	    sem_wait(&sem_rec);
	    ++cnt;
	}
	if (cnt == 2) {
	    break;
	}
    }

    // sem_post(&semx);
    // sem_post(&semy);

    s = pthread_join(t1, &res);
    t = pthread_join(t2, &ret);
    if (s != 0)
	perror("pthread_join");
    if (t != 0)
	perror("pthread_join");


    printf("END OF WHOLE PROCESS\n");

    exit(EXIT_SUCCESS);
}
