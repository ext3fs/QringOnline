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

//G	int	MK_GetPacket_WB_REGSERVER(char *);
//G	int	MK_GetPacket_WB_GAMESTART(char *, const int, char *);
//G	int	MK_GetPacket_WB_GAMESTOP(char *, const int, char *, char *);
//G	int	MK_GetPacket_WB_USER_CHECK_SUCC(char *, const int, char *);
//G	int	MK_GetPacket_WB_USER_CHECK_FAIL(char *, const int, char *);

G	int	MK_GetPacket_TW_WAKEUP(char *);

G	int	MK_GetPacket_WS_INIT(char *);
G	int	MK_GetPacket_WS_MYINFO(char *);
G	int	MK_GetPacket_WS_WORLD_INFO(char *, const int);
G	int	MK_GetPacket_WS_GAMESERV_INFO(char *, const int, const int);
G	int	MK_GetPacket_WS_DBAGENT_INFO(char *, const int, const int);
G	int	MK_GetPacket_WS_MOBSERV_INFO(char *, const int, const int);
G	int	MK_GetPacket_WS_MAP_FILES(char *);
G	int	MK_GetPacket_WS_MOB_FILES(char *);
G	int	MK_GetPacket_WS_TYPE_MOB_FILES(char *);
G	int	MK_GetPacket_WS_LEVEL_FILES(char *);
G	int	MK_GetPacket_WS_TYPE_ITEM_FILES(char *);
G	int	MK_GetPacket_WS_TYPE_DROP_FILES(char *);
G	int	MK_GetPacket_WS_TYPE_WARP_FILES(char *);
G	int	MK_GetPacket_WS_TYPE_STORE_FILES(char *);
G	int	MK_GetPacket_WS_TYPE_SKILL_FILES(char *);
G	int	MK_GetPacket_WS_TYPE_MOB_SKILL_FILES(char *);

G	int	MK_GetPacket_WA_INIT(char *, const int);
G	int	MK_GetPacket_WA_LIVE(char *);
G	int	MK_GetPacket_WA_CHAR_SAVE(char *, const int, const char *, const int);
G	int	MK_GetPacket_WA_CHAR_LOAD(char *, const int, const char *);
G	int	MK_GetPacket_WA_LOGIN(char *, const int, const char *, const char *);
G	int	MK_GetPacket_WA_LOGOUT(char *, const int, const int, const int, const char *);
G	int	MK_GetPacket_WA_CURRENT_USER(char *, const int);
G	int	MK_GetPacket_WA_GAME_LOG(char *, const int, const int, const int, const int);

G	int	MK_GetPacket_GM_LIVE(char *);
G	int	MK_GetPacket_GM_INIT_RESULT(char *, const int);
G	int	MK_GetPacket_GM_CHAR_CHANNEL_IN(char *, int *, const int);
G	int	MK_GetPacket_GM_CHAR_CHANNEL_OUT(char *, int *, const int);
G	int	MK_GetPacket_GM_CHAR_MAP_CHANGE(char *, const int);
G	int	MK_GetPacket_GM_CHAR_MOVE(char *, const int, const int, const float, const float, const float);
G	int	MK_GetPacket_GM_CHAR_ATTACK_MOB(char *, const int,
					const int, const int, const int, const int, const int, const float, const float, const float);
G	int	MK_GetPacket_GM_CHAR_SKILL_ACTION(char *);
G	int	MK_GetPacket_GM_MOB_BUFF(char *, const int, const int, const int);

G	int	MK_GetPacket_GC_SERVER_TROUBLE(char *, const int);
G	int	MK_GetPacket_GC_LOGIN_RESULT(char *, const int);
G	int	MK_GetPacket_GC_LIVE(char *);
G	int	MK_GetPacket_GC_CHANNEL_LIST_RESULT(char *, const int, const int);
G	int	MK_GetPacket_GC_CHAR_CHANNEL_MOVE_RESULT(char *, const int, const int, const int, const int);
G	int	MK_GetPacket_GC_CHAR_LOAD_RESULT(char *, const int, const int, const char *);
G	int	MK_GetPacket_GC_OTHER_CHAR_APPEAR(char *, int *, const int);
G	int	MK_GetPacket_GC_OTHER_CHAR_DISAPPEAR(char *, int *, const int);
G	int	MK_GetPacket_GC_CHAR_MOVE(char *, const int, const int, const int, const float, const float, const float, const float);
G	int	MK_GetPacket_GC_CHAR_CHAT(char *, const int, const int, const char *);
G	int	MK_GetPacket_GC_MOB_APPEAR(char *, const int, int *, const int);
G	int	MK_GetPacket_GC_MOB_DISAPPEAR(char *, const int, int *, const int);
G	int	MK_GetPacket_GC_MOB_MOVE(char *, const int, const int, const int, const float, const float, const float, const float);
G	int	MK_GetPacket_GC_CHAR_ATTACK_MOB(char *, const int, const int, const int, const int, const int, const int, const int, const int, const float,
							const float, const float, const float);
G	int	MK_GetPacket_GC_CHAR_ACQUIRE_EXP(char *, const int, const int);
G	int	MK_GetPacket_GC_CHAR_LEVEL_UP(char *, const int);
G	int	MK_GetPacket_GC_MOB_ATTACK_CHAR(char *, const int, const int, const int, const int, const int, const int, const int);
G	int	MK_GetPacket_GC_CHAR_REGEN_RESULT(char *, const int, const int);
G	int	MK_GetPacket_GC_ITEM_APPEAR(char *, int *, const int);
G	int	MK_GetPacket_GC_ITEM_DISAPPEAR(char *, int *, const int);
G	int	MK_GetPacket_GC_CHAR_ITEM_SLOT(char *, const int, const int);
G	int	MK_GetPacket_GC_CHAR_ITEM_PICKUP_RESULT(char *, const int, const int, const int, const int, const int);
G	int	MK_GetPacket_GC_CHAR_ITEM_ENCHANT_RESULT(char *, const int, const int, const int, const int);
G	int	MK_GetPacket_GC_CHAR_BUFF(char *, const int, const int);
G	int	MK_GetPacket_GC_CHAR_WARP_RESULT(char *, const int, const int);
G	int	MK_GetPacket_GC_CHAR_STORE_ITEM_BUY_RESULT(char *, const int, const int, const int);
G	int	MK_GetPacket_GC_CHAR_STORE_ITEM_SELL_RESULT(char *, const int, const int, const int);
G	int	MK_GetPacket_GC_CHAR_RECOVER(char *, const int);
G	int	MK_GetPacket_GC_CHAR_ITEM_USE_RESULT(char *, const int, const int, const int);
G	int	MK_GetPacket_GC_CHAR_SKILL_READY(char *, const int, const int,
							const int, const float, const float, const float, const float);
G	int	MK_GetPacket_GC_CHAR_SKILL_ACTION(char *);
G	int	MK_GetPacket_GC_CHAR_SKILL_CANCEL(char *, const int);
G	int	MK_GetPacket_GC_MOB_SKILL_READY(char *, const int, const int, const int, const float, const float, const float, const float);
G	int	MK_GetPacket_GC_MOB_SKILL_ACTION(char *);
G	int	MK_GetPacket_GC_MOB_SKILL_CANCEL(char *, const int);
G	int	MK_GetPacket_GC_CHAR_TRANSFORM(char *, const int, const int);
G	int	MK_GetPacket_GC_MANAGER_CHAT(char *,  const int, const char *);

#undef	G
#undef	_MK_PACKET_H_

