#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>

#define  STDIN  0
int main()
{
    int fd = STDIN;//键盘

    fd_set fdset;//收集描述符的集合

    while( 1 )
    {
        FD_ZERO(&fdset);//情况集合
        FD_SET(fd,&fdset);//一般需要循环添加多个描述符

        struct timeval tv = {5,0};//超时时间5s
        int n = select(fd+1,&fdset,NULL,NULL,&tv);//阻塞
        if( -1 == n )
        {
            printf("select err\n");
            continue;
        }
        else if ( 0 == n )
        {
            printf("time out\n");
            continue;
        }
        else
        {
            if(FD_ISSET(fd,&fdset))//有读事件，结果为真
            {
                char buff[128] = {0};
                read(fd,buff,128);
                printf("buff=%s\n",buff);
            }
        }
    }

}