/*____________________________________________________________

	Project name : Qring Online
	File name    : mk_packet.c
	Description  : make packet function
	Date         : February 14 2005 
_____________________________________________________________*/

#include	"global.h"
#include	"util.h"
#include	"db.h"
#include	"crypt.h"
#define	_MK_PACKET_H_
#include	"mk_packet.h"

/*
//____________________________________________________________
//WB
int	MK_GetPacket_WB_REGSERVER(char *send_buf)
{
	struct	header_billing		*header	= (struct header_billing *)send_buf;
	struct	body_WB_REGSERVER	*body	= (struct body_WB_REGSERVER *)(send_buf + SIZEOF_HEADER_BILLING);


	header->total_size	= SIZEOF_HEADER_BILLING + sizeof(struct body_WB_REGSERVER);
	header->command		= WB_REGSERVER;
	
	body->server_idx	= G_Server_code;
	strncpy(body->game_code, BILLING_GAME_CODE, 3);

	C_Billing_encrypt(send_buf+4, send_buf+4, header->total_size-4);
	return	(header->total_size);
}


int	MK_GetPacket_WB_GAMESTART(char *send_buf, const int fd, char *user_id)
{
	struct	header_billing		*header	= (struct header_billing *)send_buf;
	struct	body_WB_GAMESTART	*body	= (struct body_WB_GAMESTART *)(send_buf + SIZEOF_HEADER_BILLING);


	header->total_size	= SIZEOF_HEADER_BILLING + sizeof(struct body_WB_GAMESTART);
	header->command		= WB_GAMESTART;

	body->id		= fd;
	strncpy(body->user_id, user_id, USER_ID_LEN+1);
	strncpy(body->game_code, BILLING_GAME_CODE, 3);
	strncpy(body->user_ip, D_GetClient_ip(fd), SIZEOF_IP+1);
	
	C_Billing_encrypt(send_buf+4, send_buf+4, header->total_size-4);
	return	(header->total_size);
}

		
int	MK_GetPacket_WB_GAMESTOP(char *send_buf, const int fd, char *user_id, char *ip)
{
	struct	header_billing		*header	= (struct header_billing *)send_buf;
	struct	body_WB_GAMESTOP	*body	= (struct body_WB_GAMESTOP *)(send_buf + SIZEOF_HEADER_BILLING);


	header->total_size	= SIZEOF_HEADER_BILLING + sizeof(struct body_WB_GAMESTOP);
	header->command		= WB_GAMESTOP;

	body->id		= fd;
	strncpy(body->user_id, user_id, USER_ID_LEN+1);
	strncpy(body->game_code, BILLING_GAME_CODE, 3);
	strncpy(body->user_ip, ip, SIZEOF_IP+1);
	
	C_Billing_encrypt(send_buf+4, send_buf+4, header->total_size-4);
	return	(header->total_size);
}


int	MK_GetPacket_WB_USER_CHECK_SUCC(char *send_buf, const int fd, char *user_id)
{
	struct	header_billing		*header	= (struct header_billing *)send_buf;
	struct	body_WB_USER_CHECK_SUCC	*body	= (struct body_WB_USER_CHECK_SUCC *)(send_buf + SIZEOF_HEADER_BILLING);


	header->total_size	= SIZEOF_HEADER_BILLING + sizeof(struct body_WB_USER_CHECK_SUCC);
	header->command		= WB_USER_CHECK_SUCC;

	body->id		= fd;
	strncpy(body->user_id, user_id, USER_ID_LEN+1);
	
	C_Billing_encrypt(send_buf+4, send_buf+4, header->total_size-4);
	return	(header->total_size);
}


int	MK_GetPacket_WB_USER_CHECK_FAIL(char *send_buf, const int fd, char *user_id)
{
	struct	header_billing		*header	= (struct header_billing *)send_buf;
	struct	body_WB_USER_CHECK_FAIL	*body	= (struct body_WB_USER_CHECK_FAIL *)(send_buf + SIZEOF_HEADER_BILLING);


	header->total_size	= SIZEOF_HEADER_BILLING + sizeof(struct body_WB_USER_CHECK_FAIL);
	header->command		= WB_USER_CHECK_FAIL;

	body->id		= fd;
	strncpy(body->user_id, user_id, USER_ID_LEN+1);
	
	C_Billing_encrypt(send_buf+4, send_buf+4, header->total_size-4);
	return	(header->total_size);
}
*/


//____________________________________________________________
//TW
int	MK_GetPacket_TW_WAKEUP(char *send_buf)
{
	struct	header_normal	*header	= (struct header_normal *)send_buf;


	header->body_size	= sizeof(struct body_TW_WAKEUP);
	header->command		= TW_WAKEUP;
	
	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}



//____________________________________________________________
//WS
int	MK_GetPacket_WS_INIT(char *send_buf)
{
	struct	header_special	*header	= (struct header_special *)send_buf;
	struct	body_WS_INIT	*body	= (struct body_WS_INIT *)(send_buf + SIZEOF_HEADER_SPECIAL);


	header->body_size	= sizeof(struct body_WS_INIT);
	header->command		= WS_INIT;
	
	strncpy(body->version, VERSION, VERSION_LEN+1);	

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}


int	MK_GetPacket_WS_MYINFO(char *send_buf)
{
	struct	header_special	*header	= (struct header_special *)send_buf;


	header->body_size	= sizeof(struct body_WS_MYINFO);
	header->command		= WS_MYINFO;

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}

	
int	 MK_GetPacket_WS_WORLD_INFO(char *send_buf, const int game)
{
	struct	header_special		*header	= (struct header_special *)send_buf;
	struct	body_WS_WORLD_INFO	*body	= (struct body_WS_WORLD_INFO *)(send_buf + SIZEOF_HEADER_SPECIAL);


	header->body_size	= sizeof(struct body_WS_WORLD_INFO);
	header->command		= WS_WORLD_INFO;

	body->game		= game;

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);

}


int	 MK_GetPacket_WS_GAMESERV_INFO(char *send_buf, const int game, const int world)
{
	struct	header_special		*header	= (struct header_special *)send_buf;
	struct	body_WS_GAMESERV_INFO	*body	= (struct body_WS_GAMESERV_INFO *)(send_buf + SIZEOF_HEADER_SPECIAL);


	header->body_size	= sizeof(struct body_WS_GAMESERV_INFO);
	header->command		= WS_GAMESERV_INFO;

	body->game		= game;
	body->world		= world;

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);

}


