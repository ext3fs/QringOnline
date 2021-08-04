/*____________________________________________________________

	Project name : Qring Online
	File name    : util.c
	Description  : utility
	Date         : February 14 2005 
_____________________________________________________________*/

#include	"global.h"
#include	"wrap.h"
#define	_UTIL_H_
#include	"util.h"


static	int	U_log(const char *);


static	pthread_mutex_t			U_recv_mutex;
static	pthread_cond_t			U_recv_cond;
static	volatile	sig_atomic_t	U_recv_head;
static	volatile	sig_atomic_t	U_recv_tail;
static	struct	recv_q			U_recv_queue[QUEUE_CNT];

static	pthread_mutex_t			U_send_mutex;
static	pthread_cond_t			U_send_cond;
static	volatile	sig_atomic_t	U_send_head;
static	volatile	sig_atomic_t	U_send_tail;
static	struct	send_q			U_send_queue[QUEUE_CNT];

static	volatile	sig_atomic_t	U_curr_rand_idx;
static	struct	rand_val		U_rand[RAND_CNT];



//____________________________________________________________
//queue
void	U_Init_queue(void)
{
	int	i;
	

	//recv_q
	W_Pthread_mutex_init(&U_recv_mutex, NULL);
	W_Pthread_cond_init(&U_recv_cond, NULL);
	U_recv_head	= 0;
	U_recv_tail	= 0;
	for( i=0; i<(QUEUE_CNT-1); i++ )
		U_recv_queue[i].next	= i+1;
	U_recv_queue[i].next	= 0;

	//send_q
	W_Pthread_mutex_init(&(U_send_mutex), NULL);		
	W_Pthread_cond_init(&(U_send_cond), NULL);
	U_send_head	= 0;
	U_send_tail	= 0;
	for( i=0; i<(QUEUE_CNT-1); i++ )
		U_send_queue[i].next	= i+1;
	U_send_queue[i].next	= 0;
}


void	U_Recv_queue_in(const int fd, const int fd_or_seq, const int command, const int sec_active, const int session,
			const int data_size, const char *data, const int recv_cnt)
{
	int	temp_tail;


	while( 1 )
	{	
		pthread_mutex_lock(&U_recv_mutex);
		if( (temp_tail = U_recv_queue[U_recv_tail].next) == U_recv_head )
		{
			U_Printf("[UTIL]recv queue Full\n");
			pthread_mutex_unlock(&U_recv_mutex);
			continue;
		}

		break;
	}
	
	U_recv_queue[temp_tail].fd		= fd;
	U_recv_queue[temp_tail].fd_or_seq	= fd_or_seq;
	U_recv_queue[temp_tail].command		= command;
	U_recv_queue[temp_tail].sec_active	= sec_active;
	U_recv_queue[temp_tail].session		= session;
	U_recv_queue[temp_tail].data_size	= data_size;
	memcpy(U_recv_queue[temp_tail].data, data, data_size);
	U_recv_queue[temp_tail].recv_cnt	= recv_cnt;
	
	U_recv_tail = temp_tail;
	pthread_cond_signal(&U_recv_cond);
	pthread_mutex_unlock(&U_recv_mutex);
}


struct	recv_q	*U_Recv_queue_wait(void)
{
	pthread_mutex_lock(&U_recv_mutex);
	while( U_recv_head == U_recv_tail )
		pthread_cond_wait(&U_recv_cond, &U_recv_mutex);
	pthread_mutex_unlock(&U_recv_mutex);
	
	return	(U_recv_queue + U_recv_queue[U_recv_head].next);
}


void	U_Recv_queue_proc_end(void)
{
	U_recv_head	= U_recv_queue[U_recv_head].next;
}


void	U_Send_queue_in(const int send_type, const int fd, const int map, const int block_x, int data_size, const char *data)
{
	int	temp_tail;


	while( 1 )
	{	
		pthread_mutex_lock(&U_send_mutex);
		if( (temp_tail = U_send_queue[U_send_tail].next) == U_send_head )
		{
			U_Printf("[UTIL]send queue Full\n");
			pthread_mutex_unlock(&U_send_mutex);
			continue;
		}

		break;
	}
	
	U_send_queue[temp_tail].send_type	= send_type;
	U_send_queue[temp_tail].fd		= fd;
	U_send_queue[temp_tail].map		= map;
	U_send_queue[temp_tail].block_x		= block_x;
	U_send_queue[temp_tail].data_size	= data_size;
	memcpy(U_send_queue[temp_tail].data, data, data_size);
	
	U_send_tail = temp_tail;
	pthread_cond_signal(&U_send_cond);
	pthread_mutex_unlock(&U_send_mutex);
}


