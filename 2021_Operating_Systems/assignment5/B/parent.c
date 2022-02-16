#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define COUNTING_NUMBER 2000000
struct smStruct{

    int processidassign;
    int turn;
    int flag[2];
    int critical_section_variable;

};

int main(){

    pid_t pid;

    struct smStruct *smstruct;
    int shmid;
    void *shmaddr;
    int ret, key = 9956;
    
    // get shared memory id
    shmid = shmget((key_t)key, sizeof(struct smStruct), IPC_CREAT|0666);
    if (shmid == -1){
        perror("shared memory access is failed\n");
        return 0;
    }

    // attach the shared memory
    shmaddr = shmat(shmid, (void*)0, 0);
    if (shmaddr == (void*)-1){
        perror("attach failed\n");
        return 0;
    }

    smstruct = shmaddr;
    
    // init shared memory mstruct
    smstruct->turn = 0;
    smstruct->processidassign = 0;
    smstruct->flag[0] = 0;
    smstruct->flag[1] = 0;
    smstruct->critical_section_variable = 0;

    pid = fork();
    if (pid == 0)
        execl("./child", "./child", NULL);
    else{

        pid = fork();
        if (pid == 0)
            execl("./child", "./child", NULL);
        else{

            waitpid(-1, NULL, 0);
            waitpid(-1, NULL, 0);
            printf("Actual Count: %d | Expected Count: %d\n",
                    smstruct->critical_section_variable, COUNTING_NUMBER*2);
        }
    
    }

    // detach the shared memory
    ret = shmdt(shmaddr);
    if (ret == -1){
        perror("detach failed\n");
        return 0;
    }

    // remove the shared memory
    ret = shmctl(shmid, IPC_RMID, 0);
    if (ret == -1){
        perror("remove failed\n");
        return 0;
    }
    
    return 0;

}
