/*____________________________________________________________

	Project name : Qring Online
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


static	void	P_all_mob_extinct(const int);
static	void	P_send_q_in_block_GC_XXXX_APPEAR(const int, const int, const int, const int);
static	void	P_send_q_in_block_GC_XXXX_DISAPPEAR(const int, const int, const int, const int);
static	int	P_char_move(int, const int, const int, const float, const float, const float, const float);
static	int	P_char_attack_mob(int, int, const int, const int, const int, const float, const float, const float, const float);
static	int	P_char_skill_action(int, struct body1_CG_CHAR_SKILL_ACTION *, const int, const float, const float, const float, const float);
static	int	P_mob_move(const int, int, const int, const int, const float, const float, const float, const float);

static	void	P_remove_client(int);

/*
static	void	P_proc_BW_GAMESTART_SUCC(const int);
static	void	P_proc_BW_GAMESTART_FAIL(const int);
static	void	P_proc_BW_GAMESTOP_SUCC(const int);
static	void	P_proc_BW_GAMESTOP_FAIL(const int);
static	void	P_proc_BW_USER_CHECK(const int);
static	void	P_proc_BW_DATABASE_ERROR(const int);
static	void	P_proc_BW_SYSTEM_ERROR(const int);
static	void	P_proc_BW_REGSERVER_SUCC(const int);
static	void	P_proc_BW_REGSERVER_FAIL(const int);
static	void	P_proc_BW_ALIVECHECK(const int);
*/

//static	void	P_proc_INTER_CONN_BILLING_FD(const int);
static	void	P_proc_INTER_CONN_DBAGENT_FD(const int);
static	void	P_proc_INTER_CONN_LOGIN_DBAGENT_FD(const int);
static	void	P_proc_INTER_CONN_MOBSERV_FD(const int);
static	void	P_proc_INTER_CONN_CLIENT_FD(const int);
static	void	P_proc_INTER_CLOSE_FD(const int);
static	void	P_proc_INTER_CHAR_RECOVER(const int);
static	void	P_proc_INTER_MOB_RECOVER(const int, const int);
static	void	P_proc_INTER_ITEM_REMOVE(const int);
static	void	P_proc_INTER_CHAR_BUFF_END(const int);
static	void	P_proc_INTER_MOB_BUFF_END(const int, const int);
static	void	P_proc_INTER_TICK(const int);

static	void	P_proc_AW_INIT_RESULT(const int);
static	void	P_proc_AW_LIVE(const int);
static	void	P_proc_AW_CHAR_LOAD_RESULT(const int);
static	void	P_proc_AW_LOGIN_RESULT(const int);
				
static	void	P_proc_MG_INIT(const int);
static	void	P_proc_MG_LIVE(const int);
static	void	P_proc_MG_MOB_REGEN(const int);
static	void	P_proc_MG_MOB_EXTINCT(const int);
static	void	P_proc_MG_MOB_MOVE(const int);
static	void	P_proc_MG_MOB_ATTACK_CHAR(const int);
static	void	P_proc_MG_MOB_DAMAGE_LIST(const int);
static	void	P_proc_MG_MOB_PERFECT_RECOVER(const int);
static	void	P_proc_MG_MOB_SKILL_READY(const int);
static	void	P_proc_MG_MOB_SKILL_ACTION(const int);
static	void	P_proc_MG_MOB_SKILL_CANCEL(const int);

static	void	P_proc_CG_LOGIN(const int);
static	void	P_proc_CG_LIVE(const int);
static	void	P_proc_CG_CHANNEL_LIST(const int);
static	void	P_proc_CG_CHAR_CHANNEL_MOVE(int);
static	void	P_proc_CG_CHAR_LOAD(const int);
static	void	P_proc_CG_CHAR_LOAD_END(const int);
static	void	P_proc_CG_CHAR_MOVE(const int);
static	void	P_proc_CG_CHAR_CHAT(const int);
static	void	P_proc_CG_CHAR_ATTACK_MOB(const int);
static	void	P_proc_CG_CHAR_REGEN(int);
static	void	P_proc_CG_CHAR_ITEM_MOVE(const int);
static	void	P_proc_CG_CHAR_ITEM_PICKUP(const int);
static	void	P_proc_CG_CHAR_ITEM_DROP(const int);
static	void	P_proc_CG_CHAR_ITEM_ENCHANT(const int);
static	void	P_proc_CG_CHAR_ITEM_USE(const int);
static	void	P_proc_CG_CHAR_WARP(int);
static	void	P_proc_CG_CHAR_STORE_ITEM_BUY(const int);
static	void	P_proc_CG_CHAR_STORE_ITEM_SELL(const int);
static	void	P_proc_CG_CHAR_SKILL_READY(const int);
static	void	P_proc_CG_CHAR_SKILL_ACTION(const int);
static	void	P_proc_CG_CHAR_SKILL_CANCEL(const int);
static	void	P_proc_CG_CHAR_SKILL_LEARN(const int);
static	void	P_proc_CG_CHAR_TRANSFORM(const int);
static	void	P_proc_CG_MANAGER_CHAT(const int);


static	struct	recv_q	*P_recv_q;

