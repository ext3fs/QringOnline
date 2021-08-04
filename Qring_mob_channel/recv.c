
#include	"global.h"
#include	"util.h"
#include	"db.h"
#include	"send.h"
#include	"recv.h"


struct channel
{
	struct connection		connect;

	volatile char			complete_start;
	volatile time_t			complete_sec;

	volatile char			send_live;
	volatile time_t			sec_live;

	char				recv_buf[SIZEOF_RCVBUF];
	volatile int			recv_pos;

};


// ----------------------------------------------------------------------------
// recv	variable
static struct channel			*R_channel;
static struct epoll_event		R_event;





// ----------------------------------------------------------------------------
// recv	function
static inline int	R_inner_close_msg	( const int );
static inline int	R_inner_send_msg	( const int );
static inline int	R_inner_check_msg	( const int );

static inline int	R_channel_connect	( const int );
static inline void	R_channel_close		( const int );
static inline int	R_channel_live		( const int );
static inline int	R_channel_recv		( const int );
static inline int	R_channel_recv_queue_in	( const int );





void	*R_pro_main( void *arg )
{

	int			i, nEvent, tmp_fd, isExit = 0;
	struct connection	*tmp_fds;
	struct epoll_event	ev[1024];


	dprintf(0, "[RECV]Thread Start !! \n");


	while( !isExit )
	{

		// epoll_wait()
		if( (nEvent = epoll_wait(G_Input_fds, ev, 1024, -1)) < 0 )
		{
			dprintf(0, "[RECV]Error epoll_wait() : %s \n", strerror(errno) ), sleep(1);
			continue;
		}

		// event proc()
		for( i=0; i<nEvent; i++ )
		{
			tmp_fd	= ev[i].data.fd;

			if( tmp_fd == G_inner_close_fd[1] )
			{
				if( R_inner_close_msg(tmp_fd) )
				{
					dprintf(0, "[RECV]R_inner_close_msg() \n"), isExit = 1;
					break;
				}

				continue;
			}

			if( tmp_fd == G_inner_send_fd[1] )
			{
				if( R_inner_send_msg(tmp_fd) )
				{
					dprintf(0, "[RECV]R_inner_send_msg() \n"), isExit = 1;	
					break;
				}

				continue;
			}

			if( tmp_fd == G_inner_check_fd[1] )
			{
				if( R_inner_check_msg(tmp_fd) )
				{
					dprintf(0, "[RECV]R_inner_check_msg() \n"), isExit = 1;	
					break;
				}

				continue;
			}


			tmp_fds	= (struct connection *)(*(&(ev[i].data.ptr)+1));
		
			if( (tmp_fds->channel < 1) || (tmp_fds->channel > TOTAL_CHANNEL_CNT) )
			{
				dprintf(0, "[RECV]Error epoll_event channel(%d) \n", tmp_fds->channel ), isExit = 1;
				break;
			}

			if( R_channel_recv(tmp_fds->channel) )
			{
				dprintf(0, "[RECV]Error R_channel_recv(%d) \n", tmp_fds->channel );
				R_channel_close( tmp_fds->channel ); 
			}

		}// end event_proc for() !!

	}// end while() !!
	

	R_channel_release();

	dprintf(0, "[RECV]Thread End !! \n");

	return (NULL);
}


int	R_channel_init( void )
{
	int	i;


	if( (R_channel = calloc((TOTAL_CHANNEL_CNT + 1), sizeof(struct channel))) == NULL )
	{
		dprintf(0, "[RECV]Error R_channel_init() calloc() : %s \n", strerror(errno) );
		return (-1);
	}

	for( i=1; i<=TOTAL_CHANNEL_CNT; i++ )
	{
		R_channel[i].connect.channel	= i;
		R_channel[i].connect.fd		= 0;

		R_channel[i].complete_start	= 0;
		R_channel[i].complete_sec	= 0;

		R_channel[i].send_live		= 0;
		R_channel[i].sec_live		= 0;

		R_channel[i].recv_pos		= 0;
	}

	return (0);
}


