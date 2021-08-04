/*____________________________________________________________

	Project name : Qring Online
	File name    : recv.c
	Description  : recv module
	Date         : February 14 2005
_____________________________________________________________*/


#include	"global.h"
#include	"util.h"
#include	"mk_packet.h"
#include	"db.h"
#include	"crypt.h"
#define	_RECV_H_
#include	"recv.h"


struct	recv_fd
{
	volatile	int	epoll_add;
	int			recv_cnt;
	volatile	time_t	recv_sec;
	char			ip[SIZEOF_IP+1];
	int			billing_idx;
	int			dbagent_idx;
	int			login_dbagent_idx;
	int			mobserv_flag;
	int			connecting; 
	int			recv_size;
	char			recv_buf[APP_BUF_SIZE];
};


static	void	R_udp_proc(const int);
static	void	R_proc_TW_LIVE(void);
static	void	R_proc_TW_KILL(void);
static	void	R_proc_TW_QUEUE(void);

static	void	R_del_fd(const int);
static	void	R_close_fd(const int);
static	void	R_accept_mobserv_fd(const int, const void *);
static	void	R_accept_game_fd(const int, const void *);

static	void	R_recv_unix_tick(void);
static	void	R_check_unix_billing_idx(void);
static	void	R_check_unix_login_dbagent_idx(void);
static	void	R_check_unix_dbagent_idx(void);
static	void	R_check_unix_close_fd(void);
static	void	R_check_unix_live_fd(void);

static	void	R_recv_mobserv(const int);
static	int	R_connect_billing(const int);
static	void	R_recv_billing(const int);
static	int	R_connect_login_dbagent(const int);
static	void	R_recv_login_dbagent(const int);
static	int	R_connect_dbagent(const int);
static	void	R_recv_dbagent(const int);



static	struct	recv_fd		R_recv_fd[MAX_FD+1];
static	volatile	int	R_billing_fd[CONNECTION_BILLING+1];
static	volatile	int	R_dbagent_fd[CONNECTION_DBAGENT+1];
static	volatile	int	R_login_dbagent_fd[CONNECTION_LOGIN_DBAGENT+1];
static	struct	epoll_event	R_event;
static	struct	sockaddr_in	R_addr;
static	int			R_addr_len;
static	char			R_recv_buf[8192];
static	char			R_send_buf[8192];


//__________________________________________________________
//
void *R_Recv_main(void *arg)
{
	int			check_unix_billing_idx = 0, check_unix_login_dbagent_idx = 0, check_unix_dbagent_idx = 0;
	int			check_unix_close_fd = 0, check_unix_live_fd = 0;
	int			sock_fd, nready, i, n;
	struct	epoll_event	ev[MAX_FD+1];
	struct	header_normal	header;


	U_Printf("\t[RECV]Recv Thread Start !\n");
	
	while( G_Run )
	{
		//check billing
		if( check_unix_billing_idx )
		{
			R_check_unix_billing_idx();
			check_unix_billing_idx	= 0;
		}
		
		//check login_dbagent
		if( check_unix_login_dbagent_idx )
		{
			R_check_unix_login_dbagent_idx();
			check_unix_login_dbagent_idx	= 0;
		}

		//check dbagent
		if( check_unix_dbagent_idx )
		{
			R_check_unix_dbagent_idx();
			check_unix_dbagent_idx	= 0;
		}

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
			
			//tick
			if( sock_fd == G_Unix_tick[0] )
			{
				R_recv_unix_tick();
				continue;
			}
		
			//mobserv
			if( R_recv_fd[sock_fd].mobserv_flag )
			{
				R_recv_mobserv(sock_fd);
				continue;
			}

			//billing
			if( R_recv_fd[sock_fd].billing_idx )
			{
				R_recv_billing(sock_fd);
				continue;
			}
			
			//login_dbagent
			if( R_recv_fd[sock_fd].login_dbagent_idx )
			{
				R_recv_login_dbagent(sock_fd);
				continue;
			}

			//dbagent
			if( R_recv_fd[sock_fd].dbagent_idx )
			{
				R_recv_dbagent(sock_fd);
				continue;
			}

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

			//G_Unix_billing_idx
			if( sock_fd == G_Unix_billing_idx[0] )
			{
				check_unix_billing_idx	= 1;
				continue;
			}
			
			//G_Unix_login_dbagent_idx
			if( sock_fd == G_Unix_login_dbagent_idx[0] )
			{
				check_unix_login_dbagent_idx	= 1;
				continue;
			}

			//G_Unix_dbagent_idx
			if( sock_fd == G_Unix_dbagent_idx[0] )
			{
				check_unix_dbagent_idx	= 1;
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
			
			//G_Listen_mob_fd
			if( sock_fd == G_Listen_mob_fd )
			{
				R_addr_len	= sizeof(R_addr);
				
				if( (sock_fd = accept(G_Listen_mob_fd, (struct sockaddr *)&R_addr, &R_addr_len)) < 0 )
					U_Printf("[RECV]Error accept() : %s\n", strerror(errno));
				else
					R_accept_mobserv_fd(sock_fd, &R_addr.sin_addr);

				continue;
			}
		

			//G_Listen_game_fd
			if( sock_fd == G_Listen_game_fd )
			{
				R_addr_len	= sizeof(R_addr);
				
				if( (sock_fd = accept(G_Listen_game_fd, (struct sockaddr *)&R_addr, &R_addr_len)) < 0 )
					U_Printf("[RECV]Error accept() : %s\n", strerror(errno));
				else
					R_accept_game_fd(sock_fd, &R_addr.sin_addr);

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
			if( (n != SIZEOF_HEADER_NORMAL) || (header.body_size > SIZEOF_MAX_GAME_PACKET_BODY) )
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
					U_Printf("[RECV]Error recv fd(%d) : bad body_size(%d)(%d)\n", sock_fd, n, header.body_size);
					R_del_fd(sock_fd);
					continue;
				}
			}

			//check command 
			if( ((header.command) & 0xff000000 ) != _CG )
			{
				U_Printf("[RECV]Error recv fd(%d) : bad command(%x)\n", sock_fd, header.command);
				R_del_fd(sock_fd);
				continue;
			}
				
			//set recv_sec, recv_cnt
			R_recv_fd[sock_fd].recv_sec	= G_Sec;
			++(R_recv_fd[sock_fd].recv_cnt);

			U_Recv_queue_in(sock_fd, header.seq, header.command, 0, 0, header.body_size, R_recv_buf, R_recv_fd[sock_fd].recv_cnt);
		}//end for
	}//end while  		


	//thread exit
	U_Printf("\t[RECV]Recv Thread End !\n");
	U_Recv_queue_in(0, 0, INTER_EXIT_THREAD, 0, 0, 0, NULL, 0);
	return	(NULL);
}



