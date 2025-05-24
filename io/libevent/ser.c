#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <event.h>

struct mess
{
    struct event* ev;
    //char data[128];//用户数据
};
int socket_init();
void recv_data(int c, short ev, void *arg)
{
    struct mess * p = (struct mess*)arg;
    if( ev & EV_READ)
    {
        char buff[128] = {0};
        int n = recv(c,buff,127,0);
        if( n <= 0 )
        {
            //del
            event_free(p->ev);
            close(c);
            free(p);
            printf("client close(%d)\n",c);
            return;
        }

        printf("recv(%d)=%s\n",c,buff);
        send(c,"ok",2,0);
    }
}
void accepet_client(int sockfd, short ev, void *arg)
{
    struct event_base *base = (struct event_base *)arg;
    if (NULL == base)
    {
        return;
    }

    if (ev & EV_READ)
    {
        int c = accept(sockfd,NULL,NULL);
        if( c < 0 )
        {
            return ;
        }

        printf("accept c=%d\n",c);
        struct mess* p = (struct mess*)malloc(sizeof(struct mess));

        p->ev = event_new(base,c,EV_READ|EV_PERSIST,recv_data,p);
        event_add(p->ev,NULL);
    }
}
int main()
{
    int sockfd = socket_init();
    if (-1 == sockfd)
    {
        exit(1);
    }

    struct event_base *base = event_init(); // 定义libevent实例
    if (NULL == base)
    {
        exit(1);
    }

    struct event *sock_ev = event_new(base, sockfd, EV_READ | EV_PERSIST, accepet_client, base); // 定义事件
    if (NULL != sock_ev)
    {
        event_add(sock_ev, NULL); // 添加事件
    }

    event_base_dispatch(base); // 启动事件循环
    event_free(sock_ev);
    event_base_free(base);
    close(sockfd);
    exit(0);
}

int socket_init()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockfd)
    {
        return -1;
    }

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6000);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int res = bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (-1 == res)
    {
        printf("bind err\n");
        return -1;
    }

    res = listen(sockfd, 5);
    if (-1 == res)
    {
        return -1;
    }

    return sockfd;
}