int	 MK_GetPacket_WS_DBAGENT_INFO(char *send_buf, const int game, const int world)
{
	struct	header_special		*header	= (struct header_special *)send_buf;
	struct	body_WS_DBAGENT_INFO	*body	= (struct body_WS_DBAGENT_INFO *)(send_buf + SIZEOF_HEADER_SPECIAL);


	header->body_size	= sizeof(struct body_WS_DBAGENT_INFO);
	header->command		= WS_DBAGENT_INFO;

	body->game		= game;
	body->world		= world;

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);

}


int	 MK_GetPacket_WS_MOBSERV_INFO(char *send_buf, const int game, const int world)
{
	struct	header_special		*header	= (struct header_special *)send_buf;
	struct	body_WS_MOBSERV_INFO	*body	= (struct body_WS_MOBSERV_INFO *)(send_buf + SIZEOF_HEADER_SPECIAL);


	header->body_size	= sizeof(struct body_WS_MOBSERV_INFO);
	header->command		= WS_MOBSERV_INFO;

	body->game		= game;
	body->world		= world;

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}


int	MK_GetPacket_WS_MAP_FILES(char *send_buf)
{
	struct	header_special		*header	= (struct header_special *)send_buf;


	header->body_size	= sizeof(struct body_WS_MAP_FILES);
	header->command		= WS_MAP_FILES;

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}


int	MK_GetPacket_WS_MOB_FILES(char *send_buf)
{
	struct	header_special		*header	= (struct header_special *)send_buf;


	header->body_size	= sizeof(struct body_WS_MOB_FILES);
	header->command		= WS_MOB_FILES;

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}


int	MK_GetPacket_WS_TYPE_MOB_FILES(char *send_buf)
{
	struct	header_special		*header	= (struct header_special *)send_buf;


	header->body_size	= sizeof(struct body_WS_TYPE_MOB_FILES);
	header->command		= WS_TYPE_MOB_FILES;

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}


int	MK_GetPacket_WS_LEVEL_FILES(char *send_buf)
{
	struct	header_special		*header	= (struct header_special *)send_buf;


	header->body_size	= sizeof(struct body_WS_LEVEL_FILES);
	header->command		= WS_LEVEL_FILES;

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}


int	MK_GetPacket_WS_TYPE_ITEM_FILES(char *send_buf)
{
	struct	header_special		*header	= (struct header_special *)send_buf;


	header->body_size	= sizeof(struct body_WS_TYPE_ITEM_FILES);
	header->command		= WS_TYPE_ITEM_FILES;

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}


int	MK_GetPacket_WS_TYPE_DROP_FILES(char *send_buf)
{
	struct	header_special		*header	= (struct header_special *)send_buf;


	header->body_size	= sizeof(struct body_WS_TYPE_DROP_FILES);
	header->command		= WS_TYPE_DROP_FILES;

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}


int	MK_GetPacket_WS_TYPE_WARP_FILES(char *send_buf)
{
	struct	header_special		*header	= (struct header_special *)send_buf;


	header->body_size	= sizeof(struct body_WS_TYPE_WARP_FILES);
	header->command		= WS_TYPE_WARP_FILES;

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}


int	MK_GetPacket_WS_TYPE_STORE_FILES(char *send_buf)
{
	struct	header_special		*header	= (struct header_special *)send_buf;


	header->body_size	= sizeof(struct body_WS_TYPE_STORE_FILES);
	header->command		= WS_TYPE_STORE_FILES;

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}


int	MK_GetPacket_WS_TYPE_SKILL_FILES(char *send_buf)
{
	struct	header_special		*header	= (struct header_special *)send_buf;


	header->body_size	= sizeof(struct body_WS_TYPE_SKILL_FILES);
	header->command		= WS_TYPE_SKILL_FILES;

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}


int	MK_GetPacket_WS_TYPE_MOB_SKILL_FILES(char *send_buf)
{
	struct	header_special		*header	= (struct header_special *)send_buf;


	header->body_size	= sizeof(struct body_WS_TYPE_MOB_SKILL_FILES);
	header->command		= WS_TYPE_MOB_SKILL_FILES;

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}



//_________________________________________________________________________
//WA
int	MK_GetPacket_WA_INIT(char *send_buf, const int record_lock)
{
	struct	header_special	*header	= (struct header_special *)send_buf;
	struct	body_WA_INIT	*body	= (struct body_WA_INIT *)(send_buf + SIZEOF_HEADER_SPECIAL);


	header->body_size	= sizeof(struct body_WA_INIT);
	header->command		= WA_INIT;
	
	strncpy(body->version, VERSION, VERSION_LEN+1);
	body->record_lock	= record_lock;

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}


int	MK_GetPacket_WA_LIVE(char *send_buf)
{
	struct	header_special	*header	= (struct header_special *)send_buf;
	

	header->body_size	= sizeof(struct body_WA_LIVE);
	header->command		= WA_LIVE;

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}


int	MK_GetPacket_WA_CHAR_SAVE(char *send_buf, const int fd, const char *aw_char_load_result_data, const int aw_char_load_result_data_len)
{
	struct	header_special		*header	= (struct header_special *)send_buf;
	struct	body1_WA_CHAR_SAVE	*body1	= (struct body1_WA_CHAR_SAVE *)(send_buf + SIZEOF_HEADER_SPECIAL);
	struct	body2_WA_CHAR_SAVE	*body2
					= (struct body2_WA_CHAR_SAVE *)(send_buf + SIZEOF_HEADER_SPECIAL + sizeof(struct body1_WA_CHAR_SAVE));


	header->command		= WA_CHAR_SAVE;

	if( aw_char_load_result_data )
	{
		memcpy(body1, aw_char_load_result_data + SIZEOF_INT, aw_char_load_result_data_len - SIZEOF_INT);
		header->body_size	= aw_char_load_result_data_len - SIZEOF_INT;
	}
	else
	{	
		D_GetClient_save_data(body1, body2, fd);	
		header->body_size	= sizeof(struct body1_WA_CHAR_SAVE) + sizeof(struct body2_WA_CHAR_SAVE)*(body1->item_slot_cnt);
	}

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}


int	MK_GetPacket_WA_CHAR_LOAD(char *send_buf, const int fd, const char *char_name)
{
	struct	header_special		*header	= (struct header_special *)send_buf;
	struct	body_WA_CHAR_LOAD	*body	= (struct body_WA_CHAR_LOAD *)(send_buf + SIZEOF_HEADER_SPECIAL);


	header->body_size	= sizeof(struct body_WA_CHAR_LOAD);
	header->fd		= fd;
	header->command		= WA_CHAR_LOAD;
	header->sec_active	= D_GetClient_sec_active(fd);
	header->session		= D_GetClient_session(fd);
	
	strncpy(body->user_id, D_GetClient_user_id(fd), USER_ID_LEN+1);
	strncpy(body->char_name, char_name, CHAR_NAME_LEN+1);
	
	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}


