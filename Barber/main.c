#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>

const int NUM_OF_CHAIRS = 4;

sem_t mutex, customers, barber;
int waiting = 0;
int chair[4];

void *barberFunction();

void *customerFunction(void *arg);

int main() {
    memset(chair, -1, sizeof(chair));

    pthread_t customerThread[20], barberThread;

    sem_init(&mutex, 0, 1);  //互斥信号量初始化1
    sem_init(&customers, 0, 0);//顾客信号量初始化0
    sem_init(&barber, 0, 1);//理发师信号量初始化1

    int i = 20;
    pthread_create(&barberThread, NULL, (void *) barberFunction, NULL);
    for (i = 0; i < 20; i++) {
        pthread_create(&customerThread[i], NULL, (void *) customerFunction, (void *) i);
    }


    for (i = 0; i < 20; i++) {
        pthread_join(customerThread[i], NULL);
    }
    pthread_join(barberThread, NULL);

    return 0;
}

void *barberFunction() {
    while(1) {
        sem_wait(&customers);
        sem_wait(&mutex);

        int i, nextCustomer;
        for (i = 0; i < NUM_OF_CHAIRS; i++) {
            if (chair[i] != -1) {
                nextCustomer = chair[i];
                chair[i] = -1;
                break;
            }
        }
        waiting--;

        printf("The barber is dealing with the %dth customer's hair.\n", nextCustomer);
        sleep(3);
        sem_post(&mutex);
        sem_post(&barber);
    }
}

void *customerFunction(void *arg) {
    int customerId = (int) arg;

    sem_wait(&mutex);
    if (waiting < NUM_OF_CHAIRS) {
        waiting++;

        int i;
        for (i = 0; i < NUM_OF_CHAIRS; i++) {
            if (chair[i] == -1) {
                chair[i] = customerId;
                break;
            }
        }

        printf("The %dth customer is sitting on the %dth chair.\n", customerId, i);

        sem_post(&mutex);
        sem_post(&customers);
        sem_wait(&barber);
    } else {
        printf("The %dth customer comes but there's no empty chair left, so he leaves.\n", customerId);
        sem_post(&mutex);
    }
}