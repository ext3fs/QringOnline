/*____________________________________________________________

	Project name : Qring Online
	File name    : tick_char.c
	Description  : tick char module
	Date         : February 14 2005
_____________________________________________________________*/

#include	"global.h"
#include	"util.h"
#include	"db.h"
#define	_TICK_CHAR_H_
#include	"tick_char.h"



//__________________________________________________________
//
void *TC_Tick_char_main(void *arg)
{
	int	i, j, fd, tick_char_cnt;
	time_t	check_sec[MAX_FD], recover_sec[MAX_FD], buff_end_sec;
	

	U_Printf("\t[TICK_CHAR]Tick Char Thread Start !\n");
	
	fd		= 1;
	tick_char_cnt	= MAX_FD/1000 + 1; 

	while( 1 )
	{
		usleep(0);
		
		if( G_Run == 0 )
			goto Exit;		

		for( i=0; i<tick_char_cnt; i++ )
		{
			if( (D_GetClient_status(fd) < _USER_DEAD) && (G_Sec != check_sec[fd]) )
			{
				//recover
				if( (G_Sec - recover_sec[fd]) >= SEC_RECOVER )
				{
					U_Recv_queue_in(fd, 0, INTER_CHAR_RECOVER, 0, 0, 0, NULL, 0);
					recover_sec[fd]	= G_Sec;		
				}

				//buff
				for( j=TIMELESS_BUFF_CNT; j<BUFF_SERIES_CNT; j++ )
				{
					buff_end_sec	= D_GetClient_buff_end_sec(fd, j);	
					
					if( (buff_end_sec) && (buff_end_sec < G_Sec) )
						U_Recv_queue_in(fd, 0, INTER_CHAR_BUFF_END, 0, 0, SIZEOF_INT, (char *)(&j), 0);
				}
				
				check_sec[fd]	= G_Sec;
			}		

			fd	= ( (fd == MAX_FD) ? 1 : fd+1);
		}
	} 		

Exit:
	U_Printf("\t[TICK_CHAR]Tick Char Thread End !\n");
	return NULL;
}