int	MK_GetPacket_WA_LOGIN(char *send_buf, const int fd, const char *user_id, const char *encodedstr)
{
	struct	header_special		*header	= (struct header_special *)send_buf;
	struct	body_WA_LOGIN		*body	= (struct body_WA_LOGIN *)(send_buf + SIZEOF_HEADER_SPECIAL);
		

	header->body_size	= sizeof(struct body_WA_LOGIN);
	header->fd		= fd;
	header->command		= WA_LOGIN;
	header->sec_active	= D_GetClient_sec_active(fd);
	header->session		= D_GetClient_session(fd);
	
	strncpy(body->user_id, user_id, USER_ID_LEN+1);
	strncpy(body->encodedstr, encodedstr, ENCODEDSTR_LEN+1);
	body->record_lock	= G_Server_code;
	
	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}
	
		
int	MK_GetPacket_WA_LOGOUT(char *send_buf, const int fd, const int sec_active, const int session, const char *user_id)
{
	struct	header_special		*header	= (struct header_special *)send_buf;
	struct	body_WA_LOGOUT		*body	= (struct body_WA_LOGOUT *)(send_buf + SIZEOF_HEADER_SPECIAL);


	header->body_size	= sizeof(struct body_WA_LOGOUT);
	header->fd		= fd;
	header->command		= WA_LOGOUT;
	header->sec_active	= sec_active;
	header->session		= session;
	
	strncpy(body->user_id, user_id, USER_ID_LEN+1);
	body->record_lock	= G_Server_code;
	
	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}

int	MK_GetPacket_WA_CURRENT_USER(char *send_buf, const int channel_link)
{
	struct	header_special		*header	= (struct header_special *)send_buf;
	struct	body_WA_CURRENT_USER	*body	= (struct body_WA_CURRENT_USER *)(send_buf + SIZEOF_HEADER_SPECIAL);


	header->body_size	= sizeof(struct body_WA_CURRENT_USER);
	header->command		= WA_CURRENT_USER;
	
	body->game_code		= PARAN_GAME_CODE;
	body->msvr_code		= G_World;
	body->ssvr_code		= channel_link + (CHANNEL_PER_MACHINE*(G_Machine-1));
	strncpy(body->channel_ip, D_GetWorld_machine_outer_ip(G_World, G_Machine), SIZEOF_IP+1);	
	body->channel_port	= GAME_PORT;
	snprintf(body->channel_name, CHANNEL_NAME_LEN+1, "%s-%d", D_GetWorld_name(G_World), channel_link+(CHANNEL_PER_MACHINE*(G_Machine-1)));
	body->channel_name[CHANNEL_NAME_LEN]	= 0;
	body->user_cnt		= D_GetWorld_channel_user(G_World, channel_link+(CHANNEL_PER_MACHINE*(G_Machine-1)));

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}

int	MK_GetPacket_WA_GAME_LOG(char *send_buf, const int fd, const int channel_link, const int log_type, const int game_time)
{
	struct	header_special		*header	= (struct header_special *)send_buf;
	struct	body_WA_GAME_LOG	*body	= (struct body_WA_GAME_LOG *)(send_buf + SIZEOF_HEADER_SPECIAL);


	header->body_size	= sizeof(struct body_WA_GAME_LOG);
	header->command		= WA_GAME_LOG;
	
	body->game_code		= PARAN_GAME_CODE;
	strncpy(body->user_id, D_GetClient_user_id(fd), USER_ID_LEN+1);
	strncpy(body->char_name, D_GetClient_char_name(fd), CHAR_NAME_LEN+1);
	snprintf(body->channel_name, CHANNEL_NAME_LEN+1, "%s-%d", D_GetWorld_name(G_World), channel_link+(CHANNEL_PER_MACHINE*(G_Machine-1)));
	body->channel_name[CHANNEL_NAME_LEN]	= 0;
	strncpy(body->user_ip, D_GetClient_ip(fd), SIZEOF_IP+1);
	body->log_type		= log_type;
	body->game_time		= game_time;
	body->level		= D_GetClient_level(fd);
	body->exp		= D_GetClient_exp(fd);
	
	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}


//_________________________________________________________________________
//GM
int	MK_GetPacket_GM_LIVE(char *send_buf)
{
	struct	header_normal	*header	= (struct header_normal *)send_buf;
	

	header->body_size	= sizeof(struct body_GM_LIVE);
	header->command		= GM_LIVE;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}


int	MK_GetPacket_GM_INIT_RESULT(char *send_buf, const int result_code)
{	
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body_GM_INIT_RESULT	*body	= (struct body_GM_INIT_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size	= sizeof(struct body_GM_INIT_RESULT);
	header->command		= GM_INIT_RESULT;

	body->result_code	= result_code;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}


int	MK_GetPacket_GM_CHAR_CHANNEL_IN(char *send_buf, int *link_ptr, const int link_follow)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body1_GM_CHAR_CHANNEL_IN	*body1	= (struct body1_GM_CHAR_CHANNEL_IN *)(send_buf + SIZEOF_HEADER_NORMAL);
	struct	body2_GM_CHAR_CHANNEL_IN	*body2	
				= (struct body2_GM_CHAR_CHANNEL_IN *)(send_buf + SIZEOF_HEADER_NORMAL + sizeof(struct body1_GM_CHAR_CHANNEL_IN));


	header->command		= GM_CHAR_CHANNEL_IN;
	body1->char_cnt		= 1;
	D_GetClient_channelin_data(body2, *link_ptr);
	header->body_size	= sizeof(struct body1_GM_CHAR_CHANNEL_IN) + sizeof(struct body2_GM_CHAR_CHANNEL_IN);

	if( link_follow )
	{
		while( (*link_ptr = D_GetClient_block_next(*link_ptr)) )
		{
			if( (header->body_size + sizeof(struct body2_GM_CHAR_CHANNEL_IN)) > SIZEOF_MAX_SERV_PACKET_BODY )
				break;
			
			body2			= (struct body2_GM_CHAR_CHANNEL_IN *)(send_buf + SIZEOF_HEADER_NORMAL + header->body_size);
			D_GetClient_channelin_data(body2, *link_ptr);
			header->body_size	+= sizeof(struct body2_GM_CHAR_CHANNEL_IN);
			++(body1->char_cnt);
		}
	}

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}