void	R_channel_release( void )
{
	int	i;

	for( i=1; i<=TOTAL_CHANNEL_CNT; i++ )
		U_recv_queue_in( G_channel_proc_thr[i], i, INTER_EXIT_THREAD, 0, NULL );

	for( i=1; i<=TOTAL_CHANNEL_CNT; i++ )
		if( R_channel[i].connect.fd )
			R_channel_close(i);

	close(G_inner_close_fd[0]);
	close(G_inner_close_fd[1]);
	close(G_inner_send_fd[0]);
	close(G_inner_send_fd[1]);
	close(G_inner_check_fd[0]);
	close(G_inner_check_fd[1]);
}


static inline int	R_inner_close_msg( const int inner_close_fd )
{
	int	n, msg_type;

	
	if( (n = recv(inner_close_fd, &msg_type, SIZEOF_INT, 0)) != SIZEOF_INT )
	{
		dprintf(0, "[RECV]Error R_inner_close_msg() n(%d) \n", n );
		return (-1);
	}

	switch( msg_type )
	{
		case	INTER_EXIT_THREAD :
			dprintf(0, "[RECV]recv INTER_EXIT_THREAD \n");
			return (-1);
		default :
			dprintf(0, "[RECV]R_inner_close_msg() Unknown type(%x) \n", msg_type );
			return (-1);
	}


	return (0);
}


static inline int	R_inner_send_msg( const int inner_send_fd )
{
	int	i, nrecv, n, close_channel, check_channel[TOTAL_CHANNEL_CNT * SIZEOF_INT];

	
	if( (nrecv = recv(inner_send_fd, &check_channel, (TOTAL_CHANNEL_CNT * SIZEOF_INT), 0)) < SIZEOF_INT )
	{
		dprintf(0, "[RECV]Error R_inner_send_msg() nrecv(%d) \n", nrecv );
		return (-1);
	}

	if( (nrecv % SIZEOF_INT) != 0 )
	{
		dprintf(0, "[RECV]Error R_inner_send_msg() nrecv(%d) \n", nrecv );
		return (-1);
	}

	n	= nrecv / SIZEOF_INT;

	for( i=0; i<n; i ++ )
	{
		close_channel	= *(check_channel + i);

		R_channel_close( close_channel );
	}

	return (0);
}


static inline int	R_inner_check_msg( const int inner_check_fd )
{
	int	i, nrecv, n, temp_channel, check_channel[TOTAL_CHANNEL_CNT * SIZEOF_INT];

	
	if( (nrecv = recv(inner_check_fd, &check_channel, (TOTAL_CHANNEL_CNT * SIZEOF_INT), 0)) < SIZEOF_INT )
	{
		dprintf(0, "[RECV]Error R_inner_check_msg() nrecv(%d) \n", nrecv );
		return (-1);
	}

	if( (nrecv % SIZEOF_INT) != 0 )
	{
		dprintf(0, "[RECV]Error R_inner_check_msg() nrecv(%d) \n", nrecv );
		return (-1);
	}

	n	= nrecv / SIZEOF_INT;

	for( i=0; i<n; i ++ )
	{
		temp_channel	= *(check_channel + i);

		if( R_channel[temp_channel].connect.fd )
		{ 
			if( R_channel_live(temp_channel) )
			{
				dprintf(0, "[RECV]Error R_channel_live(%d) \n", temp_channel);
				R_channel_close(temp_channel);
			}
		}
		else
		{
			if( R_channel_connect(temp_channel) )
			{
				dprintf(0, "[RECV]Error R_channel_connect(%d) \n", temp_channel);
				continue;
			}
		}
	}

	return (0);
}


