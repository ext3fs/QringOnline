
#include	"global.h"
#include	"util.h"
#include	"db.h"
#include	"recv.h"



// ----------------------------------------------------------------------------
// recv	variable
static volatile int				R_Input_FDS		[MAX_RECVT_CNT];
static struct epoll_event 			R_event			[MAX_RECVT_CNT];

static volatile char				R_complete_start	[MAX_RECVT_CNT];
static volatile time_t				R_complete_sec		[MAX_RECVT_CNT];

static char					R_recv_buf		[MAX_RECVT_CNT][SIZEOF_RCVBUF];
static volatile int				R_recv_pre		[MAX_RECVT_CNT];
static volatile int				R_recv_pos		[MAX_RECVT_CNT];

static char					R_send_buf		[MAX_RECVT_CNT][SIZEOF_SNDBUF];
static volatile int				R_send_pre		[MAX_RECVT_CNT];
static volatile int				R_send_pos		[MAX_RECVT_CNT];







// ----------------------------------------------------------------------------
// recv	function
static inline void	R_Thread_Exit			( const int );

static inline int	R_pro_inner_recv_fd		( const int, const int );
static inline int	R_pro_INTER_CONN_FD		( const int, const int, const char * );
static inline int	R_pro_Close			( const int );

static inline int	R_pro_RecvMsg			( const int, const int );
static inline int	R_pro_ProcMsg			( const int, const int, const int );
static inline int	R_pro_SendMsg			( const int, const int );
static inline int	R_pro_RemainMsg			( const int, const int );

static inline int	R_pro_RecvRemainPos		( const int );
static inline int	R_pro_SendRemainPos		( const int );
static inline int	R_pro_InputHeader		( const int, const int, const short, const short, const int, const int );
static inline int	R_pro_InputBody			( const int, const int, const int, const char * );
static inline int	R_pro_GetPacket			( const int, struct header_special *, char * );

