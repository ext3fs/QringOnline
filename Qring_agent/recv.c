
#include	"global.h"
#include	"util.h"
#include	"db.h"
#include	"recv.h"



// ----------------------------------------------------------------------------
// recv	variable
static volatile int			R_Input_FDS[MAX_RECVT_CNT];
static struct epoll_event 		R_event[MAX_RECVT_CNT];

static volatile char			R_complete_start[MAX_RECVT_CNT];
static volatile time_t			R_complete_sec[MAX_RECVT_CNT];

static char				R_recv_buf[MAX_RECVT_CNT][SIZEOF_RCVBUF];
static volatile int			R_recv_pre[MAX_RECVT_CNT];
static volatile int			R_recv_pos[MAX_RECVT_CNT];

static char				R_send_buf[MAX_RECVT_CNT][SIZEOF_SNDBUF];
static volatile int			R_send_pre[MAX_RECVT_CNT];
static volatile int			R_send_pos[MAX_RECVT_CNT];






// ----------------------------------------------------------------------------
// recv	function
static inline void	R_Thread_Exit			( const int );

static inline int	R_pro_inner_recv_fd		( const int, const int );
static inline int	R_pro_INTER_CONN_FD		( const int, const int );
static inline void	R_pro_Close			( const int );

static inline int	R_pro_LiveCheck			( const int, const int );
static inline int	R_pro_RecvMsg			( const int, const int );
static inline int	R_pro_ProcMsg			( const int, const int, const int );
static inline int	R_pro_SendMsg			( const int, const int );
static inline int	R_pro_RemainMsg			( const int, const int );

static inline int	R_pro_InputHeader		( const int, const int, const short, const short, const int, const int );
static inline int	R_pro_InputBody			( const int, const int, const int, const char * );
static inline int	R_pro_GetPacket			( const int, struct header_special *, char * );