//___________________________________________________________
//
int	R_GetBilling_fd(const int billing_idx)
{
	return	(R_billing_fd[billing_idx]);
}


int	R_GetDbagent_fd(const int dbagent_idx)
{
	return	(R_dbagent_fd[dbagent_idx]);
}


int	R_GetLogin_dbagent_fd(const int login_dbagent_idx)
{
	return	(R_login_dbagent_fd[login_dbagent_idx]);
}


int	R_GetRecvfd_epoll_add(const int fd)
{
	return	(R_recv_fd[fd].epoll_add);
}


time_t	R_GetRecvfd_recv_sec(const int fd)
{
	return	(R_recv_fd[fd].recv_sec);
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
			U_Printf("[RECV]Error R_udp_proc() bad command(%x)\n", header->command);
	}
}


static	void	R_proc_TW_LIVE(void)
{
	int	n, i;


	n	= snprintf(R_send_buf, 8192, "< Game (%d) :: G_Login_number(%d) G_Connection_number(%d)",
								G_Server_code, G_Login_number, G_Connection_number);
	
	n	+= snprintf(R_send_buf+n, 8192-n, " R_dbagent_fd");		
	
	for( i=1; i<=CONNECTION_DBAGENT; i++ )
		n	+= snprintf(R_send_buf+n, 8192-n, "(%d)", R_dbagent_fd[i]);		
	
	n	+= snprintf(R_send_buf+n, 8192-n, " R_login_dbagent_fd");		
	
	for( i=1; i<=CONNECTION_LOGIN_DBAGENT; i++ )
		n	+= snprintf(R_send_buf+n, 8192-n, "(%d)", R_login_dbagent_fd[i]);		
	
	n	+= snprintf(R_send_buf+n, 8192-n, " R_billing_fd");		
	
	for( i=1; i<=CONNECTION_BILLING; i++ )
		n	+= snprintf(R_send_buf+n, 8192-n, "(%d)", R_billing_fd[i]);		
	
	n	+= snprintf(R_send_buf+n, 8192-n, " user");		
	
	for( i=1; i<=CHANNEL_PER_MACHINE; i++ )
		n	+= snprintf(R_send_buf+n, 8192-n, "(%d)", D_GetWorld_channel_user(G_World, i));		
	
	n	+= snprintf(R_send_buf+n, 8192-n, " >");
		
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

	n	= snprintf(R_send_buf, 8192, "< Game (%d) :: recv_q(%d) send_q(%d) >",
				G_Server_code, recv_q, send_q);
	
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
	
	//set recv_cnt, recv_sec
	R_recv_fd[fd].recv_cnt	= 0;
	R_recv_fd[fd].recv_sec	= 0;
	
	//set G_connection_number
	--G_Connection_number;
	
	//mob
	if( R_recv_fd[fd].mobserv_flag )
	{
		U_Printf("[RECV]close mobserv fd(%d) ip(%s) G_Login_number(%d) G_Connection_number(%d)\n",
				fd, R_recv_fd[fd].ip, G_Login_number, G_Connection_number);
		
		//set mobserv_flag, ip, recv_size
		R_recv_fd[fd].mobserv_flag	= 0;
		R_recv_fd[fd].ip[0]		= 0;
		R_recv_fd[fd].recv_size		= 0;		
		return;
	}
	
	//billing
	if( R_recv_fd[fd].billing_idx )
	{
		R_billing_fd[R_recv_fd[fd].billing_idx]	= 0;
	
		U_Printf("[RECV]close billing fd(%d) idx(%d) ip(%s) G_Login_number(%d) G_Connection_number(%d)\n",
				fd, R_recv_fd[fd].billing_idx, R_recv_fd[fd].ip, G_Login_number, G_Connection_number);
		
		//set billing_idx, ip, recv_size, connecting
		R_recv_fd[fd].billing_idx	= 0;
		R_recv_fd[fd].ip[0]		= 0;
		R_recv_fd[fd].recv_size		= 0;		
		R_recv_fd[fd].connecting	= 0;
		return;
	}
	
	//login_dbagent 
	if( R_recv_fd[fd].login_dbagent_idx )
	{
		R_login_dbagent_fd[R_recv_fd[fd].login_dbagent_idx]	= 0;
	
		U_Printf("[RECV]close login_dbagent fd(%d) idx(%d) ip(%s) G_Login_number(%d) G_Connection_number(%d)\n",
				fd, R_recv_fd[fd].login_dbagent_idx, R_recv_fd[fd].ip, G_Login_number, G_Connection_number);
		
		//set login_dbagent_idx, ip, recv_size, connecting
		R_recv_fd[fd].login_dbagent_idx	= 0;
		R_recv_fd[fd].ip[0]		= 0;
		R_recv_fd[fd].recv_size		= 0;		
		R_recv_fd[fd].connecting	= 0;
		return;
	}

	//dbagent 
	if( R_recv_fd[fd].dbagent_idx )
	{
		R_dbagent_fd[R_recv_fd[fd].dbagent_idx]	= 0;
	
		U_Printf("[RECV]close dbagent fd(%d) idx(%d) ip(%s) G_Login_number(%d) G_Connection_number(%d)\n",
			fd, R_recv_fd[fd].dbagent_idx, R_recv_fd[fd].ip, G_Login_number, G_Connection_number);
		
		//set dbagent_idx, ip, recv_size, connecting
		R_recv_fd[fd].dbagent_idx	= 0;
		R_recv_fd[fd].ip[0]		= 0;
		R_recv_fd[fd].recv_size		= 0;		
		R_recv_fd[fd].connecting	= 0;
		return;
	}
	
	U_Printf("[RECV]close client fd(%d) ip(%s) G_Login_number(%d) G_Connection_number(%d)\n",
							fd, R_recv_fd[fd].ip, G_Login_number, G_Connection_number);
		
	//set ip
	R_recv_fd[fd].ip[0]	= 0;
}


