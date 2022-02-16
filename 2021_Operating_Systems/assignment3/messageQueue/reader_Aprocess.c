#include <sys/typesh>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#define BUFSIZE 50

struct msgbuf {
    long type;
    char buf[50];
};


int main(int argc, char **argv) {
    key_t key_id;
    int i, key = 9956;
    struct msgbuf rsvbuf;
    int type = 0;

    key_id = msgget((key_t)key, IPC_CREAT|0666);

    if (key_id == -1) {
        perror("msggget error : ");
        return 0;
    }

    printf("hi !\n");
    while ( 1 ) {
        if (msgrcv(key_id, (void*)&rsvbuf, 
                    sizeof(struct msgbuf), type, 0) == -1) {
            perror("msgrcv error :");
        }
        else {
            if(!strcmp("q", rsvbuf.buf)) {
                printf("bye!\n");
                break;
            }
            printf("The received message is: %s\n", rsvbuf.buf);
        }
    }

    return 0;
}
