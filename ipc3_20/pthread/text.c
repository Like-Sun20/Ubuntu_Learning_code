#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_SIZE 30
int buff[MAX_SIZE];
int in = 0;
int out = 0;

sem_t sem_empty;
sem_t sem_full;
pthread_mutex_t mutex;

void* sc_thread(void* arg)
{
    for(int i = 0; i < 30; i++ )
    {
        sem_wait(&sem_empty);//ps1
        pthread_mutex_lock(&mutex);

        buff[in] = rand() % 100;
        printf("生产者在%d位置，产生数据:%d\n",in,buff[in]);
        in = (in + 1) % MAX_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&sem_full);//vs2
    }
}

void* xf_thread(void* arg)
{
    for(int i = 0; i < 20; i++ )
    {
        sem_wait(&sem_full);//ps2

        pthread_mutex_lock(&mutex);
        printf("------消费者在%d位置消费数据：%d\n",out,buff[out]);
        out = (out + 1 ) % MAX_SIZE;
        pthread_mutex_unlock(&mutex);

        sem_post(&sem_empty);
    }
}
int main()
{
    sem_init(&sem_empty,0,MAX_SIZE);
    sem_init(&sem_full,0,0);

    pthread_mutex_init(&mutex,NULL);

    pthread_t sc_id[2];
    pthread_t xf_id[3];
    for(int i = 0; i < 2; i++ )
    {
        pthread_create(&sc_id[i],NULL,sc_thread,NULL);
    }

    for(int i = 0; i < 3; i++ )
    {
        pthread_create(&xf_id[i],NULL,xf_thread,NULL);
    }

    for(int i = 0; i < 2; i++  )
    {
        pthread_join(sc_id[i],NULL);
    }

    for(int i = 0; i < 3; i++ )
    {
        pthread_join(xf_id[i],NULL);
    }

    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    pthread_mutex_destroy(&mutex);

    exit(0);
}
