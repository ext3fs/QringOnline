/*_______________________________________________________

	Project name : Qring Online
	File name    : header.h
	Description  : Packet header type
	Date         : February 14 2005
_________________________________________________________*/

//version
#define	VERSION			"20041216"

//item slot
#define	EQUIP_SLOT_CNT			(21)
#define	ITEM_SLOT_CNT			(EQUIP_SLOT_CNT + 200)

#define	EQUIP_SLOT_HEAD			(0)
#define	EQUIP_SLOT_COAT			(1)
#define	EQUIP_SLOT_PANTS		(2)
#define	EQUIP_SLOT_SHOES		(3)
#define	EQUIP_SLOT_WEAPON1		(4)
#define	EQUIP_SLOT_WEAPON2		(5)
#define	EQUIP_SLOT_CLOAK		(6)
#define	EQUIP_SLOT_ACCESSORY1		(7)
#define	EQUIP_SLOT_ACCESSORY2		(8)
#define	EQUIP_SLOT_EARRINGS		(9)
#define	EQUIP_SLOT_RING1		(10)
#define	EQUIP_SLOT_RING2		(11)
#define	EQUIP_SLOT_HEAD_COSTUME		(12)
#define	EQUIP_SLOT_COAT_COSTUME		(13)
#define	EQUIP_SLOT_PANTS_COSTUME	(14)
#define	EQUIP_SLOT_SHOES_COSTUME	(15)
#define	EQUIP_SLOT_WEAPON1_COSTUME	(16)
#define	EQUIP_SLOT_WEAPON2_COSTUME	(17)
#define	EQUIP_SLOT_CLOAK_COSTUME	(18)
#define	EQUIP_SLOT_FACE_COSTUME		(19)
#define	EQUIP_SLOT_NECK_COSTUME		(20)

//len
#define	SIZEOF_IP			(15)
#define	VERSION_LEN			(16)
#define	USER_ID_LEN			(16)
#define	CHAR_NAME_LEN			(16)
#define	WORLD_NAME_LEN			(16)
#define	ITEM_DATA_LEN			(16)
#define	ITEM_SERIAL_LEN			(49)
#define	ENCODEDSTR_LEN			(36)
#define	CHANNEL_NAME_LEN		(WORLD_NAME_LEN+4)


//char status
#define	_USER_NORMAL_WAIT1		(0)
#define	_USER_NORMAL_WAIT2		(1)
#define	_USER_ATTACK_WAIT		(2)
#define	_USER_WALK			(3)
#define	_USER_RUN			(4)
#define	_USER_JUMP_FALL			(5)
#define	_USER_JUMP1			(6)
#define	_USER_JUMP2			(7)
#define	_USER_JUMP3			(8)
#define	_USER_DOUBLE_JUMP1		(9)
#define	_USER_DOUBLE_JUMP2		(10)
#define	_USER_DOUBLE_JUMP3		(11)
#define	_USER_READY_ATTACK		(12)
#define	_USER_JUMP_ATTACK		(13)
#define	_USER_STAND_ATTACK1		(14)
#define	_USER_STAND_ATTACK2		(15)
#define	_USER_STAND_ATTACK3		(16)
#define	_USER_DASH_ATTACK		(17)
#define	_USER_CHANGE			(18)
#define	_USER_STUN			(19)
#define	_USER_DEMAGE			(20)
#define	_USER_DEAD			(21)
#define	_USER_DEAD_REGEN		(22)
#define	_USER_REGEN			(23)
#define	_USER_EXTINCT			(24)

//mob status
#define	_MOB_WAIT1			(0)
#define	_MOB_WAIT2			(1)
#define	_MOB_WALK			(2)
#define	_MOB_RUN			(3)
#define	_MOB_QUICK_RUN			(4)
#define	_MOB_ATTACK			(5)
#define	_MOB_IMPACT			(6)
#define	_MOB_DEATH			(7)
#define	_MOB_REGEN			(8)
#define	_MOB_EXTINCT			(9)

//mob summon
#define	MAP_SUMMON_MOB_CNT	(500)

//attack type
#define	_ATTACK_JUMP		(0)
#define	_ATTACK_NORMAL		(1)
#define	_ATTACK_COMBO1		(2)
#define	_ATTACK_COMBO2		(3)
#define	_ATTACK_PIERCE		(4)

//damage type (bitmask)	
#define	_DAMAGE_NOTING		(0x00)
#define	_DAMAGE_MISS		(0x01)
#define	_DAMAGE_NORMAL		(0x02)
#define	_DAMAGE_CRITICAL	(0x04)
#define	_DAMAGE_IMPACT		(0x08)
#define	_DAMAGE_PROVOCATION	(0x10)
#define	_DAMAGE_HP_HEAL		(0x20)
#define	_DAMAGE_STUN		(0x40)
#define	_DAMAGE_SLEEP		(0x80)

//skill
#define	CHAR_SKILL_CNT		(48)

//buff 
#define	BUFF_SERIES_CNT			(27)
#define	PASSIVE_BUFF_CNT		(4)
#define	EQUIP_PASSIVE_BUFF_CNT		(7)
#define	TRANSFORM_PASSIVE_BUFF_CNT	(3)
#define	TIMELESS_BUFF_CNT		(13)

#define	_BUFF_PASSIVE_HP_RECOVER	(0)
#define	_BUFF_PASSIVE_AP_RECOVER	(1)
#define	_BUFF_PASSIVE_HP_MAX		(2)
#define	_BUFF_PASSIVE_AP_MAX		(3)
#define _BUFF_EQUIP_PASSIVE_CRI		(4)
#define	_BUFF_EQUIP_PASSIVE_I_ATT	(5)
#define	_BUFF_EQUIP_PASSIVE_IS_DEF	(6)
#define	_BUFF_EQUIP_PASSIVE_IL_DEF	(7)
#define	_BUFF_EQUIP_PASSIVE_ATT_SPEED	(8)
#define	_BUFF_EQUIP_PASSIVE_ATT_AREA	(9)
#define	_BUFF_EQUIP_PASSIVE_COND_COMBO2	(10)
#define	_BUFF_TRANSFORM_PASSIVE_ATT2	(11)
#define	_BUFF_TRANSFORM_PASSIVE_HP_MAX	(12)
#define	_BUFF_TRANSFORM_PASSIVE_EVA	(13)
#define	_BUFF_POTION_RECOVER_HP		(14)
#define	_BUFF_POTION_RECOVER_AP		(15)
#define	_BUFF_ACTIVE_ATT1		(16)
#define	_BUFF_ACTIVE_DEF1		(17)
#define	_BUFF_ACTIVE_MAGIC_DEF		(18)
#define	_BUFF_ACTIVE_MOVE_SPEED		(19)
#define	_BUFF_DE_ACTIVE_DEF1		(20)
#define	_BUFF_DE_ACTIVE_MOVE_SPEED	(21)
#define	_BUFF_DE_ACTIVE_ATT_SPEED	(22)
#define	_BUFF_DE_ACTIVE_STUN		(23)
#define	_BUFF_DE_ACTIVE_SLEEP		(24)
#define	_BUFF_DE_ACTIVE_POISONING	(25)
#define	_BUFF_DE_ACTIVE_FIREPOISONING	(26)


#define _WB			(0x01000000)
#define	_BW			(0x02000000)		