static inline int 	R_pro_WS_INIT			( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WS_MYINFO			( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WS_WORLD_INFO		( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WS_GAMESERV_INFO		( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WS_DBAGENT_INFO		( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WS_MOBSERV_INFO		( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WS_MAP_FILES		( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WS_MOB_FILES		( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WS_TYPE_MOB_FILES		( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WS_LEVEL_FILES		( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WS_TYPE_ITEM_FILES	( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WS_TYPE_DROP_FILES	( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WS_TYPE_WARP_FILES	( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WS_TYPE_STORE_FILES	( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WS_TYPE_SKILL_FILES	( const int, const int, const struct header_special, const char * );
static inline int 	R_pro_WS_TYPE_MOB_SKILL_FILES	( const int, const int, const struct header_special, const char * );




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
	if( epoll_ctl(R_Input_FDS[TIDX], EPOLL_CTL_ADD, G_inner_recv_fd[TIDX][1], &R_event[TIDX] ) < 0 )
	{
		dprintf(TIDX, "[RECV]Error epoll_ctl_add() G_inner_recv_fd : %s\n", strerror(errno) );
		R_Thread_Exit( TIDX );
		return NULL;
	}

	// mysql init()
	if( D_SLMysql_Init(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error D_SLMysql_Init()\n");
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
				wait_type	= (0);
			else
				wait_type       = (1000);
		}
		else
		{
			wait_type	= (-1);
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
					//dprintf(TIDX, "[RECV]Error R_pro_RecvMsg() \n");
					isError		= 1;
				}
			}

			// wrong fd
			else
			{
				dprintf(TIDX, "[RECV]Error epoll_wait() wrong fd(%d) \n", event_fd );

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
			dprintf(TIDX, "[RECV]Error R_pro_ProcMsg() \n");
			isError	= 1;
		}


		// Error check
		if( isConnect && isError )
		{
			R_pro_RemainMsg( TIDX, D_GetSL_fd(TIDX) );

			if( R_pro_Close(TIDX) )
				isExit	= 1;
			
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



// =============================================================================================
//
// 				Process Inner Msg
//
// =============================================================================================
static inline int	R_pro_inner_recv_fd( const int TIDX, const int FD )
{
	int	msg_type, msg_fd, n, send_msg_type;
	char	msg_ipaddress[SIZEOF_IP], send_data[1024];


	if( (n = recv(FD, &msg_type, SIZEOF_INT, 0)) != SIZEOF_INT )
	{
		dprintf(TIDX, "[RECV]Error R_pro_inner_recv_fd(): %s \n", strerror(errno) );
		return FAIL;
	}


	switch( msg_type )
	{
		case    INTER_CONN_FD:
			dprintf(TIDX, "[RECV]recv INTER_CONN_FD \n");

			send_msg_type = INTER_CONN_FD_SUCC;
			if( (n = recv(FD, &msg_fd, SIZEOF_INT, 0)) != SIZEOF_INT )
			{
				dprintf(TIDX, "[RECV]Error R_pro_inner_recv_fd(): %s \n", strerror(errno) );
				return FAIL;
			}

			if( (n = recv(FD, &msg_ipaddress, SIZEOF_IP, 0)) != SIZEOF_IP )
			{
				dprintf(TIDX, "[RECV]Error R_pro_inner_recv_fd(): %s \n", strerror(errno) );
				return FAIL;
			}

			if( R_pro_INTER_CONN_FD(TIDX, msg_fd, msg_ipaddress) )
				send_msg_type 	= INTER_CONN_FD_FAIL;

			memcpy( send_data	, &send_msg_type  , SIZEOF_INT );
			send( FD, &send_data, 4, 0 );
			return SUCC;

		case    INTER_EXIT_THREAD :
			dprintf(TIDX, "[RECV]recv INTER_EXIT_THREAD \n");
			return FAIL;

		default :
			dprintf(TIDX, "[RECV]Error R_pro_inner_recv_fd() : Unknown command(%x) \n", msg_type );
			return FAIL;
	}

	return SUCC;
}


static inline int	R_pro_INTER_CONN_FD( const int TIDX, const int fd, const char *ipaddress )
{

	// epoll add
	R_event[TIDX].events    = EPOLLIN|EPOLLERR|EPOLLHUP|EPOLLET;
	R_event[TIDX].data.fd   = fd;
	if( epoll_ctl(R_Input_FDS[TIDX], EPOLL_CTL_ADD, fd, &R_event[TIDX]) < 0 )
	{
	        dprintf(TIDX, "[RECV]Error epoll_ctl_add(%d) : %s\n", fd, strerror(errno) );
		close(fd);
		return FAIL;
	}

	// SL init()
	D_SetSL_fd( TIDX, fd );
	D_SetSL_ipaddress( TIDX, ipaddress );
	D_SetSL_login( TIDX, 0 );

	// buffer init()
	R_recv_pre[TIDX] = R_recv_pos[TIDX] = 0;
	R_send_pre[TIDX] = R_send_pos[TIDX] = 0;

	// last_recvtime()
	R_complete_start[TIDX] = 0;


	dprintf(TIDX, "[RECV]----------------------------------------\n");
	dprintf(TIDX, "[RECV]< New FD (%d) > \n", fd );

	return SUCC;
}


static inline int	R_pro_Close( const int TIDX )
{
	int	n, msg_type;
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
	if( (n = send(G_inner_recv_fd[TIDX][1], &msg_type, SIZEOF_INT, 0)) != SIZEOF_INT )
	{	
		dprintf(TIDX, "[RECV]Error R_pro_Close() send(%d) : %s \n", n, strerror(errno) );	
		return FAIL;
	}


	dprintf(TIDX, "[RECV]< Close FD (%d) > \n", close_fd);
	dprintf(TIDX, "[RECV]----------------------------------------\n");

	return SUCC;
}





// =============================================================================================
//
//			Procedure Tcp Connection - Main Function
//
// =============================================================================================

static inline int	R_pro_RecvMsg( const int TIDX, const int FD )
{
	int	n, move_len;


	if( R_pro_RecvRemainPos(TIDX) < SIZEOF_MAX_PACKET )
	{
		move_len = (R_recv_pos[TIDX] - R_recv_pre[TIDX]);
		memmove( R_recv_buf[TIDX], (R_recv_buf[TIDX] + R_recv_pre[TIDX]), move_len );

		R_recv_pre[TIDX]	= 0;
		R_recv_pos[TIDX]	= move_len;
	}


	n = recv( FD, (R_recv_buf[TIDX] + R_recv_pos[TIDX]), R_pro_RecvRemainPos(TIDX), 0 );

	if( n < 1 )
	{
		if( errno == 0 )
		{
			dprintf(TIDX, "[RECV]recv normal close FD (%d) \n", FD );
			return FAIL;
		}
		else
		{
			dprintf(TIDX, "[RECV]recv abnormal close FD (%d) : %s \n", FD, strerror(errno) );
			return FAIL;
		}
	}

	R_recv_pos[TIDX]	+= n;

	return SUCC;
}


static inline int	R_pro_SendMsg( const int TIDX, const int FD )
{
	int	n;


	if( (R_send_pos[TIDX] < 0) || (R_send_pos[TIDX] > (SIZEOF_SNDBUF - 1)) )
		return FAIL;

	if( R_send_pos[TIDX] == 0 )
		return SUCC;

	n = send( FD, (R_send_buf[TIDX] + R_send_pre[TIDX]), (R_send_pos[TIDX] - R_send_pre[TIDX]), 0 );

	if( (n < 0) || (n != R_send_pos[TIDX]) )
	{
		if( errno == 0 )
		{
			dprintf(TIDX, "[RECV]send normal close : %s \n", strerror(errno) );
			return FAIL;
		}
		else
		{
			dprintf(TIDX, "[RECV]send abnormal close : %s \n", strerror(errno) );
			return FAIL;
		}
	}

	R_send_pre[TIDX] = R_send_pos[TIDX] = 0;
	return SUCC;
}


static inline int	R_pro_RemainMsg( const int TIDX, const int FD )
{
	int	buff_len1, buff_len2;

	buff_len1 = buff_len2 = 0;

	while( (buff_len1 = (R_recv_pos[TIDX] - R_recv_pre[TIDX])) >= SIZEOF_HEADER )
	{
		if( R_pro_ProcMsg(TIDX, FD, CONN_STATUS_ABNORMAL) )
		{
			dprintf(TIDX, "[RECV]Error R_pro_RemainMsg() R_pro_ProcMsg() \n");
			break;
		}

		if( buff_len1 == buff_len2 )
		{
			break;
		}

		dprintf(TIDX, "[RECV]R_pro_RemainMsg() buff_len1(%d), buff_len2(%d) \n", buff_len1, buff_len2 );
		buff_len2	= buff_len1;
	}

	return SUCC;
}





// =============================================================================================
//
// 				Procedure Tcp Connection - Buffer 
//
// =============================================================================================

static inline int	R_pro_RecvRemainPos( const int TIDX )
{
	return 	(SIZEOF_RCVBUF - R_recv_pos[TIDX]);
}


static inline int	R_pro_SendRemainPos( const int TIDX )
{
	return 	(SIZEOF_SNDBUF - R_send_pos[TIDX]);
}


static inline int	R_pro_InputHeader( const int TIDX, const int command, const short body_size
		, const short fd, const int sec_active, const int session )
{
	struct header_special	send_header;
	int			packet_len = (SIZEOF_HEADER + body_size);


	if( packet_len > SIZEOF_MAX_PACKET )
	{
		dprintf(TIDX, "[RECV]Error R_pro_InputHeader(). packet length(%d) \n", packet_len );
		return FAIL;
	}

	else if( packet_len > R_pro_SendRemainPos(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_InputHeader(). packet_len(%d), remain(%d) \n", packet_len, R_pro_SendRemainPos(TIDX) );
		return FAIL;
	}

	send_header.command	= command;
	send_header.body_size	= body_size;
	send_header.fd		= fd;
	send_header.sec_active	= sec_active;
	send_header.session	= session;

	memcpy( (R_send_buf[TIDX] + R_send_pos[TIDX]), &send_header, SIZEOF_HEADER );
	R_send_pos[TIDX]	+= SIZEOF_HEADER;

	return SUCC;
}


static inline int	R_pro_InputBody( const int TIDX, const int pre_bodylen, const int add_bodylen, const char *add_data )
{
	if( (pre_bodylen < 0) || (add_bodylen < 0) || ((pre_bodylen+add_bodylen) > SIZEOF_MAX_BODY) )
	{
		dprintf(TIDX, "[RECV]Error R_pro_InputBody() : pre(%d), add(%d) \n", pre_bodylen, add_bodylen);
		return FAIL;
	}

	memcpy( R_send_buf[TIDX] + R_send_pos[TIDX], add_data, add_bodylen );
	R_send_pos[TIDX]	+= add_bodylen;

	return add_bodylen;
}


static int	R_pro_GetPacket( const int TIDX, struct header_special *pHeader, char *pMsgBody )
{
	unsigned long 	packet_len, flow_time;
	int		buff_len = (R_recv_pos[TIDX] - R_recv_pre[TIDX]);


	// check buffer - header
	if( buff_len < SIZEOF_HEADER )
		return (0);

	memcpy( pHeader, (R_recv_buf[TIDX] + R_recv_pre[TIDX]), SIZEOF_HEADER );
	buff_len	-= SIZEOF_HEADER;

	if( (pHeader->command & 0xff000000) != _WS )
	{
		dprintf(TIDX, "[RECV]Error R_pro_GetPacket() unknown command(%x) \n"
			, pHeader->command );
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
			R_complete_sec[TIDX]	= U_Get_time();
			R_complete_start[TIDX]	= 1;
		}

		flow_time = (U_Get_time() - R_complete_sec[TIDX]);

		if( flow_time > LIMIT_SEC_TO_PROC )
		{
			dprintf(TIDX, "[RECV]Error R_pro_GetPacket() over last proctime : header.command(%x) \n", pHeader->command);
			return (-2);
		}

		return (0);
	}

	memcpy( pMsgBody, (R_recv_buf[TIDX] + R_recv_pre[TIDX] + SIZEOF_HEADER), pHeader->body_size );


	// buffer point move
	packet_len	  = (SIZEOF_HEADER + pHeader->body_size);
	R_recv_pre[TIDX] += packet_len;


	if( R_complete_start[TIDX] )
		R_complete_start[TIDX]  = 0;

	return (1);
}





// =============================================================================================
//
//			Procedure Tcp Connection Message - Common Message
//
// =============================================================================================

static inline int	R_pro_ProcMsg( const int TIDX, const int FD, const int status )
{
	int				ret;
	struct header_special		packet_header;
	char				packet_body[SIZEOF_MAX_BODY+1];


	if( (ret = R_pro_GetPacket(TIDX, &packet_header, packet_body)) < 0 )
		return FAIL;

	if( ret == 0 )
	{
		if( status == CONN_STATUS_ABNORMAL )
			return FAIL;

		return SUCC;
	}


	//dprintf(TIDX, "[PROC] - << %x >> - (%d, %d) \n", packet_header.command, R_recv_pre[TIDX], R_recv_pos[TIDX] );


	if( status == CONN_STATUS_ABNORMAL )
	{
		if( !D_GetSL_login(TIDX) )
		{
			dprintf(TIDX, "[PROC]Abnormal not process msg( not login ). command(%x) \n", packet_header.command );
			return FAIL;
		}
	}


	switch( packet_header.command )
	{
		case	WS_INIT :
			if( R_pro_WS_INIT(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error pro_WS_INIT() \n" );
				return FAIL;
			}
			break;
		case	WS_MYINFO :
			if( R_pro_WS_MYINFO(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error pro_WS_MYINFO() \n" );
				return FAIL;
			}
			break;
		case	WS_WORLD_INFO :
			if( R_pro_WS_WORLD_INFO(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error pro_WS_WORLD_INFO() \n" );
				return FAIL;
			}
			break;
		case	WS_GAMESERV_INFO :
			if( R_pro_WS_GAMESERV_INFO(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error pro_WS_GAMESERV_INFO() \n" );
				return FAIL;
			}
			break;
		case	WS_DBAGENT_INFO :
			if( R_pro_WS_DBAGENT_INFO(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error pro_WS_DBAGENT_INFO() \n" );
				return FAIL;
			}
			break;
		case	WS_MOBSERV_INFO :
			if( R_pro_WS_MOBSERV_INFO(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error pro_WS_MOBSERV_INFO() \n" );
				return FAIL;
			}
			break;
		case	WS_MAP_FILES :
			if( R_pro_WS_MAP_FILES(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error pro_WS_MAP_FILES() \n" );
				return FAIL;
			}
			break;
		case	WS_MOB_FILES :
			if( R_pro_WS_MOB_FILES(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error pro_WS_MOB_FILES() \n" );
				return FAIL;
			}
			break;
		case	WS_TYPE_MOB_FILES :
			if( R_pro_WS_TYPE_MOB_FILES(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error pro_WS_TYPE_MOB_FILES() \n" );
				return FAIL;
			}
			break;
		case	WS_LEVEL_FILES :
			if( R_pro_WS_LEVEL_FILES(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error pro_WS_LEVEL_FILES() \n" );
				return FAIL;
			}
			break;
		case	WS_TYPE_ITEM_FILES :
			if( R_pro_WS_TYPE_ITEM_FILES(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error pro_WS_TYPE_ITEM_FILES() \n" );
				return FAIL;
			}
			break;
		case	WS_TYPE_DROP_FILES :
			if( R_pro_WS_TYPE_DROP_FILES(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error pro_WS_TYPE_DROP_FILES() \n" );
				return FAIL;
			}
			break;
		case	WS_TYPE_WARP_FILES :
			if( R_pro_WS_TYPE_WARP_FILES(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error pro_WS_TYPE_WARP_FILES() \n" );
				return FAIL;
			}
			break;
		case	WS_TYPE_STORE_FILES :
			if( R_pro_WS_TYPE_STORE_FILES(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error pro_WS_TYPE_STORE_FILES() \n" );
				return FAIL;
			}
			break;
		case	WS_TYPE_SKILL_FILES :
			if( R_pro_WS_TYPE_SKILL_FILES(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error pro_WS_TYPE_SKILL_FILES() \n" );
				return FAIL;
			}
			break;
		case	WS_TYPE_MOB_SKILL_FILES :
			if( R_pro_WS_TYPE_MOB_SKILL_FILES(TIDX, FD, packet_header, packet_body) )
			{
				dprintf(TIDX, "[PROC]Error pro_WS_TYPE_MOB_SKILL_FILES() \n" );
				return FAIL;
			}
			break;
		default :
			dprintf(TIDX, "[PROC]ProcMsg() : Unknown Type(%x) \n", packet_header.command );
			return FAIL;

	}// end switch !!


	return SUCC;
}


static inline int 	R_pro_WS_INIT( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int				ret, send_bodylen;
	struct body_WS_INIT 		*req_body;
	struct body_SW_INIT_RESULT 	res_body;


	// login check
	if( D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_INIT() : already login \n");
		return FAIL;
	}

	// body_size check()	
	if( header.body_size != sizeof(struct body_WS_INIT) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_INIT() : header's body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WS_INIT) );
		return FAIL;
	}

	req_body	= (struct body_WS_INIT *)pMsgBody;


	if( !strncmp(req_body->version, G_version, (VERSION_LEN + 1)) )
		res_body.result_code	= 0;//SUCC
	else
		res_body.result_code	= RESULT_FAIL_VERSION;


	send_bodylen   = sizeof(struct body_SW_INIT_RESULT);

	ret = R_pro_InputHeader( TIDX, SW_INIT_RESULT, send_bodylen, header.fd, header.sec_active, header.session );

	if( ret )
	{
		dprintf(TIDX, "[RECV]Error pro_InputHeader() : SW_INIT_RESULT \n");
		return FAIL;
	}

	R_pro_InputBody( TIDX, 0, send_bodylen, (char *)&res_body );


        if( R_pro_SendMsg(TIDX, FD) )
	{
                dprintf(TIDX, "[RECV]Error send() SW_INIT_RESULT \n");
                return FAIL;
        }

	if( res_body.result_code )
	{
		dprintf(TIDX, "[RECV]Error result_code(%d) \n", res_body.result_code );
		return FAIL;
	}
	

	D_SetSL_login( TIDX, 1 );


	dprintf(TIDX, "[RECV]complete R_pro_WS_INIT()\n");
	return SUCC;
}


static inline int 	R_pro_WS_MYINFO( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int				ret, send_bodylen;
	struct body_WS_MYINFO 		*req_body;
	struct body_SW_MYINFO_RESULT 	res_body;


	// login check
	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_MYINFO() : not login \n");
		return FAIL;
	}

	// body_size check()	
	if( header.body_size != sizeof(struct body_WS_MYINFO) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_MYINFO() : header's body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WS_MYINFO) );
		return FAIL;
	}

	req_body	= (struct body_WS_MYINFO *)pMsgBody;


	if( D_WS_MYINFO(TIDX, D_GetSL_ipaddress(TIDX), &(res_body.game), &(res_body.world)) )
	{
		dprintf(TIDX, "[RECV]Error D_WS_MYINFO() \n" );
		return FAIL;
	}


	send_bodylen   = sizeof(struct body_SW_MYINFO_RESULT);

	ret = R_pro_InputHeader( TIDX, SW_MYINFO_RESULT, send_bodylen, header.fd, header.sec_active, header.session );

	if( ret )
	{
		dprintf(TIDX, "[RECV]Error pro_InputHeader() : SW_MYINFO_RESULT \n");
		return FAIL;
	}

	R_pro_InputBody( TIDX, 0, send_bodylen, (char *)&res_body );


        if( R_pro_SendMsg(TIDX, FD) )
	{
                dprintf(TIDX, "[RECV]Error send() SW_MYINFO_RESULT \n");
                return FAIL;
        }

	
	dprintf(TIDX, "[RECV]complete R_pro_WS_MYINFO()\n");
	return SUCC;
}


static inline int 	R_pro_WS_WORLD_INFO( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int					n, ret, send_bodylen;
	struct body_WS_WORLD_INFO		*req_body;
	struct body1_SW_WORLD_INFO_RESULT	res_body1;
	struct body2_SW_WORLD_INFO_RESULT	*res_body2;
	char					buf[SIZEOF_MAX_PACKET + 1];


	// login check
	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_WORLD_INFO() : not login \n");
		return FAIL;
	}

	// body_size check()	
	if( header.body_size != sizeof(struct body_WS_WORLD_INFO) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_WORLD_INFO() : header's body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WS_WORLD_INFO) );
		return FAIL;
	}

	req_body	= (struct body_WS_WORLD_INFO *)pMsgBody;


	if( D_WS_WORLD_INFO(TIDX, req_body->game, &res_body1.info_cnt, buf) )
	{
		dprintf(TIDX, "[RECV]Error D_WS_WORLD_INFO() \n" );
		return FAIL;
	}

	res_body2	= (struct body2_SW_WORLD_INFO_RESULT *)buf;


	send_bodylen   	= sizeof(struct body1_SW_WORLD_INFO_RESULT)
			+ sizeof(struct body2_SW_WORLD_INFO_RESULT) * (res_body1.info_cnt);

	ret = R_pro_InputHeader( TIDX, SW_WORLD_INFO_RESULT, send_bodylen, header.fd, header.sec_active, header.session );

	if( ret )
	{
		dprintf(TIDX, "[RECV]Error pro_InputHeader() : SW_GAME_WORLD_INFO_RESULT \n");
		return FAIL;
	}

	n = R_pro_InputBody( TIDX, 0,  sizeof(struct body1_SW_WORLD_INFO_RESULT)				, (char *)&res_body1 );
	    R_pro_InputBody( TIDX, n, (sizeof(struct body2_SW_WORLD_INFO_RESULT) * (res_body1.info_cnt))	, (char *)res_body2 );


        if( R_pro_SendMsg(TIDX, FD) )
	{
                dprintf(TIDX, "[RECV]Error send() SW_WORLD_INFO_RESULT \n");
                return FAIL;
        }


	dprintf(TIDX, "[RECV]complete R_pro_WS_WORLD_INFO()\n");
	return SUCC;
}


static inline int 	R_pro_WS_GAMESERV_INFO( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int					n, ret, send_bodylen;
	struct body_WS_GAMESERV_INFO		*req_body;
	struct body1_SW_GAMESERV_INFO_RESULT	res_body1;
	struct body2_SW_GAMESERV_INFO_RESULT	*res_body2;
	char					buf[SIZEOF_MAX_PACKET + 1];


	// login check
	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_GAMESERV_INFO() : not login \n");
		return FAIL;
	}

	// body_size check()	
	if( header.body_size != sizeof(struct body_WS_GAMESERV_INFO) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_GAMESERV_INFO() : header's body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WS_GAMESERV_INFO) );
		return FAIL;
	}

	req_body	= (struct body_WS_GAMESERV_INFO *)pMsgBody;


	if( D_WS_GAMESERV_INFO(TIDX, req_body->game, req_body->world, &res_body1.info_cnt, buf) )
	{
		dprintf(TIDX, "[RECV]Error D_WS_GAMESERV_INFO() \n" );
		return FAIL;
	}

	res_body2	= (struct body2_SW_GAMESERV_INFO_RESULT *)buf;


	send_bodylen   	= sizeof(struct body1_SW_GAMESERV_INFO_RESULT)
			+ sizeof(struct body2_SW_GAMESERV_INFO_RESULT) * (res_body1.info_cnt);

	ret = R_pro_InputHeader( TIDX, SW_GAMESERV_INFO_RESULT, send_bodylen, header.fd, header.sec_active, header.session );

	if( ret )
	{
		dprintf(TIDX, "[RECV]Error pro_InputHeader() : SW_GAMESERV_INFO_RESULT \n");
		return FAIL;
	}

	n = R_pro_InputBody( TIDX, 0,  sizeof(struct body1_SW_GAMESERV_INFO_RESULT)				, (char *)&res_body1 );
	    R_pro_InputBody( TIDX, n, (sizeof(struct body2_SW_GAMESERV_INFO_RESULT) * (res_body1.info_cnt))	, (char *)res_body2 );


        if( R_pro_SendMsg(TIDX, FD) )
	{
                dprintf(TIDX, "[RECV]Error send() SW_GAMESERV_INFO_RESULT \n");
                return FAIL;
        }


	dprintf(TIDX, "[RECV]complete R_pro_WS_GAMESERV_INFO()\n");
	return SUCC;
}


static inline int 	R_pro_WS_DBAGENT_INFO( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int					n, ret, send_bodylen;
	struct body_WS_DBAGENT_INFO		*req_body;
	struct body1_SW_DBAGENT_INFO_RESULT	res_body1;
	struct body2_SW_DBAGENT_INFO_RESULT	*res_body2;
	char					buf[SIZEOF_MAX_PACKET + 1];


	// login check
	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_DBAGENT_INFO() : not login \n");
		return FAIL;
	}

	// body_size check()	
	if( header.body_size != sizeof(struct body_WS_DBAGENT_INFO) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_DBAGENT_INFO() : header's body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WS_DBAGENT_INFO) );
		return FAIL;
	}

	req_body	= (struct body_WS_DBAGENT_INFO *)pMsgBody;


	if( D_WS_DBAGENT_INFO(TIDX, req_body->game, req_body->world, &res_body1.info_cnt, buf) )
	{
		dprintf(TIDX, "[RECV]Error D_WS_DBAGENT_INFO() \n" );
		return FAIL;
	}

	res_body2	= (struct body2_SW_DBAGENT_INFO_RESULT *)buf;


	send_bodylen   	= sizeof(struct body1_SW_DBAGENT_INFO_RESULT)
			+ sizeof(struct body2_SW_DBAGENT_INFO_RESULT) * (res_body1.info_cnt);

	ret = R_pro_InputHeader( TIDX, SW_DBAGENT_INFO_RESULT, send_bodylen, header.fd, header.sec_active, header.session );

	if( ret )
	{
		dprintf(TIDX, "[RECV]Error pro_InputHeader() : SW_GAME_DBAGENT_INFO_RESULT \n");
		return FAIL;
	}

	n = R_pro_InputBody( TIDX, 0,  sizeof(struct body1_SW_DBAGENT_INFO_RESULT)				, (char *)&res_body1 );
	    R_pro_InputBody( TIDX, n, (sizeof(struct body2_SW_DBAGENT_INFO_RESULT) * (res_body1.info_cnt))	, (char *)res_body2 );


        if( R_pro_SendMsg(TIDX, FD) )
	{
                dprintf(TIDX, "[RECV]Error send() SW_DBAGENT_INFO_RESULT \n");
                return FAIL;
        }

	dprintf(TIDX, "[RECV]complete R_pro_WS_DBAGENT_INFO()\n");
	return SUCC;
}


static inline int 	R_pro_WS_MOBSERV_INFO( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int					n, ret, send_bodylen;
	struct body_WS_MOBSERV_INFO		*req_body;
	struct body1_SW_MOBSERV_INFO_RESULT	res_body1;
	struct body2_SW_MOBSERV_INFO_RESULT	*res_body2;
	char					buf[SIZEOF_MAX_PACKET + 1];


	// login check
	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_MOBSERV_INFO() : not login \n");
		return FAIL;
	}

	// body_size check()	
	if( header.body_size != sizeof(struct body_WS_MOBSERV_INFO) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_MOBSERV_INFO() : header's body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WS_MOBSERV_INFO) );
		return FAIL;
	}

	req_body	= (struct body_WS_MOBSERV_INFO *)pMsgBody;


	if( D_WS_MOBSERV_INFO(TIDX, req_body->game, req_body->world, &res_body1.info_cnt, buf) )
	{
		dprintf(TIDX, "[RECV]Error D_WS_MOBSERV_INFO() \n" );
		return FAIL;
	}

	res_body2	= (struct body2_SW_MOBSERV_INFO_RESULT *)buf;


	send_bodylen   	= sizeof(struct body1_SW_MOBSERV_INFO_RESULT)
			+ sizeof(struct body2_SW_MOBSERV_INFO_RESULT) * (res_body1.info_cnt);

	ret = R_pro_InputHeader( TIDX, SW_MOBSERV_INFO_RESULT, send_bodylen, header.fd, header.sec_active, header.session );

	if( ret )
	{
		dprintf(TIDX, "[RECV]Error pro_InputHeader() : SW_GAME_MOBSERV_INFO_RESULT \n");
		return FAIL;
	}

	n = R_pro_InputBody( TIDX, 0,  sizeof(struct body1_SW_MOBSERV_INFO_RESULT)				, (char *)&res_body1 );
	    R_pro_InputBody( TIDX, n, (sizeof(struct body2_SW_MOBSERV_INFO_RESULT) * (res_body1.info_cnt))	, (char *)res_body2 );


        if( R_pro_SendMsg(TIDX, FD) )
	{
                dprintf(TIDX, "[RECV]Error send() SW_MOBSERV_INFO_RESULT \n");
                return FAIL;
        }

	dprintf(TIDX, "[RECV]complete R_pro_WS_MOBSERV_INFO()\n");
	return SUCC;
}


static inline int 	R_pro_WS_MAP_FILES( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int					i, ret, send_bodylen;
	struct body1_SW_MAP_FILES_RESULT	res_body1;
	struct body2_SW_MAP_FILES_RESULT	res_body2;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_MAP_FILES() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WS_MAP_FILES) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_MAP_FILES() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WS_MAP_FILES) );
		return FAIL;
	}

	res_body1.file_cnt	= G_MapFileCnt;


	send_bodylen = sizeof(struct body1_SW_MAP_FILES_RESULT);

	ret = R_pro_InputHeader( TIDX, SW_MAP_FILES_RESULT, send_bodylen, header.fd, header.sec_active, header.session );

	if( ret )
	{
		dprintf(TIDX, "[RECV]Error pro_InputHeader() : SW_MAP_FILES_RESULT \n");
		return FAIL;
	}

	R_pro_InputBody( TIDX, 0, send_bodylen, (char *)&res_body1 );


	if( R_pro_SendMsg(TIDX, FD) )
	{
		dprintf(TIDX, "[RECV]Error send() SW_MAP_FILES_RESULT \n");
		return FAIL;
	}

	for( i=0; i<G_MapFileCnt; i++ )
	{
		res_body2.file_size = G_MapList[i].size;

		if( (ret = send(FD, &res_body2, sizeof(struct body2_SW_MAP_FILES_RESULT), 0)) 
			!= sizeof(struct body2_SW_MAP_FILES_RESULT) )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_MAP_FILES() : ret(%d) \n", ret );
			return FAIL;
		}

		if( (ret = send(FD, G_MapList[i].data, res_body2.file_size, 0)) != res_body2.file_size )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_MAP_FILES() : ret(%d) \n", ret );
			return FAIL;
		}
	}


	dprintf(TIDX, "[RECV]complete R_pro_WS_MAP_FILES()\n");
	return SUCC;
}


static inline int 	R_pro_WS_MOB_FILES( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int					i, ret, send_bodylen;
	struct body1_SW_MOB_FILES_RESULT	res_body1;
	struct body2_SW_MOB_FILES_RESULT	res_body2;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_MOB_FILES() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WS_MOB_FILES) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_MOB_FILES() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WS_MOB_FILES) );
		return FAIL;
	}

	res_body1.file_cnt	= G_MapFileCnt;


	send_bodylen = sizeof(struct body1_SW_MOB_FILES_RESULT);

	ret = R_pro_InputHeader( TIDX, SW_MOB_FILES_RESULT, send_bodylen, header.fd, header.sec_active, header.session );

	if( ret )
	{
		dprintf(TIDX, "[RECV]Error pro_InputHeader() : SW_MOB_FILES_RESULT \n");
		return FAIL;
	}

	R_pro_InputBody( TIDX, 0, send_bodylen, (char *)&res_body1 );


	if( R_pro_SendMsg(TIDX, FD) )
	{
		dprintf(TIDX, "[RECV]Error send() SW_MOB_FILES_RESULT \n");
		return FAIL;
	}


	for( i=0; i<G_MapFileCnt; i++ )
	{
		res_body2.file_size = G_MobList[i].size;

		if( (ret = send(FD, &res_body2, sizeof(struct body2_SW_MOB_FILES_RESULT), 0)) 
			!= sizeof(struct body2_SW_MOB_FILES_RESULT) )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_MOB_FILES() : ret(%d) \n", ret );
			return FAIL;
		}

		if( (ret = send(FD, G_MobList[i].data, res_body2.file_size, 0)) != res_body2.file_size )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_MOB_FILES() : ret(%d) \n", ret );
			return FAIL;
		}
	}


	dprintf(TIDX, "[RECV]complete R_pro_WS_MOB_FILES()\n");
	return SUCC;
}


static inline int 	R_pro_WS_TYPE_MOB_FILES( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int					i, ret, send_bodylen;
	struct body1_SW_TYPE_MOB_FILES_RESULT	res_body1;
	struct body2_SW_TYPE_MOB_FILES_RESULT	res_body2;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_TYPE_MOB_FILES() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WS_TYPE_MOB_FILES) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_TYPE_MOB_FILES() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WS_TYPE_MOB_FILES) );
		return FAIL;
	}

	res_body1.file_cnt	= G_TypeMobFileCnt;


	send_bodylen = sizeof(struct body1_SW_TYPE_MOB_FILES_RESULT);

	ret = R_pro_InputHeader( TIDX, SW_TYPE_MOB_FILES_RESULT, send_bodylen, header.fd, header.sec_active, header.session );

	if( ret )
	{
		dprintf(TIDX, "[RECV]Error pro_InputHeader() : SW_TYPE_MOB_FILES_RESULT \n");
		return FAIL;
	}

	R_pro_InputBody( TIDX, 0, send_bodylen, (char *)&res_body1 );


	if( R_pro_SendMsg(TIDX, FD) )
	{
		dprintf(TIDX, "[RECV]Error send() SW_TYPE_MOB_FILES_RESULT \n");
		return FAIL;
	}


	for( i=0; i<G_TypeMobFileCnt; i++ )
	{
		res_body2.file_size = G_MobTypeList[i].size;

		if( (ret = send(FD, &res_body2, sizeof(struct body2_SW_TYPE_MOB_FILES_RESULT), 0)) 
			!= sizeof(struct body2_SW_TYPE_MOB_FILES_RESULT) )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_TYPE_MOB_FILES() : ret(%d) \n", ret );
			return FAIL;
		}

		if( (ret = send(FD, G_MobTypeList[i].data, res_body2.file_size, 0)) 
			!= res_body2.file_size )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_TYPE_MOB_FILES() : ret(%d) \n", ret );
			return FAIL;
		}
	}


	dprintf(TIDX, "[RECV]complete R_pro_WS_TYPE_MOB_FILES()\n");
	return SUCC;
}


static inline int 	R_pro_WS_LEVEL_FILES( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int					i, ret, send_bodylen;
	struct body1_SW_LEVEL_FILES_RESULT	res_body1;
	struct body2_SW_LEVEL_FILES_RESULT	res_body2;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_LEVEL_FILES() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WS_LEVEL_FILES) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_LEVEL_FILES() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WS_LEVEL_FILES) );
		return FAIL;
	}

	res_body1.file_cnt	= G_LevelFileCnt;


	send_bodylen = sizeof(struct body1_SW_LEVEL_FILES_RESULT);

	ret = R_pro_InputHeader( TIDX, SW_LEVEL_FILES_RESULT, send_bodylen, header.fd, header.sec_active, header.session );

	if( ret )
	{
		dprintf(TIDX, "[RECV]Error pro_InputHeader() : SW_LEVEL_FILES_RESULT \n");
		return FAIL;
	}

	R_pro_InputBody( TIDX, 0, send_bodylen, (char *)&res_body1 );


	if( R_pro_SendMsg(TIDX, FD) )
	{
		dprintf(TIDX, "[RECV]Error send() SW_LEVEL_FILES_RESULT \n");
		return FAIL;
	}


	for( i=0; i<G_LevelFileCnt; i++ )
	{
		res_body2.file_size = G_LevelList[i].size;

		if( (ret = send(FD, &res_body2, sizeof(struct body2_SW_LEVEL_FILES_RESULT), 0)) 
			!= sizeof(struct body2_SW_LEVEL_FILES_RESULT) )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_LEVEL_FILES() : ret(%d) \n", ret );
			return FAIL;
		}

		if( (ret = send(FD, G_LevelList[i].data, res_body2.file_size, 0)) 
			!= res_body2.file_size )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_LEVEL_FILES() : ret(%d) \n", ret );
			return FAIL;
		}
	}


	dprintf(TIDX, "[RECV]complete R_pro_WS_LEVEL_FILES()\n");
	return SUCC;
}


static inline int 	R_pro_WS_TYPE_ITEM_FILES( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int					i, ret, send_bodylen;
	struct body1_SW_TYPE_ITEM_FILES_RESULT	res_body1;
	struct body2_SW_TYPE_ITEM_FILES_RESULT	res_body2;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_TYPE_ITEM_FILES() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WS_TYPE_ITEM_FILES) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_TYPE_ITEM_FILES() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WS_TYPE_ITEM_FILES) );
		return FAIL;
	}

	res_body1.file_cnt	= G_ItemFileCnt;


	send_bodylen = sizeof(struct body1_SW_TYPE_ITEM_FILES_RESULT);

	ret = R_pro_InputHeader( TIDX, SW_TYPE_ITEM_FILES_RESULT, send_bodylen, header.fd, header.sec_active, header.session );

	if( ret )
	{
		dprintf(TIDX, "[RECV]Error pro_InputHeader() : SW_TYPE_ITEM_FILES_RESULT \n");
		return FAIL;
	}

	R_pro_InputBody( TIDX, 0, send_bodylen, (char *)&res_body1 );


	if( R_pro_SendMsg(TIDX, FD) )
	{
		dprintf(TIDX, "[RECV]Error send() SW_TYPE_ITEM_FILES_RESULT \n");
		return FAIL;
	}


	for( i=0; i<G_ItemFileCnt; i++ )
	{
		res_body2.file_size = G_ItemList[i].size;

		if( (ret = send(FD, &res_body2, sizeof(struct body2_SW_TYPE_ITEM_FILES_RESULT), 0)) 
			!= sizeof(struct body2_SW_TYPE_ITEM_FILES_RESULT) )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_TYPE_ITEM_FILES() : ret(%d) \n", ret );
			return FAIL;
		}

		if( (ret = send(FD, G_ItemList[i].data, res_body2.file_size, 0)) 
			!= res_body2.file_size )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_TYPE_ITEM_FILES() : ret(%d) \n", ret );
			return FAIL;
		}
	}


	dprintf(TIDX, "[RECV]complete R_pro_WS_TYPE_ITEM_FILES()\n");
	return SUCC;
}


