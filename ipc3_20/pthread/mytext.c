#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

    sem_t s1;
    sem_t s2;
    sem_t s3;
void* fun1(void* arg)
{
    for(int i=0;i<5;i++)
    {
    sem_wait(&s1);
    printf("A\n");
    sem_post(&s2);
    }
}
void* fun2(void* arg)
{
    for(int i=0;i<5;i++)
    {
    sem_wait(&s2);
    printf("B\n");
    sem_post(&s3);
    }
}
 void* fun3(void* arg)
{
    for(int i=0;i<5;i++)
    {
    sem_wait(&s3);
    printf("C\n");
    sem_post(&s1);
    }
}

int main()
{
   
    sem_init(&s1,0,1);
    sem_init(&s2,0,0);
    sem_init(&s3,0,0);
    
    pthread_t id[3];
    
    pthread_create(&id[0],NULL,fun1,NULL);
    pthread_create(&id[1],NULL,fun2,NULL);
    pthread_create(&id[2],NULL,fun3,NULL);
   
    for(int i=0;i<3;i++)
    {
    pthread_join(id[i],NULL);
    }

}
