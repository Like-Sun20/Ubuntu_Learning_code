#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main()
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1) {exit(1);}

    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(6000);//
    saddr.sin_addr.s_addr=inet_addr("192.168.155.38");
int res=connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
if(res==-1) 
{
    printf("connect erro\n");
    exit(1);
}
printf("input\n");
char buff[128]={0};
fgets(buff ,128,stdin);
send(sockfd,buff,strlen(buff)-1,0);

memset(buff,0,128);
recv(sockfd,buff,127,0);
printf("buff=%s\n",buff);
close(sockfd);

exit(0);
}