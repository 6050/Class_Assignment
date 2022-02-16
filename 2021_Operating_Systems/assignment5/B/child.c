#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define COUNTING_NUMBER 2000000

struct smStruct{

    int processidassign;
    int turn;
    int flag[2];
    int critical_section_variable;

};

void lock(struct smStruct *smstruct, int Myorder){

    smstruct->flag[Myorder] = 1;
    smstruct->turn = !Myorder;

    while (smstruct->flag[!Myorder] == 1 &&
            smstruct->turn == !Myorder);

}

void unlock(struct smStruct *smstruct, int Myorder){
    
    smstruct->flag[Myorder] = 0;
}

int main(){

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

    // set process ID && Myorder
    int pid = (int)getpid();
    int Myorder;

    if (smstruct->processidassign == 0)
        Myorder = 0;
    else Myorder = 1;

    if (smstruct->processidassign != pid)
        smstruct->processidassign = pid;
    
    printf("Myorder = %d, process pid = %d\n", Myorder, pid);

    int localcount = 0;
    for (int i=0; i<COUNTING_NUMBER; i++){

        localcount++;
        lock(smstruct, Myorder);
        smstruct->critical_section_variable++;
        unlock(smstruct, Myorder);

    }

    // detach the shared memory
    ret = shmdt(shmaddr);
    if (ret == -1){
        perror("detach failed\n");
        return 0;
    }

    printf(" child(%d) finish! local count = %d\n", pid, localcount);

    return 0;

}
