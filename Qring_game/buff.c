/*____________________________________________________________

	Project name : Qring Online
	File name    : buff.c
	Description  : send_q proc module
	Date         : February 14 2005
_____________________________________________________________*/


#include	"global.h"
#include	"util.h"
#define	_BUFF_H_
#include	"buff.h"


struct	buff_view_block
{
	int	char_link[CHANNEL_PER_MACHINE+1];
};

struct	buff_map
{
	struct	buff_view_block block[VIEW_BLOCK_CNT];
};


static	void	B_proc_SEND_INTER_CONN_FD(const int);
static	void	B_proc_SEND_INTER_CLOSE_FD(const int);
static	void	B_proc_SEND_INTER_BLOCK_INSERT_CHAR(const int, const int, const int, const int);
static	void	B_proc_SEND_INTER_BLOCK_DELETE_CHAR(const int);

static	void	B_proc_SEND_LOGIN(const int);
static	void	B_proc_SEND_UNICAST(const int);
static	void	B_proc_SEND_BROADCAST(void);
static	void	B_proc_SEND_BLOCK_ALLCAST(const int, const int, const int);
static	void	B_proc_SEND_BLOCK_OTHERCAST(const int, const int, const int, const int);


static	struct	buff_map	*B_map;
static	struct	send_q		*B_send_q;


//__________________________________________________________
//
void *B_Buff_main(void *arg)
{
	U_Printf("\t[BUFF]Buffer Thread Start !\n");

	B_map	= G_Buff_memory;

	while(1)
	{
		B_send_q	= U_Send_queue_wait();

		switch ( B_send_q->send_type )
		{
			//inter
			case	SEND_EXIT_THREAD:
				U_Send_queue_proc_end();
				goto Exit;
			case	SEND_INTER_CONN_FD:
				B_proc_SEND_INTER_CONN_FD(B_send_q->fd);
				U_Send_queue_proc_end();
				break;
			case	SEND_INTER_CLOSE_FD:
				B_proc_SEND_INTER_CLOSE_FD(B_send_q->fd);
				U_Send_queue_proc_end();
				break;
			case	SEND_INTER_BLOCK_INSERT_CHAR:
				B_proc_SEND_INTER_BLOCK_INSERT_CHAR(B_send_q->fd, B_send_q->channel_link, B_send_q->map, B_send_q->block_x);
				U_Send_queue_proc_end();
				break;
			case	SEND_INTER_BLOCK_DELETE_CHAR:
				B_proc_SEND_INTER_BLOCK_DELETE_CHAR(B_send_q->fd);
				U_Send_queue_proc_end();
				break;
			
			//socket
			case	SEND_LOGIN:
				B_proc_SEND_LOGIN(B_send_q->fd);
				U_Send_queue_proc_end();
				break;
			case	SEND_UNICAST:
				B_proc_SEND_UNICAST(B_send_q->fd);
				U_Send_queue_proc_end();
				break;
			case	SEND_BROADCAST:
				B_proc_SEND_BROADCAST();
				U_Send_queue_proc_end();
				break;
			case	SEND_BLOCK_ALLCAST:
				B_proc_SEND_BLOCK_ALLCAST(B_send_q->channel_link, B_send_q->map, B_send_q->block_x);
				U_Send_queue_proc_end();
				break;
			case	SEND_BLOCK_OTHERCAST:
				B_proc_SEND_BLOCK_OTHERCAST(B_send_q->fd, B_send_q->channel_link, B_send_q->map, B_send_q->block_x);
				U_Send_queue_proc_end();
				break;

			default:
				U_Printf("[BUFF]Error fd:%d, send_type:%d \n", B_send_q->fd, B_send_q->send_type),exit(-1);
		}//end switch
	}//end while  		

Exit:
	U_Printf("\t[BUFF]Buffer Thread End !\n");
	G_Send_buf[0].active	= 1;
	return	(NULL);
}