static	int	P_session;
static	char	P_send_buf[SIZEOF_HEADER_SPECIAL+SIZEOF_MAX_SERV_PACKET_BODY];
static	char	P_send_client_buf[SIZEOF_HEADER_SPECIAL+SIZEOF_MAX_SERV_PACKET_BODY];
static	char	P_send_mobserv_buf[SIZEOF_HEADER_SPECIAL+SIZEOF_MAX_SERV_PACKET_BODY];
static	int	P_send_size;
static	int	P_billing_fd[CONNECTION_BILLING+1];
static	int	P_login_dbagent_fd[CONNECTION_LOGIN_DBAGENT+1];
static	int	P_dbagent_fd[CONNECTION_DBAGENT+1];
static	int	P_mobserv_fd[CHANNEL_PER_MACHINE+1];
static	int	P_char_buff[BUFF_SERIES_CNT];


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
			/*
			//BW
			case	BW_GAMESTART_SUCC:
				P_proc_BW_GAMESTART_SUCC(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	BW_GAMESTART_FAIL:		
				P_proc_BW_GAMESTART_FAIL(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	BW_GAMESTOP_SUCC:
				P_proc_BW_GAMESTOP_SUCC(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	BW_GAMESTOP_FAIL:
				P_proc_BW_GAMESTOP_FAIL(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	BW_USER_CHECK:
				P_proc_BW_USER_CHECK(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	BW_DATABASE_ERROR:
				P_proc_BW_DATABASE_ERROR(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	BW_SYSTEM_ERROR:
				P_proc_BW_SYSTEM_ERROR(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	BW_REGSERVER_SUCC:
				P_proc_BW_REGSERVER_SUCC(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	BW_REGSERVER_FAIL:
				P_proc_BW_REGSERVER_FAIL(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	BW_ALIVECHECK:
				P_proc_BW_ALIVECHECK(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			*/
			
			//INTER
			case	INTER_EXIT_THREAD:
				U_Recv_queue_proc_end();
				goto Exit;
			//case	INTER_CONN_BILLING_FD:
			//	P_proc_INTER_CONN_BILLING_FD(P_recv_q->fd);
			//	U_Recv_queue_proc_end();
			//	break;
			case	INTER_CONN_LOGIN_DBAGENT_FD:
				P_proc_INTER_CONN_LOGIN_DBAGENT_FD(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	INTER_CONN_DBAGENT_FD:
				P_proc_INTER_CONN_DBAGENT_FD(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	INTER_CONN_MOBSERV_FD:
				P_proc_INTER_CONN_MOBSERV_FD(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	INTER_CONN_CLIENT_FD:
				P_proc_INTER_CONN_CLIENT_FD(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	INTER_CLOSE_FD:
				P_proc_INTER_CLOSE_FD(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	INTER_CHAR_RECOVER:
				P_proc_INTER_CHAR_RECOVER(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	INTER_MOB_RECOVER:
				P_proc_INTER_MOB_RECOVER(P_recv_q->fd, P_recv_q->fd_or_seq);
				U_Recv_queue_proc_end();
				break;
			case	INTER_ITEM_REMOVE:
				P_proc_INTER_ITEM_REMOVE(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	INTER_CHAR_BUFF_END:
				P_proc_INTER_CHAR_BUFF_END(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	INTER_MOB_BUFF_END:
				P_proc_INTER_MOB_BUFF_END(P_recv_q->fd, P_recv_q->fd_or_seq);
				U_Recv_queue_proc_end();
				break;
			case	INTER_TICK:
				P_proc_INTER_TICK(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			//AW
			case	AW_INIT_RESULT:
				P_proc_AW_INIT_RESULT(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	AW_LIVE:
				P_proc_AW_LIVE(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	AW_CHAR_LOAD_RESULT:
				P_proc_AW_CHAR_LOAD_RESULT(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	AW_LOGIN_RESULT:
				P_proc_AW_LOGIN_RESULT(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			//MG
			case	MG_INIT:
				P_proc_MG_INIT(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	MG_LIVE:
				P_proc_MG_LIVE(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	MG_MOB_REGEN:
				P_proc_MG_MOB_REGEN(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	MG_MOB_EXTINCT:
				P_proc_MG_MOB_EXTINCT(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	MG_MOB_MOVE:
				P_proc_MG_MOB_MOVE(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	MG_MOB_ATTACK_CHAR:
				P_proc_MG_MOB_ATTACK_CHAR(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	MG_MOB_DAMAGE_LIST:
				P_proc_MG_MOB_DAMAGE_LIST(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	MG_MOB_PERFECT_RECOVER:
				P_proc_MG_MOB_PERFECT_RECOVER(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	MG_MOB_SKILL_READY:
				P_proc_MG_MOB_SKILL_READY(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	MG_MOB_SKILL_ACTION:
				P_proc_MG_MOB_SKILL_ACTION(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	MG_MOB_SKILL_CANCEL:
				P_proc_MG_MOB_SKILL_CANCEL(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			//CG
			case	CG_LOGIN:
				P_proc_CG_LOGIN(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_LIVE:
				P_proc_CG_LIVE(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHANNEL_LIST:
				P_proc_CG_CHANNEL_LIST(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_CHANNEL_MOVE:	
				P_proc_CG_CHAR_CHANNEL_MOVE(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_LOAD:
				P_proc_CG_CHAR_LOAD(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_LOAD_END:
				P_proc_CG_CHAR_LOAD_END(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_MOVE:
				P_proc_CG_CHAR_MOVE(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_CHAT:
				P_proc_CG_CHAR_CHAT(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_ATTACK_MOB:
				P_proc_CG_CHAR_ATTACK_MOB(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_REGEN:
				P_proc_CG_CHAR_REGEN(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_ITEM_MOVE:
				P_proc_CG_CHAR_ITEM_MOVE(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_ITEM_PICKUP:
				P_proc_CG_CHAR_ITEM_PICKUP(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_ITEM_DROP:
				P_proc_CG_CHAR_ITEM_DROP(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_ITEM_ENCHANT:
				P_proc_CG_CHAR_ITEM_ENCHANT(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_ITEM_USE:
				P_proc_CG_CHAR_ITEM_USE(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_WARP:
				P_proc_CG_CHAR_WARP(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_STORE_ITEM_BUY:
				P_proc_CG_CHAR_STORE_ITEM_BUY(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_STORE_ITEM_SELL:
				P_proc_CG_CHAR_STORE_ITEM_SELL(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_SKILL_READY:
				P_proc_CG_CHAR_SKILL_READY(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_SKILL_ACTION:
				P_proc_CG_CHAR_SKILL_ACTION(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_SKILL_CANCEL:
				P_proc_CG_CHAR_SKILL_CANCEL(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_SKILL_LEARN:
				P_proc_CG_CHAR_SKILL_LEARN(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_CHAR_TRANSFORM:
				P_proc_CG_CHAR_TRANSFORM(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CG_MANAGER_CHAT:
				P_proc_CG_MANAGER_CHAT(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
				
			default:
				U_Printf("[PROC]Hack fd(%d) command(%x) user_id(%s)\n",
						P_recv_q->fd, P_recv_q->command, D_GetClient_user_id(P_recv_q->fd));
				P_proc_INTER_CLOSE_FD(P_recv_q->fd);
				U_Recv_queue_proc_end();
		}//end switch
	}//end while  		

Exit:
	U_Printf("\t[PROC]Proc Thread End !\n");
	
	//send GC_SERVER_TROUBLE
	U_Printf("[PROC]send_queue_in BROADCAST GC_SERVER_TROUBLE(%d)\n", SERVER_TROUBLE_INSPECT_TIME);
	P_send_size	= MK_GetPacket_GC_SERVER_TROUBLE(P_send_buf, SERVER_TROUBLE_INSPECT_TIME);
	U_Send_queue_in(SEND_BROADCAST, 0, 0, 0, 0, P_send_size, P_send_buf);
	
	U_Send_queue_in(SEND_EXIT_THREAD, 0, 0, 0, 0, 0, NULL);
	return	(NULL);
}



//_________________________________________________________
//
static	void	P_all_mob_extinct(const int channel_link)
{
	int	i, j, mob_link;


	for( i=1; i<=D_GetMax_map(); i++ )
	{
		for( j=1; j<(VIEW_BLOCK_CNT-1); j++ )
		{
			//send GC_MOB_DISAPPEAR to 3block all
			mob_link	= D_GetMapBlock_mob_link(channel_link, i, j);

			while( mob_link )
			{
				U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_MOB_DISAPPEAR channel_link(%d) map(%d) block_x(%d)(%d)(%d)\n",
														channel_link, i, j-1, j, j+1);
				P_send_size	= MK_GetPacket_GC_MOB_DISAPPEAR(P_send_buf, channel_link, &mob_link, 1);
				U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, i, j, P_send_size, P_send_buf);
				U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, i, j-1, P_send_size, P_send_buf);
				U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, i, j+1, P_send_size, P_send_buf);
			}
		
			//mob clear
			mob_link	= D_GetMapBlock_mob_link(channel_link, i, j);

			while( mob_link )
			{
				D_SetMapBlock_delete_mob(channel_link, mob_link);
				D_SetMob_clear(channel_link, mob_link);
				mob_link	= D_GetMob_block_next(channel_link, mob_link);
			}
		}
	}
}


static	void	P_send_q_in_block_GC_XXXX_APPEAR(const int fd, const int channel_link, const int map, const int block_x)
{
	int	link;

	
	//send GC_OTHER_CHAR_APPEAR
	link	= D_GetMapBlock_char_link(channel_link, map, block_x);

	while( link )
	{
		U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GC_OTHER_CHAR_APPEAR\n", fd, channel_link);
		P_send_size	= MK_GetPacket_GC_OTHER_CHAR_APPEAR(P_send_buf, &link, 1);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
	}	

	//send GC_MOB_APPEAR
	link	= D_GetMapBlock_mob_link(channel_link, map, block_x);
	
	while( link )
	{
		U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GC_MOB_APPEAR\n", fd, channel_link);
		P_send_size	= MK_GetPacket_GC_MOB_APPEAR(P_send_buf, channel_link, &link, 1);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
	}
	
	//send GC_ITEM_APPEAR
	link	= D_GetMapBlock_item_link(channel_link, map, block_x);
	
	while( link )
	{
		U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GC_ITEM_APPEAR\n", fd, channel_link);
		P_send_size	= MK_GetPacket_GC_ITEM_APPEAR(P_send_buf, &link, 1);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
	}
}

			
static	void	P_send_q_in_block_GC_XXXX_DISAPPEAR(const int fd, const int channel_link, const int map, const int block_x)
{
	int	link;


	//send GC_OTHER_CHAR_DISAPPEAR
	link	= D_GetMapBlock_char_link(channel_link, map, block_x);

	while( link )
	{
		U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GC_OTHER_CHAR_DISAPPEAR\n", fd, channel_link);
		P_send_size	= MK_GetPacket_GC_OTHER_CHAR_DISAPPEAR(P_send_buf, &link, 1);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
	}
	
	//send GC_MOB_DISAPPEAR
	link	= D_GetMapBlock_mob_link(channel_link, map, block_x);
	
	while( link )
	{
		U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GC_MOB_DISAPPEAR\n", fd, channel_link);
		P_send_size	= MK_GetPacket_GC_MOB_DISAPPEAR(P_send_buf, channel_link, &link, 1);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
	}
	
	//send GC_ITEM_DISAPPEAR
	link	= D_GetMapBlock_item_link(channel_link, map, block_x);
	
	while( link )
	{
		U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GC_ITEM_DISAPPEAR\n", fd, channel_link);
		P_send_size	= MK_GetPacket_GC_ITEM_DISAPPEAR(P_send_buf, &link, 1);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
	}
}


static	int	P_char_move(int fd, const int dir, const int status, const float speed, const float new_x, const float new_y, const float new_z)
{
	int	channel_link, map, old_block_x, new_block_x, gab_block_x;
	float	gab_x, gab_z, old_x, old_y, old_z;


	D_GetClient_xyz(fd, &old_x, &old_y, &old_z);	
	channel_link	= D_GetClient_channel_link(fd);
	map		= D_GetClient_map(fd);
	old_block_x	= D_GetClient_block_x(fd);
	new_block_x	= (((int)new_x) / VIEW_BLOCK_TILE) + 2;
	gab_block_x	= old_block_x - new_block_x;
	gab_x		= old_x - new_x;
	gab_z		= old_z - new_z;

	//check dir
	if( dir >= 360 )
	{
		U_Printf("[PROC]Error fd(%d) bad dir(%d)\n", fd, dir);
		return (-1);
	}
	
	//check status
	if( status >= _USER_DEAD )
	{
		U_Printf("[PROC]Error fd(%d) bad status(%d)\n", fd, status);
		return (-1);
	}

	//check speed
	if( (speed < 0) || (speed > 10) )
	{
		U_Printf("[PROC]Error fd(%d) bad speed(%f)\n", fd, speed);
		return (-1);
	}

	//check new_x, new_z
	if( (new_x < 1) || (new_x > (MAP_X_TILE-2)) || (new_z < 1) || (new_z > (MAP_Z_TILE-2)) )
	{
		U_Printf("[PROC]Error fd(%d) bad new_x(%f) new_y(%f) new_z(%f)\n", fd, new_x, new_y, new_z);	
		return	(-1);
	}
	
	//check map_attr
	if( D_GetMap_attr(map, new_x, new_z) == MAP_ATTR_COLLISION )
	{
		U_Printf("[PROC]Error fd(%d) new_x(%f) new_y(%f) new_z(%f) bad attr(%d)\n",
					fd, new_x, new_y, new_z, D_GetMap_attr(map, new_x, new_z));	
		return	(-1);
	}
	
	//check gab 
	if( (gab_x < -10) || (gab_x > 10) || (gab_z < -10) || (gab_z > 10) )
	{
		U_Printf("[PROC]Error fd(%d) bad gab old_x(%f) old_y(%f) old_z(%f) new_x(%f) new_y(%f) new_z(%f)\n",
										fd, old_x, old_y, old_z, new_x, new_y, new_z);	
		return	(-1);
	}

	//set dir, status, xyz
	D_SetClient_dir(fd, dir);
	D_SetClient_status(fd, status);
	D_SetClient_xyz(fd, new_x, new_y, new_z);

	//check gab_block_x
	if( gab_block_x )
	{
		//map block delete char
		D_SetMapBlock_delete_char(fd);
		U_Send_queue_in(SEND_INTER_BLOCK_DELETE_CHAR, fd, 0, 0, 0, 0, NULL);
		
		if( gab_block_x < 0 )
		{	
			//send GC_OTHER_CHAR_DISAPPEAR to 1block other
			//U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_OTHER_CHAR_DISAPPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, old_block_x-1);
			P_send_size	= MK_GetPacket_GC_OTHER_CHAR_DISAPPEAR(P_send_buf, &fd, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, old_block_x-1, P_send_size, P_send_buf);

			//send GC_OTHER_CHAR_APPEAR to 1block other
			//U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_OTHER_CHAR_APPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, new_block_x+1);
			P_send_size	= MK_GetPacket_GC_OTHER_CHAR_APPEAR(P_send_buf, &fd, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x+1, P_send_size, P_send_buf);

			//send GC_CHAR_MOVE to 2block other
			//U_Printf("[PROC]send_queue_in BLOCK_OTHERCAST GC_CHAR_MOVE user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, new_block_x-1, new_block_x);
			P_send_size	= MK_GetPacket_GC_CHAR_MOVE(P_send_buf, fd, dir, status, speed, new_x, new_y, new_z);	
			U_Send_queue_in(SEND_BLOCK_OTHERCAST, fd, channel_link, map, new_block_x, P_send_size, P_send_buf);
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x-1, P_send_size, P_send_buf);

			//send 1block GC_XXXX_DISAPPEAR to own
			P_send_q_in_block_GC_XXXX_DISAPPEAR(fd, channel_link, map, old_block_x-1);

			//send 1block GC_XXXX_APPEAR to own
			P_send_q_in_block_GC_XXXX_APPEAR(fd, channel_link, map, new_block_x+1);
		}
		else
		{
			//send GC_OTHER_CHAR_DISAPEAR to 1block other
			//U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_OTHER_CHAR_DISAPPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, old_block_x+1);
			P_send_size	= MK_GetPacket_GC_OTHER_CHAR_DISAPPEAR(P_send_buf, &fd, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, old_block_x+1, P_send_size, P_send_buf);

			//send GC_OTHER_CHAR_APPEAR to 1block other
			//U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_OTHER_CHAR_APPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, new_block_x-1);
			P_send_size	= MK_GetPacket_GC_OTHER_CHAR_APPEAR(P_send_buf, &fd, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x-1, P_send_size, P_send_buf);

			//send GC_CHAR_MOVE to 2block other
			//U_Printf("[PROC]send_queue_in BLOCK_OTHERCAST GC_CHAR_MOVE user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, new_block_x, new_block_x+1);
			P_send_size	= MK_GetPacket_GC_CHAR_MOVE(P_send_buf, fd, dir, status, speed, new_x, new_y, new_z);	
			U_Send_queue_in(SEND_BLOCK_OTHERCAST, fd, channel_link, map, new_block_x, P_send_size, P_send_buf);
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x+1, P_send_size, P_send_buf);

			//send 1block GC_XXXX_DISAPPEAR to own
			P_send_q_in_block_GC_XXXX_DISAPPEAR(fd, channel_link, map, old_block_x+1);

			//send 1block GC_XXXX_APPEAR to own 
			P_send_q_in_block_GC_XXXX_APPEAR(fd, channel_link, map, new_block_x-1);
		}	
	
		//set block_x		
		D_SetClient_block_x(fd, new_block_x);		
		
		//map block insert char
		D_SetMapBlock_insert_char(fd);
		U_Send_queue_in(SEND_INTER_BLOCK_INSERT_CHAR, fd, channel_link, map, new_block_x, 0, NULL);
	}		
	else
	{
		//send GC_CHAR_MOVE to 3block other 
		//U_Printf("[PROC]send_queue_in BLOCK_OTHERCAST GC_CHAR_MOVE user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)(%d)\n",
		//	D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, new_block_x-1, new_block_x, new_block_x+1);
		P_send_size	= MK_GetPacket_GC_CHAR_MOVE(P_send_buf, fd, dir, status, speed, new_x, new_y, new_z);	
		U_Send_queue_in(SEND_BLOCK_OTHERCAST, fd, channel_link, map, new_block_x, P_send_size, P_send_buf);
		U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x-1, P_send_size, P_send_buf);
		U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x+1, P_send_size, P_send_buf);
	}


	//send GM_CHAR_MOVE
	if( P_mobserv_fd[channel_link] && (((int)old_x != (int)new_x) || ((int)old_z != (int)new_z)) )
	{
		//U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GM_CHAR_MOVE user_id(%s) char_name(%s)\n",
		//			P_mobserv_fd[channel_link], channel_link, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_send_size	= MK_GetPacket_GM_CHAR_MOVE(P_send_buf, fd, status, new_x, new_y, new_z);	
		U_Send_queue_in(SEND_UNICAST, P_mobserv_fd[channel_link], 0, 0, 0, P_send_size, P_send_buf);
	}

	return	(0);	
} 	


static	int	P_char_attack_mob(int fd, int mob_id, const int attack_id, const int attack_type,
							const int dir, const float speed, const float new_x, const float new_y, const float new_z)
{
	int	channel_link, map, old_block_x, new_block_x, gab_block_x, mob_hp, damage_type, damage, attack_block_x;
	float	gab_x, gab_z, old_x, old_y, old_z;


	D_GetClient_xyz(fd, &old_x, &old_y, &old_z);	
	channel_link	= D_GetClient_channel_link(fd);
	map		= D_GetClient_map(fd);
	old_block_x	= D_GetClient_block_x(fd);
	new_block_x	= (((int)new_x) / VIEW_BLOCK_TILE) + 2;
	gab_block_x	= old_block_x - new_block_x;
	gab_x		= old_x - new_x;
	gab_z		= old_z - new_z;

	//check mob_id
	if( mob_id > D_GetMax_mob() )
	{
		U_Printf("[PROC]Error fd(%d) bad mob_id(%d)\n", fd, mob_id);
		return (-1);
	}
	
	//check attack_type
	if( attack_type > _ATTACK_PIERCE )
	{
		U_Printf("[PROC]Error fd(%d) bad attack_type(%d)\n", fd, attack_type);
		return (-1);
	}

	//check dir
	if( dir >= 360 )
	{
		U_Printf("[PROC]Error fd(%d) bad dir(%d)\n", fd, dir);
		return (-1);
	}
	
	//check speed
	if( (speed < 0) || (speed > 10) )
	{
		U_Printf("[PROC]Error fd(%d) bad speed(%f)\n", fd, speed);
		return (-1);
	}

	//check new_x, new_z
	if( (new_x < 1) || (new_x > (MAP_X_TILE-2)) || (new_z < 1) || (new_z > (MAP_Z_TILE-2)) )
	{
		U_Printf("[PROC]Error fd(%d) bad new_x(%f) new_y(%f) new_z(%f)\n", fd, new_x, new_y, new_z);	
		return	(-1);
	}
	
	//check map_attr
	if( D_GetMap_attr(map, new_x, new_z) == MAP_ATTR_COLLISION )
	{
		U_Printf("[PROC]Error fd(%d) new_x(%f) new_y(%f) new_z(%f) bad attr(%d)\n",
					fd, new_x, new_y, new_z, D_GetMap_attr(map, new_x, new_z));	
		return	(-1);
	}
	
	//check gab 
	if( (gab_x < -10) || (gab_x > 10) || (gab_z < -10) || (gab_z > 10) )
	{
		U_Printf("[PROC]Error fd(%d) bad gab old_x(%f) old_y(%f) old_z(%f) new_x(%f) new_y(%f) new_z(%f)\n",
				fd, old_x, old_y, old_z, new_x, new_y, new_z);	
		return	(-1);
	}
	
	//set dir, xyz
	D_SetClient_dir(fd, dir);
	D_SetClient_xyz(fd, new_x, new_y, new_z);

	//attack 
	if( (mob_id == 0) || (D_GetMob_status(channel_link, mob_id) >= _MOB_DEATH) )
	{
		mob_id		= 0;
		mob_hp		= 0;
		damage_type	= _DAMAGE_NOTING;
		damage		= 0;
	
		attack_block_x	= new_block_x;
	}
	else
	{
		if( D_Char_attack_mob(fd, mob_id, attack_type, &mob_hp, &damage_type, &damage) )
		{
			U_Printf("[PROC]Error fd(%d) channel_link(%d) D_Char_attack_mob()\n", fd, channel_link);
			return (-1);
		}
		
		attack_block_x	= D_GetMob_block_x(channel_link, mob_id);
	}
		
	//set last_attack_type
	D_SetClient_last_attack_type(fd, attack_type);

	//check gab_block_x
	if( gab_block_x )
	{
		//map block delete char
		D_SetMapBlock_delete_char(fd);
		U_Send_queue_in(SEND_INTER_BLOCK_DELETE_CHAR, fd, 0, 0, 0, 0, NULL);
		
		if( gab_block_x < 0 )
		{	
			//send GC_OTHER_CHAR_DISAPPEAR to 1block other
			U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_OTHER_CHAR_DISAPPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, old_block_x-1);
			P_send_size	= MK_GetPacket_GC_OTHER_CHAR_DISAPPEAR(P_send_buf, &fd, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, old_block_x-1, P_send_size, P_send_buf);

			//send GC_OTHER_CHAR_APPEAR to 1block other
			U_Printf(
			"[PROC]send_queue_in BLOCK_ALLCAST GC_OTHER_CHAR_APPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)\n",
				D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, new_block_x+1);
			P_send_size	= MK_GetPacket_GC_OTHER_CHAR_APPEAR(P_send_buf, &fd, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x+1, P_send_size, P_send_buf);

			//send 1block GC_XXXX_DISAPPEAR to own
			P_send_q_in_block_GC_XXXX_DISAPPEAR(fd, channel_link, map, old_block_x-1);

			//send 1block GC_XXXX_APPEAR to own
			P_send_q_in_block_GC_XXXX_APPEAR(fd, channel_link, map, new_block_x+1);
		}
		else
		{
			//send GC_OTHER_CHAR_DISAPEAR to 1block other
			U_Printf(
			"[PROC]send_queue_in BLOCK_ALLCAST GC_OTHER_CHAR_DISAPPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)\n",
				D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, old_block_x+1);
			P_send_size	= MK_GetPacket_GC_OTHER_CHAR_DISAPPEAR(P_send_buf, &fd, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, old_block_x+1, P_send_size, P_send_buf);

			//send GC_OTHER_CHAR_APPEAR to 1block other
			U_Printf(
			"[PROC]send_queue_in BLOCK_ALLCAST GC_OTHER_CHAR_APPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)\n",
				D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, new_block_x-1);
			P_send_size	= MK_GetPacket_GC_OTHER_CHAR_APPEAR(P_send_buf, &fd, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x-1, P_send_size, P_send_buf);

			//send 1block GC_XXXX_DISAPPEAR to own
			P_send_q_in_block_GC_XXXX_DISAPPEAR(fd, channel_link, map, old_block_x+1);

			//send 1block GC_XXXX_APPEAR to own 
			P_send_q_in_block_GC_XXXX_APPEAR(fd, channel_link, map, new_block_x-1);
		}	
	
		//set block_x		
		D_SetClient_block_x(fd, new_block_x);		
		
		//map block insert char
		D_SetMapBlock_insert_char(fd);
		U_Send_queue_in(SEND_INTER_BLOCK_INSERT_CHAR, fd, channel_link, map, new_block_x, 0, NULL);
	}	
	
	if( P_mobserv_fd[channel_link] )
	{
		if( damage == 0 )
		{
			if( ((int)old_x != (int)new_x) || ((int)old_z != (int)new_z) )
			{
				//send GM_CHAR_MOVE
				U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GM_CHAR_MOVE user_id(%s) char_name(%s)\n",
						P_mobserv_fd[channel_link], channel_link, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
				P_send_size	= MK_GetPacket_GM_CHAR_MOVE(P_send_buf, fd, D_GetClient_status(fd), new_x, new_y, new_z);	
				U_Send_queue_in(SEND_UNICAST, P_mobserv_fd[channel_link], 0, 0, 0, P_send_size, P_send_buf);
			}
		}
		else
		{
			//send GM_CHAR_ATTACK_MOB	
			U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GM_CHAR_ATTACK_MOB user_id(%s) char_name(%s)\n",
						P_mobserv_fd[channel_link], channel_link, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
			P_send_size	= MK_GetPacket_GM_CHAR_ATTACK_MOB(P_send_buf, fd,
										mob_id, mob_hp, damage_type, damage, dir, new_x, new_y, new_z);
			U_Send_queue_in(SEND_UNICAST, P_mobserv_fd[channel_link], 0, 0, 0, P_send_size, P_send_buf);
		}
	}
	
	//send GC_CHAR_ATTACK_MOB to 3block all
	U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_CHAR_ATTACK_MOB user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)(%d) damage(%d) mob_id(%d) mob_hp(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, attack_block_x-1, attack_block_x, attack_block_x+1, damage, mob_id, mob_hp);
	P_send_size	= MK_GetPacket_GC_CHAR_ATTACK_MOB(P_send_buf, fd, mob_id, attack_id, attack_type, mob_hp, damage_type, damage,
														dir, speed, new_x, new_y, new_z);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, attack_block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, attack_block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, attack_block_x+1, P_send_size, P_send_buf);
	
	return	(0);	
}


static	int	P_char_skill_ready(int fd, const int skill_type, 
						const int dir, const float speed, const float new_x, const float new_y, const float new_z)
{
	int	channel_link, map, old_block_x, new_block_x, gab_block_x;
	float	gab_x, gab_z, old_x, old_y, old_z;


	D_GetClient_xyz(fd, &old_x, &old_y, &old_z);	
	channel_link	= D_GetClient_channel_link(fd);
	map		= D_GetClient_map(fd);
	old_block_x	= D_GetClient_block_x(fd);
	new_block_x	= (((int)new_x) / VIEW_BLOCK_TILE) + 2;
	gab_block_x	= old_block_x - new_block_x;
	gab_x		= old_x - new_x;
	gab_z		= old_z - new_z;

	//check skill_type
	if( (skill_type < 1) || (skill_type > CHAR_SKILL_CNT) )
	{
		U_Printf("[PROC]Error fd(%d) bad skill_type(%d)\n", fd, skill_type);
		return (-1);
	}
	
	//check dir
	if( dir >= 360 )
	{
		U_Printf("[PROC]Error fd(%d) bad dir(%d)\n", fd, dir);
		return (-1);
	}
	
	//check speed
	if( (speed < 0) || (speed > 10) )
	{
		U_Printf("[PROC]Error fd(%d) bad speed(%f)\n", fd, speed);
		return (-1);
	}

	//check new_x, new_z
	if( (new_x < 1) || (new_x > (MAP_X_TILE-2)) || (new_z < 1) || (new_z > (MAP_Z_TILE-2)) )
	{
		U_Printf("[PROC]Error fd(%d) bad new_x(%f) new_y(%f) new_z(%f)\n", fd, new_x, new_y, new_z);	
		return	(-1);
	}
	
	//check map_attr
	if( D_GetMap_attr(map, new_x, new_z) == MAP_ATTR_COLLISION )
	{
		U_Printf("[PROC]Error fd(%d) new_x(%f) new_y(%f) new_z(%f) bad attr(%d)\n",
					fd, new_x, new_y, new_z, D_GetMap_attr(map, new_x, new_z));	
		return	(-1);
	}
	
	//check gab 
	if( (gab_x < -10) || (gab_x > 10) || (gab_z < -10) || (gab_z > 10) )
	{
		U_Printf("[PROC]Error fd(%d) bad gab old_x(%f) old_y(%f) old_z(%f) new_x(%f) new_y(%f) new_z(%f)\n",
				fd, old_x, old_y, old_z, new_x, new_y, new_z);	
		return	(-1);
	}
	
	//set dir, xyz
	D_SetClient_dir(fd, dir);
	D_SetClient_xyz(fd, new_x, new_y, new_z);

	//skill ready
	if( D_SetClient_skill_ready(fd, skill_type) )
	{
		U_Printf("[PROC]Error fd(%d) D_Char_attack_mob()\n", fd);
		return (-1);
	}

	//check gab_block_x
	if( gab_block_x )
	{
		//map block delete char
		D_SetMapBlock_delete_char(fd);
		U_Send_queue_in(SEND_INTER_BLOCK_DELETE_CHAR, fd, 0, 0, 0, 0, NULL);
		
		if( gab_block_x < 0 )
		{	
			//send GC_OTHER_CHAR_DISAPPEAR to 1block other
			U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_OTHER_CHAR_DISAPPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, old_block_x-1);
			P_send_size	= MK_GetPacket_GC_OTHER_CHAR_DISAPPEAR(P_send_buf, &fd, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, old_block_x-1, P_send_size, P_send_buf);

			//send GC_OTHER_CHAR_APPEAR to 1block other
			U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_OTHER_CHAR_APPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, new_block_x+1);
			P_send_size	= MK_GetPacket_GC_OTHER_CHAR_APPEAR(P_send_buf, &fd, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x+1, P_send_size, P_send_buf);

			//send 1block GC_XXXX_DISAPPEAR to own
			P_send_q_in_block_GC_XXXX_DISAPPEAR(fd, channel_link, map, old_block_x-1);

			//send 1block GC_XXXX_APPEAR to own
			P_send_q_in_block_GC_XXXX_APPEAR(fd, channel_link, map, new_block_x+1);
		}
		else
		{
			//send GC_OTHER_CHAR_DISAPEAR to 1block other
			U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_OTHER_CHAR_DISAPPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, old_block_x+1);
			P_send_size	= MK_GetPacket_GC_OTHER_CHAR_DISAPPEAR(P_send_buf, &fd, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, old_block_x+1, P_send_size, P_send_buf);

			//send GC_OTHER_CHAR_APPEAR to 1block other
			U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_OTHER_CHAR_APPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, new_block_x-1);
			P_send_size	= MK_GetPacket_GC_OTHER_CHAR_APPEAR(P_send_buf, &fd, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x-1, P_send_size, P_send_buf);

			//send 1block GC_XXXX_DISAPPEAR to own
			P_send_q_in_block_GC_XXXX_DISAPPEAR(fd, channel_link, map, old_block_x+1);

			//send 1block GC_XXXX_APPEAR to own 
			P_send_q_in_block_GC_XXXX_APPEAR(fd, channel_link, map, new_block_x-1);
		}	
	
		//set block_x		
		D_SetClient_block_x(fd, new_block_x);		
		
		//map block insert char
		D_SetMapBlock_insert_char(fd);
		U_Send_queue_in(SEND_INTER_BLOCK_INSERT_CHAR, fd, channel_link, map, new_block_x, 0, NULL);
	}		
	
	if( P_mobserv_fd[channel_link] )
	{
		if( ((int)old_x != (int)new_x) || ((int)old_z != (int)new_z) )
		{
			//send GM_CHAR_MOVE
			U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GM_CHAR_MOVE user_id(%s) char_name(%s)\n",
						P_mobserv_fd[channel_link], channel_link, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
			P_send_size	= MK_GetPacket_GM_CHAR_MOVE(P_send_buf, fd, D_GetClient_status(fd), new_x, new_y, new_z);	
			U_Send_queue_in(SEND_UNICAST, P_mobserv_fd[channel_link], 0, 0, 0, P_send_size, P_send_buf);
		}
	}

	//send GC_CHAR_SKILL_READY to 3block other
	U_Printf("[PROC]send_queue_in BLOCK_OTHERCAST GC_CHAR_SKILL_READY user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)(%d)\n",
				D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, new_block_x-1, new_block_x, new_block_x+1);
	P_send_size	= MK_GetPacket_GC_CHAR_SKILL_READY(P_send_buf, fd, skill_type, dir, speed, new_x, new_y, new_z);
	U_Send_queue_in(SEND_BLOCK_OTHERCAST, fd, channel_link, map, new_block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x+1, P_send_size, P_send_buf);
	
	return	(0);	
}


static	int	P_char_skill_action(int fd, struct body1_CG_CHAR_SKILL_ACTION *body1,
						const int dir, const float speed, const float new_x, const float new_y, const float new_z)
{
	int					i, j, channel_link, map, old_block_x, new_block_x, gab_block_x;
	float					gab_x, gab_z, old_x, old_y, old_z;
	struct	skill_action_result		result;
	struct	body1_GM_CHAR_SKILL_ACTION	*mobserv_send_body1
							= (struct body1_GM_CHAR_SKILL_ACTION *)(P_send_mobserv_buf + SIZEOF_HEADER_NORMAL);


	D_GetClient_xyz(fd, &old_x, &old_y, &old_z);
	channel_link	= D_GetClient_channel_link(fd);
	map		= D_GetClient_map(fd);
	old_block_x	= D_GetClient_block_x(fd);
	new_block_x	= (((int)new_x) / VIEW_BLOCK_TILE) + 2;
	gab_block_x	= old_block_x - new_block_x;
	gab_x		= old_x - new_x;
	gab_z		= old_z - new_z;

	//check skill_type
	if( (body1->skill_type < 1) || (body1->skill_type > CHAR_SKILL_CNT) )
	{
		U_Printf("[PROC]Error fd(%d) bad skill_type(%d)\n", fd, body1->skill_type);
		return (-1);
	}
	
	//check dir
	if( dir >= 360 )
	{
		U_Printf("[PROC]Error fd(%d) bad dir(%d)\n", fd, dir);
		return (-1);
	}
	
	//check speed
	if( (speed < 0) || (speed > 10) )
	{
		U_Printf("[PROC]Error fd(%d) bad speed(%f)\n", fd, speed);
		return (-1);
	}

	//check new_x, new_z
	if( (new_x < 1) || (new_x > (MAP_X_TILE-2)) || (new_z < 1) || (new_z > (MAP_Z_TILE-2)) )
	{
		U_Printf("[PROC]Error fd(%d) bad new_x(%f) new_y(%f) new_z(%f)\n", fd, new_x, new_y, new_z);	
		return	(-1);
	}
	
	//check map_attr
	if( D_GetMap_attr(map, new_x, new_z) == MAP_ATTR_COLLISION )
	{
		U_Printf("[PROC]Error fd(%d) new_x(%f) new_y(%f) new_z(%f) bad attr(%d)\n",
					fd, new_x, new_y, new_z, D_GetMap_attr(map, new_x, new_z));	
		return	(-1);
	}
	
	//check gab 
	if( (gab_x < -10) || (gab_x > 10) || (gab_z < -10) || (gab_z > 10) )
	{
		U_Printf("[PROC]Error fd(%d) bad gab old_x(%f) old_y(%f) old_z(%f) new_x(%f) new_y(%f) new_z(%f)\n",
				fd, old_x, old_y, old_z, new_x, new_y, new_z);	
		return	(-1);
	}
	
	//check target xz 
	if( (body1->target_x < 1) || (body1->target_x > (MAP_X_TILE-2)) || (body1->target_z < 1) || (body1->target_z > (MAP_Z_TILE-2)) )
	{
		U_Printf("[PROC]Error fd(%d) bad target_x(%f) target_y(%f) target_z(%f)\n",
						fd, body1->target_x, body1->target_y, body1->target_z);	
		return	(-1);
	}
	
	//set dir, xyz
	D_SetClient_dir(fd, dir);
	D_SetClient_xyz(fd, new_x, new_y, new_z);
	
	//skill action	
	if( D_Char_skill_action(fd, body1, P_send_client_buf, P_send_mobserv_buf, &result) )	
	{
		U_Printf("[PROC]Error fd(%d) channel_link(%d) D_Char_skill_action()\n", fd, channel_link);
		return	(-1);
	}

	//check gab_block_x
	if( gab_block_x )
	{
		//map block delete char
		D_SetMapBlock_delete_char(fd);
		U_Send_queue_in(SEND_INTER_BLOCK_DELETE_CHAR, fd, 0, 0, 0, 0, NULL);
		
		if( gab_block_x < 0 )
		{	
			//send GC_OTHER_CHAR_DISAPPEAR to 1block other
			U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_OTHER_CHAR_DISAPPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, old_block_x-1);
			P_send_size	= MK_GetPacket_GC_OTHER_CHAR_DISAPPEAR(P_send_buf, &fd, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, old_block_x-1, P_send_size, P_send_buf);

			//send GC_OTHER_CHAR_APPEAR to 1block other
			U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_OTHER_CHAR_APPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, new_block_x+1);
			P_send_size	= MK_GetPacket_GC_OTHER_CHAR_APPEAR(P_send_buf, &fd, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x+1, P_send_size, P_send_buf);

			//send 1block GC_XXXX_DISAPPEAR to own
			P_send_q_in_block_GC_XXXX_DISAPPEAR(fd, channel_link, map, old_block_x-1);

			//send 1block GC_XXXX_APPEAR to own
			P_send_q_in_block_GC_XXXX_APPEAR(fd, channel_link, map, new_block_x+1);
		}
		else
		{
			//send GC_OTHER_CHAR_DISAPEAR to 1block other
			U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_OTHER_CHAR_DISAPPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, old_block_x+1);
			P_send_size	= MK_GetPacket_GC_OTHER_CHAR_DISAPPEAR(P_send_buf, &fd, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, old_block_x+1, P_send_size, P_send_buf);

			//send GC_OTHER_CHAR_APPEAR to 1block other
			U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_OTHER_CHAR_APPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, new_block_x-1);
			P_send_size	= MK_GetPacket_GC_OTHER_CHAR_APPEAR(P_send_buf, &fd, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x-1, P_send_size, P_send_buf);

			//send 1block GC_XXXX_DISAPPEAR to own
			P_send_q_in_block_GC_XXXX_DISAPPEAR(fd, channel_link, map, old_block_x+1);

			//send 1block GC_XXXX_APPEAR to own 
			P_send_q_in_block_GC_XXXX_APPEAR(fd, channel_link, map, new_block_x-1);
		}	
	
		//set block_x		
		D_SetClient_block_x(fd, new_block_x);		
		
		//map block insert char
		D_SetMapBlock_insert_char(fd);
		U_Send_queue_in(SEND_INTER_BLOCK_INSERT_CHAR, fd, channel_link, map, new_block_x, 0, NULL);
	}		
	
	if( P_mobserv_fd[channel_link] )
	{
		//send GM_CHAR_SKILL_ACTION
		if( mobserv_send_body1->target_mob_cnt )
		{ 
			U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GM_CHAR_SKILL_ACTION fd(%d) user_id(%s) char_name(%s)\n",
						P_mobserv_fd[channel_link], channel_link, fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
			P_send_size	= MK_GetPacket_GM_CHAR_SKILL_ACTION(P_send_mobserv_buf);
			U_Send_queue_in(SEND_UNICAST, P_mobserv_fd[channel_link], 0, 0, 0, P_send_size, P_send_mobserv_buf);
		}

		//send GM_MOB_BUFF
		for( i=0; i<result.buff_cnt; i++ )
		{
			for( j=0; j<result.buff_mob_cnt; j++)
			{
				U_Printf("[PROC]send_queue_in UNICAST(%d) GM_MOB_BUFF mob_id(%d)\n",
								P_mobserv_fd[channel_link], result.buff_mob_list[j]);
				P_send_size	= MK_GetPacket_GM_MOB_BUFF(P_send_buf, channel_link, result.buff_mob_list[j], result.buff_serise[i]);
				U_Send_queue_in(SEND_UNICAST, P_mobserv_fd[channel_link], 0, 0, 0, P_send_size, P_send_buf);
			}			
		}
	}
	
	//send GC_CHAR_SKILL_ACTION to 5block all
	U_Printf("[PROC]send_queue_in 5block BLOCK_ALLCAST GC_CHAR_SKILL_ACTION skill(%d) user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)(%d)(%d)(%d)\n", body1->skill_type, D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, new_block_x-2, new_block_x-1, new_block_x, new_block_x+1, new_block_x+2);
	P_send_size	= MK_GetPacket_GC_CHAR_SKILL_ACTION(P_send_client_buf);	
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x, P_send_size, P_send_client_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x-1, P_send_size, P_send_client_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x+1, P_send_size, P_send_client_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x-2, P_send_size, P_send_client_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x+2, P_send_size, P_send_client_buf);

	//send GC_CHAR_BUFF
	for( i=0; i<result.buff_cnt; i++ )
	{
		for( j=0; j<result.buff_char_cnt; j++)
		{
			U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_BUFF(%d) user_id(%s) char_name(%s)\n", result.buff_char_list[j],
			result.buff_serise[i], D_GetClient_user_id(result.buff_char_list[j]), D_GetClient_char_name(result.buff_char_list[j]));
			P_send_size	= MK_GetPacket_GC_CHAR_BUFF(P_send_buf, result.buff_char_list[j], result.buff_serise[i]);
			U_Send_queue_in(SEND_UNICAST, result.buff_char_list[j], 0, 0, 0, P_send_size, P_send_buf);
		}		
	}

	return	(0);				
}	


static	int	P_mob_move(const int channel_link, int mob_id,
					const int dir, const int status, const float speed, const float new_x, const float new_y, const float new_z)
{
	int	map, old_block_x, new_block_x, gab_block_x;
	float	gab_x, gab_z, old_x, old_y, old_z;


	D_GetMob_xyz(channel_link, mob_id, &old_x, &old_y, &old_z);	
	map		= D_GetMob_map(channel_link, mob_id);
	old_block_x	= D_GetMob_block_x(channel_link, mob_id);
	new_block_x	= (((int)new_x) / VIEW_BLOCK_TILE) + 2;
	gab_block_x	= old_block_x - new_block_x;
	gab_x		= old_x - new_x;
	gab_z		= old_z - new_z;

	
	//check dir
	if( (dir < 0) || (dir >= 360) )
	{	
		U_Printf("[PROC]Error mob_id(%d) bad dir(%d)\n", mob_id, dir);
		return	(-1);
	}

	//check status
	if( (status < 0 ) || (status >= _MOB_DEATH) )
	{
		U_Printf("[PROC]Error mob_id(%d) bad status(%d)\n", mob_id, status);
		return	(-1);
	}

	//check speed
	if( (speed < 0) || (speed > 10) )
	{
		U_Printf("[PROC]Error mob_id(%d) bad speed(%f)\n", mob_id, speed);
		return	(-1);
	}

	//check new_x, new_y, new_z
	if( (new_x < 1) || (new_x > (MAP_X_TILE-2)) || (new_y) || (new_z < 1) || (new_z > (MAP_Z_TILE-2)) )
	{
		U_Printf("[PROC]Error mob_id(%d) bad new_x(%f) new_y(%f) new_z(%f)\n", mob_id, new_x, new_y, new_z);	
		return	(-1);
	}
	
	//check gab 
	if( (gab_x < -10) || (gab_x > 10) || (gab_z < -10) || (gab_z > 10) )
	{
		U_Printf("[PROC]Error mob_id(%d) bad gab old_x(%f) old_y(%f) old_z(%f) new_x(%f) new_y(%f) new_z(%f)\n",
				mob_id, old_x, old_y, old_z, new_x, new_y, new_z);	
		return	(-1);
	}

	//set dir, status, xyz
	D_SetMob_dir(channel_link, mob_id, dir);
	D_SetMob_status(channel_link, mob_id, status);
	D_SetMob_xyz(channel_link, mob_id, new_x, new_y, new_z);

	//check gab_block_x
	if( gab_block_x )
	{
		//map block delete mob
		D_SetMapBlock_delete_mob(channel_link, mob_id);
		
		if( gab_block_x < 0 )
		{
			//send GC_MOB_DISAPPEAR to 1block char
			//U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_MOB_DISAPPEAR channel_link(%d) mob_id(%d) map(%d) block_x(%d)\n", 
			//										channel_link, mob_id,  map, old_block_x-1);
			P_send_size	= MK_GetPacket_GC_MOB_DISAPPEAR(P_send_buf, channel_link, &mob_id, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, old_block_x-1, P_send_size, P_send_buf);

			//send GC_MOB_APPEAR to 1block char
			//U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_MOB_APPEAR channel_link(%d) mob_id(%d) map(%d) block_x(%d)\n",
			//										channel_link, mob_id, map, new_block_x+1);
			P_send_size	= MK_GetPacket_GC_MOB_APPEAR(P_send_buf, channel_link, &mob_id, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x+1, P_send_size, P_send_buf);

			//send GC_MOB_MOVE to 2block char 
			//U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_MOB_MOVE channel_link(%d) mob_id(%d) map(%d) block_x(%d)(%d)\n",
			//								channel_link, mob_id, map, new_block_x-1, new_block_x);
			P_send_size	= MK_GetPacket_GC_MOB_MOVE(P_send_buf, mob_id, dir, status, speed, new_x, new_y, new_z);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x, P_send_size, P_send_buf);
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x-1, P_send_size, P_send_buf);
		}
		else
		{
			//send GC_MOB_DISAPEAR to 1block char
			//U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_MOB_DISAPPEAR channel_link(%d) mob_id(%d) map(%d) block_x(%d)\n",
			//										channel_link, mob_id, map, old_block_x+1);
			P_send_size	= MK_GetPacket_GC_MOB_DISAPPEAR(P_send_buf, channel_link, &mob_id, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, old_block_x+1, P_send_size, P_send_buf);

			//send GC_MOB_APPEAR to 1block char
			//U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_MOB_APPEAR channel_link(%d) mob_id(%d) map(%d) block_x(%d)\n",
			//										channel_link, mob_id, map, new_block_x-1);
			P_send_size	= MK_GetPacket_GC_MOB_APPEAR(P_send_buf, channel_link, &mob_id, 0);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x-1, P_send_size, P_send_buf);

			//send GC_MOB_MOVE to 2block char
			//U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_MOB_MOVE channel_link(%d) mob_id(%d) map(%d) block_x(%d)(%d)\n",
			//								channel_link, mob_id, map, new_block_x, new_block_x+1);
			P_send_size	= MK_GetPacket_GC_MOB_MOVE(P_send_buf, mob_id, dir, status, speed, new_x, new_y, new_z);	
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x, P_send_size, P_send_buf);
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x+1, P_send_size, P_send_buf);
		}
		
		//set block_x		
		D_SetMob_block_x(channel_link, mob_id, new_block_x);		
		
		//map block insert mob
		D_SetMapBlock_insert_mob(channel_link, mob_id);
	}
	else
	{
		//send GC_MOB_MOVE to 3block all
		//U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_MOB_MOVE channel_link(%d) mob_id(%d) map(%d) block_x(%d)(%d)(%d)\n",
		//		channel_link(%d) mob_id, map, new_block_x-1, new_block_x, new_block_x+1);
		P_send_size	= MK_GetPacket_GC_MOB_MOVE(P_send_buf, mob_id, dir, status, speed, new_x, new_y, new_z);	
		U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x, P_send_size, P_send_buf);
		U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x-1, P_send_size, P_send_buf);
		U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, new_block_x+1, P_send_size, P_send_buf);
	}	
	
	return	(0);	
}


//_________________________________________________________
//
static	void	P_remove_client(int fd)
{
	int	channel_link, billing_idx, use_billing_fd, use_login_dbagent_fd, use_dbagent_fd, map, block_x;


	//billing
	if( (billing_idx = D_GetClient_billing_idx(fd)) )
	{
		U_Printf("[PROC]do noting logout fd(%d) user_id(%s)\n", fd, D_GetClient_user_id(fd));
		P_billing_fd[billing_idx]	= 0;
		D_SetClient_clear(fd);
		U_Send_queue_in(SEND_INTER_CLOSE_FD, fd, 0, 0, 0, 0, NULL);

		//for( i=1; i<=MAX_FD; i++ )
		//{
		//	if( (D_GetClient_sec_active(i)) && (*D_GetClient_user_id(i)) && (D_GetClient_use_billing_idx(i)) )
		//	{
		//		//send GC_SERVER_STOP
		//		U_Printf("[PROC]send_queue_in UNICAST(%d) GC_SERVER_TROUBLE(%d)\n", i, SERVER_TROUBLE_BILLING);
		//		P_send_size	= MK_GetPacket_GC_SERVER_TROUBLE(P_send_buf, SERVER_TROUBLE_BILLING);
		//		U_Send_queue_in(SEND_UNICAST, i, 0, 0, 0, P_send_size, P_send_buf);

		//		P_remove_client(i);
		//	}
		//}

		return;
	}
	
	//login_dbagent
	if( D_GetClient_login_dbagent_idx(fd) )
	{
		P_login_dbagent_fd[D_GetClient_login_dbagent_idx(fd)]	= 0;
		D_SetClient_clear(fd);
		U_Send_queue_in(SEND_INTER_CLOSE_FD, fd, 0, 0, 0, 0, NULL);
		return;
	}

	//dbagent
	if( D_GetClient_dbagent_idx(fd) )
	{
		P_dbagent_fd[D_GetClient_dbagent_idx(fd)]	= 0;
		D_SetClient_clear(fd);
		U_Send_queue_in(SEND_INTER_CLOSE_FD, fd, 0, 0, 0, 0, NULL);
		return;
	}

	//mob 
	if( D_GetClient_mobserv_charge_channel_link(fd) )
	{
		P_all_mob_extinct(D_GetClient_mobserv_charge_channel_link(fd));
		
		P_mobserv_fd[D_GetClient_mobserv_charge_channel_link(fd)]	= 0;
		D_SetClient_clear(fd);
		U_Send_queue_in(SEND_INTER_CLOSE_FD, fd, 0, 0, 0, 0, NULL);
		return;	
	}


	use_billing_fd		= P_billing_fd[D_GetClient_use_billing_idx(fd)];
	use_login_dbagent_fd	= P_login_dbagent_fd[D_GetClient_use_login_dbagent_idx(fd)];
	use_dbagent_fd		= P_dbagent_fd[D_GetClient_use_dbagent_idx(fd)];

	//char loading
	if( *D_GetClient_char_name(fd) )
	{
		channel_link	= D_GetClient_channel_link(fd);
		map		= D_GetClient_map(fd);	
		block_x		= D_GetClient_block_x(fd);	

		//map block delete char
		D_SetMapBlock_delete_char(fd);
		U_Send_queue_in(SEND_INTER_BLOCK_DELETE_CHAR, fd, 0, 0, 0, 0, NULL);
			
		//send GC_OTHER_CHAR_DISAPPEAR to 3block char
		U_Printf("[PROC]send_queue_in 3block BLOCK_ALLCAST GC_OTHER_CHAR_DISAPPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, block_x-1, block_x, block_x+1);
		P_send_size	= MK_GetPacket_GC_OTHER_CHAR_DISAPPEAR(P_send_buf, &fd, 0);	
		U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_buf);
		U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
		U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);
		
		//send GM_CHAR_CHANNEL_OUT 
		if( P_mobserv_fd[channel_link] )
		{
			U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GM_CHAR_CHANNEL_OUT fd(%d) user_id(%s) char_name(%s)\n",
						P_mobserv_fd[channel_link], channel_link, fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
			P_send_size	= MK_GetPacket_GM_CHAR_CHANNEL_OUT(P_send_buf, &fd, 0);
			U_Send_queue_in(SEND_UNICAST, P_mobserv_fd[channel_link], 0, 0, 0, P_send_size, P_send_buf);
		}
		
		//set channel user
		D_SetWorld_channel_user_plus(G_World, channel_link+CHANNEL_PER_MACHINE*(G_Machine-1), -1);
	
		//query
		P_send_size	= MK_GetPacket_WA_CHAR_SAVE(P_send_buf, fd, NULL, 0);
		U_Print_query(P_send_buf);

		//save
		if( use_dbagent_fd )
		{
			U_Printf("[PROC]send_queue_in UNICAST(%d) WA_CHAR_SAVE user_id(%s) char_name(%s)\n",
					use_dbagent_fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
			//P_send_size	= MK_GetPacket_WA_CHAR_SAVE(P_send_buf, fd, NULL, 0);
			U_Send_queue_in(SEND_UNICAST, use_dbagent_fd, 0, 0, 0, P_send_size, P_send_buf);
		}	
		else
		{
			U_Printf("[PROC]Error save fd(%d) user_id(%s) char_name(%s)\n", fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		}

		//send WA_GAME_LOG
		if( use_login_dbagent_fd )
		{
			U_Printf("[PROC]send_queue_in UNICAST(%d) WA_GAMELOG fd(%d) user_id(%s) char_name(%s)\n",
				use_login_dbagent_fd, fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
			P_send_size	= MK_GetPacket_WA_GAME_LOG(P_send_buf, fd, channel_link, 1, (G_Sec - D_GetClient_char_load_sec(fd))/60);
			U_Send_queue_in(SEND_UNICAST, use_login_dbagent_fd, 0, 0, 0, P_send_size, P_send_buf);
		}
	}
	
	//login
	if( *D_GetClient_user_id(fd) )
	{
		--G_Login_number;

		//logout
		if( use_login_dbagent_fd  )
		{
			//send WB_GAMESTOP
			//U_Printf("[PROC]send_queue_in UNICAST(%d) WB_GAMESTOP fd(%d) user_id(%s)\n", use_billing_fd, fd, D_GetClient_user_id(fd));
			//P_send_size	= MK_GetPacket_WB_GAMESTOP(P_send_buf, fd, D_GetClient_user_id(fd), D_GetClient_ip(fd));
			//U_Send_queue_in(SEND_UNICAST, use_billing_fd, 0, 0, 0, P_send_size, P_send_buf);
			
			//send WA_LOGOUT
			U_Printf("[PROC]send_queue_in UNICAST(%d) WA_LOGOUT fd(%d) user_id(%s)\n",
									use_login_dbagent_fd, fd, D_GetClient_user_id(fd));
			P_send_size
			= MK_GetPacket_WA_LOGOUT(P_send_buf, fd, D_GetClient_sec_active(fd), D_GetClient_session(fd), D_GetClient_user_id(fd));
			U_Send_queue_in(SEND_UNICAST, use_login_dbagent_fd, 0, 0, 0, P_send_size, P_send_buf);
		}
		else
		{
			//do noting.. 
			U_Printf("[PROC]do noting logout fd(%d) user_id(%s)\n", fd, D_GetClient_user_id(fd));
		}
	}

	D_SetClient_clear(fd);
	U_Send_queue_in(SEND_INTER_CLOSE_FD, fd, 0, 0, 0, 0, NULL);
}


//____________________________________________________________
//INTER
static	void	P_proc_INTER_TICK(const int tick)
{
	int	i;


	if( P_login_dbagent_fd[1] )
	{
		for( i=1; i<=CHANNEL_PER_MACHINE; i++ )
		{
			if( (i + (CHANNEL_PER_MACHINE*(G_Machine-1))) <= D_GetWorld_channel_cnt(G_World) )
			{
				U_Printf("[PROC]send_queue_in UNICAST(%d) WA_CURRENT_USER channel_link(%d)\n", P_login_dbagent_fd[1], i);
				P_send_size = MK_GetPacket_WA_CURRENT_USER(P_send_buf, i);
				U_Send_queue_in(SEND_UNICAST, P_login_dbagent_fd[1], 0, 0, 0, P_send_size, P_send_buf);
			}
		}
	}
}

/*
static	void	P_proc_INTER_CONN_BILLING_FD(const int billing_fd)
{
	D_SetClient_sec_active(billing_fd, G_Sec);
	D_SetClient_session(billing_fd, ++P_session);
	D_SetClient_billing_idx(billing_fd, P_recv_q->fd_or_seq);
	D_SetClient_ip(billing_fd, P_recv_q->data);
	U_Send_queue_in(SEND_INTER_CONN_FD, billing_fd, 0, 0, 0, 0, NULL);
	
	//send	WB_REGSERVER
	U_Printf("[PROC]send_queue_in UNICAST(%d) WB_REGSERVER\n", billing_fd);
	P_send_size	= MK_GetPacket_WB_REGSERVER(P_send_buf);	
	U_Send_queue_in(SEND_UNICAST, billing_fd, 0, 0, 0, P_send_size, P_send_buf);
}
*/

static	void	P_proc_INTER_CONN_LOGIN_DBAGENT_FD(const int login_dbagent_fd)
{
	D_SetClient_sec_active(login_dbagent_fd, G_Sec);
	D_SetClient_session(login_dbagent_fd, ++P_session);
	D_SetClient_login_dbagent_idx(login_dbagent_fd, P_recv_q->fd_or_seq);
	D_SetClient_ip(login_dbagent_fd, P_recv_q->data);
	U_Send_queue_in(SEND_INTER_CONN_FD, login_dbagent_fd, 0, 0, 0, 0, NULL);
	
	//send	WA_INIT	
	U_Printf("[PROC]send_queue_in UNICAST(%d) WA_INIT\n", login_dbagent_fd);
	P_send_size	= MK_GetPacket_WA_INIT(P_send_buf, 0);	
	U_Send_queue_in(SEND_UNICAST, login_dbagent_fd, 0, 0, 0, P_send_size, P_send_buf);
}


static	void	P_proc_INTER_CONN_DBAGENT_FD(const int dbagent_fd)
{
	D_SetClient_sec_active(dbagent_fd, G_Sec);
	D_SetClient_session(dbagent_fd, ++P_session);
	D_SetClient_dbagent_idx(dbagent_fd, P_recv_q->fd_or_seq);
	D_SetClient_ip(dbagent_fd, P_recv_q->data);
	U_Send_queue_in(SEND_INTER_CONN_FD, dbagent_fd, 0, 0, 0, 0, NULL);
	
	//send	WA_INIT	
	U_Printf("[PROC]send_queue_in UNICAST(%d) WA_INIT\n", dbagent_fd);
	P_send_size	= MK_GetPacket_WA_INIT(P_send_buf, 0);	
	U_Send_queue_in(SEND_UNICAST, dbagent_fd, 0, 0, 0, P_send_size, P_send_buf);
}


static	void	P_proc_INTER_CONN_MOBSERV_FD(const int mobserv_fd)
{
	D_SetClient_sec_active(mobserv_fd, G_Sec);
	D_SetClient_session(mobserv_fd, ++P_session);
	D_SetClient_ip(mobserv_fd, P_recv_q->data);
	U_Send_queue_in(SEND_INTER_CONN_FD, mobserv_fd, 0, 0, 0, 0, NULL);
}


static	void	P_proc_INTER_CONN_CLIENT_FD(const int fd)
{
	D_SetClient_sec_active(fd, G_Sec);
	D_SetClient_session(fd, ++P_session);
	D_SetClient_ip(fd, P_recv_q->data);
	U_Send_queue_in(SEND_INTER_CONN_FD, fd, 0, 0, 0, 0, NULL);
}

	
static	void	P_proc_INTER_CLOSE_FD(const int fd)
{
	if( D_GetClient_sec_active(fd) )
		P_remove_client(fd);
}

				
static	void	P_proc_INTER_CHAR_RECOVER(const int fd)
{
	int	change;


	if( D_GetClient_status(fd) >= _USER_DEAD )
		return;

	D_SetClient_recover(fd, &change);
			
	if( change )
	{
		//send GC_CHAR_RECOVER
		//U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_RECOVER user_id(%s) char_name(%s) char_hp(%d)\n",
		//	fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), D_GetClient_hp(fd));
		P_send_size	= MK_GetPacket_GC_CHAR_RECOVER(P_send_buf, fd);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
	}
}
				

static	void	P_proc_INTER_MOB_RECOVER(const int channel_link, const int mob_id)
{
	if( D_GetMob_status(channel_link, mob_id) >= _MOB_DEATH )
		return;

	D_SetMob_recover(channel_link, mob_id);
}


static	void	P_proc_INTER_ITEM_REMOVE(int item_id)
{
	time_t	drop_time;
	int	channel_link, map, block_x;


	drop_time	= D_GetItem_drop_time(item_id);

	if( (drop_time == 0) ||  ((drop_time + MAX_ITEM_DROP_TIME) > G_Sec) )
		return;
		
	channel_link	= D_GetItem_channel_link(item_id);
	map	= D_GetItem_map(item_id);	
	block_x	= D_GetItem_block_x(item_id);	
	
	//map block delete item
	D_SetMapBlock_delete_item(item_id);
		
	//send GC_ITEM_DISAPPEAR to 3block char
	//U_Printf("[PROC]send_queue_in 3block BLOCK_ALLCAST GC_ITEM_DISAPPEAR item_id(%d) channel_link(%d) map(%d) block_x(%d)(%d)(%d)\n",
	//		item_id, channel_link, map, block_x-1, block_x, block_x+1);
	P_send_size	= MK_GetPacket_GC_ITEM_DISAPPEAR(P_send_buf, &item_id, 0);	
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);

	D_SetItem_clear(item_id);
}


static	void	P_proc_INTER_CHAR_BUFF_END(const int fd)
{
	time_t	buff_end_sec;


	if( D_GetClient_status(fd) >= _USER_DEAD )
		return;

	buff_end_sec	= D_GetClient_buff_end_sec(fd, *((int *)(P_recv_q->data)));	

	if( (buff_end_sec == 0) || (buff_end_sec > G_Sec) )					
		return;
	
	D_SetClient_buff_clear(fd, *((int *)(P_recv_q->data)));	
		
	//send GC_CHAR_BUFF
	U_Printf("[PROC]send_queue_in UNICAST(%d) end GC_CHAR_BUFF(%d) user_id(%s) char_name(%s)\n",
							fd, *((int *)(P_recv_q->data)), D_GetClient_user_id(fd), D_GetClient_char_name(fd));
	P_send_size	= MK_GetPacket_GC_CHAR_BUFF(P_send_buf, fd, *((int *)(P_recv_q->data)));
	U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
}


static	void	P_proc_INTER_MOB_BUFF_END(const int channel_link, const int mob_id)
{
	time_t	buff_end_sec;


	if( D_GetMob_status(channel_link, mob_id) >= _MOB_DEATH )
		return;

	buff_end_sec	= D_GetMob_buff_end_sec(channel_link, mob_id, *((int *)(P_recv_q->data)));	

	if( (buff_end_sec == 0) || (buff_end_sec > G_Sec) )					
		return;
	
	D_SetMob_buff_clear(channel_link, mob_id, *((int *)(P_recv_q->data)));	

	//send GM_MOB_BUFF
	U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GM_MOB_BUFF mob_id(%d)\n", P_mobserv_fd[channel_link], channel_link, mob_id);
	P_send_size	= MK_GetPacket_GM_MOB_BUFF(P_send_buf, channel_link, mob_id, *((int *)(P_recv_q->data)));
	U_Send_queue_in(SEND_UNICAST, P_mobserv_fd[channel_link], 0, 0, 0, P_send_size, P_send_buf);
}


//______________________________________________________________
//BW
/*
static	void	P_proc_BW_GAMESTART_SUCC(const int billing_fd)
{
	struct	body_BW_GAMESTART_SUCC	*body	= (struct body_BW_GAMESTART_SUCC *)(P_recv_q->data);

	
	if( D_GetClient_sec_active(billing_fd) == 0 )	
		return;

	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_BW_GAMESTART_SUCC) )
	{
		U_Printf("[PROC]Error billing_fd(%d) BW_GAMESTART_SUCC(%d) bad body_size(%d)\n",
				billing_fd, sizeof(struct body_BW_GAMESTART_SUCC), P_recv_q->data_size);
		P_remove_client(billing_fd);
		return;
	}

	//check id
	if( (body->id < 1) || (body->id > MAX_FD) )
	{
		U_Printf("[PROC]Error billing_fd(%d) BW_GAMESTART_SUCC bad id(%d)\n", billing_fd, body->id);
		P_remove_client(billing_fd);
		return;
	}

	//check user_id	
	if( (body->user_id[0] == 0) || (body->user_id[USER_ID_LEN] != 0) )
	{
		body->user_id[USER_ID_LEN]	= 0;
		U_Printf("[PROC]Error billing_fd(%d) BW_GAMESTART_SUCC bad user_id(%s)\n", billing_fd, body->user_id);
		P_remove_client(billing_fd);
		return;
	}

	//check ip
	if( (body->user_ip[0] == 0) || (body->user_ip[SIZEOF_IP] != 0) )
	{
		body->user_ip[SIZEOF_IP]	= 0;
		U_Printf("[PROC]Error billing_fd(%d) BW_GAMESTART_SUCC user_id(%s) bad user_ip(%s)\n", billing_fd, body->user_id, body->user_ip);
		P_remove_client(billing_fd);
		return;
	}
	
	//check session
	if( strncmp(D_GetClient_lock_user_id(body->id), body->user_id, USER_ID_LEN+1) 
									|| strncmp(D_GetClient_ip(body->id), body->user_ip, SIZEOF_IP+1) )
	{
		U_Printf("[PROC]Discord billing_fd(%d) BW_GAMESTART_SUCC fd(%d) lock_user_id(%s) user_id(%s) user_ip(%s)\n",
							billing_fd, body->id, D_GetClient_lock_user_id(body->id), body->user_id, body->user_ip);
		//send WB_GAMESTOP
		U_Printf("[PROC]send_queue_in UNICAST(%d) WB_GAMESTOP  user_id(%s) user_ip(%s)\n", billing_fd, body->user_id, body->user_ip);
		P_send_size	= MK_GetPacket_WB_GAMESTOP(P_send_buf, body->id, body->user_id, body->user_ip);
		U_Send_queue_in(SEND_UNICAST, billing_fd, 0, 0, 0, P_send_size, P_send_buf);
		return;
	}
	
	//check already login
	if( *D_GetClient_user_id(body->id) )
	{
		U_Printf("[PROC]Error billing_fd:%d BW_GAMESTART already login user_id(%s) user_id(%s)\n",
				billing_fd, D_GetClient_user_id(body->id), body->user_id);
		P_remove_client(billing_fd);
		return;
	}
	
	switch( body->result )
	{
		case	0:
			body->result	= RESULT_SUCC;
			break;
		case	1:
			body->result	= RESULT_FAIL_PAY_INFO;
			break;
		case	2:
			body->result	= RESULT_FAIL_LOGIN;
			break;
		case	3:
			body->result	= RESULT_FAIL_BLOCK_USERID;
			break;
		case	4:
			body->result	= RESULT_FAIL_SQL;
			break;
		case	5:
			body->result	= RESULT_FAIL_OTHER_GAME;
			break;
		default	:
			body->result	= RESULT_FAIL_BILLING;
	}
	
	
	//send GC_LOGIN_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) GC_LOGIN_RESULT result_code(%d) user_id(%s)\n", body->id, body->result, body->user_id);
	P_send_size	= MK_GetPacket_GC_LOGIN_RESULT(P_send_buf, body->result);
	U_Send_queue_in(SEND_UNICAST, body->id, 0, 0, 0, P_send_size, P_send_buf);


	if( body->result )
	{
		//remove client	
		P_remove_client(body->id);	
	}
	else
	{
		//set G_Login_number
		++G_Login_number;

		//set user_id
		D_SetClient_user_id(body->id, body->user_id);

		U_Printf("[PROC]Login success fd(%d) user_id(%s) user_ip(%s) G_Login_number(%d) G_Connection_number(%d)\n",
							body->id, body->user_id, body->user_ip, G_Login_number, G_Connection_number);
	}
}


static	void	P_proc_BW_GAMESTART_FAIL(const int billing_fd)
{
	struct	body_BW_GAMESTART_FAIL	*body	= (struct body_BW_GAMESTART_FAIL *)(P_recv_q->data);

	
	if( D_GetClient_sec_active(billing_fd) == 0 )	
		return;

	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_BW_GAMESTART_FAIL) )
	{
		U_Printf("[PROC]Error billing_fd(%d) BW_GAMESTART_FAIL(%d) bad body_size(%d)\n",
				billing_fd, sizeof(struct body_BW_GAMESTART_FAIL), P_recv_q->data_size);
		P_remove_client(billing_fd);
		return;
	}

	//check id
	if( (body->id < 1) || (body->id > MAX_FD) )
	{
		U_Printf("[PROC]Error billing_fd(%d) BW_GAMESTART_FAIL bad id(%d)\n", billing_fd, body->id);
		P_remove_client(billing_fd);
		return;
	}

	//check user_id	
	if( (body->user_id[0] == 0) || (body->user_id[USER_ID_LEN] != 0) )
	{
		body->user_id[USER_ID_LEN]	= 0;
		U_Printf("[PROC]Error billing_fd(%d) BW_GAMESTART_FAIL bad user_id(%s)\n", billing_fd, body->user_id);
		P_remove_client(billing_fd);
		return;
	}
	
	//check session
	if( strncmp(D_GetClient_lock_user_id(body->id), body->user_id, USER_ID_LEN+1) )
	{
		U_Printf("[PROC]Discord billing_fd(%d) BW_GAMESTART_SUCC fd(%d) lock_user_id(%s) user_id(%s)\n",
							billing_fd, body->id, D_GetClient_lock_user_id(body->id), body->user_id);
		return;
	}
	
	//check already login
	if( *D_GetClient_user_id(body->id) )
	{
		U_Printf("[PROC]Error billing_fd:%d BW_GAMESTOP already login user_id(%s) user_id(%s)\n",
							billing_fd, D_GetClient_user_id(body->id), body->user_id);
		P_remove_client(billing_fd);
		return;
	}

	//send GC_LOGIN_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) GC_LOGIN_RESULT RESULT_FAIL_BILLING user_id(%s)\n", body->id, body->user_id);
	P_send_size	= MK_GetPacket_GC_LOGIN_RESULT(P_send_buf, RESULT_FAIL_BILLING);
	U_Send_queue_in(SEND_UNICAST, body->id, 0, 0, 0, P_send_size, P_send_buf);
	
	//remove client	
	P_remove_client(body->id);	
}


static	void	P_proc_BW_GAMESTOP_SUCC(const int billing_fd)
{
	if( D_GetClient_sec_active(billing_fd) == 0 )
		return;
}


static	void	P_proc_BW_GAMESTOP_FAIL(const int billing_fd)
{
	if( D_GetClient_sec_active(billing_fd) == 0 )
		return;
}


static	void	P_proc_BW_USER_CHECK(const int billing_fd)
{
	struct	body_BW_USER_CHECK	*body	= (struct body_BW_USER_CHECK *)(P_recv_q->data);
	

	if( D_GetClient_sec_active(billing_fd) == 0 )
		return;

	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_BW_USER_CHECK) )
	{
		U_Printf("[PROC]Error billing_fd(%d) BW_USER_CHECK(%d) bad body_size(%d)\n",
				billing_fd, sizeof(struct body_BW_USER_CHECK), P_recv_q->data_size);
		P_remove_client(billing_fd);
		return;
	}

	//check id
	if( (body->id < 1) || (body->id > MAX_FD) )
	{
		U_Printf("[PROC]Error billing_fd(%d) BW_USER_CHECK bad id(%d)\n", billing_fd, body->id);
		P_remove_client(billing_fd);
		return;
	}

	//check user_id	
	if( (body->user_id[0] == 0) || (body->user_id[USER_ID_LEN] != 0) )
	{
		body->user_id[USER_ID_LEN]	= 0;
		U_Printf("[PROC]Error billing_fd(%d) BW_USER_CHECK bad user_id(%s)\n", billing_fd, body->user_id);
		P_remove_client(billing_fd);
		return;
	}
	
	//check session
	if( strncmp(D_GetClient_user_id(body->id), body->user_id, USER_ID_LEN+1) )
	{
		//send WB_USER_CHECK_FAIL
		U_Printf("[PROC]send_queue_in UNICAST(%d) WB_USER_CHECK_FAIL  user_id(%s)\n", billing_fd, body->user_id);
		P_send_size	= MK_GetPacket_WB_USER_CHECK_FAIL(P_send_buf, body->id, body->user_id);
		U_Send_queue_in(SEND_UNICAST, billing_fd, 0, 0, 0, P_send_size, P_send_buf);
		return;
	}
	else
	{
		//send WB_USER_CHECK_SUCC
		U_Printf("[PROC]send_queue_in UNICAST(%d) WB_USER_CHECK_SUCC  user_id(%s)\n", billing_fd, body->user_id);
		P_send_size	= MK_GetPacket_WB_USER_CHECK_SUCC(P_send_buf, body->id, body->user_id);
		U_Send_queue_in(SEND_UNICAST, billing_fd, 0, 0, 0, P_send_size, P_send_buf);
		return;
	}
}


static	void	P_proc_BW_DATABASE_ERROR(const int billing_fd)
{
	struct	body_BW_DATABASE_ERROR	*body	= (struct body_BW_DATABASE_ERROR *)(P_recv_q->data);
	

	if( D_GetClient_sec_active(billing_fd) == 0 )
		return;

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_BW_DATABASE_ERROR) )
	{
		U_Printf("[PROC]Error billing_fd(%d) BW_DATABASE_ERROR(%d) bad body_size(%d)\n",
				billing_fd, sizeof(struct body_BW_DATABASE_ERROR), P_recv_q->data_size);
		P_remove_client(billing_fd);
		return;
	}

	U_Printf("[PROC]billing_fd(%d) BW_DATABASE_ERROR(%d)\n", billing_fd, body->id);
}


static	void	P_proc_BW_SYSTEM_ERROR(const int billing_fd)
{
	struct	body_BW_SYSTEM_ERROR	*body	= (struct body_BW_SYSTEM_ERROR *)(P_recv_q->data);
	

	if( D_GetClient_sec_active(billing_fd) == 0 )
		return;

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_BW_SYSTEM_ERROR) )
	{
		U_Printf("[PROC]Error billing_fd(%d) BW_SYSTEM_ERROR(%d) bad body_size(%d)\n",
				billing_fd, sizeof(struct body_BW_SYSTEM_ERROR), P_recv_q->data_size);
		P_remove_client(billing_fd);
		return;
	}

	U_Printf("[PROC]billing_fd(%d) BW_SYSTEM_ERROR(%d)\n", billing_fd, body->id);
}


static	void	P_proc_BW_REGSERVER_SUCC(const int billing_fd)
{
	if( D_GetClient_sec_active(billing_fd) == 0 )
		return;

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_BW_REGSERVER_SUCC) )
	{
		U_Printf("[PROC]Error billing_fd(%d) BW_REGSERVER_SUCC(%d) bad body_size(%d)\n",
				billing_fd, sizeof(struct body_BW_REGSERVER_SUCC), P_recv_q->data_size);
		P_remove_client(billing_fd);
		return;
	}

	P_billing_fd[D_GetClient_billing_idx(billing_fd)]	= billing_fd;
	U_Printf("[PROC]billing_fd(%d) BW_REGSERVER_SUCC\n", billing_fd);
}


static	void	P_proc_BW_REGSERVER_FAIL(const int billing_fd)
{
	if( D_GetClient_sec_active(billing_fd) == 0 )
		return;

	U_Printf("[PROC]billing_fd(%d) BW_REGSERVER_FAIL\n", billing_fd);
	P_remove_client(billing_fd);
}


static	void	P_proc_BW_ALIVECHECK(const int billing_fd)
{
	if( D_GetClient_sec_active(billing_fd) == 0 )
		return;
}
*/


//______________________________________________________________
//AW
static	void	P_proc_AW_INIT_RESULT(const int dbagent_fd)
{
	struct	body_AW_INIT_RESULT	*body	= (struct body_AW_INIT_RESULT *)(P_recv_q->data);


	if( D_GetClient_sec_active(dbagent_fd) == 0 )
		return;

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_AW_INIT_RESULT) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_INIT_RESULT(%d) bad body_size(%d)\n",
				dbagent_fd, sizeof(struct body_AW_INIT_RESULT), P_recv_q->data_size);
		P_remove_client(dbagent_fd);
		return;
	}

	//check result_code
	if( body->result_code )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_INIT_RESULT result_code(%d)\n", dbagent_fd, body->result_code);
		P_remove_client(dbagent_fd);
		return;
	}

	if( D_GetClient_dbagent_idx(dbagent_fd) )
	{
		P_dbagent_fd[D_GetClient_dbagent_idx(dbagent_fd)]		= dbagent_fd;
		U_Printf("[PROC]dbagent_fd(%d) AW_INIT_RESULT success\n", dbagent_fd);
	}
	else
	{
		P_login_dbagent_fd[D_GetClient_login_dbagent_idx(dbagent_fd)]	= dbagent_fd;
		U_Printf("[PROC]login_dbagent_fd(%d) AW_INIT_RESULT success\n", dbagent_fd);
	}
}


static	void	P_proc_AW_LIVE(const int dbagent_fd)
{
	if( D_GetClient_sec_active(dbagent_fd) == 0 )
		return;

	P_send_size	= MK_GetPacket_WA_LIVE(P_send_buf);
	U_Send_queue_in(SEND_UNICAST, dbagent_fd, 0, 0, 0, P_send_size, P_send_buf);
}


static	void	P_proc_AW_CHAR_LOAD_RESULT(const int dbagent_fd)
{	
	int					channel_link, map, block_x, i, use_login_dbagent_fd;
	struct	body1_AW_CHAR_LOAD_RESULT	*body1	= (struct body1_AW_CHAR_LOAD_RESULT *)(P_recv_q->data);
	struct	body2_AW_CHAR_LOAD_RESULT	*body2
					= (struct body2_AW_CHAR_LOAD_RESULT *)(P_recv_q->data + sizeof(struct body1_AW_CHAR_LOAD_RESULT));


	if( D_GetClient_sec_active(dbagent_fd) == 0 )
		return;
	
	//check body1_size 
	if( P_recv_q->data_size < sizeof(struct body1_AW_CHAR_LOAD_RESULT) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_LOAD_RESULT(%d) bad body_size(%d)\n"
			, dbagent_fd, sizeof(struct body1_AW_CHAR_LOAD_RESULT), P_recv_q->data_size);
		P_remove_client(dbagent_fd);
		return;
	}

	//check item_slot_cnt
	if( (body1->item_slot_cnt < 0) || (body1->item_slot_cnt > ITEM_SLOT_CNT) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_LOAD_RESULT bad item_slot_cnt(%d)\n", dbagent_fd, body1->item_slot_cnt);
		P_remove_client(dbagent_fd);
		return;
	}

	//check body2_size
	if( P_recv_q->data_size != (sizeof(struct body1_AW_CHAR_LOAD_RESULT)+sizeof(struct body2_AW_CHAR_LOAD_RESULT)*(body1->item_slot_cnt)) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_LOAD_RESULT(%d) bad body_size(%d)\n", dbagent_fd,
		sizeof(struct body1_AW_CHAR_LOAD_RESULT)+sizeof(struct body2_AW_CHAR_LOAD_RESULT)*(body1->item_slot_cnt), P_recv_q->data_size);
		P_remove_client(dbagent_fd);
		return;
	}
	

	//check fd
	if( (P_recv_q->fd_or_seq < 1) || (P_recv_q->fd_or_seq > MAX_FD) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_LOAD_RESULT bad fd(%d)\n", dbagent_fd, P_recv_q->fd_or_seq);
		P_remove_client(dbagent_fd);
		return;
	}

	//check user_id
	if( (body1->user_id[0] == 0) || (body1->user_id[USER_ID_LEN] != 0) )
	{
		body1->user_id[USER_ID_LEN]	= 0;
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_LOAD_RESULT bad user_id(%s)\n", dbagent_fd, body1->user_id);
		P_remove_client(dbagent_fd);
		return;
	}

	//check char_name
	if( (body1->char_name[0] == 0) || (body1->char_name[CHAR_NAME_LEN] != 0) )
	{
		body1->char_name[CHAR_NAME_LEN]	= 0;
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_LOAD_RESULT bad char_name(%s)\n", dbagent_fd, body1->char_name);
		P_remove_client(dbagent_fd);
		return;
	}

	//check session
	if( (P_recv_q->sec_active != D_GetClient_sec_active(P_recv_q->fd_or_seq)) || (P_recv_q->session != D_GetClient_session(P_recv_q->fd_or_seq))
		|| (strncmp(body1->user_id, D_GetClient_user_id(P_recv_q->fd_or_seq), USER_ID_LEN+1)) )
	{
		U_Printf("[PROC]Discord dbagent_fd(%d) AW_CHAR_LOAD_RESULT fd_or_seq(%d) sec_active(%d) session(%d) user_id(%s) char_name(%s)\n",
					dbagent_fd, P_recv_q->fd_or_seq, P_recv_q->sec_active, P_recv_q->session, body1->user_id, body1->char_name);
		//send WA_CHAR_SAVE
		U_Printf("[PROC]send_queue_in UNICAST(%d) WA_CHAR_SAVE user_id(%s) char_name(%s)\n", dbagent_fd, body1->user_id, body1->char_name);
		P_send_size	= MK_GetPacket_WA_CHAR_SAVE(P_send_buf, 0, P_recv_q->data, P_recv_q->data_size);
		U_Print_query(P_send_buf);
		U_Send_queue_in(SEND_UNICAST, dbagent_fd, 0, 0, 0, P_send_size, P_send_buf);
		return;
	}

	//check char loading
	if( *D_GetClient_char_name(P_recv_q->fd_or_seq) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_LOAD_RESULT user_id(%s) char_name(%s) bad char_loading(%s)\n",
				dbagent_fd, body1->user_id, body1->char_name, D_GetClient_char_name(P_recv_q->fd_or_seq));
		P_remove_client(dbagent_fd);
		return;
	}

	//check result_code	
	if( body1->result_code )
	{
		//send GC_CHAR_LOAD_RESULT
		U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_LOAD_RESULT result_code(%d) user_id(%s) char_name(%s)\n",
			P_recv_q->fd_or_seq, body1->result_code, body1->user_id, body1->char_name);
		P_send_size	= MK_GetPacket_GC_CHAR_LOAD_RESULT(P_send_buf, body1->result_code, P_recv_q->fd_or_seq, body1->char_name);
		U_Send_queue_in(SEND_UNICAST, P_recv_q->fd_or_seq, 0, 0, 0, P_send_size, P_send_buf);

		P_remove_client(P_recv_q->fd_or_seq);
		return;
	}
		
	//set char from db
	if( D_SetClient_from_db(P_recv_q->fd_or_seq, body1, body2, P_char_buff) )
	{
		U_Printf("[PROC]Error fd(%d) user_id(%s) char_name(%s) AG_CHAR_LOAD_RESULT D_SetClient_from_db()\n",
				P_recv_q->fd_or_seq, body1->user_id, body1->char_name);
	
		//send WA_CHAR_SAVE
		U_Printf("[PROC]send_queue_in UNICAST(%d) WA_CHAR_SAVE user_id(%s) char_name(%s)\n", dbagent_fd, body1->user_id, body1->char_name);
		P_send_size	= MK_GetPacket_WA_CHAR_SAVE(P_send_buf, 0, P_recv_q->data, P_recv_q->data_size);
		U_Send_queue_in(SEND_UNICAST, dbagent_fd, 0, 0, 0, P_send_size, P_send_buf);
			
		//send GC_CHAR_LOAD_RESULT
		body1->result_code	= RESULT_FAIL_DB;	
		U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_LOAD_RESULT result_code(%d) user_id(%s) char_name(%s)\n",
								P_recv_q->fd_or_seq, body1->result_code, body1->user_id, body1->char_name);
		P_send_size	= MK_GetPacket_GC_CHAR_LOAD_RESULT(P_send_buf, body1->result_code, P_recv_q->fd_or_seq, body1->char_name);
		U_Send_queue_in(SEND_UNICAST, P_recv_q->fd_or_seq, 0, 0, 0, P_send_size, P_send_buf);
	
		P_remove_client(P_recv_q->fd_or_seq);
		return;
	}
		

	//send GC_CHAR_LOAD_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_LOAD_RESULT result_code(%d) user_id(%s) char_name(%s)\n",
		P_recv_q->fd_or_seq, body1->result_code, body1->user_id, body1->char_name);
	P_send_size	= MK_GetPacket_GC_CHAR_LOAD_RESULT(P_send_buf, body1->result_code, P_recv_q->fd_or_seq, body1->char_name);
	U_Send_queue_in(SEND_UNICAST, P_recv_q->fd_or_seq, 0, 0, 0, P_send_size, P_send_buf);

	//send GC_CHAR_BUFF
	for( i=0; i<PASSIVE_BUFF_CNT+EQUIP_PASSIVE_BUFF_CNT; i++ )
	{
		if( P_char_buff[i] )
		{
			U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_BUFF(%d) user_id(%s) char_name(%s)\n",
			P_recv_q->fd_or_seq, i, D_GetClient_user_id(P_recv_q->fd_or_seq), D_GetClient_char_name(P_recv_q->fd_or_seq));
			P_send_size	= MK_GetPacket_GC_CHAR_BUFF(P_send_buf, P_recv_q->fd_or_seq, i);
			U_Send_queue_in(SEND_UNICAST, P_recv_q->fd_or_seq, 0, 0, 0, P_send_size, P_send_buf);
		}
	}

	channel_link	= D_GetClient_channel_link(P_recv_q->fd_or_seq);
	map		= D_GetClient_map(P_recv_q->fd_or_seq);
	block_x		= D_GetClient_block_x(P_recv_q->fd_or_seq);

	//send 3block GC_XXXX_APPEAR to owner
	P_send_q_in_block_GC_XXXX_APPEAR(P_recv_q->fd_or_seq, channel_link, map, block_x);
	P_send_q_in_block_GC_XXXX_APPEAR(P_recv_q->fd_or_seq, channel_link, map, block_x-1);
	P_send_q_in_block_GC_XXXX_APPEAR(P_recv_q->fd_or_seq, channel_link, map, block_x+1);
						
	//send GC_OTHER_CHAR_APPEAR to 3block char
	U_Printf("[PROC]send_queue_in 3block BLOCK_ALLCAST GC_OTHER_CHAR_APPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)(%d)\n", body1->user_id, body1->char_name, channel_link, map, block_x-1, block_x, block_x+1);
	P_send_size	= MK_GetPacket_GC_OTHER_CHAR_APPEAR(P_send_buf, &(P_recv_q->fd_or_seq), 0);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);
			
	//map block insert char
	D_SetMapBlock_insert_char(P_recv_q->fd_or_seq);
	U_Send_queue_in(SEND_INTER_BLOCK_INSERT_CHAR, P_recv_q->fd_or_seq, channel_link, map, block_x, 0, NULL);

	//send GM_CHAR_CHANNEL_IN
	if( P_mobserv_fd[channel_link] )
	{
		U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GM_CHAR_CHANNEL_IN fd(%d) user_id(%s) char_name(%s)\n",
						P_mobserv_fd[channel_link], channel_link, P_recv_q->fd_or_seq, body1->user_id, body1->char_name);
		P_send_size	= MK_GetPacket_GM_CHAR_CHANNEL_IN(P_send_buf, &(P_recv_q->fd_or_seq), 0);
		U_Send_queue_in(SEND_UNICAST, P_mobserv_fd[channel_link], 0, 0, 0, P_send_size, P_send_buf);
	}
		
	//set channel user
	D_SetWorld_channel_user_plus(G_World, channel_link+CHANNEL_PER_MACHINE*(G_Machine-1), 1);

	//send WA_GAME_LOG
	if( (use_login_dbagent_fd = P_login_dbagent_fd[D_GetClient_use_login_dbagent_idx(P_recv_q->fd_or_seq)]) )
	{
		U_Printf("[PROC]send_queue_in UNICAST(%d) WA_GAMELOG fd(%d) user_id(%s) char_name(%s)\n",
		use_login_dbagent_fd, P_recv_q->fd_or_seq, D_GetClient_user_id(P_recv_q->fd_or_seq), D_GetClient_char_name(P_recv_q->fd_or_seq));
		P_send_size	= MK_GetPacket_WA_GAME_LOG(P_send_buf, P_recv_q->fd_or_seq, channel_link, 0, 0);
		U_Send_queue_in(SEND_UNICAST, use_login_dbagent_fd, 0, 0, 0, P_send_size, P_send_buf);
	}
}


static	void	P_proc_AW_LOGIN_RESULT(const int dbagent_fd)
{
	struct	body_AW_LOGIN_RESULT	*body	= (struct body_AW_LOGIN_RESULT *)(P_recv_q->data);


	if( D_GetClient_sec_active(dbagent_fd) == 0 )
		return;

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_AW_LOGIN_RESULT) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_LOGIN_RESULT(%d) bad body_size(%d)\n",
					dbagent_fd, sizeof(struct body_AW_LOGIN_RESULT), P_recv_q->data_size);
		P_remove_client(dbagent_fd);
		return;
	}

	//check fd
	if( (P_recv_q->fd_or_seq < 1) || (P_recv_q->fd_or_seq > MAX_FD) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_LOGIN_RESULT bad fd(%d)\n", dbagent_fd, P_recv_q->fd_or_seq);
		P_remove_client(dbagent_fd);
		return;
	}

	//check user_id
	if( (body->user_id[0] == 0) || (body->user_id[USER_ID_LEN] != 0) )
	{
		body->user_id[USER_ID_LEN]	= 0;
		U_Printf("[PROC]Error dbagent_fd(%d) AW_LOGIN_RESULT bad user_id(%s)\n", dbagent_fd, body->user_id);
		P_remove_client(dbagent_fd);
		return;
	}

	//check session
	if( (P_recv_q->sec_active != D_GetClient_sec_active(P_recv_q->fd_or_seq))
				|| (P_recv_q->session != D_GetClient_session(P_recv_q->fd_or_seq)) )
	{
		U_Printf("[PROC]Discord dbagent_fd(%d) AW_LOGIN_RESULT fd_or_seq(%d) sec_active(%d) session(%d) user_id(%s)\n",
								dbagent_fd, P_recv_q->fd_or_seq, P_recv_q->session, body->user_id);
		//send GA_LOGOUT
		U_Printf("[PROC]send_queue_in UNICAST(%d) WA_LOGOUT user_id(%s)\n", P_recv_q->fd_or_seq, body->user_id);
		P_send_size	= MK_GetPacket_WA_LOGOUT(P_send_buf, P_recv_q->fd_or_seq, P_recv_q->sec_active, P_recv_q->session, body->user_id);
		U_Send_queue_in(SEND_UNICAST, dbagent_fd, 0, 0, 0, P_send_size, P_send_buf);
		return;		
	}
	
	//check already login
	if( *D_GetClient_user_id(P_recv_q->fd_or_seq) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_LOGIN_RESULT already login user_id(%s)(%s)\n",
							dbagent_fd, D_GetClient_user_id(P_recv_q->fd_or_seq), body->user_id);
		P_remove_client(dbagent_fd);
		return;
	}



	if( body->result_code )
	{
		//send GC_LOGIN_RESULT
		U_Printf("[PROC]send_queue_in UNICAST(%d) GC_LOGIN_RESULT result_code(%d) user_id(%s)\n",
								P_recv_q->fd_or_seq, body->result_code, body->user_id);
		P_send_size	= MK_GetPacket_GC_LOGIN_RESULT(P_send_buf, body->result_code);
		U_Send_queue_in(SEND_UNICAST, P_recv_q->fd_or_seq, 0, 0, 0, P_send_size, P_send_buf);
		
		//remove client	
		P_remove_client(P_recv_q->fd_or_seq);	
	}
	else
	{
		//set G_Login_number
		++G_Login_number;

		//set user_id
		D_SetClient_user_id(P_recv_q->fd_or_seq, body->user_id);

		U_Printf("[PROC]Login success fd(%d) user_id(%s) user_ip(%s) G_Login_number(%d) G_Connection_number(%d)\n",
				P_recv_q->fd_or_seq, body->user_id, D_GetClient_ip(P_recv_q->fd_or_seq), G_Login_number, G_Connection_number);
		
		//send GC_LOGIN_RESULT
		U_Printf("[PROC]send_queue_in UNICAST(%d) GC_LOGIN_RESULT result_code(%d) user_id(%s)\n",
								P_recv_q->fd_or_seq, body->result_code, body->user_id);
		P_send_size	= MK_GetPacket_GC_LOGIN_RESULT(P_send_buf, body->result_code);
		U_Send_queue_in(SEND_LOGIN, P_recv_q->fd_or_seq, 0, 0, 0, P_send_size, P_send_buf);
	}
}



