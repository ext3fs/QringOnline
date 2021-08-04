
#ifndef	__send_h__
#define	__send_h__





#ifdef __cplusplus
extern "C" {
#endif

void*	S_pro_main		( void * );
int	S_channel_init		( void );
void	S_channel_sendbuf_init	( const int, const int );
void	S_channel_sendbuf_in	( const int, const char *, const unsigned short );


#ifdef __cplusplus
}
#endif	



#endif
