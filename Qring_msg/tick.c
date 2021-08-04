/*____________________________________________________________

	Project name : Qring online
	File name    : tick.c
	Description  : tick module
	Date         : February 14 2005
_____________________________________________________________*/

#include	"global.h"
#include	"util.h"
#include	"recv.h"
#define	_TICK_H_
#include	"tick.h"



//__________________________________________________________
//
void *T_Tick_main(void *arg)
{
	long	live_check_sec;
	int	i, start_fd, end_fd, stored_end_fd=0;
	

	U_Printf("\t[TICK]Tick Thread Start !\n");
		
	live_check_sec = G_Sec;

	while( 1 )
	{
		usleep(100000);
		
		G_Sec	= time(NULL);

		if( G_Run == 0 )
			goto Exit;		
			
		//live check	
		if( (G_Sec - live_check_sec) > 10 )
		{	
			live_check_sec	= G_Sec;

			//client
			start_fd	= stored_end_fd;
			stored_end_fd	= start_fd + MAX_FD/100;

			if( stored_end_fd >= MAX_FD )
			{
				stored_end_fd	= 0;
				end_fd		= MAX_FD;
			}
			else
			{
				end_fd		= stored_end_fd;
			}

			for( i=start_fd; i<end_fd; i++ )
				if( R_GetRecvfd_epoll_add(i) && ((G_Sec - R_GetRecvfd_live_sec(i)) > SEC_HEARTBEAT) )
					send(G_Unix_live_fd[1], &i, SIZEOF_INT, 0);
		}
		
	}//end while  		

Exit:
	U_Printf("\t[TICK]Tick Thread End !\n");
	return NULL;
}
