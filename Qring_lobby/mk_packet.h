/*____________________________________________________________

	Project name : Qring Online
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

/*
G	int	MK_GetPacket_WB_REGSERVER(char *);
G	int	MK_GetPacket_WB_GAMESTART(char *, const int, char *);
G	int	MK_GetPacket_WB_GAMESTOP(char *, const int, char *, char *);
G	int	MK_GetPacket_WB_USER_CHECK_SUCC(char *, const int, char *);
G	int	MK_GetPacket_WB_USER_CHECK_FAIL(char *, const int, char *);
*/
G	int	MK_GetPacket_TW_WAKEUP(char *);

G	int	MK_GetPacket_WS_INIT(char *);
G	int	MK_GetPacket_WS_MYINFO(char *);
G	int	MK_GetPacket_WS_WORLD_INFO(char *, const int);
G	int	MK_GetPacket_WS_GAMESERV_INFO(char *, const int, const int);
G	int	MK_GetPacket_WS_DBAGENT_INFO(char *, const int, const int);
G	int	MK_GetPacket_WS_LEVEL_FILES(char *);

G	int	MK_GetPacket_WA_INIT(char *, const int);
G	int	MK_GetPacket_WA_LIVE(char *);
G	int	MK_GetPacket_WA_CHAR_LIST(char *, const int);
G	int	MK_GetPacket_WA_CHAR_CREATE(char *, const int, const char *, const unsigned char, const unsigned char, const unsigned char);
G	int	MK_GetPacket_WA_CHAR_DELETE(char *, const int, const char *);
G	int	MK_GetPacket_WA_LOGIN(char *, const int, const char *, const char *);
G	int	MK_GetPacket_WA_LOGOUT(char *, const int, const int, const int, const char *);
G	int	MK_GetPacket_WA_GET_USER(char *, const int, const int);

G	int	MK_GetPacket_LC_SERVER_TROUBLE(char *, const int);
G	int	MK_GetPacket_LC_LOGIN_RESULT(char *, const int);
G	int	MK_GetPacket_LC_LIVE(char *);
G	int	MK_GetPacket_LC_WORLD_LIST_RESULT(char *, const int);
G	int	MK_GetPacket_LC_CHANNEL_LIST_RESULT(char *, const int, const int);
G	int	MK_GetPacket_LC_CHAR_LIST_RESULT(char *, const int, const int, struct body2_AW_CHAR_LIST_RESULT *);
G	int	MK_GetPacket_LC_CHAR_CREATE_RESULT(char *, const int, const char *);
G	int	MK_GetPacket_LC_CHAR_DELETE_RESULT(char *, const int, const char *);
G	int	MK_GetPacket_LC_CHANNEL_CONNECT_RESULT(char *, const int, const int, const int);


#undef	G
#undef	_MK_PACKET_H_