//_______________________________________________________
//
static	void	B_proc_SEND_INTER_CONN_FD(const int fd)
{
	pthread_mutex_lock(&(G_Send_buf[fd].mutex));
	G_Send_buf[fd].active	= 1;
	pthread_mutex_unlock(&(G_Send_buf[fd].mutex));
}


static	void	B_proc_SEND_INTER_CLOSE_FD(const int fd)
{
	pthread_mutex_lock(&(G_Send_buf[fd].mutex));
	if( G_Send_buf[fd].size )
		send(fd, G_Send_buf[fd].buf, G_Send_buf[fd].size, 0);
	G_Send_buf[fd].active	= 0;
	G_Send_buf[fd].login	= 0;
	G_Send_buf[fd].size	= 0;
	pthread_mutex_unlock(&(G_Send_buf[fd].mutex));
	
	send(G_Unix_close_fd[1], &fd, SIZEOF_INT, 0);
}


static	void	B_proc_SEND_INTER_BLOCK_INSERT_CHAR(const int fd, const int channel_link, const int map, const int block_x)
{
	G_Send_buf[fd].block_next	= B_map[map].block[block_x].char_link[channel_link];
	G_Send_buf[fd].block_prev	= &(B_map[map].block[block_x].char_link[channel_link]);

	if( G_Send_buf[fd].block_next )
		G_Send_buf[G_Send_buf[fd].block_next].block_prev	= &(G_Send_buf[fd].block_next);

	B_map[map].block[block_x].char_link[channel_link]	= fd;
}


static	void	B_proc_SEND_INTER_BLOCK_DELETE_CHAR(const int fd)
{
	*(G_Send_buf[fd].block_prev)	= G_Send_buf[fd].block_next;

	if( G_Send_buf[fd].block_next )
		G_Send_buf[G_Send_buf[fd].block_next].block_prev	= G_Send_buf[fd].block_prev;
}



//_______________________________________________________
//
static	void	B_proc_SEND_LOGIN(const int fd)
{
	pthread_mutex_lock(&(G_Send_buf[fd].mutex));
	if( G_Send_buf[fd].active )
	{
		if( (B_send_q->data_size + G_Send_buf[fd].size) > APP_BUF_SIZE )
		{
			U_Printf("[BUFF]Error fd:%d send_buf full\n", fd);
			send(fd, G_Send_buf[fd].buf, G_Send_buf[fd].size, 0);
			G_Send_buf[fd].active	= 0;
			G_Send_buf[fd].login	= 0;
			G_Send_buf[fd].size	= 0;
			U_Recv_queue_in(fd, 0, INTER_CLOSE_FD, 0, 0, 0, NULL, 0);
		}
		else
		{
			memcpy(G_Send_buf[fd].buf+G_Send_buf[fd].size,	B_send_q->data,	B_send_q->data_size);
			G_Send_buf[fd].size	+= B_send_q->data_size;

			//set login
			G_Send_buf[fd].login	= 1;
		}
	}
	pthread_mutex_unlock(&(G_Send_buf[fd].mutex));
}


static	void	B_proc_SEND_UNICAST(const int fd)
{
	pthread_mutex_lock(&(G_Send_buf[fd].mutex));
	if( G_Send_buf[fd].active )
	{
		if( (B_send_q->data_size + G_Send_buf[fd].size) > APP_BUF_SIZE )
		{
			U_Printf("[BUFF]Error fd:%d send_buf full\n", fd);
			send(fd, G_Send_buf[fd].buf, G_Send_buf[fd].size, 0);
			G_Send_buf[fd].active	= 0;
			G_Send_buf[fd].login	= 0;
			G_Send_buf[fd].size	= 0;
			U_Recv_queue_in(fd, 0, INTER_CLOSE_FD, 0, 0, 0, NULL, 0);
		}
		else
		{
			memcpy(G_Send_buf[fd].buf+G_Send_buf[fd].size,	B_send_q->data,	B_send_q->data_size);
			G_Send_buf[fd].size	+= B_send_q->data_size;
		}
	}
	pthread_mutex_unlock(&(G_Send_buf[fd].mutex));
}


