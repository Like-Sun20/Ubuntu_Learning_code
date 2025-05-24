#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<mysql/mysql.h>

int main()
{
    MYSQL mysql_con;
    MYSQL* mysql=mysql_init(&mysql_con);
    if(mysql==NULL)
    {
        exit(1);
    }
    //连接数据库 ip,port,用户名,密码，
    if( mysql_real_connect(mysql,"127.0.0.1","root","020329","c2409db",3306,NULL,0) == NULL )
    {
        printf("连接失败\n");
        exit(1);
    }

    printf("连接成功\n");

    char* sql = "insert into student values(1004,'小张',17)";

    if( mysql_query(mysql,sql) != 0)
    {
        printf("sql err\n");
    }
    
    mysql_close(mysql);
    exit(0);
}