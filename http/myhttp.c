#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

int socket_init();

char* get_filename(char buff[])
{
    char* s = strtok(buff," ");
    if( s == NULL )
    {
        return NULL;
    }

    printf("请求方法:%s\n",s);

    s = strtok(NULL," ");
    return s;
}
int main()
{
    int sockfd = socket_init();
    if( sockfd == -1)
    {
        exit(1);
    }

    while( 1 )
    {
        int c = accept(sockfd,NULL,NULL);
        if( c < 0 )
        {
            continue;
        }

        printf("accept c=%d\n",c);

        char buff[1024] = {0};
        recv(c,buff,1023,0);
        printf("buff=%s\n",buff);
        char* filename = get_filename(buff);
        if( filename == NULL )
        {
            send(c,"404",3,0);
            close(c);
            continue;
        }

        if( strcmp("/",filename) == 0)
        {
            filename = "/index.html";
        } 

        char path[256] = {"/home/li/mycode/http"};
        strcat(path,filename);
        printf("path:%s\n",path);

        int fd = open(path,O_RDONLY);
        if( -1 == fd)
        {
            send(c,"404",3,0);
            close(c);
            continue;
        }

        int filesize = lseek(fd,0,SEEK_END);
        lseek(fd,0,SEEK_SET);

        char head[256] = {"HTTP/1.1 200 OK\r\n"};
        strcat(head,"Server: myhttp\r\n");
        sprintf(head+strlen(head),"Content-Length: %d\r\n",filesize);
        strcat(head,"\r\n");//分割行
        send(c,head,strlen(head),0);

        char data[512];
        int num = 0;
        while( (num = read(fd,data,512)) > 0 )
        {
            send(c,data,num,0);
        }
        close(fd);
        close(c);//短连接
    }

}

int socket_init()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if( -1 == sockfd )
    {
        return -1;
    }

    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(80);//http 默认的端口
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int res = bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if( -1 == res)
    {
        printf("bind err\n");
        return -1;
    }

    res = listen(sockfd,5);
    if( -1 == res)
    {
        return -1;
    }

    return sockfd;
}