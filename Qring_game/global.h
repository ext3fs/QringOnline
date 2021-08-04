/*____________________________________________________________

	Project name : Qring Online
	File name    : global.h
	Description  : global value 
	Date         : February 14 2005
_____________________________________________________________*/



//____________________________________________________________
//billing
#define	BILLING_GAME_CODE			"Qr"

//____________________________________________________________
//paran
#define	PARAN_GAME_CODE				601


//_____________________________________________________________
//port
/*
#define	LOBBY_PORT				(1818)
#define	GAME_PORT				(1828)
#define	MOB_PORT				(2828)
#define	SCRIPT_PORT				(1819)
#define	BILLING_PORT				(1818)
#define	DBAGENT_PORT				(1820)
*/

#define	LOBBY_PORT				(7024)
#define	GAME_PORT				(7024)
#define	MOB_PORT				(8024)
#define	SCRIPT_PORT				(7024)
#define	BILLING_PORT				(7024)
#define	DBAGENT_PORT				(7024)


//_____________________________________________________________
//socket
#define	MAX_FD					(4096)
#define	SOCKET_BUF_SIZE				(8192*20)
#define	APP_BUF_SIZE				(8192*20)


//_____________________________________________________________
//sizeof
#define	SIZEOF_SHORT				(2)
#define	SIZEOF_INT				(4)
#define	SIZEOF_HEADER_BILLING			(8)
#define	SIZEOF_HEADER_NORMAL			(8)
#define	SIZEOF_HEADER_SPECIAL			(16)
#define	SIZEOF_MAX_GAME_PACKET_BODY		(8192-SIZEOF_HEADER_NORMAL)
#define	SIZEOF_MAX_SERV_PACKET_BODY		(8192*2-SIZEOF_HEADER_SPECIAL)


//_____________________________________________________________
//tick
#define	SEC_HEARTBEAT				(300)
#define	SEC_RECOVER				(2)


//_____________________________________________________________
//queue
#define	QUEUE_CNT				(30000)


//_____________________________________________________________
//rand
#define	RAND_CNT				(30000)


//_____________________________________________________________
//channel
#define	CHANNEL_PER_MACHINE			(6)
#define	USER_PER_CHANNEL			(500)
#define	MAX_MACHINE				(999/CHANNEL_PER_MACHINE)


//_____________________________________________________________
//connection
#define	CONNECTION_BILLING			(1)
#define	CONNECTION_DBAGENT			(1)
#define	CONNECTION_LOGIN_DBAGENT		(1)
#define	MAX_CONNECTION				(CHANNEL_PER_MACHINE*USER_PER_CHANNEL+CHANNEL_PER_MACHINE+CONNECTION_BILLING+CONNECTION_LOGIN_DBAGENT+CONNECTION_DBAGENT)


//_____________________________________________________________
//script
#define	MAX_SCRIPT_FILE_CNT			(32)


//_____________________________________________________________
//item
#define	ITEM_TYPE_MONEY				(1)

#define	MAX_DROP_ITEM				(30000)
#define	MAX_ITEM_DROP_TIME			(20)

#define	MAX_ENCHANT_CNT				(3)
#define	MAX_ENCHANT_GRADE			(9)
#define	MAX_ENCHANT_EQUIP_GROUP			(16)
#define	MAX_ENCHANT_OPTION_GROUP		(10)

