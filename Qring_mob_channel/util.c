
#include 	"global.h"
#include	"db.h"
#include 	"util.h"


struct random_value
{
	volatile unsigned long		random;
	volatile int			next;
};

struct random_queue
{
	volatile int			head;
	struct random_value		*value;
};




// ---------------------------------------------------------------------------
// inner function
static int	U_set_log		( const int, const char * );

static int      U_move_to_section	( FILE *, char * );
static void	U_remove_tail		( char * );
static void	U_remove_head		( char * );





// ---------------------------------------------------------------------------
// inner variable
static struct recv_queue_pool		*U_recv_queue;
static struct random_queue		U_random_queue;





// ---------------------------------------------------------------------------
// queue
int	U_set_queue()
{
	int	i, j;


	if( (U_recv_queue = calloc((G_proc_thr_cnt + 1), sizeof(struct recv_queue_pool))) == NULL )
	{
		dprintf(0, "[UTIL]Error U_set_queue() calloc() : %s \n", strerror(errno) );
		return (-1);
	}

	for( i=1; i<=G_proc_thr_cnt; i++ )
	{
		pthread_mutex_init( &(U_recv_queue[i].locked), NULL );

		U_recv_queue[i].head = 0;
		U_recv_queue[i].tail = 0;

		for( j=0; j<(MAX_QUEUE_CNT - 1); j++ )
			U_recv_queue[i].queue[j].next = (j + 1);

		U_recv_queue[i].queue[j].next = 0;
	}

	dprintf(0, "\t[UTIL]Success U_set_queue(%d)\n", MAX_QUEUE_CNT);
	return (0);
}


int	U_recv_queue_in( const int tidx, const int channel, const int command, const int buf_size, const char *p_buf )
{
	int	temp_tail, isSuccess = 1;


	while( isSuccess )
	{
		pthread_mutex_lock(&U_recv_queue[tidx].locked);

			temp_tail	= U_recv_queue[tidx].tail;

			if( U_recv_queue[tidx].queue[temp_tail].next == U_recv_queue[tidx].head )
			{
				dprintf(0, "[UTIL]Error recv_q full tidx(%d), channel(%d), head(%d), tail(%d) \n"
					, tidx, channel, U_recv_queue[tidx].head, temp_tail);

				pthread_mutex_unlock(&U_recv_queue[tidx].locked);
				return (-1);
				//usleep(100000);
				//continue;
			}

			U_recv_queue[tidx].queue[temp_tail].channel	= channel;
			U_recv_queue[tidx].queue[temp_tail].command	= command;

			U_recv_queue[tidx].queue[temp_tail].buf_size	= buf_size;
			memcpy( U_recv_queue[tidx].queue[temp_tail].buf	, p_buf, buf_size );

			U_recv_queue[tidx].tail	= U_recv_queue[tidx].queue[temp_tail].next;

		pthread_mutex_unlock(&U_recv_queue[tidx].locked);

		isSuccess	= 0;
	}

	//dprintf(0, "[UTIL]U_recv_queue_in() tidx(%d), channel(%d), head(%d), tail(%d), command(%x) \n"
	//	, tidx, channel, U_recv_queue[tidx].head, U_recv_queue[tidx].tail, command );

	return (0);
}


struct recv_q*	U_recv_queue_out( const int tidx )
{
	struct recv_q*	q=NULL;

	if( U_recv_queue[tidx].head == U_recv_queue[tidx].tail )
		return q;

	pthread_mutex_lock(&U_recv_queue[tidx].locked);

		q	= (struct recv_q *)&(U_recv_queue[tidx].queue[U_recv_queue[tidx].head]);		

	pthread_mutex_unlock(&U_recv_queue[tidx].locked);

	//dprintf(0, "[UTIL]U_recv_queue_out() tidx(%d), channel(%d), head(%d), tail(%d), command(%x) \n"
	//	, tidx, q->channel, U_recv_queue[tidx].head, U_recv_queue[tidx].tail, q->command );

	U_recv_queue[tidx].head = q->next;

	return (q);
}


int	U_recv_queue_head( const int tidx )
{
	return (U_recv_queue[tidx].head);
}


int	U_recv_queue_tail( const int tidx )
{
	return (U_recv_queue[tidx].tail);
}






