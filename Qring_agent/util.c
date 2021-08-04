
#include 	"global.h"
#include 	"util.h"



// ---------------------------------------------------------------------------
// INNER FUNCTION-PROTOTYPE

static int		U_set_log			( const int, const char * );

static int		U_move_to_section		( FILE *, char * );
static void		U_remove_tail			( char * );
static void		U_remove_head			( char * );

//static void		U_GetTime_Standard		( char * );
//static void		U_GetTime_Arg			( struct timeval, char * );





#define iseuc_kr(c)     ((0xa1<=(unsigned char)(c) && (unsigned char)(c)<=0xfe))

static  int  ismbchar_euc_kr(const char* p, const char *e)
{
         return ((*(unsigned char*)(p)<0x80)? 0:iseuc_kr(*(p)) && (e)-(p)>1 && iseuc_kr(*((p)+1))? 2:0);
}

unsigned long U_change_escape_string(char *to, const char *from, unsigned long length)
{
        const char *to_start=to;
        const char *end;
        int l;


        for (end=from+length; from != end ; from++)
        {
                if ( (l = ismbchar_euc_kr(from, end)) )
                {
                        while (l--)
                                *to++ = *from++;
                        from--;
                        continue;
                }

                switch (*from)
                {
                        case '\n':
                                *to++= '\\';
                                *to++= 'n';
                                break;
                        case '\r':
                                *to++= '\\';
                                *to++= 'r';
                                break;
                        case '\\':
                                *to++= '\\';
                                *to++= '\\';
                                break;
                        case '\'':
                        case '"':
                        case '\t':
                                return 0;
                        default:
                                *to++= *from;
                }
          }

          *to=0;
          return (unsigned long)(to-to_start);
}







// ---------------------------------------------------------------------------
// log
int	dprintf( const int TIDX, const char *fmt, ... )
{
	va_list args;
	char    buf[1024];

	va_start( args, fmt );
	vsnprintf( buf, sizeof(buf), fmt, args );
	va_end( args );

	if( G_daemon )
	{
		U_set_log(TIDX, buf);
	}
	else{
		fputs(buf,stdout);
		fflush(stdout);
	}

	return SUCC;
}


int     U_set_log( const int TIDX, const char *a_log_data )
{
	char 		temp_file_name  [128];
	char		temp_write_data [128];
	char		temp_date_buf   [128];
	struct tm *     tm_ptr;
	time_t          raw;

	FILE *          log_fp;

	time(&raw);
	tm_ptr = localtime(&raw);

	U_GetTime_yyyymmddHH( temp_date_buf );
	sprintf ( temp_file_name, "./error_log/agentserv_%s.log", temp_date_buf );
	strftime( temp_write_data, 128, "[%Y-%m-%d %H:%M:%S]", tm_ptr );

	if ((log_fp = fopen(temp_file_name, "a")) == NULL) 
		return FAIL;

	fprintf( log_fp, "%s\t%s", temp_write_data, a_log_data );

	fclose(log_fp);
	return SUCC;
}




// ---------------------------------------------------------------------------
// time
/*
static void	U_GetTime_Standard( char *p_date )
{
	time_t  raw;

	time( &raw );
	sprintf( p_date, "%s", ctime(&raw) );
	p_date[strlen(p_date)-1] = 0;
}


static void	U_GetTime_Arg( struct timeval a_timeval, char *p_date )
{
	sprintf( p_date, "%s", ctime(&(a_timeval.tv_sec)) );
}
*/

void	U_GetTime_yyyymmddHH( char *p_date )
{
	struct  tm *tm_ptr;
	time_t  raw;

	time( &raw );
	tm_ptr = localtime( &raw );
	strftime( p_date, 128, "%Y-%m-%d-%H", tm_ptr );
}


time_t	U_get_time( void )
{
	return time(NULL);
}