static inline int 	R_pro_WS_TYPE_DROP_FILES( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int					i, ret, send_bodylen;
	struct body1_SW_TYPE_DROP_FILES_RESULT	res_body1;
	struct body2_SW_TYPE_DROP_FILES_RESULT	res_body2;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_TYPE_DROP_FILES() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WS_TYPE_DROP_FILES) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_TYPE_DROP_FILES() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WS_TYPE_DROP_FILES) );
		return FAIL;
	}

	res_body1.file_cnt	= G_DropFileCnt;


	send_bodylen = sizeof(struct body1_SW_TYPE_DROP_FILES_RESULT);

	ret = R_pro_InputHeader( TIDX, SW_TYPE_DROP_FILES_RESULT, send_bodylen, header.fd, header.sec_active, header.session );

	if( ret )
	{
		dprintf(TIDX, "[RECV]Error pro_InputHeader() : SW_TYPE_DROP_FILES_RESULT \n");
		return FAIL;
	}

	R_pro_InputBody( TIDX, 0, send_bodylen, (char *)&res_body1 );


	if( R_pro_SendMsg(TIDX, FD) )
	{
		dprintf(TIDX, "[RECV]Error send() SW_TYPE_DROP_FILES_RESULT \n");
		return FAIL;
	}


	for( i=0; i<G_DropFileCnt; i++ )
	{
		res_body2.file_size = G_DropList[i].size;

		if( (ret = send(FD, &res_body2, sizeof(struct body2_SW_TYPE_DROP_FILES_RESULT), 0)) 
			!= sizeof(struct body2_SW_TYPE_DROP_FILES_RESULT) )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_TYPE_DROP_FILES() : ret(%d) \n", ret );
			return FAIL;
		}

		if( (ret = send(FD, G_DropList[i].data, res_body2.file_size, 0)) 
			!= res_body2.file_size )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_TYPE_DROP_FILES() : ret(%d) \n", ret );
			return FAIL;
		}
	}


	dprintf(TIDX, "[RECV]complete R_pro_WS_TYPE_DROP_FILES()\n");
	return SUCC;
}