static	void	R_accept_mobserv_fd(const int accept_fd, const void *sin_addr)
{
	int	flags;		
	char	ip[SIZEOF_IP+1];


	//ip
	if( inet_ntop(AF_INET, sin_addr, ip, SIZEOF_IP+1) == NULL )
	{
		U_Printf("[RECV]Error inet_ntop() : %s\n", strerror(errno));
		close(accept_fd);	
		return;
	}

	//check ip
	if( strncmp(ip, D_GetWorld_machine_mobserv_ip(G_World, G_Machine), SIZEOF_IP+1) )
	{
		U_Printf("[RECV]Error G_World(%d) G_Machine(%d) mobserv_ip(%s) bad ip(%s)\n",
					G_World, G_Machine, D_GetWorld_machine_mobserv_ip(G_World, G_Machine), ip);
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

	//set epoll_add, recv_sec, ip, mobserv_flag
	R_recv_fd[accept_fd].epoll_add		= 1;
	R_recv_fd[accept_fd].recv_sec		= G_Sec;
	strncpy(R_recv_fd[accept_fd].ip, ip, SIZEOF_IP+1);
	R_recv_fd[accept_fd].mobserv_flag	= 1;
	

	//set G_Connection_number	
	++G_Connection_number;

	U_Printf("[RECV]accept mob fd(%d), ip(%s), G_Login_number(%d), G_Connection_number(%d)\n"
		, accept_fd, R_recv_fd[accept_fd].ip, G_Login_number, G_Connection_number);

	U_Recv_queue_in(accept_fd, 0, INTER_CONN_MOBSERV_FD, 0, 0, SIZEOF_IP+1, ip, 0);
	return;
}


static	void	R_accept_game_fd(const int accept_fd, const void *sin_addr)
{
	int	flags, n;		
	char	ip[SIZEOF_IP+1];


	//ip
	if( inet_ntop(AF_INET, sin_addr, ip, SIZEOF_IP+1) == NULL )
	{
		U_Printf("[RECV]Error inet_ntop() : %s\n", strerror(errno));
		close(accept_fd);	
		return;
	}

	//check server full
	if( G_Connection_number > MAX_CONNECTION )
	{
		U_Printf("[RECV]Server full : G_Connection_number(%d)\n", G_Connection_number);
		n	= MK_GetPacket_GC_SERVER_TROUBLE(R_send_buf, SERVER_TROUBLE_FULL);
		send(accept_fd, R_send_buf, n, 0);
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

	//set epoll_add, recv_sec, ip
	R_recv_fd[accept_fd].epoll_add	= 1;
	R_recv_fd[accept_fd].recv_sec	= G_Sec;
	strncpy(R_recv_fd[accept_fd].ip, ip, SIZEOF_IP+1);

	//set G_Connection_number	
	++G_Connection_number;

	U_Printf("[RECV]accept client fd(%d), ip(%s), G_Login_number(%d), G_Connection_number(%d)\n"
		, accept_fd, R_recv_fd[accept_fd].ip, G_Login_number, G_Connection_number);

	U_Recv_queue_in(accept_fd, 0, INTER_CONN_CLIENT_FD, 0, 0, SIZEOF_IP+1, ip, 0);
	return;
}



//______________________________________________________
//
static	void	R_recv_unix_tick(void)
{
	int	n, i, tick;


	n	= recv(G_Unix_tick[0], R_recv_buf, 8192, 0) / SIZEOF_INT;
	
	for( i=0; i<n; i++ )
	{
		tick	= *((int *)R_recv_buf +i);

		U_Recv_queue_in(tick, 0, INTER_TICK, 0, 0, 0, NULL, 0);
	}
}


static	void	R_check_unix_billing_idx(void)
{
	int	n, i, billing_idx;


	n	= recv(G_Unix_billing_idx[0], R_recv_buf, 8192, 0) / SIZEOF_INT;
	
	for( i=0; i<n; i++ )
	{
		billing_idx	= *((int *)R_recv_buf +i);

		if( R_billing_fd[billing_idx] == 0 )
		{
			if( R_connect_billing(billing_idx) )
				U_Printf("[RECV]Error R_connect_billing(%d)\n", billing_idx);
		}
	}
}


static	void	R_check_unix_login_dbagent_idx(void)
{
	int	n, i, login_dbagent_idx;


	n	= recv(G_Unix_login_dbagent_idx[0], R_recv_buf, 8192, 0) / SIZEOF_INT;
	
	for( i=0; i<n; i++ )
	{
		login_dbagent_idx	= *((int *)R_recv_buf +i);

		if( R_login_dbagent_fd[login_dbagent_idx] == 0 )
		{
			if( R_connect_login_dbagent(login_dbagent_idx) )
				U_Printf("[RECV]Error R_connect_login_dbagent(%d)\n", login_dbagent_idx);
		}
	}
}


static	void	R_check_unix_dbagent_idx(void)
{
	int	n, i, dbagent_idx;


	n	= recv(G_Unix_dbagent_idx[0], R_recv_buf, 8192, 0) / SIZEOF_INT;
	
	for( i=0; i<n; i++ )
	{
		dbagent_idx	= *((int *)R_recv_buf +i);

		if( R_dbagent_fd[dbagent_idx] == 0 )
		{
			if( R_connect_dbagent(dbagent_idx) )
				U_Printf("[RECV]Error R_connect_dbagent(%d)\n", dbagent_idx);
		}
	}
}


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


	n	= recv(G_Unix_live_fd[0], R_recv_buf, 8192, 0) / SIZEOF_INT;

	for( i=0; i<n; i++ )
	{
		sock_fd	= *((int *)R_recv_buf +i);

		if( R_recv_fd[sock_fd].epoll_add && ((G_Sec - R_recv_fd[sock_fd].recv_sec) > SEC_HEARTBEAT) )
		{
			U_Printf("[RECV]Error fd(%d) recv_time check!\n", sock_fd);
			R_del_fd(sock_fd);
		}
	}
}



//______________________________________________________
//
static	void	R_recv_mobserv(const int mobserv_fd)
{
	int			n;
	struct	header_normal	*header;
	char			*buf_ptr;
	

	buf_ptr		= R_recv_fd[mobserv_fd].recv_buf;
	
	//recv data
	if( (n = recv(mobserv_fd,
			R_recv_fd[mobserv_fd].recv_buf+R_recv_fd[mobserv_fd].recv_size, APP_BUF_SIZE-R_recv_fd[mobserv_fd].recv_size, 0)) < 1 )
	{	
		if( n )
			U_Printf("[RECV]Error mobserv fd(%d), recv() : %s\n", mobserv_fd, strerror(errno));
		else
			U_Printf("[RECV]FIN mobserv fd(%d)\n", mobserv_fd);
			
		R_del_fd(mobserv_fd);
		return;
	}
		
	//set recv_sec, recv_size
	R_recv_fd[mobserv_fd].recv_sec	= G_Sec;
	R_recv_fd[mobserv_fd].recv_size	+= n;
	
	//proc buf
	while( R_recv_fd[mobserv_fd].recv_size >= SIZEOF_HEADER_NORMAL )	
	{	
		header	= (struct header_normal *)buf_ptr;

		//check body_size
		if( header->body_size > SIZEOF_MAX_SERV_PACKET_BODY )
		{
			U_Printf("[RECV]Error mobserv fd(%d), bad header.body_size(%d)\n", mobserv_fd, header->body_size);
			R_del_fd(mobserv_fd);
			return;			
		} 

		//check command 
		if( ((header->command) & 0xff000000 ) != _MG )
		{
			U_Printf("[RECV]Error mobserv fd(%d), bad header.command(%x)\n", mobserv_fd, header->command);
			R_del_fd(mobserv_fd);
			return;			
		}

		//check packet
		if( (header->body_size + SIZEOF_HEADER_NORMAL) > R_recv_fd[mobserv_fd].recv_size )
		{
			memmove(R_recv_fd[mobserv_fd].recv_buf, buf_ptr, R_recv_fd[mobserv_fd].recv_size);
			return;
		}
			
		//complete packet
		U_Recv_queue_in(mobserv_fd, 0, header->command, 0, 0, header->body_size, buf_ptr+SIZEOF_HEADER_NORMAL, 0);
	
		//set recv_size
		R_recv_fd[mobserv_fd].recv_size	-= (header->body_size + SIZEOF_HEADER_NORMAL);
		buf_ptr				+= (header->body_size + SIZEOF_HEADER_NORMAL);
	}

	memmove(R_recv_fd[mobserv_fd].recv_buf, buf_ptr, R_recv_fd[mobserv_fd].recv_size);
}


static	int	R_connect_login_dbagent(const int login_dbagent_idx)
{
	int			sock_fd, flags, n;
	struct	sockaddr_in	addr;


	//create socket
	if( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		U_Printf("[RECV]Error socket() : %s\n", strerror(errno));
		return	(-1);
	}
		
	flags	= fcntl(sock_fd, F_GETFL, 0);
	fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK);

	//set socket buf
	n	= SOCKET_BUF_SIZE;
	if( setsockopt(sock_fd, SOL_SOCKET, SO_RCVBUF, &n , 4) < 0 )
	{
		U_Printf("[RECV]Error setsockopt() : %s\n", strerror(errno));
    		close(sock_fd);
		return	(-1);
	}

	n	= SOCKET_BUF_SIZE;
	if( setsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, &n , 4) < 0 )
	{
		U_Printf("[RECV]Error setsockopt() : %s\n", strerror(errno));
    		close(sock_fd);
    		return	(-1);
	}
	
	//set addr
	bzero(&addr, sizeof(struct sockaddr_in));
 	addr.sin_family	= AF_INET;
	addr.sin_port	= htons(DBAGENT_PORT);
		
	if( inet_pton(AF_INET, G_Login_dbagent_ip, &(addr.sin_addr)) <= 0 )
	{
		U_Printf("[RECV]Error inet_pton(%s)\n", G_Login_dbagent_ip);
    		close(sock_fd);
    		return	(-1);
	}
	
	//connect
	if( (n = connect(sock_fd, (struct sockaddr *)&(addr), sizeof(struct sockaddr_in))) )
	{
		if( errno != EINPROGRESS )
		{
			U_Printf("[RECV]Error connect() : %s\n", strerror(errno));
			close(sock_fd);
    			return	(-1);
		}
	}
	
	if( n == 0 )
	{
		U_Printf("[RECV]Error connect success in local\n");
		close(sock_fd);
		return	(-1);
	}
	
	//epoll add		
	R_event.events	= EPOLLIN|EPOLLERR|EPOLLHUP|EPOLLOUT;
	R_event.data.fd	= sock_fd;
	
	if( epoll_ctl(G_Epoll_handler, EPOLL_CTL_ADD, sock_fd, &R_event) < 0 )
	{
		U_Printf("[RECV]Error epoll_ctl() : %s\n", strerror(errno));
		close(sock_fd);
		return	(-1);
	}

	//set R_login_dbagent_fd
	R_login_dbagent_fd[login_dbagent_idx]	= sock_fd;
	
	//set login_dbagent_idx, epoll_add, recv_sec, connecting, ip
	R_recv_fd[sock_fd].connecting	= 1;
	R_recv_fd[sock_fd].login_dbagent_idx	= login_dbagent_idx;
	R_recv_fd[sock_fd].epoll_add	= 1;
	R_recv_fd[sock_fd].recv_sec	= G_Sec;
	strncpy(R_recv_fd[sock_fd].ip, G_Login_dbagent_ip, SIZEOF_IP+1);

	++G_Connection_number;
	return	(0);
}		