// ---------------------------------------------------------------------------
// network
int	U_ConnectToServer( const int fd, const char *ipaddress, const int port, const int nSec )
{
	int			handler, nEvent, optval, optlen, ret;
	struct sockaddr_in	addr;
	struct epoll_event	event;


	bzero( (char *)&addr, sizeof(addr) );
	inet_aton( ipaddress, &addr.sin_addr );
	addr.sin_family 	= AF_INET;
	addr.sin_port		= htons(port);

	if( (ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr))) < 0 )
	{
		if( errno != EINPROGRESS )
		{
			dprintf(0, "[UTIL]Error connect() : %s \n", strerror(errno) );
			return (-1);
		}
	}

	if( (handler = epoll_create(1)) < 0 )
	{
		dprintf(0, "[UTIL]Error epoll_create() : %s \n", strerror(errno) );
		return (-1);
	}

	event.events 	= EPOLLIN|EPOLLOUT|EPOLLERR|EPOLLHUP;
	event.data.fd	= fd;
	if( epoll_ctl(handler, EPOLL_CTL_ADD, fd, &event) < 0 )
	{
		dprintf(0, "[UTIL]Error epoll_ctl() : %s \n", strerror(errno) );
		close( handler );
		return (-1);
	}

	if( (nEvent = epoll_wait(handler, &event, 1, nSec ? (nSec*1000) : -1)) < 1 )
	{
		dprintf(0, "[UTIL]Error epoll_wait() : %s \n", strerror(errno) );
		close( handler );
		return (-1);
	}

	close( handler );


	optval	= 0;
	optlen	= SIZEOF_INT;
	if( getsockopt(fd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0 )
	{
		dprintf(0, "[UTIL]Error getsockopt() : %s \n", strerror(errno) );
		return (-1);
	}

	if( optval != 0 )
	{
		dprintf(0, "[UTIL]Error connect optval:%d : %s \n", optval, strerror(errno) );
		return (-1);
	}

	return (0);
}


int	U_SendToServer( const int fd, const int nByte, char *pBuf, const int nSec )
{
	struct epoll_event	event, ev[1];
	int			epoll_handler, nEvent, nleft, nsend;
	char                    *ptr;


	if( (epoll_handler = epoll_create(1)) < 0 )
	{
		dprintf(0, "[UTIL]Error epoll_create() : %s\n", strerror(errno));
		return (-1);
	}

	ptr     = pBuf;
	nleft   = nByte;


	event.events    = EPOLLOUT|EPOLLERR|EPOLLHUP;
	event.data.fd   = fd;

	if( epoll_ctl(epoll_handler, EPOLL_CTL_ADD, fd, &event) < 0 )
	{
		dprintf(0, "[UTIL]Error epoll_ctl() : %s\n", strerror(errno));
		close(epoll_handler);
		return (-1);
	}

	while( nleft > 0 )
	{
		if( (nEvent = epoll_wait(epoll_handler, ev, 1, nSec? (1000*nSec) : -1)) < 0 )
		{
			dprintf(0, "[UTIL]Error epoll_wait() : %s\n", strerror(errno));
			close(epoll_handler);
			return (-1);
		}

		if( nEvent == 0 )
		{
			dprintf(0, "[UTIL]Error send() %dsec timeout\n", nSec);
			close(epoll_handler);
			return (-1);
		}

		if( ev[0].events & EPOLLOUT )
		{
			if( (nEvent != 1) || (ev[0].data.fd != fd) )
			{
				dprintf(0, "[UTIL]Error nEvent:%d, ev[0].data.fd:%d\n", nEvent, ev[0].data.fd);
				close(epoll_handler);
				return (-1);
			}

			if( (nsend = send(fd, ptr, nleft, 0)) < 1 )
			{
				dprintf(0, "[UTIL]Error send() : %s\n", strerror(errno));
				close(epoll_handler);
				return (-1);
			}
		}
		else
		{
			dprintf(0, "[UTIL]Error send() : %s \n", strerror(errno) );
			close(epoll_handler);
			return (-1);
		}

		nleft   -= nsend;
		ptr     += nsend;
	}

        close(epoll_handler);
	return (0);
}