struct	send_q	*U_Send_queue_wait(void)
{
	pthread_mutex_lock(&U_send_mutex);
	while( U_send_head == U_send_tail )
		pthread_cond_wait(&U_send_cond, &U_send_mutex);
	pthread_mutex_unlock(&U_send_mutex);
	
	return	(U_send_queue + U_send_queue[U_send_head].next);
}


void	U_Send_queue_proc_end(void)
{
	U_send_head	= U_send_queue[U_send_head].next;
}


int	U_Curr_recv_q_command(void)
{
	return	(U_recv_queue[U_recv_queue[U_recv_head].next].command);
}


int	U_GetRecv_head(void)
{
	return	((int)(U_recv_head));
}


int	U_GetRecv_tail(void)
{
	return	((int)(U_recv_tail));
}


int	U_GetSend_head(void)
{
	return	((int)(U_send_head));
}


int	U_GetSend_tail(void)
{
	return	((int)(U_send_tail));
}


//____________________________________________________________________
//max fd
int	U_SetMax_fd(const int max_fd)
{
	struct	rlimit	set_lim;
	
	set_lim.rlim_cur	= max_fd;
	set_lim.rlim_max	= max_fd;

	if( setrlimit(RLIMIT_NOFILE, &set_lim) )
	{
		U_Printf("[UTIL]Error setrlimit() : %s\n", strerror(errno));
		return (-1);
	}	

	return (0);
}


//___________________________________________________________________
//rand
void	U_Init_rand(void)
{
	time_t	now;
	int	i;


	time(&now);
	srand(now);

	for( i=0; i<RAND_CNT; i++ )
		U_rand[i].val	= rand();

	U_curr_rand_idx	= 0;
	
	for( i=0; i<(RAND_CNT-1); i++ )
		U_rand[i].next	= i + 1;
	
	U_rand[i].next	= 0;
}


int	U_GetRand_val(const int max_val)
{
	U_curr_rand_idx	= U_rand[U_curr_rand_idx].next;

	return	( ((float)max_val)*(U_rand[U_curr_rand_idx].val)/(RAND_MAX+1.0) );
}


//____________________________________________________________
//log
static	int	U_log(const char *buf)
{
	char	temp_file_name[128];
	char	temp_write_data[128];
	char 	temp_date_buf[128];
	struct	tm *tm_ptr;
	
	FILE 	*log_fp;


	tm_ptr	= localtime((time_t *)(&(G_Sec)));
	strftime(temp_write_data, 128, "%Y-%m-%d %H:%M:%S\t", tm_ptr);
	strftime(temp_date_buf, 128, "%Y-%m-%d-%H", tm_ptr);
	snprintf(temp_file_name, 128, "./err_log/lobby%d_%s.log", G_Server_code, temp_date_buf);
	
	if ((log_fp = fopen(temp_file_name, "a")) == NULL){ 
		return (-1);
	}

	fprintf(log_fp,"%s%s",temp_write_data, buf);
	fclose(log_fp);
	return (0);
}


void	U_Printf(const char *fmt, ...)
{
	va_list	args;
	char    buf[8192];
	
	va_start(args,fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

	if( G_Demon )
	{
		U_log(buf);
	}
	else
	{
		fflush(stdout);
		fputs(buf,stderr);
		fflush(stderr);
	}
}


//____________________________________________________________
//nonblock
int	U_Nonb_connect(const int sock_fd, const struct sockaddr *addr, const int nsec)
{
	struct	pollfd	client[1];
	int		nready, flags, optlen, opterror;

	
	flags	= fcntl(sock_fd, F_GETFL, 0);
	fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK);

	if( (opterror = connect(sock_fd, addr, sizeof(struct sockaddr_in))) )
	{
		if( errno != EINPROGRESS )
		{
			U_Printf("[UTIL]Error connect() : %s\n", strerror(errno));
			return	(-1);
		}
	}
	
	if( opterror == 0 )
	{
		U_Printf("[UTIL]Error connect success in local\n");
		return	(-1);
	}


	client[0].fd		= sock_fd;
	client[0].events	= POLLRDNORM | POLLWRNORM;

	if( (nready = poll(client, 1, nsec? (1000*nsec) : -1)) < 0 )
	{
		U_Printf("[UTIL]Error poll() : %s\n", strerror(errno));
		return	(-1);
	}

	if( nready == 0 )
	{
		U_Printf("[UTIL]Error connect() %dsec timeout\n", nsec);
		return	(-1);
	}


	opterror	= 0;
	optlen		= SIZEOF_INT;

	if( getsockopt(sock_fd, SOL_SOCKET, SO_ERROR, &opterror, &optlen) < 0 )
	{
		U_Printf("[UTIL]Error getsockopt() : %s\n", strerror(errno));
		return	(-1);
	}

	if( opterror )
	{
		U_Printf("[UTIL]Error opterror : %s\n", strerror(opterror));
		return	(-1);
	}

	return	(0);		
}