#define	_INTER			(0x03000000)

#define _TW			(0x04000000)
#define	_WT			(0x05000000)		

#define _WS			(0x06000000)
#define	_SW			(0x07000000)		

#define _WA			(0x08000000)
#define	_AW			(0x09000000)		

#define _MG			(0x0a000000)
#define	_GM			(0x0b000000)		

#define _CL			(0x0c000000)
#define	_LC			(0x0d000000)		

#define _CG			(0x0e000000)
#define	_GC			(0x0f000000)		



//_______________________________________________________
//server trouble
#define	SERVER_TROUBLE_FULL		(0)
#define	SERVER_TROUBLE_BILLING		(1)
#define	SERVER_TROUBLE_INSPECT_TIME	(2)


//________________________________________________________
//result code
#define	RESULT_SUCC			(0)
#define	RESULT_FAIL_VERSION		(1)
//#define	RESULT_FAIL_BILLING		(2)
#define	RESULT_FAIL_DBAGENT		(3)
#define	RESULT_FAIL_DB			(4)
#define	RESULT_FAIL_USERID_OR_PW	(5)
#define	RESULT_FAIL_DUP_LOGIN		(6)
#define	RESULT_FAIL_DUP_CHAR_NAME	(7)
#define	RESULT_FAIL_DATA		(8)
#define	RESULT_FAIL_EXTINCT_ITEM	(9)
//#define	RESULT_FAIL_PAY_INFO		(10)
//#define	RESULT_FAIL_LOGIN		(11)
//#define	RESULT_FAIL_BLOCK_USERID	(12)
//#define	RESULT_FAIL_SQL			(13)
//#define	RESULT_FAIL_OTHER_GAME		(14)
#define	RESULT_FAIL_EXTINCT_MOB		(15)
#define	RESULT_FAIL_CHANNEL_FULL	(16)
#define	RESULT_FAIL_CHANNEL_RECONNECT	(17)
#define	RESULT_FAIL_ENCODEDSTR		(18)


/*
//_________________________________________________________
//xxxx to billing
#define	WB_GAMESTART			(0x6006)
#define	WB_GAMESTOP			(0x6009)
#define	WB_USER_CHECK_SUCC		(0x6013)
#define	WB_USER_CHECK_FAIL		(0x6014)
#define	WB_REGSERVER			(0x601a)


//_________________________________________________________
//billing to xxxx
#define	BW_GAMESTART_SUCC		(0x6007)
#define	BW_GAMESTART_FAIL		(0x6008)
#define	BW_GAMESTOP_SUCC		(0x600a)
#define	BW_GAMESTOP_FAIL		(0x600b)
#define	BW_USER_CHECK			(0x6012)
#define	BW_DATABASE_ERROR		(0x6017)
#define	BW_SYSTEM_ERROR			(0x6018)
#define	BW_REGSERVER_SUCC		(0x601b)
#define	BW_REGSERVER_FAIL		(0x601c)
#define	BW_ALIVECHECK			(0xffff)
*/

//_________________________________________________________
//inter thread 
#define	INTER_EXIT_THREAD		(_INTER + 0x01)
#define	INTER_CONN_BILLING_FD		(_INTER + 0x02)
#define	INTER_CONN_LOGIN_DBAGENT_FD	(_INTER + 0x03)
#define	INTER_CONN_DBAGENT_FD		(_INTER + 0x04)
#define	INTER_CONN_MOBSERV_FD		(_INTER + 0x05)
#define	INTER_CONN_CLIENT_FD		(_INTER + 0x06)
#define	INTER_CLOSE_FD			(_INTER + 0x07)
#define	INTER_CONN_FD			(_INTER + 0x08)
#define INTER_CONN_FD_SUCC		(_INTER + 0x09)
#define INTER_CONN_FD_RUNNING		(_INTER + 0x0a)
#define INTER_CONN_FD_FAIL		(_INTER + 0x0b)
#define	INTER_CHAR_RECOVER		(_INTER + 0x0c)			
#define	INTER_MOB_RECOVER		(_INTER + 0x0d)			
#define	INTER_ITEM_REMOVE		(_INTER + 0x0e)
#define	INTER_CHAR_BUFF_END		(_INTER + 0x0f)
#define	INTER_MOB_BUFF_END		(_INTER + 0x10)
#define	INTER_CHANNEL_LIVE		(_INTER + 0x11)
#define	INTER_TICK			(_INTER + 0x12)


//_________________________________________________________
//tool to xxxx
#define	TW_WAKEUP			(_TW + 0x01)	
#define	TW_LIVE				(_TW + 0x02)	
#define	TW_KILL				(_TW + 0x03)	
#define	TW_QUEUE			(_TW + 0x04)


//_________________________________________________________
//xxxx to script
#define	WS_INIT				(_WS + 0x01)
#define	WS_MYINFO			(_WS + 0x02)
#define	WS_WORLD_INFO			(_WS + 0x03)
#define	WS_GAMESERV_INFO		(_WS + 0x04)
#define	WS_DBAGENT_INFO			(_WS + 0x05)
#define	WS_MOBSERV_INFO			(_WS + 0x06)
#define	WS_MAP_FILES			(_WS + 0x07)
#define	WS_MOB_FILES			(_WS + 0x08)
#define	WS_TYPE_MOB_FILES		(_WS + 0x09)
#define	WS_LEVEL_FILES			(_WS + 0x0a)
#define	WS_TYPE_ITEM_FILES		(_WS + 0x0b)
#define	WS_TYPE_DROP_FILES		(_WS + 0x0c)
#define	WS_TYPE_WARP_FILES		(_WS + 0x0d)
#define	WS_TYPE_STORE_FILES		(_WS + 0x0e)
#define	WS_TYPE_SKILL_FILES		(_WS + 0x0f)
#define	WS_TYPE_MOB_SKILL_FILES		(_WS + 0x10)


//_________________________________________________________
//script to xxxx
#define	SW_INIT_RESULT			(_SW + 0x01)
#define	SW_MYINFO_RESULT		(_SW + 0x02)
#define	SW_WORLD_INFO_RESULT		(_SW + 0x03)
#define	SW_GAMESERV_INFO_RESULT		(_SW + 0x04)
#define	SW_DBAGENT_INFO_RESULT		(_SW + 0x05)
#define	SW_MOBSERV_INFO_RESULT		(_SW + 0x06)
#define	SW_MAP_FILES_RESULT		(_SW + 0x07)
#define	SW_MOB_FILES_RESULT		(_SW + 0x08)
#define	SW_TYPE_MOB_FILES_RESULT	(_SW + 0x09)
#define	SW_LEVEL_FILES_RESULT		(_SW + 0x0a)
#define	SW_TYPE_ITEM_FILES_RESULT	(_SW + 0x0b)
#define	SW_TYPE_DROP_FILES_RESULT	(_SW + 0x0c)
#define	SW_TYPE_WARP_FILES_RESULT	(_SW + 0x0d)
#define	SW_TYPE_STORE_FILES_RESULT	(_SW + 0x0e)
#define	SW_TYPE_SKILL_FILES_RESULT	(_SW + 0x0f)
#define	SW_TYPE_MOB_SKILL_FILES_RESULT	(_SW + 0x10)