int	MK_GetPacket_GM_CHAR_CHANNEL_OUT(char *send_buf, int *link_ptr, const int link_follow)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body1_GM_CHAR_CHANNEL_OUT	*body1	= (struct body1_GM_CHAR_CHANNEL_OUT *)(send_buf + SIZEOF_HEADER_NORMAL);
	struct	body2_GM_CHAR_CHANNEL_OUT	*body2
			= (struct body2_GM_CHAR_CHANNEL_OUT *)(send_buf + SIZEOF_HEADER_NORMAL + sizeof(struct body1_GM_CHAR_CHANNEL_OUT));


	header->command		= GM_CHAR_CHANNEL_OUT;
	body1->char_cnt		= 1;
	body2->char_id		= *link_ptr;
	header->body_size	= sizeof(struct body1_GM_CHAR_CHANNEL_OUT) + sizeof(struct body2_GM_CHAR_CHANNEL_OUT);

	if( link_follow )
	{
		while( (*link_ptr = D_GetClient_block_next(*link_ptr)) )
		{
			if( (header->body_size + sizeof(struct body2_GM_CHAR_CHANNEL_OUT)) > SIZEOF_MAX_SERV_PACKET_BODY )
				break;
			
			body2			= (struct body2_GM_CHAR_CHANNEL_OUT *)(send_buf + SIZEOF_HEADER_NORMAL + header->body_size);
			body2->char_id		= *link_ptr;
			header->body_size	+= sizeof(struct body2_GM_CHAR_CHANNEL_OUT);
			++(body1->char_cnt);
		}
	}

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}


int	MK_GetPacket_GM_CHAR_MAP_CHANGE(char *send_buf, const int fd)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body_GM_CHAR_MAP_CHANGE		*body	= (struct body_GM_CHAR_MAP_CHANGE *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size	= sizeof(struct body_GM_CHAR_MAP_CHANGE);
	header->command		= GM_CHAR_MAP_CHANGE;

	D_GetClient_mapchange_data(body, fd);

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}


int	MK_GetPacket_GM_CHAR_MOVE(char *send_buf, const int char_id, const int status, const float x, const float y, const float z)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body_GM_CHAR_MOVE	*body	= (struct body_GM_CHAR_MOVE *)(send_buf + SIZEOF_HEADER_NORMAL);

	
	header->body_size	= sizeof(struct body_GM_CHAR_MOVE);
	header->command		= GM_CHAR_MOVE;
	
	body->char_id	= char_id;
	body->status	= status;
	body->x		= x;
	body->y		= y;
	body->z		= z;		

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}


int	MK_GetPacket_GM_CHAR_ATTACK_MOB(char *send_buf, const int fd, const int mob_id,
			const int mob_hp, const int damage_type, const int damage, const int dir, const float x, const float y, const float z)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body_GM_CHAR_ATTACK_MOB	*body	= (struct body_GM_CHAR_ATTACK_MOB *)(send_buf + SIZEOF_HEADER_NORMAL);

	
	header->body_size	= sizeof(struct body_GM_CHAR_ATTACK_MOB);
	header->command		= GM_CHAR_ATTACK_MOB;
	
	body->char_id		= fd;
	body->mob_id		= mob_id;
	body->mob_hp		= mob_hp;
	body->damage_type	= damage_type;
	body->damage		= damage;
	body->dir		= dir;
	body->x			= x;
	body->y			= y;
	body->z			= z;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}


int	MK_GetPacket_GM_CHAR_SKILL_ACTION(char *send_buf)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body1_GM_CHAR_SKILL_ACTION	*body1	= (struct body1_GM_CHAR_SKILL_ACTION *)(send_buf + SIZEOF_HEADER_NORMAL);
	

	header->body_size	= sizeof(struct body1_GM_CHAR_SKILL_ACTION) + sizeof(struct body2_GM_CHAR_SKILL_ACTION)*(body1->target_mob_cnt);
	header->command		= GM_CHAR_SKILL_ACTION;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}


int	MK_GetPacket_GM_MOB_BUFF(char *send_buf, const int channel, const int mob_id, const int buff_series)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body_GM_MOB_BUFF	*body	= (struct body_GM_MOB_BUFF *)(send_buf + SIZEOF_HEADER_NORMAL);
	
	
	header->body_size	= sizeof(struct body_GM_MOB_BUFF);
	header->command		= GM_MOB_BUFF;

	body->mob_id		= mob_id;
	body->buff_series	= buff_series;
	body->buff_value	= D_GetMob_buff_value(channel, mob_id, buff_series);
	body->buff_sec		= D_GetMob_buff_end_sec(channel, mob_id, buff_series);

	if( body->buff_sec )
		body->buff_sec	-= G_Sec;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}


	
//_________________________________________________________________________
//GC
int	MK_GetPacket_GC_SERVER_TROUBLE(char *send_buf, const int trouble)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body_GC_SERVER_TROUBLE	*body	= (struct body_GC_SERVER_TROUBLE *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size	= sizeof(struct body_GC_SERVER_TROUBLE);
	header->command		= GC_SERVER_TROUBLE;

	body->trouble		= trouble;	

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}


int	MK_GetPacket_GC_LOGIN_RESULT(char *send_buf, const int result_code)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body_GC_LOGIN_RESULT	*body	= (struct body_GC_LOGIN_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size	= sizeof(struct body_GC_LOGIN_RESULT);
	header->command		= GC_LOGIN_RESULT;

	body->result_code	= result_code;	

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}


int	MK_GetPacket_GC_LIVE(char *send_buf)
{
	struct	header_normal	*header	= (struct header_normal *)send_buf;
	

	header->body_size	= sizeof(struct body_GC_LIVE);
	header->command		= GC_LIVE;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}


int	MK_GetPacket_GC_CHANNEL_LIST_RESULT(char *send_buf, const int result_code, const int world)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body1_GC_CHANNEL_LIST_RESULT	*body1	= (struct body1_GC_CHANNEL_LIST_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);
	struct	body2_GC_CHANNEL_LIST_RESULT	*body2
			= (struct body2_GC_CHANNEL_LIST_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL + sizeof(struct body1_GC_CHANNEL_LIST_RESULT));
	int					i;


	header->body_size
		= sizeof(struct body1_GC_CHANNEL_LIST_RESULT) + sizeof(struct body2_GC_CHANNEL_LIST_RESULT) * D_GetWorld_channel_cnt(world);
	header->command		= GC_CHANNEL_LIST_RESULT;

	body1->result_code	= result_code;	
	body1->channel_cnt	= D_GetWorld_channel_cnt(world);
		
	for( i=1; i<=D_GetWorld_channel_cnt(world); i++)
	{
		body2->user		= D_GetWorld_channel_user(world, i);	
		++body2; 
	}

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}

		
int	MK_GetPacket_GC_CHAR_CHANNEL_MOVE_RESULT(char *send_buf, const int result_code, const int fd, const int world, const int channel_idx)
{
	struct	header_normal				*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_CHANNEL_MOVE_RESULT	*body	= (struct body_GC_CHAR_CHANNEL_MOVE_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);
	

	header->body_size	= sizeof(struct body_GC_CHAR_CHANNEL_MOVE_RESULT);
	header->command		= GC_CHAR_CHANNEL_MOVE_RESULT;

	body->result_code	= result_code;
	strncpy(body->channel_ip, D_GetWorld_machine_outer_ip(world, (channel_idx-1)/CHANNEL_PER_MACHINE+1), SIZEOF_IP+1);	
	body->channel_port	= GAME_PORT;
	D_GetClient_channel_move_data(body, fd);
	
	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}

		
