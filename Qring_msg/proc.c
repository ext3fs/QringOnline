/*____________________________________________________________

	Project name : Qring online
	File name    : proc.c
	Description  : proc module
	Date         : February 14 2005
_____________________________________________________________*/


#include	"global.h"
#include	"util.h"
#include	"db.h"
#include	"mk_packet.h"
#define	_PROC_H_
#include	"proc.h"


static	void	P_remove_client(const int);

static	void	P_proc_INTER_CONN_CLIENT_FD(const int);
static	void	P_proc_INTER_CLOSE_FD(const int);


static	struct	recv_q	*P_recv_q;

static	int			P_session;
static	char			P_send_buf[SIZEOF_HEADER_SPECIAL+SIZEOF_MAX_PACKET_BODY];
static	int			P_send_size;





//____________________________________________________________
//
void *P_Proc_main(void *arg)
{

	U_Printf("\t[PROC]Proc Thread Start !\n");
	
	while(1)
	{
		P_recv_q	= U_Recv_queue_wait();

		switch ( P_recv_q->command )
		{
			//INTER
			case	INTER_EXIT_THREAD:
				U_Recv_queue_proc_end();
				goto Exit;
			case	INTER_CONN_CLIENT_FD:
				P_proc_INTER_CONN_CLIENT_FD(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	INTER_CLOSE_FD:
				P_proc_INTER_CLOSE_FD(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;

			default:
				U_Printf("[PROC]Hack fd:%d, command:%x, char_name:%s\n",
					P_recv_q->fd, P_recv_q->command, D_GetClient_char_name(P_recv_q->fd));
				P_proc_INTER_CLOSE_FD(P_recv_q->fd);
				U_Recv_queue_proc_end();
		}//end switch
	}//end while  		

Exit:
	U_Printf("\t[PROC]Proc Thread End !\n");
	U_Send_queue_in(SEND_EXIT_THREAD, 0, 0, 0, 0, NULL);
	return	(NULL);
}


//_________________________________________________________
//
static	void	P_remove_client(const int fd)
{
	//login
	if( *D_GetClient_char_name(fd) )
	{
		--G_Login_number;

		U_Printf("[PROC]do noting logout fd(%d) char_name(%s)\n", fd, D_GetClient_char_name(fd));
	}

	D_SetClient_clear(fd);
	U_Send_queue_in(SEND_INTER_CLOSE_FD, fd, 0, 0, 0, NULL);
}


//____________________________________________________________
//INTER
static	void	P_proc_INTER_CONN_CLIENT_FD(const int fd)
{
	D_SetClient_sec_active(fd, G_Sec);
	D_SetClient_session(fd, ++P_session);
	U_Send_queue_in(SEND_INTER_CONN_FD, fd, 0, 0, 0, NULL);
}

	
static	void	P_proc_INTER_CLOSE_FD(const int fd)
{
	if( D_GetClient_sec_active(fd) )
		P_remove_client(fd);
}
