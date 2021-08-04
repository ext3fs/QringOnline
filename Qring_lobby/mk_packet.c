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
	struct	body_WS_INIT	*body	= (struct body_XS_INIT *)(send_buf + SIZEOF_HEADER_SPECIAL);


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


int	MK_GetPacket_WS_LEVEL_FILES(char *send_buf)
{
	struct	header_special		*header	= (struct header_special *)send_buf;


	header->body_size	= sizeof(struct body_WS_LEVEL_FILES);
	header->command		= WS_LEVEL_FILES;

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
		

int	MK_GetPacket_WA_CHAR_LIST(char *send_buf, const int fd)
{
	struct	header_special		*header	= (struct header_special *)send_buf;
	struct	body_WA_CHAR_LIST	*body	= (struct body_WA_CHAR_LIST *)(send_buf + SIZEOF_HEADER_SPECIAL);


	header->body_size	= sizeof(struct body_WA_CHAR_LIST);
	header->fd		= fd;
	header->command		= WA_CHAR_LIST;
	header->sec_active	= D_GetClient_sec_active(fd);
	header->session		= D_GetClient_session(fd);
	
	strncpy(body->user_id, D_GetClient_user_id(fd), USER_ID_LEN+1);

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}

		
int	MK_GetPacket_WA_CHAR_CREATE(char *send_buf, const int fd,
			const char *char_name, const unsigned char sex, const unsigned char hair, const unsigned char face)
{
	struct	header_special		*header	= (struct header_special *)send_buf;
	struct	body_WA_CHAR_CREATE	*body	= (struct body_WA_CHAR_CREATE *)(send_buf + SIZEOF_HEADER_SPECIAL);


	header->body_size	= sizeof(struct body_WA_CHAR_CREATE);
	header->fd		= fd;
	header->command		= WA_CHAR_CREATE;
	header->sec_active	= D_GetClient_sec_active(fd);
	header->session		= D_GetClient_session(fd);
	
	strncpy(body->user_id, D_GetClient_user_id(fd), USER_ID_LEN+1);
	strncpy(body->char_name, char_name, CHAR_NAME_LEN+1);
	body->map	= 1;
	body->x		= 173;
	body->y		= 0;
	body->z		= 14;
	body->regen_map	= 1;
	body->regen_x	= 173;
	body->regen_y	= 0;
	body->regen_z	= 14;
	body->sex	= sex;
	body->hair	= hair;
	body->face	= face;
	body->level	= 1;
	body->exp	= 0;
	body->hp	= D_GetLevel_hp_max(1);
	body->ap	= D_GetLevel_ap_max(1);

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}


int	MK_GetPacket_WA_CHAR_DELETE(char *send_buf, const int fd, const char *char_name)
{
	struct	header_special		*header	= (struct header_special *)send_buf;
	struct	body_WA_CHAR_DELETE	*body	= (struct body_WA_CHAR_DELETE *)(send_buf + SIZEOF_HEADER_SPECIAL);


	header->body_size	= sizeof(struct body_WA_CHAR_DELETE);
	header->fd		= fd;
	header->command		= WA_CHAR_DELETE;
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

			
int	MK_GetPacket_WA_GET_USER(char *send_buf, const int world, const int channel)
{
	struct	header_special		*header	= (struct header_special *)send_buf;
	struct	body_WA_GET_USER	*body	= (struct body_WA_GET_USER *)(send_buf + SIZEOF_HEADER_SPECIAL);


	header->body_size	= sizeof(struct body_WA_GET_USER);
	header->command		= WA_GET_USER;
	
	body->msvr_code		= world;
	body->ssvr_code		= channel;
	
	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}

	
//_________________________________________________________________________
//LC
int	MK_GetPacket_LC_SERVER_TROUBLE(char *send_buf, const int trouble)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body_LC_SERVER_TROUBLE	*body	= (struct body_LC_SERVER_TROUBLE *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size	= sizeof(struct body_LC_SERVER_TROUBLE);
	header->command		= LC_SERVER_TROUBLE;

	body->trouble		= trouble;	

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}


int	MK_GetPacket_LC_LOGIN_RESULT(char *send_buf, const int result_code)
{
	struct	header_normal		*header	= (struct header_normal *)send_buf;
	struct	body_LC_LOGIN_RESULT	*body	= (struct body_LC_LOGIN_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);


	header->body_size	= sizeof(struct body_LC_LOGIN_RESULT);
	header->command		= LC_LOGIN_RESULT;

	body->result_code	= result_code;	
	
	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}

int	MK_GetPacket_LC_LIVE(char *send_buf)
{
	struct	header_normal	*header	= (struct header_normal *)send_buf;
	

	header->body_size	= sizeof(struct body_LC_LIVE);
	header->command		= LC_LIVE;

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}

	
int	MK_GetPacket_LC_WORLD_LIST_RESULT(char *send_buf, const int result_code)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body1_LC_WORLD_LIST_RESULT	*body1	= (struct body1_LC_WORLD_LIST_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);
	struct	body2_LC_WORLD_LIST_RESULT	*body2
			= (struct body2_LC_WORLD_LIST_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL + sizeof(struct body1_LC_WORLD_LIST_RESULT));
	int					i;


	header->body_size	= sizeof(struct body1_LC_WORLD_LIST_RESULT) + sizeof(struct body2_LC_WORLD_LIST_RESULT) * D_GetMax_world();
	header->command		= LC_WORLD_LIST_RESULT;

	body1->result_code	= result_code;	
	body1->world_cnt	= D_GetMax_world();
		
	for( i=1; i<=D_GetMax_world(); i++)
	{
		body2->world_idx	= i;
		strncpy(body2->world_name, D_GetWorld_name(i), WORLD_NAME_LEN+1);	
		++body2; 
	}

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}


int	MK_GetPacket_LC_CHANNEL_LIST_RESULT(char *send_buf, const int result_code, const int world_idx)
{
	int					i;
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body1_LC_CHANNEL_LIST_RESULT	*body1	= (struct body1_LC_CHANNEL_LIST_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);
	struct	body2_LC_CHANNEL_LIST_RESULT	*body2
			= (struct body2_LC_CHANNEL_LIST_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL + sizeof(struct body1_LC_CHANNEL_LIST_RESULT));


	header->body_size
		= sizeof(struct body1_LC_CHANNEL_LIST_RESULT) + sizeof(struct body2_LC_CHANNEL_LIST_RESULT) * D_GetWorld_channel_cnt(world_idx);
	header->command		= LC_CHANNEL_LIST_RESULT;

	body1->result_code	= result_code;	
	body1->channel_cnt	= D_GetWorld_channel_cnt(world_idx);
		
	for( i=1; i<=body1->channel_cnt; i++)
	{
		body2->user		= D_GetWorld_channel_user(world_idx, i);	
		++body2; 
	}

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}
		

int	MK_GetPacket_LC_CHAR_LIST_RESULT(char *send_buf, const int result_code, const int char_cnt, struct body2_AW_CHAR_LIST_RESULT *char_list)
{
	int					i;	
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body1_LC_CHAR_LIST_RESULT	*body1	= (struct body1_LC_CHAR_LIST_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);
	struct	body2_LC_CHAR_LIST_RESULT	*body2
			= (struct body2_LC_CHAR_LIST_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL + sizeof(struct body1_LC_CHAR_LIST_RESULT));

	header->body_size	= sizeof(struct body1_LC_CHAR_LIST_RESULT) + char_cnt*sizeof(struct body2_LC_CHAR_LIST_RESULT);
	header->command		= LC_CHAR_LIST_RESULT;

	body1->result_code	= result_code;
	body1->char_cnt		= char_cnt;

	for( i=0; i<char_cnt; i++ )
	{
		strncpy(body2->char_name, char_list->char_name, CHAR_NAME_LEN+1);
		body2->sex	= char_list->sex;
		body2->hair	= char_list->hair;
		body2->face	= char_list->face;
		body2->level	= char_list->level;

		++body2;
		++char_list;
	}

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}


int	MK_GetPacket_LC_CHAR_CREATE_RESULT(char *send_buf, const int result_code, const char *char_name)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body_LC_CHAR_CREATE_RESULT	*body	= (struct body_LC_CHAR_CREATE_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);
	

	header->body_size	= sizeof(struct body_LC_CHAR_CREATE_RESULT);
	header->command		= LC_CHAR_CREATE_RESULT;

	body->result_code	= result_code;
	strncpy(body->char_name, char_name, CHAR_NAME_LEN+1);	

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}
		

