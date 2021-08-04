/*____________________________________________________________

	Project name : Qring online
	File name    : mk_packet.h
	Description  : make packet function
	Date         : February 14 2005 
_____________________________________________________________*/

#ifdef	G
#undef	G
#endif

#ifdef	_MK_PACKET_H_
#define	G
#else
#define	G	extern
#endif


//____________________________________________________________
//TG
G	int	MK_GetPacket_TW_WAKEUP(char *);


//____________________________________________________________
//WS
G	int	MK_GetPacket_WS_INIT(char *);
G	int	MK_GetPacket_WS_MYINFO(char *);
G	int	MK_GetPacket_WS_WORLD_INFO(char *, const unsigned short);
G	int	MK_GetPacket_WS_GAMESERV_INFO(char *, const unsigned short, const unsigned short);
G	int	MK_GetPacket_WS_DBAGENT_INFO(char *, const unsigned short, const unsigned short);


//____________________________________________________________
//XA
G	int	MK_GetPacket_WA_INIT(char *);
G	int	MK_GetPacket_WA_LIVE(char *);


#undef	G
#undef	_MK_PACKET_H_