//_________________________________________________________
//xxxx to dbagent
#define	WA_INIT				(_WA + 0x01)
#define	WA_LIVE				(_WA + 0x02)
#define	WA_CHAR_LIST			(_WA + 0x03)
#define	WA_CHAR_CREATE			(_WA + 0x04)
#define	WA_CHAR_DELETE			(_WA + 0x05)
#define	WA_CHAR_LOAD			(_WA + 0x06)
#define	WA_CHAR_SAVE			(_WA + 0x07)
#define	WA_LOGIN			(_WA + 0x08)
#define	WA_LOGOUT			(_WA + 0x09)
#define	WA_CURRENT_USER			(_WA + 0x0a)
#define	WA_GAME_LOG			(_WA + 0x0b)
#define	WA_GET_USER			(_WA + 0x0c)


//_________________________________________________________
//dbagent to xxxx
#define	AW_INIT_RESULT			(_AW + 0x01)
#define	AW_LIVE				(_AW + 0x02)
#define	AW_CHAR_LIST_RESULT		(_AW + 0x03)
#define	AW_CHAR_CREATE_RESULT		(_AW + 0x04)
#define	AW_CHAR_DELETE_RESULT		(_AW + 0x05)
#define	AW_CHAR_LOAD_RESULT		(_AW + 0x06)
#define	AW_LOGIN_RESULT			(_AW + 0x07)
#define	AW_GET_USER_RESULT		(_AW + 0x08)


//_________________________________________________________
//mob to game
#define	MG_INIT				(_MG + 0x01)
#define	MG_LIVE				(_MG + 0x02)
#define	MG_MOB_REGEN			(_MG + 0x03)
#define	MG_MOB_EXTINCT			(_MG + 0x04)
#define	MG_MOB_MOVE			(_MG + 0x05)
#define	MG_MOB_ATTACK_CHAR		(_MG + 0x06)
#define	MG_MOB_DAMAGE_LIST		(_MG + 0x07)
#define	MG_MOB_PERFECT_RECOVER		(_MG + 0x08)
#define	MG_MOB_SKILL_READY		(_MG + 0x09)
#define	MG_MOB_SKILL_CANCEL		(_MG + 0x0a)
#define	MG_MOB_SKILL_ACTION		(_MG + 0x0b)


//_________________________________________________________
//game to mob
#define	GM_INIT_RESULT			(_GM + 0x01)
#define	GM_LIVE				(_GM + 0x02)
#define	GM_CHAR_CHANNEL_IN		(_GM + 0x03)
#define	GM_CHAR_CHANNEL_OUT		(_GM + 0x04)
#define	GM_CHAR_MAP_CHANGE		(_GM + 0x05)
#define	GM_CHAR_MOVE			(_GM + 0x06)
#define	GM_CHAR_ATTACK_MOB		(_GM + 0x07)
#define	GM_MOB_SUMMON			(_GM + 0x08)
#define	GM_CHAR_SKILL_ACTION		(_GM + 0x09)
#define	GM_MOB_BUFF			(_GM + 0x0a)


//_________________________________________________________
//client to login
#define	CL_LOGIN			(_CL + 0x01)
#define	CL_LIVE				(_CL + 0x02)
#define	CL_WORLD_LIST			(_CL + 0x03)	
#define	CL_CHANNEL_LIST			(_CL + 0x04)
#define	CL_CHAR_LIST			(_CL + 0x05)
#define	CL_CHAR_CREATE			(_CL + 0x06)
#define	CL_CHAR_DELETE			(_CL + 0x07)
#define	CL_CHANNEL_CONNECT		(_CL + 0x08)


//_________________________________________________________
//login to client
#define	LC_SERVER_TROUBLE		(_LC + 0x01)
#define	LC_LOGIN_RESULT			(_LC + 0x02)
#define	LC_LIVE				(_LC + 0x03)
#define	LC_WORLD_LIST_RESULT		(_LC + 0x04)	
#define	LC_CHANNEL_LIST_RESULT		(_LC + 0x05)
#define	LC_CHAR_LIST_RESULT		(_LC + 0x06)
#define	LC_CHAR_CREATE_RESULT		(_LC + 0x07)
#define	LC_CHAR_DELETE_RESULT		(_LC + 0x08)
#define	LC_CHANNEL_CONNECT_RESULT	(_LC + 0x09)


//_________________________________________________________
//client to game
#define	CG_LOGIN			(_CG + 0x01)
#define	CG_LIVE				(_CG + 0x02)
#define	CG_CHANNEL_LIST			(_CG + 0x03)
#define	CG_CHAR_CHANNEL_MOVE		(_CG + 0x04)
#define	CG_CHAR_LOAD			(_CG + 0x05)
#define	CG_CHAR_LOAD_END		(_CG + 0x06)
#define	CG_CHAR_MOVE			(_CG + 0x07)
#define	CG_CHAR_CHAT			(_CG + 0x08)
#define	CG_CHAR_ATTACK_MOB		(_CG + 0x09)
#define	CG_CHAR_REGEN			(_CG + 0x0a)
#define	CG_CHAR_ITEM_MOVE		(_CG + 0x0b)
#define	CG_CHAR_ITEM_PICKUP		(_CG + 0x0c)
#define	CG_CHAR_ITEM_DROP		(_CG + 0x0d)
#define	CG_CHAR_ITEM_ENCHANT		(_CG + 0x0e)
#define	CG_CHAR_ITEM_USE		(_CG + 0x0f)
#define	CG_CHAR_WARP			(_CG + 0x10)
#define	CG_CHAR_STORE_ITEM_BUY		(_CG + 0x11)
#define	CG_CHAR_STORE_ITEM_SELL		(_CG + 0x12)
#define	CG_CHAR_SKILL_READY		(_CG + 0x13)
#define	CG_CHAR_SKILL_ACTION		(_CG + 0x14)
#define	CG_CHAR_SKILL_CANCEL		(_CG + 0x15)
#define	CG_CHAR_SKILL_LEARN		(_CG + 0x16)
#define	CG_CHAR_TRANSFORM		(_CG + 0x17)
#define	CG_MANAGER_CHAT			(_CG + 0x18)