// ---------------------------------------------------------------------------
// Process fork()
pid_t	U_pro_fork()
{
	pid_t pid;

	if((pid=fork()) == -1)
	{
		fprintf( stdout, "[UTIL]U_pro_fork() error\n" );
		exit(FAIL);
	}

	return(pid);
}




// ---------------------------------------------------------------------------
// Process file()
int     U_get_value( char* p_file, char* p_section, char *p_item, char *p_value )
{
	FILE* fp;
	char  tmp_buf[256];
	char  *ptr1, *ptr2;


	if( (fp = fopen( p_file, "r" )) == NULL )
	{
		printf("%s's Read Fail !! \n", p_file );
		return FAIL;
	}

	if( U_move_to_section( fp, p_section ) < 0 )
	{
		fclose( fp );
		return FAIL;
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

		if( !strncmp( tmp_buf, p_item, strlen( p_item ) ) )
		{
			strcpy( p_value, ptr2 + 1 );

			U_remove_head( p_value );
			U_remove_tail( p_value );

			ptr2 = strchr( p_value, ';' );
			if( ptr2 != NULL )
				*ptr2 = '\0';

			fclose( fp );
			return SUCC;
		}

	}/* while End */

	p_value[0] = '\0';

	fclose( fp );
	return FAIL;
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
			return 0;

	}

	/* Can't find p_section in p_fp */
	return -1;
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


// ---------------------------------------------------------------------------
//
int	U_ConnectToServer( const int fd, const char *ipaddress, const int port, const int nSec )
{
	int			handler, nEvent, optval, optlen, ret;
	struct sockaddr_in	addr;
	struct epoll_event	event;

	bzero( (char *)&addr, sizeof(struct sockaddr_in) );
	inet_aton( ipaddress, &addr.sin_addr );
	addr.sin_family		= AF_INET;
	addr.sin_port		= htons(port);

	if( (ret = connect(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in))) < 0 )
	{
		if( errno != EINPROGRESS )
		{
			dprintf(M_LOG, "[UTIL]Error connect() : %s \n", strerror(errno) );
			return FAIL;
		}
	}

	if( (handler = epoll_create(1)) < 0 )
	{
		dprintf(M_LOG, "[UTIL]Error epoll_create() : %s \n", strerror(errno) );
		return FAIL;
	}

	event.events	= EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP ;
	event.data.fd	= fd;
	if( epoll_ctl(handler, EPOLL_CTL_ADD, fd, &event) < 0 )
	{
		dprintf(M_LOG, "[UTIL]Error epoll_ctl() : %s \n", strerror(errno) );
		close(handler);
		return FAIL;
	}

	if( (nEvent = epoll_wait(handler, &event, 1, nSec ? (nSec*1000) : -1)) < 0 )
	{
		dprintf(M_LOG, "[UTIL]Error epoll_wait() : %s \n", strerror(errno) );
		close(handler);	
		return FAIL;
	}

	close(handler);


	optval	= 0;
	optlen	= SIZEOF_INT;
	if( getsockopt(fd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0 )
	{
		dprintf(M_LOG, "[UTIL]Error getsockopt() : %s \n", strerror(errno) );
		return FAIL;
	}

	if( optval != 0 )
	{
		dprintf(M_LOG, "[UTIL]Error connect() optval = %d : %s \n", optval, strerror(errno) );
		return FAIL;
	}

	return SUCC;
}