static inline int 	R_pro_WA_INIT			( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WA_LIVE			( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WA_LOGIN			( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WA_LOGOUT			( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WA_CHAR_LIST		( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WA_CHAR_CREATE		( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WA_CHAR_DELETE		( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WA_CHAR_LOAD		( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WA_CHAR_SAVE		( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WA_CURRENT_USER		( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WA_GAME_LOG		( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WA_GET_USER		( const int, const int, const struct header_special, const char * );





void	*R_pro_main( void *arg )
{

	int			event_fd;
	int			i, nEvent, wait_type;
	int			isConnect, isError, isExit;
	struct epoll_event	ev[2];
	int			TIDX = (int)arg;


	dprintf(TIDX, "[RECV]Thread(%d) Start !! \n\n", TIDX );


	// checking TIDX 
	if( TIDX < 0 || TIDX > (MAX_RECVT_CNT - 1) )
	{
		dprintf(TIDX, "[RECV]Error thread_idx(%d) \n", TIDX);
		return NULL;
	}
	

	// epoll_create()
	if( (R_Input_FDS[TIDX] = epoll_create(2)) < 0 )
	{
		dprintf(TIDX, "[RECV]Error epoll_create() : %s \n", strerror(errno) );
		R_Thread_Exit( TIDX );
		return NULL;
	}


	// add recv_close_fd
	R_event[TIDX].events    = EPOLLIN|EPOLLERR|EPOLLHUP;
	R_event[TIDX].data.fd   = G_inner_recv_fd[TIDX][1];
	if( epoll_ctl(R_Input_FDS[TIDX], EPOLL_CTL_ADD, G_inner_recv_fd[TIDX][1], &R_event[TIDX] ) < 0 ){
		dprintf(TIDX, "[RECV]Error epoll_ctl_add()-close : %s\n", strerror(errno) );
		R_Thread_Exit( TIDX );
		return NULL;
	}


	// mysql init()
	if( D_SLMysql_Init(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error D_SLMysql_Init(%d) \n", TIDX);
		R_Thread_Exit( TIDX );
		return NULL;
	}

	isConnect	= 0;
	isError		= 0;
	isExit		= 0;


	while( !isExit )
	{

		// waiting type
		if( isConnect )
		{
			if( (R_recv_pos[TIDX] - R_recv_pre[TIDX]) || (R_send_pos[TIDX] - R_send_pre[TIDX]) )
				wait_type	= 0;
			else
				wait_type       = (1000);
		}
		else
		{
			wait_type	= -1;
		}


                // event wait
                if((nEvent = epoll_wait(R_Input_FDS[TIDX], ev, 2, wait_type)) < 0 )
		{
                        dprintf(TIDX, "[RECV]Error epoll_wait() nEvent(%d) : %s\n", nEvent, strerror(errno) ), sleep(1);
                        continue;
                }


		// recv events
		for( i=0; i<nEvent; i++ )
		{

			event_fd	= ev[i].data.fd;


			// inner message
			if( event_fd == G_inner_recv_fd[TIDX][1] )
			{
				if( R_pro_inner_recv_fd(TIDX, event_fd) )
					isExit		= 1;
				else
					isConnect	= 1;
			}

			// client message
			else if( event_fd == D_GetSL_fd(TIDX) )
			{
				if( R_pro_RecvMsg(TIDX, event_fd) )
				{
					//dprintf(TIDX, "[RECV]Error RecvMsg() \n");
					isError		= 1;
				}
			}

			// wrong fd
			else
			{
				dprintf(TIDX, "[RECV]Error epoll_wait(). wrong fd(%d) \n", event_fd );

				R_event[TIDX].data.fd   = event_fd;
				if( epoll_ctl(R_Input_FDS[TIDX], EPOLL_CTL_DEL, event_fd, &R_event[TIDX]) < 0 )
					dprintf(TIDX, "[RECV]Error epoll_ctl_del(%d) : %s\n", event_fd, strerror(errno) );
				close( event_fd );

				isExit = 1;
			}


		}// end for !!


		if( !isConnect && !isError )
			continue;

		
		// proc msg
		if( isConnect && !isError && R_pro_ProcMsg(TIDX, D_GetSL_fd(TIDX), CONN_STATUS_NORMAL) )
		{
			dprintf(TIDX, "[RECV]Error ProcMsg() \n");
			isError	= 1;
		}


		// send msg
		if( isConnect && !isError && R_pro_SendMsg(TIDX, D_GetSL_fd(TIDX)) )
		{
			dprintf(TIDX, "[RECV]Error SendMsg() \n");
			isError	= 1;
		}

		// live check
		if( isConnect && !isError && R_pro_LiveCheck(TIDX, D_GetSL_fd(TIDX)) )
		{
			dprintf(TIDX, "[RECV]Error LiveCheck() \n");
			isError	= 1;
		}

		
		// Error check
		if( isConnect && isError )
		{
			R_pro_RemainMsg( TIDX, D_GetSL_fd(TIDX) );
			R_pro_Close( TIDX );
			
			isError		= 0;
			isConnect	= 0;
		}


	}// end while !!


	if( isConnect && !isError )
	{
		R_pro_RemainMsg( TIDX, D_GetSL_fd(TIDX) );
		R_pro_Close( TIDX );
	}

	R_Thread_Exit( TIDX );
	dprintf(TIDX, "[RECV]Thread(%d) End !! \n\n", TIDX );
	return NULL;
}


static inline void	R_Thread_Exit( const int TIDX )
{
	dprintf(TIDX, "[RECV]Thread_Exit(%d) \n", TIDX );
	close( G_inner_recv_fd[TIDX][1] );
	close( R_Input_FDS[TIDX] );

	// mysql close()
	D_SLMysql_Close( TIDX );
}



// ----------------------------------------------------------------------------
//
static inline int	R_pro_inner_recv_fd( const int TIDX, const int FD )
{
	int	msg_type, msg_fd, n, send_msg_type;


	if( (n = recv(FD, &msg_type, SIZEOF_INT, 0)) != SIZEOF_INT )
	{
		dprintf(TIDX, "[RECV]Error R_pro_inner_recv_fd() : %s \n", strerror(errno) );
		return FAIL;
	}


	switch( msg_type )
	{
		case    INTER_CONN_FD:
			send_msg_type = INTER_CONN_FD_SUCC;
			if( (n = recv(FD, &msg_fd, SIZEOF_INT, 0)) != SIZEOF_INT )
			{
				dprintf(TIDX, "[RECV]Error R_pro_inner_recv_fd() : %s \n", strerror(errno) );
				return FAIL;
			}

			if( R_pro_INTER_CONN_FD(TIDX, msg_fd) )
				send_msg_type 	= INTER_CONN_FD_FAIL;

			send( FD, &send_msg_type, 4, 0 );
			return SUCC;

		case    INTER_EXIT_THREAD :
			dprintf(TIDX, "[RECV]recv INTER_EXIT_THREAD \n");
			return FAIL;

		default :
			dprintf(TIDX, "[RECV]Error R_pro_inner_recv_fd() unknown command(%x) \n", msg_type );
			return FAIL;
	}

	return SUCC;
}


static inline int	R_pro_INTER_CONN_FD( const int TIDX, const int fd )
{

	// epoll add
	R_event[TIDX].events    = EPOLLIN|EPOLLERR|EPOLLHUP|EPOLLET;
	R_event[TIDX].data.fd   = fd;
	if( epoll_ctl(R_Input_FDS[TIDX], EPOLL_CTL_ADD, fd, &R_event[TIDX]) < 0 )
	{
	        dprintf(TIDX, "[RECV]Error R_pro_INTER_CONN_FD() epoll_ctl_add(%d) : %s\n", fd, strerror(errno) );
		close(fd);
		return FAIL;
	}

	// SL init()
	D_SetSL_fd( TIDX, fd );
	D_SetSL_last_recvtime( TIDX );
	D_SetSL_live_send( TIDX, 0 );
	D_SetSL_login( TIDX, 0 );

	// buffer init()
	R_recv_pre[TIDX] = R_recv_pos[TIDX] = 0;
	R_send_pre[TIDX] = R_send_pos[TIDX] = 0;

	// last_recvtime()
	R_complete_start[TIDX] = 0;


	dprintf(TIDX, "[RECV]R_pro_INTER_CONN_FD(%d)\n", fd);
	return SUCC;
}


static inline void	R_pro_Close( const int TIDX )
{
	int	msg_type;
	int	close_fd = D_GetSL_fd(TIDX);


	// SL init()
	D_SetSL_login( TIDX, 0 );
	D_SetSL_fd( TIDX, FD_IS_EMPTY );

	// buffer init()
	R_recv_pre[TIDX] = R_recv_pos[TIDX] = 0;
	R_send_pre[TIDX] = R_send_pos[TIDX] = 0;

	// epoll del()
	epoll_ctl( R_Input_FDS[TIDX], EPOLL_CTL_DEL, close_fd, &R_event[TIDX] );
	close( close_fd );

	// notify acpt()
	msg_type = INTER_CLOSE_FD;
	send(G_inner_recv_fd[TIDX][1], &msg_type, SIZEOF_INT, 0);

	dprintf(TIDX, "[RECV]R_pro_INTER_CLOSE_FD(%d)\n", close_fd);
}





// ----------------------------------------------------------------------------
//
static inline int	R_pro_LiveCheck( const int TIDX, const int FD )
{
	long	flow_time = (U_get_time() - D_GetSL_last_recvtime(TIDX));


	// close fd status 
	if( flow_time > LIMIT_SEC_TO_CLOSE )
	{
		dprintf(TIDX, "[RECV]R_pro_LiveCheck() time out !! \n");
		return FAIL;
	}

	
	// warning fd status 
	if( flow_time > LIMIT_SEC_TO_WARNING )
	{
		if( D_GetSL_live_send(TIDX) )
			return SUCC;

		if( R_pro_InputHeader(TIDX, AW_LIVE, 0, 0, 0, 0) )
		{
			dprintf(TIDX, "[RECV]Error R_pro_InputHeader() : AW_LIVE \n");
			return FAIL;
		}

		D_SetSL_live_send( TIDX, 1 );
	}


	return SUCC;
}


static inline int	R_pro_RecvMsg( const int TIDX, const int FD )
{
	int	n, move_len;


	if( (SIZEOF_RCVBUF - R_recv_pos[TIDX]) < SIZEOF_MAX_PACKET )
	{
		move_len = (R_recv_pos[TIDX] - R_recv_pre[TIDX]);
		memmove( R_recv_buf[TIDX], (R_recv_buf[TIDX] + R_recv_pre[TIDX]), move_len );

		R_recv_pre[TIDX]	= 0;
		R_recv_pos[TIDX]	= move_len;
	}


	n = recv( FD, (R_recv_buf[TIDX] + R_recv_pos[TIDX]), (SIZEOF_RCVBUF - R_recv_pos[TIDX]), 0 );

	if( n < 1 )
	{
		if( errno == 0 ){
			dprintf(TIDX, "[RECV]recv normal close FD (%d) \n", FD );
			return FAIL;
		}
		else if( errno != EWOULDBLOCK ){
			dprintf(TIDX, "[RECV]recv abnormal close FD (%d) : %s \n", FD, strerror(errno) );
			return FAIL;
		}

		return SUCC;
	}

	R_recv_pos[TIDX]	+= n;


	return SUCC;
}


static inline int	R_pro_SendMsg( const int TIDX, const int FD )
{
	int	n, move_len;


	if( (R_send_pos[TIDX] < 0) || (R_send_pos[TIDX] > (SIZEOF_SNDBUF - 1)) )
		return FAIL;


	if( R_send_pos[TIDX] == 0 )
		return SUCC;


	n = send( FD, (R_send_buf[TIDX] + R_send_pre[TIDX]), (R_send_pos[TIDX] - R_send_pre[TIDX]), 0 );

	if( n < 0 )
	{
		if( errno == 0 ){
			dprintf(TIDX, "[RECV]send normal close : %s \n", strerror(errno) );
			return FAIL;
		}
		else if( errno != EWOULDBLOCK ){
			dprintf(TIDX, "[RECV]send abnormal close : %s \n", strerror(errno) );
			return FAIL;
		}
	}

	if( n > 0 )
		R_send_pre[TIDX]	+= n;

	if( (SIZEOF_SNDBUF - R_send_pos[TIDX]) < SIZEOF_MAX_PACKET )
	{
		move_len = (R_send_pos[TIDX] - R_send_pre[TIDX]);
		memmove( R_send_buf[TIDX], (R_send_buf[TIDX] + R_send_pre[TIDX]), move_len );

		R_send_pre[TIDX]	= 0;
		R_send_pos[TIDX]	= move_len;
	}


	return SUCC;
}


static inline int	R_pro_RemainMsg( const int TIDX, const int FD )
{
	int	buff_len1, buff_len2;


	buff_len1 = buff_len2 = 0;

	while( (buff_len1 = (R_recv_pos[TIDX] - R_recv_pre[TIDX])) >= SIZEOF_HEADER_DBAGENT )
	{
		if( R_pro_ProcMsg(TIDX, FD, CONN_STATUS_ABNORMAL) )
		{
			dprintf(TIDX, "[RECV]Error R_pro_RemainMsg() R_pro_ProcMsg() \n");
			break;
		}

		if( buff_len1 == buff_len2 )
			break;
		else
			buff_len2	= buff_len1;
	}

	return SUCC;
}





// ----------------------------------------------------------------------------
//
static inline int	R_pro_InputHeader( const int TIDX, const int command, const short body_size
		, const short fd, const int sec_active, const int session )
{
	struct header_special	send_header;
	int			packet_len = (SIZEOF_HEADER_DBAGENT + body_size);


	if( packet_len > SIZEOF_MAX_PACKET )
	{
		dprintf(TIDX, "[RECV]Error R_pro_InputHeader() packet length(%d) \n", packet_len );
		return FAIL;
	}

	else if( packet_len > (SIZEOF_SNDBUF - R_send_pos[TIDX]) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_InputHeader() packet_len(%d), remain(%d) \n"
			, packet_len, (SIZEOF_SNDBUF - R_send_pos[TIDX]) );
		return FAIL;
	}

	send_header.command	= command;
	send_header.body_size	= body_size;
	send_header.fd		= fd;
	send_header.sec_active	= sec_active;
	send_header.session	= session;

	memcpy( (R_send_buf[TIDX] + R_send_pos[TIDX]), &send_header, SIZEOF_HEADER_DBAGENT );
	R_send_pos[TIDX]	+= SIZEOF_HEADER_DBAGENT;

	return SUCC;
}


static inline int	R_pro_InputBody( const int TIDX, const int pre_bodylen, const int add_bodylen, const char *add_data )
{
	if( (pre_bodylen < 0) || (add_bodylen < 0) || ((pre_bodylen + add_bodylen) > SIZEOF_MAX_BODY) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_InputBody() pre(%d), add(%d) \n", pre_bodylen, add_bodylen);
		return FAIL;
	}

	memcpy( R_send_buf[TIDX] + R_send_pos[TIDX], add_data, add_bodylen );
	R_send_pos[TIDX]	+= add_bodylen;

	return add_bodylen;
}


static inline int	R_pro_GetPacket( const int TIDX, struct header_special *pHeader, char *pMsgBody )
{
	unsigned long 	packet_len, flow_time;
	int		buff_len = (R_recv_pos[TIDX] - R_recv_pre[TIDX]);


	// check buffer - header
	if( buff_len < SIZEOF_HEADER_DBAGENT )
		return (0);

	memcpy( pHeader, (R_recv_buf[TIDX] + R_recv_pre[TIDX]), SIZEOF_HEADER_DBAGENT );
	buff_len	-= SIZEOF_HEADER_DBAGENT;

	if( (pHeader->command & 0xff000000) != _WA )
	{
		dprintf(TIDX, "[RECV]Error R_pro_GetPacket() unknown command(%x), body_size(%d) \n"
			 , pHeader->command, pHeader->body_size );
		return (-1);
	}


	// check buffer - body
	if( pHeader->body_size > SIZEOF_MAX_BODY )
	{
		dprintf(TIDX, "[RECV]Error R_pro_GetPacket() Header's body length. (%d/%d) \n"
			, pHeader->body_size, SIZEOF_MAX_BODY );
		return (-1);
	}

	if( pHeader->body_size > buff_len )
	{
		if( !R_complete_start[TIDX] )
		{
			R_complete_sec[TIDX] 	= U_get_time();
			R_complete_start[TIDX] 	= 1;
		}

		flow_time = (U_get_time() - R_complete_sec[TIDX]);

		if( flow_time > LIMIT_SEC_TO_PROC )
		{
			dprintf(TIDX, "[RECV]Error R_pro_GetPacket() over last proctime : header.command(%x) \n", pHeader->command);
			return (-2);
		}

		return (0);
	}

	memcpy( pMsgBody, (R_recv_buf[TIDX] + R_recv_pre[TIDX] + SIZEOF_HEADER_DBAGENT), pHeader->body_size );


	// buffer point move
	packet_len	  = (SIZEOF_HEADER_DBAGENT + pHeader->body_size);
	R_recv_pre[TIDX] += packet_len;


	if( R_complete_start[TIDX] )
		R_complete_start[TIDX] 	= 0;

	return (1);
}





// ----------------------------------------------------------------------------
//
static inline int	R_pro_ProcMsg( const int TIDX, const int FD, const int status )
{
	int			error;
	struct header_special	packet_header;
	char			packet_body[SIZEOF_MAX_BODY+1];


	if( (error = R_pro_GetPacket(TIDX, &packet_header, packet_body)) < 0 )
		return FAIL;

	if( error == 0 )
	{
		if( status == CONN_STATUS_ABNORMAL )
			return FAIL;
		else
			return SUCC;
	}

	if( status == CONN_STATUS_ABNORMAL )
	{
		if( !D_GetSL_login(TIDX) )
		{
			dprintf(TIDX, "[PROC]Abnormal not process msg( not login ). command(%x) \n", packet_header.command );
			return FAIL;
		}

		if( packet_header.command != WA_CHAR_SAVE )
		{
			dprintf(TIDX, "[PROC]Abnormal not process msg( not proc msg ). command(%x) \n", packet_header.command );
			return SUCC;
		}
	}

	if( packet_header.command != WA_LIVE )
		dprintf(TIDX, "[PROC]<< %x >>\n", packet_header.command );


	switch( packet_header.command )
	{
		case	WA_INIT :
			if( R_pro_WA_INIT(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error R_pro_WA_INIT() \n" );
				return FAIL;
			}
			break;
		case	WA_LIVE :
			if( R_pro_WA_LIVE(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error R_pro_WA_LIVE() \n");
				return FAIL;
			}
			break;
		case	WA_LOGIN :
			if( R_pro_WA_LOGIN(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error R_pro_WA_LOGIN() \n");
				return FAIL;
			}
			break;
		case	WA_LOGOUT :
			if( R_pro_WA_LOGOUT(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error R_pro_WA_LOGOUT() \n");
				return FAIL;
			}
			break;
		case	WA_CHAR_LIST :
			if( R_pro_WA_CHAR_LIST(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error R_pro_WA_CHAR_LIST() \n");
				return FAIL;
			}
			break;
		case	WA_CHAR_CREATE :
			if( R_pro_WA_CHAR_CREATE(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error R_pro_WA_CHAR_CREATE() \n");
				return FAIL;
			}
			break;
		case	WA_CHAR_DELETE :
			if( R_pro_WA_CHAR_DELETE(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error R_pro_WA_CHAR_DELETE() \n");
				return FAIL;
			}
			break;
		case	WA_CHAR_LOAD :
			if( R_pro_WA_CHAR_LOAD(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error R_pro_WA_CHAR_LOAD() \n");
				return FAIL;
			}
			break;
		case	WA_CHAR_SAVE :
			if( R_pro_WA_CHAR_SAVE(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error R_pro_WA_CHAR_SAVE() \n");
				return FAIL;
			}
			break;
		case	WA_CURRENT_USER :
			if( R_pro_WA_CURRENT_USER(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error R_pro_WA_CURRENT_USER() \n");
				return FAIL;
			}
			break;
		case	WA_GAME_LOG :
			if( R_pro_WA_GAME_LOG(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error R_pro_WA_GAME_LOG() \n");
				return FAIL;
			}
			break;
		case	WA_GET_USER:
			if( R_pro_WA_GET_USER(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error R_pro_WA_GET_USER() \n");
				return FAIL;
			}
			break;
		default :
			dprintf(TIDX, "[PROC]ProcMsg() : Unknown Type(%x) \n", packet_header.command );
			return FAIL;

	}// end switch !!


	return SUCC;
}


static inline int 	R_pro_WA_INIT( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int				send_bodylen;
	struct body_WA_INIT 		*req_body;
	struct body_AW_INIT_RESULT 	res_body;


	// login check
	if( D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_INIT() : already login \n");
		return FAIL;
	}

	// body_size check()	
	if( header.body_size != sizeof(struct body_WA_INIT) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_INIT() : header's body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WA_INIT) );
		return FAIL;
	}

	req_body	= (struct body_WA_INIT *)pMsgBody;


	// version check
	if( !strncmp(req_body->version, VERSION, (VERSION_LEN + 1)) )
	{
		if( req_body->record_lock && D_Mysql_all_logout(TIDX, req_body->record_lock) )
		{
			dprintf(TIDX, "[RECV]Error R_pro_WA_INIT() : record_lock(%d) \n", req_body->record_lock );
			res_body.result_code	= RESULT_FAIL_DB;
		}
		else
			res_body.result_code	= 0;//SUCC
	}
	else
	{
		res_body.result_code	= RESULT_FAIL_VERSION;
	}


	send_bodylen   = sizeof(struct body_AW_INIT_RESULT);

	if( R_pro_InputHeader(TIDX, AW_INIT_RESULT, send_bodylen, header.fd, header.sec_active, header.session) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_InputHeader() : AW_INIT_RESULT \n");
		return FAIL;
	}

	R_pro_InputBody( TIDX, 0, send_bodylen, (char *)&res_body );


	if( res_body.result_code )
	{
		dprintf(TIDX, "[RECV]Error result_code(%d) \n", res_body.result_code );
		return FAIL;
	}
	

	D_SetSL_login( TIDX, 1 );


	return SUCC;
}


static inline int 	R_pro_WA_LIVE( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_LIVE() : not login \n");
		return FAIL;
	}

	D_SetSL_last_recvtime( TIDX );
	D_SetSL_live_send( TIDX, 0 );

	return SUCC;
}


static inline int 	R_pro_WA_LOGIN( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int				send_bodylen;
	struct body_WA_LOGIN		*req_body;
	struct body_AW_LOGIN_RESULT	res_body;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_LOGIN() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WA_LOGIN) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_LOGIN() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WA_LOGIN) );
		return FAIL;
	}

	req_body	= (struct body_WA_LOGIN *)pMsgBody;


	if( D_Mysql_WA_LOGIN(TIDX, req_body->user_id, req_body->encodedstr, req_body->record_lock, header.fd, header.sec_active
		, header.session, &(res_body.lock_fd), &(res_body.lock_sec_active), &(res_body.lock_session), &(res_body.record_lock)
		, &(res_body.result_code)) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_LOGIN() : D_Mysql_WA_LOGIN() \n" );
		return FAIL;
	}

	strncpy( res_body.user_id	, req_body->user_id	, (USER_ID_LEN + 1) );


	send_bodylen	= sizeof(struct body_AW_LOGIN_RESULT);

	if( R_pro_InputHeader(TIDX, AW_LOGIN_RESULT, send_bodylen, header.fd, header.sec_active, header.session) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_InputHeader() : AW_LOGIN_RESULT\n");
		return FAIL;
	}

	R_pro_InputBody( TIDX, 0, sizeof(struct body_AW_LOGIN_RESULT), (char *)&res_body );


	return SUCC;
}


static inline int 	R_pro_WA_LOGOUT( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int				result_code;
	struct body_WA_LOGOUT		*req_body;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_LOGOUT() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WA_LOGOUT) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_LOGOUT() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WA_LOGOUT) );
		return FAIL;
	}

	req_body	= (struct body_WA_LOGOUT *)pMsgBody;


	if( D_Mysql_WA_LOGOUT(TIDX, req_body->user_id, req_body->record_lock, header.fd, header.sec_active, header.session, &result_code) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_LOGOUT() : D_Mysql_WA_LOGOUT() \n" );
		return FAIL;
	}

	if( result_code )
		dprintf(TIDX, "** Error logout user_id(%s) \n", req_body->user_id );

	return SUCC;
}


static inline int 	R_pro_WA_CHAR_LIST( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int					n, send_bodylen;
	char					char_buf[SIZEOF_MAX_BODY + 1];
	struct body_WA_CHAR_LIST		*req_body;
	struct body1_AW_CHAR_LIST_RESULT	res_body1;
	struct body2_AW_CHAR_LIST_RESULT	*res_body2;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_CHAR_LIST() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WA_CHAR_LIST) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_CHAR_LIST() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WA_CHAR_LIST) );
		return FAIL;
	}

	req_body	= (struct body_WA_CHAR_LIST *)pMsgBody;


	res_body1.char_cnt 	= 0;
	res_body2		= (struct body2_AW_CHAR_LIST_RESULT *)char_buf;

	if( D_Mysql_WA_CHAR_LIST(TIDX, req_body->user_id, &(res_body1.char_cnt), res_body2, &(res_body1.result_code)) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_CHAR_LIST() : D_Mysql_WA_CHAR_LIST() \n" );
		return FAIL;
	}

	strncpy( res_body1.user_id	, req_body->user_id	, (USER_ID_LEN + 1) );


	send_bodylen	= sizeof(struct body1_AW_CHAR_LIST_RESULT) + ((res_body1.char_cnt) * sizeof(struct body2_AW_CHAR_LIST_RESULT));

	if( R_pro_InputHeader(TIDX, AW_CHAR_LIST_RESULT, send_bodylen, header.fd, header.sec_active, header.session) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_InputHeader() : AW_CHAR_LIST_RESULT\n");
		return FAIL;
	}

	n = R_pro_InputBody( TIDX, 0, sizeof(struct body1_AW_CHAR_LIST_RESULT), (char *)&res_body1 );
	    R_pro_InputBody( TIDX, n, (res_body1.char_cnt * sizeof(struct body2_AW_CHAR_LIST_RESULT)), (char *)res_body2 );


	return SUCC;
}


static inline int 	R_pro_WA_CHAR_CREATE( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int					send_bodylen;
	struct body_WA_CHAR_CREATE		*req_body;
	struct body_AW_CHAR_CREATE_RESULT	res_body;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_CHAR_CREATE() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WA_CHAR_CREATE) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_CHAR_CREATE() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WA_CHAR_CREATE) );
		return FAIL;
	}

	req_body	= (struct body_WA_CHAR_CREATE *)pMsgBody;


	if( D_Mysql_WA_CHAR_CREATE(TIDX, req_body, &(res_body.result_code)) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_CHAR_CREATE() : D_Mysql_WA_CHAR_CREATE() \n");
		return FAIL;
	}

	strncpy( res_body.user_id	, req_body->user_id	, (USER_ID_LEN + 1) );
	strncpy( res_body.char_name	, req_body->char_name	, (CHAR_NAME_LEN + 1) );


	send_bodylen	= sizeof(struct body_AW_CHAR_CREATE_RESULT);

	if( R_pro_InputHeader(TIDX, AW_CHAR_CREATE_RESULT, send_bodylen, header.fd, header.sec_active, header.session) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_InputHeader() : AW_CHAR_CREATE_RESULT\n");
		return FAIL;
	}

	R_pro_InputBody( TIDX, 0, send_bodylen, (char *)&res_body );


	return SUCC;
}


static inline int 	R_pro_WA_CHAR_DELETE( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int					send_bodylen;
	struct body_WA_CHAR_DELETE		*req_body;
	struct body_AW_CHAR_DELETE_RESULT	res_body;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_CHAR_DELETE() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WA_CHAR_DELETE) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_CHAR_DELETE() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WA_CHAR_DELETE) );
		return FAIL;
	}

	req_body	= (struct body_WA_CHAR_DELETE *)pMsgBody;


	if( D_Mysql_WA_CHAR_DELETE(TIDX, req_body->user_id, req_body->char_name, &(res_body.result_code)) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_CHAR_DELETE() : D_Mysql_WA_CHAR_DELETE() \n");
		return FAIL;
	}

	strncpy( res_body.user_id	, req_body->user_id	, (USER_ID_LEN + 1) );
	strncpy( res_body.char_name	, req_body->char_name	, (CHAR_NAME_LEN + 1) );


	send_bodylen	= sizeof(struct body_AW_CHAR_DELETE_RESULT);

	if( R_pro_InputHeader(TIDX, AW_CHAR_DELETE_RESULT, send_bodylen, header.fd, header.sec_active, header.session) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_InputHeader() : AW_CHAR_DELETE_RESULT\n");
		return FAIL;
	}

	R_pro_InputBody( TIDX, 0, send_bodylen, (char *)&res_body );


	return SUCC;
}


static inline int 	R_pro_WA_CHAR_LOAD( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int					send_bodylen;
	struct body_WA_CHAR_LOAD		*req_body;
	struct body1_AW_CHAR_LOAD_RESULT	*res_body1;
	struct body2_AW_CHAR_LOAD_RESULT	*res_body2;
	char					buf[SIZEOF_MAX_BODY + 1];


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_CHAR_LOAD() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WA_CHAR_LOAD) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_CHAR_LOAD() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WA_CHAR_LOAD) );
		return FAIL;
	}

	req_body	= (struct body_WA_CHAR_LOAD *)pMsgBody;


	res_body1	= (struct body1_AW_CHAR_LOAD_RESULT *)buf;
	res_body2	= (struct body2_AW_CHAR_LOAD_RESULT *)(buf + sizeof(struct body1_AW_CHAR_LOAD_RESULT));

	if( D_Mysql_WA_CHAR_LOAD(TIDX, req_body->user_id, req_body->char_name, res_body1, res_body2, &(res_body1->result_code)) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_CHAR_LOAD() : D_Mysql_WA_CHAR_LOAD() \n");
		return FAIL;
	}

	strncpy( res_body1->user_id	, req_body->user_id	, (USER_ID_LEN + 1) );
	strncpy( res_body1->char_name	, req_body->char_name	, (CHAR_NAME_LEN + 1) );


	send_bodylen	= sizeof(struct body1_AW_CHAR_LOAD_RESULT) 	
			+ (res_body1->item_slot_cnt * sizeof(struct body2_AW_CHAR_LOAD_RESULT));

	if( R_pro_InputHeader(TIDX, AW_CHAR_LOAD_RESULT, send_bodylen, header.fd, header.sec_active, header.session) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_InputHeader() : AW_CHAR_LOAD_RESULT\n");
		return FAIL;
	}

	R_pro_InputBody( TIDX, 0, send_bodylen, buf );


	return SUCC;
}


