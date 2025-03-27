#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        printf("sig erro\n");
        exit(1);
    }
    int sig=atoi(argv[1]);
    int pid=atoi(argv[2]);
    if(kill(pid,sig)==-1)
    {
        printf("kill erro\n");
    }
    exit(0);
}