//_________________________________________________________
//game to client
#define	GC_SERVER_TROUBLE		(_GC + 0x01)
#define	GC_LOGIN_RESULT			(_GC + 0x02)
#define	GC_LIVE				(_GC + 0x03)
#define	GC_CHANNEL_LIST_RESULT		(_GC + 0x04)
#define	GC_CHAR_CHANNEL_MOVE_RESULT	(_GC + 0x05)
#define	GC_CHAR_LOAD_RESULT		(_GC + 0x06)
#define	GC_OTHER_CHAR_APPEAR		(_GC + 0x07)
#define	GC_OTHER_CHAR_DISAPPEAR		(_GC + 0x08)
#define	GC_CHAR_MOVE			(_GC + 0x09)
#define	GC_CHAR_CHAT			(_GC + 0x0a)
#define	GC_MOB_APPEAR			(_GC + 0x0b)
#define	GC_MOB_DISAPPEAR		(_GC + 0x0c)
#define	GC_MOB_MOVE			(_GC + 0x0d)
#define	GC_CHAR_ATTACK_MOB		(_GC + 0x0e)
#define	GC_MOB_ATTACK_CHAR		(_GC + 0x0f)
#define GC_CHAR_ACQUIRE_EXP		(_GC + 0x10)
#define	GC_CHAR_LEVEL_UP		(_GC + 0x11)
#define	GC_CHAR_REGEN_RESULT		(_GC + 0x12)
#define	GC_ITEM_APPEAR			(_GC + 0x13)
#define	GC_ITEM_DISAPPEAR		(_GC + 0x14)
#define	GC_CHAR_ITEM_SLOT		(_GC + 0x15)
#define	GC_CHAR_ITEM_PICKUP_RESULT	(_GC + 0x16)
#define	GC_CHAR_ITEM_ENCHANT_RESULT	(_GC + 0x17)
#define	GC_CHAR_BUFF			(_GC + 0x18)
#define	GC_CHAR_WARP_RESULT		(_GC + 0x19)
#define	GC_CHAR_STORE_ITEM_BUY_RESULT	(_GC + 0x1a)
#define	GC_CHAR_STORE_ITEM_SELL_RESULT	(_GC + 0x1b)
#define	GC_CHAR_RECOVER			(_GC + 0x1c)
#define	GC_CHAR_ITEM_USE_RESULT		(_GC + 0x1d)
#define	GC_CHAR_SKILL_READY		(_GC + 0x1e)
#define	GC_CHAR_SKILL_ACTION		(_GC + 0x1f)
#define	GC_CHAR_SKILL_CANCEL		(_GC + 0x20)
#define	GC_MOB_SKILL_READY		(_GC + 0x21)
#define	GC_MOB_SKILL_ACTION		(_GC + 0x22)
#define	GC_MOB_SKILL_CANCEL		(_GC + 0x23)
#define	GC_CHAR_TRANSFORM		(_GC + 0x24)
#define	GC_MANAGER_CHAT			(_GC + 0x25)


#pragma	pack(1)
//_______________________________________________________
//header
//struct	header_billing
//{
//	int		total_size;
//	int		command;
//};

struct	header_special
{
	unsigned	short	body_size;
	unsigned	short	fd;
	int			command;
	int			sec_active;
	int			session;
};

struct	header_normal
{
	unsigned 	short	body_size;
	unsigned 	short	seq;
	int			command;
};

/*
//_________________________________________________________
//xxxx to billing
struct	body_WB_REGSERVER
{
	int	server_idx;
	char	game_code[3];
};

struct	body_WB_GAMESTART
{
	int	id;
	char	user_id[USER_ID_LEN+1];
	char	game_code[3];
	char	user_ip[32];
};

struct	body_WB_GAMESTOP
{
	int	id;
	char	user_id[USER_ID_LEN+1];
	char	game_code[3];
	char	user_ip[32];
};

struct	body_WB_USER_CHECK_SUCC
{
	int	id;
	char	user_id[USER_ID_LEN+1];
};

struct	body_WB_USER_CHECK_FAIL
{
	int	id;
	char	user_id[USER_ID_LEN+1];
};


//_________________________________________________________
//billing to xxxx
struct	body_BW_REGSERVER_SUCC
{
	//noting
};

struct	body_BW_REGSERVER_FAIL
{
	//noting	
};

struct	body_BW_GAMESTART_SUCC
{
	int	id;
	char	user_id[USER_ID_LEN+1];
	int	result;
	char	user_ip[32];
};

struct	body_BW_GAMESTART_FAIL
{
	int	id;
	char	user_id[USER_ID_LEN+1];
};

struct	body_BW_GAMESTOP_SUCC
{
	int	id;
	char	user_id[USER_ID_LEN+1];
};

struct	body_BW_GAMESTOP_FAIL
{
	int	id;
	char	user_id[USER_ID_LEN+1];
};

struct	body_BW_USER_CHECK
{
	int	id;
	char	user_id[USER_ID_LEN+1];
};

struct	body_BW_DATABASE_ERROR
{
	int	id;
};

struct	body_BW_SYSTEM_ERROR
{
	int	id;
};

struct	body_BW_ALIVECHECK
{
	//noting
};
*/

//_______________________________________________________
//tool to xxxx
struct	body_TW_WAKEUP
{
	//noting
};

struct	body_TW_LIVE
{
	//noting
};

struct	body_TW_KILL
{
	//noting
};

struct	body_TW_QUEUE
{
	//noting
};


//_________________________________________________________
//xxxx to script
struct	body_WS_INIT
{
	char	version[VERSION_LEN+1];
};

struct	body_WS_MYINFO
{
	//noting
};

struct	body_WS_WORLD_INFO
{
	int	game;
};

struct	body_WS_GAMESERV_INFO
{
	int	game;
	int	world;
};

struct	body_WS_DBAGENT_INFO
{
	int	game;
	int	world;
};

struct	body_WS_MOBSERV_INFO
{
	int	game;
	int	world;
};

struct	body_WS_MAP_FILES
{
	//noting
};

struct	body_WS_MOB_FILES
{
	//noting
};

struct	body_WS_TYPE_MOB_FILES
{
	//noting
};

struct	body_WS_LEVEL_FILES
{
	//noting
};

struct	body_WS_TYPE_ITEM_FILES
{
	//noting
};

struct	body_WS_TYPE_DROP_FILES
{
	//noting
};

struct	body_WS_TYPE_WARP_FILES
{
	//noting
};

struct	body_WS_TYPE_STORE_FILES
{
	//noting
};

struct	body_WS_TYPE_SKILL_FILES
{
	//noting
};

struct	body_WS_TYPE_MOB_SKILL_FILES
{
	//noting
};


//_________________________________________________________
//script to xxxx
struct	body_SW_INIT_RESULT
{
	int	result_code;
};

struct	body_SW_MYINFO_RESULT
{
	int	game;
	int	world;
};

struct	body1_SW_WORLD_INFO_RESULT
{
	int	info_cnt;
	//struct	body2_SW_WORLD_INFO_RESULT	info[info_cnt];
};

struct	body2_SW_WORLD_INFO_RESULT
{
	int	world;
	char	name[WORLD_NAME_LEN+1];
};

struct	body1_SW_GAMESERV_INFO_RESULT
{
	int	info_cnt;
	//struct	body2_SW_GAMESERV_INFO_RESULT	info[info_cnt];
};

struct	body2_SW_GAMESERV_INFO_RESULT
{
	char	ip[SIZEOF_IP+1];
	char	outer_ip[SIZEOF_IP+1];
};

struct	body1_SW_DBAGENT_INFO_RESULT
{
	int	info_cnt;
	//struct	body2_SW_DBAGENT_INFO_RESULT	info[info_cnt];
};

struct	body2_SW_DBAGENT_INFO_RESULT
{
	char	ip[SIZEOF_IP+1];
};

struct	body1_SW_MOBSERV_INFO_RESULT
{
	int	info_cnt;
	//struct	body2_SW_MOBSERV_INFO_RESULT	info[info_cnt];
};

struct	body2_SW_MOBSERV_INFO_RESULT
{
	char	ip[SIZEOF_IP+1];
};

struct	body1_SW_MAP_FILES_RESULT
{
	int	file_cnt;
	//struct body2_SW_MAP_FILES_RESULT files[file_cnt]	
};

struct	body2_SW_MAP_FILES_RESULT
{
	int	file_size;
	//char	file[file_size];
};

struct	body1_SW_MOB_FILES_RESULT
{
	int	file_cnt;
	//struct body2_SW_MOB_FILES_RESULT	files[file_cnt];
};