static inline int 	R_pro_WA_CHAR_SAVE( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int				result_code;
	struct body1_WA_CHAR_SAVE	*req_body1;
	struct body2_WA_CHAR_SAVE	*req_body2;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_CHAR_SAVE() : not login \n");
		return FAIL;
	}

	req_body1	= (struct body1_WA_CHAR_SAVE *)pMsgBody;
	req_body2	= (struct body2_WA_CHAR_SAVE *)(pMsgBody + sizeof(struct body1_WA_CHAR_SAVE));

	if( header.body_size != (sizeof(struct body1_WA_CHAR_SAVE) + (req_body1->item_slot_cnt * sizeof(struct body2_WA_CHAR_SAVE))) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_CHAR_SAVE() : body_size(%d/%d) \n"
			, header.body_size
			, (sizeof(struct body1_WA_CHAR_SAVE) + (req_body1->item_slot_cnt * sizeof(struct body2_WA_CHAR_SAVE))) );
		return FAIL;
	}


	if( D_Mysql_WA_CHAR_SAVE(TIDX, req_body1, req_body2, &result_code) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_CHAR_SAVE() : D_Mysql_WA_CHAR_SAVE() \n");
		return FAIL;
	}

	if( result_code )
		dprintf(TIDX, "** Error char_save user_id(%s), char_name(%s) \n", req_body1->user_id, req_body1->char_name);


	return SUCC;
}


