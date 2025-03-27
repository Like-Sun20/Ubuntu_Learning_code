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
            printf("shmid err\n");
            exit(1);
        }
    
        char* s  = (void*)shmat(shmid,NULL,0);//映射，可以使用 s
        if( s == (char*)-1 )
        {
            printf("shmat err\n");
            exit(1);
        }
    
        sem_init();
    
        while( 1 )
        {
            printf("input\n");
            char buff[128] = {0};
            fgets(buff,128,stdin);
            sem_p(0);
            strcpy(s,buff);//
            sem_v(1);
            if( strncmp(buff,"end",3) == 0 )
            {
                break;
            }
    
        }
        shmdt(s);//断开映射
    
    }
    