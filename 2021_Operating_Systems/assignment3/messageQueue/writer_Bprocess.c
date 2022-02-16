#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFSIZE 50

struct msgbuf {
    long type;
    char buf[50];
};

int main() {
    key_t key_id;
    int i, key = 9956;
    struct msgbuf sndbuf;
    char *str;
    ssize_t strSize = 40;
    key_id = msgget((key_t)key, IPC_CREAT|0666);

    if (key_id == -1) {
        perror("msggget error : ");
        return 0;
    }

    sndbuf.type = 3;
    str = (char*)malloc(strSize*sizeof(char));
    printf("hi !\n");

    while ( 1 ) {
        getline(&str, &strSize, stdin);
        str[strlen(str) - 1] = '\0';
        strcpy(sndbuf.buf, str);

        if (msgsnd( key_id, (void*)&sndbuf, sizeof(struct msgbuf), IPC_NOWAIT) 
                == -1) {
            perror("msgsnd error : ");
        }
        if(!strcmp("q", str)) {
            printf("bye!\n");
            break;
        }
        printf("sending msg is succeed\n");
    }

    return 0;
}