static inline int 	R_pro_WS_TYPE_WARP_FILES( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int					i, ret, send_bodylen;
	struct body1_SW_TYPE_WARP_FILES_RESULT	res_body1;
	struct body2_SW_TYPE_WARP_FILES_RESULT	res_body2;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_TYPE_WARP_FILES() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WS_TYPE_WARP_FILES) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_TYPE_WARP_FILES() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WS_TYPE_WARP_FILES) );
		return FAIL;
	}

	res_body1.file_cnt	= G_WarpFileCnt;


	send_bodylen = sizeof(struct body1_SW_TYPE_WARP_FILES_RESULT);

	ret = R_pro_InputHeader( TIDX, SW_TYPE_WARP_FILES_RESULT, send_bodylen, header.fd, header.sec_active, header.session );

	if( ret )
	{
		dprintf(TIDX, "[RECV]Error pro_InputHeader() : SW_TYPE_WARP_FILES_RESULT \n");
		return FAIL;
	}

	R_pro_InputBody( TIDX, 0, send_bodylen, (char *)&res_body1 );


	if( R_pro_SendMsg(TIDX, FD) )
	{
		dprintf(TIDX, "[RECV]Error send() SW_TYPE_WARP_FILES_RESULT \n");
		return FAIL;
	}


	for( i=0; i<G_WarpFileCnt; i++ )
	{
		res_body2.file_size = G_WarpList[i].size;

		if( (ret = send(FD, &res_body2, sizeof(struct body2_SW_TYPE_WARP_FILES_RESULT), 0)) 
			!= sizeof(struct body2_SW_TYPE_WARP_FILES_RESULT) )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_TYPE_WARP_FILES() : ret(%d) \n", ret );
			return FAIL;
		}

		if( (ret = send(FD, G_WarpList[i].data, res_body2.file_size, 0)) 
			!= res_body2.file_size )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_TYPE_WARP_FILES() : ret(%d) \n", ret );
			return FAIL;
		}
	}


	dprintf(TIDX, "[RECV]complete R_pro_WS_TYPE_WARP_FILES()\n");
	return SUCC;
}


