/*____________________________________________________________

	Project name : Qring online
	File name    : mk_packet.c
	Description  : make packet function
	Date         : February 14 2005 
_____________________________________________________________*/

#include	"global.h"
#include	"util.h"
#include	"db.h"
#define	_MK_PACKET_H_
#include	"mk_packet.h"



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
	
	strncpy(body->version, VERSION, VERSION_LEN);	

	return	(SIZEOF_HEADER_SPECIAL + header->body_size);
}


int     MK_GetPacket_WS_MYINFO(char *send_buf)
{
	struct  header_special  *header = (struct header_special *)send_buf;


	header->body_size       = sizeof(struct body_WS_MYINFO);
	header->command         = WS_MYINFO;

	return  (SIZEOF_HEADER_SPECIAL + header->body_size);
}


int      MK_GetPacket_WS_WORLD_INFO(char *send_buf, const unsigned short game)
{
	struct  header_special          *header = (struct header_special *)send_buf;
	struct  body_WS_WORLD_INFO      *body   = (struct body_WS_WORLD_INFO *)(send_buf + SIZEOF_HEADER_SPECIAL);


	header->body_size       = sizeof(struct body_WS_WORLD_INFO);
	header->command         = WS_WORLD_INFO;

	body->game              = game;

	return  (SIZEOF_HEADER_SPECIAL + header->body_size);

}


int      MK_GetPacket_WS_GAMESERV_INFO(char *send_buf, const unsigned short game, const unsigned short world)
{
	struct  header_special          *header = (struct header_special *)send_buf;
	struct  body_WS_GAMESERV_INFO   *body   = (struct body_WS_GAMESERV_INFO *)(send_buf + SIZEOF_HEADER_SPECIAL);


	header->body_size       = sizeof(struct body_WS_GAMESERV_INFO);
	header->command         = WS_GAMESERV_INFO;

	body->game              = game;
	body->world             = world;

	return  (SIZEOF_HEADER_SPECIAL + header->body_size);

}


int      MK_GetPacket_WS_DBAGENT_INFO(char *send_buf, const unsigned short game, const unsigned short world)
{
	struct  header_special          *header = (struct header_special *)send_buf;
	struct  body_WS_DBAGENT_INFO    *body   = (struct body_WS_DBAGENT_INFO *)(send_buf + SIZEOF_HEADER_SPECIAL);


	header->body_size       = sizeof(struct body_WS_DBAGENT_INFO);
	header->command         = WS_DBAGENT_INFO;

	body->game              = game;
	body->world             = world;

	return  (SIZEOF_HEADER_SPECIAL + header->body_size);

}


//____________________________________________________________
//WA
int	MK_GetPacket_WA_INIT(char *send_buf)
{
	struct  header_special  *header = (struct header_special *)send_buf;
	struct  body_WA_INIT    *body   = (struct body_WA_INIT *)(send_buf + SIZEOF_HEADER_SPECIAL);


	header->body_size       = sizeof(struct body_WA_INIT);
	header->command         = WA_INIT;

	strncpy(body->version, VERSION, VERSION_LEN);

	return  (SIZEOF_HEADER_SPECIAL + header->body_size);
}


int	MK_GetPacket_WA_LIVE(char *send_buf)
{
	struct  header_special  *header = (struct header_special *)send_buf;


	header->body_size       = sizeof(struct body_WA_LIVE);
	header->command         = WA_LIVE;

	return  (SIZEOF_HEADER_SPECIAL + header->body_size);
}





