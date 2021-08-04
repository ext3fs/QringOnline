/*____________________________________________________________

	Project name : Qring Online
	File name    : main.c
	Description  : main function
	Date         : February 14 2005 ( for shadowcat. )
_____________________________________________________________*/


#include	"global.h"
#include	"wrap.h"
#include	"util.h"
#include	"db.h"
#include	"mk_packet.h"
#include	"recv.h"
#include	"proc.h"
#include	"tick.h"
#include	"tick_char.h"
#include	"tick_mob.h"
#include	"tick_item.h"
#include	"buff.h"
#include	"send.h"
#include	"crypt.h"


static	int	M_ini_load(const char *);
static	int	M_init_sock(void);
static	int	M_all_logout(void);
static	int	M_server_release(void);
static	void	M_signal_segv(int);
static	void	M_signal_fpe(int);


pid_t				G_Pid;
volatile	sig_atomic_t	G_Run;
volatile	time_t		G_Sec;
int				G_Game;
int				G_World;
int				G_Machine;
int				G_Server_code;
int				G_Demon;
char				G_Script_ip[SIZEOF_IP+1];
char				G_Billing_ip[SIZEOF_IP+1];
char				G_Login_dbagent_ip[SIZEOF_IP+1];
int				G_Epoll_handler;
int				G_Unix_close_fd[2];
int				G_Unix_live_fd[2];
int				G_Unix_dbagent_idx[2];
int				G_Unix_billing_idx[2];
int				G_Unix_login_dbagent_idx[2];
int				G_Unix_tick[2];
int				G_Udp_fd;
int				G_Listen_game_fd;
int				G_Listen_mob_fd;
volatile	int		G_Connection_number;
volatile	int		G_Login_number;
struct	send_buf		G_Send_buf[MAX_FD+1];
void				*G_Buff_memory;
void				*G_Tick_mob_memory;



