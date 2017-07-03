#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

int semaphore_create();
int vNumOfWaiting(int sid);
int pNumOfWaiting(int sid);
int vNumOfFree(int sid);
int pNumOfFree(int sid);