int	MK_GetPacket_GC_CHAR_LOAD_RESULT(char *send_buf, const int result_code, const int fd, const char *char_name)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body1_GC_CHAR_LOAD_RESULT	*body1	= (struct body1_GC_CHAR_LOAD_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);
	struct	body2_GC_CHAR_LOAD_RESULT	*body2
				= (struct body2_GC_CHAR_LOAD_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL + sizeof(struct body1_GC_CHAR_LOAD_RESULT));


	header->command		= GC_CHAR_LOAD_RESULT;
	body1->result_code	= result_code;

	if( result_code )
	{
		strncpy(body1->char_name, char_name, CHAR_NAME_LEN+1);
		header->body_size	= sizeof(struct body1_GC_CHAR_LOAD_RESULT);
	}
	else
	{
		D_GetClient_owner_data(body1, body2, fd);
		header->body_size	= sizeof(struct body1_GC_CHAR_LOAD_RESULT) + sizeof(struct body2_GC_CHAR_LOAD_RESULT)*(body1->item_slot_cnt);
	}
	
	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}
		

int	MK_GetPacket_GC_OTHER_CHAR_APPEAR(char *send_buf, int *link_ptr, const int link_follow)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body1_GC_OTHER_CHAR_APPEAR	*body1	= (struct body1_GC_OTHER_CHAR_APPEAR *)(send_buf + SIZEOF_HEADER_NORMAL);
	struct	body2_GC_OTHER_CHAR_APPEAR	*body2	
				= (struct body2_GC_OTHER_CHAR_APPEAR *)(send_buf + SIZEOF_HEADER_NORMAL + sizeof(struct body1_GC_OTHER_CHAR_APPEAR));


	header->command		= GC_OTHER_CHAR_APPEAR;
	body1->other_char_cnt	= 1;
	D_GetClient_appear_data(body2, *link_ptr);
	header->body_size	= sizeof(struct body1_GC_OTHER_CHAR_APPEAR) + sizeof(struct body2_GC_OTHER_CHAR_APPEAR);

	if( link_follow )
	{
		while( (*link_ptr = D_GetClient_block_next(*link_ptr)) )
		{
			if( (header->body_size + sizeof(struct body2_GC_OTHER_CHAR_APPEAR)) > SIZEOF_MAX_GAME_PACKET_BODY )
				break;
			
			body2			= (struct body2_GC_OTHER_CHAR_APPEAR *)(send_buf + SIZEOF_HEADER_NORMAL + header->body_size);
			D_GetClient_appear_data(body2, *link_ptr);
			header->body_size	+= sizeof(struct body2_GC_OTHER_CHAR_APPEAR);
			++(body1->other_char_cnt);
		}
	}

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}
		

int	MK_GetPacket_GC_OTHER_CHAR_DISAPPEAR(char *send_buf, int *link_ptr, const int link_follow)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body1_GC_OTHER_CHAR_DISAPPEAR	*body1	= (struct body1_GC_OTHER_CHAR_DISAPPEAR *)(send_buf + SIZEOF_HEADER_NORMAL);
	struct	body2_GC_OTHER_CHAR_DISAPPEAR	*body2
			= (struct body2_GC_OTHER_CHAR_DISAPPEAR *)(send_buf + SIZEOF_HEADER_NORMAL + sizeof(struct body1_GC_OTHER_CHAR_DISAPPEAR));


	header->command		= GC_OTHER_CHAR_DISAPPEAR;
	body1->other_char_cnt	= 1;
	body2->char_id		= *link_ptr;
	header->body_size	= sizeof(struct body1_GC_OTHER_CHAR_DISAPPEAR) + sizeof(struct body2_GC_OTHER_CHAR_DISAPPEAR);

	if( link_follow )
	{
		while( (*link_ptr = D_GetClient_block_next(*link_ptr)) )
		{
			if( (header->body_size + sizeof(struct body2_GC_OTHER_CHAR_DISAPPEAR)) > SIZEOF_MAX_GAME_PACKET_BODY )
				break;
			
			body2			= (struct body2_GC_OTHER_CHAR_DISAPPEAR *)(send_buf + SIZEOF_HEADER_NORMAL + header->body_size);
			body2->char_id		= *link_ptr;
			header->body_size	+= sizeof(struct body2_GC_OTHER_CHAR_DISAPPEAR);
			++(body1->other_char_cnt);
		}
	}

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}
			

int	MK_GetPacket_GC_CHAR_MOVE(char *send_buf, const int fd,
		const int dir, const int status, const float speed, const float x, const float y, const float z)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_MOVE	*body	= (struct body_GC_CHAR_MOVE *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size	= sizeof(struct body_GC_CHAR_MOVE);
	header->command		= GC_CHAR_MOVE;

	body->char_id	= fd;
	body->dir	= dir;
	body->status	= status;
	body->speed	= speed;
	body->x		= x;
	body->y		= y;
	body->z		= z;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}	


int	MK_GetPacket_GC_CHAR_CHAT(char *send_buf, const int fd, const int msg_size, const char *msg)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_CHAT	*body	= (struct body_GC_CHAR_CHAT *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size	= sizeof(struct body_GC_CHAR_CHAT) + msg_size;
	header->command		= GC_CHAR_CHAT;

	body->char_id	= fd;
	strncpy(send_buf + SIZEOF_HEADER_NORMAL + sizeof(struct body_GC_CHAR_CHAT), msg, msg_size);

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}


