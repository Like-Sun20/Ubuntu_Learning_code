#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
int main(int argc,char* argv[],char* envp[])
   {
       printf("pid=%d\n",getpid());
      //execl("/usr/bin/ps","ps","-f",(char*)0);
      //execlp("ps","ps","-f",(char*)0);
      //execle("/usr/bin/ps","ps","-f",(char*)0,envp);
      exit(0);
  }   