//______________________________________________________________
//MG
static	void	P_proc_MG_INIT(const int mobserv_fd)
{
	int			i, j, channel_link, char_link;
	struct	body_MG_INIT	*body	= (struct body_MG_INIT *)(P_recv_q->data);

	
	if( D_GetClient_sec_active(mobserv_fd) == 0 )
		return;

	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_MG_INIT) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) MG_INIT(%d) bad body_size(%d)\n",
				mobserv_fd, sizeof(struct body_MG_INIT), P_recv_q->data_size);
		P_remove_client(mobserv_fd);
		return;
	}

	//check version
	if( (body->version[0] == 0) || (body->version[VERSION_LEN] != 0) || (strncmp(body->version, VERSION, VERSION_LEN+1)) )
	{
		body->version[VERSION_LEN]	= 0;
		U_Printf("[PROC]Error mobserv_fd(%d) MG_INIT bad version(%s)\n", mobserv_fd, body->version);
		P_send_size	= MK_GetPacket_GM_INIT_RESULT(P_send_buf, RESULT_FAIL_VERSION);	
		U_Send_queue_in(SEND_UNICAST, mobserv_fd, 0, 0, 0, P_send_size, P_send_buf);
		P_remove_client(mobserv_fd);
		return;
	}

	//check max_map, max_type_mob, max_mob
	if( body->charge_channel < 7 )
	{
		if( (body->max_map != D_GetMax_map()) || (body->max_type_mob != D_GetMax_type_mob()) || (body->max_mob != D_GetMax_mob()) )
		{
			U_Printf("[PROC]Error mobserv_fd(%d) MG_INIT bad max_map(%d)(%d) max_type_mob(%d)(%d) max_mob(%d)(%d)\n",
				mobserv_fd, D_GetMax_map(), body->max_map, D_GetMax_type_mob(), body->max_type_mob, D_GetMax_mob(), body->max_mob);
			P_send_size	= MK_GetPacket_GM_INIT_RESULT(P_send_buf, RESULT_FAIL_DATA);	
			U_Send_queue_in(SEND_UNICAST, mobserv_fd, 0, 0, 0, P_send_size, P_send_buf);
			P_remove_client(mobserv_fd);
			return;
		}
	}
	else
	{
		if( (body->max_map != D_GetMax_map()) || (body->max_type_mob != D_GetMax_type_mob())
									|| (body->max_mob != MAP_SUMMON_MOB_CNT*D_GetMax_map()) )
		{
			U_Printf("[PROC]Error mobserv_fd(%d) MG_INIT bad max_map(%d)(%d) max_type_mob(%d)(%d) max_mob(%d)(%d)\n", mobserv_fd,
			D_GetMax_map(), body->max_map, D_GetMax_type_mob(), body->max_type_mob, MAP_SUMMON_MOB_CNT*D_GetMax_map(), body->max_mob);
			P_send_size	= MK_GetPacket_GM_INIT_RESULT(P_send_buf, RESULT_FAIL_DATA);	
			U_Send_queue_in(SEND_UNICAST, mobserv_fd, 0, 0, 0, P_send_size, P_send_buf);
			P_remove_client(mobserv_fd);
			return;
		}
	}

	//check charge_channel
	if( (body->charge_channel < (CHANNEL_PER_MACHINE*(G_Machine-1)+1) ) || (body->charge_channel > (CHANNEL_PER_MACHINE*G_Machine)) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) MG_INIT bad charge_channel(%d)\n", mobserv_fd, body->charge_channel);
		P_send_size	= MK_GetPacket_GM_INIT_RESULT(P_send_buf, RESULT_FAIL_DATA);	
		U_Send_queue_in(SEND_UNICAST, mobserv_fd, 0, 0, 0, P_send_size, P_send_buf);
		P_remove_client(mobserv_fd);
		return;
	}

	channel_link	= body->charge_channel - (CHANNEL_PER_MACHINE*(G_Machine-1));

	//check P_mobserv_fd
	if( P_mobserv_fd[channel_link] ) 
	{
		U_Printf("[PROC]Error mobserv_fd(%d) MG_INIT P_mobserv_fd[%d](%d)\n", mobserv_fd, channel_link, P_mobserv_fd[channel_link]);
		P_send_size	= MK_GetPacket_GM_INIT_RESULT(P_send_buf, RESULT_FAIL_DATA);	
		U_Send_queue_in(SEND_UNICAST, mobserv_fd, 0, 0, 0, P_send_size, P_send_buf);
		P_remove_client(mobserv_fd);
		return;
	}

	D_SetClient_mobserv_charge_channel_link(mobserv_fd, channel_link);
	P_mobserv_fd[channel_link]	= mobserv_fd;
	U_Printf("[PROC]mobserv_fd(%d) MG_INIT charge_channel(%d) channel_link(%d) success\n", mobserv_fd, body->charge_channel, channel_link);
	P_send_size	= MK_GetPacket_GM_INIT_RESULT(P_send_buf, RESULT_SUCC);	
	U_Send_queue_in(SEND_UNICAST, mobserv_fd, 0, 0, 0, P_send_size, P_send_buf);

	//send GM_CHAR_CHANNEL_IN
	for( i=1; i<=D_GetMax_map(); i++ )
	{
		for( j=1; j<(VIEW_BLOCK_CNT-1); j++ )
		{
			char_link	= D_GetMapBlock_char_link(channel_link, i, j);
		
			while( char_link )
			{
				P_send_size	= MK_GetPacket_GM_CHAR_CHANNEL_IN(P_send_buf, &char_link, 1);
				U_Send_queue_in(SEND_UNICAST, mobserv_fd, 0, 0, 0, P_send_size, P_send_buf);
			}
		}
	}
}