int	U_SendToServer( const int fd, const int nByte, char *pBuf, const int nSec )
{
	struct epoll_event	event, ev[1];
	int			epoll_handler, nEvent, nleft, nsend;
	char			*ptr;

	
	if( (epoll_handler = epoll_create(1)) < 0 )
	{
		dprintf(M_LOG, "[UTIL]Error epoll_create() : %s \n", strerror(errno) );
		return FAIL;
	}

	ptr	= pBuf;
	nleft	= nByte;


	event.events	= EPOLLOUT | EPOLLERR | EPOLLHUP;
	event.data.fd	= fd;
	if( epoll_ctl(epoll_handler, EPOLL_CTL_ADD, fd, &event) < 0 )
	{
		dprintf(M_LOG, "[UTIL]Error epoll_ctl() : %s \n", strerror(errno) );
		close(epoll_handler);
		return FAIL;
	}

	while( nleft > 0 )
	{
		if( (nEvent = epoll_wait(epoll_handler, ev, 1, nSec ? (nSec*1000) : -1)) < 0 )
		{
			dprintf(M_LOG, "[UTIL]Error epoll_wait() : %s \n", strerror(errno) );
			close(epoll_handler);
			return FAIL;
		}

		if( nEvent == 0 )
		{
			dprintf(M_LOG, "[UTIL]Error send() %dsec timeout \n", nSec );
			close(epoll_handler);
			return FAIL;
		}

		if( ev[0].events & EPOLLOUT )
		{
			if( (nEvent != 1) || (ev[0].data.fd != fd) )
			{
				dprintf(M_LOG, "[UTIL]Error nEvent = %d ev[0].data.fd = %d \n", nEvent, ev[0].data.fd );
				close(epoll_handler);
				return FAIL;
			}

			if( (nsend = send(fd, ptr, nleft, 0)) < 1 )
			{
				dprintf(M_LOG, "[UTIL]Error send() : %s \n", strerror(errno) );
				close(epoll_handler);
				return FAIL;
			}
		}
		else
		{
			dprintf(M_LOG, "[UTIL]Error send() : %s \n", strerror(errno) );
			close(epoll_handler);
			return FAIL;
		}

		nleft	-= nsend;
		ptr	+= nsend;
	}

	close(epoll_handler);
	return SUCC;
}


int	U_RecvFromServer( const int fd, const int nByte, char *pBuf, const int nSec )
{
	struct epoll_event	event, ev[1];
	int			epoll_handler, nEvent, nleft, nrecv;
	char			*ptr;


	if( (epoll_handler = epoll_create(1)) < 0 )
	{
		dprintf(M_LOG, "[UTIL]Error epoll_create() : %s\n", strerror(errno));
		return FAIL;
	}

	ptr     = pBuf;
	nleft   = nByte;


	event.events    = EPOLLIN|EPOLLERR|EPOLLHUP;
	event.data.fd   = fd;

	if( epoll_ctl(epoll_handler, EPOLL_CTL_ADD, fd, &event) < 0 )
	{
		dprintf(M_LOG, "[UTIL]Error epoll_ctl() : %s\n", strerror(errno));
		close(epoll_handler);
		return FAIL;
	}

	while( nleft > 0 )
	{

		if( (nEvent = epoll_wait(epoll_handler, ev, 1, nSec? (1000*nSec) : -1)) < 0 )
		{
			dprintf(M_LOG, "[UTIL]Error epoll_wait() : %s\n", strerror(errno));
			close(epoll_handler);
			return FAIL;
		}

		if( nEvent == 0 )
		{
			dprintf(M_LOG, "[UTIL]Error recv() %dsec timeout\n", nSec );
			close(epoll_handler);
			return FAIL;
		}

		if( ev[0].events & EPOLLIN )
		{
			if( (nEvent != 1) || (ev[0].data.fd != fd) )
			{
				dprintf(M_LOG, "[UTIL]Error nEvent = %d, ev[0].data.fd = %d\n", nEvent, ev[0].data.fd);
				close(epoll_handler);
				return FAIL;
			}

			if( (nrecv = recv(fd, ptr, nleft, 0)) < 1 )
			{
				dprintf(M_LOG, "[UTIL]Error recv() : %s\n", strerror(errno));
				close(epoll_handler);
				return FAIL;
			}
		}
		else
		{
			dprintf(M_LOG, "[UTIL]Error recv() : %s \n", strerror(errno) );
			close(epoll_handler);
			return FAIL;
		}

		nleft   -= nrecv;
		ptr     += nrecv;
	}

	close(epoll_handler);
	return SUCC;
}

