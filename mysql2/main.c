#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mysql/mysql.h>

int main()
{
    MYSQL mysql_con;//mysql连接句柄

    MYSQL* mysql = mysql_init(&mysql_con);//初始化
    if( mysql == NULL)
    {
        exit(1);
    }

    mysql = mysql_real_connect(mysql,"127.0.0.1","root","020329","c2409db",3306,NULL,0);
    if( mysql == NULL)
    {
        printf("连接失败\n");
        exit(1);
    }

    printf("连接成功\n");

    char * sql = "select * from student";//"select name from student where age=14";
    if( mysql_query(mysql,sql) != 0 )//执行sql语句，成功返回值为0
    {
        printf("sql query err\n");
    }

    MYSQL_RES * r = mysql_store_result(mysql);//提取结果集, r动态分配
    if( r == NULL )
    {
        printf("提取结果及失败\n");
        exit(1);
    }

    int num = mysql_num_rows(r);//获取查询结果 有多少行
    if( num == 0 )
    {
        printf("没有查到信息\n");
        mysql_free_result(r);//释放r
        exit(0);
    }

    printf("行:%d\n",num);
    int count = mysql_field_count(mysql);//获取有几列
    printf("列：%d\n",count);

    for(int i = 0 ;i < num; i++)
    {
        MYSQL_ROW row = mysql_fetch_row(r);//获取结果集中的一行信息
        for(int j = 0; j < count; j++)
        {
            printf("%s  ",row[j]);
        }
        printf("\n");
    }

    mysql_free_result(r);//释放结果集占用的空间
    mysql_close(mysql);

    exit(0);
}