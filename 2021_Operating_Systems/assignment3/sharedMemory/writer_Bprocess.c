#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
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
    int i, ret, key = 9956;
    struct mem *shmaddr;
    char *str;
    ssize_t strSize = 60;

    printf("hi !\n");
    str = (char*)malloc(MAX_LEN * sizeof(char));

    
    // make a shared memory
    shmid = shmget((key_t)key, sizeof(struct mem), IPC_CREAT|0666);
    if(shmid < 0) {
        perror("shmget");
        return 0;
    }

    //attach the shared memory
    shmaddr = (struct mem*)shmat(shmid, (void *)0, 0);
    if(shmaddr == -1) {
        perror("attach failed\n");
        return 0;
    }

    shmaddr -> status = NOTREADY;
    
    while ( 1 ) {
        getline(&str, &strSize, stdin);
        str[strlen(str) - 1] = '\0';
        strcpy(shmaddr->buf, str);
        shmaddr->status = READY;
        if(!strcmp(str, "q")) {
            printf("bye !\n");
            break;
        }
    }

    //detach the shared memory
    ret = shmdt(shmaddr);
    if(ret == -1) {
        perror("detach failed\n");
        return 0;
    }

    return 0;
}