static inline int 	R_pro_WS_TYPE_STORE_FILES( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int					i, ret, send_bodylen;
	struct body1_SW_TYPE_STORE_FILES_RESULT	res_body1;
	struct body2_SW_TYPE_STORE_FILES_RESULT	res_body2;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_TYPE_STORE_FILES() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WS_TYPE_STORE_FILES) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_TYPE_STORE_FILES() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WS_TYPE_STORE_FILES) );
		return FAIL;
	}

	res_body1.file_cnt	= G_StoreFileCnt;


	send_bodylen = sizeof(struct body1_SW_TYPE_STORE_FILES_RESULT);

	ret = R_pro_InputHeader( TIDX, SW_TYPE_STORE_FILES_RESULT, send_bodylen, header.fd, header.sec_active, header.session );

	if( ret )
	{
		dprintf(TIDX, "[RECV]Error pro_InputHeader() : SW_TYPE_STORE_FILES_RESULT \n");
		return FAIL;
	}

	R_pro_InputBody( TIDX, 0, send_bodylen, (char *)&res_body1 );


	if( R_pro_SendMsg(TIDX, FD) )
	{
		dprintf(TIDX, "[RECV]Error send() SW_TYPE_STORE_FILES_RESULT \n");
		return FAIL;
	}


	for( i=0; i<G_StoreFileCnt; i++ )
	{
		res_body2.file_size = G_StoreList[i].size;

		if( (ret = send(FD, &res_body2, sizeof(struct body2_SW_TYPE_STORE_FILES_RESULT), 0)) 
			!= sizeof(struct body2_SW_TYPE_STORE_FILES_RESULT) )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_TYPE_STORE_FILES() : ret(%d) \n", ret );
			return FAIL;
		}

		if( (ret = send(FD, G_StoreList[i].data, res_body2.file_size, 0)) 
			!= res_body2.file_size )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_TYPE_STORE_FILES() : ret(%d) \n", ret );
			return FAIL;
		}
	}


	dprintf(TIDX, "[RECV]complete R_pro_WS_TYPE_STORE_FILES()\n");
	return SUCC;
}