//____________________________________________________________
//
int	main(int argc, char **argv)
{
	int			i, signo;
	char			buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	sigset_t		blockmask, waitmask;
	struct	sigaction	segv_act, fpe_act;
	struct	sockaddr_in	addr;
	pthread_t		tids[8];


	//set G_Sec
	G_Sec	= time(NULL);

	//init G_Send_buf
	for( i=0; i<=MAX_FD; i++ )
		W_Pthread_mutex_init(&(G_Send_buf[i].mutex), NULL);

	//init queue
	U_Init_queue();

	//init rand
	U_Init_rand();

	//set max_fd
	if( U_SetMax_fd(MAX_FD) )
	{
		U_Printf("[MAIN]Error U_SetMax_fd()\n");
		return	(-1);
	}
	
	//read server.ini
	if( M_ini_load("server.ini") )	
	{
		U_Printf("[MAIN]Error M_ini_load()\n");
		return	(-1);
	}
	
	//init db
	if( D_SetDb_init() )
	{
		U_Printf("[MAIN]Error D_SetDb_init()\n");
		return	(-1);
	}

	//init socket
	if( M_init_sock() )
	{
		U_Printf("[MAIN]Error M_init_sock()\n");
		return	(-1);
	}
	
	//logout
	if( M_all_logout() )
	{
		U_Printf("[MAIN]Error M_all_logout()\n");
		return	(-1);
	}

	U_Printf("-----------------------------------------------------------\n");
	U_Printf("\t# Game Qring : %d - %s #\n", G_Server_code, VERSION);
	sprintf(buf,"%s",ctime((time_t *)(&G_Sec)));
	U_Printf("\t%s",buf);
	U_Printf("\tGame Port = %d\n", GAME_PORT);
	U_Printf("------------------------------------------------------------\n");
	
	while ( (i = getopt(argc, argv,"d")) != -1 )
	{
		switch(i)
		{
			case 'd':
		       		G_Demon = 1;	
		        	break;
				
			case '?' :
				exit(-1);
		}
	}

	//demon 
	if( G_Demon  )
	{
		if( W_Fork() )
		 	exit(0);
								         
		setsid();
		W_Signal(SIGHUP, SIG_IGN);
																         
		if( W_Fork() )
                    	exit(0);

		i = W_Open("/dev/null", O_RDWR | O_CREAT | O_TRUNC, 0);
		
		W_Dup2(i, 0);
		W_Dup2(i, 1);
		W_Dup2(i, 2);
		
		close(i);
		
		U_Printf("\t[MAIN]Game %d Start !! ( Demon mode ) \n", G_Server_code);
	}


	//signal
	G_Pid	= getpid();
	
	W_Signal(SIGPIPE, SIG_IGN);
	
	segv_act.sa_handler	= M_signal_segv;
	sigfillset(&segv_act.sa_mask);
	segv_act.sa_flags	= SA_RESTART;
	W_Sigaction(SIGSEGV, &segv_act, NULL);
	
	fpe_act.sa_handler	= M_signal_fpe;
	sigfillset(&fpe_act.sa_mask);
	fpe_act.sa_flags	= SA_RESTART;
	W_Sigaction(SIGFPE, &fpe_act, NULL);
	
	sigfillset(&blockmask);
	sigdelset(&blockmask, SIGSEGV);
	sigdelset(&blockmask, SIGFPE);
	
	W_Pthread_sigmask(SIG_BLOCK, &blockmask, NULL);


	//thread
	G_Run	= 1;
	i	= 0;

	W_Pthread_create(tids+i, NULL, T_Tick_main, NULL),		i++;
	W_Pthread_create(tids+i, NULL, TC_Tick_char_main, NULL),	i++;
	W_Pthread_create(tids+i, NULL, TM_Tick_mob_main, NULL), 	i++;
	W_Pthread_create(tids+i, NULL, TI_Tick_item_main, NULL), 	i++;
	W_Pthread_create(tids+i, NULL, S_Send_main, NULL), 		i++;
	W_Pthread_create(tids+i, NULL, B_Buff_main, NULL), 		i++;
	W_Pthread_create(tids+i, NULL, P_Proc_main, NULL), 		i++;
	W_Pthread_create(tids+i, NULL, R_Recv_main, NULL), 		i++;

	
	//wait 
	sigemptyset(&waitmask);
	sigaddset(&waitmask, SIGINT);
	sigaddset(&waitmask, SIGTERM);
	
	sigwait(&waitmask, &signo);


	//wake up
	G_Run	= 0;
	U_Printf("\t[MAIN]Wake Up(signo:%d)!!\n", signo);

	bzero(&addr, sizeof(addr));
  	addr.sin_family	= AF_INET;
  	addr.sin_port 	= htons(MOB_PORT);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	
	MK_GetPacket_TW_WAKEUP(buf);
	sendto(G_Udp_fd, buf, SIZEOF_HEADER_NORMAL, 0, (struct sockaddr *)&addr, sizeof(addr));

	while( i-- )
		W_Pthread_join(tids[i], NULL);
	
	if( M_server_release()  )
		U_Printf("[MAIN]Error M_server_release()\n");

	U_Printf("\t[MAIN]Game %d Shutdown\n", G_Server_code);
	return	(0);
}



