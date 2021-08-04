/*____________________________________________________________

	Project name : Qring Online
	File name    : recv.h
	Description  : recv module
	Date         : February 14 2005
_____________________________________________________________*/


#ifdef	G 
#undef	G 
#endif

#ifdef	_RECV_H_
#define	G
#else
#define	G extern 
#endif

G	void	*R_Recv_main(void *);

G	int	R_GetBilling_fd(const int);
G	int	R_GetLogin_dbagent_fd(const int);
G	int	R_GetDbagent_fd(const int);
G	int	R_GetRecvfd_epoll_add(const int);
G	time_t	R_GetRecvfd_recv_sec(const int);


#undef G
#undef _RECV_H_