static inline int 	R_pro_WA_CURRENT_USER( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int				result_code;
	struct body_WA_CURRENT_USER	*req_body;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_CURRENT_USER() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WA_CURRENT_USER) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_CURRENT_USER() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WA_CURRENT_USER) );
		return FAIL;
	}

	req_body	= (struct body_WA_CURRENT_USER *)pMsgBody;


	if( D_Mysql_WA_CURRENT_USER(TIDX, req_body, &result_code) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_CURRENT_USER() : D_Mysql_WA_CURRENT_USER() \n");
		return FAIL;
	}

	if( result_code )
		dprintf(TIDX, "** Error save current_user \n");


	return SUCC;
}


static inline int 	R_pro_WA_GAME_LOG( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int				result_code;
	struct body_WA_GAME_LOG		*req_body;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_GAME_LOG() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WA_GAME_LOG) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_GAME_LOG() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WA_GAME_LOG) );
		return FAIL;
	}

	req_body	= (struct body_WA_GAME_LOG *)pMsgBody;


	if( D_Mysql_WA_GAME_LOG(TIDX, req_body, &result_code) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_GAME_LOG() : D_Mysql_WA_GAME_LOG() \n");
		return FAIL;
	}

	if( result_code )
		dprintf(TIDX, "** Error save game_log \n");


	return SUCC;
}


static inline int 	R_pro_WA_GET_USER( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int				send_bodylen;
	struct body_WA_GET_USER		*req_body;
	struct body_AW_GET_USER_RESULT	res_body;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_GET_USER() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WA_GET_USER) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_GET_USER() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WA_GET_USER) );
		return FAIL;
	}

	req_body	= (struct body_WA_GET_USER *)pMsgBody;


	if( D_Mysql_WA_GET_USER(TIDX, req_body, &(res_body.user)) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_WA_GET_USER() : D_Mysql_WA_GET_USER() \n");
		return FAIL;
	}

	res_body.msvr_code	= req_body->msvr_code;
	res_body.ssvr_code	= req_body->ssvr_code;


	send_bodylen	= sizeof(struct body_AW_GET_USER_RESULT); 

	if( R_pro_InputHeader(TIDX, AW_GET_USER_RESULT, send_bodylen, header.fd, header.sec_active, header.session) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_InputHeader() : AW_GET_USER_RESULT \n");
		return FAIL;
	}

	R_pro_InputBody( TIDX, 0, send_bodylen, (char *)&res_body );


	return SUCC;
}
