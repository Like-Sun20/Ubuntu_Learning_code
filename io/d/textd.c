#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include<pthread.h>

void set_shouhujinch()
{
    pid_t pid = fork();//子进程是组员进程
    if( pid != 0 )
    {
        exit(1);
    }

    setsid();//创建新会话 
    pid = fork();
    if( pid != 0 )
    {
        exit(1);
    }

    chdir("/");//
    umask(0);

    int maxfd = getdtablesize();
    for(int i = 0; i < maxfd; i++)
    {
        close(i);
    }

}

int main()
{
   set_shouhujinch();

   while( 1 )
   {
        time_t tv;
        time(&tv);//获取

        FILE * fp = fopen("/tmp/c2409d.log","a");
        if( fp == NULL )
        {
            break;
        }
        fprintf(fp,"Time is %s", asctime(localtime(&tv))); 
        fclose(fp);

        sleep(5);
   }

}