struct	body2_SW_MOB_FILES_RESULT
{
	int	file_size;
	//char	file[file_size];
};

struct	body1_SW_TYPE_MOB_FILES_RESULT
{
	int	file_cnt;
	//struct body2_SW_TYPE_MOB_FILES_RESULT	files[file_cnt];
};

struct	body2_SW_TYPE_MOB_FILES_RESULT
{
	int	file_size;
	//char	file[file_size];
};

struct	body1_SW_LEVEL_FILES_RESULT
{
	int	file_cnt;
	//struct body2_SW_LEVEL_FILES_RESULT	files[file_cnt];
};

struct	body2_SW_LEVEL_FILES_RESULT
{
	int	file_size;
	//char	file[file_size];
};

struct	body1_SW_TYPE_ITEM_FILES_RESULT
{
	int	file_cnt;
	//struct body2_SW_TYPE_ITEM_FILES_RESULT	files[file_cnt];
};

struct	body2_SW_TYPE_ITEM_FILES_RESULT
{
	int	file_size;
	//char	file[file_size];
};

struct	body1_SW_TYPE_DROP_FILES_RESULT
{
	int	file_cnt;
	//struct body2_SW_TYPE_DROP_FILES_RESULT	files[file_cnt];
};

struct	body2_SW_TYPE_DROP_FILES_RESULT
{
	int	file_size;
	//char	file[file_size];
};

struct	body1_SW_TYPE_WARP_FILES_RESULT
{
	int	file_cnt;
	//struct body2_SW_TYPE_WARP_FILES_RESULT	files[file_cnt];
};

struct	body2_SW_TYPE_WARP_FILES_RESULT
{
	int	file_size;
	//char	file[file_size];
};

struct	body1_SW_TYPE_STORE_FILES_RESULT
{
	int	file_cnt;
	//struct body2_SW_TYPE_STORE_FILES_RESULT	files[file_cnt];
};

struct	body2_SW_TYPE_STORE_FILES_RESULT
{
	int	file_size;
	//char	file[file_size];
};

struct	body1_SW_TYPE_SKILL_FILES_RESULT
{
	int	file_cnt;
	//struct body2_SW_TYPE_SKILL_FILES_RESULT	files[file_cnt];
};

struct	body2_SW_TYPE_SKILL_FILES_RESULT
{
	int	file_size;
	//char	file[file_size];
};

struct	body1_SW_TYPE_MOB_SKILL_FILES_RESULT
{
	int	file_cnt;
	//struct body2_SW_TYPE_MOB_SKILL_FILES_RESULT	files[file_cnt];
};

struct	body2_SW_TYPE_MOB_SKILL_FILES_RESULT
{
	int	file_size;
	//char	file[file_size];
};


//_________________________________________________________
//xxxx to agent
struct	body_WA_INIT
{
	char	version[VERSION_LEN+1];
	int	record_lock;
};

struct	body_WA_LIVE
{
	//noting
};


struct	body_WA_CHAR_LIST
{
	char	user_id[USER_ID_LEN+1];	
};

struct	body_WA_CHAR_CREATE
{
	char	user_id[USER_ID_LEN+1];
	char	char_name[CHAR_NAME_LEN+1];
	int	map;
	int	x;
	int	y;
	int	z;
	int	regen_map;
	int	regen_x;
	int	regen_y;
	int	regen_z;
	int	sex;
	int	hair;
	int	face;
	int	level;
	int	exp;
	int	hp;
	int	ap;
};

struct	body_WA_CHAR_DELETE
{
	char	user_id[USER_ID_LEN+1];	
	char	char_name[CHAR_NAME_LEN+1];
};


struct	body_WA_CHAR_LOAD
{
	char	user_id[USER_ID_LEN+1];	
	char	char_name[CHAR_NAME_LEN+1];
};

struct	body1_WA_CHAR_SAVE
{
	char	user_id[USER_ID_LEN+1];	
	char	char_name[CHAR_NAME_LEN+1];
	int	map;
	int	x;
	int	y;
	int	z;
	int	regen_map;
	int	regen_x;
	int	regen_y;
	int	regen_z;
	int	sex;
	int	hair;
	int	face;
	int	level;
	int	exp;
	int	hp;
	int	ap;
	int	money;	
	int	skill_point;	
	int	load_cnt;
	int	skill_slot[CHAR_SKILL_CNT];
	int	item_slot_cnt;
	//struct	body2_WA_CHAR_SAVE	item_slot[item_slot_cnt]
};

struct	body2_WA_CHAR_SAVE
{
	int	item_slot;
	char	item[ITEM_DATA_LEN+ITEM_SERIAL_LEN];
};	

struct	body_WA_LOGIN
{
	char	user_id[USER_ID_LEN+1];
	char	encodedstr[ENCODEDSTR_LEN+1];
	int	record_lock;
	
};

struct	body_WA_LOGOUT
{
	char	user_id[USER_ID_LEN+1];
	int	record_lock;
};

struct	body_WA_CURRENT_USER
{
	int	game_code;
	int	msvr_code;
	int	ssvr_code;
	char	channel_ip[SIZEOF_IP+1];
	int	channel_port;
	char	channel_name[CHANNEL_NAME_LEN+1];
	int	user_cnt;
};

struct	body_WA_GAME_LOG
{
	int	game_code;
	char	user_id[USER_ID_LEN+1];
	char	char_name[CHAR_NAME_LEN+1];
	char	channel_name[CHANNEL_NAME_LEN+1];
	char	user_ip[SIZEOF_IP+1];
	int	log_type;
	int	game_time;
	int	level;
	int	exp;
};

struct	body_WA_GET_USER
{
	int	msvr_code;
	int	ssvr_code;
};


//_________________________________________________________
//agent to xxxx
struct	body_AW_INIT_RESULT
{
	int	result_code;
};

struct	body_LIVE
{
	//noting
};

struct	body1_AW_CHAR_LIST_RESULT
{
	int	result_code;
	char	user_id[USER_ID_LEN+1];
	int	char_cnt;
	//struct	body2_AW_CHAR_LIST_RESULT	list[char_cnt];		
};

struct	body2_AW_CHAR_LIST_RESULT
{
	char	char_name[CHAR_NAME_LEN+1];
	int	sex;
	int	hair;
	int	face;
	int	level;
};

struct	body_AW_CHAR_CREATE_RESULT
{
	int	result_code;
	char	user_id[USER_ID_LEN+1];	
	char	char_name[CHAR_NAME_LEN+1];
};

struct	body_AW_CHAR_DELETE_RESULT
{
	int	result_code;
	char	user_id[USER_ID_LEN+1];
	char	char_name[CHAR_NAME_LEN+1];
};

struct	body1_AW_CHAR_LOAD_RESULT
{
	int	result_code;
	char	user_id[USER_ID_LEN+1];	
	char	char_name[CHAR_NAME_LEN+1];
	int	map;
	int	x;
	int	y;
	int	z;
	int	regen_map;
	int	regen_x;
	int	regen_y;
	int	regen_z;
	int	sex;
	int	hair;
	int	face;
	int	level;
	int	exp;
	int	hp;
	int	ap;
	int	skill_point;	
	int	money;	
	int	load_cnt;
	int	skill_slot[CHAR_SKILL_CNT];
	int	item_slot_cnt;	
	//struct	body2_AW_CHAR_LOAD_RESULT	item_slot[item_slot_cnt]
};