static	void	P_proc_MG_LIVE(const int mobserv_fd)
{
	if( D_GetClient_sec_active(mobserv_fd) == 0 )
		return;

	P_send_size	= MK_GetPacket_GM_LIVE(P_send_buf);
	U_Send_queue_in(SEND_UNICAST, mobserv_fd, 0, 0, 0, P_send_size, P_send_buf);
}


static	void	P_proc_MG_MOB_REGEN(const int mobserv_fd)
{
	int				channel_link, block_x;
	struct	body_MG_MOB_REGEN	*body	= (struct body_MG_MOB_REGEN *)(P_recv_q->data);
	
	
	if( D_GetClient_sec_active(mobserv_fd) == 0 )
		return;
	
	channel_link	= D_GetClient_mobserv_charge_channel_link(mobserv_fd);	
	
	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_MG_MOB_REGEN) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_REGEN(%d) bad body_size(%d)\n",
						mobserv_fd, channel_link, sizeof(struct body_MG_MOB_REGEN), P_recv_q->data_size);
		P_remove_client(mobserv_fd);
		return;
	}

	//check map
	if( (body->map < 1) || (body->map > D_GetMax_map()) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_REGEN bad map(%d)\n", mobserv_fd, channel_link,  body->map);
		P_remove_client(mobserv_fd);
		return;
	}

	//check mob_id
	if( (body->mob_id < 1) || (body->mob_id > D_GetMax_mob()) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_REGEN bad mob_id(%d)\n", mobserv_fd, channel_link, body->mob_id);
		P_remove_client(mobserv_fd);
		return;
	}
	
	//check mob_type
	if( (body->mob_type < 1) || (body->mob_type > D_GetMax_type_mob()) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_REGEN bad mob_type(%d)\n", mobserv_fd, channel_link, body->mob_type);
		P_remove_client(mobserv_fd);
		return;
	}

	//check dir
	if( (body->dir < 0) || (body->dir >= DIR_NOTING) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_REGEN bad mob_id(%d) dir(%d)\n",
									mobserv_fd, channel_link, body->mob_id, body->dir);
		P_remove_client(mobserv_fd);
		return;
	}

	//check regen_x, regen_y, regen_z
	if( (body->regen_x < 1) || (body->regen_x > (MAP_X_TILE-2)) || (body->regen_y) || (body->regen_z < 1) || (body->regen_z > (MAP_Z_TILE-2)) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_REGEN bad mob_id(%d) regen_x(%d) regen_y(%d) regen_z(%d)\n",
								mobserv_fd, channel_link, body->mob_id, body->regen_x, body->regen_y, body->regen_z);
		P_remove_client(mobserv_fd);
		return;
	}

	//check map_attr
	if( D_GetMap_attr(body->map, body->regen_x, body->regen_z) == MAP_ATTR_COLLISION )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_REGEN bad mob_id(%d) regen_x(%d) regen_y(%d) regen_z(%d) attr(%d)\n",
							mobserv_fd, channel_link, body->mob_id, body->regen_x, body->regen_y, body->regen_z,
							D_GetMap_attr(body->map, body->regen_x, body->regen_z));
		P_remove_client(mobserv_fd);
		return;
	}

	//check current status
	if( D_GetMob_status(channel_link, body->mob_id) != _MOB_EXTINCT )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_REGEN bad mob_id(%d) status(%d)\n",
							mobserv_fd, channel_link, body->mob_id, D_GetMob_status(channel_link, body->mob_id));
		P_remove_client(mobserv_fd);
		return;
	}

	//mob regen
	if( D_SetMob_regen(channel_link, body->mob_id, body->mob_type, body->table_idx,
							body->map, body->dir, body->regen_x, body->regen_y, body->regen_z) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_REGEN bad D_SetMob_regen()\n", mobserv_fd, channel_link);
		P_remove_client(mobserv_fd);
		return;
	}

	//map block insert mob
	D_SetMapBlock_insert_mob(channel_link, body->mob_id);
	
	block_x	= D_GetMob_block_x(channel_link, body->mob_id);
	
	//send GC_MOB_APPEAR to 3block all
	U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_MOB_APPEAR channel_link(%d) mob_id(%d) map(%d) block_x(%d)(%d)(%d)\n", 
									channel_link, body->mob_id, body->map, block_x-1, block_x, block_x+1);
	P_send_size	= MK_GetPacket_GC_MOB_APPEAR(P_send_buf, channel_link, &(body->mob_id), 0);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, body->map, block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, body->map, block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, body->map, block_x+1, P_send_size, P_send_buf);
}

				
static	void	P_proc_MG_MOB_EXTINCT(const int mobserv_fd)
{
	int				channel_link, map, block_x;
	struct	body_MG_MOB_EXTINCT	*body	= (struct body_MG_MOB_EXTINCT *)(P_recv_q->data);


	if( D_GetClient_sec_active(mobserv_fd) == 0 )
		return;
	
	channel_link	= D_GetClient_mobserv_charge_channel_link(mobserv_fd);	

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_MG_MOB_EXTINCT) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_EXTINCT(%d) bad body_size(%d)\n"
				, mobserv_fd, channel_link, sizeof(struct body_MG_MOB_EXTINCT), P_recv_q->data_size);
		P_remove_client(mobserv_fd);
		return;
	}

	//check mob_id
	if( (body->mob_id < 1) || (body->mob_id > D_GetMax_mob()) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_EXTINCT bad mob_id(%d)\n", mobserv_fd, channel_link, body->mob_id);
		P_remove_client(mobserv_fd);
		return;
	}

	//check current status
	if( D_GetMob_status(channel_link, body->mob_id) != _MOB_DEATH )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_EXTINCT bad mob_id(%d) status(%d)\n",
							mobserv_fd, channel_link, body->mob_id, D_GetMob_status(channel_link, body->mob_id));
		P_remove_client(mobserv_fd);
		return;
	}

	//map block delete mob
	D_SetMapBlock_delete_mob(channel_link, body->mob_id);

	map	= D_GetMob_map(channel_link, body->mob_id);
	block_x	= D_GetMob_block_x(channel_link, body->mob_id);

	//send GC_MOB_DISAPPEAR to 3block all
	U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_MOB_DISAPPEAR channel_link(%d) mob_id(%d) map(%d) block_x(%d)(%d)(%d)\n",
									channel_link, body->mob_id, map, block_x-1, block_x, block_x+1);
	P_send_size	= MK_GetPacket_GC_MOB_DISAPPEAR(P_send_buf, channel_link, &(body->mob_id), 0);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);
	
	//mob clear
	D_SetMob_clear(channel_link, body->mob_id);
}


