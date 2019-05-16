// Compile and run with: clang -Wall -lpthread -o rendezvous rendezvous.c; ./rendezvous; rm rendezvous

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// Global variables
int i = 0;
int j = 0;
sem_t *a_arrived;
sem_t *b_arrived;

// Functions to be run by individual threads
void *code_a() {
    i = 5;
    sem_post(a_arrived);
    sem_wait(b_arrived);
    printf("j = %d\n", j); 
    return NULL;
}

void *code_b() {
    j = 5;
    sem_post(b_arrived);
    sem_wait(a_arrived);
    printf("i = %d\n", i);
    return NULL;
}

int main() {
    a_arrived = sem_open("/a_arrived", O_CREAT, S_IRWXU, 0);
    b_arrived = sem_open("/b_arrived", O_CREAT, S_IRWXU, 0);

    pthread_t A, B;
    pthread_create(&A, NULL, code_a, NULL);
    pthread_create(&B, NULL, code_b, NULL);

    pthread_join(A, NULL);
    pthread_join(B, NULL);

    return 0;
}
