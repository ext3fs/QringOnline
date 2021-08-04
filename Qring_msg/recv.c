/*____________________________________________________________

	Project name : Qring online
	File name    : recv.c
	Description  : recv module
	Date         : February 14 2005
_____________________________________________________________*/


#include	"global.h"
#include	"util.h"
#include	"db.h"
#define	_RECV_H_
#include	"recv.h"


struct  recv_fd
{
        volatile        int     epoll_add;
        int                     recv_cnt;
        volatile        time_t  recv_sec;
	volatile	time_t	live_sec;
	volatile	char	send_live;
        char                    ip[SIZEOF_IP+1];
};


static	void	R_udp_proc(const int);
static	void	R_proc_TW_LIVE(void);
static	void	R_proc_TW_KILL(void);
static	void	R_proc_TW_QUEUE(void);

static	void	R_del_fd(const int);
static	void	R_close_fd(const int);
static	void	R_accept_fd(const int, const void *);

static	void	R_check_unix_close_fd(void);
static	void	R_check_unix_live_fd(void);


static	struct	recv_fd		R_recv_fd[MAX_FD];
static	struct	epoll_event	R_event;
static	struct	sockaddr_in	R_addr;
static	int			R_addr_len;
static	char			R_recv_buf[8192];
static	char			R_send_buf[8192];


//__________________________________________________________
//
void *R_Recv_main(void *arg)
{
	int			check_unix_close_fd = 0, check_unix_live_fd = 0;
	int			sock_fd, nready, i, n;
	struct	epoll_event	ev[MAX_FD];
	struct	header_normal	header;


	U_Printf("\t[RECV]Recv Thread Start !\n");
	
	while( G_Run )
	{
		//check close
		if( check_unix_close_fd )
		{
			R_check_unix_close_fd();
			check_unix_close_fd	= 0;
		}
		
		//check live
		if( check_unix_live_fd )
		{
			R_check_unix_live_fd();
			check_unix_live_fd	= 0;
		}
	
		//epoll wait
		if( (nready = epoll_wait(G_Epoll_handler, ev, MAX_FD, -1)) < 0 )
			U_Printf("[RECV]Error epoll_wait() : %s\n", strerror(errno)), exit(-1);

		for( i=0; i<nready; i++ )
		{
			sock_fd	= ev[i].data.fd;	

			//G_Unix_close_fd
			if( sock_fd == G_Unix_close_fd[0] )
			{
				check_unix_close_fd	= 1;
				continue;
			}

			//G_Unix_live_fd
			if( sock_fd == G_Unix_live_fd[0] )
			{
				check_unix_live_fd	= 1;
				continue;
			}

			//G_Udp_fd
			if( sock_fd == G_Udp_fd )
			{
				R_addr_len	= sizeof(R_addr);
				
				if( (n = recvfrom(G_Udp_fd, R_recv_buf, 8192, 0, (struct sockaddr *)&R_addr, &R_addr_len) ) )
					R_udp_proc(n);

				continue;		
			}

			//G_Listen_fd
			if( sock_fd == G_Listen_fd )
			{
				R_addr_len	= sizeof(R_addr);
				
				if( (sock_fd = accept(G_Listen_fd, (struct sockaddr *)&R_addr, &R_addr_len)) < 0 )
					U_Printf("[RECV]Error accept() : %s\n", strerror(errno));
				else
					R_accept_fd(sock_fd, &R_addr.sin_addr);

				continue;
			}
	
		
			//recv header
			if( (n = recv(sock_fd ,&header, SIZEOF_HEADER_NORMAL, 0)) < 1 )
			{
				if( n )
					U_Printf("[RECV]Error recv fd(%d) : %s\n", sock_fd, strerror(errno));
				else
					U_Printf("[RECV]FIN recv fd(%d)\n", sock_fd);
				
				R_del_fd(sock_fd);			
				continue;
			}	

			//check header
			if( (n != SIZEOF_HEADER_NORMAL) || (header.body_size > SIZEOF_MAX_PACKET_BODY)  )
			{
				U_Printf("[RECV]Error recv fd(%d) : bad packet_header\n", sock_fd);
				R_del_fd(sock_fd);
				continue;
			}

			//recv body
			if( header.body_size )
			{ 
				if( (n = recv(sock_fd, R_recv_buf, header.body_size, 0)) < 1 )
				{
					if( n )
						U_Printf("[RECV]Error recv fd(%d) : %s\n", sock_fd, strerror(errno));
					else
						U_Printf("[RECV]FIN recv fd(%d)\n", sock_fd);
				
					R_del_fd(sock_fd);			
					continue;
				}

				if( n != header.body_size )
				{
					U_Printf("[RECV]Error recv fd(%d) : bad pakcet_body\n", sock_fd);
					R_del_fd(sock_fd);
					continue;
				}
			}
			else
			{
				n	= 0;	
			}


			//check command 
			if( ((header.command) & 0xff000000 ) == _CM )
			{
				if( header.command & CM_LIVE )
				{
					//set live_sec
					R_recv_fd[sock_fd].live_sec	= G_Sec;
					R_recv_fd[sock_fd].send_live 	= 0;
					continue;
				}

				//set recv_sec, recv_cnt
				R_recv_fd[sock_fd].recv_sec	= G_Sec;
				++(R_recv_fd[sock_fd].recv_cnt);

				U_Recv_queue_in(sock_fd, header.seq, header.command
					, 0, 0, header.body_size, R_recv_buf, R_recv_fd[sock_fd].recv_cnt);
			}
			else
			{
				U_Printf("[RECV]Error recv fd(%d) : bad command:%x\n", sock_fd, header.command);
				R_del_fd(sock_fd);
			}

		}//end for

	}//end while  		


	//thread exit
	U_Printf("\t[RECV]Recv Thread End !\n");
	U_Recv_queue_in(0, 0, INTER_EXIT_THREAD, 0, 0, 0, NULL, 0);
	return	(NULL);
}



