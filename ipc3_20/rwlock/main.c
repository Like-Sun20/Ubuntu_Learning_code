#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

pthread_rwlock_t lock;

void *fun_rd1(void* arg)
{
  for(int i=0;i<30;i++)
  {
    pthread_rwlock_rdlock(&lock);
    printf("rd1 read start\n");
    int n=rand()%3;
    sleep(n);
    printf("rd1 read end\n");
    pthread_rwlock_unlock(&lock);
    sleep(1);
  }
}

void* fun_rd2(void* arg)
{
    for(int i=0;i<20;i++)
    { 
      pthread_rwlock_rdlock(&lock);
      printf("rd2 read start\n");
      int n=rand()%3;
      sleep(n);
      printf("rd2 read end\n");
      pthread_rwlock_unlock(&lock);
      n=rand()%3;
      sleep(n);
    }
}

void* fun_wr(void* arg)
{
    for(int i=0;i<10;i++)
    {
        pthread_rwlock_wrlock(&lock);
        printf("--------------write start\n");
        int n=rand()%3;
        sleep(n);
        printf("--------------write end\n");
        pthread_rwlock_unlock(&lock);
        n=rand()%3;
        sleep(n);
    }
}
int main()
{
pthread_t id1,id2,id3;
pthread_rwlock_init(&lock,NULL);

    pthread_create(&id1,NULL,fun_rd1,NULL);
    pthread_create(&id2,NULL,fun_rd2,NULL);
    pthread_create(&id3,NULL,fun_wr,NULL);

    pthread_join(id1,NULL);
    pthread_join(id2,NULL);
    pthread_join(id3,NULL);

    pthread_rwlock_destroy(&lock);

exit(0);
}