static inline int	R_channel_connect( const int channel )
{
	struct linger			ling;
	struct header_normal		header;
	struct body_MG_INIT		send_body;
	struct body_GM_INIT_RESULT	*body_ptr;
	int				channel_fd, flags, buff_size;
	char				buf[SIZEOF_HEADER_NORMAL+SIZEOF_MAX_BODY+1];


	if( R_channel[channel].connect.fd )
	{
		dprintf(0, "[RECV]Error R_channel_connect() connecting channel(%d) \n", channel );
		return (0);
	}

	if( (channel_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		dprintf(0, "[RECV]Error create socket() : %s \n", strerror(errno) );
		return (-1);	
	}		

	flags = fcntl( channel_fd, F_GETFL, 0 );
	fcntl( channel_fd, F_SETFL, flags | O_NONBLOCK );

	ling.l_onoff = ling.l_linger = 0;
	setsockopt( channel_fd, SOL_SOCKET, SO_LINGER, (char *)&ling, sizeof(struct linger) );

	buff_size	= SIZEOF_SNDBUF;
	setsockopt( channel_fd, SOL_SOCKET, SO_SNDBUF, &buff_size, SIZEOF_INT );
	buff_size	= SIZEOF_RCVBUF;
	setsockopt( channel_fd, SOL_SOCKET, SO_SNDBUF, &buff_size, SIZEOF_INT );

	if( U_ConnectToServer(channel_fd, D_get_gameserv_ip(), GAMESERV_PORT, 3) )
	{
		dprintf(0, "[RECV]Error U_ConnectToServer() \n" );
		close(channel_fd);
		return (-1);	
	}

	header.command 			= MG_INIT;
	header.body_size		= sizeof(struct body_MG_INIT);
	header.seq			= 0;

	strncpy( send_body.version	, VERSION, (VERSION_LEN + 1) );
	send_body.max_map		= D_get_map_cnt();
	send_body.max_type_mob		= D_get_type_mob_cnt();
	send_body.max_mob		= D_get_mob_cnt(channel);
	send_body.charge_channel	= channel;

	memcpy( buf, &header, SIZEOF_HEADER_NORMAL );
	memcpy( buf + SIZEOF_HEADER_NORMAL, &send_body, sizeof(struct body_MG_INIT) );

	if( U_SendToServer(channel_fd, SIZEOF_HEADER_NORMAL + header.body_size, buf, 3) )
	{
		dprintf(0, "[RECV]Error U_SendToServer() \n");
		close(channel_fd);
		return (-1);
	}

	if( U_RecvFromServer(channel_fd, SIZEOF_HEADER_NORMAL + sizeof(struct body_GM_INIT_RESULT), buf, 3) )
	{
		dprintf(0, "[RECV]Error U_RecvFromServer() \n");
		close(channel_fd);
		return (-1);
	}

	body_ptr	= (struct body_GM_INIT_RESULT *)(buf + SIZEOF_HEADER_NORMAL);

	if( body_ptr->result_code )
	{
		dprintf(0, "[RECV]Error result_code(%d) \n", body_ptr->result_code);
		close(channel_fd);
		return (-1);
	}

	R_event.events			= EPOLLIN|EPOLLHUP|EPOLLERR;
	R_event.data.fd			= channel_fd;
	*(&(R_event.data.ptr) +1)	= &(R_channel[channel].connect);
	if( epoll_ctl(G_Input_fds, EPOLL_CTL_ADD, channel_fd, &R_event) < 0 )
	{
		dprintf(0, "[RECV]Error EPOLL_CTL_ADD : %s \n", strerror(errno) );
		close(channel_fd);
		return (-1);
	}


	R_channel[channel].connect.fd		= channel_fd;
	R_channel[channel].connect.channel	= channel;

	R_channel[channel].recv_pos		= 0;

	R_channel[channel].complete_start	= 0;
	R_channel[channel].sec_live		= G_Sec;
	R_channel[channel].send_live		= 0;

	S_channel_sendbuf_init( channel, channel_fd );

	U_recv_queue_in( G_channel_proc_thr[channel], channel, INTER_CONN_FD, sizeof(struct connection)
		, (char *)&(R_channel[channel].connect) );

	dprintf(0, "[RECV]Success R_channel_connect() channel(%d), channel_fd(%d) \n", channel, channel_fd );

	return (0);
}


static inline void	R_channel_close( const int channel )
{

	if( R_channel[channel].connect.fd )
	{
		dprintf(0, "[RECV]R_channel_close() channel(%d), channel_fd(%d) \n", channel, R_channel[channel].connect.fd );

		epoll_ctl( G_Input_fds, EPOLL_CTL_DEL, R_channel[channel].connect.fd, &R_event );
		close( R_channel[channel].connect.fd );

		R_channel[channel].connect.channel	= channel;
		R_channel[channel].connect.fd		= 0;

		R_channel[channel].recv_pos		= 0;

		R_channel[channel].complete_start	= 0;
		R_channel[channel].sec_live		= 0;
		R_channel[channel].send_live		= 0;

		S_channel_sendbuf_init( channel, 0 );

		U_recv_queue_in( G_channel_proc_thr[channel], channel, INTER_CLOSE_FD, sizeof(struct connection)
			, (char *)&(R_channel[channel].connect) );
	}
}


static inline int	R_channel_live( const int channel )
{
	struct header_normal	header;
	long			flow_time = (G_Sec - R_channel[channel].sec_live);


	if( (R_channel[channel].send_live) && (flow_time > LIMIT_SEC_TO_CLOSE) )
	{
		dprintf(0, "[RECV] live timeout channel(%d) \n", channel);
		return (-1);
	}

	if( flow_time > LIMIT_SEC_TO_WARNING )
	{
		if( R_channel[channel].send_live )
			return (0);

		header.command		= MG_LIVE;
		header.body_size	= 0;
		header.seq		= 0;
		S_channel_sendbuf_in( channel, (char *)&header, sizeof(struct header_normal) );

		R_channel[channel].send_live	= 1;
	}
	
	return (0);
}


static inline int	R_channel_recv( const int channel )
{
	int	n, error;


	if( (n = recv(R_channel[channel].connect.fd, (R_channel[channel].recv_buf + R_channel[channel].recv_pos)
		, (SIZEOF_RCVBUF - R_channel[channel].recv_pos), 0)) < 1 )
	{
		if( errno == 0 )
		{
			dprintf(0, "[RECV]Normal close channel(%d) \n", channel );
			return (-1);
		}

		else if( errno != EWOULDBLOCK )
		{
			dprintf(0, "[RECV]Abnormal close channel(%d) : %s \n", channel, strerror(errno) );
			return (-1);
		}

		return (0);
	}

	R_channel[channel].recv_pos	+= n;


	while( (error = R_channel_recv_queue_in(channel)) ) 
	{
		if( error < 0 )
		{
			dprintf(0, "[RECV]Error R_map_recv_queue_in() channel(%d) \n", channel );
			return (-1);
		}
	}

	return (0);
}


static inline int	R_channel_recv_queue_in( const int channel )
{
	struct header_normal	*p_header;
	int			packet_size, move_size;
	

	if( R_channel[channel].recv_pos < SIZEOF_HEADER_NORMAL )
		return (0);

	p_header	= (struct header_normal *)R_channel[channel].recv_buf;

	// check header
	if( (p_header->command & 0xff000000) != _GM )
	{
		dprintf(0, "[RECV]Error unknown command(%x) \n", p_header->command );
		return (-1);
	}

	if( p_header->body_size > SIZEOF_MAX_BODY )
	{
		dprintf(0, "[RECV]Error header's body size(%d) \n", p_header->body_size );
		return (-1);
	}

	// check body
	packet_size	= (SIZEOF_HEADER_NORMAL + p_header->body_size);
	move_size	= R_channel[channel].recv_pos - packet_size;

	if( packet_size > R_channel[channel].recv_pos )
	{
		if( !(R_channel[channel].complete_start) )
		{
			R_channel[channel].complete_sec		= G_Sec;
			R_channel[channel].complete_start	= 1;
		}

		if( (G_Sec - R_channel[channel].complete_sec) > LIMIT_SEC_TO_PROC )
		{
			dprintf(0, "[RECV]Error over last proctime : header.command(%x), body_size(%d) \n"
				, p_header->command, p_header->body_size );
			return (-1);
		}

		return (0);
	}

	if( R_channel[channel].complete_start )
		R_channel[channel].complete_start	= 0;

	if( p_header->command == GM_LIVE )
	{
		R_channel[channel].sec_live	= G_Sec;
		R_channel[channel].send_live	= 0;

		memmove( R_channel[channel].recv_buf, (R_channel[channel].recv_buf + packet_size), move_size );
		R_channel[channel].recv_pos	= move_size;
		return (1);
	}

	U_recv_queue_in( G_channel_proc_thr[channel], channel, p_header->command, p_header->body_size
			, (R_channel[channel].recv_buf + SIZEOF_HEADER_NORMAL) );

	memmove( R_channel[channel].recv_buf, (R_channel[channel].recv_buf + packet_size), move_size );
	R_channel[channel].recv_pos	= move_size;
	return (1);
}


