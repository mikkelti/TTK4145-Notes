// Compile and run with: clang -Wall -lpthread -o barrier barrier.c; ./barrier; rm barrier

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <assert.h>

#define N 5

/* Questions:
* Why does count get printed as different values, but assert works?
* Why did it work to change the names of the threads?
* There might be something with sem_open, and the semaphores never having been deleted.
* Try on Linux, where sem_init works.
*/

// Global variables
int count = 0;
sem_t *mutex;
sem_t *barrier;

void *child_code() {
    // Other code

    // Rendezvous
    sem_wait(mutex);
    count = count + 1;
    sem_post(mutex);

    if (count == N) {
        sem_post(barrier);
    }

    // Turnstile
    sem_wait(barrier);
    sem_post(barrier);

    // Critical point
    assert(count == 5);

    return NULL;
}

int main() {
    mutex = sem_open("/mutex", O_EXCL, S_IRWXU, 1);
    barrier = sem_open("/barrier", O_EXCL, S_IRWXU, 0);

    pthread_t child[N];
    for (int i = 0; i < N; ++i) {
        pthread_create(&child[i], NULL, child_code, NULL);
    }   

    for (int i = 0; i < N; ++i) {
        pthread_join(child[i], NULL);
    }

    sem_close(mutex);
    sem_close(barrier);

    return 0;
}
