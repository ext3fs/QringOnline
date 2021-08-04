
#ifndef	__db_h__
#define	__db_h__



#ifdef __cplusplus
extern "C" {
#endif

int	D_check_version			( void );

void	D_ProSL_init			( void );
void	D_SetSL_fd			( const int, const int );
int	D_GetSL_fd			( const int );
void	D_SetSL_ipaddress		( const int, const char * );
char *  D_GetSL_ipaddress		( const int );
void	D_SetSL_login			( const int, const int );
int	D_GetSL_login			( const int );
void	D_SetSL_live_send		( const int, const int );
int	D_GetSL_live_send		( const int );
void	D_SetSL_last_recvtime		( const int );
time_t	D_GetSL_last_recvtime		( const int );

int	D_SLMysql_Init			( const int );
int	D_SLMysql_Close			( const int );

int	D_Mysql_all_logout		( const int, const int );
int	D_Mysql_WA_LOGIN		( const int, const char *, const char *, const int, const int, const int, const int
					, int *, int *, int *, int *, int * );
int	D_Mysql_RecordLockFree		( const int, const char *, const int, const int, const int, const int, int * );
int	D_Mysql_WA_LOGOUT		( const int, const char *, const int, const int, const int, const int, int * );
int	D_Mysql_WA_CURRENT_USER		( const int, const struct body_WA_CURRENT_USER *, int * );
int	D_Mysql_WA_GAME_LOG		( const int, const struct body_WA_GAME_LOG *, int * );
int	D_Mysql_WA_GET_USER		( const int, const struct body_WA_GET_USER *, int * );

int	D_Mysql_WA_CHAR_LIST		( const int, const char *, int *, struct body2_AW_CHAR_LIST_RESULT *, int * );
int	D_Mysql_WA_CHAR_CREATE		( const int, const struct body_WA_CHAR_CREATE *, int * );
int	D_Mysql_WA_CHAR_DELETE		( const int, const char *, const char *, int * );
int	D_Mysql_WA_CHAR_LOAD		( const int, const char *, const char *, struct body1_AW_CHAR_LOAD_RESULT *
					, struct body2_AW_CHAR_LOAD_RESULT *, int * );
int	D_Mysql_WA_CHAR_SAVE		( const int, const struct body1_WA_CHAR_SAVE *, const struct body2_WA_CHAR_SAVE *, int * );


#ifdef __cplusplus
}
#endif	



#endif