struct	body2_AW_CHAR_LOAD_RESULT
{
	int	item_slot;
	char	item[ITEM_DATA_LEN+ITEM_SERIAL_LEN];
};

struct	body_AW_LOGIN_RESULT
{
	int	result_code;
	char	user_id[USER_ID_LEN+1];
	int	record_lock;
	int	lock_fd;
	int	lock_sec_active;
	int	lock_session;	
};

struct	body_AW_GET_USER_RESULT
{
	int	msvr_code;
	int	ssvr_code;
	int	user;
};


//_________________________________________________________
//mob to game
struct	body_MG_INIT
{
	char	version[VERSION_LEN+1];
	int	max_map;
	int	max_type_mob;
	int	max_mob;
	int	charge_channel;
};

struct	body_MG_LIVE
{
	//noting
};

struct	body_MG_MOB_REGEN
{
	int	map;
	int	mob_id;
	int	mob_type;
	int	table_idx;
	int	dir;
	float	regen_x;
	float	regen_y;
	float	regen_z;
};

struct	body_MG_MOB_EXTINCT
{
	int	mob_id;
};

struct	body_MG_MOB_MOVE
{
	int	mob_id;
	int	dir;
	int	status;
	float	speed;
	float	x;
	float	y;
	float	z;
};

struct	body_MG_MOB_ATTACK_CHAR
{
	int	mob_id;
	int	char_id;
	int	dir;
};

struct	body1_MG_MOB_DAMAGE_LIST
{
	int	mob_id;
	int	list_cnt;
	//struct	body2_MG_MOB_DAMAGE_LIST	list[list_cnt];
};

struct	body2_MG_MOB_DAMAGE_LIST
{
	int	char_id;
	int	damage;
};

struct	body_MG_MOB_PERFECT_RECOVER
{
	int	mob_id;
};

struct	body_MG_MOB_SKILL_READY
{
	int	mob_id;
	int	skill_type;
	int	dir;
};

struct	body1_MG_MOB_SKILL_ACTION
{
	int	mob_id;
	int	skill_type;
	int	action_cnt;
	int	dir;
	float	target_x;
	float	target_y;
	float	target_z;
	int	target_cnt;
	//struct body2_MG_MOB_SKILL_ACTION	target[target_cnt]; 
};

struct	body2_MG_MOB_SKILL_ACTION
{
	int	target_id;
};

struct	body_MG_MOB_SKILL_CANCEL
{
	int	mob_id;
};


//_________________________________________________________
//game to mob
struct	body_GM_INIT_RESULT
{
	int	result_code;
};

struct	body_GM_LIVE
{
	//noting
};

struct	body1_GM_CHAR_CHANNEL_IN
{
	int	char_cnt;
	//struct	body2_GM_CHAR_CHANNEL_IN	char_info[char_cnt];
};

struct	body2_GM_CHAR_CHANNEL_IN
{
	int	map;
	int	char_id;
	int	status;
	float	x;
	float	y;
	float	z;
};

struct	body1_GM_CHAR_CHANNEL_OUT
{
	int	char_cnt;
	//struct	body2_GM_CHAR_CHANNEL_OUT	char_info[char_cnt];
};

struct	body2_GM_CHAR_CHANNEL_OUT
{
	int	char_id;
};

struct	body_GM_CHAR_MAP_CHANGE
{
	int	map;
	int	char_id;
	int	status;
	float	x;
	float	y;
	float	z;
};

struct	body_GM_CHAR_MOVE
{
	int	char_id;
	int	status;
	float	x;
	float	y;
	float	z;
};

struct	body_GM_CHAR_ATTACK_MOB
{
	int	char_id;
	int	mob_id;
	int	mob_hp;
	int	damage_type;
	int	damage;
	int	dir;
	float	x;
	float	y;
	float	z;
};

struct	body_GM_MOB_SUMMON
{
	int	map;
	int	mob_type;
	int	x;
	int	y;
	int	z;
};

struct body1_GM_CHAR_SKILL_ACTION
{
	int	char_id;
	int	dir;
	float	x;
	float	y;
	float	z;
	int	target_mob_cnt;
	//struct body2_GM_CHAR_SKILL_ACTION	target[target_mob_cnt]; 
};

struct body2_GM_CHAR_SKILL_ACTION
{
	int	target_id;
	int	target_hp;
	int	damage_type;
	int	damage;
};

struct	body_GM_MOB_BUFF
{
	int	mob_id;
	int	buff_series;
	int	buff_value;
	int	buff_sec;
};


//_________________________________________________________
//client to login
struct	body_CL_LOGIN
{
	char			version[VERSION_LEN+1];
	char			user_id[USER_ID_LEN+1];
	char			encodedstr[ENCODEDSTR_LEN+1];
};

struct	body_CL_LIVE
{
	//noting
};

struct	body_CL_WORLD_LIST
{
	//noting
};

struct	body_CL_CHANNEL_LIST
{
	unsigned	short	world_idx;
};

struct	body_CL_CHAR_LIST
{
	unsigned	short	world_idx;
};

struct	body_CL_CHAR_CREATE
{
	unsigned	short	world_idx;
	char			char_name[CHAR_NAME_LEN+1];
	unsigned	char	sex;
	unsigned 	char	hair;
	unsigned 	char	face;
};

struct	body_CL_CHAR_DELETE
{
	unsigned	short	world_idx;
	char			char_name[CHAR_NAME_LEN+1];
};

struct	body_CL_CHANNEL_CONNECT
{
	unsigned	short	world_idx;
	unsigned	short	channel_idx;
};

	
//_________________________________________________________
//login to client
struct	body_LC_SERVER_TROUBLE
{
	int			trouble;
};

struct	body_LC_LOGIN_RESULT
{
	int			result_code;
};

struct	body_LC_LIVE
{
	//noting
};

struct	body1_LC_WORLD_LIST_RESULT
{
	int			result_code;
	unsigned	short	world_cnt;
	//struct	body2_LC_WORLD_LIST_RESULT	list[world_cnt];
};

struct	body2_LC_WORLD_LIST_RESULT
{
	unsigned	short	world_idx;
	char			world_name[WORLD_NAME_LEN+1];
};

struct	body1_LC_CHANNEL_LIST_RESULT
{
	int			result_code;
	unsigned	short	channel_cnt;
	//struct	body2_LC_CHANNEL_LIST_RESULT	list[channel_cnt];
};

struct	body2_LC_CHANNEL_LIST_RESULT
{
	unsigned	short	user;
};

struct	body1_LC_CHAR_LIST_RESULT
{
	int			result_code;
	unsigned	char	char_cnt;
	//struct	body2_LC_CHAR_LIST_RESULT	list[char_cnt];
};

struct	body2_LC_CHAR_LIST_RESULT
{
	char			char_name[CHAR_NAME_LEN+1];
	unsigned	char	sex;
	unsigned 	char	hair;
	unsigned 	char	face;
	unsigned	short	level;
};

struct	body_LC_CHAR_CREATE_RESULT
{
	int			result_code;
	char			char_name[CHAR_NAME_LEN+1];	
};

