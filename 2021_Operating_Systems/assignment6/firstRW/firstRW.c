#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// The number of writer thread
#define WRITER_NUMBER 5
// The number of reader thread
#define READER_NUMBER 2
// The number of loop
#define COUNTING_NUMBER 100

// S semaphore and write semaphore 
sem_t S;
sem_t wrt;

// Some variables for critical section 
long long cur_count = 0;
long long cur_writer = -1;
long long readCount = 0;

// arguments for pthread_create() 
void *writer(void*);
void *reader();

int main() {

    long long i;
    
    // init semaphores and threads 
    sem_init(&S, 0, 1);
    sem_init(&wrt, 0, 1);

    pthread_t readerThread[READER_NUMBER];
    pthread_t writerThread[WRITER_NUMBER];

    for(i = 0; i < READER_NUMBER; ++i)
        pthread_create(&readerThread[i], NULL, reader, NULL);
    for(i = 0; i < WRITER_NUMBER; ++i)
        pthread_create(&writerThread[i], NULL, writer, (void*)i);

    // collect threads 
    for(i = 0; i < READER_NUMBER; ++i)
        pthread_join(readerThread[i], NULL);
    for(i = 0; i < WRITER_NUMBER; ++i)
        pthread_join(writerThread[i], NULL);

    // destroy semaphores 
    sem_destroy(&S);
    sem_destroy(&wrt);

    printf("done\n");

    return 0;
}

void *writer(void *n) {

    for (int i=0; i<COUNTING_NUMBER; i++) {
        
        usleep(100000);

        // LOCK 
        sem_wait(&wrt);

        // CRITICAL SECTION
        cur_count += 1;
        cur_writer = (long long)n;

        // UNLOCK
        sem_post(&wrt);
    }

}

void *reader() {

    for (int i=0; i<COUNTING_NUMBER; i++) {
        usleep(30000);

        // LOCK 
        sem_wait(&S);
        readCount += 1;
        if (readCount == 1)
            sem_wait(&wrt);
        sem_post(&S);

        // CRITICAL SECTION
        printf("Count: %lld, The most recent writer ID: %lld\n", 
                cur_count, cur_writer);

        // UNLOCK
        sem_wait(&S);
        readCount -= 1;
        if (readCount == 0)
            sem_post(&wrt);
        sem_post(&S);

    }
}
