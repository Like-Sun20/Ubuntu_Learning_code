#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <event.h>

void sig_cb(int fd, short ev, void* arg)
{
    if( ev & EV_SIGNAL)
    {
        printf("sig=%d\n",fd);
    }

    if( ev &EV_TIMEOUT)
    {

    }
}
void time_cb(int fd, short ev,void* arg)
{
    if( ev & EV_TIMEOUT)
    {
        printf("timeout\n");
    }
}
int main()
{
    struct event_base * base = event_init();
    if( base == NULL)
    {
        exit(1);
    }

    //struct event * sig_ev = evsignal_new(base,SIGINT,sig_cb,NULL);
    struct event* sig_ev = event_new(base,SIGINT,EV_SIGNAL,sig_cb,NULL);
    if( sig_ev != NULL )
    {
        event_add(sig_ev,NULL);
    }

    //struct event * time_ev = evtimer_new(base,time_cb,NULL);
    struct event* time_ev = event_new(base,-1,EV_TIMEOUT,time_cb,NULL);
    if( time_ev != NULL)
    {
        struct timeval tv = {5,0};
        event_add(time_ev,&tv);
    }

    event_base_dispatch(base);// select poll epoll 循环检测 阻塞
    event_free(time_ev);
    event_free(sig_ev);
    event_base_free(base);

    exit(0);
}