#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <mysql/mysql.h>

#define SIZE 128

int main (int argc, char* argv[])
{
    MYSQL * mysql = NULL;
    MYSQL_RES * mysql_result = NULL;
    MYSQL_FIELD *  mysql_fields ;
    MYSQL_ROW  mysql_row;
    
    int num_column = 0;
    int ret  = -1;
    char* sql;
    char buffer[SIZE];

    // 1. construct of mysql handle;
    mysql = mysql_init(mysql);
    if( NULL == mysql)
    {
        printf("mysql_init failed...\n");
        goto err0;
    }
    if(NULL == mysql_real_connect(mysql, "127.0.0.1", "root", "123456", "scott", 3306, NULL, 0 ))
    {
        printf("\033[31mError Massage: \033[0m%s\n", mysql_error(mysql));
        goto err0;
    }

    while(1)
    {
        printf("mysql>");
        fflush(stdout);

        bzero(buffer, SIZE);
        ret = read(STDIN_FILENO, buffer, SIZE);
        if(-1 == ret)
        {
            perror("read");
            goto err1;
        }
        sql = buffer;
        if(strncmp(sql, "exit\n", 5) == 0 || strncmp(sql, "quit\n", 5) == 0 )
        {
            printf("Auf Wiedersehen...\n");
            break;
        }
        if(strncmp(sql, "\n", 1) == 0)
        {
            continue;
        }
        
        if( mysql_query(mysql, sql))
        {
            printf("mysql_query failed...\n");
            continue;
        }

        mysql_result = mysql_store_result(mysql);
        if(NULL == mysql_result)
        {
            printf("mysql_store_result failed...\n");
            continue;
        }

        num_column = mysql_num_fields(mysql_result);
        mysql_fields = mysql_fetch_field(mysql_result);

        for(int i = 0; i< num_column; i++)
        {
            printf("%10s\t", mysql_fields[i].name);
        }
        printf("\n---\n");


        while(1)
        {
            mysql_row = mysql_fetch_row(mysql_result);
            if(NULL == mysql_row)
            {
                printf("mysql_fetch_row ended...\n");
                break;
            }
            for(int i = 0; i< num_column; i++)
            {
                printf("%10s\t ", NULL == mysql_row[i] ? "null": mysql_row[i]  );
            }
            printf("\n");

        }

    }

    // 2. destroy mysql handle;
    mysql_close(mysql);

    return 0;
err1:
    mysql_close(mysql);
err0:
    return 1;
}