//___________________________________________________________
//
int	R_GetRecvfd_epoll_add(const int fd)
{
	return	(R_recv_fd[fd].epoll_add);
}

time_t	R_GetRecvfd_live_sec(const int fd)
{
	return	(R_recv_fd[fd].live_sec);
}



//___________________________________________________________
//udp
static	void	R_udp_proc(const int recv_byte)
{
	struct	header_normal	*header;


	header	= (struct header_normal *)R_recv_buf;
	
	switch( header->command )
	{
		case	TW_WAKEUP:
			break;
		case	TW_LIVE:
			R_proc_TW_LIVE();
			break;
		case	TW_KILL:
			R_proc_TW_KILL();
			break;
		case	TW_QUEUE:
			R_proc_TW_QUEUE();
			break;
		default:
			U_Printf("[RECV]Error R_udp_proc() bad command:%x\n", header->command);
	}
}


static	void	R_proc_TW_LIVE(void)
{
	int	n;


	n	= snprintf(R_send_buf, 8192, "< Messenger %03d%03d :: G_Login_number:%d, G_Connection_number:%d >",
				G_Game, G_World, G_Login_number, G_Connection_number);

	sendto(G_Udp_fd, R_send_buf, n, 0, (struct sockaddr *)&R_addr, R_addr_len);
	sendto(G_Udp_fd, "SEnd", SIZEOF_INT, 0, (struct sockaddr *)&R_addr, R_addr_len);
}


static	void	R_proc_TW_KILL(void)
{
	U_Printf("[RECV]recv TW_KILL\n");

	kill(G_Pid, SIGINT);
	sendto(G_Udp_fd, "SEnd", SIZEOF_INT, 0, (struct sockaddr *)&R_addr, R_addr_len);
}


static	void	R_proc_TW_QUEUE(void)
{
	int	n, recv_q, send_q;
	

	if( (recv_q = U_GetRecv_tail() - U_GetRecv_head()) < 0 )
		recv_q	= QUEUE_CNT - U_GetRecv_head() + U_GetRecv_tail();
	
	if( (send_q = U_GetSend_tail() - U_GetSend_head()) < 0 )
		send_q	= QUEUE_CNT - U_GetSend_head() + U_GetSend_tail();

	n	= snprintf(R_send_buf, 8192, "< Messenger %03d%03d :: recv_q:%d, send_q:%d >",
				G_Game, G_World, recv_q, send_q);
	
	sendto(G_Udp_fd, R_send_buf, n, 0, (struct sockaddr *)&R_addr, R_addr_len);
	sendto(G_Udp_fd, "SEnd", SIZEOF_INT, 0, (struct sockaddr *)&R_addr, R_addr_len);
}



//___________________________________________________________
//
static	void	R_del_fd(const int fd)
{
	//epoll del
	if( epoll_ctl(G_Epoll_handler, EPOLL_CTL_DEL, fd, &R_event) < 0 )
		U_Printf("[RECV]Error DEL epoll_ctl()  : %s\n", strerror(errno)),exit(-1);
	
	//set epoll_add
	R_recv_fd[fd].epoll_add	= 0;

	U_Recv_queue_in(fd, 0, INTER_CLOSE_FD, 0, 0, 0, NULL, 0);
}


