
#include	"global.h"
#include	"util.h"
#include	"db.h"
#include	"accept.h"





// ----------------------------------------------------------------------------
// acpt	variable
static int			A_Input_FDS;

static int			A_recvt_status[MAX_RECVT_CNT];

static int			A_msg_type;
static int			A_msg_fd;

static struct sockaddr_in	A_acpt_addr;
static int			A_addr_len;
static struct epoll_event	A_event;





// ----------------------------------------------------------------------------
// acpt	function
static inline void	A_thread_exit			( void );
static inline void	A_recv_thr_exit			( const int );

static inline int	A_proc_inner_close_fd		( const int );
static inline void	A_proc_inner_recv_fd		( const int, const int );
static inline void	A_proc_INTER_CONN_FD_SUCC	( const int );
static inline void	A_proc_INTER_CONN_FD_FAIL	( const int );
static inline void	A_proc_INTER_CLOSE_FD		( const int );
static inline void	A_proc_tcp_fd			( const int );
static inline int	A_proc_acpt_fd			( void );




void	*A_pro_main( void *arg )
{
	
	int			isExit, nEvent, event_fd, i, j;
	struct epoll_event	ev[256];



	dprintf(M_LOG, "[ACPT]Thread Start !! \n\n" );


	// RecvT Status
	for( i=0; i<MAX_RECVT_CNT; i++ )
		A_recvt_status[i]	= THREAD_STATUS_EMPTY;


	// epoll_create()
	if( (A_Input_FDS = epoll_create(256)) < 0 )
	{
		dprintf(M_LOG, "[ACPT]Error epoll_create() : %s \n", strerror(errno) );
		A_thread_exit();
		return NULL;
	}


	// add acpt_close_fd
	A_event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
	A_event.data.fd	= G_inner_acpt_fd[1];
	if( epoll_ctl(A_Input_FDS, EPOLL_CTL_ADD, G_inner_acpt_fd[1], &A_event) < 0 )
	{
		dprintf(M_LOG, "[ACPT]Error epoll_ctl_add()-close : %s \n", strerror(errno) );
		A_thread_exit();
		return NULL;
	}


	// add recvT_fd
	for( i=0; i<MAX_RECVT_CNT; i++ )
	{
		A_event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
		A_event.data.fd	= G_inner_recv_fd[i][0];
		if( epoll_ctl(A_Input_FDS, EPOLL_CTL_ADD, G_inner_recv_fd[i][0], &A_event) < 0 )
		{
			dprintf(M_LOG, "[ACPT]Error epoll_ctl_add()-recvT: %s \n", strerror(errno) );
			A_thread_exit();
			return NULL;
		}
	}


	// add tcp_fd
	A_event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
	A_event.data.fd	= G_tcp_fd;
	if( epoll_ctl(A_Input_FDS, EPOLL_CTL_ADD, G_tcp_fd, &A_event) < 0 )
	{
		dprintf(M_LOG, "[ACPT]Error epoll_ctl_add()-tcp : %s \n", strerror(errno) );
		A_thread_exit();
		return NULL;
	}


	isExit	= 0;


	while( !isExit )
	{
	
		if( (nEvent = epoll_wait(A_Input_FDS, ev, 256, -1)) < 0 )
		{
			dprintf(M_LOG, "[ACPT]Error epoll_wait(). nEvent(%d) : %s \n", nEvent, strerror(errno) ), sleep(1);
			continue;
		}


		for( i=0; i<nEvent; i++ )
		{

			event_fd	= ev[i].data.fd;
			

			// inner_acpt_fd
			if( event_fd == G_inner_acpt_fd[1] )
			{
				if( A_proc_inner_close_fd(event_fd) )
					isExit	= 1;

				continue;
			}

			// inner_recv_fd
			for( j=0; j<MAX_RECVT_CNT; j ++ )
			{
				if( event_fd == G_inner_recv_fd[j][0] )
				{
					A_proc_inner_recv_fd( j, event_fd );
					break;
				}
			}

			// tcp_fd
			if( event_fd == G_tcp_fd )
			{
				A_proc_tcp_fd( event_fd );
				continue;
			}


		}// end for !!


	}// end while !!


	A_thread_exit();


	dprintf(M_LOG, "[ACPT]Thread End !! \n\n" );

	return NULL;
}