int	MK_GetPacket_GC_MOB_APPEAR(char *send_buf, const int channel, int *link_ptr, const int link_follow)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body1_GC_MOB_APPEAR	*body1	= (struct body1_GC_MOB_APPEAR *)(send_buf + SIZEOF_HEADER_NORMAL);
	struct	body2_GC_MOB_APPEAR	*body2	
				= (struct body2_GC_MOB_APPEAR *)(send_buf + SIZEOF_HEADER_NORMAL + sizeof(struct body1_GC_MOB_APPEAR));


	header->command		= GC_MOB_APPEAR;
	body1->mob_cnt	= 1;
	D_GetMob_appear_data(body2, channel, *link_ptr);
	header->body_size	= sizeof(struct body1_GC_MOB_APPEAR) + sizeof(struct body2_GC_MOB_APPEAR);

	if( link_follow )
	{
		while( (*link_ptr = D_GetMob_block_next(channel, *link_ptr)) )
		{
			if( (header->body_size + sizeof(struct body2_GC_MOB_APPEAR)) > SIZEOF_MAX_GAME_PACKET_BODY )
				break;
			
			body2			= (struct body2_GC_MOB_APPEAR *)(send_buf + SIZEOF_HEADER_NORMAL + header->body_size);
			D_GetMob_appear_data(body2, channel, *link_ptr);
			header->body_size	+= sizeof(struct body2_GC_MOB_APPEAR);
			++(body1->mob_cnt);
		}
	}

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}


int	MK_GetPacket_GC_MOB_DISAPPEAR(char *send_buf, const int channel, int *link_ptr, const int link_follow)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body1_GC_MOB_DISAPPEAR	*body1	= (struct body1_GC_MOB_DISAPPEAR *)(send_buf + SIZEOF_HEADER_NORMAL);
	struct	body2_GC_MOB_DISAPPEAR	*body2
			= (struct body2_GC_MOB_DISAPPEAR *)(send_buf + SIZEOF_HEADER_NORMAL + sizeof(struct body1_GC_MOB_DISAPPEAR));


	header->command		= GC_MOB_DISAPPEAR;
	body1->mob_cnt		= 1;
	body2->mob_id		= *link_ptr;
	header->body_size	= sizeof(struct body1_GC_MOB_DISAPPEAR) + sizeof(struct body2_GC_MOB_DISAPPEAR);

	if( link_follow )
	{
		while( (*link_ptr = D_GetMob_block_next(channel, *link_ptr)) )
		{
			if( (header->body_size + sizeof(struct body2_GC_MOB_DISAPPEAR)) > SIZEOF_MAX_GAME_PACKET_BODY )
				break;
			
			body2			= (struct body2_GC_MOB_DISAPPEAR *)(send_buf + SIZEOF_HEADER_NORMAL + header->body_size);
			body2->mob_id		= *link_ptr;
			header->body_size	+= sizeof(struct body2_GC_MOB_DISAPPEAR);
			++(body1->mob_cnt);
		}
	}

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}


int	MK_GetPacket_GC_MOB_MOVE(char *send_buf, const int mob_id,
		const int dir, const int status, const float speed, const float x, const float y, const float z)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body_GC_MOB_MOVE	*body	= (struct body_GC_MOB_MOVE *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size	= sizeof(struct body_GC_MOB_MOVE);
	header->command		= GC_MOB_MOVE;

	body->mob_id	= mob_id;
	body->dir	= dir;
	body->status	= status;
	body->speed	= speed;
	body->x		= x;
	body->y		= y;
	body->z		= z;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}	
	

int	MK_GetPacket_GC_CHAR_ATTACK_MOB(char *send_buf, const int fd, const int mob_id, const int attack_id, const int attack_type, const int mob_hp,
			const int damage_type, const int damage, const int dir, const float speed, const float x, const float y, const float z)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_ATTACK_MOB	*body	= (struct body_GC_CHAR_ATTACK_MOB *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size	= sizeof(struct body_GC_CHAR_ATTACK_MOB);
	header->command		= GC_CHAR_ATTACK_MOB;

	body->char_id		= fd;
	body->mob_id		= mob_id;
	body->attack_id		= attack_id;
	body->attack_type	= attack_type;
	body->mob_hp		= mob_hp;
	body->damage_type	= damage_type;
	body->damage		= damage;
	body->dir		= dir;
	body->speed		= speed;
	body->x			= x;
	body->y			= y;
	body->z			= z;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}	


int	MK_GetPacket_GC_CHAR_ACQUIRE_EXP(char *send_buf, const int fd, const int acquire_exp)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_ACQUIRE_EXP	*body	= (struct body_GC_CHAR_ACQUIRE_EXP *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size	= sizeof(struct body_GC_CHAR_ACQUIRE_EXP);
	header->command		= GC_CHAR_ACQUIRE_EXP;

	body->exp		= D_GetClient_exp(fd);
	body->acquire_exp	= acquire_exp;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}


int	MK_GetPacket_GC_CHAR_LEVEL_UP(char *send_buf, const int fd)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_LEVEL_UP	*body	= (struct body_GC_CHAR_LEVEL_UP *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size	= sizeof(struct body_GC_CHAR_LEVEL_UP);
	header->command		= GC_CHAR_LEVEL_UP;

	body->char_id		= fd;
	body->char_hp		= D_GetClient_hp(fd);
	body->char_ap		= D_GetClient_ap(fd);
	body->skill_point	= D_GetClient_skill_point(fd);

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}
	

int	MK_GetPacket_GC_MOB_ATTACK_CHAR(char *send_buf, const int channel, const int mob_id,
						const int char_id, const int char_hp, const int damage_type, const int damage, const int dir)
{
	float				x, y, z;
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body_GC_MOB_ATTACK_CHAR	*body	= (struct body_GC_MOB_ATTACK_CHAR *)(send_buf + SIZEOF_HEADER_NORMAL);


	D_GetMob_xyz(channel, mob_id, &x, &y, &z);

	header->body_size	= sizeof(struct body_GC_MOB_ATTACK_CHAR);
	header->command		= GC_MOB_ATTACK_CHAR;

	body->mob_id		= mob_id;
	body->char_id		= char_id;
	body->char_hp		= char_hp;
	body->damage_type	= damage_type;
	body->damage		= damage;
	body->dir		= dir;
	body->speed		= 0;
	body->x			= x;
	body->y			= y;
	body->z			= z;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}	
	

int	MK_GetPacket_GC_CHAR_REGEN_RESULT(char *send_buf, const int result_code, const int fd)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_REGEN_RESULT	*body	= (struct body_GC_CHAR_REGEN_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size	= sizeof(struct body_GC_CHAR_REGEN_RESULT);
	header->command		= GC_CHAR_REGEN_RESULT;

	body->result_code	= result_code;
	D_GetClient_regen_data(body, fd);

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}


