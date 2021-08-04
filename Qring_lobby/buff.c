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


static	void	B_proc_SEND_INTER_CONN_FD(const int);
static	void	B_proc_SEND_INTER_CLOSE_FD(const int);

static	void	B_proc_SEND_LOGIN(const int);
static	void	B_proc_SEND_UNICAST(const int);
static	void	B_proc_SEND_BROADCAST(void);


static	struct	send_q		*B_send_q;


//__________________________________________________________
//
void *B_Buff_main(void *arg)
{
	U_Printf("\t[BUFF]Buffer Thread Start !\n");

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
	
	send(G_Unix_close_fd[1], &fd, 4, 0);
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

	
	for( i=1; i<MAX_FD; i++ )
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