#define	MAX_ITEM_CLASS				(34)
#define	ITEM_CLASS_MONEY			(1)
#define	ITEM_CLASS_ONEHAND_SWORD		(2)
#define	ITEM_CLASS_TWOHAND_SWORD		(3)
#define	ITEM_CLASS_BOW				(4)
#define	ITEM_CLASS_ARROW			(5)
#define	ITEM_CLASS_HAT				(6)
#define	ITEM_CLASS_JACKET			(7)
#define	ITEM_CLASS_PANTS			(8)
#define	ITEM_CLASS_SHOES			(9)
#define	ITEM_CLASS_SHIELD			(10)
#define	ITEM_CLASS_EARRING			(11)
#define	ITEM_CLASS_LEFT_RING			(12)
#define	ITEM_CLASS_RIGHT_RING			(13)
#define	ITEM_CLASS_QUEST			(14)
#define	ITEM_CLASS_RESTORE_POTION		(15)
//#define	ITEM_CLASS_BUFF_POTION			(16)
#define	ITEM_CLASS_PORTAL			(17)
#define	ITEM_CLASS_RESURRECTION			(18)
//#define	ITEM_CLASS_MATERIAL			(19)
#define	ITEM_CLASS_ENCHANT			(20)
#define	ITEM_CLASS_TRANSFORM_BROW_CRYSTAL	(21)
#define	ITEM_CLASS_CASH_COSTUME_ONEHAND_SWORD	(22)
#define	ITEM_CLASS_CASH_COSTUME_TWOHAND_SWORD	(23)
#define	ITEM_CLASS_CASH_COSTUME_BOW		(24)
#define	ITEM_CLASS_CASH_COSTUME_SHIELD		(25)
#define	ITEM_CLASS_CASH_COSTUME_HAT		(26)
#define	ITEM_CLASS_CASH_COSTUME_FACE		(27)
#define	ITEM_CLASS_CASH_COSTUME_JACKET		(28)
#define	ITEM_CLASS_CASH_COSTUME_PANTS		(29)
#define	ITEM_CLASS_CASH_COSTUME_ONEPIECE	(30)
#define	ITEM_CLASS_CASH_COSTUME_CLOAK		(31)
#define	ITEM_CLASS_CASH_COSTUME_SHOES		(32)
#define	ITEM_CLASS_CASH_COSTUME_NECKLACE	(33)
#define	ITEM_CLASS_TRANSFORM_EYE_CRYSTAL	(34)


#define	MAX_ENCHANT_OPTION			(17)
#define	ENCHANT_OPTION_ATT1			(1)
#define	ENCHANT_OPTION_ATT2			(2)
#define	ENCHANT_OPTION_CRI			(3)
#define	ENCHANT_OPTION_I_ATT			(4)
#define	ENCHANT_OPTION_ATT_LAND			(5)
#define	ENCHANT_OPTION_ATT_WATER		(6)
#define	ENCHANT_OPTION_ATT_FIRE			(7)
#define	ENCHANT_OPTION_ATT_WIND			(8)	
#define	ENCHANT_OPTION_DEF1			(9)
#define	ENCHANT_OPTION_DEF2			(10)
#define	ENCHANT_OPTION_EVA			(11)
#define	ENCHANT_OPTION_IL_DEF			(12)
#define	ENCHANT_OPTION_IS_DEF			(13)
#define	ENCHANT_OPTION_HP_MAX			(14)
#define	ENCHANT_OPTION_AP_MAX			(15)
#define	ENCHANT_OPTION_RECOVER_HP		(16)
#define	ENCHANT_OPTION_RECOVER_AP		(17)


//_____________________________________________________________
//npc type
#define	NPC_TYPE_MOB				(0)
#define	NPC_TYPE_STORE				(1)
#define	NPC_TYPE_WARP				(2)
#define	NPC_TYPE_NPC				(3)


//_____________________________________________________________
//store
#define	STORE_ITEM_SLOT_CNT			(120)


//_____________________________________________________________
//map
#define	MAP_X_TILE				(1024)
#define	MAP_Z_TILE				(128)	
#define	VIEW_BLOCK_TILE				(32)

#if (MAP_X_TILE % VIEW_BLOCK_TILE) == 0
	#define	VIEW_BLOCK_CNT	(MAP_X_TILE/VIEW_BLOCK_TILE + 4)
#else
	#define	VIEW_BLOCK_CNT	(MAP_X_TILE/VIEW_BLOCK_TILE + 5)
#endif	

#define	MAP_ATTR_0STAIR				(0)
#define	MAP_ATTR_1STAIR				(1)
#define	MAP_ATTR_2STAIR				(2)
#define	MAP_ATTR_MOB_COLLISION			(127)
#define	MAP_ATTR_COLLISION			(-1)


//_____________________________________________________________
//char
#define	MAX_CHAR_SEX				(1)
#define	MAX_CHAR_HAIR				(0)
#define	MAX_CHAR_FACE				(0)

#define	BASE_CHAR_AREA_ATTACK			(1)
#define	BASE_CHAR_MILLI_DELAY_ATTACK		(10000)

			
//_____________________________________________________________
//cool time
#define	MAX_COOL_SERIES				(20)


//_____________________________________________________________
//attack 
#define	CHAR_ATTACK_MOB				(0)
#define	MOB_ATTACK_CHAR				(1)


