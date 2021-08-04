
#ifndef	__db_h__
#define	__db_h__





#ifdef __cplusplus
extern "C" {
#endif

int	D_set_version			( void );
int	D_check_script			( void );
int	D_set_script			( void );

int	D_ProSL_init			( void );
void	D_SetSL_fd			( const int, const int );
int	D_GetSL_fd			( const int );
void	D_SetSL_ipaddress		( const int, const char * );
char *	D_GetSL_ipaddress		( const int );
void	D_SetSL_login			( const int, const int );
int	D_GetSL_login			( const int );

int	D_SLMysql_Init			( const int );
int	D_SLMysql_Close			( const int );

int	D_Mysql_CheckIDX		( const int );
int	D_Mysql_Init			( const int );
int	D_Mysql_Close			( const int );
int	D_Mysql_version			( const int, char * );

int	D_WS_MYINFO			( const int, const char *, int *, int * );
int	D_WS_WORLD_INFO			( const int, const int, int *, char * );
int	D_WS_GAMESERV_INFO		( const int, const int, const int, int *, char * );
int	D_WS_DBAGENT_INFO		( const int, const int, const int, int *, char * );
int	D_WS_MOBSERV_INFO		( const int, const int, const int, int *, char * );


#ifdef __cplusplus
}
#endif	



#endif
