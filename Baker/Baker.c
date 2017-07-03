#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "Semaphore.h"

void *bakerFunction(void *arg);

int main()
{
    const int NUM_OF_BAKER = 3;
    int i = 0;
    //模拟三个销售人员
    pthread_t bakerThread[NUM_OF_BAKER];

    for (i = 0; i < NUM_OF_BAKER; i++)
    {
        pthread_create(&bakerThread[i], NULL, bakerFunction, (void *)i);
    }

    for (i = 0; i < NUM_OF_BAKER; i++)
    {
        pthread_join(bakerThread[i], NULL);
    }
}

void *bakerFunction(void *arg)
{
    int sid = semaphore_create();
    up_num_of_free(sid);
    while (1)
    {
        int tid = (int)arg;
        down_num_of_waiting(sid); //没有顾客则会阻塞
        printf("销售人员 %d ：有顾客在排队等候，我去为他服务。\n", tid);
        sleep(3);
    }
}