#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

char buff[128] = {0};
pthread_cond_t cond;
pthread_mutex_t mutex;

void* funa(void* arg)
{
    while( 1 )
    {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond,&mutex);//加入条件变量的等待队列,阻塞
        pthread_mutex_unlock(&mutex);

        if( strncmp(buff,"end",3) == 0 )
        {
            break;
        }
        printf("funa read:%s\n",buff);
    }
}

void* funb(void* arg)
{
    while( 1 )
    {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond,&mutex);//加入条件变量的等待队列,阻塞
        pthread_mutex_unlock(&mutex);

        if( strncmp(buff,"end",3) == 0 )
        {
            break;
        }

        printf("funb read:%s\n",buff);
    }

}


int main()
{
    pthread_mutex_init(&mutex,NULL);//
    pthread_cond_init(&cond,NULL);

    pthread_t ida,idb;
    pthread_create(&ida,NULL,funa,NULL);
    pthread_create(&idb,NULL,funb,NULL);

    while( 1 )
    {
        fgets(buff,128,stdin);
        
        if( strncmp(buff,"end",3) == 0)
        {
            pthread_mutex_lock(&mutex);
            pthread_cond_broadcast(&cond);//唤醒所有线程
            pthread_mutex_unlock(&mutex);
            break;
        }
        else
        {
            pthread_mutex_lock(&mutex);
            pthread_cond_signal(&cond);//唤醒一个线程
            pthread_mutex_unlock(&mutex);
        }
    }

    pthread_join(ida,NULL);
    pthread_join(idb,NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    exit(0);
}