static	void	R_recv_login_dbagent(const int login_dbagent_fd)
{
	int			n, opterror, optlen;
	struct	header_special	*header;
	char			*buf_ptr;
	

	//recv first event					
	if( R_recv_fd[login_dbagent_fd].connecting )
	{
		opterror	= 0;
		optlen		= SIZEOF_INT;
					
		if( getsockopt(login_dbagent_fd, SOL_SOCKET, SO_ERROR, &opterror, &optlen) < 0 )
		{
			U_Printf("[RECV]Error login_dbagent_fd(%d), idx(%d), getsockopt() : %s\n",
					login_dbagent_fd, R_recv_fd[login_dbagent_fd].login_dbagent_idx, strerror(errno));
			R_close_fd(login_dbagent_fd);	
			return;		
		}

		if( opterror )
		{
			U_Printf("[RECV]Error login_dbagent_fd(%d), idx(%d), opterror : %s\n",
					login_dbagent_fd, R_recv_fd[login_dbagent_fd].login_dbagent_idx, strerror(opterror));
			R_close_fd(login_dbagent_fd);
			return;			
		}
	
		U_Printf("[RECV]success connect loign_dbagent_fd(%d), idx(%d)\n",
							login_dbagent_fd, R_recv_fd[login_dbagent_fd].login_dbagent_idx);
						
		R_event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
		R_event.data.fd	= login_dbagent_fd;
				
		if( epoll_ctl(G_Epoll_handler, EPOLL_CTL_MOD, login_dbagent_fd, &R_event) < 0 )
			U_Printf("[RECV]Error EPOLL_CTL_MOD epoll_ctl() : %s\n", strerror(errno)),exit(-1);
			
		//set connecting, recv_sec 		
		R_recv_fd[login_dbagent_fd].connecting	= 0;
		R_recv_fd[login_dbagent_fd].recv_sec		= G_Sec;

		U_Recv_queue_in(login_dbagent_fd, R_recv_fd[login_dbagent_fd].login_dbagent_idx,
					INTER_CONN_LOGIN_DBAGENT_FD, 0, 0, SIZEOF_IP+1, R_recv_fd[login_dbagent_fd].ip, 0);
		return;
	}

	//data receive
	buf_ptr		= R_recv_fd[login_dbagent_fd].recv_buf;
	
	//recv data
	if( (n = recv(login_dbagent_fd, R_recv_fd[login_dbagent_fd].recv_buf+R_recv_fd[login_dbagent_fd].recv_size,
									APP_BUF_SIZE-R_recv_fd[login_dbagent_fd].recv_size, 0)) < 1 )
	{	
		if( n )
			U_Printf("[RECV]Error login_dbagent_fd(%d), idx(%d), recv() : %s\n",
					login_dbagent_fd, R_recv_fd[login_dbagent_fd].login_dbagent_idx, strerror(errno));
		else
			U_Printf("[RECV]FIN login_dbagent fd(%d), idx(%d)\n", login_dbagent_fd, R_recv_fd[login_dbagent_fd].login_dbagent_idx);
			
		R_del_fd(login_dbagent_fd);
		return;
	}
		
	//set recv_sec, recv_size
	R_recv_fd[login_dbagent_fd].recv_sec	= G_Sec;
	R_recv_fd[login_dbagent_fd].recv_size	+= n;
	
	//proc buf
	while( R_recv_fd[login_dbagent_fd].recv_size >= SIZEOF_HEADER_SPECIAL )	
	{	
		header	= (struct header_special *)buf_ptr;

		//check body_size
		if( header->body_size > SIZEOF_MAX_SERV_PACKET_BODY )
		{
			U_Printf("[RECV]Error login_dbagent_fd(%d), idx(%d), bad header.body_size(%d)\n",
					login_dbagent_fd, R_recv_fd[login_dbagent_fd].login_dbagent_idx, header->body_size);
			R_del_fd(login_dbagent_fd);
			return;			
		} 

		//check command 
		if( ((header->command) & 0xff000000 ) !=  _AW )
		{
			U_Printf("[RECV]Error login_dbagent_fd(%d), idx(%d), bad header.command(%x)\n",
					login_dbagent_fd, R_recv_fd[login_dbagent_fd].login_dbagent_idx, header->command);
			R_del_fd(login_dbagent_fd);
			return;			
		}

		//check packet
		if( (header->body_size + SIZEOF_HEADER_SPECIAL) > R_recv_fd[login_dbagent_fd].recv_size )
		{
			memmove(R_recv_fd[login_dbagent_fd].recv_buf, buf_ptr, R_recv_fd[login_dbagent_fd].recv_size);
			return;
		}
			
		//complete packet
		U_Recv_queue_in(login_dbagent_fd, header->fd, header->command
				, header->sec_active, header->session, header->body_size, buf_ptr+SIZEOF_HEADER_SPECIAL, 0);
	
		//set recv_size
		R_recv_fd[login_dbagent_fd].recv_size	-= (header->body_size + SIZEOF_HEADER_SPECIAL);
		buf_ptr				+= (header->body_size + SIZEOF_HEADER_SPECIAL);
	}

	memmove(R_recv_fd[login_dbagent_fd].recv_buf, buf_ptr, R_recv_fd[login_dbagent_fd].recv_size);
}


