#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main() {
    // 1. Generate a unique key
    key_t key = ftok("shmfile", 65);

    // 2. Create shared memory segment (shmget)
    // IPC_CREAT creates it, 0666 sets read/write permissions
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    // 3. Attach process to shared memory (shmat)
    char *str = (char*) shmat(shmid, (void*)0, 0);

    // --- PROCESS 1: Writing ---
    printf("Write Data: ");
    fgets(str, 1024, stdin);
    printf("Data written in memory: %s", str);

    // 4. Detach from shared memory (shmdt)
    shmdt(str);

    // --- PROCESS 2: Reading (Re-attaching to demonstrate sharing) ---
    char *read_str = (char*) shmat(shmid, (void*)0, 0);
    printf("Data read from memory: %s", read_str);

    // 5. Detach and Destroy the segment (shmctl)
    shmdt(read_str);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
