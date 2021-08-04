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
#define	ITEM_SLOT_CNT			(EQUIP_SLOT_CNT + 40)

#define	_EQUIP_SLOT_HEAD		(0)
#define	_EQUIP_SLOT_EARRINGS		(1)
#define	_EQUIP_SLOT_CLOAK		(2)
#define	_EQUIP_SLOT_COAT		(3)
#define	_EQUIP_SLOT_PANTS		(4)
#define	_EQUIP_SLOT_SHOES		(5)
#define	_EQUIP_SLOT_RING1		(6)
#define	_EQUIP_SLOT_RING2		(7)
#define	_EQUIP_SLOT_WEAPON1		(8)
#define	_EQUIP_SLOT_WEAPON2		(9)
#define	_EQUIP_SLOT_ACCESSORY1		(10)
#define	_EQUIP_SLOT_ACCESSORY2		(11)
#define	_EQUIP_SLOT_HEAD_COSTUME	(12)
#define	_EQUIP_SLOT_FACE_COSTUME	(13)
#define	_EQUIP_SLOT_NECK_COSTUME	(14)
#define	_EQUIP_SLOT_CLOAK_COSTUME	(15)
#define	_EQUIP_SLOT_COAT_COSTUME	(16)
#define	_EQUIP_SLOT_PANTS_COSTUME	(17)
#define	_EQUIP_SLOT_SHOES_COSTUME	(18)
#define	_EQUIP_SLOT_WEAPON1_COSTUME	(19)
#define	_EQUIP_SLOT_WEAPON2_COSTUME	(20)

//len
#define	SIZEOF_IP		(15)
#define	VERSION_LEN		(16)
#define	USER_ID_LEN		(16)
#define	CHAR_NAME_LEN		(16)
#define	WORLD_NAME_LEN		(16)
#define	PASSWORD_LEN		(16)
#define	ITEM_DATA_LEN		(16)
#define	ITEM_SERIAL_LEN		(49)

//char status
#define	_USER_NORMAL_WAIT1	(0)
#define	_USER_NORMAL_WAIT2	(1)
#define	_USER_ATTACK_WAIT	(2)
#define	_USER_WALK		(3)
#define	_USER_RUN		(4)
#define	_USER_JUMP_FALL		(5)
#define	_USER_JUMP1		(6)
#define	_USER_JUMP2		(7)
#define	_USER_JUMP3		(8)
#define	_USER_DOUBLE_JUMP1	(9)
#define	_USER_DOUBLE_JUMP2	(10)
#define	_USER_DOUBLE_JUMP3	(11)
#define	_USER_READY_ATTACK	(12)
#define	_USER_JUMP_ATTACK	(13)
#define	_USER_STAND_ATTACK1	(14)
#define	_USER_STAND_ATTACK2	(15)
#define	_USER_STAND_ATTACK3	(16)
#define	_USER_DASH_ATTACK	(17)
#define	_USER_CHANGE		(18)
#define	_USER_STUN		(19)
#define	_USER_DEMAGE		(20)
#define	_USER_DEAD		(21)
#define	_USER_DEAD_REGEN	(22)
#define	_USER_REGEN		(23)
#define	_USER_EXTINCT		(24)


//mob status
#define	_MOB_WAIT1		(0)
#define	_MOB_WAIT2		(1)
#define	_MOB_WALK		(2)
#define	_MOB_RUN		(3)
#define	_MOB_QUICK_RUN		(4)
#define	_MOB_ATTACK		(5)
#define	_MOB_IMPACT		(6)
#define	_MOB_DEATH		(7)
#define	_MOB_REGEN		(8)
#define	_MOB_EXTINCT		(9)

//mob summon
#define	MAP_SUMMON_MOB_CNT	(500)

//attack type
#define	_ATTACK_JUMP		(0)
#define	_ATTACK_NORMAL		(1)
#define	_ATTACK_COMBO1		(2)
#define	_ATTACK_COMBO2		(3)
#define	_ATTACK_PIERCE		(4)

