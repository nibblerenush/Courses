#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char** argv) {
    key_t key1 = atoi(argv[1]);
    key_t key2 = atoi(argv[2]);

    int shmid1 = shmget(key1, 1000, 0666 | IPC_CREAT);
    int* shm1 = (int*)shmat(shmid1, NULL, SHM_RDONLY);

    int shmid2 = shmget(key2, 1000, 0666 | IPC_CREAT);
    int* shm2 = (int*)shmat(shmid2, NULL, SHM_RDONLY);

    key_t max_key = 0;
    if (key1 > key2) {
        max_key = key1;
    }
    else {
        max_key = key2;
    }
    key_t new_key = max_key / 2;

    int new_shmid = shmget(new_key, 1000, 0666 | IPC_CREAT);
    int* new_shm = (int*)shmat(new_shmid, NULL, 0);

    int i = 0;
    for (i = 0; i < 100; ++i) {
        new_shm[i] = shm1[i] + shm2[i];
    }
    
    shmdt(shm1);
    shmdt(shm2);
    shmdt(new_shm);

    printf("%d\n", new_key);
    return 0;
}
