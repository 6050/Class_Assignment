#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wait.h> 

int main(){

    pid_t f_pid, s_pid;
    
    if ((f_pid = fork()) > 0){

        if((s_pid = fork()) > 0){
        
            // parent process
            printf("Waiting for Child Processes\n");
            waitpid(-1, NULL, 0);
            waitpid(-1, NULL, 0);
            printf("Child processes are exit\n");

        }
        else{

            // child2 process
            execl("./child2", NULL);

        }

    }
    else{

        // child1 process
        execl("./child1", NULL);

    }

    return 0;

}