int	U_Nonb_recv(const int sock_fd, char *buf, const int byte, const int nsec)
{
	struct	pollfd	client[1];
	int		nready, nleft, nrecv, flags;
	char		*ptr;


	ptr	= buf;
	nleft	= byte;
	
	flags	= fcntl(sock_fd, F_GETFL, 0);
	fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK);
	
	client[0].fd		= sock_fd;
	client[0].events	= POLLRDNORM;

	while( nleft > 0)
	{
		if( (nready = poll(client, 1, nsec? (1000*nsec) : -1)) < 0 )
		{
			U_Printf("[UTIL]Error epoll_wait() : %s\n", strerror(errno));
  	  		return	(-1);
		}

		if( nready == 0 )
		{
			U_Printf("[UTIL]Error recv() %dsec timeout\n", nsec);
  	  		return	(-1);
		}

		if( (nrecv = recv(sock_fd, ptr, nleft, 0)) < 0 )
		{
			U_Printf("[UTIL]Error recv() : %s\n", strerror(errno));
  	  		return	(-1);
		}
		else if( nrecv == 0 )
		{
			U_Printf("[UTIL]recv FIN\n");
  	  		return (-1);
		}

		nleft	-= nrecv;
		ptr	+= nrecv;
	}

  	return	(0);
}	


int	U_Nonb_send(const int sock_fd, char *buf, const int byte, const int nsec)
{
	struct	pollfd	client[1];
	int		nready, nleft, nsend, flags;
	char		*ptr;


	ptr	= buf;
	nleft	= byte;

	flags	= fcntl(sock_fd, F_GETFL, 0);
	fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK);
	
	client[0].fd		= sock_fd;
	client[0].events	= POLLWRNORM;

	while( nleft > 0)
	{
		if( (nready = poll(client, 1, nsec? (1000*nsec) : -1)) < 0 )
		{
			U_Printf("[UTIL]Error epoll_wait() : %s\n", strerror(errno));
			return	(-1);
		}

		if( nready == 0 )
		{
			U_Printf("[UTIL]Error recv() %dsec timeout\n", nsec);
			return	(-1);
		}

		if( (nsend = send(sock_fd, ptr, nleft, 0)) < 0 )
		{
			U_Printf("[UTIL]Error send() : %s\n", strerror(errno));
			return (-1);
		}

		nleft	-= nsend;
		ptr	+= nsend;
	}
    	
	return (0);
}	


//____________________________________________________________
//string
#define	iseuc_kr(c)	((0xa1<=(unsigned char)(c) && (unsigned char)(c)<=0xfe))


static	int	U_ismbchar_euc_kr(const char *p, const char *e)
{
         return ((*(unsigned char *)(p)<0x80)? 0:iseuc_kr(*(p)) && (e)-(p)>1 && iseuc_kr(*((p)+1))? 2:0);
}


int	U_Check_string(char *string, const int length)
{
        char	*end;
        int	len;

	
	if( *string == 0 )
		return	(-1);

	for( end=string+length; (*string) && (string!=end) ; string++ )
	{
		if( (len = U_ismbchar_euc_kr(string, end)) )
		{
			while( --len )
				++string;
			continue;
		}

		if( (*string < '0') || ((*string > '9') && (*string < 'A')) || ((*string > 'Z') && (*string < 'a')) || (*string > 'z') )
			return	(-1);
	}

	return	(0);
}