static inline void	A_thread_exit( void )
{
	int	i;

	dprintf(M_LOG, "[ACPT]Thread_Exit \n");

        // delete epoll
	epoll_ctl(A_Input_FDS, EPOLL_CTL_DEL, G_inner_acpt_fd[1], &A_event);
	epoll_ctl(A_Input_FDS, EPOLL_CTL_DEL, G_tcp_fd, &A_event);
	for(i=0; i<MAX_RECVT_CNT; i++)
		epoll_ctl(A_Input_FDS, EPOLL_CTL_DEL, G_inner_recv_fd[i][0], &A_event);
	close(A_Input_FDS);

	// send inner_recv_fd
	for( i=0; i<MAX_RECVT_CNT; i++ )
		A_recv_thr_exit(i);
}


static inline void	A_recv_thr_exit( const int thread )
{
	dprintf(M_LOG, "[ACPT]A_recv_thr_exit() : thread(%d) \n", thread);

	epoll_ctl( A_Input_FDS, EPOLL_CTL_DEL, G_inner_recv_fd[thread][0], &A_event );

	A_msg_type = INTER_EXIT_THREAD;
	send( G_inner_recv_fd[thread][0], (char *)&A_msg_type, SIZEOF_INT, 0 );

	close(G_inner_recv_fd[thread][0]);
	A_recvt_status[thread]	= THREAD_STATUS_ERROR;
}




// --------------------------------------------------------------------------------------
//
static inline int	A_proc_inner_close_fd( const int close_fd )
{
	int	n;


	if( (n = recv( close_fd, &A_msg_type, SIZEOF_INT, 0)) != SIZEOF_INT )
	{	
		dprintf(M_LOG, "[ACPT]Error A_proc_inner_close_fd() recv byte(%d) \n", n );	
		return FAIL;
	}


	switch( A_msg_type )
	{
		case	INTER_EXIT_THREAD :
			dprintf(M_LOG, "[ACPT]recv INTER_EXIT_THREAD. \n" );
			return FAIL;
		default :
			dprintf(M_LOG, "[ACPT]A_proc_inner_close_fd() unknown type(%x) \n", A_msg_type );
			return FAIL;
	}

	return SUCC;
}




// --------------------------------------------------------------------------------------
//
static inline void	A_proc_inner_recv_fd( const int thread, const int inner_fd )
{
	int	n;


	if( (n = recv( inner_fd, &A_msg_type, SIZEOF_INT, 0)) != SIZEOF_INT )
	{	
		dprintf(M_LOG, "[ACPT]Error A_proc_inner_recv_fd() recv byte(%d) \n", n );	
		A_recv_thr_exit(thread);
		return;
	}

	switch( A_msg_type )
	{
		case	INTER_CONN_FD_SUCC :
			A_proc_INTER_CONN_FD_SUCC(thread);
			return;
		case	INTER_CONN_FD_FAIL :
			A_proc_INTER_CONN_FD_FAIL(thread);
			return;
		case	INTER_CLOSE_FD :
			A_proc_INTER_CLOSE_FD(thread);
			return;
		default :
			dprintf(M_LOG, "[ACPT]A_proc_inner_recv_fd() unknown type(%x) \n", A_msg_type);
			A_recv_thr_exit(thread);
			return;
	}
}


static inline void	A_proc_INTER_CONN_FD_SUCC( const int thread )
{
	A_recvt_status[thread] = THREAD_STATUS_RUNNING;

	//dprintf(M_LOG, "[ACPT]recv INTER_CONN_FD_SUCC thread(%d), connection(%d) \n", thread, G_connection );
}


