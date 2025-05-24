#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include "sem.h"

int main()
{
    int shmid = shmget((key_t)1234,128,IPC_CREAT|0600);
    if( shmid == -1 )
    {
        printf("shmget err\n");
        exit(1);
    }

    char* s = shmat(shmid,NULL,0);
    if( s == (char*)-1 )
    {
        exit(1);
    }

    sem_init();

    while( 1 )
    {
        sem_p(1);//ps2
        if( strncmp(s,"end",3) ==  0 )
        {
            break;
        }
        printf("read:%s\n",s);
        sem_v(0);//vs1
    }

    shmdt(s);
    sem_destroy();
    shmctl(shmid,IPC_RMID,NULL);
    exit(0);
}
