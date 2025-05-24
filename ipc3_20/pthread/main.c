#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>


//sem_t sem;
pthread_mutex_t mutex;

void* fun(void* arg)
{
    for(int i = 0; i < 5; i++ )
    {
        //sem_wait(&sem);//p
        pthread_mutex_lock(&mutex);
        printf("A");
        fflush(stdout);
        int n = rand() % 3;
        sleep(n);
        printf("A");
        fflush(stdout);
        //sem_post(&sem);//v
        pthread_mutex_unlock(&mutex);

        n = rand() % 3;
        sleep(n);
    }
}

int main()
{
    //sem_init(&sem,0,1);
    pthread_mutex_init(&mutex,NULL);
    pthread_t id;
    pthread_create(&id,NULL,fun,NULL);

    for(int i = 0; i < 5; i++ )
    {
        //sem_wait(&sem);//p
        pthread_mutex_lock(&mutex);
        printf("B");
        fflush(stdout);
        int n = rand() % 3;
        sleep(n);
        printf("B");
        fflush(stdout);
        //sem_post(&sem);//v
        pthread_mutex_unlock(&mutex);

        n = rand() % 3;
        sleep(n);
    }

    pthread_join(id,NULL);
    //sem_destroy(&sem);
    pthread_mutex_destroy(&mutex);
    exit(0);
}
