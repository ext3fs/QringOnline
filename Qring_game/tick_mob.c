/*____________________________________________________________

	Project name : Qring Online
	File name    : tick_mob.c
	Description  : tick mob module
	Date         : February 14 2005
_____________________________________________________________*/

#include	"global.h"
#include	"util.h"
#include	"db.h"
#define	_TICK_MOB_H_
#include	"tick_mob.h"



//__________________________________________________________
//
void *TM_Tick_mob_main(void *arg)
{
	int	i, j, channel_link, max_mob, mob_id, tick_mob_cnt;
	time_t	(*check_sec)[CHANNEL_PER_MACHINE+1], (*recover_sec)[CHANNEL_PER_MACHINE+1], buff_end_sec;
	

	U_Printf("\t[TICK_MOB]Tick Mob Thread Start !\n");
	
	if( D_GetMax_mob() == 0 )
		goto Exit;
	
	max_mob		= D_GetMax_mob();		
	check_sec	= G_Tick_mob_memory;
	recover_sec	= G_Tick_mob_memory + (max_mob + 1)*(CHANNEL_PER_MACHINE+1);
	channel_link	= 1;
	mob_id		= 1;
	tick_mob_cnt	= max_mob*(CHANNEL_PER_MACHINE+1)/1000 + 1; 

	while( 1 )
	{
		usleep(0);
		
		if( G_Run == 0 )
			goto Exit;		

		for( i=0; i<tick_mob_cnt; i++ )
		{
			if( (D_GetMob_status(channel_link, mob_id) < _MOB_DEATH) && (G_Sec != check_sec[mob_id][channel_link]) )
			{
				//recover
				if( (G_Sec - recover_sec[mob_id][channel_link]) >= SEC_RECOVER)
				{
					U_Recv_queue_in(channel_link, mob_id, INTER_MOB_RECOVER, 0, 0, 0, NULL, 0);
					recover_sec[mob_id][channel_link]	= G_Sec;
				}

				//buff
				for( j=TIMELESS_BUFF_CNT; j<BUFF_SERIES_CNT; j++ )
				{
					buff_end_sec	= D_GetMob_buff_end_sec(channel_link, mob_id, j);

					if( (buff_end_sec) && (buff_end_sec < G_Sec) )
						U_Recv_queue_in(channel_link, mob_id, INTER_MOB_BUFF_END, 0, 0, SIZEOF_INT, (char *)(&j), 0);
				}
				
				check_sec[mob_id][channel_link]	= G_Sec;	
			}		

			if( mob_id == max_mob )
			{
				mob_id = 1;
				channel_link	= ( (channel_link == (CHANNEL_PER_MACHINE+1)) ? 1 : channel_link+1);
			}
			else
			{
				++mob_id;
			}	

		}
	} 		

Exit:
	U_Printf("\t[TICK_MOB]Tick Mob Thread End !\n");
	return NULL;
}
