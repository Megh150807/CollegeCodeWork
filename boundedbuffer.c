#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5 // Fixed size of the buffer

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t mutex, empty, full;

void* producer(void* arg) {
    for(int i = 0; i < 10; i++) {
        int item = rand() % 100; // Produce an item

        sem_wait(&empty); // Wait if there are no empty slots
        sem_wait(&mutex); // Lock the buffer

        buffer[in] = item;
        printf("Producer produced: %d at index %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex); // Unlock the buffer
        sem_post(&full);  // Signal that a slot is now full
        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg) {
    for(int i = 0; i < 10; i++) {
        sem_wait(&full);  // Wait if there are no full slots
        sem_wait(&mutex); // Lock the buffer

        int item = buffer[out];
        printf("Consumer consumed: %d from index %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex); // Unlock the buffer
        sem_post(&empty); // Signal that a slot is now empty
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    // Initialize semaphores
    sem_init(&mutex, 0, 1);           // Binary semaphore for lock
    sem_init(&empty, 0, BUFFER_SIZE); // Starts with N empty slots
    sem_init(&full, 0, 0);            // Starts with 0 full slots

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
