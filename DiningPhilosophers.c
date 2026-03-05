#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5 // Number of philosophers

sem_t forks[N];

void* philosopher(void* num) {
    int id = *(int*)num;

    while (1) {
        printf("Philosopher %d is thinking...\n", id);
        sleep(1);

        // ASYMMETRIC SOLUTION to avoid deadlock
        if (id % 2 == 0) {
            sem_wait(&forks[id]);           // Pick up left
            sem_wait(&forks[(id + 1) % N]); // Pick up right
        } else {
            sem_wait(&forks[(id + 1) % N]); // Pick up right
            sem_wait(&forks[id]);           // Pick up left
        }

        printf("Philosopher %d is EATING \n", id);
        sleep(2);

        // Put down forks
        sem_post(&forks[id]);
        sem_post(&forks[(id + 1) % N]);

        printf("Philosopher %d finished eating.\n", id);
    }
}

int main() {
    pthread_t threads[N];
    int ids[N];

    // Initialize semaphores (forks) to 1 (available)
    for (int i = 0; i < N; i++) {
        sem_init(&forks[i], 0, 1);
        ids[i] = i;
    }

    // Create philosopher threads
    for (int i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    // Wait for threads (infinite loop in this case)
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
