/*____________________________________________________________

	Project name : Qring Online
	File name    : tick_item.c
	Description  : tick item module
	Date         : February 14 2005
_____________________________________________________________*/

#include	"global.h"
#include	"util.h"
#include	"db.h"
#define	_TICK_ITEM_H_
#include	"tick_item.h"



//__________________________________________________________
//
void *TI_Tick_item_main(void *arg)
{
	int	item_id;
	time_t	drop_time;	


	U_Printf("\t[TICK_ITEM]Tick Item Thread Start !\n");
	
	item_id	= 1;

	while( 1 )
	{
		usleep(0);
		
		if( G_Run == 0 )
			goto Exit;		
		
		drop_time	= D_GetItem_drop_time(item_id);

		if( (drop_time) && ((drop_time + MAX_ITEM_DROP_TIME) < G_Sec) )
			U_Recv_queue_in(item_id, 0, INTER_ITEM_REMOVE, 0, 0, 0, NULL, 0);

		item_id	= ( (item_id == MAX_DROP_ITEM) ? 1 : item_id+1);
	} 		

Exit:
	U_Printf("\t[TICK_ITEM]Tick Item Thread End !\n");
	return NULL;
}
