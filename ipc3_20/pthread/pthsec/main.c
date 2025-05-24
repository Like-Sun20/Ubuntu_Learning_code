#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void* fun(void* argv)
{
    char buff[] = "a b c";
    char* ptr = NULL;
    char* s = strtok_r(buff," ",&ptr);
    while( s != NULL )
    {
        printf("fun s=%s\n",s);
        sleep(1);
        s = strtok_r(NULL," ",&ptr);
    }
}
int main()
{
    pthread_t id;
    pthread_create(&id,NULL,fun,NULL);
    char buff[]="1 2 3";
    char* ptr=NULL;
    char* s=strtok_r(buff," ",&ptr);
while(s!=NULL)
{
    printf("main s=%s\n",s);
    sleep(1);
    s=strtok_r(NULL," ",&ptr);
}
pthread_join(id,NULL);

exit(0);


}