static	int	R_connect_dbagent(const int dbagent_idx)
{
	int			sock_fd, flags, n;
	struct	sockaddr_in	addr;


	//create socket
	if( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		U_Printf("[RECV]Error socket() : %s\n", strerror(errno));
		return	(-1);
	}
		
	flags	= fcntl(sock_fd, F_GETFL, 0);
	fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK);

	//set socket buf
	n	= SOCKET_BUF_SIZE;
	if( setsockopt(sock_fd, SOL_SOCKET, SO_RCVBUF, &n , 4) < 0 )
	{
		U_Printf("[RECV]Error setsockopt() : %s\n", strerror(errno));
    		close(sock_fd);
		return	(-1);
	}

	n	= SOCKET_BUF_SIZE;
	if( setsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, &n , 4) < 0 )
	{
		U_Printf("[RECV]Error setsockopt() : %s\n", strerror(errno));
    		close(sock_fd);
    		return	(-1);
	}
	
	//set addr
	bzero(&addr, sizeof(struct sockaddr_in));
 	addr.sin_family	= AF_INET;
	addr.sin_port	= htons(DBAGENT_PORT);
		
	if( inet_pton(AF_INET, D_GetWorld_machine_dbagent_ip(G_World, G_Machine), &(addr.sin_addr)) <= 0 )
	{
		U_Printf("[RECV]Error inet_pton(%s)\n", D_GetWorld_machine_dbagent_ip(G_World, G_Machine));
    		close(sock_fd);
    		return	(-1);
	}
	
	//connect
	if( (n = connect(sock_fd, (struct sockaddr *)&(addr), sizeof(struct sockaddr_in))) )
	{
		if( errno != EINPROGRESS )
		{
			U_Printf("[RECV]Error connect() : %s\n", strerror(errno));
			close(sock_fd);
    			return	(-1);
		}
	}
	
	if( n == 0 )
	{
		U_Printf("[RECV]Error connect success in local\n");
		close(sock_fd);
		return	(-1);
	}
	
	//epoll add		
	R_event.events	= EPOLLIN|EPOLLERR|EPOLLHUP|EPOLLOUT;
	R_event.data.fd	= sock_fd;
	
	if( epoll_ctl(G_Epoll_handler, EPOLL_CTL_ADD, sock_fd, &R_event) < 0 )
	{
		U_Printf("[RECV]Error epoll_ctl() : %s\n", strerror(errno));
		close(sock_fd);
		return	(-1);
	}

	//set R_dbagent_fd
	R_dbagent_fd[dbagent_idx]	= sock_fd;
	
	//set dbagent_idx, epoll_add, recv_sec, connecting, ip
	R_recv_fd[sock_fd].connecting	= 1;
	R_recv_fd[sock_fd].dbagent_idx	= dbagent_idx;
	R_recv_fd[sock_fd].epoll_add	= 1;
	R_recv_fd[sock_fd].recv_sec	= G_Sec;
	strncpy(R_recv_fd[sock_fd].ip, D_GetWorld_machine_dbagent_ip(G_World, G_Machine), SIZEOF_IP+1);

	++G_Connection_number;
	return	(0);
}		


