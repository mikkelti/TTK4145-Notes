// Compile and run with: clang -Wall -lpthread -o multiplex multiplex.c; ./multiplex; rm multiplex

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_CHILDREN 5

// Global variables
int count = 0;
sem_t *multiplex; // n = 3

void *increment_loop() {
    for (int i = 0; i  < 100000; ++i) {
        sem_wait(multiplex);
        ++count;
        sem_post(multiplex);
    } 
    return NULL;
}

int main() {
    // Init semaphore, weird syntax, see man-pages.
    multiplex = sem_open("/mutex", O_CREAT, S_IRWXU, 3);
    
    // See man-pages for syntax

    pthread_t child[NUM_CHILDREN];
    for (int i = 0; i < NUM_CHILDREN; ++i) {
        pthread_create(&child[i], NULL, increment_loop, NULL);
    }

    for (int i = 0; i < NUM_CHILDREN; ++i) {
        pthread_join(child[i], NULL);
    }

    printf("count = %d\n", count);

    return 0;
}