static inline int 	R_pro_WS_TYPE_SKILL_FILES( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int					i, ret, send_bodylen;
	struct body1_SW_TYPE_SKILL_FILES_RESULT	res_body1;
	struct body2_SW_TYPE_SKILL_FILES_RESULT	res_body2;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_TYPE_SKILL_FILES() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WS_TYPE_SKILL_FILES) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_TYPE_SKILL_FILES() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WS_TYPE_SKILL_FILES) );
		return FAIL;
	}

	res_body1.file_cnt	= G_CharSkillFileCnt;


	send_bodylen = sizeof(struct body1_SW_TYPE_SKILL_FILES_RESULT);

	ret = R_pro_InputHeader( TIDX, SW_TYPE_SKILL_FILES_RESULT, send_bodylen, header.fd, header.sec_active, header.session );

	if( ret )
	{
		dprintf(TIDX, "[RECV]Error pro_InputHeader() : SW_TYPE_SKILL_FILES_RESULT \n");
		return FAIL;
	}

	R_pro_InputBody( TIDX, 0, send_bodylen, (char *)&res_body1 );


	if( R_pro_SendMsg(TIDX, FD) )
	{
		dprintf(TIDX, "[RECV]Error send() SW_TYPE_SKILL_FILES_RESULT \n");
		return FAIL;
	}


	for( i=0; i<G_CharSkillFileCnt; i++ )
	{
		res_body2.file_size = G_CharSkillList[i].size;

		if( (ret = send(FD, &res_body2, sizeof(struct body2_SW_TYPE_SKILL_FILES_RESULT), 0)) 
			!= sizeof(struct body2_SW_TYPE_SKILL_FILES_RESULT) )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_TYPE_SKILL_FILES() : ret(%d) \n", ret );
			return FAIL;
		}

		if( (ret = send(FD, G_CharSkillList[i].data, res_body2.file_size, 0)) 
			!= res_body2.file_size )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_TYPE_SKILL_FILES() : ret(%d) \n", ret );
			return FAIL;
		}
	}


	dprintf(TIDX, "[RECV]complete R_pro_WS_TYPE_SKILL_FILES()\n");
	return SUCC;
}