static	void	R_recv_dbagent(const int dbagent_fd)
{
	int			n, opterror, optlen;
	struct	header_special	*header;
	char			*buf_ptr;
	

	//recv first event					
	if( R_recv_fd[dbagent_fd].connecting )
	{
		opterror	= 0;
		optlen		= SIZEOF_INT;
					
		if( getsockopt(dbagent_fd, SOL_SOCKET, SO_ERROR, &opterror, &optlen) < 0 )
		{
			U_Printf("[RECV]Error dbagent fd(%d), idx(%d), getsockopt() : %s\n",
					dbagent_fd, R_recv_fd[dbagent_fd].dbagent_idx, strerror(errno));
			R_close_fd(dbagent_fd);	
			return;		
		}

		if( opterror )
		{
			U_Printf("[RECV]Error dbagent fd(%d), idx(%d), opterror : %s\n",
					dbagent_fd, R_recv_fd[dbagent_fd].dbagent_idx, strerror(opterror));
			R_close_fd(dbagent_fd);
			return;			
		}
	
		U_Printf("[RECV]success connect dbagent fd(%d), idx(%d)\n", dbagent_fd, R_recv_fd[dbagent_fd].dbagent_idx);
						
		R_event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
		R_event.data.fd	= dbagent_fd;
				
		if( epoll_ctl(G_Epoll_handler, EPOLL_CTL_MOD, dbagent_fd, &R_event) < 0 )
			U_Printf("[RECV]Error EPOLL_CTL_MOD epoll_ctl() : %s\n", strerror(errno)),exit(-1);
			
		//set connecting, recv_sec 		
		R_recv_fd[dbagent_fd].connecting	= 0;
		R_recv_fd[dbagent_fd].recv_sec		= G_Sec;

		U_Recv_queue_in(dbagent_fd, R_recv_fd[dbagent_fd].dbagent_idx,
							INTER_CONN_DBAGENT_FD, 0, 0, SIZEOF_IP+1, R_recv_fd[dbagent_fd].ip, 0);
		return;
	}

	//data receive
	buf_ptr		= R_recv_fd[dbagent_fd].recv_buf;
	
	//recv data
	if( (n = recv(dbagent_fd,
			R_recv_fd[dbagent_fd].recv_buf+R_recv_fd[dbagent_fd].recv_size, APP_BUF_SIZE-R_recv_fd[dbagent_fd].recv_size, 0)) < 1 )
	{	
		if( n )
			U_Printf("[RECV]Error dbagent fd(%d), idx(%d), recv() : %s\n",
					dbagent_fd, R_recv_fd[dbagent_fd].dbagent_idx, strerror(errno));
		else
			U_Printf("[RECV]FIN dbagent fd(%d), idx(%d)\n", dbagent_fd, R_recv_fd[dbagent_fd].dbagent_idx);
			
		R_del_fd(dbagent_fd);
		return;
	}
		
	//set recv_sec, recv_size
	R_recv_fd[dbagent_fd].recv_sec	= G_Sec;
	R_recv_fd[dbagent_fd].recv_size	+= n;
	
	//proc buf
	while( R_recv_fd[dbagent_fd].recv_size >= SIZEOF_HEADER_SPECIAL )	
	{	
		header	= (struct header_special *)buf_ptr;

		//check body_size
		if( header->body_size > SIZEOF_MAX_SERV_PACKET_BODY )
		{
			U_Printf("[RECV]Error dbagent fd(%d), idx(%d), bad header.body_size(%d)\n",
					dbagent_fd, R_recv_fd[dbagent_fd].dbagent_idx, header->body_size);
			R_del_fd(dbagent_fd);
			return;			
		} 

		//check command 
		if( ((header->command) & 0xff000000 ) !=  _AW )
		{
			U_Printf("[RECV]Error dbagent fd(%d), idx(%d), bad header.command(%x)\n",
					dbagent_fd, R_recv_fd[dbagent_fd].dbagent_idx, header->command);
			R_del_fd(dbagent_fd);
			return;			
		}

		//check packet
		if( (header->body_size + SIZEOF_HEADER_SPECIAL) > R_recv_fd[dbagent_fd].recv_size )
		{
			memmove(R_recv_fd[dbagent_fd].recv_buf, buf_ptr, R_recv_fd[dbagent_fd].recv_size);
			return;
		}
			
		//complete packet
		U_Recv_queue_in(dbagent_fd, header->fd, header->command
				, header->sec_active, header->session, header->body_size, buf_ptr+SIZEOF_HEADER_SPECIAL, 0);
	
		//set recv_size
		R_recv_fd[dbagent_fd].recv_size	-= (header->body_size + SIZEOF_HEADER_SPECIAL);
		buf_ptr				+= (header->body_size + SIZEOF_HEADER_SPECIAL);
	}

	memmove(R_recv_fd[dbagent_fd].recv_buf, buf_ptr, R_recv_fd[dbagent_fd].recv_size);
}


