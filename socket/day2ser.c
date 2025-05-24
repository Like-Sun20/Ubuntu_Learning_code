#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <pthread.h>

void* fun(void* arg)
{
    int * p = (int*)arg;
    if( p == NULL )
    {
        return NULL;
    }

    int c = *p;
    free(p);
    while( 1 )
    {
        char buff[128] = {0};
        int n = recv(c,buff,127,0);//阻塞，对方关闭连接，0==recv() 
        if( n <= 0 )
        {
            break;
        }
        printf("buff=%s\n",buff);
        send(c,"ok",2,0);
    }
    close(c);
    printf("client close\n");

}
int main()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);//创建套接字 tcp协议    sockfd文件描述符
    if( -1 == sockfd )
    {
        exit(1);
    }

    struct sockaddr_in saddr,caddr;//
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6000);//1024以内 知名端口 管理才能使用,  4096保留， 5000+  临时端口
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int res = bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if( res == -1) 
    {
        printf("bind err\n");
        exit(1);
    }

    res = listen(sockfd,5);//创建监听队列

    while( 1 )
    {
        int len = sizeof(caddr);
        int c = accept(sockfd,(struct sockaddr*)&caddr,&len);// c = accept(sockfd,NULL,NULL) 接受连接
        if( c < 0 )
        {
            continue;
        }
        printf("accept c=%d\n",c);



////////////////////////////////////////////////

/*
   pid_t id=fork();
   if(id == -1)
   {
    exit(1);
   }
   if(id==0)
   {
    char buff[128] = {0};
        int n = recv(c,buff,127,0);//阻塞，对方关闭连接，0==recv() 
        if( n <= 0 )
        {
            break;
        }
        printf("buff=%s\n",buff);
        send(c,"ok",2,0);
    }
    close(c);
    printf("client close\n");

   }
*/


////////////////////////////////////////////



        //启动线程
        pthread_t id;
        int *p = (int*)malloc(sizeof(int));
        if( p == NULL )
        {
            close(c);
            continue;
        }
        *p = c;
        pthread_create(&id,NULL,fun,(void*)p);

       
    }
}

/*
int main()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if( -1 == sockfd )
    {
        exit(1);
    }

    struct sockaddr_in saddr,caddr;//创建套接字 tcp协议    sockfd文件描述符
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6000);//1024以内 知名端口 管理才能使用,  4096保留， 5000+  临时端口
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int res = bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if( res == -1) 
    {
        printf("bind err\n");
        exit(1);
    }

    res = listen(sockfd,5);//创建监听队列

    while( 1 )
    {
        int len = sizeof(caddr);
        int c = accept(sockfd,(struct sockaddr*)&caddr,&len);// c = accept(sockfd,NULL,NULL) 接受连接
        if( c < 0 )
        {
            continue;
        }
        printf("accept c=%d\n",c);

        while( 1 )
        {
            char buff[128] = {0};
            int n = recv(c,buff,1,0);//阻塞，对方关闭连接，0==recv() 
            if( n <= 0 )
            {
                break;
            }
            printf("buff=%s\n",buff);
            send(c,"ok",2,0);
        }
        close(c);
        printf("client close\n");
    }
}
*/