int	MK_GetPacket_LC_CHAR_DELETE_RESULT(char *send_buf, const int result_code, const char *char_name)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body_LC_CHAR_DELETE_RESULT	*body	= (struct body_LC_CHAR_DELETE_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);
	

	header->body_size	= sizeof(struct body_LC_CHAR_DELETE_RESULT);
	header->command		= LC_CHAR_DELETE_RESULT;

	body->result_code	= result_code;
	strncpy(body->char_name, char_name, CHAR_NAME_LEN+1);	

	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}
	

int	MK_GetPacket_LC_CHANNEL_CONNECT_RESULT(char *send_buf, const int result_code, const int world_idx, const int channel_idx)
{
	struct	header_normal			*header	= (struct header_normal *)send_buf;
	struct	body_LC_CHANNEL_CONNECT_RESULT	*body	= (struct body_LC_CHANNEL_CONNECT_RESULT *)(send_buf + SIZEOF_HEADER_NORMAL);
	

	header->body_size	= sizeof(struct body_LC_CHANNEL_CONNECT_RESULT);
	header->command		= LC_CHANNEL_CONNECT_RESULT;

	body->result_code	= result_code;
	strncpy(body->channel_ip, D_GetWorld_machine_outer_ip(world_idx, (channel_idx-1)/CHANNEL_PER_MACHINE +1), SIZEOF_IP+1);	
	body->channel_port	= GAME_PORT;
	U_Printf("result_code=%d, ip=%s, port=%d\n", body->result_code, body->channel_ip, body->channel_port);
	
	return	(SIZEOF_HEADER_NORMAL + header->body_size);
}
