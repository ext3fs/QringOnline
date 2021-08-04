#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/time.h>
#include	<unistd.h>
#include	<errno.h>
//#include	</usr/local/mysql/include/mysql/mysql.h>
#include	</home/db/include/mysql/mysql.h>

MYSQL 		mysql; 

int 	Mysql_init(void);
int 	Mysql_world_name_table(void);



int 	main()
{
	if( Mysql_init() )
	{
		printf("Error connect() \n" );
		exit(0);
	}

//	if( Mysql_world_name_table() )
//	{
//		printf("Error Mysql_world_name_table() \n" );
//		exit(0);
//	}

	if( Mysql_char_table() )
	{
		printf("Error Mysql_char_table() \n" );
		exit(0);
	}

	return 0;
}


int 	Mysql_init(void)
{ 
	mysql_init(&mysql); 

	//if( !mysql_real_connect(&mysql,"localhost","qring","zbfldeoqkr","script_db", 0, MYSQL_UNIX_ADDR, 0) )
	if( !mysql_real_connect(&mysql,"localhost","qring","dkanrjsk","script_db", 0, MYSQL_UNIX_ADDR, 0) )
	{ 
		printf("** error : %s",mysql_error(&mysql));
		return -1;
	} 


	return 0;
} 


int 	Mysql_world_name_table(void)
{
	char	str_query[1024];
	int	n, row_cnt;


        n  = snprintf( str_query, 1024, "update world_name_table set world_name='피터팬'");

        if( mysql_real_query(&mysql, str_query, n) )
        {
                printf("Error mysql_real_query() : %s \n", mysql_error(&mysql) );
                return -1;
        }

        if( (row_cnt = mysql_affected_rows(&mysql)) != 1 )
        {
                printf("Error mysql_affected_rows(%d) : %s \n", row_cnt, mysql_error(&mysql) );
                return -1;
        }


	return 0;
}


int 	Mysql_char_table(void)
{
	char	str_query[1024];
	int	n, row_cnt;


        n  = snprintf( str_query, 1024, "update char_table set money=100000000 where char_name='내가니아비다'");

        if( mysql_real_query(&mysql, str_query, n) )
        {
                printf("Error mysql_real_query() : %s \n", mysql_error(&mysql) );
                return -1;
        }

        if( (row_cnt = mysql_affected_rows(&mysql)) != 1 )
        {
                printf("Error mysql_affected_rows(%d) : %s \n", row_cnt, mysql_error(&mysql) );
                return -1;
        }


	return 0;
}