static	int	R_connect_billing(const int billing_idx)
{
	int			sock_fd, flags, n;
	struct	sockaddr_in	addr;


	//create socket
	if( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		U_Printf("[RECV]Error socket() : %s\n", strerror(errno));
		return	(-1);
	}
		
	flags	= fcntl(sock_fd, F_GETFL, 0);
	fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK);

	//set socket buf
	n	= SOCKET_BUF_SIZE;
	if( setsockopt(sock_fd, SOL_SOCKET, SO_RCVBUF, &n , 4) < 0 )
	{
		U_Printf("[RECV]Error setsockopt() : %s\n", strerror(errno));
    		close(sock_fd);
		return	(-1);
	}

	n	= SOCKET_BUF_SIZE;
	if( setsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, &n , 4) < 0 )
	{
		U_Printf("[RECV]Error setsockopt() : %s\n", strerror(errno));
    		close(sock_fd);
    		return	(-1);
	}
	
	//set addr
	bzero(&addr, sizeof(struct sockaddr_in));
 	addr.sin_family	= AF_INET;
	addr.sin_port	= htons(BILLING_PORT);
		
	if( inet_pton(AF_INET, G_Billing_ip, &(addr.sin_addr)) <= 0 )
	{
		U_Printf("[RECV]Error inet_pton(%s)\n", G_Billing_ip);
    		close(sock_fd);
    		return	(-1);
	}
	
	//connect
	if( (n = connect(sock_fd, (struct sockaddr *)&(addr), sizeof(struct sockaddr_in))) )
	{
		if( errno != EINPROGRESS )
		{
			U_Printf("[RECV]Error connect() : %s\n", strerror(errno));
			close(sock_fd);
    			return	(-1);
		}
	}
	
	if( n == 0 )
	{
		U_Printf("[RECV]Error connect success in local\n");
		close(sock_fd);
		return	(-1);
	}
	
	//epoll add		
	R_event.events	= EPOLLIN|EPOLLERR|EPOLLHUP|EPOLLOUT;
	R_event.data.fd	= sock_fd;
	
	if( epoll_ctl(G_Epoll_handler, EPOLL_CTL_ADD, sock_fd, &R_event) < 0 )
	{
		U_Printf("[RECV]Error epoll_ctl() : %s\n", strerror(errno));
		close(sock_fd);
		return	(-1);
	}

	//set R_billing_fd
	R_billing_fd[billing_idx]	= sock_fd;
	
	//set billing_idx, epoll_add, recv_sec, connecting, ip
	R_recv_fd[sock_fd].connecting	= 1;
	R_recv_fd[sock_fd].billing_idx	= billing_idx;
	R_recv_fd[sock_fd].epoll_add	= 1;
	R_recv_fd[sock_fd].recv_sec	= G_Sec;
	strncpy(R_recv_fd[sock_fd].ip, G_Billing_ip, SIZEOF_IP+1);

	++G_Connection_number;
	return	(0);
}		


