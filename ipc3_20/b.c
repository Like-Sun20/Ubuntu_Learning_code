#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h> 
int main()
   {
       int fdr=open("fifo",O_RDONLY);
      if(fdr==-1)
      {
          printf("open fifo erro\n");
          exit(1);
      }
      printf("fdr=%d\n",fdr);
      while(1)
      {
      char buff[128]={0};
      int n=read(fdr,buff,127);
      if(n==0)
      {
       break;
      }
printf("%s\n",buff);
      }
      close(fdr);
exit(0);
 }
