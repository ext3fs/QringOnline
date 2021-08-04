/*____________________________________________________________

	Project name : Qring online
	File name    : db.h
	Description  : data structure
	Date         : February 14 2005
_____________________________________________________________*/

#ifdef	G
#undef	G
#endif

#ifdef	_DB_H_
#define	G
#else
#define	G	extern
#endif


G	int	D_SetDb_init(void);

G	int	D_GetClient_sec_active(const unsigned short);
G	void	D_SetClient_sec_active(const unsigned short, const int);
G	int	D_GetClient_session(const unsigned short);
G	void	D_SetClient_session(const unsigned short, const int);
G	char  *	D_GetClient_char_name(const unsigned short);
G	void	D_SetClient_char_name(const unsigned short, const char *);
G	void	D_SetClient_clear(const unsigned short);


#undef	G
#undef	_DB_H_