static	void	B_proc_SEND_BROADCAST(void)
{
	int	i;

	
	for( i=1; i<=MAX_FD; i++ )
	{
		pthread_mutex_lock(&(G_Send_buf[i].mutex));
		if( (G_Send_buf[i].active) && (G_Send_buf[i].login) )
		{
			if( (B_send_q->data_size + G_Send_buf[i].size) > APP_BUF_SIZE )
			{
				U_Printf("[BUFF]Error fd:%d send_buf full\n", i);
				send(i, G_Send_buf[i].buf, G_Send_buf[i].size, 0);
				G_Send_buf[i].active	= 0;
				G_Send_buf[i].login	= 0;
				G_Send_buf[i].size	= 0;
				U_Recv_queue_in(i, 0, INTER_CLOSE_FD, 0, 0, 0, NULL, 0);
			}
			else
			{
				memcpy(G_Send_buf[i].buf+G_Send_buf[i].size, B_send_q->data, B_send_q->data_size);
				G_Send_buf[i].size	+= B_send_q->data_size;
			}
		}
		pthread_mutex_unlock(&(G_Send_buf[i].mutex));
	}
}


static	void	B_proc_SEND_BLOCK_ALLCAST(const int channel_link, const int map, const int block_x)
{
	int	char_link;


	char_link	= B_map[map].block[block_x].char_link[channel_link];
	
	while( char_link )
	{
		pthread_mutex_lock(&(G_Send_buf[char_link].mutex));
		if( G_Send_buf[char_link].active )
		{
			if( (B_send_q->data_size + G_Send_buf[char_link].size) > APP_BUF_SIZE )
			{
				U_Printf("[BUFF]Error fd:%d send_buf full\n", char_link);
				send(char_link, G_Send_buf[char_link].buf, G_Send_buf[char_link].size, 0);
				G_Send_buf[char_link].active	= 0;
				G_Send_buf[char_link].login	= 0;
				G_Send_buf[char_link].size	= 0;
				U_Recv_queue_in(char_link, 0, INTER_CLOSE_FD, 0, 0, 0, NULL, 0);
			}
			else
			{
				memcpy(G_Send_buf[char_link].buf+G_Send_buf[char_link].size, B_send_q->data, B_send_q->data_size);
				G_Send_buf[char_link].size	+= B_send_q->data_size;
			}
		}
		pthread_mutex_unlock(&(G_Send_buf[char_link].mutex));

		char_link	= G_Send_buf[char_link].block_next;	
	}	
}


static	void	B_proc_SEND_BLOCK_OTHERCAST(const int fd, const int channel_link, const int map, const int block_x)
{
	int	char_link;


	char_link	= B_map[map].block[block_x].char_link[channel_link];
	
	while( char_link )
	{
		pthread_mutex_lock(&(G_Send_buf[char_link].mutex));
		if( (G_Send_buf[char_link].active) && (char_link != fd) )
		{
			if( (B_send_q->data_size + G_Send_buf[char_link].size) > APP_BUF_SIZE )
			{
				U_Printf("[BUFF]Error fd:%d send_buf full\n", char_link);
				send(char_link, G_Send_buf[char_link].buf, G_Send_buf[char_link].size, 0);
				G_Send_buf[char_link].active	= 0;
				G_Send_buf[char_link].login	= 0;
				G_Send_buf[char_link].size	= 0;
				U_Recv_queue_in(char_link, 0, INTER_CLOSE_FD, 0, 0, 0, NULL, 0);
			}
			else
			{
				memcpy(G_Send_buf[char_link].buf+G_Send_buf[char_link].size, B_send_q->data, B_send_q->data_size);
				G_Send_buf[char_link].size	+= B_send_q->data_size;
			}
		}
		pthread_mutex_unlock(&(G_Send_buf[char_link].mutex));

		char_link	= G_Send_buf[char_link].block_next;	
	}	
}