int	MK_GetPacket_GC_ITEM_APPEAR(char *send_buf, int *link_ptr, const int link_follow)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body1_GC_ITEM_APPEAR	*body1	= (struct body1_GC_ITEM_APPEAR *)(send_buf + SIZEOF_HEADER_NORMAL);
	struct	body2_GC_ITEM_APPEAR	*body2	
				= (struct body2_GC_ITEM_APPEAR *)(send_buf + SIZEOF_HEADER_NORMAL + sizeof(struct body1_GC_ITEM_APPEAR));


	header->command		= GC_ITEM_APPEAR;
	body1->item_cnt		= 1;
	D_GetItem_appear_data(body2, *link_ptr);
	header->body_size	= sizeof(struct body1_GC_ITEM_APPEAR) + sizeof(struct body2_GC_ITEM_APPEAR);

	if( link_follow )
	{
		while( (*link_ptr = D_GetItem_block_next(*link_ptr)) )
		{
			if( (header->body_size + sizeof(struct body2_GC_ITEM_APPEAR)) > SIZEOF_MAX_GAME_PACKET_BODY )
				break;
			
			body2			= (struct body2_GC_ITEM_APPEAR *)(send_buf + SIZEOF_HEADER_NORMAL + header->body_size);
			D_GetItem_appear_data(body2, *link_ptr);
			header->body_size	+= sizeof(struct body2_GC_ITEM_APPEAR);
			++(body1->item_cnt);
		}
	}

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}


int	MK_GetPacket_GC_ITEM_DISAPPEAR(char *send_buf, int *link_ptr, const int link_follow)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body1_GC_ITEM_DISAPPEAR	*body1	= (struct body1_GC_ITEM_DISAPPEAR *)(send_buf + SIZEOF_HEADER_NORMAL);
	struct	body2_GC_ITEM_DISAPPEAR	*body2
			= (struct body2_GC_ITEM_DISAPPEAR *)(send_buf + SIZEOF_HEADER_NORMAL + sizeof(struct body1_GC_ITEM_DISAPPEAR));


	header->command		= GC_ITEM_DISAPPEAR;
	body1->item_cnt		= 1;
	body2->item_id		= *link_ptr;
	header->body_size	= sizeof(struct body1_GC_ITEM_DISAPPEAR) + sizeof(struct body2_GC_ITEM_DISAPPEAR);

	if( link_follow )
	{
		while( (*link_ptr = D_GetItem_block_next(*link_ptr)) )
		{
			if( (header->body_size + sizeof(struct body2_GC_ITEM_DISAPPEAR)) > SIZEOF_MAX_GAME_PACKET_BODY )
				break;
			
			body2			= (struct body2_GC_ITEM_DISAPPEAR *)(send_buf + SIZEOF_HEADER_NORMAL + header->body_size);
			body2->item_id		= *link_ptr;
			header->body_size	+= sizeof(struct body2_GC_ITEM_DISAPPEAR);
			++(body1->item_cnt);
		}
	}

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}
		

int	MK_GetPacket_GC_CHAR_ITEM_SLOT(char *send_buf, const int fd, const int item_slot)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_ITEM_SLOT	*body	= (struct body_GC_CHAR_ITEM_SLOT *)(send_buf + SIZEOF_HEADER_NORMAL);
	
	
	header->body_size	= sizeof(struct body_GC_CHAR_ITEM_SLOT);
	header->command		= GC_CHAR_ITEM_SLOT;

	body->char_id		= fd;
	body->item_slot		= item_slot;
	memcpy(body->item, D_GetClient_item_slot(fd, item_slot), ITEM_DATA_LEN);
	
	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}
		

int	MK_GetPacket_GC_CHAR_ITEM_PICKUP_RESULT(char *send_buf, const int result_code, const int fd,
								const int item_id, const int item_slot, const int acquire_money)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_ITEM_PICKUP_RESULT	*body	= (struct body_GC_CHAR_ITEM_PICKUP_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);
	
	
	header->body_size	= sizeof(struct body_GC_CHAR_ITEM_PICKUP_RESULT);
	header->command		= GC_CHAR_ITEM_PICKUP_RESULT;

	body->result_code	= result_code;
	body->item_id		= item_id;
	body->money		= D_GetClient_money(fd);
	body->acquire_money	= acquire_money;
	body->item_slot		= item_slot;
	memcpy(body->item, D_GetClient_item_slot(fd, item_slot), ITEM_DATA_LEN);

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}

		
int	 MK_GetPacket_GC_CHAR_ITEM_ENCHANT_RESULT(char *send_buf, const int result_code, const int fd, const int item_slot1, const int item_slot2)
{
	struct	header_normal				*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_ITEM_ENCHANT_RESULT	*body	= (struct body_GC_CHAR_ITEM_ENCHANT_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);
	
	
	header->body_size	= sizeof(struct body_GC_CHAR_ITEM_ENCHANT_RESULT);
	header->command		= GC_CHAR_ITEM_ENCHANT_RESULT;

	body->result_code	= result_code;
	body->item_slot1	= item_slot1;
	memcpy(body->item1, D_GetClient_item_slot(fd, item_slot1), ITEM_DATA_LEN);
	body->item_slot2	= item_slot2;
	memcpy(body->item2, D_GetClient_item_slot(fd, item_slot2), ITEM_DATA_LEN);

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}
	

int	MK_GetPacket_GC_CHAR_BUFF(char *send_buf, const int fd, const int buff_series)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_BUFF		*body	= (struct body_GC_CHAR_BUFF *)(send_buf + SIZEOF_HEADER_NORMAL);
	
	
	header->body_size	= sizeof(struct body_GC_CHAR_BUFF);
	header->command		= GC_CHAR_BUFF;

	body->buff_series	= buff_series;
	body->buff_value	= D_GetClient_buff_value(fd, buff_series);
	body->buff_sec		= D_GetClient_buff_end_sec(fd, buff_series);

	if( body->buff_sec )
		body->buff_sec	-= G_Sec;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}


int	MK_GetPacket_GC_CHAR_WARP_RESULT(char *send_buf, const int result_code, const int fd)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_WARP_RESULT	*body	= (struct body_GC_CHAR_WARP_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size	= sizeof(struct body_GC_CHAR_WARP_RESULT);
	header->command		= GC_CHAR_WARP_RESULT;

	body->result_code	= result_code;
	D_GetClient_warp_data(body, fd);

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}
	

