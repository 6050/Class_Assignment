#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Set number of philosophers
//  It can be dynamic
#define PHILOSOPHER_NUMBER 6

// Chopsticks for philosophers  
sem_t chopstick[PHILOSOPHER_NUMBER];

// Argument for pthread_create 
void *func(void*);

int main() {
    long long i;
    pthread_t tid[PHILOSOPHER_NUMBER];

    // init semaphores 
    for (i=0; i<PHILOSOPHER_NUMBER; i++) 
        sem_init(&chopstick[i], 0, 1);
    
    // Do infinite loop until they got deadlock
    while ( 1 ) {

        // Create all threads 
        for (i=0; i<PHILOSOPHER_NUMBER; i++) 
            pthread_create(&tid[i], NULL, func, (void*)i);


        // Collect all threads 
        for (i=0; i<PHILOSOPHER_NUMBER; i++)
            pthread_join(tid[i], NULL);
    
    }

}

// Core section for threads.
// Some philosophers wait for a chopstick.
// In R->L order.
 
void *func(void *n) {

    // His/her right hand 
    long long num = (long long)n;
    // His/her left hand 
    long long _num = (num + 1) % PHILOSOPHER_NUMBER;

    // Every philosophers pick R->L chopsticks 
    sem_wait(&chopstick[num]);
    printf(" Philosopher[%lld] picked %lldth chopstick\n", num, num);
    sem_wait(&chopstick[_num]);
    printf(" Philosopher[%lld] picked %lldth chopstick\n", _num, _num);

    printf("\n***Philosopher[%lld] : Eating***\n\n", num);
    sleep(1);
    
    // Every philosophers release L->R chopstick 
    sem_post(&chopstick[_num]);
    printf(" Philosopher[%lld] released %lldth chopstick\n", _num, _num);
    sem_post(&chopstick[num]);
    printf(" Philosopher[%lld] released %lldth chopstick\n", num, num);

    // When philosophers fully enjoyed his/her meal,
    // think until they become hungry again.
     
    printf("\n***Philosopher[%lld] : Thinking***\n\n", num);

}