static inline void	A_proc_INTER_CONN_FD_FAIL( const int thread )
{
	G_connection --;

	A_recvt_status[thread] = THREAD_STATUS_EMPTY;

	//dprintf(M_LOG, "[ACPT]recv INTER_CONN_FD_FAIL thread(%d), connection(%d) \n", thread, G_connection );
}


static inline void	A_proc_INTER_CLOSE_FD( const int thread )
{
	G_connection --;

	A_recvt_status[thread] = THREAD_STATUS_EMPTY;

	//dprintf(M_LOG, "[ACPT]recv INTER_CLOSE_FD thread(%d), connection(%d) \n", thread, G_connection );
}




// --------------------------------------------------------------------------------------
//
static inline void	A_proc_tcp_fd( const int accept_fd )
{
	struct linger		ling;
	int			buf_size, flags;
	char			ipaddress[16];


	A_addr_len	= sizeof(A_acpt_addr);


	if( (A_msg_fd = accept(accept_fd, (struct sockaddr *)&A_acpt_addr, &A_addr_len)) < 0 )
	{
		dprintf(M_LOG, "[ACPT]Error A_proc_tcp_fd() accept() : %s \n", strerror(errno) );
		return;
	}

	if( G_connection > (MAX_RECVT_CNT - 1) )
	{
		close( A_msg_fd );
		return;
	}

	strcpy( ipaddress, inet_ntoa(A_acpt_addr.sin_addr) );


	// setsocketopt()
	flags = fcntl( A_msg_fd, F_GETFL, 0 );
	fcntl( A_msg_fd, F_SETFL, flags | O_NONBLOCK );

	ling.l_onoff = ling.l_linger = 0;
	setsockopt( A_msg_fd, SOL_SOCKET, SO_LINGER, (char *)&ling, sizeof(struct linger) );

	buf_size = SIZEOF_SNDBUF;
	setsockopt( A_msg_fd, SOL_SOCKET, SO_SNDBUF, &buf_size, SIZEOF_INT );
	buf_size = SIZEOF_RCVBUF;
	setsockopt( A_msg_fd, SOL_SOCKET, SO_RCVBUF, &buf_size, SIZEOF_INT );

	if( A_proc_acpt_fd() )
	{
		dprintf(M_LOG, "[ACPT]Error A_proc_acpt_fd() : fd(%d), ipaddress(%s) \n", A_msg_fd, ipaddress);
		close( A_msg_fd );
		return;
	}

	G_connection ++;

	dprintf(M_LOG, "[ACPT]accept fd(%d), ipaddress(%s), connection(%d) \n", A_msg_fd, ipaddress, G_connection );
}


static inline int	A_proc_acpt_fd( void )
{
	int	i, n, isSuccess=0;
	char	send_data[1024];


	A_msg_type = INTER_CONN_FD;
	memcpy( send_data	, &A_msg_type	, SIZEOF_INT );
	memcpy( send_data + 4	, &A_msg_fd	, SIZEOF_INT );

	for( i=0; i<MAX_RECVT_CNT; i++ )
	{
		if( A_recvt_status[i] != THREAD_STATUS_EMPTY )
			continue;

		if( (n = send(G_inner_recv_fd[i][0], send_data, 8, 0)) != 8 )
		{
			dprintf(M_LOG, "[ACPT]Error A_proc_acpt_fd() send(%d) : %s \n", n, strerror(errno) );
			A_recv_thr_exit( i );
			continue;
		}

		A_recvt_status[i] = THREAD_STATUS_RUNNING;
		isSuccess	= 1;
		break;
	}
	
	if( !isSuccess )
	{
		dprintf(M_LOG, "[ACPT]Error A_proc_acpt_fd() RecvThread full \n");
		return FAIL;
	}


	//dprintf(M_LOG, "[ACPT]send INTER_CONN_FD(%d) \n", A_msg_fd);
	return SUCC;
}