static	void	R_recv_billing(const int billing_fd)
{
	/*
	int			n, opterror, optlen;
	struct	header_billing	*header;
	char			*buf_ptr;
	
	//recv first event					
	if( R_recv_fd[billing_fd].connecting )
	{
		opterror	= 0;
		optlen		= SIZEOF_INT;
					
		if( getsockopt(billing_fd, SOL_SOCKET, SO_ERROR, &opterror, &optlen) < 0 )
		{
			U_Printf("[RECV]Error billing fd(%d), idx(%d), getsockopt() : %s\n",
					billing_fd, R_recv_fd[billing_fd].billing_idx, strerror(errno));
			R_close_fd(billing_fd);	
			return;		
		}

		if( opterror )
		{
			U_Printf("[RECV]Error billing fd(%d), idx(%d), opterror : %s\n",
					billing_fd, R_recv_fd[billing_fd].billing_idx, strerror(opterror));
			R_close_fd(billing_fd);
			return;			
		}
	
		U_Printf("[RECV]success connect billing fd(%d), idx(%d)\n", billing_fd, R_recv_fd[billing_fd].billing_idx);
						
		R_event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
		R_event.data.fd	= billing_fd;
				
		if( epoll_ctl(G_Epoll_handler, EPOLL_CTL_MOD, billing_fd, &R_event) < 0 )
			U_Printf("[RECV]Error EPOLL_CTL_MOD epoll_ctl() : %s\n", strerror(errno)),exit(-1);
			
		//set connecting, recv_sec 		
		R_recv_fd[billing_fd].connecting	= 0;
		R_recv_fd[billing_fd].recv_sec		= G_Sec;

		U_Recv_queue_in(billing_fd, R_recv_fd[billing_fd].billing_idx,
						INTER_CONN_BILLING_FD, 0, 0, SIZEOF_IP+1, R_recv_fd[billing_fd].ip, 0);
		return;
	}

	//data receive
	buf_ptr		= R_recv_fd[billing_fd].recv_buf;
	
	//recv data
	if( (n = recv(billing_fd,
			R_recv_fd[billing_fd].recv_buf+R_recv_fd[billing_fd].recv_size, APP_BUF_SIZE-R_recv_fd[billing_fd].recv_size, 0)) < 1 )
	{	
		if( n )
			U_Printf("[RECV]Error billing fd(%d), idx(%d), recv() : %s\n",
					billing_fd, R_recv_fd[billing_fd].billing_idx, strerror(errno));
		else
			U_Printf("[RECV]FIN billing fd(%d), idx(%d)\n", billing_fd, R_recv_fd[billing_fd].billing_idx);
			
		R_del_fd(billing_fd);
		return;
	}
		
	//set recv_sec, recv_size
	R_recv_fd[billing_fd].recv_sec	= G_Sec;
	R_recv_fd[billing_fd].recv_size	+= n;
	
	//proc buf
	while( R_recv_fd[billing_fd].recv_size >= SIZEOF_HEADER_BILLING )	
	{	
		header	= (struct header_billing *)buf_ptr;

		//check total_size
		if( (header->total_size < SIZEOF_HEADER_BILLING) || (header->total_size > SIZEOF_MAX_SERV_PACKET_BODY) )
		{
			U_Printf("[RECV]Error billing fd(%d), idx(%d), bad header.total_size(%d)\n",
					billing_fd, R_recv_fd[billing_fd].billing_idx, header->total_size);
			R_del_fd(billing_fd);
			return;			
		} 

		//check packet
		if( header->total_size > R_recv_fd[billing_fd].recv_size )
		{
			memmove(R_recv_fd[billing_fd].recv_buf, buf_ptr, R_recv_fd[billing_fd].recv_size);
			return;
		}
			
		//complete packet
		C_Billing_decrypt(buf_ptr+4, buf_ptr+4, header->total_size-4);
		U_Recv_queue_in(billing_fd, 0, header->command, 0, 0, header->total_size - SIZEOF_HEADER_BILLING, buf_ptr+SIZEOF_HEADER_BILLING, 0);
	
		//set recv_size
		R_recv_fd[billing_fd].recv_size	-= (header->total_size);
		buf_ptr				+= (header->total_size);
	}

	memmove(R_recv_fd[billing_fd].recv_buf, buf_ptr, R_recv_fd[billing_fd].recv_size);
*/
}
