#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

pthread_mutex_t mutex;


int N=0;
int NUM_OF_THREADS=0;
static double PI=0;
static int ave=0;
static int timeUsed=0;

void getInput()
{
    printf("Please input N:");
    scanf("%d",&N);
    printf("Please input the number of threads:");
    scanf("%d",&NUM_OF_THREADS);
    ave=N/NUM_OF_THREADS;
}

void* pCompute(void* args)
{
    pthread_mutex_lock(&mutex);//加锁
    int k;
    int i=args;
    for(k=i*ave;k<(i+1)*ave;k++)
    {
        PI+=4.0/((double)(1+(pow((k+0.5)/N,2 ))))/N;
    }
    pthread_mutex_unlock(&mutex);//解锁
}

int main()
{
    getInput();

    struct timeval start_time,end_time;
    gettimeofday(&start_time,NULL);

    pthread_t *myThread;
    myThread=(pthread_t*)malloc(sizeof(pthread_t)*NUM_OF_THREADS);

    int i;
    int result;
    for(i=0;i<NUM_OF_THREADS;i++)
    {
        result=pthread_create(&myThread[i],NULL,pCompute,(void*)i);
        if(result!=0)
        {
            printf("Failed to create threads.\n");
            break;
        }
    }

    for(i=0;i<NUM_OF_THREADS;i++)
    {
        pthread_join(myThread[i],NULL);
    }

    int rest;
    for(rest=ave*NUM_OF_THREADS;rest<N;rest++)
    {
        PI+=4.0/((double)(1+(pow((rest+0.5)/N,2 ))))/N;
    }

    gettimeofday(&end_time,NULL);
    double timeUsed = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec)/1000;

    printf("N is %d , and num of threads is %d\n",N,NUM_OF_THREADS);
    printf("Pi is %0.10f \n Used time %f ms\n",PI,(timeUsed));

    return 0;
}
