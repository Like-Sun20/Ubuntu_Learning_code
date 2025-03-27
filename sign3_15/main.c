#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>
 
void fun(int sig)
{
    printf("sig=%d",sig);
    signal(sig,SIG_DFL);
}
int main()
{
   // signal(SIGINT,fun);
   signal(SIGINT,SIG_IGN);
    while(1)
    {
        printf("hello\n");
        sleep(1);
    }
}
