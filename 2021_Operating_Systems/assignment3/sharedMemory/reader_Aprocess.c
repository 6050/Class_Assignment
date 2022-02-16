#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#define MAX_LEN 80                                                              
#define READY 1                                                                 
#define NOTREADY -1

struct mem {
    char buf[100];
    int status;
};

int main() {
    int shmid;
    struct mem *shmaddr;
    int ret, key = 9956;

    // get shared memory id
    shmid = shmget((key_t)key, sizeof(struct mem), IPC_CREAT|0666);
    if(shmid == -1) {
        perror("shared memory access is failed\n");
        return 0;
    }

    //attach the shared memory
    shmaddr = (struct mem*)shmat(shmid, (void *)0, 0);
    if(shmaddr == -1) {
        perror("attach failed\n");
        return 0;
    }

    printf("hi !\n");

    while( 1 ) {
        while(shmaddr->status != READY) {
            continue;
        }
        if(!strcmp("q", shmaddr->buf)) {
            printf("bye !\n");
            break;
        }
        printf("data read from shared memory: %s\n", shmaddr->buf);
        shmaddr->status = NOTREADY;
    }


    //detach the shared memory
    ret = shmdt(shmaddr);
    if(ret == -1) {
        perror("detach failed\n");
        return 0;
    }

    //remove the shared memory
    ret = shmctl(shmid, IPC_RMID, 0);
    if(ret == -1) {
        perror("remove failed\n");
        return 0;
    }

    return 0;
}