//damage type (bitmask)	
#define	_DAMAGE_NORMAL		(0)
#define	_DAMAGE_MISS		(1)
#define	_DAMAGE_CRITICAL	(2)
#define	_DAMAGE_IMPACT		(4)
#define	_DAMAGE_SKILL		(8)
#define	_DAMAGE_STUN		(16)
#define	_DAMAGE_PROVOCATION	(32)
#define	_DAMAGE_SLEEP		(64)

	
//buff 
#define	BUFF_SERIES_CNT		(2)

#define	_BUFF_POTION_RECOVER_HP	(0)
#define	_BUFF_POTION_RECOVER_AP	(1)


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

#define _CM			(0x10000000)
#define	_MC			(0x20000000)		


//________________________________________________________
//result code
#define	RESULT_SUCC			(0)
#define	RESULT_FAIL_VERSION		(1)
#define	RESULT_FAIL_BILLING		(2)
#define	RESULT_FAIL_DBAGENT		(3)
#define	RESULT_FAIL_DB			(4)
#define	RESULT_FAIL_USERID_OR_PW	(5)
#define	RESULT_FAIL_DUP_LOGIN		(6)
#define	RESULT_FAIL_DUP_CHAR_NAME	(7)
#define	RESULT_FAIL_SERVER_FULL		(8)
#define	RESULT_FAIL_DATA		(9)
#define	RESULT_FAIL_EXTINCT_ITEM	(10)
#define	RESULT_FAIL_BUFF_ABILITY	(11)


//_________________________________________________________
//xxxx to billing
#define	CM_INIT				(_CM + 0x01)
#define	CM_LIVE				(_CM + 0x02)


//_________________________________________________________
//xxxx to billing
#define	MC_INIT				(_MC + 0x01)
#define	MC_LIVE				(_MC + 0x02)


//_________________________________________________________
//xxxx to billing
#define	WB_INIT				(_WB + 0x01)
#define	WB_LIVE				(_WB + 0x02)
#define	WB_LOGIN			(_WB + 0x03)
#define	WB_LOGOUT			(_WB + 0x04)


//_________________________________________________________
//billing to xxxx
#define	BW_INIT_RESULT			(_BW + 0x01)
#define	BW_LIVE				(_BW + 0x02)
#define	BW_LOGIN_RESULT			(_BW + 0x03)


//_________________________________________________________
//inter thread 
#define	INTER_EXIT_THREAD		(_INTER + 0x01)
#define	INTER_CONN_BILLING_FD		(_INTER + 0x02)
#define	INTER_CONN_DBAGENT_FD		(_INTER + 0x03)
#define	INTER_CONN_MOBSERV_FD		(_INTER + 0x04)
#define	INTER_CONN_CLIENT_FD		(_INTER + 0x05)
#define	INTER_CLOSE_FD			(_INTER + 0x06)
#define	INTER_CONN_FD			(_INTER + 0x07)
#define INTER_CONN_FD_SUCC		(_INTER + 0x08)
#define INTER_CONN_FD_RUNNING		(_INTER + 0x09)
#define INTER_CONN_FD_FAIL		(_INTER + 0x0a)
#define	INTER_CHAR_RECOVER		(_INTER + 0x0b)			
#define	INTER_MOB_RECOVER		(_INTER + 0x0c)			
#define	INTER_ITEM_REMOVE		(_INTER + 0x0d)
#define	INTER_CHAR_BUFF_END		(_INTER + 0x0e)


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