int	U_RecvFromServer( const int fd, const int nByte, char *pBuf, const int nSec )
{
	struct epoll_event	event, ev[1];
	int			epoll_handler, nEvent, nleft, nrecv;
	char                    *ptr;


	if( (epoll_handler = epoll_create(1)) < 0 )
	{
		dprintf(0, "[UTIL]Error epoll_create() : %s\n", strerror(errno));
		return (-1);
	}

	ptr     = pBuf;
	nleft   = nByte;


	event.events    = EPOLLIN|EPOLLERR|EPOLLHUP;
	event.data.fd   = fd;

	if( epoll_ctl(epoll_handler, EPOLL_CTL_ADD, fd, &event) < 0 )
	{
		dprintf(0, "[UTIL]Error epoll_ctl() : %s\n", strerror(errno));
		close(epoll_handler);
		return (-1);
	}

	while( nleft > 0 )
	{

		if( (nEvent = epoll_wait(epoll_handler, ev, 1, nSec? (1000*nSec) : -1)) < 0 )
		{
			dprintf(0, "[UTIL]Error epoll_wait() : %s\n", strerror(errno));
			close(epoll_handler);
			return (-1);
		}

		if( nEvent == 0 )
		{
			dprintf(0, "[UTIL]Error recv() %dsec timeout\n", nSec );
			close(epoll_handler);
			return (-1);
		}

		if( ev[0].events & EPOLLIN )
		{
			if( (nEvent != 1) || (ev[0].data.fd != fd) )
			{
				dprintf(0, "[UTIL]Error nEvent:%d, ev[0].data.fd:%d\n", nEvent, ev[0].data.fd);
				close(epoll_handler);
				return (-1);
			}

			if( (nrecv = recv(fd, ptr, nleft, 0)) < 1 )
			{
				dprintf(0, "[UTIL]Error recv() : %s\n", strerror(errno));
				close(epoll_handler);
				return (-1);
			}
		}
		else
		{
			dprintf(0, "[UTIL]Error recv() : %s \n", strerror(errno) );
			close(epoll_handler);
			return (-1);
		}
		
		nleft   -= nrecv;
		ptr     += nrecv;
	}

        close(epoll_handler);
	return (0);
}



// ---------------------------------------------------------------------------
// random data queue
int	U_set_random( const int SIZE )
{
	int		i;


	srand(time(NULL));

	if( (U_random_queue.value = (struct random_value *)calloc(SIZE, sizeof(struct random_value))) < 0 )
	{
		dprintf(0, "[UTIL]Error calloc() : %s \n", strerror(errno) );
		return (-1);
	}

	U_random_queue.head	= 0;

	for( i=0; i<(SIZE - 1); i++ )
	{
		U_random_queue.value[i].random	= rand();	
		U_random_queue.value[i].next	= (i + 1);
	}

	U_random_queue.value[i].random	= rand();	
	U_random_queue.value[i].next	= 0;

	dprintf(0, "\t[UTIL]Success U_set_random(%d) \n", SIZE);
	return (0);
}


int	U_get_random( const int MAX )
{
	int	random;

	if( MAX )
	{
		random = (U_random_queue.value[U_random_queue.head].random) % MAX;
		U_random_queue.head = U_random_queue.value[U_random_queue.head].next;
		return (random);
	}

	return (0);
}




// ---------------------------------------------------------------------------
// log
int     U_set_logdir( void )
{
	int	i;
	char	dir[256];

	for( i=0; i<=TOTAL_CHANNEL_CNT; i++ )
	{
		sprintf(dir, "./error_log/ch%03d", i);

		if( (access(dir, F_OK) == -1) && errno == ENOENT)
			mkdir(dir, 0777);
	}

	return (0);
}


int	dprintf( const int channel,  const char *fmt, ... )
{
	va_list args;
	char    buf[1024];

	va_start( args, fmt );
	vsnprintf( buf, sizeof(buf), fmt, args );
	va_end( args );

	if( G_daemon )
	{
		U_set_log(channel, buf);
	}
	else{
		fflush(stderr);
		fputs(buf,stdout);
		fflush(stdout);
	}

	return (0);
}


