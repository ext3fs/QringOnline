
#ifndef	__util_h__
#define	__util_h__



#ifdef __cplusplus
extern "C" {
#endif

unsigned long 	U_change_escape_string		(char *, const char *, unsigned long );

int             dprintf                         ( const int, const char *, ... );

void            U_GetTime_yyyymmddHH		( char * );
time_t		U_get_time			( void );

pid_t           U_pro_fork			( void );

int             U_get_value			( char *, char *, char *, char * );

int		U_ConnectToServer		( const int, const char *, const int, const int );
int		U_SendToServer			( const int, const int, char *, const int );
int		U_RecvFromServer		( const int, const int, char *, const int );

#ifdef __cplusplus
}
#endif	


#endif