//_________________________________________________________
//dbagent to xxxx
#define	AW_INIT_RESULT			(_AW + 0x01)
#define	AW_LIVE				(_AW + 0x02)
#define	AW_CHAR_LIST_RESULT		(_AW + 0x03)
#define	AW_CHAR_CREATE_RESULT		(_AW + 0x04)
#define	AW_CHAR_DELETE_RESULT		(_AW + 0x05)
#define	AW_CHAR_LOAD_RESULT		(_AW + 0x06)


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
#define	GM_CHAR_MAP_IN			(_GM + 0x03)
#define	GM_CHAR_MAP_OUT			(_GM + 0x04)
#define	GM_CHAR_MOVE			(_GM + 0x05)
#define	GM_CHAR_ATTACK_MOB		(_GM + 0x06)
#define	GM_MOB_SUMMON			(_GM + 0x07)


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
#define	LC_LOGIN_RESULT			(_LC + 0x01)
#define	LC_LIVE				(_LC + 0x02)
#define	LC_WORLD_LIST_RESULT		(_LC + 0x03)	
#define	LC_CHANNEL_LIST_RESULT		(_LC + 0x04)
#define	LC_CHAR_LIST_RESULT		(_LC + 0x05)
#define	LC_CHAR_CREATE_RESULT		(_LC + 0x06)
#define	LC_CHAR_DELETE_RESULT		(_LC + 0x07)
#define	LC_CHANNEL_CONNECT_RESULT	(_LC + 0x08)


//_________________________________________________________
//client to game
#define	CG_LOGIN			(_CG + 0x01)
#define	CG_LIVE				(_CG + 0x02)
#define	CG_CHANNEL_LIST			(_CG + 0x03)
#define	CG_CHANNEL_CONNECT		(_CG + 0x04)
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


//_________________________________________________________
//game to client
#define	GC_LOGIN_RESULT			(_GC + 0x01)
#define	GC_LIVE				(_GC + 0x02)
#define	GC_CHANNEL_LIST_RESULT		(_GC + 0x03)
#define	GC_CHANNEL_CONNECT_RESULT	(_GC + 0x04)
#define	GC_CHAR_LOAD_RESULT		(_GC + 0x05)
#define	GC_OTHER_CHAR_APPEAR		(_GC + 0x06)
#define	GC_OTHER_CHAR_DISAPPEAR		(_GC + 0x07)
#define	GC_CHAR_MOVE			(_GC + 0x08)
#define	GC_CHAR_CHAT			(_GC + 0x09)
#define	GC_MOB_APPEAR			(_GC + 0x0a)
#define	GC_MOB_DISAPPEAR		(_GC + 0x0b)
#define	GC_MOB_MOVE			(_GC + 0x0c)
#define	GC_CHAR_ATTACK_MOB		(_GC + 0x0d)
#define	GC_MOB_ATTACK_CHAR		(_GC + 0x0e)
#define GC_CHAR_ACQUIRE_EXP		(_GC + 0x0f)
#define	GC_CHAR_LEVEL_UP		(_GC + 0x10)
#define	GC_CHAR_REGEN_RESULT		(_GC + 0x11)
#define	GC_ITEM_APPEAR			(_GC + 0x12)
#define	GC_ITEM_DISAPPEAR		(_GC + 0x13)
#define	GC_CHAR_ITEM_SLOT		(_GC + 0x14)
#define	GC_CHAR_ITEM_PICKUP_RESULT	(_GC + 0x15)
#define	GC_CHAR_ITEM_ENCHANT_RESULT	(_GC + 0x16)
#define	GC_CHAR_BUFF			(_GC + 0x17)
#define	GC_CHAR_WARP_RESULT		(_GC + 0x18)
#define	GC_CHAR_STORE_ITEM_BUY_RESULT	(_GC + 0x19)
#define	GC_CHAR_STORE_ITEM_SELL_RESULT	(_GC + 0x1a)
#define	GC_CHAR_RECOVER			(_GC + 0x1b)
#define	GC_CHAR_HP			(_GC + 0x1c)
#define	GC_CHAR_AP			(_GC + 0x1d)


//_______________________________________________________
//header
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


//_________________________________________________________
//xxxx to billing
struct	body_WB_INIT			
{
	int	server_code;
	int	all_logout;
};

struct	body_WB_LIVE
{
	//noting
};

struct	body_WB_LOGIN
{
	char	user_id[USER_ID_LEN];
	char	password[PASSWORD_LEN];
};

struct	body_WB_LOGOUT
{
	char	user_id[USER_ID_LEN];
};