//____________________________________________________________
//
static	int	M_ini_load(const char *file_name)
{
	FILE	*fp;
	char	line[8192], *ptr;


	if( (fp = fopen(file_name, "r")) == NULL )
	{
		U_Printf("[MAIN]Error fopen(%s) : %s\n", file_name, strerror(errno));
		return	(-1);
	}

	line[8191]	= 0;	

	while( fgets(line, 8192, fp) )
	{
		if( line[8191] )
		{
			U_Printf("[MAIN]Error too Long line\n");
			fclose(fp);
			return	(-1);
		}
		
		if( (line[0] == '#') || (line[0] == '\n') )
			continue;
		
		if( (ptr = strstr(line, "=")) == NULL )
		{
			U_Printf("[MAIN]Error bad line(%s)\n", line);
			fclose(fp);
			return	(-1);
		}
	
		line[strlen(line)-1]=0;
		*ptr = 0;
		
		if( strcmp("machine", line) == 0 )
		{
			G_Machine = strtoul(ptr+1, (char **)NULL, 10);
			continue;
		}
		
		if( strcmp("script_ip", line) == 0 )
		{
			strncpy(G_Script_ip, ptr+1, SIZEOF_IP);	
			continue;
		}

		if( strcmp("billing_ip", line) == 0 )
		{
			strncpy(G_Billing_ip, ptr+1, SIZEOF_IP);	
			continue;
		}
		
		if( strcmp("login_dbagent_ip", line) == 0 )
		{
			strncpy(G_Login_dbagent_ip, ptr+1, SIZEOF_IP);	
			continue;
		}
	}
	
	if( (G_Machine < 1) || (G_Machine >= 999) )
	{
		U_Printf("[MAIN]Error bad G_Machine(%d)\n", G_Machine);
		return	(-1);
	}

	fclose(fp);
	return	(0);
}			


