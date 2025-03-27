#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <pwd.h>

#define  MAX_SIZE  10

char* get_cmd(char buff[],char* myargv[])
{
    int i = 0;
    char* s = strtok(buff," ");//cp a.c b.c
    while( s != NULL )
    {
        myargv[i++] = s;
        s = strtok(NULL," ");
    }

    return myargv[0];
}
void print_info()
{
    char * str = "$";
    int uid = getuid();
    if( uid == 0 )
    {
        str = "#";
    }

    struct passwd * ptr = getpwuid(uid);
    if( ptr == NULL )
    {
        printf("mybash $");
        fflush(stdout);
        return;
    }

    char host[128] = {0};
    gethostname(host,128);

    char path[256] = {0};
    getcwd(path,256);

 printf("\033[1;32m%s@%s\033[0m:\033[1;34m%s\033[0m%s ",ptr->pw_name,host,path,str);
    fflush(stdout);
}
int main(int argc, char* argv[],char* envp[])
{
    while( 1 )
    {
        //printf("stu@localhost ~$ ");
        //fflush(stdout);
        print_info();
        char buff[128] = {0};
        fgets(buff,128,stdin);//"\n"
        buff[strlen(buff)-1] = '\0';

        char* myargv[MAX_SIZE] = {0};
        char * cmd = get_cmd(buff,myargv);
        if( cmd == NULL )
        {
            continue;
        }

        if ( strcmp(cmd,"cd") == 0 )
        {
          chdir(myargv[1]);// cd  /home myargv[0]="cd", myargv[1] = "/home"
            continue;
        }
        //ceshi
        //for(int i = 0; myargv[i] != NULL; i++ )
        //{
        //    printf("myargv[%d]=%s\n",i,myargv[i]);
        //}

        if( strcmp(cmd,"exit") == 0)
        {
            break;
        }

        pid_t pid = fork();
        if ( pid == -1 )
        {
            continue;
        }

        if( pid == 0 )
        {
            //execlp(buff,buff,(char*)0);
            execvp(cmd,myargv);
            printf("exec失败\n");
            exit(0);
        }

        wait(NULL);
    }

}

/*
#define  MAX_SIZE  10

char* get_cmd(char buff[],char* myargv[])
{
    int i = 0;
    char* s = strtok(buff," ");//cp a.c b.c 
    while( s != NULL )
    {
        myargv[i++] = s;
        s = strtok(NULL," ");
    }

    return myargv[0];
}
int main(int argc, char* argv[],char* envp[])
{
    while( 1 )
    {
        printf("stu@localhost ~$ ");
        fflush(stdout);
        
        char buff[128] = {0};
        fgets(buff,128,stdin);//"\n"
        buff[strlen(buff)-1] = '\0';

        char* myargv[MAX_SIZE] = {0};
        char* cmd = get_cmd(buff,myargv);
        if( cmd == NULL )
        {
            continue;
        }
        //ceshi
        //for(int i = 0; myargv[i] != NULL; i++ )
        //{
        //    printf("myargv[%d]=%s\n",i,myargv[i]);
        //}

        if( strcmp(cmd,"exit") == 0)
        {
            break;
        }

        pid_t pid = fork();
        if ( pid == -1 )
        {
            continue;
        }

        if( pid == 0 )
        {
            //execlp(buff,buff,(char*)0);
            execvp(cmd,myargv);
            printf("exec失败\n");
            exit(0);
        }

        wait(NULL);
    }

}
*/

/*
int main(int argc, char* argv[],char* envp[])
{
    while( 1 )
    {
        printf("stu@localhost ~$ ");
        fflush(stdout);
        
        char buff[128] = {0};
        fgets(buff,128,stdin);//"\n"
        buff[strlen(buff)-1] = '\0';

        if( strcmp(buff,"exit") == 0)
        {
            break;
        }

        pid_t pid = fork();
        if ( pid == -1 )
        {
            continue;
        }

        if( pid == 0 )
        {
            execlp(buff,buff,(char*)0);
            printf("exec失败\n");
            exit(0);
        }

        wait(NULL);
    }

}
*/
