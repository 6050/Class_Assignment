#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Set number of philosophers
// It can be dynamic
 
#define PHILOSOPHER_NUMBER 6

// Chopsticks for philosophers  
sem_t chopstick[PHILOSOPHER_NUMBER];

// Argument for pthread_create 
void *func(void*);

// Instead of pthread_broad_cast,
// it starts worker_threads
// simultaneously when all threads are
// successfully created.
int flag = 1;

int main() {

    long long i;
    pthread_t tid[PHILOSOPHER_NUMBER];

    // init semaphores 
    for (i=0; i<PHILOSOPHER_NUMBER; i++) 
        sem_init(&chopstick[i], 0, 1);
    
    // Do infinite loop until they got deadlock
    // In this case, they never stop (No deadlock)
    while ( 1 ) {

        // Create all threads 
        for (i=0; i<PHILOSOPHER_NUMBER; i++) 
            pthread_create(&tid[i], NULL, func, (void*)i);

        // For the very first time threads are created 
        // Start all threads in the same timing
        if(flag)
            flag = 0;

        // Collect all threads 
        for (i=0; i<PHILOSOPHER_NUMBER; i++)
            pthread_join(tid[i], NULL);
    
    }

}

// Core section for threads.
// Some philosophers wait for a chopstick.
// In other word, at least one philosopher
// can't have any chopsticks in his/her both hands
void *func(void *n) {

    // His/her right hand 
    long long num = (long long)n;
    // His/her left hand 
    long long _num = (num + 1) % PHILOSOPHER_NUMBER;

    // Wait for synchronization from main_thread 
    while(flag)
        ;

    // Case 0:
    // even indexed philosophers try to pick R chopstick first,
    // Then try to pick L chopstick
    if (num % 2 == 0) {
    
        sem_wait(&chopstick[num]);
        printf(" Philosopher[%lld] picked %lldth chopstick\n", num, num);
        sem_wait(&chopstick[_num]);
        printf(" Philosopher[%lld] picked %lldth chopstick\n", _num, _num);
    
    // Case 1:
    // odd indexed philosophers try to pick L chopstick first,
    // Then try to pick R chopstick
    } else {
    
        sem_wait(&chopstick[_num]);
        printf(" Philosopher[%lld] picked %lldth chopstick\n", _num, _num);
        sem_wait(&chopstick[num]);
        printf(" Philosopher[%lld] picked %lldth chopstick\n", num, num);
    
    }

    printf("\n***Philosopher[%lld] : Eating***\n\n", num);
    sleep(1);

    // Case 0:
    // even indexed philosophers release L chopstick first.
    // Then release R chopstick
    if (num % 2 == 0) {

        sem_post(&chopstick[_num]);
        printf(" Philosopher[%lld] released %lldth chopstick\n", _num, _num);
        sem_post(&chopstick[num]);
        printf(" Philosopher[%lld] released %lldth chopstick\n", num, num);
    
    // Case 1:
    // odd indexed philosophers release R chopstick first
    // Then release L chopstick
    } else {
    
        sem_post(&chopstick[num]);
        printf(" Philosopher[%lld] released %lldth chopstick\n", num, num);
        sem_post(&chopstick[_num]);
        printf(" Philosopher[%lld] released %lldth chopstick\n", _num, _num);
    
    }
    
    // When philosophers fully enjoyed his/her meal,
    // think until they become hungry again.
     
    printf("\n***Philosopher[%lld] : Thinking***\n\n", num);

}