static int     U_set_log( const int channel, const char *p_log_data )
{
	char 		temp_file_name  [128];
	char		temp_write_data [128];
	char		temp_date_buf   [128];
	struct tm *	tm_ptr;
	FILE *          log_fp;

	tm_ptr  = localtime((time_t *)&G_Sec);
	strftime( temp_date_buf		, 128	, "%Y-%m-%d-%H"				, tm_ptr);
	//snprintf( temp_file_name	, 128	, "./error_log/ch%03d/mobserv_%s.log"	, channel, temp_date_buf);
	snprintf( temp_file_name	, 128	, "./error_log/mobserv_ch%03d_%s.log"	, channel, temp_date_buf);
	strftime( temp_write_data	, 128	, "[%Y-%m-%d %H:%M:%S]"			, tm_ptr);

	if( (log_fp = fopen(temp_file_name, "a")) == NULL )  
		return (-1);

	fprintf( log_fp, "%s\t%s", temp_write_data, p_log_data );

	fclose(log_fp);
	return (0);
}




// ---------------------------------------------------------------------------
// Process fork()
pid_t	U_pro_fork()
{
	pid_t pid;

	if((pid=fork()) == -1)
	{
		fprintf( stdout, "[UTIL]U_pro_fork() error\n" );
		exit(-1);
	}

	return(pid);
}



// ---------------------------------------------------------------------------
// file access
int	U_get_line( const char *p_file, const int file_size )
{
	int	i, line_cnt = 0;

	for( i=0; i<file_size; i++ )
	{
		if( p_file[i] == '\n' )
			line_cnt ++;
	}

	return (line_cnt);
}


int     U_get_value( char* p_file, char* p_section, char *p_item, char *p_value )
{
	FILE* fp;
	char  tmp_buf[256];
	char  *ptr1, *ptr2;


	if( (fp = fopen(p_file, "r")) == NULL )
	{
		printf("%s's Read Fail !! \n", p_file );
		return (-1);
	}

	if( U_move_to_section( fp, p_section ) < 0 )
	{
		fclose( fp );
		return (-1);
	}

        while( !feof( fp ) )
        {
		if( fgets( tmp_buf, sizeof( tmp_buf ), fp ) == NULL )
			break;

		U_remove_head( tmp_buf );

		if( tmp_buf[0] == '[' )
			break;

		ptr1 = strchr( tmp_buf, ';' );
		ptr2 = strchr( tmp_buf, '=' );

		if( ptr2 == NULL )
			continue;

		if( ptr1 != NULL )
		{
			if( ptr1 < ptr2 )
				continue;
			else
				*ptr1 = '\0';
		}

		if( !strncmp(tmp_buf, p_item, strlen(p_item)) )
		{
			strcpy( p_value, ptr2 + 1 );

			U_remove_head( p_value );
			U_remove_tail( p_value );

			ptr2 = strchr( p_value, ';' );
			if( ptr2 != NULL )
				*ptr2 = '\0';

			fclose( fp );
			return (0);
		}

	}/* while End */

	p_value[0] = '\0';

	fclose( fp );
	return (-1);
}


static int      U_move_to_section( FILE* p_fp, char* p_section )
{
	char section[50];
	char tmp_buf[256];


	sprintf( section, "[%s]", p_section );

	fseek( p_fp, 0, SEEK_SET );


	while( !feof( p_fp ) )
	{
		memset( tmp_buf, 0x00, sizeof(tmp_buf));

		if( fgets( tmp_buf, sizeof( tmp_buf ), p_fp ) == NULL )
			break;

		if( !strncmp( tmp_buf, section, strlen( section )) )
			return (0);

	}

	/* Can't find p_section in p_fp */
	return (-1);
}


static void	U_remove_tail( char* p_str )
{
	int i, len;
	char *ptr;

	len = strlen( p_str );

	for( i = (len - 1) ; i >= 0 ; i-- )
	{
		ptr = p_str + i;

		if( *ptr == ' '  || *ptr == '\n' || *ptr == '\r' || *ptr == '\t' )
			*ptr = '\0';
		else
			break;
	}
}


static void	U_remove_head( char* p_str )
{
	int i, len;
	char* tmp_ptr;

	len = strlen( p_str );

	tmp_ptr = p_str;

	for( i = 0; i < len ; i++ )
	{
		if( *tmp_ptr == ' ' || *tmp_ptr == '\t' )
			tmp_ptr++;
		else
			break;
	}

	strcpy( p_str, tmp_ptr );
}




