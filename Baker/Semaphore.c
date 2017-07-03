#include "Semaphore.h"

int semaphore_create()
{
    int key = ftok(".", 1);
    return semget(key, 2, IPC_CREAT);
}

int vNumOfWaiting(int sid)
{
    struct sembuf operation = {0, 1, SEM_UNDO};
    return semop(sid, &operation, 1);
}

int pNumOfWaiting(int sid)
{
    struct sembuf operation = {0, -1, SEM_UNDO};
    return semop(sid, &operation, 1);
}

int vNumOfFree(int sid)
{
    struct sembuf operation = {1, 1, SEM_UNDO};
    return semop(sid, &operation, 1);
}

int pNumOfFree(int sid)
{
    struct sembuf operation = {1, -1, SEM_UNDO};
    return semop(sid, &operation, 1);
}