static	int	M_init_sock(void)
{
	int			on, flags;
	struct	epoll_event	event;
	struct	sockaddr_in	addr;


	//G_Epoll_handle
	if( (G_Epoll_handler = epoll_create( MAX_FD )) < 0 )
	{
		U_Printf("[MAIN]Error epoll_create() : %s\n", strerror(errno));
  	  	return	(-1);
	}

	//G_Unix_close_fd
	if( socketpair(AF_LOCAL, SOCK_STREAM, 0, G_Unix_close_fd) < 0 )
	{
		U_Printf("[MAIN]Error socketpair() : %s\n", strerror(errno));
    		return	(-1);
	}

	event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
	event.data.fd	= G_Unix_close_fd[0];

	if( epoll_ctl(G_Epoll_handler, EPOLL_CTL_ADD, G_Unix_close_fd[0], &event) < 0 )
	{
		U_Printf("[MAIN]Error epoll_ctl() : %s\n", strerror(errno));
  	  	return	(-1);
	}
	
	// G_Unix_live_fd
	if( socketpair(AF_LOCAL, SOCK_STREAM, 0, G_Unix_live_fd) < 0 )
	{
		U_Printf("[MAIN]Error socketpair() : %s\n", strerror(errno));
   	 	return	(-1);
	}	

	event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
	event.data.fd	= G_Unix_live_fd[0];
	
	if( epoll_ctl(G_Epoll_handler, EPOLL_CTL_ADD, G_Unix_live_fd[0], &event) < 0 )
	{
		U_Printf("[MAIN]Error epoll_ctl() : %s\n", strerror(errno));
  	  	return	(-1);
	}

	//G_Unix_dbagent_idx
	if( socketpair(AF_LOCAL, SOCK_STREAM, 0, G_Unix_dbagent_idx) < 0 )
	{
		U_Printf("[MAIN]Error socketpair() : %s\n", strerror(errno));
   	 	return	(-1);
	}	

	event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
	event.data.fd	= G_Unix_dbagent_idx[0];
	
	if( epoll_ctl(G_Epoll_handler, EPOLL_CTL_ADD, G_Unix_dbagent_idx[0], &event) < 0 )
	{
		U_Printf("[MAIN]Error epoll_ctl() : %s\n", strerror(errno));
  	  	return	(-1);
	}
	
	//G_Unix_billing_idx
	if( socketpair(AF_LOCAL, SOCK_STREAM, 0, G_Unix_billing_idx) < 0 )
	{
		U_Printf("[MAIN]Error socketpair() : %s\n", strerror(errno));
   	 	return	(-1);
	}	

	event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
	event.data.fd	= G_Unix_billing_idx[0];
	
	if( epoll_ctl(G_Epoll_handler, EPOLL_CTL_ADD, G_Unix_billing_idx[0], &event) < 0 )
	{
		U_Printf("[MAIN]Error epoll_ctl() : %s\n", strerror(errno));
  	  	return	(-1);
	}

	//G_Unix_login_dbagent_idx
	if( socketpair(AF_LOCAL, SOCK_STREAM, 0, G_Unix_login_dbagent_idx) < 0 )
	{
		U_Printf("[MAIN]Error socketpair() : %s\n", strerror(errno));
   	 	return	(-1);
	}	

	event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
	event.data.fd	= G_Unix_login_dbagent_idx[0];
	
	if( epoll_ctl(G_Epoll_handler, EPOLL_CTL_ADD, G_Unix_login_dbagent_idx[0], &event) < 0 )
	{
		U_Printf("[MAIN]Error epoll_ctl() : %s\n", strerror(errno));
  	  	return	(-1);
	}
	
	//G_Unix_tick
	if( socketpair(AF_LOCAL, SOCK_STREAM, 0, G_Unix_tick) < 0 )
	{
		U_Printf("[MAIN]Error socketpair() : %s\n", strerror(errno));
   	 	return	(-1);
	}	

	event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
	event.data.fd	= G_Unix_tick[0];
	
	if( epoll_ctl(G_Epoll_handler, EPOLL_CTL_ADD, G_Unix_tick[0], &event) < 0 )
	{
		U_Printf("[MAIN]Error epoll_ctl() : %s\n", strerror(errno));
  	  	return	(-1);
	}

	//G_Udp_fd	
	if( (G_Udp_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
		U_Printf("[MAIN]Error socket(): %s\n", strerror(errno));
   	 	return (-1);
	}
	
	bzero(&addr, sizeof(addr));
  	addr.sin_family	= AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
  	addr.sin_port 	= htons(MOB_PORT);
		
  	if( bind(G_Udp_fd,(struct sockaddr *)&addr, sizeof(addr)) < 0 )
	{
		U_Printf("[MAIN]Error bind() : %s\n", strerror(errno));
		return	(-1);
	}

	event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
	event.data.fd	= G_Udp_fd;
	
	if( epoll_ctl(G_Epoll_handler, EPOLL_CTL_ADD, G_Udp_fd, &event) < 0 )
	{
		U_Printf("[MAIN]Error epoll_ctl() : %s\n", strerror(errno));
  	  	return	(-1);
	}
	
	//G_Listen_mob_fd
	if( (G_Listen_mob_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		U_Printf("[MAIN]Error socket() : %s\n", strerror(errno));
    		return	(-1);
	}
	
	on	= 1;
	if( setsockopt(G_Listen_mob_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0 )
	{
		U_Printf("[MAIN]Error setsockopt() : %s\n", strerror(errno));
    		return	(-1);
	}

	on	= SOCKET_BUF_SIZE;
	if( setsockopt(G_Listen_mob_fd, SOL_SOCKET, SO_RCVBUF, &on , 4) < 0 )
	{
		U_Printf("[MAIN]Error setsockopt() : %s\n", strerror(errno));
		return	(-1);
	}

	on	= SOCKET_BUF_SIZE;
	if( setsockopt(G_Listen_mob_fd, SOL_SOCKET, SO_SNDBUF, &on , 4) < 0 )
	{
		U_Printf("[MAIN]Error setsockopt() : %s\n", strerror(errno));
    		return	(-1);
	}

	flags	= fcntl(G_Listen_mob_fd, F_GETFL, 0);
	fcntl(G_Listen_mob_fd, F_SETFL, flags | O_NONBLOCK);
			
	bzero(&addr, sizeof(addr));
  	addr.sin_family		= AF_INET;
	addr.sin_addr.s_addr	= htonl(INADDR_ANY);
  	addr.sin_port 		= htons(MOB_PORT);
  	  	   
  	if( bind(G_Listen_mob_fd,(struct sockaddr *)&addr, sizeof(addr)) < 0 )
	{
		U_Printf("[MAIN]Error bind() : %s\n", strerror(errno));
   	 	return	(-1);
	}
        		
  	if( listen(G_Listen_mob_fd, 1024) < 0 )
	{
		U_Printf("[MAIN]Error listen() : %s\n", strerror(errno));
  	  	return	(-1);
	}
	
	event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
	event.data.fd	= G_Listen_mob_fd;
	
	if( epoll_ctl(G_Epoll_handler, EPOLL_CTL_ADD, G_Listen_mob_fd, &event) < 0 )
	{
		U_Printf("[MAIN]Error epoll_ctl() : %s\n", strerror(errno));
		return	(-1);
	}


	//G_Listen_game_fd
	if( (G_Listen_game_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		U_Printf("[MAIN]Error socket() : %s\n", strerror(errno));
    		return	(-1);
	}
	
	on	= 1;
	if( setsockopt(G_Listen_game_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0 )
	{
		U_Printf("[MAIN]Error setsockopt() : %s\n", strerror(errno));
    		return	(-1);
	}

	on	= SOCKET_BUF_SIZE;
	if( setsockopt(G_Listen_game_fd, SOL_SOCKET, SO_RCVBUF, &on , 4) < 0 )
	{
		U_Printf("[MAIN]Error setsockopt() : %s\n", strerror(errno));
		return	(-1);
	}

	on	= SOCKET_BUF_SIZE;
	if( setsockopt(G_Listen_game_fd, SOL_SOCKET, SO_SNDBUF, &on , 4) < 0 )
	{
		U_Printf("[MAIN]Error setsockopt() : %s\n", strerror(errno));
    		return	(-1);
	}

	flags	= fcntl(G_Listen_game_fd, F_GETFL, 0);
	fcntl(G_Listen_game_fd, F_SETFL, flags | O_NONBLOCK);
			
	bzero(&addr, sizeof(addr));
  	addr.sin_family		= AF_INET;
	addr.sin_addr.s_addr	= htonl(INADDR_ANY);
  	addr.sin_port 		= htons(GAME_PORT);
  	  	   
  	if( bind(G_Listen_game_fd,(struct sockaddr *)&addr, sizeof(addr)) < 0 )
	{
		U_Printf("[MAIN]Error bind() : %s\n", strerror(errno));
   	 	return	(-1);
	}
        		
  	if( listen(G_Listen_game_fd, 1024) < 0 )
	{
		U_Printf("[MAIN]Error listen() : %s\n", strerror(errno));
  	  	return	(-1);
	}

	event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
	event.data.fd	= G_Listen_game_fd;
	
	if( epoll_ctl(G_Epoll_handler, EPOLL_CTL_ADD, G_Listen_game_fd, &event) < 0 )
	{
		U_Printf("[MAIN]Error epoll_ctl() : %s\n", strerror(errno));
		return	(-1);
	}

	U_Printf("\t[MAIN]Socket Init(G_Listen_game_fd:%d)\n", G_Listen_game_fd);
	return	(0);
}


static	int	M_all_logout(void)
{
	int				sock_fd, n;
	char				buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	struct	sockaddr_in		addr;
	struct	body_AW_INIT_RESULT	body;	


	//connect login agent
	bzero(&addr, sizeof(addr));
  	addr.sin_family	= AF_INET;
  	addr.sin_port	= htons(DBAGENT_PORT);
	inet_pton(AF_INET, G_Login_dbagent_ip, &addr.sin_addr);

	if( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		U_Printf("[MAIN]Error socket() : %s\n", strerror(errno));
		return	(-1);
	}

	if( U_Nonb_connect(sock_fd, (struct sockaddr *)&addr, 3) < 0 )
	{
		U_Printf("[MAIN]Error U_Nonb_connect()\n");
		return	(-1);
	}

	//send WA_INIT
	n	= MK_GetPacket_WA_INIT(buf, G_Server_code);

	if( U_Nonb_send(sock_fd, buf, n, 3) )
	{
		U_Printf("[MAIN]Error U_Nonb_send()\n");
		return	(-1);
	}

	//recv header
	if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 10) )
	{
		U_Printf("[MAIN]Error U_Nonb_recv()\n");
		return	(-1);
	}

	//recv body
	if( U_Nonb_recv(sock_fd, (char *)&body, sizeof(struct body_AW_INIT_RESULT), 10) )
	{
		U_Printf("[MAIN]Error U_Nonb_recv()\n");
		return	(-1);
	}

	// check result_code	
	if( body.result_code )
	{
		U_Printf("[MAIN]Error bad result_code(%d)\n", body.result_code);
		return	(-1);
	}

	close(sock_fd);
	return	(0);
}


static	int	M_server_release(void)
{
	int				sock_fd, i, n, ret=0;
	char				buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	struct	sockaddr_in		addr;
	struct	body_AW_INIT_RESULT	body;


	//connect dbagent
	bzero(&addr, sizeof(addr));
  	addr.sin_family	= AF_INET;
  	addr.sin_port	= htons(DBAGENT_PORT);
	inet_pton(AF_INET, D_GetWorld_machine_dbagent_ip(G_World, G_Machine), &addr.sin_addr);

	if( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		U_Printf("[MAIN]Error socket() : %s\n", strerror(errno));
		ret	= -1;
	}
	else if( U_Nonb_connect(sock_fd, (struct sockaddr *)&addr, 3) )
	{
		U_Printf("[MAIN]Error U_Nonb_connect()\n");
		ret	= -1;
	}
	else
	{
		// send	WA_INIT
		n	= MK_GetPacket_WA_INIT(buf, 0);	
	
		if( U_Nonb_send(sock_fd, buf, n, 3) )
		{
			U_Printf("[MAIN]Error U_Nonb_send()\n");
			ret	 = -1;
		}
		else if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
		{
			U_Printf("[MAIN]Error U_Nonb_recv()\n");
			ret	 = -1;
		} 
		else if( U_Nonb_recv(sock_fd, (char *)&body, sizeof(struct body_AW_INIT_RESULT), 3) )
		{
			U_Printf("[MAIN]Error U_Nonb_recv()\n");
			ret	 = -1;
		} 
		else if( body.result_code )
		{ 
			U_Printf("[MAIN]Error bad result_code(%d)\n", body.result_code);
			ret	 = -1;
		}
	}
	
	//save char data	
	for( i=G_Listen_game_fd+1; i<MAX_FD; i++ )
	{
		//char loading
		if( *D_GetClient_char_name(i) )
		{
			n	= MK_GetPacket_WA_CHAR_SAVE(buf, i, NULL, 0);
			U_Print_query(buf);
			
			if( ret == 0 )
			{
				if( U_Nonb_send(sock_fd, buf, n, 3) == 0 )
				{
					U_Printf("[MAIN]send GA_CHAR_SAVE(fd:%d, user_id:%s, char_name:%s)\n",
							i, D_GetClient_user_id(i), D_GetClient_char_name(i));
					continue;
				}
				else
				{
					U_Printf("[MAIN]Error U_Nonb_send()\n");
					ret	= -1;
				}
			}
	
			U_Printf("[MAIN]Error save fd(%d) user_id(%s) char_name(%s)\n", i, D_GetClient_user_id(i), D_GetClient_char_name(i));
		}
	}

	close(sock_fd);

	//logout
	if( M_all_logout() )
	{
		U_Printf("[MAIN]Error M_all_logout()\n");
		ret	= -1;
	}
	
	return	(ret);
}


static	void	M_signal_segv(int sig)
{
	W_Signal(SIGSEGV, SIG_IGN);
	
	U_Printf("[MAIN]recv SIGSEGV, current recv queue command = %x\n", U_Curr_recv_q_command());
	
	if( M_server_release() )
		U_Printf("[MAIN]Error M_server_release()\n");

	W_Signal(SIGSEGV, SIG_DFL);
}


static	void	M_signal_fpe(int sig)
{
	W_Signal(SIGFPE, SIG_IGN);
	
	U_Printf("[MAIN]recv SIGFPE, current recv queue command = %x\n", U_Curr_recv_q_command());
	
	if( M_server_release() )
		U_Printf("[MAIN]Error M_server_release()\n");
	
	W_Signal(SIGFPE, SIG_DFL);
}