struct	body_LC_CHAR_DELETE_RESULT
{
	int			result_code;
	char			char_name[CHAR_NAME_LEN+1];	
};

struct	body_LC_CHANNEL_CONNECT_RESULT
{
	int			result_code;
	char			channel_ip[SIZEOF_IP+1];	
	unsigned	short	channel_port;
};


//_________________________________________________________
//client to game
struct	body_CG_LOGIN
{
	char			version[VERSION_LEN+1];
	char			user_id[USER_ID_LEN+1];
	char			encodedstr[ENCODEDSTR_LEN+1];
};

struct	body_CG_LIVE
{
	//noting
};

struct	body_CG_CHANNEL_LIST
{
	//noting
};

struct	body_CG_CHAR_CHANNEL_MOVE
{
	unsigned	short	channel_idx;
};

struct	body_CG_CHAR_LOAD
{
	int			channel_idx;
	char			char_name[CHAR_NAME_LEN+1];
};

struct	body_CG_CHAR_LOAD_END
{
	//noting
};

struct	body_CG_CHAR_MOVE
{
	unsigned	short	dir;
	unsigned	char	status;
	float			speed;
	float			x;
	float			y;
	float			z;
};

struct	body_CG_CHAR_CHAT
{
	//char	msg[?]
};

struct	body_CG_CHAR_ATTACK_MOB
{
	unsigned	short	mob_id;
	unsigned	short	attack_id;
	unsigned	char	attack_type;
	unsigned	short	dir;
	float			speed;
	float			x;
	float			y;
	float			z;
};

struct	body_CG_CHAR_REGEN
{
	//noting
};

struct	body_CG_CHAR_ITEM_MOVE
{
	unsigned	char	item_slot1;
	unsigned	char	item_slot2;
};

struct	body_CG_CHAR_ITEM_PICKUP
{
	unsigned	short	item_id;
	unsigned	char	item_slot;
};

struct	body_CG_CHAR_ITEM_DROP
{
	unsigned	char	item_slot;
};

struct	body_CG_CHAR_ITEM_ENCHANT
{
	unsigned	char	item_slot1;
	unsigned	char	item_slot2;
};

struct	body_CG_CHAR_ITEM_USE
{
	unsigned	char	item_slot;
};

struct	body_CG_CHAR_WARP
{
	unsigned	short	mob_id;
	unsigned	char	item_slot;
};

struct	body_CG_CHAR_STORE_ITEM_BUY
{
	unsigned	short	mob_id;
	unsigned	char	store_item_slot;
	unsigned	short	item_cnt;
	unsigned	char	char_item_slot;
};

struct	body_CG_CHAR_STORE_ITEM_SELL
{
	unsigned	short	mob_id;
	unsigned	char	item_slot;
	unsigned	short	item_cnt;
};

struct	body_CG_CHAR_SKILL_READY
{
	unsigned	short	skill_type;
	unsigned	short	dir;
	float			speed;
	float			x;
	float			y;
	float			z;	
};

struct	body1_CG_CHAR_SKILL_ACTION
{
	unsigned	short	attack_id;
	unsigned	short	skill_type;
	unsigned	char	action_cnt;
	unsigned	short	dir;
	float			speed;
	float			x;
	float			y;
	float			z;
	float			target_x;
	float			target_y;
	float			target_z;
	unsigned	char	target_mob_cnt;
	unsigned	char	target_char_cnt;
	//struct body2_CG_CHAR_SKILL_ACTION	target[target_mob_cnt+target_char_cnt]; 
};

struct	body2_CG_CHAR_SKILL_ACTION
{
	unsigned	short	target_id;
};

struct	body_CG_CHAR_SKILL_CANCEL
{
	//noting	
};

struct	body_CG_CHAR_SKILL_LEARN
{
	unsigned	short	skill_type;
};

struct	body_CG_CHAR_TRANSFORM
{
	unsigned	char	transform_type;
};

struct	body_CG_MANAGER_CHAT
{
	//char	msg[?]
};
	

//_________________________________________________________
//game to client
struct	body_GC_SERVER_TROUBLE
{
	int			trouble;
};

struct	body_GC_LOGIN_RESULT
{
	int			result_code;
};

struct	body_GC_LIVE
{
	//noting
};

struct	body1_GC_CHANNEL_LIST_RESULT
{
	int			result_code;
	unsigned	short	channel_cnt;
	//struct	body2_GC_CHANNEL_LIST_RESULT	list[channel_cnt];
};

struct	body2_GC_CHANNEL_LIST_RESULT
{
	unsigned	short	user;
};

struct	body_GC_CHAR_CHANNEL_MOVE_RESULT
{
	int			result_code;
	char			channel_ip[SIZEOF_IP+1];	
	unsigned	short	channel_port;
	unsigned	short	regen_map;
	float			regen_x;
	float			regen_y;
	float			regen_z;
	unsigned	short	hp;
	unsigned	short	ap;
	unsigned	short	dir;
	unsigned	char	status;
};

struct	body1_GC_CHAR_LOAD_RESULT
{
	int			result_code;
	char			char_name[CHAR_NAME_LEN+1];
	unsigned	short	char_id;
	unsigned	short	map;
	float			x;
	float			y;
	float			z;
	unsigned	char	sex;
	unsigned 	char	hair;
	unsigned 	char	face;
	unsigned	short	level;
	int			exp;			
	unsigned	short	hp;
	unsigned	short	ap;
	unsigned	short	dir;
	unsigned	char	status;
	int			money;
	int			skill_point;
	char			skill_slot[CHAR_SKILL_CNT];
	unsigned	short	item_slot_cnt;
	//struct	body2_GC_CHAR_LOAD_RESULT	item_slot[item_slot_cnt];
};

struct	body2_GC_CHAR_LOAD_RESULT
{
	unsigned	short	item_slot;
	char			item[ITEM_DATA_LEN];
};


struct	body1_GC_OTHER_CHAR_APPEAR
{
	unsigned	short	other_char_cnt;
	//struct	body2_GC_OTHER_CHAR_APPEAR	other_char_info[other_char_cnt];
};

struct	body2_GC_OTHER_CHAR_APPEAR
{
	unsigned	short	char_id;
	char			char_name[CHAR_NAME_LEN+1];
	float			x;
	float			y;
	float			z;
	unsigned	char	sex;
	unsigned 	char	hair;
	unsigned 	char	face;
	unsigned	short	dir;
	unsigned	char	status;
	float			speed;

	unsigned	char	transform_type;

	char			item_slot[EQUIP_SLOT_CNT][ITEM_DATA_LEN];
};

struct	body1_GC_OTHER_CHAR_DISAPPEAR
{
	unsigned	short	other_char_cnt;
	//struct	body2_GC_OTHER_CHAR_DISAPPEAR	other_char_info[other_char_cnt];
};

struct	body2_GC_OTHER_CHAR_DISAPPEAR
{
	unsigned	short	char_id;
};

struct	body_GC_CHAR_MOVE
{
	unsigned	short	char_id;
	unsigned	short	dir;
	unsigned	char	status;
	float			speed;
	float			x;
	float			y;
	float			z;
};

struct	body_GC_CHAR_CHAT
{
	unsigned	short	char_id;
	//char			msg[?]
};

struct	body1_GC_MOB_APPEAR
{
	unsigned	short	mob_cnt;
	//struct	body2_GC_MOB_APPEAR	mob_info[mob_cnt];
};

