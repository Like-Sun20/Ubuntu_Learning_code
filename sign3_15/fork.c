#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>

void fun(int sig)
{
printf("sig=%d\n",sig);
    wait(NULL);
}

int main()
{
    char* s=NULL;
    int n=0;
    signal(SIGCHLD,fun);
    pid_t pid=fork();
if(pid==-1)
{
    exit(1);
}
if(pid==0)
{
    n=3;
    s="child";
}
else
{
    n=7;
    s="parent";
}
for(int i=0;i<n;i++)
{
    printf("pid=%d\n",getpid());
    sleep(1);
}
exit(0);
}