static	void	P_proc_MG_MOB_MOVE(const int mobserv_fd)
{
	int				channel_link;
	struct	body_MG_MOB_MOVE	*body	= (struct body_MG_MOB_MOVE *)(P_recv_q->data);


	if( D_GetClient_sec_active(mobserv_fd) == 0 )
		return;

	channel_link	= D_GetClient_mobserv_charge_channel_link(mobserv_fd);

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_MG_MOB_MOVE) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_MOVE(%d) bad body_size(%d)\n"
				, mobserv_fd, channel_link, sizeof(struct body_MG_MOB_MOVE), P_recv_q->data_size);
		P_remove_client(mobserv_fd);
		return;
	}

	//check mob_id
	if( (body->mob_id < 1) || (body->mob_id > D_GetMax_mob()) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_MOVE bad mob_id(%d)\n", mobserv_fd, channel_link, body->mob_id);
		P_remove_client(mobserv_fd);
		return;
	}

	//check current status
	if( D_GetMob_status(channel_link, body->mob_id) >= _MOB_DEATH )
		if( D_GetMob_status(channel_link, body->mob_id) != _MOB_REGEN )
			return;

	//move
	if( P_mob_move(channel_link, body->mob_id, body->dir, body->status, body->speed, body->x, body->y, body->z) )			
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) mob_id(%d) MG_MOB_MOVE P_mob_move()\n",
											mobserv_fd, channel_link, body->mob_id);
		P_remove_client(mobserv_fd);
		return;
	}
}


static	void	P_proc_MG_MOB_ATTACK_CHAR(const int mobserv_fd)
{
	int				channel_link, map, char_hp, damage, damage_type, attack_block_x;
	float				x, y, z;
	struct	body_MG_MOB_ATTACK_CHAR	*body	= (struct body_MG_MOB_ATTACK_CHAR *)(P_recv_q->data);


	if( D_GetClient_sec_active(mobserv_fd) == 0 )
		return;

	channel_link	= D_GetClient_mobserv_charge_channel_link(mobserv_fd);

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_MG_MOB_ATTACK_CHAR) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_ATTACK_CHAR(%d) bad body_size(%d)\n"
				, mobserv_fd, channel_link, sizeof(struct body_MG_MOB_ATTACK_CHAR), P_recv_q->data_size);
		P_remove_client(mobserv_fd);
		return;
	}

	//check mob_id
	if( (body->mob_id < 1) || (body->mob_id > D_GetMax_mob()) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_ATTACK_CHAR bad mob_id(%d)\n", mobserv_fd, channel_link, body->mob_id);
		P_remove_client(mobserv_fd);
		return;
	}

	//check char_id
	if( (body->char_id < 1) || (body->char_id > MAX_FD) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_ATTACK_CHAR bad char_id(%d)\n",
										mobserv_fd, channel_link, body->char_id);
		P_remove_client(mobserv_fd);
		return;
	}

	//check dir	
	if( (body->dir < 0) || (body->dir >= 360) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_ATTACK_CHAR bad dir(%d)\n", mobserv_fd, channel_link, body->dir);
		P_remove_client(mobserv_fd);
		return;
	}
	
	//check mob status
	if( D_GetMob_status(channel_link, body->mob_id) >= _MOB_DEATH )
		return;

	//check char status
	if( D_GetClient_status(body->char_id) >= _USER_DEAD )
		return;
	
	//attack
	if( D_Mob_attack_char(channel_link, body->mob_id, body->char_id, &char_hp, &damage_type, &damage) )
		return;
	
	//set dir
	D_SetMob_dir(channel_link, body->mob_id, body->dir);

	//char dead
	if( char_hp == 0 )
	{
		D_GetClient_xyz(body->char_id, &x, &y, &z);
		
		//send GC_CHAR_MOVE 
		U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GM_CHAR_MOVE _USER_DEATH user_id(%s) char_name(%s)\n",
				mobserv_fd, channel_link, D_GetClient_user_id(body->char_id), D_GetClient_char_name(body->char_id));
		P_send_size	= MK_GetPacket_GM_CHAR_MOVE(P_send_buf, body->char_id, _USER_DEAD, x, y, z);	
		U_Send_queue_in(SEND_UNICAST, mobserv_fd, 0, 0, 0, P_send_size, P_send_buf);
	}
	
	map		= D_GetMob_map(channel_link, body->mob_id);
	attack_block_x	= D_GetClient_block_x(body->char_id);

	//send GC_MOB_ATTACK_CHAR to 3block all
	U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_MOB_ATTACK_CHAR channel_link(%d) mob_id(%d) map(%d) block_x(%d)(%d)(%d) damage(%d) char_id(%d) char_name(%s) char_hp(%d)\n", channel_link, body->mob_id, map, attack_block_x-1, attack_block_x, attack_block_x+1, damage, body->char_id, D_GetClient_char_name(body->char_id), char_hp);
	P_send_size
		= MK_GetPacket_GC_MOB_ATTACK_CHAR(P_send_buf, channel_link, body->mob_id, body->char_id, char_hp, damage_type, damage, body->dir);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, attack_block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, attack_block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, attack_block_x+1, P_send_size, P_send_buf);
}