//_________________________________________________________
//billing to xxxx
struct	body_BW_INIT_RESULT
{
	int	result_code;
};

struct	body_BW_LIVE
{
	//noting
};

struct	body_BW_LOGIN_RESULT
{
	int	result_code;
	char	user_id[USER_ID_LEN];
};


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
	char	version[VERSION_LEN];
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
	int			result_code;
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
	char	name[WORLD_NAME_LEN];
};

struct	body1_SW_GAMESERV_INFO_RESULT
{
	int	info_cnt;
	//struct	body2_SW_GAMESERV_INFO_RESULT	info[info_cnt];
};

struct	body2_SW_GAMESERV_INFO_RESULT
{
	char	ip[SIZEOF_IP];
	char	outer_ip[SIZEOF_IP];
};

struct	body1_SW_DBAGENT_INFO_RESULT
{
	int	info_cnt;
	//struct	body2_SW_DBAGENT_INFO_RESULT	info[info_cnt];
};

struct	body2_SW_DBAGENT_INFO_RESULT
{
	char	ip[SIZEOF_IP];
};

struct	body1_SW_MOBSERV_INFO_RESULT
{
	int	info_cnt;
	//struct	body2_SW_MOBSERV_INFO_RESULT	info[info_cnt];
};

struct	body2_SW_MOBSERV_INFO_RESULT
{
	char	ip[SIZEOF_IP];
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
	char	version[VERSION_LEN];
};

struct	body_WA_LIVE
{
	//noting
};


struct	body_WA_CHAR_LIST
{
	char	user_id[USER_ID_LEN];	
};

struct	body_WA_CHAR_CREATE
{
	char	user_id[USER_ID_LEN];
	char	char_name[CHAR_NAME_LEN];
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
	char	user_id[USER_ID_LEN];	
	char	char_name[CHAR_NAME_LEN];
};


struct	body_WA_CHAR_LOAD
{
	char	user_id[USER_ID_LEN];	
	char	char_name[CHAR_NAME_LEN];
};

struct	body_WA_CHAR_SAVE
{
	char	user_id[USER_ID_LEN];	
	char	char_name[CHAR_NAME_LEN];
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
	char	item_slot[ITEM_SLOT_CNT][ITEM_DATA_LEN+ITEM_SERIAL_LEN];
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
	char	user_id[USER_ID_LEN];
	int	char_cnt;
	//struct	body2_AW_CHAR_LIST_RESULT	list[char_cnt];		
};

struct	body2_AW_CHAR_LIST_RESULT
{
	char	char_name[CHAR_NAME_LEN];
	int	sex;
	int	hair;
	int	face;
	int	level;
};

struct	body_AW_CHAR_CREATE_RESULT
{
	int	result_code;
	char	user_id[USER_ID_LEN];	
	char	char_name[CHAR_NAME_LEN];
};

struct	body_AW_CHAR_DELETE_RESULT
{
	int	result_code;
	char	user_id[USER_ID_LEN];
	char	char_name[CHAR_NAME_LEN];
};

struct	body_AW_CHAR_LOAD_RESULT
{
	int	result_code;
	char	user_id[USER_ID_LEN];	
	char	char_name[CHAR_NAME_LEN];
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
	char	item_slot[ITEM_SLOT_CNT][ITEM_DATA_LEN+ITEM_SERIAL_LEN];
};


//_________________________________________________________
//mob to game
struct	body_MG_INIT
{
	char	version[VERSION_LEN];
	int	max_map;
	int	max_type_mob;
	int	max_mob;
	int	charge_map;
};

struct	body_MG_LIVE
{
	//noting
};