struct	body2_GC_MOB_APPEAR
{
	unsigned	short	mob_id;
	unsigned	short	mob_type;
	unsigned	short	table_idx;
	unsigned	char	mob_stair;
	float			x;
	float			y;
	float			z;
	unsigned	short	dir;
	unsigned	char	status;
	float			speed;
	unsigned	short	hp;
};

struct	body1_GC_MOB_DISAPPEAR
{
	unsigned	short	mob_cnt;
	//struct	body2_GC_MOB_DISAPPEAR	mob_info[mob_cnt];
};

struct	body2_GC_MOB_DISAPPEAR
{
	unsigned	short	mob_id;
};

struct	body_GC_MOB_MOVE
{
	unsigned	short	mob_id;
	unsigned	short	dir;
	unsigned	char	status;
	float			speed;
	float			x;
	float			y;
	float			z;
};

struct	body_GC_CHAR_ATTACK_MOB
{
	unsigned	short	char_id;
	unsigned	short	mob_id;
	unsigned	short	attack_id;
	unsigned	char	attack_type;
	unsigned	short	mob_hp;
	unsigned	char	damage_type;
	unsigned	short	damage;
	unsigned	short	dir;
	float			speed;
	float			x;
	float			y;
	float			z;
};

struct	body_GC_MOB_ATTACK_CHAR
{
	unsigned	short	mob_id;
	unsigned	short	char_id;
	unsigned	short	char_hp;
	unsigned	char	damage_type;
	unsigned	short	damage;
	unsigned	short	dir;
	float			speed;
	float			x;
	float			y;
	float			z;
};

struct	body_GC_CHAR_ACQUIRE_EXP
{
	int			exp;
	int			acquire_exp;
};

struct	body_GC_CHAR_LEVEL_UP
{
	unsigned	short	char_id;
	unsigned	short	char_hp;
	unsigned	short	char_ap;
	int			skill_point;
};

struct	body_GC_CHAR_REGEN_RESULT
{
	int			result_code;
	unsigned	short	regen_map;
	float			regen_x;
	float			regen_y;
	float			regen_z;
	int			exp;
	unsigned	short	hp;
	unsigned	short	ap;
	unsigned	short	dir;
	unsigned	char	status;
};

struct	body1_GC_ITEM_APPEAR
{
	unsigned	short	item_cnt;		
	//struct	body2_GC_ITEM_APPEAR item_info[item_cnt];
};

struct	body2_GC_ITEM_APPEAR
{
	unsigned	short	item_id;
	char			item[ITEM_DATA_LEN];
	float			x;
	float			y;
	float			z;
	char			own_char_name[CHAR_NAME_LEN+1];
};

struct	body1_GC_ITEM_DISAPPEAR
{
	unsigned	short	item_cnt;
	//struct	body2_GC_ITEM_DISAPPEAR	item_info[item_cnt];
};

struct	body2_GC_ITEM_DISAPPEAR
{
	unsigned	short	item_id;
};

struct	body_GC_CHAR_ITEM_SLOT
{
	unsigned	short	char_id;
	unsigned	char	item_slot;
	char			item[ITEM_DATA_LEN];
};

struct	body_GC_CHAR_ITEM_PICKUP_RESULT
{
	int			result_code;
	unsigned	short	item_id;
	int			money;
	int			acquire_money;
	unsigned	char	item_slot;
	char			item[ITEM_DATA_LEN];
};

struct	body_GC_CHAR_ITEM_ENCHANT_RESULT
{
	int			result_code;
	unsigned	char	item_slot1;
	char			item1[ITEM_DATA_LEN];
	unsigned	char	item_slot2;
	char			item2[ITEM_DATA_LEN];
};

struct	body_GC_CHAR_BUFF
{
	unsigned	char	buff_series;
	int			buff_value;
	int			buff_sec;
};

struct	body_GC_CHAR_WARP_RESULT
{
	int			result_code;
	unsigned	short	regen_map;
	float			regen_x;
	float			regen_y;
	float			regen_z;
	unsigned	short	hp;
	unsigned	short	ap;
	unsigned	short	dir;
	unsigned	char	status;
};

struct	body_GC_CHAR_STORE_ITEM_BUY_RESULT
{
	int			result_code;
	int			money;
	unsigned	char	item_slot;
	char			item[ITEM_DATA_LEN];
};

struct	body_GC_CHAR_STORE_ITEM_SELL_RESULT
{
	int			result_code;
	int			money;
	unsigned	char	item_slot;
	char			item[ITEM_DATA_LEN];
};

struct	body_GC_CHAR_RECOVER
{
	unsigned	short	hp;
	unsigned	short	ap;
};

struct	body_GC_CHAR_ITEM_USE_RESULT
{
	unsigned	short	item_type;
	unsigned	short	hp;
	unsigned	short	ap;
	unsigned	char	item_slot;
	char			item[ITEM_DATA_LEN];
};

struct	body_GC_CHAR_SKILL_READY
{
	unsigned	short	char_id;
	unsigned	short	skill_type;
	unsigned	short	dir;
	float			speed;
	float			x;
	float			y;
	float			z;	
};

struct body1_GC_CHAR_SKILL_ACTION
{
	unsigned	short	char_id;
	unsigned	short	char_ap;
	unsigned	short	attack_id;
	unsigned	short	skill_type;
	unsigned	char	action_cnt;
	unsigned	short	dir;
	float			speed;
	float			x;
	float			y;
	float			z;
	float			target_x;
	float			target_y;
	float			target_z;
	unsigned	char	target_mob_cnt;
	unsigned	char	target_char_cnt;
	//struct body2_GC_CHAR_SKILL_ACTION	target[target_mob_cnt+target_char_cnt]; 
};

struct body2_GC_CHAR_SKILL_ACTION
{
	unsigned	short	target_id;
	unsigned	short	target_hp;
	unsigned	char	damage_type;
	unsigned	short	damage;
};

struct	body_GC_CHAR_SKILL_CANCEL
{
	unsigned	short	char_id;
};

struct	body_GC_MOB_SKILL_READY
{
	unsigned	short	mob_id;
	unsigned	short	skill_type;
	unsigned	short	dir;
	float			speed;
	float			x;
	float			y;
	float			z;
};

struct body1_GC_MOB_SKILL_ACTION
{
	unsigned	short	mob_id;
	unsigned	short	skill_type;
	unsigned	char	action_cnt;
	unsigned	short	dir;
	float			speed;
	float			x;
	float			y;
	float			z;
	float			target_x;
	float			target_y;
	float			target_z;
	unsigned	char	target_mob_cnt;
	unsigned	char	target_char_cnt;
	//struct body2_GC_MOB_SKILL_ACTION	target[target_mob_cnt+target_char_cnt]; 
};

struct body2_GC_MOB_SKILL_ACTION
{
	unsigned	short	target_id;
	unsigned	short	target_hp;
	unsigned	char	damage_type;
	unsigned	short	damage;
};

struct	body_GC_MOB_SKILL_CANCEL
{
	unsigned	short	mob_id;
};

struct	body_GC_CHAR_TRANSFORM
{
	unsigned	short	char_id;
	unsigned	char	transform_type;
};

struct	body_GC_MANAGER_CHAT
{
	//char	msg[?]
};

#pragma	pack()
