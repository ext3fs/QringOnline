/*____________________________________________________________

	Project name : Qring online
	File name    : send.c
	Description  : send module
	Date         : February 14 2005
_____________________________________________________________*/


#include "global.h"
#include "util.h"
#define  _SEND_H_
#include "send.h"


static	void	S_send_buf(void);

//__________________________________________________________
//
void *S_Send_main(void *arg)
{

	U_Printf("\t[SEND]Send Thread Start !\n");

	while(1)
	{
		usleep(1);
		
		if( G_Send_buf[0].active )
		{	
			S_send_buf();
			goto Exit;		
		}

		S_send_buf();
		
	}//end while  		

Exit:
	U_Printf("\t[SEND]Send Thread End !\n");
	return	(NULL);
}


//__________________________________________________________
//
static	void	S_send_buf(void)
{
	int	i, size;
	

	for( i=1; i<MAX_FD; i++)
	{
		pthread_mutex_lock(&(G_Send_buf[i].mutex));
		if( G_Send_buf[i].size )
		{
			if( (size = send(i, G_Send_buf[i].buf, G_Send_buf[i].size, 0)) < 0 )
			{
				U_Printf("[SEND]Error fd:%d, send() : %s\n", i, strerror(errno));
				G_Send_buf[i].active	= 0;
				G_Send_buf[i].size	= 0;
				U_Recv_queue_in(i, 0, INTER_CLOSE_FD, 0, 0, 0, NULL, 0);
			}
			else
			{
				G_Send_buf[i].size	-= size;
				memmove(G_Send_buf[i].buf, G_Send_buf[i].buf+size, G_Send_buf[i].size);
			}
		}
		pthread_mutex_unlock(&(G_Send_buf[i].mutex));
	}		
}