static inline int 	R_pro_WS_TYPE_MOB_SKILL_FILES( const int TIDX, const int FD, const struct header_special header, const char *pMsgBody )
{
	int						i, ret, send_bodylen;
	struct body1_SW_TYPE_MOB_SKILL_FILES_RESULT	res_body1;
	struct body2_SW_TYPE_MOB_SKILL_FILES_RESULT	res_body2;


	if( !D_GetSL_login(TIDX) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_TYPE_MOB_SKILL_FILES() : not login \n");
		return FAIL;
	}

	if( header.body_size != sizeof(struct body_WS_TYPE_MOB_SKILL_FILES) )
	{
		dprintf(TIDX, "[RECV]Error pro_WS_TYPE_MOB_SKILL_FILES() : body_size(%d/%d) \n"
			, header.body_size, sizeof(struct body_WS_TYPE_MOB_SKILL_FILES) );
		return FAIL;
	}

	res_body1.file_cnt	= G_MobSkillFileCnt;


	send_bodylen = sizeof(struct body1_SW_TYPE_MOB_SKILL_FILES_RESULT);

	ret = R_pro_InputHeader( TIDX, SW_TYPE_MOB_SKILL_FILES_RESULT, send_bodylen, header.fd, header.sec_active, header.session );

	if( ret )
	{
		dprintf(TIDX, "[RECV]Error pro_InputHeader() : SW_TYPE_MOB_SKILL_FILES_RESULT \n");
		return FAIL;
	}

	R_pro_InputBody( TIDX, 0, send_bodylen, (char *)&res_body1 );


	if( R_pro_SendMsg(TIDX, FD) )
	{
		dprintf(TIDX, "[RECV]Error send() SW_TYPE_MOB_SKILL_FILES_RESULT \n");
		return FAIL;
	}


	for( i=0; i<G_MobSkillFileCnt; i++ )
	{
		res_body2.file_size = G_MobSkillList[i].size;

		if( (ret = send(FD, &res_body2, sizeof(struct body2_SW_TYPE_MOB_SKILL_FILES_RESULT), 0)) 
			!= sizeof(struct body2_SW_TYPE_MOB_SKILL_FILES_RESULT) )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_TYPE_MOB_SKILL_FILES() : ret(%d) \n", ret );
			return FAIL;
		}

		if( (ret = send(FD, G_MobSkillList[i].data, res_body2.file_size, 0)) 
			!= res_body2.file_size )
		{
			dprintf(TIDX, "[RECV]Error pro_WS_TYPE_MOB_SKILL_FILES() : ret(%d) \n", ret );
			return FAIL;
		}
	}


	dprintf(TIDX, "[RECV]complete R_pro_WS_TYPE_MOB_SKILL_FILES()\n");
	return SUCC;
}
