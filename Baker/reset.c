#include "Semaphore.h"

int main()
{
    int sid = semaphore_create();
    semun sem;
    sem.val = 0;
    semctl(sid, 0, SETVAL, sem);
    semctl(sid, 1, SETVAL, sem);
    return 0;
}