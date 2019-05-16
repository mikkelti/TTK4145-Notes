// Compile and run with: clang -Wall -lpthread -o mutex mutex.c; ./mutex; rm mutex

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// Global variables
int count = 0;
sem_t *mutex;

void *increment_loop() {
    for (int i = 0; i  < 100000; ++i) {
        sem_wait(mutex);
        ++count;
        sem_post(mutex);
    } 
    return NULL;
}

int main() {
    // Init semaphore, weird syntax, see man-pages.
    mutex = sem_open("/mutex", O_CREAT, S_IRWXU, 1);
    
    // See man-pages for syntax
    pthread_t A, B;
    pthread_create(&A, NULL, increment_loop, NULL);
    pthread_create(&B, NULL, increment_loop, NULL);

    pthread_join(A, NULL);
    pthread_join(B, NULL);

    printf("count = %d\n", count);

    return 0;
}
