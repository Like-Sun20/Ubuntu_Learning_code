#include "sem.h"

static int semid = -1;
void sem_init()
{
    semid = semget((key_t)1234,2,IPC_CREAT|IPC_EXCL|0600);
    if( semid == -1 )//全新创建失败，可能被别人创建了
    {
        semid = semget((key_t)1234,2,0600);
        if( semid == -1 )
        {
            printf("semget err\n");
        }
    }
    else//全新创建成功，初始化
    {
        union semun a;
        int arr[2] = {1,0};
        for(int i = 0; i < 2; i++ )
        {
            a.val = arr[i];
            if( semctl(semid,i,SETVAL,a) == -1 )
            {
                printf("semctl init err\n");
            }
        }
    }
}
void sem_p(int index)
{
    struct sembuf buf;
    buf.sem_num = index;
    buf.sem_op =-1;//p
    buf.sem_flg = SEM_UNDO;

    if ( semop(semid,&buf,1) == -1 )
    {
        printf("p err\n");
    }
}
void sem_v(int index)
{
    struct sembuf buf;
    buf.sem_num = index;
    buf.sem_op =1;//v
    buf.sem_flg = SEM_UNDO;

    if ( semop(semid,&buf,1) == -1 )
    {
        printf("v err\n");
    }

}
void sem_destroy()
{
    if( semctl(semid,0,IPC_RMID) == -1 )
    {
        printf("semctl del err\n");
    }
}