static	void	P_proc_MG_MOB_DAMAGE_LIST(const int mobserv_fd)
{
	int					channel_link, owner, max_damage, i, level_up, acquire_exp, map, block_x, item_id_list[5];
	struct	body1_MG_MOB_DAMAGE_LIST	*body1	= (struct body_MG_MOB_DAMAGE_LIST *)(P_recv_q->data);
	struct	body2_MG_MOB_DAMAGE_LIST	*body2
				= (struct body2_MG_MOB_DAMAGE_LIST *)((P_recv_q->data) + sizeof(struct body1_MG_MOB_DAMAGE_LIST));


	if( D_GetClient_sec_active(mobserv_fd) == 0 )
		return;

	channel_link	= D_GetClient_mobserv_charge_channel_link(mobserv_fd);

	//check body1_size 
	if( P_recv_q->data_size < sizeof(struct body1_MG_MOB_DAMAGE_LIST) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_DAMAGE_LIST(%d) bad body_size(%d)\n",
					mobserv_fd, channel_link, sizeof(struct body1_MG_MOB_DAMAGE_LIST), P_recv_q->data_size);
		P_remove_client(mobserv_fd);
		return;
	}

	//check list_cnt
	if( (body1->list_cnt < 1) || (body1->list_cnt > 10) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_DAMAGE_LIST bad list_cnt(%d)\n",
										mobserv_fd, channel_link, body1->list_cnt);
		P_remove_client(mobserv_fd);
		return;
	}
	
	//check body2_size 
	if( P_recv_q->data_size != (sizeof(struct body1_MG_MOB_DAMAGE_LIST)+sizeof(struct body2_MG_MOB_DAMAGE_LIST)*(body1->list_cnt)) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_DAMAGE_LIST(%d) bad body_size(%d)\n", mobserv_fd, channel_link,
			sizeof(struct body1_MG_MOB_DAMAGE_LIST)+sizeof(struct body2_MG_MOB_DAMAGE_LIST)*(body1->list_cnt), P_recv_q->data_size);
		P_remove_client(mobserv_fd);
		return;
	}

	//check mob_id
	if( (body1->mob_id < 1) || (body1->mob_id > D_GetMax_mob()) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_DAMAGE_LIST bad mob_id(%d)\n", mobserv_fd, channel_link, body1->mob_id);
		P_remove_client(mobserv_fd);
		return;
	}

	//check current status
	if( D_GetMob_status(channel_link, body1->mob_id) != _MOB_DEATH )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_DAMAGE_LIST bad mob_id(%d) status(%d)\n",
							mobserv_fd, channel_link, body1->mob_id, D_GetMob_status(channel_link, body1->mob_id));
		P_remove_client(mobserv_fd);
		return;
	}

	//find owner
	owner		= 0;
	max_damage	= 0;
	body2->damage	+= (body2->damage)*0.3f;

	for( i=0; i<body1->list_cnt; i++ )
	{
		//check char_id
		if( (body2->char_id < 1) || (body2->char_id > MAX_FD) )
		{
			U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_DAMAGE_LIST bad char_id(%d)\n",
											mobserv_fd, channel_link, body2->char_id);
			P_remove_client(mobserv_fd);
			return;
		}
	
		//check char loading
		if( *D_GetClient_char_name(body2->char_id) == 0 )
			continue;
		
		if( max_damage < body2->damage )	 
			owner	= body2->char_id;
	
		++body2;		
	}

	if( owner )
	{
		D_Char_kill_mob(channel_link, owner, body1->mob_id, &level_up, &acquire_exp, item_id_list);
			
		channel_link	= D_GetClient_channel_link(owner);
		map		= D_GetClient_map(owner);		
		block_x		= D_GetClient_block_x(owner);		
	
		if( acquire_exp )
		{
			//send GC_CHAR_ACQUIRE_EXP
			U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_ACQUIRE_EXP user_id(%s) char_name(%s)\n",
					owner, D_GetClient_user_id(owner), D_GetClient_char_name(owner));
			P_send_size	= MK_GetPacket_GC_CHAR_ACQUIRE_EXP(P_send_buf, owner, acquire_exp);
			U_Send_queue_in(SEND_UNICAST, owner, 0, 0, 0, P_send_size, P_send_buf);
		}

		while( level_up-- )
		{

			//send GC_CHAR_LEVEL_UP to 3block all
			U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_CHAR_LEVEL_UP channel_link(%d) fd(%d) map(%d) block_x(%d)(%d)(%d)\n",
											channel_link, owner, map, block_x-1, block_x, block_x+1);
			P_send_size	= MK_GetPacket_GC_CHAR_LEVEL_UP(P_send_buf, owner);
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_buf);
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
			U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);
		}

		//item drop
		for( i=0; i<5; i++ )
		{
			if( item_id_list[i] )
			{
				channel_link	= D_GetItem_channel_link(item_id_list[i]);
				map		= D_GetItem_map(item_id_list[i]);	
				block_x		= D_GetItem_block_x(item_id_list[i]);

				//map block insert item
				D_SetMapBlock_insert_item(item_id_list[i]);

				//send GC_ITEM_APPEAR to 3block all
				U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_ITEM_APPEAR channel_link(%d) item_id(%d) map(%d) block_x(%d)(%d)(%d)\n", channel_link, item_id_list[i], map, block_x-1, block_x, block_x+1);
				P_send_size	= MK_GetPacket_GC_ITEM_APPEAR(P_send_buf, &(item_id_list[i]), 0);
				U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_buf);
				U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
				U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);
			}	
		}	
	}
}


static	void	P_proc_MG_MOB_PERFECT_RECOVER(const int mobserv_fd)
{
	int					channel_link;
	struct	body_MG_MOB_PERFECT_RECOVER	*body	= (struct body_MG_MOB_PERFECT_RECOVER *)(P_recv_q->data);


	if( D_GetClient_sec_active(mobserv_fd) == 0 )
		return;

	channel_link	= D_GetClient_mobserv_charge_channel_link(mobserv_fd);

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_MG_MOB_PERFECT_RECOVER) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_PERFECT_RECOVER(%d) bad body_size(%d)\n"
				, mobserv_fd, channel_link, sizeof(struct body_MG_MOB_PERFECT_RECOVER), P_recv_q->data_size);
		P_remove_client(mobserv_fd);
		return;
	}

	//check mob_id
	if( (body->mob_id < 1) || (body->mob_id > D_GetMax_mob()) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_PERFECT_RECOVER bad mob_id(%d)\n",
										mobserv_fd, channel_link, body->mob_id);
		P_remove_client(mobserv_fd);
		return;
	}

	//check current status
	if( D_GetMob_status(channel_link, body->mob_id) >= _MOB_DEATH )
		return;

	//perfect recover
	D_SetMob_perfect_recover(channel_link, body->mob_id);	
}


static	void	P_proc_MG_MOB_SKILL_READY(const int mobserv_fd)
{
	int				channel_link, map, block_x;
	float				x, y, z;
	struct	body_MG_MOB_SKILL_READY	*body	= (struct body_MG_MOB_SKILL_READY *)(P_recv_q->data);


	if( D_GetClient_sec_active(mobserv_fd) == 0 )
		return;

	channel_link	= D_GetClient_mobserv_charge_channel_link(mobserv_fd);

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_MG_MOB_SKILL_READY) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_SKILL_READY(%d) bad body_size(%d)\n",
					mobserv_fd, channel_link, sizeof(struct body_MG_MOB_SKILL_READY), P_recv_q->data_size);
		P_remove_client(mobserv_fd);
		return;
	}
	
	//check mob_id
	if( (body->mob_id < 1) || (body->mob_id > D_GetMax_mob()) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_SKILL_READY bad mob_id(%d)\n", mobserv_fd, channel_link, body->mob_id);
		P_remove_client(mobserv_fd);
		return;
	}

	//check skill_type
	if( (body->skill_type < 1) || (body->skill_type > D_GetMax_type_mob_skill()) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_SKILL_READY mob_id(%d) bad skill_type(%d)\n",
									mobserv_fd, channel_link, body->mob_id, body->skill_type);
		P_remove_client(mobserv_fd);
		return;
	}
	
	//check dir	
	if( (body->dir < 0) || (body->dir >= 360) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_SKILL_READY mob_id(%d) bad dir(%d)\n",
									mobserv_fd, channel_link, body->mob_id, body->dir);
		P_remove_client(mobserv_fd);
		return;
	}

	//check current status
	if( D_GetMob_status(channel_link, body->mob_id) >= _MOB_DEATH )
		return;

	//set dir
	D_SetMob_dir(channel_link, body->mob_id, body->dir);

	map		= D_GetMob_map(channel_link, body->mob_id);
	block_x		= D_GetMob_block_x(channel_link, body->mob_id);
	D_GetMob_xyz(channel_link, body->mob_id, &x, &y, &z);

	//send GC_MOB_SKILL_READY to 3block all
	U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_MOB_SKILL_READY channel_link(%d) mob_id(%d) map(%d) block_x(%d)(%d)(%d)\n",
									channel_link, body->mob_id, map, block_x-1, block_x, block_x+1);
	P_send_size	= MK_GetPacket_GC_MOB_SKILL_READY(P_send_buf, body->mob_id, body->skill_type, body->dir, 0.0f, x, y, z);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);
}


static	void	P_proc_MG_MOB_SKILL_ACTION(const int mobserv_fd)
{
	int	channel_link, map, block_x, i, j;
	float	x, y, z;
	struct	skill_action_result		result;
	struct	body1_MG_MOB_SKILL_ACTION	*body1			= (struct body1_MG_MOB_SKILL_ACTION *)(P_recv_q->data);
	

	if( D_GetClient_sec_active(mobserv_fd) == 0 )
		return;
	
	channel_link	= D_GetClient_mobserv_charge_channel_link(mobserv_fd);

	//check body1_size
	if( P_recv_q->data_size < sizeof(struct body1_MG_MOB_SKILL_ACTION) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_SKILL_ACTION(%d) bad body_size(%d)\n",
						mobserv_fd, channel_link, sizeof(struct body1_MG_MOB_SKILL_ACTION), P_recv_q->data_size);
		P_remove_client(mobserv_fd);
		return;
	}
	
	//check target_cnt
	if(  (body1->target_cnt < 0) || (body1->target_cnt > MAX_SKILL_TARGET_CNT) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_SKILL_ACTION mob_id(%d) bad target_cnt(%d)\n",
									mobserv_fd, channel_link, body1->mob_id, body1->target_cnt);
		P_remove_client(mobserv_fd);
		return;
	}
	
	//check body2_size
	if( P_recv_q->data_size != (sizeof(struct body1_MG_MOB_SKILL_ACTION)+sizeof(struct body2_MG_MOB_SKILL_ACTION)*(body1->target_cnt)) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_SKILL_ACTION(%d) bad body_size(%d)\n", mobserv_fd, channel_link,
			sizeof(struct body1_MG_MOB_SKILL_ACTION)+sizeof(struct body2_MG_MOB_SKILL_ACTION)*(body1->target_cnt), P_recv_q->data_size);
		P_remove_client(mobserv_fd);
		return;
	}

	//check mob_id
	if( (body1->mob_id < 1) || (body1->mob_id > D_GetMax_mob()) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_SKILL_ACTION bad mob_id(%d)\n",
										mobserv_fd, channel_link, body1->mob_id);
		P_remove_client(mobserv_fd);
		return;
	}

	//check skill_type
	if( (body1->skill_type < 1) || (body1->skill_type > D_GetMax_type_mob_skill()) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_SKILL_ACTION mob_id(%d) bad skill_type(%d)\n",
									mobserv_fd, channel_link, body1->mob_id, body1->skill_type);
		P_remove_client(mobserv_fd);
		return;
	
	}
	
	//check dir
	if( (body1->dir < 0) || (body1->dir >= 360) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_SKILL_ACTION mob_id(%d) bad dir(%d)\n",
									mobserv_fd, channel_link, body1->mob_id, body1->dir);
		P_remove_client(mobserv_fd);
		return;
	}
	
	//check target xyz 
	if( (body1->target_x < 1) || (body1->target_x > (MAP_X_TILE-2)) || (body1->target_z < 1) || (body1->target_z > (MAP_Z_TILE-2)) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_SKILL_ACTION mob_id(%d) bad target_x(%f) target_y(%f) target_z(%f)\n",
							mobserv_fd, channel_link, body1->mob_id, body1->target_x, body1->target_y, body1->target_z);
		P_remove_client(mobserv_fd);
		return;
	}

	//check current status
	if( D_GetMob_status(channel_link, body1->mob_id) >= _MOB_DEATH )
		return;
	
	//skill action	
	if( D_Mob_skill_action(channel_link, body1->mob_id, body1, P_send_client_buf, &result) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_SKILL_ACTION D_Mob_skill_action()\n", mobserv_fd, channel_link);
		return;
	}

	//set dir
	D_SetMob_dir(channel_link, body1->mob_id, body1->dir);

	//send GM_CHAR_MOVE
	for( i=0; i<result.dead_char_cnt; i++)
	{
		D_GetClient_xyz(result.dead_char_list[i], &x, &y, &z);

		U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GM_CHAR_MOVE _USER_DEATH user_id(%s) char_name(%s)\n",
			mobserv_fd, channel_link, D_GetClient_user_id(result.dead_char_list[i]), D_GetClient_char_name(result.dead_char_list[i]));
		P_send_size	= MK_GetPacket_GM_CHAR_MOVE(P_send_buf, result.dead_char_list[i], _USER_DEAD, x, y, z);	
		U_Send_queue_in(SEND_UNICAST, mobserv_fd, 0, 0, 0, P_send_size, P_send_buf);
	}
	
	map	= D_GetMob_map(channel_link, body1->mob_id);	
	block_x	= D_GetMob_block_x(channel_link, body1->mob_id);	
	
	//send GC_MOB_SKILL_ACTION to 5block all
	U_Printf("[PROC]send_queue_in 5block BLOCK_ALLCAST GC_MOB_SKILL_ACTION skill_type(%d) channel_link(%d) mob_id(%d) map(%d) block_x(%d)(%d)(%d)(%d)(%d)\n", body1->skill_type, channel_link, body1->mob_id, map, block_x-2, block_x-1, block_x, block_x+1, block_x+2);
	P_send_size	= MK_GetPacket_GC_MOB_SKILL_ACTION(P_send_client_buf);	
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_client_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_client_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-2, P_send_size, P_send_client_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_client_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+2, P_send_size, P_send_client_buf);

	//send GC_CHAR_BUFF
	for( i=0; i<result.buff_cnt; i++ )
	{
		for( j=0; j<result.buff_char_cnt; j++)
		{
			U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_BUFF(%d) user_id(%s) char_name(%s)\n", result.buff_char_list[j],
			result.buff_serise[i], D_GetClient_user_id(result.buff_char_list[j]), D_GetClient_char_name(result.buff_char_list[j]));
			P_send_size	= MK_GetPacket_GC_CHAR_BUFF(P_send_buf, result.buff_char_list[j], result.buff_serise[i]);
			U_Send_queue_in(SEND_UNICAST, result.buff_char_list[j], 0, 0, 0, P_send_size, P_send_buf);
		}		
	}
	
	//send GM_MOB_BUFF
	for( i=0; i<result.buff_cnt; i++ )
	{
		for( j=0; j<result.buff_mob_cnt; j++)
		{
			U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GM_MOB_BUFF mob_id(%d)\n",
									mobserv_fd, channel_link, result.buff_mob_list[j]);
			P_send_size	= MK_GetPacket_GM_MOB_BUFF(P_send_buf, channel_link, result.buff_mob_list[j], result.buff_serise[i]);
			U_Send_queue_in(SEND_UNICAST, mobserv_fd, 0, 0, 0, P_send_size, P_send_buf);
		}			
	}
}

				
static	void	P_proc_MG_MOB_SKILL_CANCEL(const int mobserv_fd)
{
	int					channel_link, map, block_x;
	struct	body_MG_MOB_SKILL_CANCEL	*body	= (struct body_MG_MOB_SKILL_CANCEL *)(P_recv_q->data);


	if( D_GetClient_sec_active(mobserv_fd) == 0 )
		return;

	channel_link	= D_GetClient_mobserv_charge_channel_link(mobserv_fd);

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_MG_MOB_SKILL_CANCEL) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_SKILL_CANCEL(%d) bad body_size(%d)\n",
					mobserv_fd, channel_link, sizeof(struct body_MG_MOB_SKILL_CANCEL), P_recv_q->data_size);
		P_remove_client(mobserv_fd);
		return;
	}
	
	//check mob_id
	if( (body->mob_id < 1) || (body->mob_id > D_GetMax_mob()) )
	{
		U_Printf("[PROC]Error mobserv_fd(%d) channel_link(%d) MG_MOB_SKILL_CANCEL bad mob_id(%d)\n", mobserv_fd, channel_link, body->mob_id);
		P_remove_client(mobserv_fd);
		return;
	}

	//check current status
	if( D_GetMob_status(channel_link, body->mob_id) >= _MOB_DEATH )
		return;

	map		= D_GetMob_map(channel_link, body->mob_id);
	block_x		= D_GetMob_block_x(channel_link, body->mob_id);

	//send GC_MOB_SKILL_READY to 3block all
	U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_MOB_SKILL_CANCEL channel_link(%d) mob_id(%d) map(%d) block_x(%d)(%d)(%d)\n",
									channel_link, body->mob_id, map, block_x-1, block_x, block_x+1);
	P_send_size	= MK_GetPacket_GC_MOB_SKILL_CANCEL(P_send_buf, body->mob_id);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);
}



//______________________________________________________________
//CG
static	void	P_proc_CG_LOGIN(const int fd)
{
	int			use_login_dbagent_fd;
	struct	body_CG_LOGIN	*body	= (struct body_CG_LOGIN *)(P_recv_q->data);


	if( D_GetClient_sec_active(fd) == 0 )
		return;
	
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_LOGIN) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_LOGIN(%d) bad body_size(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_LOGIN), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}

	//check first packet
	if( P_recv_q->recv_cnt != 1 )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_LOGIN bad recv_cnt(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), P_recv_q->recv_cnt);
		P_remove_client(fd);
		return;
	}

	//check version
	if( (body->version[0] == 0) || (body->version[VERSION_LEN] != 0) || (strncmp(body->version, VERSION, VERSION_LEN+1)) )
	{
		body->version[VERSION_LEN]	= 0;
		U_Printf("[PROC]Error fd(%d) user_id(%s) char_name(%s) CG_LOGIN bad version(%s)\n",
					fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->version);
		P_remove_client(fd);
		return;
	}
	
	//check user_id
	if( (body->user_id[0] == 0) || (body->user_id[USER_ID_LEN] != 0) )
	{
		body->user_id[USER_ID_LEN]	= 0;
		U_Printf("[PROC]Error fd(%d) user_id(%s) char_name(%s) CG_LOGIN bad user_id(%s)\n",
					fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->user_id);
		P_remove_client(fd);
		return;
	}

	//check encodedstr
	if( (body->encodedstr[0] == 0) || (body->encodedstr[ENCODEDSTR_LEN] != 0) )
	{
		body->encodedstr[ENCODEDSTR_LEN]	= 0;
		U_Printf("[PROC]Error fd(%d) user_id(%s) char_name(%s) CG_LOGIN bad user_id(%s) encodedstr(%s)\n",
					fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->user_id, body->encodedstr);
		P_remove_client(fd);
		return;
	}
		
	/*
	use_billing_fd	= P_billing_fd[D_GetClient_use_billing_idx(fd)];
	
	if( use_billing_fd )
	{
		//set lock_user_id
		D_SetClient_lock_user_id(fd, body->user_id);

		//send WB_GAMESTART
		U_Printf("[PROC]send_queue_in UNICAST(%d) WB_GAMESTART user_id(%s)\n", use_billing_fd, body->user_id);
		P_send_size	= MK_GetPacket_WB_GAMESTART(P_send_buf, fd, body->user_id);
		U_Send_queue_in(SEND_UNICAST, use_billing_fd, 0, 0, 0, P_send_size, P_send_buf);
	}
	else
	{
		//send GC_LOGIN_RESULT
		U_Printf("[PROC]send_queue_in UNICAST(%d) GC_LOGIN_RESULT RESULT_FAIL_BILLING user_id(%s)\n", fd, body->user_id);
		P_send_size	= MK_GetPacket_GC_LOGIN_RESULT(P_send_buf, RESULT_FAIL_BILLING);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);

		//remove client
		P_remove_client(fd);
	}
	*/

	use_login_dbagent_fd	= P_login_dbagent_fd[D_GetClient_use_login_dbagent_idx(fd)];
	
	if( use_login_dbagent_fd )
	{
		//send WA_LOGIN
		U_Printf("[PROC]send_queue_in UNICAST(%d) WA_LOGIN user_id(%s)\n", use_login_dbagent_fd, body->user_id);
		P_send_size	= MK_GetPacket_WA_LOGIN(P_send_buf, fd, body->user_id, body->encodedstr);
		U_Send_queue_in(SEND_UNICAST, use_login_dbagent_fd, 0, 0, 0, P_send_size, P_send_buf);
	}
	else
	{
		//send GC_LOGIN_RESULT
		U_Printf("[PROC]send_queue_in UNICAST(%d) GC_LOGIN_RESULT result_code(%d) user_id(%s)\n", fd, RESULT_FAIL_DBAGENT, body->user_id);
		P_send_size	= MK_GetPacket_GC_LOGIN_RESULT(P_send_buf, RESULT_FAIL_DBAGENT);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);

		//remove client
		P_remove_client(fd);
	}
	
	/*	
	//set G_Login_number
	++G_Login_number;

	//set user_id
	D_SetClient_user_id(fd, body->user_id);
		
	//send GC_LOGIN_RESULT
	U_Printf("[PROC]send_queue_in LOGIN(%d) GC_LOGIN_RESULT RESULT_SUCC user_id(%s)\n", fd, body->user_id);
	P_send_size	= MK_GetPacket_GC_LOGIN_RESULT(P_send_buf, RESULT_SUCC);
	U_Send_queue_in(SEND_LOGIN, fd, 0, 0, 0, P_send_size, P_send_buf);
	*/
}


