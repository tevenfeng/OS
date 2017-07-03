#include "Semaphore.h"

int semaphore_create()
{
    int key = ftok(".", 1);
    return semget(key, 2, IPC_CREAT);
}

int output_semaphore(int sid)
{
    printf("num of waiting: %d\n", semctl(sid, 0, GETVAL));
    printf("num of free   : %d\n", semctl(sid, 1, GETVAL));
}

int up_num_of_waiting(int sid)
{
    struct sembuf operation = {0, 1, SEM_UNDO};
    return semop(sid, &operation, 1);
}

int down_num_of_waiting(int sid)
{
    struct sembuf operation = {0, -1, SEM_UNDO};
    return semop(sid, &operation, 1);
}

int up_num_of_free(int sid)
{
    struct sembuf operation = {1, 1, SEM_UNDO};
    return semop(sid, &operation, 1);
}

int down_num_of_free(int sid)
{
    struct sembuf operation = {1, -1, SEM_UNDO};
    return semop(sid, &operation, 1);
}