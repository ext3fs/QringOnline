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


struct	rand_val
{
	int	val;
	int	next;
};

struct item_slot
{
	int		item_slot;

	int		item1;
	int		item2;
	int		item3;
	int		item4;

	char		serial[ITEM_SERIAL_LEN];
};


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

static	unsigned	short		U_serial_seq;



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


void	U_Send_queue_in(const int send_type, const int fd,
					const int channel_link, const int map, const int block_x, const int data_size, const char *data)
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
	U_send_queue[temp_tail].channel_link	= channel_link;
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
	char		temp_file_name[128];
	char		temp_write_data[128];
	char 		temp_date_buf[128];
	struct	tm	*tm_ptr;
	FILE 		*log_fp;


	tm_ptr	= localtime((time_t *)(&(G_Sec)));
	strftime(temp_write_data, 128, "%Y-%m-%d %H:%M:%S\t", tm_ptr);
	strftime(temp_date_buf, 128, "%Y-%m-%d-%H", tm_ptr);
	snprintf(temp_file_name, 128, "./err_log/Qring%d_%s.log", G_Server_code, temp_date_buf);
	
	if ((log_fp = fopen(temp_file_name, "a")) == NULL) 
		return (-1);

	fprintf(log_fp,"%s%s",temp_write_data, buf);
	fclose(log_fp);
	return (0);
}