static	void	P_proc_CG_LIVE(const int fd)
{
	if( D_GetClient_sec_active(fd) == 0 )
		return;

	P_send_size	= MK_GetPacket_GC_LIVE(P_send_buf);
	U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
}


static	void	P_proc_CG_CHANNEL_LIST(const int fd)
{
	if( D_GetClient_sec_active(fd) == 0 )
		return;

	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHANNEL_LIST) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHANNEL_LIST(%d) bad body_size(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_CHANNEL_LIST), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}
	
	//check char status
	if( D_GetClient_status(fd) >= _USER_DEAD_REGEN )
		return;
	
	//send GC_CHANNEL_LIST_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHANNEL_LIST_RESULT user_id(%s) char_name(%s)\n",
								fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
	P_send_size	= MK_GetPacket_GC_CHANNEL_LIST_RESULT(P_send_buf, RESULT_SUCC, G_World);
	U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
}


static	void	P_proc_CG_CHAR_CHANNEL_MOVE(int fd)
{
	int					result_code, channel_link, map, block_x;
	struct	body_CG_CHAR_CHANNEL_MOVE	*body	= (struct body_CG_CHAR_CHANNEL_MOVE *)(P_recv_q->data);


	if( D_GetClient_sec_active(fd) == 0 )
		return;

	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHAR_CHANNEL_MOVE) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_CHANNEL_MOVE(%d) bad body_size(%d)\n", fd,
			D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_CHAR_CHANNEL_MOVE), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}
	
	//check char status
	if( D_GetClient_status(fd) >= _USER_DEAD_REGEN )
		return;
	
	//check channel_idx
	if( (body->channel_idx < 1) || (body->channel_idx > D_GetWorld_channel_cnt(G_World)) ) 
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHANNEL_CONNECT bad channel_idx(%d)\n",
						fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->channel_idx);
		P_remove_client(fd);
		return;
	}
	
	if( D_GetWorld_channel_user(G_World, body->channel_idx) >= USER_PER_CHANNEL )
		result_code	= RESULT_FAIL_CHANNEL_FULL;
	else if( ((body->channel_idx-1)/CHANNEL_PER_MACHINE +1) != G_Machine )
		result_code	= RESULT_FAIL_CHANNEL_RECONNECT;
	else
		result_code	= RESULT_SUCC;
	
	if( result_code )
	{	
		//send GC_CHAR_CHANNEL_MOVE_RESULT
		U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_CHANNEL_MOVE_RESULT user_id(%s) char_name(%s)\n",
									fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_send_size	= MK_GetPacket_GC_CHAR_CHANNEL_MOVE_RESULT(P_send_buf, result_code, fd, G_World, body->channel_idx);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
		return;
	}

	channel_link	= D_GetClient_channel_link(fd);	
	map		= D_GetClient_map(fd);	
	block_x		= D_GetClient_block_x(fd);	
	
	//set channel user
	D_SetWorld_channel_user_plus(G_World, channel_link+CHANNEL_PER_MACHINE*(G_Machine-1), -1);
	D_SetWorld_channel_user_plus(G_World, body->channel_idx, 1);

	//map block delete char
	D_SetMapBlock_delete_char(fd);
	U_Send_queue_in(SEND_INTER_BLOCK_DELETE_CHAR, fd, 0, 0, 0, 0, NULL);
	
	//send GC_OTHER_CHAR_DISAPPEAR to 3block char
	U_Printf("[PROC]send_queue_in 3block BLOCK_ALLCAST GC_OTHER_CHAR_DISAPPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, block_x-1, block_x, block_x+1);
	P_send_size	= MK_GetPacket_GC_OTHER_CHAR_DISAPPEAR(P_send_buf, &fd, 0);	
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);
	
	channel_link	= body->channel_idx - (CHANNEL_PER_MACHINE*(G_Machine-1));
	
	//set status, channel_link
	D_SetClient_status(fd, _USER_REGEN);
	D_SetClient_channel_link(fd, channel_link);
		
	//send GC_CHAR_CHANNEL_MOVE_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_CHANNEL_MOVE_RESULT user_id(%s) char_name(%s)\n",
								fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
	P_send_size	= MK_GetPacket_GC_CHAR_CHANNEL_MOVE_RESULT(P_send_buf, result_code, fd, G_World, body->channel_idx);
	U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);

	//send 3block GC_XXXX_APPEAR to owner
	P_send_q_in_block_GC_XXXX_APPEAR(fd, channel_link, map, block_x);
	P_send_q_in_block_GC_XXXX_APPEAR(fd, channel_link, map, block_x-1);
	P_send_q_in_block_GC_XXXX_APPEAR(fd, channel_link, map, block_x+1);
						
	//send GC_OTHER_CHAR_APPEAR to 3block char
	U_Printf("[PROC]send_queue_in 3block BLOCK_ALLCAST GC_OTHER_CHAR_APPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, block_x-1, block_x, block_x+1);
	P_send_size	= MK_GetPacket_GC_OTHER_CHAR_APPEAR(P_send_buf, &fd, 0);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);
			
	//map block insert char
	D_SetMapBlock_insert_char(fd);
	U_Send_queue_in(SEND_INTER_BLOCK_INSERT_CHAR, fd, channel_link, map, block_x, 0, NULL);

	//send GM_CHAR_CHANNEL_IN
	if( P_mobserv_fd[channel_link] )
	{
		U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GM_CHAR_CHANNEL_IN fd(%d) user_id(%s) char_name(%s)\n",
						P_mobserv_fd[channel_link], channel_link, fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_send_size	= MK_GetPacket_GM_CHAR_CHANNEL_IN(P_send_buf, &fd, 0);
		U_Send_queue_in(SEND_UNICAST, P_mobserv_fd[channel_link], 0, 0, 0, P_send_size, P_send_buf);
	}
		
}


static	void	P_proc_CG_CHAR_LOAD(const int fd)
{
	int				use_dbagent_fd;
	struct	body_CG_CHAR_LOAD	*body	= (struct body_CG_CHAR_LOAD *)(P_recv_q->data);	


	if( D_GetClient_sec_active(fd) == 0 )
		return;

	//check login
	if( *D_GetClient_user_id(fd) == 0 )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_LOAD not login\n",
							fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_remove_client(fd);
		return;
	}
	
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHAR_LOAD) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_LOAD(%d) bad body_size(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_CHAR_LOAD), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}

	//check pc loading
	if( *D_GetClient_char_name(fd) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_LOAD already pc loading\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_remove_client(fd);
		return;
	}
	
	//check channel_idx
	if( (body->channel_idx < (CHANNEL_PER_MACHINE*(G_Machine-1)+1) ) || (body->channel_idx > (CHANNEL_PER_MACHINE*G_Machine)) )
	{
		U_Printf("[PROC]Error fd(%d) user_id(%s) char_name(%s) CG_CHAR_LOAD bad channel_idx(%d)\n",
					fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->channel_idx);
		P_remove_client(fd);
		return;
	}

	//check char_name
	if( (body->char_name[0] == 0) || (body->char_name[CHAR_NAME_LEN] != 0) || (U_Check_string(body->char_name, CHAR_NAME_LEN)) )
	{
		body->char_name[CHAR_NAME_LEN]	= 0;
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_LOAD bad char_name(%s)\n",
					fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->char_name);
		P_remove_client(fd);
		return;
	}


	use_dbagent_fd	= P_dbagent_fd[D_GetClient_use_dbagent_idx(fd)];	

	if( use_dbagent_fd )	
	{
		//set channel_link
		D_SetClient_channel_link(fd, body->channel_idx - (CHANNEL_PER_MACHINE*(G_Machine-1)));

		
		//send WA_CHAR_LOAD
		U_Printf("[PROC]send_queue_in UNICAST(%d) WA_CHAR_LOAD user_id(%s) char_name(%s)\n",
						use_dbagent_fd, D_GetClient_user_id(fd), body->char_name);
		P_send_size	= MK_GetPacket_WA_CHAR_LOAD(P_send_buf, fd, body->char_name);
		U_Send_queue_in(SEND_UNICAST, use_dbagent_fd, 0, 0, 0, P_send_size, P_send_buf);
	}
	else
	{	
		//send GC_CHAR_LOAD_RESULT
		U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_LOAD_RESULT RESULT_FAIL_DBAGENT user_id(%s) char_name(%s)\n",
				fd, D_GetClient_user_id(fd), body->char_name);
		P_send_size	= MK_GetPacket_GC_CHAR_LOAD_RESULT(P_send_buf, RESULT_FAIL_DBAGENT, fd, body->char_name);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
	}
}


static	void	P_proc_CG_CHAR_LOAD_END(const int fd)
{
	float	x, y, z;
	int	channel_link, status, map, block_x;


	if( D_GetClient_sec_active(fd) == 0 )
		return;

	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHAR_LOAD_END) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_LOAD_END(%d) bad body_size(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_CHAR_LOAD_END), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}
	
	//check char status 
	status	= D_GetClient_status(fd);

	if( status == _USER_REGEN )
	{	
		status	= _USER_NORMAL_WAIT1;
	}
	else if( status == _USER_DEAD_REGEN )
	{
		status	= _USER_DEAD;
	}
	else
	{		
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%d) CG_CHAR_LOAD_END bad status(%d)\n",
						fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), status);
		P_remove_client(fd);
		return;
	}

	//set status
	D_SetClient_status(fd, status);
	
	//send GC_CHAR_MOVE to 3block all
	channel_link	= D_GetClient_channel_link(fd);
	map		= D_GetClient_map(fd);
	block_x		= D_GetClient_block_x(fd);
	D_GetClient_xyz(fd, &x, &y, &z);	
	
	U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_CHAR_MOVE user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)(%d)\n",
					D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, block_x-1, block_x, block_x+1);
	P_send_size	= MK_GetPacket_GC_CHAR_MOVE(P_send_buf, fd, DIR_S, status, 0, x, y, z);	
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);
	
	//send GM_CHAR_MOVE
	if( P_mobserv_fd[channel_link] )
	{
		U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GM_CHAR_MOVE user_id(%s) char_name(%s)\n",
				P_mobserv_fd[channel_link], channel_link, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_send_size	= MK_GetPacket_GM_CHAR_MOVE(P_send_buf, fd, status, x, y, z);	
		U_Send_queue_in(SEND_UNICAST, P_mobserv_fd[channel_link], 0, 0, 0, P_send_size, P_send_buf);
	}
}


static	void	P_proc_CG_CHAR_MOVE(const int fd)
{
	struct	body_CG_CHAR_MOVE	*body	= (struct body_CG_CHAR_MOVE *)(P_recv_q->data);

	
	if( D_GetClient_sec_active(fd) == 0 )
		return;
	
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHAR_MOVE) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_MOVE(%d) bad body_size(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_CHAR_MOVE), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}

	//check char status
	if( D_GetClient_status(fd) >= _USER_DEAD )
		return;

	//move
	if( P_char_move(fd, body->dir, body->status, body->speed, body->x, body->y, body->z) )			
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_MOVE P_char_move()\n",
							fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_remove_client(fd);
		return;
	}
}
				
				
static	void	P_proc_CG_CHAR_CHAT(const int fd)
{
	int	channel_link, map, block_x;


	if( D_GetClient_sec_active(fd) == 0 )
		return;
	
	//check body_size
	if( P_recv_q->data_size < 2 )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_CHAT bad body_size(%d)\n",
						fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}
	
	//check char status
	if( D_GetClient_status(fd) >= _USER_DEAD_REGEN )
		return;

	//check msg
	if( (*(P_recv_q->data) == 0) || (P_recv_q->data[P_recv_q->data_size-1]) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_CHAT msg is null\n",
						fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_remove_client(fd);
		return;
	}	

	channel_link	= D_GetClient_channel_link(fd);
	map		= D_GetClient_map(fd);
	block_x		= D_GetClient_block_x(fd);
		
	//send GC_CHAR_CHAT to 3block all
	U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_CHAR_CHAT fd(%d) user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)(%d) msg(%s)\n", fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, block_x-1, block_x, block_x+1, P_recv_q->data);
	P_send_size	= MK_GetPacket_GC_CHAR_CHAT(P_send_buf, fd, P_recv_q->data_size, P_recv_q->data);	
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);
}


static	void	P_proc_CG_CHAR_ATTACK_MOB(const int fd)
{
	struct	body_CG_CHAR_ATTACK_MOB	*body	= (struct body_CG_CHAR_ATTACK_MOB *)(P_recv_q->data);

	
	if( D_GetClient_sec_active(fd) == 0 )
		return;
	
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHAR_ATTACK_MOB) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_ATTACK_MOB(%d) bad body_size(%d)\n",
			fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_CHAR_ATTACK_MOB), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}

	//check current status
	if( D_GetClient_status(fd) >= _USER_DEAD )
		return;
	
	if( P_char_attack_mob(fd, body->mob_id, body->attack_id, body->attack_type, body->dir, body->speed, body->x, body->y, body->z) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_ATTACK_MOB P_char_attack_mob()\n", 
						fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_remove_client(fd);
		return;
	}
}


static	void	P_proc_CG_CHAR_REGEN(int fd)
{
	int	channel_link, map, block_x;
	float	x, y, z;


	if( D_GetClient_sec_active(fd) == 0 )
		return;
	
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHAR_REGEN) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_REGEN(%d) bad body_size(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_CHAR_REGEN), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}

	//check char status
	if( D_GetClient_status(fd) != _USER_DEAD )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_REGEN bad current status(%d)\n",
					fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), D_GetClient_status(fd));
		P_remove_client(fd);
		return;
	}

	channel_link	= D_GetClient_channel_link(fd);
	map		= D_GetClient_map(fd);	
	block_x		= D_GetClient_block_x(fd);	

	//map block delete char
	D_SetMapBlock_delete_char(fd);
	U_Send_queue_in(SEND_INTER_BLOCK_DELETE_CHAR, fd, 0, 0, 0, 0, NULL);
	
	//send GC_OTHER_CHAR_DISAPPEAR to 3block char
	U_Printf("[PROC]send_queue_in 3block BLOCK_ALLCAST GC_OTHER_CHAR_DISAPPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, block_x-1, block_x, block_x+1);
	P_send_size	= MK_GetPacket_GC_OTHER_CHAR_DISAPPEAR(P_send_buf, &fd, 0);	
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);

	//char regen
	D_SetClient_regen(fd);

	//send GC_CHAR_REGEN_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_REGEN_RESULT user_id(%s) char_name(%s)\n",
							fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
	P_send_size	= MK_GetPacket_GC_CHAR_REGEN_RESULT(P_send_buf, RESULT_SUCC, fd);
	U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);

	map	= D_GetClient_map(fd);	
	block_x	= D_GetClient_block_x(fd);	
	
	//send 3block GC_XXXX_APPEAR to owner
	P_send_q_in_block_GC_XXXX_APPEAR(fd, channel_link, map, block_x);
	P_send_q_in_block_GC_XXXX_APPEAR(fd, channel_link, map, block_x-1);
	P_send_q_in_block_GC_XXXX_APPEAR(fd, channel_link, map, block_x+1);
						
	//send GC_OTHER_CHAR_APPEAR to 3block char
	U_Printf("[PROC]send_queue_in 3block BLOCK_ALLCAST GC_OTHER_CHAR_APPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, block_x-1, block_x, block_x+1);
	P_send_size	= MK_GetPacket_GC_OTHER_CHAR_APPEAR(P_send_buf, &fd, 0);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);
			
	//map block insert char
	D_SetMapBlock_insert_char(fd);
	U_Send_queue_in(SEND_INTER_BLOCK_INSERT_CHAR, fd, channel_link, map, block_x, 0, NULL);

	//send GM_CHAR_MAP_CHANGE
	if( P_mobserv_fd[channel_link] )
	{
		U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GM_CHAR_MAP_CHANGE fd(%d) user_id(%s) char_name(%s)\n",
						P_mobserv_fd[channel_link], channel_link, fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_send_size	= MK_GetPacket_GM_CHAR_MAP_CHANGE(P_send_buf, fd);
		U_Send_queue_in(SEND_UNICAST, P_mobserv_fd[channel_link], 0, 0, 0, P_send_size, P_send_buf);
	}
	
	map	= D_GetClient_map(fd);
	D_GetClient_xyz(fd, &x, &y, &z);

	U_Printf("[PROC]regen fd(%d) user_id(%s) char_name(%s) map(%d) x(%f) z(%f)\n",
							fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), map, x, z);

}


static	void	P_proc_CG_CHAR_ITEM_MOVE(const int fd)
{
	int				channel_link, map, block_x, i;
	struct	body_CG_CHAR_ITEM_MOVE	*body	= (struct body_CG_CHAR_ITEM_MOVE *)(P_recv_q->data);

	
	if( D_GetClient_sec_active(fd) == 0 )
		return;
	
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHAR_ITEM_MOVE) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_ITEM_MOVE(%d) bad body_size(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_CHAR_ITEM_MOVE), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}

	//check char status
	if( D_GetClient_status(fd) >= _USER_DEAD_REGEN )
		return;

	//check item_slot1, item_slot2
	if( (body->item_slot1 == body->item_slot2) || (body->item_slot1 >= ITEM_SLOT_CNT)
				|| (body->item_slot2 >= ITEM_SLOT_CNT) || (body->item_slot2 < EQUIP_SLOT_CNT) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_ITEM_MOVE bad item_slot1(%d) item_slot2(%d)\n",
				fd, D_GetClient_user_id(fd), body->item_slot1, body->item_slot2);
		P_remove_client(fd);
		return;
	}

	//item move
	if( D_SetClient_item_move(fd, body->item_slot1, body->item_slot2, P_char_buff) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_ITEM_MOVE D_SetClient_item_move()\n",
									fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_remove_client(fd);
		return;
	}
	
	//send GC_CHAR_BUFF
	for( i=PASSIVE_BUFF_CNT; i<EQUIP_PASSIVE_BUFF_CNT; i++ )
	{
		if( P_char_buff[i] )
		{
			U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_BUFF(%d) user_id(%s) char_name(%s)\n",
								fd, i, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
			P_send_size	= MK_GetPacket_GC_CHAR_BUFF(P_send_buf, fd, i);
			U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
		}
	}
	
	//send GC_CHAR_ITEM_SLOT to 3block char
	channel_link	= D_GetClient_channel_link(fd);
	map		= D_GetClient_map(fd);	
	block_x		= D_GetClient_block_x(fd);	

	if( body->item_slot1 < EQUIP_SLOT_CNT )
	{
		U_Printf("[PROC]send_queue_in 3block BLOCK_OTHERCAST GC_CHAR_ITEM_SLOT user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, block_x-1, block_x, block_x+1);
		P_send_size	= MK_GetPacket_GC_CHAR_ITEM_SLOT(P_send_buf, fd, body->item_slot1);	
		U_Send_queue_in(SEND_BLOCK_OTHERCAST, fd, channel_link, map, block_x, P_send_size, P_send_buf);
		U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
		U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);
	}
}
	
			
static	void	P_proc_CG_CHAR_ITEM_PICKUP(const int fd)
{
	int					item_id, item_class, acquire_money, channel_link, map, block_x, result_code;
	struct	body_CG_CHAR_ITEM_PICKUP	*body	= (struct body_CG_CHAR_ITEM_PICKUP *)(P_recv_q->data);
	

	if( D_GetClient_sec_active(fd) == 0 )
		return;
	
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHAR_ITEM_PICKUP) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_ITEM_PICKUP(%d) bad body_size(%d)\n",
			fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_CHAR_ITEM_PICKUP), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}

	//check char status
	if( D_GetClient_status(fd) >= _USER_DEAD )
		return;

	//check item_id
	if( (body->item_id < 1 ) || (body->item_id > MAX_DROP_ITEM ) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_ITEM_PICKUP bad item_id(%d)\n",
						fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->item_id);
		P_remove_client(fd);
		return;
	}

	//check item_slot
	if( (body->item_slot < EQUIP_SLOT_CNT) || (body->item_slot > ITEM_SLOT_CNT) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_ITEM_PICKUP bad item_slot(%d)\n",
						fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->item_slot);
		P_remove_client(fd);
		return;
	}
	
	//item pick up
	if( D_SetClient_item_pickup(fd, body->item_slot, body->item_id, &item_class, &acquire_money, P_char_buff) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_ITEM_PICKUP D_SetClient_item_pickup()\n",
									fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_remove_client(fd);
		return;
	}


	if( item_class )
	{
		result_code	= RESULT_SUCC;
		channel_link	= D_GetItem_channel_link(body->item_id);
		map		= D_GetItem_map(body->item_id);	
		block_x		= D_GetItem_block_x(body->item_id);
		item_id		= body->item_id;	

		//map block delete item
		D_SetMapBlock_delete_item(body->item_id);

		//item clear		
		D_SetItem_clear(body->item_id);
	
		//send GC_CHAR_ITEM_PICKUP_RESULT
		U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_ITEM_PICKUP_RESULT(%d) user_id(%s) char_name(%s)\n",
								fd, result_code, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_send_size = MK_GetPacket_GC_CHAR_ITEM_PICKUP_RESULT(P_send_buf, result_code, fd, body->item_id, body->item_slot, acquire_money);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);

		//send GC_ITEM_DISAPPEAR to 3block all
		U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_ITEM_DISAPPEAR item_id(%d) channel_link(%d) map(%d) block_x(%d)(%d)(%d)\n",
										body->item_id, channel_link, map, block_x-1, block_x, block_x+1);
		P_send_size	= MK_GetPacket_GC_ITEM_DISAPPEAR(P_send_buf, &(item_id), 0);
		U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_buf);
		U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
		U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);
	
	}
	else
	{		
		//send GC_CHAR_ITEM_PICKUP_RESULT
		U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_ITEM_PICKUP_RESULT(%d) user_id(%s) char_name(%s)\n",
							fd, RESULT_FAIL_EXTINCT_ITEM, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_send_size = MK_GetPacket_GC_CHAR_ITEM_PICKUP_RESULT(P_send_buf, RESULT_FAIL_EXTINCT_ITEM, fd, body->item_id, body->item_slot, 0);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
	}
}
	
			
static	void	P_proc_CG_CHAR_ITEM_DROP(const int fd)
{
	int					item_id, channel_link, map, block_x;
	struct	body_CG_CHAR_ITEM_DROP		*body	= (struct body_CG_CHAR_ITEM_DROP *)(P_recv_q->data);


	if( D_GetClient_sec_active(fd) == 0 )
		return;
	
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHAR_ITEM_DROP) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_ITEM_DROP(%d) bad body_size(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_CHAR_ITEM_DROP), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}

	//check char status
	if( D_GetClient_status(fd) >= _USER_DEAD_REGEN )
		return;

	//check item_id, item_slot
	if( (body->item_slot < EQUIP_SLOT_CNT) || (body->item_slot > ITEM_SLOT_CNT) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_ITEM_PICKUP bad item_slot(%d)\n",
							fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->item_slot);
		P_remove_client(fd);
		return;
	}
	
	//item drop		
	if( D_SetClient_item_drop(fd, body->item_slot, &item_id) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_ITEM_DROP D_SetClient_item_drop()\n",
									fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_remove_client(fd);
		return;
	}
	
	if( item_id )
	{
		channel_link	= D_GetItem_channel_link(item_id);
		map		= D_GetItem_map(item_id);	
		block_x		= D_GetItem_block_x(item_id);

		//map block insert item
		D_SetMapBlock_insert_item(item_id);

		//send GC_ITEM_APPEAR to 3block all
		U_Printf("[PROC]send_queue_in BLOCK_ALLCAST GC_ITEM_APPEAR item_id(%d) channel_link(%d) map(%d) block_x(%d)(%d)(%d)\n",
										item_id, channel_link, map, block_x-1, block_x, block_x+1);
		P_send_size	= MK_GetPacket_GC_ITEM_APPEAR(P_send_buf, &(item_id), 0);
		U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_buf);
		U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
		U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);
	}
}
				

