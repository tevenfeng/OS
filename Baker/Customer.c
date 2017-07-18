#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "Semaphore.h"

void *customerFunction(void *arg);

int main() {

    int i = 0;
    const int cusomterNum = 20;

    //模拟顾客不断到来
    pthread_t customersThread[cusomterNum];

    for (i = 0; i < cusomterNum; i++) {
        pthread_create(&customersThread[i], NULL, customerFunction, (void *) i);
        sleep(2);
    }

    for (i = 0; i < cusomterNum; i++) {
        pthread_join(customersThread[i], NULL);
    }

    return 0;
}

void *customerFunction(void *arg) {
    int tid = (int) arg;
    printf("一个顾客到达，取到 %d 号。\n", tid);
    int sid = semaphore_create();
    vNumOfWaiting(sid);
    pNumOfFree(sid);
    printf("一个销售人员空闲，为 %d 号顾客服务中。\n", tid);
    sleep(3);
    printf("%d 号顾客服务结束,已离开。\n", tid);
}