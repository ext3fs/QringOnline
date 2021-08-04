
#ifndef	__util_h__
#define	__util_h__



#ifdef __cplusplus
extern "C" {
#endif

unsigned long 	U_change_escape_string		(char *, const char *, unsigned long );

int             dprintf                         ( const int, const char *, ... );

void            U_GetTime_yyyymmdd		( char * );
time_t		U_Get_time			( void );

pid_t           U_pro_fork			( void );

int		U_get_length			( const char *, const char *, unsigned int * );
int		U_get_data			( const char *, const char *, const int, char * );
int		U_get_line			( const char *, const char * );

int             U_get_value			( char *, char *, char *, char * );


#ifdef __cplusplus
}
#endif	


#endif