static	void	P_proc_CG_CHAR_ITEM_ENCHANT(const int fd)
{
	struct	body_CG_CHAR_ITEM_ENCHANT	*body	= (struct body_CG_CHAR_ITEM_ENCHANT *)(P_recv_q->data);

	
	if( D_GetClient_sec_active(fd) == 0 )
		return;
	
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHAR_ITEM_ENCHANT) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_ITEM_ENCHANT(%d) bad body_size(%d)\n",
			fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_CHAR_ITEM_ENCHANT), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}

	//check char status
	if( D_GetClient_status(fd) >= _USER_DEAD )
		return;

	//check item_slot1, item_slot2
	if( (body->item_slot1 < EQUIP_SLOT_CNT) || (body->item_slot1 >= ITEM_SLOT_CNT) 
			|| (body->item_slot1 < EQUIP_SLOT_CNT) || (body->item_slot2 >= ITEM_SLOT_CNT) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_ITEM_ENCHANT bad item_slot1(%d) item_slot2(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->item_slot1, body->item_slot2);
		P_remove_client(fd);
		return;
	}

	//item enchant
	if( D_SetClient_item_enchant(fd, body->item_slot1, body->item_slot2) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_ITEM_ENCHANT D_SetClient_item_enchant()\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_remove_client(fd);
		return;
	}
	
	//send GC_CHAR_ITEM_ENCHANT_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_ITEM_ENCHANT_RESULT user_id(%s) char_name(%s)\n",
			fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
	P_send_size	= MK_GetPacket_GC_CHAR_ITEM_ENCHANT_RESULT(P_send_buf, RESULT_SUCC, fd, body->item_slot1, body->item_slot2);
	U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
}
				

static	void	P_proc_CG_CHAR_ITEM_USE(const int fd)
{
	int				result, item_type;
	struct	body_CG_CHAR_ITEM_USE	*body	= (struct body_CG_CHAR_ITEM_USE *)(P_recv_q->data);

	
	if( D_GetClient_sec_active(fd) == 0 )
		return;
	
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHAR_ITEM_USE) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_ITEM_USE(%d) bad body_size(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_CHAR_ITEM_USE), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}
		
	//check char status
	if( D_GetClient_status(fd) >= _USER_DEAD )
		return;

	//check item_slot
	if( (body->item_slot < EQUIP_SLOT_CNT) || (body->item_slot >= ITEM_SLOT_CNT) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_ITEM_USE bad item_slot(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->item_slot);
		P_remove_client(fd);
		return;
	}
		
	//item use
	if( D_SetClient_item_use(fd, body->item_slot, &result, &item_type) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_ITEM_USE D_SetClient_item_use()\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_remove_client(fd);
		return;
	}
	
	//send GC_CHAR_ITEM_USE_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_ITEM_USE_RESULT user_id(%s) char_name(%s)\n",
							fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
	P_send_size	= MK_GetPacket_GC_CHAR_ITEM_USE_RESULT(P_send_buf, fd, body->item_slot, item_type);
	U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);

	switch( result )
	{
		case	-2:
			break;
		case	-1:
			//send GC_CHAR_BUFF
			U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_BUFF(%d) user_id(%s) char_name(%s)\n",
								fd, _BUFF_POTION_RECOVER_HP, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
			P_send_size	= MK_GetPacket_GC_CHAR_BUFF(P_send_buf, fd, _BUFF_POTION_RECOVER_HP);
			U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
			//send GC_CHAR_BUFF
			U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_BUFF(%d) user_id(%s) char_name(%s)\n",
								fd, _BUFF_POTION_RECOVER_AP, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
			P_send_size	= MK_GetPacket_GC_CHAR_BUFF(P_send_buf, fd, _BUFF_POTION_RECOVER_AP);
			U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
			break;
		default:
			//send GC_CHAR_BUFF
			U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_BUFF(%d) user_id(%s) char_name(%s)\n",
								fd, result, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
			P_send_size	= MK_GetPacket_GC_CHAR_BUFF(P_send_buf, fd, result);
			U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
	}
}


static	void	P_proc_CG_CHAR_WARP(int fd)
{
	int				channel_link, map, block_x, dest_map, dest_x, dest_z, warp_num;
	struct	body_CG_CHAR_WARP	*body	= (struct body_CG_CHAR_WARP *)(P_recv_q->data);
	
	
	if( D_GetClient_sec_active(fd) == 0 )
		return;

	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHAR_WARP) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_WARP(%d) bad body_size(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_CHAR_WARP), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}

	//check char status
	if( D_GetClient_status(fd) >= _USER_DEAD )
		return;
	
	//check mob_id
	if( (body->mob_id < 1) || (body->mob_id > D_GetMax_mob()) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_WARP bad mob_id(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->mob_id);
		P_remove_client(fd);
		return;
	}
	
	//check item_slot
	if( body->item_slot >= ITEM_SLOT_CNT )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_WARP bad item_slot(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->item_slot);
		P_remove_client(fd);
		return;
	}
	
	channel_link	= D_GetClient_channel_link(fd);	
	
	//check mob status
	if( D_GetMob_status(channel_link, body->mob_id) >= _MOB_DEATH )
	{
		//send GC_CHAR_WARP_RESULT
		U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_WARP_RESULT result_code(%d) user_id(%s) char_name(%s)\n",
							fd, RESULT_FAIL_EXTINCT_MOB, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_send_size	= MK_GetPacket_GC_CHAR_WARP_RESULT(P_send_buf, RESULT_FAIL_EXTINCT_MOB, fd);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
		return;
	}

	//check warp
	if( D_Char_check_warp(fd, body->mob_id, body->item_slot, &dest_map, &dest_x, &dest_z, &warp_num) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) channel_link(%d) CG_CHAR_WARP D_Char_check_warp()\n", 
								fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link);
		P_remove_client(fd);
		return;
	}

	map	= D_GetClient_map(fd);	
	block_x	= D_GetClient_block_x(fd);	

	//map block delete char
	D_SetMapBlock_delete_char(fd);
	U_Send_queue_in(SEND_INTER_BLOCK_DELETE_CHAR, fd, 0, 0, 0, 0, NULL);
	
	//send GC_OTHER_CHAR_DISAPPEAR to 3block char
	U_Printf("[PROC]send_queue_in 3block BLOCK_ALLCAST GC_OTHER_CHAR_DISAPPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, block_x-1, block_x, block_x+1);
	P_send_size	= MK_GetPacket_GC_OTHER_CHAR_DISAPPEAR(P_send_buf, &fd, 0);	
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);

	//char warp
	D_SetClient_warp(fd, dest_map, dest_x, dest_z);

	//send GC_CHAR_WARP_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_WARP_RESULT result_code(%d) user_id(%s) char_name(%s)\n",
							fd, RESULT_SUCC, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
	P_send_size	= MK_GetPacket_GC_CHAR_WARP_RESULT(P_send_buf, RESULT_SUCC, fd);
	U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);

	map	= D_GetClient_map(fd);	
	block_x	= D_GetClient_block_x(fd);	
	
	//send 3block GC_XXXX_APPEAR to owner
	P_send_q_in_block_GC_XXXX_APPEAR(fd, channel_link, map, block_x);
	P_send_q_in_block_GC_XXXX_APPEAR(fd, channel_link, map, block_x-1);
	P_send_q_in_block_GC_XXXX_APPEAR(fd, channel_link, map, block_x+1);
						
	//send GC_OTHER_CHAR_APPEAR to 3block char
	U_Printf("[PROC]send_queue_in 3block BLOCK_ALLCAST GC_OTHER_CHAR_APPEAR user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, block_x-1, block_x, block_x+1);
	P_send_size	= MK_GetPacket_GC_OTHER_CHAR_APPEAR(P_send_buf, &fd, 0);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);
			
	//map block insert char
	D_SetMapBlock_insert_char(fd);
	U_Send_queue_in(SEND_INTER_BLOCK_INSERT_CHAR, fd, channel_link, map, block_x, 0, NULL);

	//send GM_CHAR_MAP_IN
	if( P_mobserv_fd[channel_link] )
	{
		U_Printf("[PROC]send_queue_in UNICAST(%d) channel_link(%d) GM_CHAR_MAP_CHANGE fd(%d) user_id(%s) char_name(%s)\n",
						P_mobserv_fd[channel_link], channel_link, fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_send_size	= MK_GetPacket_GM_CHAR_MAP_CHANGE(P_send_buf, fd);
		U_Send_queue_in(SEND_UNICAST, P_mobserv_fd[channel_link], 0, 0, 0, P_send_size, P_send_buf);
	}
	
	U_Printf("[PROC]warp fd(%d) user_id(%s) char_name(%s) map(%d) x(%d) z(%d) warp_num(%d)\n",
							fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), map, dest_x, dest_z, warp_num);
}
		
		
static	void	P_proc_CG_CHAR_STORE_ITEM_BUY(const int fd)
{
	int					result_code;
	struct	body_CG_CHAR_STORE_ITEM_BUY	*body	= (struct body_CG_CHAR_STORE_ITEM_BUY *)(P_recv_q->data);

		
	if( D_GetClient_sec_active(fd) == 0 )
		return;

	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHAR_STORE_ITEM_BUY) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_STORE_ITEM_BUY(%d) bad body_size(%d)\n",
			fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_CHAR_STORE_ITEM_BUY), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}

	//check char status
	if( D_GetClient_status(fd) >= _USER_DEAD )
		return;
	
	//check mob_id
	if( (body->mob_id < 1) || (body->mob_id > D_GetMax_mob()) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_STORE_ITEM_BUY bad mob_id(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->mob_id);
		P_remove_client(fd);
		return;
	}
	
	//check store_item_slot
	if( body->store_item_slot >= STORE_ITEM_SLOT_CNT )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_STORE_ITEM_BUY bad store_item_slot(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->store_item_slot);
		P_remove_client(fd);
		return;
	}

	//check char_item_slot
	if( (body->char_item_slot < EQUIP_SLOT_CNT) || (body->char_item_slot >= ITEM_SLOT_CNT) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_STORE_ITEM_BUY bad char_item_slot(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->char_item_slot);
		P_remove_client(fd);
		return;
	}	
	
	//check mob status
	if( D_GetMob_status(D_GetClient_channel_link(fd), body->mob_id) >= _MOB_DEATH )
	{
		result_code	= RESULT_FAIL_EXTINCT_MOB;
	}
	else
	{
		//item buy	
		if( D_Char_buy_store_item(fd, body->mob_id, body->store_item_slot, body->item_cnt, body->char_item_slot, P_char_buff) )
		{
			U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) channel_link(%d) CG_CHAR_STORE_ITEM_BUY D_Char_buy_store_item()\n",
							fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), D_GetClient_channel_link(fd));
			P_remove_client(fd);
			return;
		}

		result_code	= RESULT_SUCC;
	}

	//send GC_CHAR_STORE_ITEM_BUY_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_STORE_ITEM_BUY_RESULT result_code(%d) user_id(%s) char_name(%s)\n",
								fd, result_code, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
	P_send_size	= MK_GetPacket_GC_CHAR_STORE_ITEM_BUY_RESULT(P_send_buf, result_code, fd, body->char_item_slot);
	U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
}

				
static	void	P_proc_CG_CHAR_STORE_ITEM_SELL(const int fd)
{
	int					result_code;
	struct	body_CG_CHAR_STORE_ITEM_SELL	*body	= (struct body_CG_CHAR_STORE_ITEM_SELL *)(P_recv_q->data);


	if( D_GetClient_sec_active(fd) == 0 )
		return;

	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHAR_STORE_ITEM_SELL) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_STORE_ITEM_SELL(%d) bad body_size(%d)\n",
			fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_CHAR_STORE_ITEM_SELL), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}

	//check char status
	if( D_GetClient_status(fd) >= _USER_DEAD )
		return;
	
	//check mob_id
	if( (body->mob_id < 1) || (body->mob_id > D_GetMax_mob()) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_STORE_ITEM_SELL bad mob_id(%d)\n",
						fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->mob_id);
		P_remove_client(fd);
		return;
	}
	
	//check item_slot
	if( (body->item_slot < EQUIP_SLOT_CNT) || (body->item_slot >= ITEM_SLOT_CNT) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_STORE_ITEM_SELL bad item_slot(%d)\n",
							fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->item_slot);
		P_remove_client(fd);
		return;
	}	
	
	//check mob status
	if( D_GetMob_status(D_GetClient_channel_link(fd), body->mob_id) >= _USER_DEAD )
	{
		result_code	= RESULT_FAIL_EXTINCT_MOB;
	}
	else
	{
		//item sell
		if( D_Char_sell_store_item(fd, body->mob_id, body->item_slot, body->item_cnt) )
		{
			U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) channel_link(%d) CG_CHAR_STORE_ITEM_SELL D_Char_sell_store_item()\n",
							fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), D_GetClient_channel_link(fd));
			P_remove_client(fd);
			return;
		}
		
		result_code	= RESULT_SUCC;
	}
	
	//send GC_CHAR_STORE_ITEM_SELL_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_STORE_ITEM_SELL_RESULT result_code(%d) user_id(%s) char_name(%s)\n",
								fd, result_code, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
	P_send_size	= MK_GetPacket_GC_CHAR_STORE_ITEM_SELL_RESULT(P_send_buf, result_code, fd, body->item_slot);
	U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
}


static	void	P_proc_CG_CHAR_SKILL_READY(const int fd)
{
	struct	body_CG_CHAR_SKILL_READY	*body	= (struct body_CG_CHAR_SKILL_READY *)(P_recv_q->data);

	
	if( D_GetClient_sec_active(fd) == 0 )
		return;
	
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHAR_SKILL_READY) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_SKILL_READY(%d) bad body_size(%d)\n",
			fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_CHAR_SKILL_READY), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}

	//check current status
	if( D_GetClient_status(fd) >= _USER_DEAD )
		return;

	//skill ready	
	if( P_char_skill_ready(fd, body->skill_type, body->dir, body->speed, body->x, body->y, body->z) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_SKILL_READY P_char_skill_ready()\n", 
					fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_remove_client(fd);
		return;
	}
}


static	void	P_proc_CG_CHAR_SKILL_ACTION(const int fd)
{
	struct	body1_CG_CHAR_SKILL_ACTION	*body1			= (struct body1_CG_CHAR_SKILL_ACTION *)(P_recv_q->data);
	

	if( D_GetClient_sec_active(fd) == 0 )
		return;
	
	//check body1_size
	if( P_recv_q->data_size < sizeof(struct body1_CG_CHAR_SKILL_ACTION) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_SKILL_ACTION(%d) bad body_size(%d)\n",
			fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body1_CG_CHAR_SKILL_ACTION), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}
	
	//check target_cnt
	if( (body1->target_mob_cnt + body1->target_char_cnt) > MAX_SKILL_TARGET_CNT )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_SKILL_ACTION bad target_mob_cnt(%d) target_char_cnt(%d)\n",
					fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body1->target_mob_cnt, body1->target_char_cnt);
		P_remove_client(fd);
		return;
	}
	
	//check body2_size
	if( P_recv_q->data_size
	!= (sizeof(struct body1_CG_CHAR_SKILL_ACTION)+sizeof(struct body2_CG_CHAR_SKILL_ACTION)*(body1->target_mob_cnt+body1->target_char_cnt)) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_SKILL_ACTION(%d) bad body_size(%d)\n",
		fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd),
		sizeof(struct body1_CG_CHAR_SKILL_ACTION)+sizeof(struct body2_CG_CHAR_SKILL_ACTION)*(body1->target_mob_cnt+body1->target_char_cnt),
		P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}
	
	//check current status
	if( D_GetClient_status(fd) >= _USER_DEAD )
		return;
	
	//skill action	
	if( P_char_skill_action(fd, body1, body1->dir, body1->speed, body1->x, body1->y, body1->z) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_SKILL_ACTION P_char_skill_action()\n", 
									fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_remove_client(fd);
		return;
	}
}

				
static	void	P_proc_CG_CHAR_SKILL_CANCEL(const int fd)
{
	int	channel_link, map, block_x;
	
	
	if( D_GetClient_sec_active(fd) == 0 )
		return;
	
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHAR_SKILL_CANCEL) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_SKILL_CANCEL(%d) bad body_size(%d)\n",
			fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_CHAR_SKILL_CANCEL), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}

	//check current status
	if( D_GetClient_status(fd) >= _USER_DEAD )
		return;

	//skill cancel	
	D_SetClient_skill_cancel(fd);
	
	channel_link	= D_GetClient_channel_link(fd);
	map		= D_GetClient_map(fd);	
	block_x		= D_GetClient_block_x(fd);	

	//send GC_CHAR_SKILL_CANCEL to 3block other
	U_Printf("[PROC]send_queue_in 3block BLOCK_OTHERCAST GC_CHAR_SKILL_CANCEL user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, block_x-1, block_x, block_x+1);
	P_send_size	= MK_GetPacket_GC_CHAR_SKILL_CANCEL(P_send_buf, fd);
	U_Send_queue_in(SEND_BLOCK_OTHERCAST, fd, channel_link, map, block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);
}


static	void	P_proc_CG_CHAR_SKILL_LEARN(const int fd)
{
	int					i;
	struct	body_CG_CHAR_SKILL_LEARN	*body	= (struct body1_CG_CHAR_SKILL_LEARN *)(P_recv_q->data);


	if( D_GetClient_sec_active(fd) == 0 )
		return;
	
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHAR_SKILL_LEARN) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_SKILL_LEARN(%d) bad body_size(%d)\n",
				fd, D_GetClient_user_id(fd), sizeof(struct body_CG_CHAR_SKILL_LEARN), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}
	
	//check skill_type
	if( (body->skill_type < 1) || (body->skill_type > CHAR_SKILL_CNT) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_SKILL_LEARN(%d) bad skill_type(%d)\n",
							fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->skill_type);
		P_remove_client(fd);
		return;
	}

	//check current status
	if( D_GetClient_status(fd) >= _USER_DEAD_REGEN )
		return;

	//skill learn	
	if( D_SetClient_skill_learn(fd, body->skill_type, P_char_buff) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_SKILL_LEARN D_SetClient_skill_learn()\n",
									fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_remove_client(fd);
		return;
	}

	//send GC_CHAR_BUFF
	for( i=0; i<PASSIVE_BUFF_CNT+EQUIP_PASSIVE_BUFF_CNT; i++ )
	{
		if( P_char_buff[i] )
		{
			U_Printf("[PROC]send_queue_in UNICAST(%d) GC_CHAR_BUFF(%d) user_id(%s) char_name(%s)\n",
								fd, i, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
			P_send_size	= MK_GetPacket_GC_CHAR_BUFF(P_send_buf, fd, i);
			U_Send_queue_in(SEND_UNICAST, fd, 0, 0, 0, P_send_size, P_send_buf);
		}
	}
}


static	void	P_proc_CG_CHAR_TRANSFORM(const int fd)
{
	int				channel_link, map, block_x;
	struct	body_CG_CHAR_TRANSFORM	*body	= (struct body_CG_CHAR_TRANSFORM *)(P_recv_q->data);


	if( D_GetClient_sec_active(fd) == 0 )
		return;

	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CG_CHAR_TRANSFORM) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_TRANSFORM(%d) bad body_size(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), sizeof(struct body_CG_CHAR_TRANSFORM), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}

	//check char status
	if( D_GetClient_status(fd) >= _USER_DEAD )
		return;

	//check client transform
	///if( D_GetClient_transform(fd) == body->transform_type )
	//	return;	

	if( body->transform_type > MAX_TRANSFORM_TYPE )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_TRANSFORM bad transform_type(%d)\n",
						fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), body->transform_type);
		P_remove_client(fd);
		return;
	}
	
	//transfrom 
	if( D_SetClient_transform(fd, body->transform_type) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_CHAR_TRANSFORM D_SetClient_transform()\n",
								fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd));
		P_remove_client(fd);
		return;
	}
	
	channel_link	= D_GetClient_channel_link(fd);
	map		= D_GetClient_map(fd);	
	block_x		= D_GetClient_block_x(fd);	

	//send GC_CHAR_TRANSFORM to 3block other
	U_Printf("[PROC]send_queue_in 3block BLOCK_OTHERCAST GC_CHAR_TRANSFORM user_id(%s) char_name(%s) channel_link(%d) map(%d) block_x(%d)(%d)(%d)\n", D_GetClient_user_id(fd), D_GetClient_char_name(fd), channel_link, map, block_x-1, block_x, block_x+1);
	P_send_size	= MK_GetPacket_GC_CHAR_TRANSFORM(P_send_buf, fd, body->transform_type);
	U_Send_queue_in(SEND_BLOCK_OTHERCAST, fd, channel_link, map, block_x, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x-1, P_send_size, P_send_buf);
	U_Send_queue_in(SEND_BLOCK_ALLCAST, 0, channel_link, map, block_x+1, P_send_size, P_send_buf);
}


static	void	P_proc_CG_MANAGER_CHAT(const int fd)
{
	if( D_GetClient_sec_active(fd) == 0 )
		return;
	
	//check body_size
	if( P_recv_q->data_size < 2 )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_MANAGER_CHAT bad body_size(%d)\n",
						fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}
	
	//check char status
	if( D_GetClient_status(fd) >= _USER_DEAD_REGEN )
		return;

	//check msg
	if( (P_recv_q->data[0] == 0) || (P_recv_q->data[P_recv_q->data_size-1]) )
	{
		P_recv_q->data[P_recv_q->data_size-1]	= 0;
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_MANAGER_CHAT bad msg(%s)\n",
						fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), P_recv_q->data);
		P_remove_client(fd);
		return;
	}	

	//check char_name
	if( strncmp(D_GetClient_char_name(fd), "±ôÂïÀÌÅ×µð", CHAR_NAME_LEN+1) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) char_name(%s) CG_MANAGER_CHAT bad char_name(%s)\n",
						fd, D_GetClient_user_id(fd), D_GetClient_char_name(fd), D_GetClient_char_name(fd));
		P_remove_client(fd);
		return;
	}

	//send GC_MANAGER_CHAT
	U_Printf("[PROC]send_queue_in BROADCAST GC_MANAGER_CHAT msg(%s)\n", P_recv_q->data);
	P_send_size	= MK_GetPacket_GC_MANAGER_CHAT(P_send_buf,  P_recv_q->data_size, P_recv_q->data);
	U_Send_queue_in(SEND_BROADCAST, 0, 0, 0, 0, P_send_size, P_send_buf);
}
