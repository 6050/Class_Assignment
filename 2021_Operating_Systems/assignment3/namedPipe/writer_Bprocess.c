#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MSG_SIZE 80
#define STR_SIZE 40
#define PIPENAME "./named_pipe_file"

int main() {
    int ret, i;
    char msg[MSG_SIZE];
    char *str;
    int fd;
    size_t strSize = STR_SIZE;

    // open the named pipe
    fd = open(PIPENAME, O_WRONLY);
    if(fd < 0) {
        printf("Open failed\n");
        return 0;
    }

    str = (char*)malloc(strSize*sizeof(char));
    printf("B process starts !\n");
    while( 1 ) {
        if(!strcmp(str, "q")) {
            printf("B process ends. Bye!\n");
            return 0;
        }
        getline(&str, &strSize, stdin);
        str[strlen(str)-1] = '\0';
        snprintf(msg, sizeof(msg), "%s", str);
        ret = write(fd, msg, sizeof(msg));
        if (ret < 0) {
            printf("Write failed\n");
            return 0;
        }
    }
    return 0;
}