int	MK_GetPacket_GC_CHAR_STORE_ITEM_BUY_RESULT(char *send_buf, const int result_code, const int fd, const int item_slot)
{
	struct	header_normal				*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_STORE_ITEM_BUY_RESULT	*body	= (struct body_GC_CHAR_STORE_ITEM_BUY_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size	= sizeof(struct body_GC_CHAR_STORE_ITEM_BUY_RESULT);
	header->command		= GC_CHAR_STORE_ITEM_BUY_RESULT;

	body->result_code	= result_code;
	body->money		= D_GetClient_money(fd);
	body->item_slot		= item_slot;
	memcpy(body->item, D_GetClient_item_slot(fd, item_slot), ITEM_DATA_LEN);

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}


int	MK_GetPacket_GC_CHAR_STORE_ITEM_SELL_RESULT(char *send_buf, const int result_code, const int fd, const int item_slot)
{
	struct	header_normal				*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_STORE_ITEM_SELL_RESULT	*body	= (struct body_GC_CHAR_STORE_ITEM_SELL_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size	= sizeof(struct body_GC_CHAR_STORE_ITEM_SELL_RESULT);
	header->command		= GC_CHAR_STORE_ITEM_SELL_RESULT;

	body->result_code	= result_code;
	body->money		= D_GetClient_money(fd);
	body->item_slot		= item_slot;
	memcpy(body->item, D_GetClient_item_slot(fd, item_slot), ITEM_DATA_LEN);

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}


int	MK_GetPacket_GC_CHAR_RECOVER(char *send_buf, const int fd)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_RECOVER	*body	= (struct body_GC_CHAR_RECOVER *)(send_buf + SIZEOF_HEADER_NORMAL);
	
	
	header->body_size	= sizeof(struct body_GC_CHAR_RECOVER);
	header->command		= GC_CHAR_RECOVER;

	body->hp		= D_GetClient_hp(fd);
	body->ap		= D_GetClient_ap(fd);

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}


int	MK_GetPacket_GC_CHAR_ITEM_USE_RESULT(char *send_buf, const int fd, const int item_slot, const int item_type)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_ITEM_USE_RESULT	*body	= (struct body_GC_CHAR_ITEM_USE_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size	= sizeof(struct body_GC_CHAR_ITEM_USE_RESULT);
	header->command		= GC_CHAR_ITEM_USE_RESULT;

	body->item_type		= item_type;
	body->hp		= D_GetClient_hp(fd);
	body->ap		= D_GetClient_ap(fd);
	body->item_slot		= item_slot;
	memcpy(body->item, D_GetClient_item_slot(fd, item_slot), ITEM_DATA_LEN);

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}


int	MK_GetPacket_GC_CHAR_SKILL_READY(char *send_buf, const int fd, const int skill_type,
							const int dir, const float speed, const float x, const float y, const float z)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_SKILL_READY	*body	= (struct body_GC_CHAR_SKILL_READY *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size	= sizeof(struct body_GC_CHAR_SKILL_READY);
	header->command		= GC_CHAR_SKILL_READY;

	body->char_id		= fd;
	body->skill_type	= skill_type;
	body->dir		= dir;
	body->speed		= speed;
	body->x			= x;
	body->y			= y;
	body->z			= z;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}
	

int	MK_GetPacket_GC_CHAR_SKILL_ACTION(char *send_buf)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body1_GC_CHAR_SKILL_ACTION	*body1	= (struct body1_GC_CHAR_SKILL_ACTION *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size
	= sizeof(struct body1_GC_CHAR_SKILL_ACTION) + sizeof(struct body2_GC_CHAR_SKILL_ACTION)*(body1->target_mob_cnt+body1->target_char_cnt);
	header->command		= GC_CHAR_SKILL_ACTION;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}	

	
int	MK_GetPacket_GC_CHAR_SKILL_CANCEL(char *send_buf, const int fd)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_SKILL_CANCEL	*body	= (struct body_GC_CHAR_SKILL_CANCEL *)(send_buf + SIZEOF_HEADER_NORMAL);
	
	
	header->body_size	= sizeof(struct body_GC_CHAR_SKILL_CANCEL);
	header->command		= GC_CHAR_SKILL_CANCEL;

	body->char_id		= fd;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}
	

int	MK_GetPacket_GC_MOB_SKILL_READY(char *send_buf, const int mob_id, const int skill_type,
								const int dir, const float speed, const float x, const float y, const float z)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body_GC_MOB_SKILL_READY	*body	= (struct body_GC_MOB_SKILL_READY *)(send_buf + SIZEOF_HEADER_NORMAL);
	
	
	header->body_size	= sizeof(struct body_GC_MOB_SKILL_READY);
	header->command		= GC_MOB_SKILL_READY;

	body->mob_id		= mob_id;
	body->skill_type	= skill_type;
	body->dir		= dir;
	body->speed		= speed;
	body->x			= x;
	body->y			= y;
	body->z			= z;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}


int	MK_GetPacket_GC_MOB_SKILL_ACTION(char *send_buf)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body1_GC_MOB_SKILL_ACTION	*body1	= (struct body1_GC_MOB_SKILL_ACTION *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size
		= sizeof(struct body1_GC_MOB_SKILL_ACTION) + sizeof(struct body2_GC_MOB_SKILL_ACTION)*(body1->target_mob_cnt+body1->target_char_cnt);
	header->command		= GC_MOB_SKILL_ACTION;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}	


int	MK_GetPacket_GC_MOB_SKILL_CANCEL(char *send_buf, const int mob_id)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body_GC_MOB_SKILL_CANCEL	*body	= (struct body_GC_MOB_SKILL_CANCEL *)(send_buf + SIZEOF_HEADER_NORMAL);
	
	
	header->body_size	= sizeof(struct body_GC_MOB_SKILL_CANCEL);
	header->command		= GC_MOB_SKILL_CANCEL;

	body->mob_id		= mob_id;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}


int	MK_GetPacket_GC_CHAR_TRANSFORM(char *send_buf, const int fd, const int transform_type)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body_GC_CHAR_TRANSFORM		*body	= (struct body_GC_CHAR_TRANSFORM *)(send_buf + SIZEOF_HEADER_NORMAL);
	
	
	header->body_size	= sizeof(struct body_GC_CHAR_TRANSFORM);
	header->command		= GC_CHAR_TRANSFORM;

	body->char_id		= fd;
	body->transform_type		= transform_type;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);	
}


int	MK_GetPacket_GC_MANAGER_CHAT(char *send_buf,  const int msg_size, const char *msg)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;


	header->body_size	= sizeof(struct body_GC_MANAGER_CHAT) + msg_size;
	header->command		= GC_MANAGER_CHAT;

	strncpy(send_buf + SIZEOF_HEADER_NORMAL + sizeof(struct body_GC_MANAGER_CHAT), msg, msg_size);

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}