void	U_Printf(const char *fmt, ...)
{
	va_list	args;
	char	buf[8192];
	
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
//serial
void	U_MakeSerial(char *serial, const char *char_name, const int item_type)
{
	struct	tm	*tm_ptr;
	char		time_buf[16];


	tm_ptr	= localtime((time_t *)(&(G_Sec)));
	strftime(time_buf, 128, "%Y%m%d%H%M%S", tm_ptr);
		
	memset(serial, 32, CHAR_NAME_LEN);
	memcpy(serial, char_name, strlen(char_name));
	snprintf(serial+CHAR_NAME_LEN, ITEM_SERIAL_LEN+1-CHAR_NAME_LEN, "%09d%s%05d%05d", G_Server_code, time_buf, item_type, ++U_serial_seq);
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


int	U_change_escape_string(char *to, const char *from, const int length)
{
        const char *to_start=to;
        const char *end;
        int l;


        for (end=from+length; from != end ; from++)
        {
                if ( (l = U_ismbchar_euc_kr(from, end)) )
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
          return (int)(to-to_start);
}


void	U_Print_query(const char *send_buf)
{
	char				str_query[4096];
	char				temp_file_name[128];
	char				temp_write_data[128];
	char 				temp_date_buf[128];
	struct	tm			*tm_ptr;
	FILE 				*log_fp;
	struct	body1_WA_CHAR_SAVE	*req_body1	= (struct body1_WA_CHAR_SAVE *)(send_buf + SIZEOF_HEADER_SPECIAL);
	struct	body2_WA_CHAR_SAVE	*req_body2
					= (struct body2_WA_CHAR_SAVE *)(send_buf + SIZEOF_HEADER_SPECIAL + sizeof(struct body1_WA_CHAR_SAVE));
	int				i, n;
	struct	item_slot		*p_item;		 

	
	tm_ptr	= localtime((time_t *)(&(G_Sec)));
	strftime(temp_write_data, 128, "%Y-%m-%d %H:%M:%S\t", tm_ptr);
	strftime(temp_date_buf, 128, "%Y-%m-%d-%H", tm_ptr);
	snprintf(temp_file_name, 128, "./save_log/Qring_save%d_%s.log", G_Server_code, temp_date_buf);
	
	if ((log_fp = fopen(temp_file_name, "a")) == NULL) 
		return;

	//char_table
	n = snprintf( str_query, 4096, "update char_table set load_cnt=%d, map_num=%d, x=%d, y=%d, z=%d, regen_map=%d, regen_x=%d, regen_y=%d, regen_z=%d, sex=%d, hair=%d, face=%d, level=%d, exp=%d, hp=%d, ap=%d, money=%d, skill_point=%d, skill1=%d, skill2=%d, skill3=%d, skill4=%d, skill5=%d, skill6=%d, skill7=%d, skill8=%d, skill9=%d, skill10=%d, skill11=%d, skill12=%d, skill13=%d, skill14=%d, skill15=%d, skill16=%d, skill17=%d, skill18=%d, skill19=%d, skill20=%d, skill21=%d, skill22=%d, skill23=%d, skill24=%d, skill25=%d, skill26=%d, skill27=%d, skill28=%d, skill29=%d, skill30=%d, skill31=%d, skill32=%d, skill33=%d, skill34=%d, skill35=%d, skill36=%d, skill37=%d, skill38=%d, skill39=%d, skill40=%d, skill41=%d, skill42=%d, skill43=%d, skill44=%d, skill45=%d, skill46=%d, skill47=%d, skill48=%d where user_id='%s' and char_name='"
		, req_body1->load_cnt
		, req_body1->map
		, req_body1->x
		, req_body1->y
		, req_body1->z
		, req_body1->regen_map
		, req_body1->regen_x
		, req_body1->regen_y
		, req_body1->regen_z
		, req_body1->sex
		, req_body1->hair
		, req_body1->face
		, req_body1->level
		, req_body1->exp
		, req_body1->hp
		, req_body1->ap
		, req_body1->money
		, req_body1->skill_point
		, req_body1->skill_slot[0]
		, req_body1->skill_slot[1]
		, req_body1->skill_slot[2]
		, req_body1->skill_slot[3]
		, req_body1->skill_slot[4]
		, req_body1->skill_slot[5]
		, req_body1->skill_slot[6]
		, req_body1->skill_slot[7]
		, req_body1->skill_slot[8]
		, req_body1->skill_slot[9]
		, req_body1->skill_slot[10]
		, req_body1->skill_slot[11]
		, req_body1->skill_slot[12]
		, req_body1->skill_slot[13]
		, req_body1->skill_slot[14]
		, req_body1->skill_slot[15]
		, req_body1->skill_slot[16]
		, req_body1->skill_slot[17]
		, req_body1->skill_slot[18]
		, req_body1->skill_slot[19]
		, req_body1->skill_slot[20]
		, req_body1->skill_slot[21]
		, req_body1->skill_slot[22]
		, req_body1->skill_slot[23]
		, req_body1->skill_slot[24]
		, req_body1->skill_slot[25]
		, req_body1->skill_slot[26]
		, req_body1->skill_slot[27]
		, req_body1->skill_slot[28]
		, req_body1->skill_slot[29]
		, req_body1->skill_slot[30]
		, req_body1->skill_slot[31]
		, req_body1->skill_slot[32]
		, req_body1->skill_slot[33]
		, req_body1->skill_slot[34]
		, req_body1->skill_slot[35]
		, req_body1->skill_slot[36]
		, req_body1->skill_slot[37]
		, req_body1->skill_slot[38]
		, req_body1->skill_slot[39]
		, req_body1->skill_slot[40]
		, req_body1->skill_slot[41]
		, req_body1->skill_slot[42]
		, req_body1->skill_slot[43]
		, req_body1->skill_slot[44]
		, req_body1->skill_slot[45]
		, req_body1->skill_slot[46]
		, req_body1->skill_slot[47]
		, req_body1->user_id );
	n += U_change_escape_string( (str_query+n), req_body1->char_name, strlen(req_body1->char_name) );
	n += snprintf( (str_query+n), (4096-n), "';" );

	fprintf(log_fp,"%suser_id(%s) char_name(%s)\n%s\n", temp_write_data, req_body1->user_id, req_body1->char_name, str_query);


	//item_table
	for( i=0; i<req_body1->item_slot_cnt; i++ )
	{
		p_item	= (struct item_slot *)((char *)req_body2 + (i * sizeof(struct body2_WA_CHAR_SAVE)));

		if( p_item->serial[0] == 0 )
		{
			n = snprintf(str_query, 4096, "insert into item_table (char_name, user_id, position, item1, item2, item3, item4) values ('");
			n += U_change_escape_string(str_query+n, req_body1->char_name, strlen(req_body1->char_name) );
			n += snprintf(str_query+n, 4096-n, "', '%s', %d, %d, %d, %d, %d );", req_body1->user_id,
							p_item->item_slot, p_item->item1, p_item->item2, p_item->item3, p_item->item4 );
		}
		else
		{
			n = snprintf(str_query, 4096, "insert into item_table (char_name, user_id, position, serial, item1, item2, item3, item4) values ('");
			n += U_change_escape_string(str_query+n, req_body1->char_name, strlen(req_body1->char_name));
			n += snprintf(str_query+n, 4096-n, "', '%s', %d, '", req_body1->user_id, p_item->item_slot);
			n += U_change_escape_string(str_query+n, p_item->serial, ITEM_SERIAL_LEN);
			n += snprintf(str_query+n, 4096-n, "', %d, %d, %d, %d);", p_item->item1, p_item->item2, p_item->item3, p_item->item4);
		}
	
		fprintf(log_fp,"%s\n", str_query);
	}

	fclose(log_fp);
}