//_____________________________________________________________
//skill
#define	MAX_SKILL_ORDER_CNT			(3)
#define	MAX_SKILL_FUNCTION			(156)
#define	MAX_SKILL_TARGET_CNT			(10)


//_____________________________________________________________
//tarnsform
#define	MAX_TRANSFORM_TYPE			(4)
#define	TRANSFORM_SKILL_CNT			(15)


//_____________________________________________________________
//dir
#define	DIR_S					(0)
#define	DIR_SE					(45)
#define	DIR_E					(90)
#define	DIR_NE					(135)
#define	DIR_N					(180)
#define	DIR_NW					(225)
#define	DIR_W					(270)
#define	DIR_SW					(315)
#define	DIR_NOTING				(360)


//_____________________________________________________________
//send type
#define	SEND_EXIT_THREAD			(1)
#define	SEND_INTER_CONN_FD			(2)
#define	SEND_INTER_CLOSE_FD			(3)
#define	SEND_INTER_BLOCK_INSERT_CHAR		(4)
#define	SEND_INTER_BLOCK_DELETE_CHAR		(5)
#define	SEND_LOGIN				(6)
#define	SEND_UNICAST				(7)
#define	SEND_BROADCAST				(8)
#define	SEND_BLOCK_ALLCAST			(9)
#define	SEND_BLOCK_OTHERCAST			(10)


//_____________________________________________________________
//include
#define _REENTRANT

#include	<errno.h>
#include	<stdio.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/time.h>
#include	<sys/socket.h>
#include	<string.h>
#include	<pthread.h>
#include	<netinet/in.h>
#include	<signal.h>
#include	<pthread.h>
#include	<stdarg.h>
#include	<arpa/inet.h>
#include	<sys/epoll.h>
#include	<sys/resource.h>
#include	<fcntl.h>
#define	__USE_ISOC99
#include	<stdlib.h>
#define	__USE_XOPEN
#include	<poll.h>

#include	"header.h"


struct	send_buf
{
	pthread_mutex_t	mutex;
	int		active;
	int		login;
	int		use_billing_idx;
	int		block_next;
	int		*block_prev;
	int		size;
	char		buf[APP_BUF_SIZE];
};  

struct	recv_q
{
	int	fd;
	int	fd_or_seq;
	int	command;
	int	sec_active;
	int	session;
	int	data_size;
	char	data[SIZEOF_MAX_SERV_PACKET_BODY];	
	int	recv_cnt;
	int	next;
};

struct	send_q
{
	int	send_type;
	int	fd;
	int	channel_link;
	int	map;
	int	block_x;
	int	data_size;
	char	data[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	int	next;
};

struct	skill_action_result
{
	int	buff_cnt;
	int	buff_serise[MAX_SKILL_ORDER_CNT];
	int	buff_char_cnt;
	int	buff_char_list[MAX_SKILL_TARGET_CNT];
	int	buff_mob_cnt;
	int	buff_mob_list[MAX_SKILL_TARGET_CNT];
	int	dead_char_cnt;
	int	dead_char_list[MAX_SKILL_TARGET_CNT];
};


extern	pid_t				G_Pid;
extern	volatile	sig_atomic_t	G_Run;
extern	volatile	time_t		G_Sec;
extern	int				G_Game;
extern	int				G_World;
extern	int				G_Machine;
extern	int				G_Server_code;
extern	int				G_Demon;
extern	char				G_Script_ip[SIZEOF_IP+1];
extern	char				G_Billing_ip[SIZEOF_IP+1];
extern	char				G_Login_dbagent_ip[SIZEOF_IP+1];
extern	int				G_Epoll_handler;
extern	int				G_Unix_close_fd[2];
extern	int				G_Unix_live_fd[2];
extern	int				G_Unix_dbagent_idx[2];
extern	int				G_Unix_billing_idx[2];
extern	int				G_Unix_login_dbagent_idx[2];
extern	int				G_Unix_tick[2];
extern	int				G_Udp_fd;
extern	int				G_Listen_game_fd;
extern	int				G_Listen_mob_fd;
extern	volatile	int		G_Connection_number;
extern	volatile	int		G_Login_number;
extern	struct	send_buf		G_Send_buf[MAX_FD+1];
extern	void				*G_Buff_memory;
extern	void				*G_Tick_mob_memory;
