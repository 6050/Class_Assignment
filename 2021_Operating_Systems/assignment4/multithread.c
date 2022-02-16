#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARGUMENT_NUMBER 20

long long result = 0;

void* ThreadFunc(void *n){

    long long i;
    long long number = *((long long *)n);
    
    // temporary variable for addition to result at the end of ThreadFunc
    // No use of lock (Mutex)
    long long tmp = 0;
    
    printf("number = %lld\n", number);

    for (i=0; i<25000000; i++)
        tmp += number;

    // add to global result from each thread's temp result
    result += tmp;

}

int main(void){

    long long argument[ARGUMENT_NUMBER];
    long long i;

    pthread_t threads[ARGUMENT_NUMBER];

    for (i=0; i<ARGUMENT_NUMBER; i++)
        argument[i] = i;

    // create threads
    for (i=0; i<ARGUMENT_NUMBER; i++)
        pthread_create(&(threads[i]), NULL, ThreadFunc, (void*)&argument[i]);
    
    printf("Main Thread is waiting for child Thread!\n");
    
    // wait threads
    for (i=0; i<ARGUMENT_NUMBER; i++)
        pthread_join(threads[i], NULL);

    printf("result = %lld\n", result);

    return 0;

}


