
#ifndef	__util_h__
#define	__util_h__


struct recv_q
{
	volatile int		channel;

	volatile int		command;
	volatile int		buf_size;
	char			buf[SIZEOF_MAX_BODY + 1];
	volatile int		next;
};

struct recv_queue_pool
{
	pthread_mutex_t		locked;
	struct recv_q		queue[MAX_QUEUE_CNT];
	volatile sig_atomic_t	head;
	volatile sig_atomic_t	tail;
};


#ifdef __cplusplus
extern "C" {
#endif

int		U_set_queue			( void );
int		U_recv_queue_in			( const int, const int, const int, const int, const char * );
struct recv_q*	U_recv_queue_out		( const int );
int		U_recv_queue_tail		( const int );
int		U_recv_queue_head		( const int );

int		U_ConnectToServer		( const int, const char *, const int, const int );
int		U_SendToServer			( const int, const int, char *, const int );
int		U_RecvFromServer		( const int, const int, char *, const int );

int		U_set_random			( const int );
int		U_get_random			( const int );

int     	U_set_logdir			( void );
int             dprintf                         ( const int, const char *, ... );

pid_t           U_pro_fork			( void );

int		U_get_line			( const char *, const int );
int             U_get_value			( char *, char *, char *, char * );


#ifdef __cplusplus
}
#endif	


#endif