static	void	R_close_fd(const int fd)
{
	//epoll del
	if( R_recv_fd[fd].epoll_add )
	{
		if( epoll_ctl(G_Epoll_handler, EPOLL_CTL_DEL, fd, &R_event) < 0 )
			U_Printf("[RECV]Error DEL epoll_ctl()  : %s\n", strerror(errno)),exit(-1);
	
		R_recv_fd[fd].epoll_add	= 0;
	}

	//close fd
	close(fd);
	
	//set G_connection_number
	--G_Connection_number;

	//set recv_cnt, recv_sec
	R_recv_fd[fd].recv_cnt	= 0;
	R_recv_fd[fd].recv_sec	= 0;
	R_recv_fd[fd].send_live = 0;
	R_recv_fd[fd].live_sec	= 0;
	
	U_Printf("[RECV]close client fd(%d), ip:%s, G_Login_number:%d, G_Connection_number:%d\n",
		fd, R_recv_fd[fd].ip, G_Login_number, G_Connection_number);
		
	//set ip
	R_recv_fd[fd].ip[0]	= 0;
}


static	void	R_accept_fd(const int accept_fd, const void *sin_addr)
{
	int	flags;		

	//set ip
	if( inet_ntop(AF_INET, sin_addr, R_recv_fd[accept_fd].ip, SIZEOF_IP) == NULL )
	{
		U_Printf("[RECV]Error inet_ntop() : %s\n", strerror(errno));
		close(accept_fd);	
		return;
	}

	//check full client
	if( accept_fd > (MAX_FD-100) )
	{
		U_Printf("[RECV]Full client\n");
		close(accept_fd);	
		return;
	}

	//set nonblock 
	flags	= fcntl(accept_fd, F_GETFL, 0);
	fcntl(accept_fd, F_SETFL, flags | O_NONBLOCK);
	
	//epoll add
	R_event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
	R_event.data.fd	= accept_fd;
	
	if( epoll_ctl(G_Epoll_handler, EPOLL_CTL_ADD, accept_fd, &R_event) < 0 )
	{
		U_Printf("[RECV]Error ADD epoll_ctl()  : %s\n", strerror(errno));
		close(accept_fd);
		return;
	}

	//set epoll_add, recv_sec
	R_recv_fd[accept_fd].epoll_add	= 1;
	R_recv_fd[accept_fd].recv_sec	= G_Sec;
	R_recv_fd[accept_fd].send_live	= 0;
	R_recv_fd[accept_fd].live_sec	= G_Sec;
		
	//set G_Connection_number	
	++G_Connection_number;

	U_Printf("[RECV]accept client fd:%d, ip:%s, G_Login_number:%d, G_Connection_number:%d\n"
		, accept_fd, R_recv_fd[accept_fd].ip, G_Login_number, G_Connection_number);

	U_Recv_queue_in(accept_fd, 0, INTER_CONN_CLIENT_FD, 0, 0, 0, NULL, 0);
	return;
}



//______________________________________________________
//
static	void	R_check_unix_close_fd(void)
{
	int	n, i;

	n	= recv(G_Unix_close_fd[0], R_recv_buf, 8192, 0) / SIZEOF_INT;
			
	for( i=0; i<n; i++ )
		R_close_fd(*((int *)R_recv_buf +i));
}


static	void	R_check_unix_live_fd(void)
{
	int	n, i, sock_fd;
	int	check_sec;


	n	= recv(G_Unix_live_fd[0], R_recv_buf, 8192, 0) / SIZEOF_INT;

	for( i=0; i<n; i++ )
	{
		sock_fd	= *((int *)R_recv_buf +i);

		if( R_recv_fd[sock_fd].epoll_add )
		{
			check_sec	= (G_Sec - R_recv_fd[sock_fd].live_sec);

			if( check_sec > SEC_TO_CLOSE )
			{
				U_Printf("[RECV]Error fd:%d recv_time out ! \n", sock_fd);
				R_del_fd(sock_fd);
				continue;
			}

			if( !(R_recv_fd[sock_fd].send_live) && (check_sec > SEC_HEARTBEAT) )
			{
				U_Send_queue_in(MC_LIVE, sock_fd, 0, 0, 0, NULL);
				R_recv_fd[sock_fd].send_live	= 1;
			}
		}
	}
}
