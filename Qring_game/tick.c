/*____________________________________________________________

	Project name : Qring Online
	File name    : tick.c
	Description  : tick module
	Date         : February 14 2005
_____________________________________________________________*/

#include	"global.h"
#include	"util.h"
#include	"recv.h"
#include	"db.h"
#define	_TICK_H_
#include	"tick.h"



//__________________________________________________________
//
void *T_Tick_main(void *arg)
{
	int	i, billing_idx, dbagent_idx, login_dbagent_idx, fd;
	time_t	send_tick;

	U_Printf("\t[TICK]Tick Thread Start !\n");
		
	billing_idx		= 1;
	dbagent_idx		= 1;
	login_dbagent_idx	= 1;
	fd			= 1;
	G_Sec			= time(NULL);
	send_tick		= G_Sec;

	//connect billing	
	//for( i=1; i<=CONNECTION_BILLING; i++)
	//	send(G_Unix_billing_idx[1], &i, SIZEOF_INT, 0);

	//connect login_dbagent
	for( i=1; i<=CONNECTION_LOGIN_DBAGENT; i++)
		send(G_Unix_login_dbagent_idx[1], &i, SIZEOF_INT, 0);
	
	//connect dbagent	
	for( i=1; i<=CONNECTION_DBAGENT; i++)
		send(G_Unix_dbagent_idx[1], &i, SIZEOF_INT, 0);

	while( 1 )
	{
		usleep(100000);
		
		G_Sec	= time(NULL);
		
		if( G_Run == 0 )
			goto Exit;		
		
		//tick
		if( (G_Sec - send_tick) > 300 )
		{
			send(G_Unix_tick[1], &send_tick, SIZEOF_INT, 0);
			send_tick	= G_Sec;
		}
	
		//billling
		//if( R_GetBilling_fd(billing_idx) == 0 )
		//	send(G_Unix_billing_idx[1], &billing_idx, SIZEOF_INT, 0);

		//billing_idx	= ( (billing_idx == CONNECTION_BILLING) ? 1 : billing_idx+1);
		
		//login_dbagent 
		if( R_GetLogin_dbagent_fd(login_dbagent_idx) == 0 )
			send(G_Unix_login_dbagent_idx[1], &login_dbagent_idx, SIZEOF_INT, 0);
		
		login_dbagent_idx	= ( (login_dbagent_idx == CONNECTION_LOGIN_DBAGENT) ? 1 : login_dbagent_idx+1);

		//dbagent 
		if( R_GetDbagent_fd(dbagent_idx) == 0 )
			send(G_Unix_dbagent_idx[1], &dbagent_idx, SIZEOF_INT, 0);
		
		dbagent_idx	= ( (dbagent_idx == CONNECTION_DBAGENT) ? 1 : dbagent_idx+1);
	
		//live check
		if( R_GetRecvfd_epoll_add(fd) && ((G_Sec - R_GetRecvfd_recv_sec(fd)) > SEC_HEARTBEAT) )
			send(G_Unix_live_fd[1], &fd, SIZEOF_INT, 0);
		
		fd		= ( (fd == MAX_FD) ? 1 : fd+1);
	} 		

Exit:
	U_Printf("\t[TICK]Tick Thread End !\n");
	return NULL;
}
