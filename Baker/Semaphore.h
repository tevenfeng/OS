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
int output_semaphore(int sid);
int up_num_of_waiting(int sid);
int down_num_of_waiting(int sid);
int up_num_of_free(int sid);
int down_num_of_free(int sid);