struct	body_MG_MOB_REGEN
{
	int	group;
	int	mob_id;
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
	float	speed;
	float	x;
	float	y;
	float	z;	
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

struct body_MG_MOB_SKILL_READY
{
	int	mob_id;
	int	skill_type;
	int	dir;
	float	speed;
	float	x;
	float	y;
	float	z;	
};

struct body1_MG_MOB_SKILL_ACTION
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

struct body2_MG_MOB_SKILL_ACTION
{
	int	target_id;
};

struct body_MG_MOB_SKILL_CANCEL
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

struct	body1_GM_CHAR_MAP_IN
{
	int	char_cnt;
	//struct	body2_GM_CHAR_MAP_IN	char_info[char_cnt];
};

struct	body2_GM_CHAR_MAP_IN
{
	int	group;
	int	char_id;
	int	status;
	float	x;
	float	y;
	float	z;
};

struct	body1_GM_CHAR_MAP_OUT
{
	int	char_cnt;
	//struct	body2_GM_CHAR_MAP_OUT	char_info[char_cnt];
};

struct	body2_GM_CHAR_MAP_OUT
{
	int	char_id;
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
	int	tick;
	int	dir;
	float	speed;
	float	x;
	float	y;
	float	z;
};

struct	body_GM_MOB_SUMMON
{
	int	group;
	int	mob_type;
	int	x;
	int	y;
	int	z;
};

//_________________________________________________________
//client to login
struct	body_CL_LOGIN
{
	char			version[VERSION_LEN];
	char			user_id[USER_ID_LEN];
	char			password[PASSWORD_LEN];
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
	char			char_name[CHAR_NAME_LEN];
	unsigned	char	sex;
	unsigned 	char	hair;
	unsigned 	char	face;
};

struct	body_CL_CHAR_DELETE
{
	unsigned	short	world_idx;
	char			char_name[CHAR_NAME_LEN];
};

struct	body_CL_CHANNEL_CONNECT
{
	unsigned	short	world_idx;
	unsigned	short	channel_idx;
};

	
//_________________________________________________________
//login to client
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
	char			world_name[WORLD_NAME_LEN];
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
	char			char_name[CHAR_NAME_LEN];
	unsigned	char	sex;
	unsigned 	char	hair;
	unsigned 	char	face;
	unsigned	short	level;
};

struct	body_LC_CHAR_CREATE_RESULT
{
	int			result_code;
	char			char_name[CHAR_NAME_LEN];	
};

struct	body_LC_CHAR_DELETE_RESULT
{
	int			result_code;
	char			char_name[CHAR_NAME_LEN];	
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
	char			version[VERSION_LEN];
	char			user_id[USER_ID_LEN];
	char			password[PASSWORD_LEN];
};

struct	body_CG_LIVE
{
	//noting
};

struct	body_CG_CHANNEL_LIST
{
	//noting
};

struct	body_CG_CHANNEL_CONNECT
{
	unsigned	short	channel_idx;
};

struct	body_CG_CHAR_LOAD
{
	char			char_name[CHAR_NAME_LEN];
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


//_________________________________________________________
//game to client
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

struct	body_GC_CHANNEL_CONNECT_RESULT
{
	int			result_code;
	char			channel_ip[SIZEOF_IP+1];	
	unsigned	short	channel_port;
};

struct	body_GC_CHAR_LOAD_RESULT
{
	int			result_code;
	char			char_name[CHAR_NAME_LEN];
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
	char			item_slot[ITEM_SLOT_CNT][ITEM_DATA_LEN];
};

struct	body1_GC_OTHER_CHAR_APPEAR
{
	unsigned	short	other_char_cnt;
	//struct	body2_GC_OTHER_CHAR_APPEAR	other_char_info[other_char_cnt];
};

struct	body2_GC_OTHER_CHAR_APPEAR
{
	unsigned	short	char_id;
	char			char_name[CHAR_NAME_LEN];
	float			x;
	float			y;
	float			z;
	unsigned	char	sex;
	unsigned 	char	hair;
	unsigned 	char	face;
	unsigned	short	dir;
	unsigned	char	status;
	float			speed;

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
	int			acquire_exp;
};

struct	body_GC_CHAR_LEVEL_UP
{
	unsigned	short	char_id;
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
	char			own_char_name[CHAR_NAME_LEN];
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

struct	body_GC_CHAR_HP
{
	unsigned	short	hp;
	
};

struct	body_GC_CHAR_AP
{
	unsigned	short	ap;
};
