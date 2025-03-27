#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>

int main()
{
    int fdw=open("fifo",O_WRONLY);
    if(fdw==-1)
    {
        printf("open fifo erro\n");
        exit(1);
    }
    printf("fdw=%d\n",fdw);
    while(1)
    {
    char buff[128]={0};
    fgets(buff,128,stdin);
    if(strncmp(buff,"end",3)==0)
    {
        break;
    }
     write(fdw,buff,strlen(buff));
    }
close(fdw);
exit(0);
}
