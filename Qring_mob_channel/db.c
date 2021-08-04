
#include	"global.h"
#include	"util.h"
#include	"tick.h"
#include	"sp.h"
#include	"mymath.h"
#include	"db.h"


struct host 
{
	char			ipaddress[SIZEOF_IP + 1];
};

struct type_warp
{
	//int			index;
	//char			name[256];
	//int			enter_flag;
	//int			enter_level;
	//int			enter_item;
	int			start_sec;
	int			end_sec;
	//int			warp_map;
	//int			warp_x;
	//int			warp_z;
	//int			minimap_start_x;
	//int			minimap_start_z;
	//int			minimap_end_x;
	//int			minimap_end_z;
};

//struct store_item
//{
//	int			item_type;
//	int			item_plus_price;
//};

struct type_store
{
	//int			index;
	//char			name[256];
	//struct store_item	item[40];
	//int			durability;
	//int			durability_plus_price;
};

struct map_link
{
	unsigned short		char_link[MAX_X_TILE][MAX_Z_TILE];
	unsigned short		mob_link[MAX_X_TILE][MAX_Z_TILE];
	unsigned short		summon_link;
	unsigned short		empty_link;
};

struct map
{
	int			height;
	int			width;

	int			start_mob_id[2];
	int			end_mob_id[2];
	int			start_summon_mob_id[2];

	char			collission[MAX_X_TILE][MAX_Z_TILE];

	struct map_link		*map_link;
};

struct type_skill_order
{
	int			function;
	int			damage;
	int			sec;
};

struct type_mob_skill
{
	//int			index;
	//char			name[255];
	struct type_skill_order	order[MAX_SKILL_ORDER_CNT];
	int			action_cnt;
	int			action_millisec;
	int			use_ap;
	int			target_object;
	int			target_apply;
	int			target_distance;
	int			range_type;
	int			range_x;
	int			range_z;
	int			casting_millisec;
	int			ai_condition;
	//int			motion_casting;
	//int			motion_active;
	//int			sound_hit;
	//char			monster[255];
	//char			scripts[255];

	int			summon;
	int			pattern;
};

struct mob_have_skill
{
	int			type;
	int			rate;
};

struct type_mob
{
	//int			index;
	//char			name[255];
	//int			level;
	int			hp_max;
	int			ap_max;
	//int			exp;
	//int			att_basic;
	//int			def_basic;
	//int			att2_basic;
	//int			def2_basic;
	//int			eva_basic;
	//int			impact_short_def;
	//int			impact_long_def;
	//int			impact_att;
	int			m_def_basic;
	//int			cri_basic;
	int			hp_recover;
	int			ap_recover;
	float			move_speed[3];
	int			milli_delay_attack;
	int			area_attack;
	//int			def_intensity;
	//int			att_intensity;
	struct mob_have_skill	skill[MAX_MOB_HAVE_SKILL_CNT];
	int			area_wander_w;
	int			area_wander_h;
	int			attack_type;
	int			area_vision;
	int			area_persuit;
	int			escape_hp;
	int			milli_delay_regen;
	int			milli_delay_death;
	//int			elements;
	int			summon_cnt;
	//char			talk[256];
	//int			graphic;
	//float			scale;
	//int			drop_type;
	//int			drop_money_rate;
	//int			drop_money_min;
	//int			drop_money_max;
	//int			quest_item_num;
	//int			quest_num;
	//int			quest_drop_rate;
	int			npc_type;
	//char			regen_effect[255];
	//int			mastery;
	//int			map_icon;
	//int			quest_list[15];

	int			have_skill[MAX_SKILL_AI_COND + 1];
};

struct target_info
{
	int			char_id;
	int			damage;
};

struct buff_info
{
	int			value;
};

struct mob 
{
	int			regen_map;
	int			regen_x;
	int			regen_z;
	int			regen_dir;
	char			collission;

	int			type;
	int			table_idx;
	int			parent_id;

	int			status;
	double			action_millisec;
	time_t			action_sec[MAX_ACTION_SEC_CNT];

	int			map;
	int			dir;
	float			x;
	float			z;

	int			area_wander_sx;
	int			area_wander_sz;
	int			area_wander_ex;
	int			area_wander_ez;

	int			area_persuit_sx;
	int			area_persuit_sz;
	int			area_persuit_ex;
	int			area_persuit_ez;

	float			random_x;
	float			random_z;

	int			wander;
	int			wander_dir;

	int			hp_cur;
	int			ap_cur;

	int			attack_type;
	int			combat_start;
	int			attack_start;

	int			target_cnt;
	struct target_info	target_list[TARGET_LIST_CNT];
	int			current_target_idx;

	time_t			skill_sec[MAX_SKILL_AI_COND + 1];
	int			skill_num[MAX_SKILL_AI_COND + 1];

	int			current_skill_type;
	int			skill_casting_millisec;
	int			skill_target_object_id;
	int			skill_action_cnt;
	int			skill_action_millisec;

	struct buff_info	buff[BUFF_SERIES_CNT];
	int			sleep;

	int			escape_cnt;
	int			summon_cnt;

	unsigned short *	link_prev;
	unsigned short		link_next;

	unsigned short *	summon_link_prev;
	unsigned short		summon_link_next;

	unsigned short *	empty_link_prev;
	unsigned short		empty_link_next;
};

struct character
{
	int			status;

	int			channel;
	int			map;
	float			x;
	float			y;
	float			z;

	struct buff_info	buff[BUFF_SERIES_CNT];

	int			link_add;
	unsigned short *	link_prev;
	unsigned short		link_next;
};


//______________________________________________________________________________________
// db variable
static int				D_world;
static struct host			D_gameserv;

static int				D_map_cnt;
static int				D_ins_start_map;
static struct map *			D_map;
static int				D_type_warp_cnt;
static struct type_warp *		D_type_warp;
static int				D_type_store_cnt;
static struct type_store *		D_type_store;
static int				D_type_mob_skill_cnt;
static struct type_mob_skill *		D_type_mob_skill;
static int				D_type_mob_cnt;
static struct type_mob *		D_type_mob;
static int				D_mob_cnt[2];
static struct mob 			*(D_mob[TOTAL_CHANNEL_CNT + 1]);
static struct character			*(D_char[TOTAL_CHANNEL_CNT + 1]);


//______________________________________________________________________________________
// script
static inline int	D_set_world				( void );
static inline int	D_set_map				( const int );
static inline int	D_set_type_warp				( const int );
static inline int	D_set_type_store			( const int );
static inline int	D_set_type_mob_skill			( const int );
static inline int	D_set_type_mob				( const int );
static inline int	D_set_mob				( const int );

static inline int	D_chk_type_warp				( const int );
static inline int	D_chk_type_store			( const int );
static inline int	D_chk_type_mob				( const int );
static inline int	D_chk_type_mob_skill			( const int );
static inline int	D_chk_all_script			( void );

static inline int	D_set_mob_regen				( const int, const int, const int, const int, const int, const int
								, const int, const int );
static inline int	D_set_mob_move_area			( const int, const int );

//______________________________________________________________________________________
// type
static inline int	D_typewarp_start_sec			( const int );
static inline int	D_typewarp_end_sec			( const int );

static inline int	D_typemobskill_ai_condition		( const int );
static inline int	D_typemobskill_casting_millisec		( const int );
static inline int	D_typemobskill_use_ap			( const int );
static inline int	D_typemobskill_range_type		( const int );
static inline int	D_typemobskill_range_x			( const int );
static inline int	D_typemobskill_range_z			( const int );
static inline int	D_typemobskill_target_object		( const int );
static inline int	D_typemobskill_target_apply		( const int );
static inline int	D_typemobskill_target_distance		( const int );
static inline int	D_typemobskill_pattern			( const int );
static inline int	D_typemobskill_action_cnt		( const int );
static inline int	D_typemobskill_action_millisec		( const int );
static inline int	D_typemobskill_damage			( const int, const int );
static inline int	D_typemobskill_function			( const int, const int );
static inline int	D_typemobskill_func_summon		( const int );

static inline int	D_typemob_attack_type			( const int );
static inline int	D_typemob_hp_max			( const int );
static inline int	D_typemob_ap_max			( const int );
static inline int	D_typemob_hp_recover			( const int );
static inline int	D_typemob_ap_recover			( const int );
static inline int	D_typemob_area_attack			( const int );
static inline int	D_typemob_area_vision			( const int );
static inline int	D_typemob_area_wander_w			( const int );
static inline int	D_typemob_area_wander_h			( const int );
static inline int	D_typemob_area_persuit			( const int );
static inline int	D_typemob_delay_regen			( const int );
static inline int	D_typemob_delay_attack			( const int );
static inline int	D_typemob_delay_death			( const int );
static inline float	D_typemob_move_speed			( const int, const int );
static inline int	D_typemob_escape_hp			( const int );
static inline int	D_typemob_skill_type			( const int, const int );
static inline int	D_typemob_skill_rate			( const int, const int );
static inline int	D_typemob_summon_cnt			( const int );
static inline int	D_typemob_npc_type			( const int );
static inline int	D_typemob_have_skill_ai_condition	( const int, const int );


//______________________________________________________________________________________
// map
static inline int	D_GetMap_collission			( const int, const int, const int );
static inline void	D_SetMap_limit_xz			( const int, int *, int *, int *, int * );

static inline int	D_GetMap_char_link			( const int, const int, const int, const int );
static inline void	D_SetMap_char_link_insert		( const int, const int, const int );
static inline void	D_SetMap_char_link_delete		( const int, const int );
static inline int	D_GetMap_char_link_next			( const int, const int );
	
static inline int	D_GetMap_mob_link			( const int, const int, const int, const int );
static inline void	D_SetMap_mob_link_insert		( const int, const int, const int );
static inline void	D_SetMap_mob_link_delete		( const int, const int );
static inline int	D_GetMap_mob_link_next			( const int, const int );
	
static inline void	D_SetMap_summon_link_insert		( const int, const int, const int );
static inline void	D_SetMap_summon_link_delete		( const int, const int );

static inline int	D_GetMap_empty_link			( const int, const int );
static inline void	D_SetMap_empty_link_insert		( const int, const int, const int );
static inline void	D_SetMap_empty_link_delete		( const int, const int );


//______________________________________________________________________________________
// mob
static inline int	D_ChkMob_possible_move			( const int, const int );
static inline float	D_GetMob_move_speed			( const int, const int, const int );
static inline int	D_GetMob_hp_max				( const int, const int );
static inline int	D_GetMob_ap_max				( const int, const int );
static inline int	D_GetMob_hp_cur				( const int, const int );
static inline void	D_SetMob_hp_cur				( const int, const int, const int );
static inline int	D_GetMob_ap_cur				( const int, const int );
static inline void	D_SetMob_ap_cur				( const int, const int, const int );
static inline int	D_GetMob_hp_cur_percent			( const int, const int );
static inline int	D_GetMob_escape_hp			( const int, const int );
static inline int	D_GetMob_escape_cnt			( const int, const int );
static inline void	D_SetMob_escape_cnt			( const int, const int , const int );
static inline int	D_GetMob_summon_cnt			( const int, const int );
static inline void	D_SetMob_summon_cnt			( const int, const int , const int );
static inline int	D_GetMob_parent_id			( const int, const int );
static inline void	D_SetMob_parent_id			( const int, const int, const int );
static inline void	D_SetMob_attack_type			( const int, const int );
static inline int	D_GetMob_attack_type			( const int, const int );
static inline int	D_GetMob_combat_start			( const int, const int );
static inline int	D_GetMob_attack_start			( const int, const int );
static inline void	D_SetMob_attack_start			( const int, const int, const int );
static inline int	D_ChkMob_have_skill_ai_condition	( const int, const int, const int );
static inline int	D_GetMob_current_skill_type		( const int, const int );
static inline int	D_GetMob_skill_target_object_id		( const int, const int );
static inline void	D_SetMob_skill_action_cnt		( const int, const int, const int );


static inline void	D_GetMob_area_vision			( const int, const int, int *, int *, int *, int * );
static inline int	D_ChkMob_over_area_vision		( const int, const int, const float, const float );
static inline int	D_ChkMob_over_area_8_vision		( const int, const int, const float, const float );
static inline void	D_GetMob_area_wander			( const int, const int, int *, int *, int *, int * );
static inline int	D_ChkMob_over_area_wander		( const int, const int, const float, const float );
static inline void	D_GetMob_area_persuit			( const int, const int, int *, int *, int *, int * );
static inline void	D_GetMob_area_attack			( const int, const int, int *, int *, int *, int * );
static inline void	D_GetMob_area_skill_distance		( const int, const int, const int, int *, int *, int *, int * );
static inline int	D_ChkMob_over_area_skill_distance	( const int, const int, const int, const int, const int );
static inline void	D_GetMob_area_skill			( const int, const int, const int, const int, const int
								, int *, int *, int *, int * );

static inline void	D_SetMob_regen_xz			( const int, const int );
static inline int	D_SetMob_move_xz			( const int, const int, const float, const float, const int, const float );
static inline int	D_SetMob_impact_xz			( const int, const int, const int, const int );
static inline int	D_GetMob_wander_move_xz			( const int, const int, const int, const float, float *, float * );
static inline int	D_SetMob_random_xz			( const int, const int, const int, const float, const float );

static inline int	D_GetMob_area_skill_distance_mob_list	( const int, const int, const int, char * );
static inline int	D_GetMob_area_my_circle_mob_list	( const int, const int, const int, const int, char * );
static inline int	D_GetMob_area_my_spray_mob_list		( const int, const int, const int, const int, char *, float *, float * );
static inline int	D_GetMob_area_other_circle_mob_list	( const int, const int, const int, const int, const int, char * );

static inline int	D_GetMob_area_vision_char		( const int, const int );
static inline int	D_GetMob_area_my_circle_char_list	( const int, const int, const int, const int, char * );
static inline int	D_GetMob_area_my_spray_char_list	( const int, const int, const int, const int, char *, float *, float * );
static inline int	D_GetMob_area_other_circle_char_list	( const int, const int, const int, const int, const int, char * );

static inline void	D_SetMob_init_target_list		( const int, const int );
static inline void	D_SetMob_target_list			( const int, const int, const int, const int, const int );
static inline void	D_SetMob_delete_target_char		( const int, const int, const int );
static inline void	D_SetMob_change_current_target_idx	( const int, const int );

static inline void	D_SetMob_init_combat			( const int, const int );
static inline void	D_SetMob_combat_start			( const int, const int );

static inline int	D_ChkMob_action_to_mob			( const int, const int, const int );

static inline int	D_ChkMob_skill_casting			( const int, const int );
static inline void	D_SetMob_current_skill			( const int, const int, const int, const int, const int
								, const int, const int );
static inline int	D_SetMob_do_skill_ready			( const int, const int, const int, char * );
static inline int	D_SetMob_do_skill_action		( const int, const int, const int, char *, int * );

static inline int	D_SetMob_skill_ready_my_my_single	( const int, const int, const int );
static inline int	D_SetMob_skill_ready_my_char_circle	( const int, const int, const int );
static inline int	D_SetMob_skill_ready_my_mob_circle	( const int, const int, const int );
static inline int	D_SetMob_skill_ready_char_char_single	( const int, const int, const int );
static inline int	D_SetMob_skill_ready_char_char_circle	( const int, const int, const int );
static inline int	D_SetMob_skill_ready_char_char_spray	( const int, const int, const int );
static inline int	D_SetMob_skill_ready_mob_mob_single	( const int, const int, const int );
static inline int	D_SetMob_skill_ready_mob_mob_circle	( const int, const int, const int );
static inline int	D_SetMob_skill_ready_mob_mob_spray	( const int, const int, const int );

static inline int	D_SetMob_skill_action_my_my_single	( const int,  const int, const int, float *, float *, char *, int * );
static inline int	D_SetMob_skill_action_my_char_circle	( const int,  const int, const int, float *, float *, char *, int * );
static inline int	D_SetMob_skill_action_my_mob_circle	( const int,  const int, const int, float *, float *, char *, int * );
static inline int	D_SetMob_skill_action_char_char_single	( const int,  const int, const int, float *, float *, char *, int * );
static inline int	D_SetMob_skill_action_char_char_circle	( const int,  const int, const int, float *, float *, char *, int * );
static inline int	D_SetMob_skill_action_char_char_spray	( const int,  const int, const int, float *, float *, char *, int * );
static inline int	D_SetMob_skill_action_mob_mob_single	( const int,  const int, const int, float *, float *, char *, int * );
static inline int	D_SetMob_skill_action_mob_mob_circle	( const int,  const int, const int, float *, float *, char *, int * );
static inline int	D_SetMob_skill_action_mob_mob_spray	( const int,  const int, const int, float *, float *, char *, int * );

static inline void	D_SetMob_init_buff			( const int, const int );
static inline int	D_GetMob_buff_value			( const int, const int, const int );
static inline int	D_SetMob_recv_impact			( const int, const int, const int, const int, const int, char * );


//______________________________________________________________________________________
// char
static inline void	D_SetChar_init				( const int, const int );







//______________________________________________________________________________________
// script
int	D_set_script( void )
{
	int				initdb_fd;
	char				initdb_ipaddress[SIZEOF_IP + 1];
	char				buf[SIZEOF_HEADER_NORMAL+SIZEOF_MAX_BODY];
	struct header_special		header;
	struct body_WS_INIT		send_body;
	struct body_SW_INIT_RESULT	*body_ptr;


	if( U_get_value("common.ini", "script", "SCRIPTSERV", initdb_ipaddress) )
	{
		dprintf(0, "[ DB ]Error D_set_script() scriptserv ipaddress \n");
		return (-1);
	}

	if( (initdb_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		dprintf(0, "[ DB ]Error D_set_script() socket create() : %s \n", strerror(errno) );
		return (-1);
	}

	if( U_ConnectToServer(initdb_fd, initdb_ipaddress, SCRIPTSERV_PORT, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_script() U_ConnectToServer() \n");
		close(initdb_fd);
		return (-1);
	}

	header.command 		= WS_INIT;
	header.body_size	= sizeof(struct body_WS_INIT);
	header.fd		= 0;
	header.sec_active	= 0;
	header.session		= 0;

	strncpy( send_body.version, VERSION, (VERSION_LEN + 1) );

	memcpy( buf, &header, SIZEOF_HEADER_SPECIAL );
	memcpy( buf + SIZEOF_HEADER_SPECIAL, &send_body, sizeof(struct body_WS_INIT) );

	if( U_SendToServer(initdb_fd, SIZEOF_HEADER_SPECIAL + header.body_size, buf, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_script() U_SendToServer() \n");
		close(initdb_fd);
		return (-1);
	}

	if( U_RecvFromServer(initdb_fd, SIZEOF_HEADER_SPECIAL + sizeof(struct body_SW_INIT_RESULT), buf, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_script() U_RecvFromServer() \n");
		close(initdb_fd);
		return (-1);
	}


	body_ptr	= (struct body_SW_INIT_RESULT *)(buf + SIZEOF_HEADER_SPECIAL);

	if( body_ptr->result_code )
	{
		dprintf(0, "[ DB ]Error D_set_script() result_code(%d) \n", body_ptr->result_code);
		close(initdb_fd);
		return (-1);
	}

	if( D_set_world() )
	{
		dprintf(0, "[ DB ]Error D_set_world() \n");
		close(initdb_fd);
		return (-1);
	}

	if( D_set_map(initdb_fd) )
	{
		dprintf(0, "[ DB ]Error D_set_map() \n");
		close(initdb_fd);
		return (-1);
	}

	if( D_set_type_warp(initdb_fd) )
	{
		dprintf(0, "[ DB ]Error D_set_type_warp() \n");
		close(initdb_fd);
		return (-1);
	}

	if( D_set_type_store(initdb_fd) )
	{
		dprintf(0, "[ DB ]Error D_set_type_store() \n");
		close(initdb_fd);
		return (-1);
	}

	if( D_set_type_mob_skill(initdb_fd) )
	{
		dprintf(0, "[ DB ]Error D_set_type_mob_skill() \n");
		close(initdb_fd);
		return (-1);
	}

	if( D_set_type_mob(initdb_fd) )
	{
		dprintf(0, "[ DB ]Error D_set_type_mob() \n");
		close(initdb_fd);
		return (-1);
	}

	if( D_set_mob(initdb_fd) )
	{
		dprintf(0, "[ DB ]Error D_set_mob() \n");
		close(initdb_fd);
		return (-1);
	}

	if( D_set_char(initdb_fd) )
	{
		dprintf(0, "[ DB ]Error D_set_char() \n");
		close(initdb_fd);
		return (-1);
	}

	close(initdb_fd);


	if( D_chk_all_script() )
	{
		dprintf(0, "[ DB ]Error D_chk_all_script() \n");
		return (-1);
	}

	dprintf(0, "\t[ DB ]Success D_set_script() \n");
	return (0);
}


static inline int	D_set_world( void )
{
	char	tmpBuf[256];


	if( U_get_value("mob_serv.ini", "basic", "world", tmpBuf) )
	{
		dprintf(0, "[ DB ]Error D_set_world() read world \n");
		return (-1);
	}

	if( (D_world = atoi(tmpBuf)) < 1 )
	{
		dprintf(0, "[ DB ]Error D_set_world() world(%d)\n", D_world );
		return (-1);
	}


	sprintf( tmpBuf, "world_%d", D_world );

	if( U_get_value("common.ini", tmpBuf, "GAMESERV", D_gameserv.ipaddress) )
	{
		dprintf(0, "[ DB ]Error D_set_world() ipaddress \n");
		return (-1);
	}

	dprintf(0, "\t[ DB ]Success D_set_world(%d, %s)\n", D_world, D_gameserv.ipaddress);
	return (0);
}


static inline int	D_set_map( const int initdb_fd )
{
	struct header_special			header;
	struct body1_SW_MAP_FILES_RESULT	body1;
	struct body2_SW_MAP_FILES_RESULT	body2;
	char					map_file[MAX_MAP_SIZE];
	int					map, x, z, collission;
	

	D_map_cnt		= 0;

	header.command		= WS_MAP_FILES;
	header.body_size	= sizeof(struct body_WS_MAP_FILES);
	header.fd		= 0;
	header.session		= 0;
	header.sec_active	= 0;

	if( U_SendToServer(initdb_fd, SIZEOF_HEADER_SPECIAL, (char *)&header, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_map() U_SendToServer() \n");
		return (-1);
	}

	if( U_RecvFromServer(initdb_fd, SIZEOF_HEADER_SPECIAL, (char *)&header, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_map() U_RecvFromServer() \n");
		return (-1);
	}

	if( header.command != SW_MAP_FILES_RESULT )
	{
		dprintf(0, "[ DB ]Error D_set_map() header.command(%x, %x) \n", header.command, SW_MAP_FILES_RESULT );
		return (-1);
	}

	if( U_RecvFromServer(initdb_fd, sizeof(struct body1_SW_MAP_FILES_RESULT), (char *)&body1, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_map() U_RecvFromServer() \n");
		return (-1);
	}

	if( (body1.file_cnt < 1) || (body1.file_cnt > (MAX_SCRIPT_FILE_CNT- 1)) )
	{
		dprintf(0, "[ DB ]Error D_set_map() map_file_cnt(%d) \n", body1.file_cnt);
		return (-1);
	}


	D_map_cnt	= body1.file_cnt;

	if( (D_map = calloc((D_map_cnt + 1), sizeof(struct map))) == NULL )
	{
		dprintf(0,  "[ DB ]Error D_set_map() calloc() : %s\n", strerror(errno));
		return (-1);
	}

	// TODO :
	D_ins_start_map	= D_map_cnt + 1;


	for( map=1; map<=D_map_cnt; map++ )
	{
		if( map < D_ins_start_map )
		{
			if( (D_map[map].map_link = calloc((NORMAL_CHANNEL_CNT + 1), sizeof(struct map_link))) == NULL )
			{
				dprintf(0,  "[ DB ]Error D_set_map() calloc() : %s\n", strerror(errno));
				return (-1);
			}
		}
		else
		{
			if( (D_map[map].map_link = calloc((TOTAL_CHANNEL_CNT + 1), sizeof(struct map_link))) == NULL )
			{
				dprintf(0,  "[ DB ]Error D_set_map() calloc() : %s\n", strerror(errno));
				return (-1);
			}
		}
	}


	for( map=1; map<(D_map_cnt + 1); map++ )
	{

		//dprintf(0, "\t[ DB ]Loading ........... %d's map \n", map );


		if( U_RecvFromServer(initdb_fd, sizeof(struct body2_SW_MAP_FILES_RESULT), (char *)&body2,  3) )
		{
			dprintf(0, "[ DB ]Error D_set_map() U_RecvFromServer() \n");
			return (-1);
		}

		if( (body2.file_size < 1) || (body2.file_size > MAX_MAP_SIZE) )
		{
			dprintf(0, "[ DB ]Error D_set_map() body2.file_size(%d, %d) \n", body2.file_size, MAX_MAP_SIZE );
			return (-1);
		}

		if( U_RecvFromServer(initdb_fd, body2.file_size, map_file, 3) )
		{
			dprintf(0, "[ DB ]Error D_set_map() U_RecvFromServer() \n");
			return (-1);
		}

		D_map[map].width	=  *((int *)&map_file[32]);
		D_map[map].height	=  *((int *)&map_file[36]);

		for( z=0; z<MAX_Z_TILE; z++ )
			for( x=0; x<MAX_X_TILE; x++ )
				D_map[map].collission[x][z]	= (-1);

		collission = 40;
		for( z=0; z<D_map[map].height; z++ )
			for( x=0; x<D_map[map].width; x++ )
				D_map[map].collission[x][z]	= map_file[collission++];
	}

	dprintf(0, "\t[ DB ]Success D_set_map(%d)\n", D_map_cnt);
	return (0);
}


static inline int	D_set_type_warp( const int initdb_fd )
{
	struct header_special				header;
	struct body1_SW_TYPE_WARP_FILES_RESULT		body1;
	struct body2_SW_TYPE_WARP_FILES_RESULT		body2;
	int						i, j, comment_row_cnt, line_row_cnt, warp_type;
	char						*buf_ptr[MAX_SCRIPT_FILE_CNT];
	char						*line_ptr, *row_ptr;


	D_type_warp_cnt		= 0;

	header.command		= WS_TYPE_WARP_FILES;
	header.body_size	= sizeof(struct body_WS_TYPE_WARP_FILES);
	header.fd		= 0;
	header.session		= 0;
	header.sec_active	= 0;

	if( U_SendToServer(initdb_fd, SIZEOF_HEADER_SPECIAL, (char *)&header, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_type_warp() U_SendToServer() \n");
		return (-1);
	}

	if( U_RecvFromServer(initdb_fd, SIZEOF_HEADER_SPECIAL, (char *)&header, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_type_warp() U_RecvFromServer() \n");
		return (-1);
	}

	if( header.command != SW_TYPE_WARP_FILES_RESULT )
	{
		dprintf(0, "[ DB ]Error D_set_type_warp() header.command(%x, %x) \n", header.command, SW_TYPE_WARP_FILES_RESULT );
		return (-1);
	}

	if( U_RecvFromServer(initdb_fd, sizeof(struct body1_SW_TYPE_WARP_FILES_RESULT), (char *)&body1, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_type_warp() U_RecvFromServer() \n");
		return (-1);
	}

	if( (body1.file_cnt < 0) || (body1.file_cnt > (MAX_SCRIPT_FILE_CNT - 1)) )
	{
		dprintf(0, "[ DB ]Error D_set_type_warp() warp_file_cnt(%d) \n", body1.file_cnt);
		return (-1);
	}


	for( i=0; i<body1.file_cnt; i++ )
	{

		if( U_RecvFromServer(initdb_fd, sizeof(struct body2_SW_TYPE_WARP_FILES_RESULT), (char *)&body2, 3) )
		{
			dprintf(0, "[ DB ]Error D_set_type_warp() U_RecvFromServer() \n");
			return (-1);
		}

		if( body2.file_size < 1 )
		{
			dprintf(0, "[ DB ]Error D_set_type_warp() file_size(%d) \n", body2.file_size );
			return (-1);
		}

		if( !(buf_ptr[i] = (char *)calloc((body2.file_size + 1), SIZEOF_CHAR)) )
		{
			dprintf(0, "[ DB ]Error D_set_type_warp() calloc() : %s\n", strerror(errno));
			return (-1);
		}

		if( U_RecvFromServer(initdb_fd, body2.file_size, buf_ptr[i], 3) )
		{
			dprintf(0, "[ DB ]Error D_set_type_warp() U_RecvFromServer() \n");
			return (-1);
		}

		if( (warp_type = U_get_line(buf_ptr[i], body2.file_size)) < 0 )
		{
			dprintf(0, "[ DB ]Error D_set_type_warp() file(%d), line(%d) \n", i, warp_type );
			return (-1);
		} 

		// comment is first's line
		warp_type	-= 1;

		D_type_warp_cnt	+= warp_type;
	}


	if( !(D_type_warp = (struct type_warp *)calloc((D_type_warp_cnt + 1), sizeof(struct type_warp))) )
	{
		dprintf(0, "[ DB ]Error D_set_type_warp() calloc() : %s\n", strerror(errno));
		return (-1);
	}


	warp_type = 1;

	for( i=0; i<body1.file_cnt; i++ )
	{

		//dprintf(0, "\t[ DB ]Loading ........... %d's type_warp \n", (i + 1) );

		// first line( not data )
		line_ptr = strtok( buf_ptr[i], "\n" );
		comment_row_cnt	= 0;
		while( (row_ptr = strsep(&line_ptr, "\t")) != NULL )
			comment_row_cnt ++;


		while( (line_ptr = strtok( NULL, "\n" )) != NULL )
		{
			line_row_cnt	= 0;

			while( (row_ptr = strsep(&line_ptr, "\t")) != NULL )
			{
                                switch( line_row_cnt )
                                {
					//case	 0 :	D_type_warp[warp_type].index		= atoi(row_ptr);	break;
					//case	 1 :	strncpy( D_type_warp[warp_type].name, 	row_ptr, 255 );		break;
					//case	 2 :	D_type_warp[warp_type].enter_flag	= atoi(row_ptr);	break;
					//case	 3 :	D_type_warp[warp_type].enter_level	= atoi(row_ptr);	break;
					//case	 4 :	D_type_warp[warp_type].enter_item	= atoi(row_ptr);	break;
					case	 5 :	D_type_warp[warp_type].start_sec	= atoi(row_ptr);	break;
					case	 6 :	D_type_warp[warp_type].end_sec		= atoi(row_ptr);	break;
					//case	 7 :	D_type_warp[warp_type].warp_map		= atoi(row_ptr);	break;
					//case	 8 :	D_type_warp[warp_type].warp_x		= atoi(row_ptr);	break;
					//case	 9 :	D_type_warp[warp_type].warp_z		= atoi(row_ptr);	break;
					//case	10 :	D_type_warp[warp_type].minimap_start_x	= atoi(row_ptr);	break;
					//case	11 :	D_type_warp[warp_type].minimap_start_z	= atoi(row_ptr);	break;
					//case	12 :	D_type_warp[warp_type].minimap_end_x	= atoi(row_ptr);	break;
					//case	13 :	D_type_warp[warp_type].minimap_end_z	= atoi(row_ptr);	break;
				}

				line_row_cnt ++;
			}

			if( line_row_cnt != comment_row_cnt )
			{
				dprintf(0, "[ DB ]Error D_set_type_warp() file(%d), line_row_cnt(%d, %d) \n"
					, (i + 1), line_row_cnt, comment_row_cnt );
				return (-1);
			}

			if( D_chk_type_warp(warp_type) )
			{
				dprintf(0, "[ DB ]Error D_set_type_warp() D_chk_type_warp(%d) \n", warp_type);
				return (-1);
			}

			if( D_type_warp_cnt < warp_type )
			{
				dprintf(0, "[ DB ]Error D_set_type_warp() warp_cnt(%d, %d) \n", D_type_warp_cnt, (warp_type - 1) );
				return (-1);
			}

			warp_type ++;
		}
	}


	for( j=0; j<body1.file_cnt; j++ )
		free(buf_ptr[j]);


	if( D_type_warp_cnt != (warp_type - 1) )
	{
		dprintf(0, "[ DB ]Error D_set_type_warp() total_warp_cnt(%d, %d) \n", D_type_warp_cnt, (warp_type - 1) );
		return (-1);
	}


	dprintf(0, "\t[ DB ]Success D_set_type_warp(%d)\n", D_type_warp_cnt);
	return (0);
}


static inline int	D_chk_type_warp( const int type_warp_id )
{

	if( (D_type_warp[type_warp_id].start_sec < -1) || (D_type_warp[type_warp_id].start_sec >= DAY) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_warp() : type_warp_id(%d), start_sec(%d) \n"
			, type_warp_id, D_type_warp[type_warp_id].start_sec );
		return (-1);
	}

	if( (D_type_warp[type_warp_id].end_sec < -1) || (D_type_warp[type_warp_id].end_sec >= DAY) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_warp() : type_warp_id(%d), end_sec(%d) \n"
			, type_warp_id, D_type_warp[type_warp_id].end_sec );
		return (-1);
	}

	return (0);
}


static inline int	D_set_type_store( const int initdb_fd )
{
	struct header_special				header;
	struct body1_SW_TYPE_STORE_FILES_RESULT		body1;
	struct body2_SW_TYPE_STORE_FILES_RESULT		body2;
	int						i, j, comment_row_cnt, line_row_cnt, store_type;
	char						*buf_ptr[MAX_SCRIPT_FILE_CNT];
	char						*line_ptr, *row_ptr;


	D_type_store_cnt	= 0;

	header.command		= WS_TYPE_STORE_FILES;
	header.body_size	= sizeof(struct body_WS_TYPE_STORE_FILES);
	header.fd		= 0;
	header.session		= 0;
	header.sec_active	= 0;

	if( U_SendToServer(initdb_fd, SIZEOF_HEADER_SPECIAL, (char *)&header, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_type_store() U_SendToServer() \n");
		return (-1);
	}

	if( U_RecvFromServer(initdb_fd, SIZEOF_HEADER_SPECIAL, (char *)&header, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_type_store() U_RecvFromServer() \n");
		return (-1);
	}

	if( header.command != SW_TYPE_STORE_FILES_RESULT )
	{
		dprintf(0, "[ DB ]Error D_set_type_store() header.command(%x, %x) \n", header.command, SW_TYPE_STORE_FILES_RESULT );
		return (-1);
	}

	if( U_RecvFromServer(initdb_fd, sizeof(struct body1_SW_TYPE_STORE_FILES_RESULT), (char *)&body1, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_type_store() U_RecvFromServer() \n");
		return (-1);
	}

	if( (body1.file_cnt < 0) || (body1.file_cnt > (MAX_SCRIPT_FILE_CNT - 1)) )
	{
		dprintf(0, "[ DB ]Error D_set_type_store() store_file_cnt(%d) \n", body1.file_cnt);
		return (-1);
	}


	for( i=0; i<body1.file_cnt; i++ )
	{

		if( U_RecvFromServer(initdb_fd, sizeof(struct body2_SW_TYPE_STORE_FILES_RESULT), (char *)&body2, 3) )
		{
			dprintf(0, "[ DB ]Error D_set_type_store() U_RecvFromServer() \n");
			return (-1);
		}

		if( body2.file_size < 1 )
		{
			dprintf(0, "[ DB ]Error D_set_type_store() file_size(%d) \n", body2.file_size );
			return (-1);
		}

		if( !(buf_ptr[i] = (char *)calloc((body2.file_size + 1), SIZEOF_CHAR)) )
		{
			dprintf(0, "[ DB ]Error D_set_type_store() calloc() : %s\n", strerror(errno));
			return (-1);
		}

		if( U_RecvFromServer(initdb_fd, body2.file_size, buf_ptr[i], 3) )
		{
			dprintf(0, "[ DB ]Error D_set_type_store() U_RecvFromServer() \n");
			return (-1);
		}

		if( (store_type = U_get_line(buf_ptr[i], body2.file_size)) < 0 )
		{
			dprintf(0, "[ DB ]Error D_set_type_store() file(%d), line(%d) \n", i, store_type );
			return (-1);
		} 

		// comment is first's line
		store_type	-= 1;

		D_type_store_cnt	+= store_type;
	}


	if( !(D_type_store = (struct type_store *)calloc((D_type_store_cnt + 1), sizeof(struct type_store))) )
	{
		dprintf(0, "[ DB ]Error D_set_type_store() calloc() : %s\n", strerror(errno));
		return (-1);
	}


	store_type = 1;

	for( i=0; i<body1.file_cnt; i++ )
	{

		//dprintf(0, "\t[ DB ]Loading ........... %d's type_store \n", (i + 1) );

		// first line( not data )
		line_ptr = strtok( buf_ptr[i], "\n" );
		comment_row_cnt	= 0;
		while( (row_ptr = strsep(&line_ptr, "\t")) != NULL )
			comment_row_cnt ++;


		while( (line_ptr = strtok( NULL, "\n" )) != NULL )
		{
			line_row_cnt	= 0;

			while( (row_ptr = strsep(&line_ptr, "\t")) != NULL )
			{
                                switch( line_row_cnt )
                                {
					//case	 0 :	D_type_store[store_type].index				= atoi(row_ptr);break;
					//case	 1 :	strncpy(D_type_store[store_type].name			, row_ptr, 256);break;
					//case	 2 :	D_type_store[store_type].item[0].item_type		= atoi(row_ptr);break;
					//case	 3 :	D_type_store[store_type].item[0].item_plus_price	= atoi(row_ptr);break;
					//case	 4 :	D_type_store[store_type].item[1].item_type		= atoi(row_ptr);break;
					//case	 5 :	D_type_store[store_type].item[1].item_plus_price	= atoi(row_ptr);break;
					//case	 6 :	D_type_store[store_type].item[2].item_type		= atoi(row_ptr);break;
					//case	 7 :	D_type_store[store_type].item[2].item_plus_price	= atoi(row_ptr);break;
					//case	 8 :	D_type_store[store_type].item[3].item_type		= atoi(row_ptr);break;
					//case 	 9 :	D_type_store[store_type].item[3].item_plus_price	= atoi(row_ptr);break;
					//case	10 :	D_type_store[store_type].item[4].item_type		= atoi(row_ptr);break;
					//case	11 :	D_type_store[store_type].item[4].item_plus_price	= atoi(row_ptr);break;
					//case	12 :	D_type_store[store_type].item[5].item_type		= atoi(row_ptr);break;
					//case	13 :	D_type_store[store_type].item[5].item_plus_price	= atoi(row_ptr);break;
					//case	14 :	D_type_store[store_type].item[6].item_type		= atoi(row_ptr);break;
					//case	15 :	D_type_store[store_type].item[6].item_plus_price	= atoi(row_ptr);break;
					//case	16 :	D_type_store[store_type].item[7].item_type		= atoi(row_ptr);break;
					//case	17 :	D_type_store[store_type].item[7].item_plus_price	= atoi(row_ptr);break;
					//case	18 :	D_type_store[store_type].item[8].item_type		= atoi(row_ptr);break;
					//case	19 :	D_type_store[store_type].item[8].item_plus_price	= atoi(row_ptr);break;
					//case	20 :	D_type_store[store_type].item[9].item_type		= atoi(row_ptr);break;
					//case	21 :	D_type_store[store_type].item[9].item_plus_price	= atoi(row_ptr);break;
					//case	22 :	D_type_store[store_type].item[10].item_type		= atoi(row_ptr);break;
					//case	23 :	D_type_store[store_type].item[10].item_plus_price	= atoi(row_ptr);break;
					//case	24 :	D_type_store[store_type].item[11].item_type		= atoi(row_ptr);break;
					//case	25 :	D_type_store[store_type].item[11].item_plus_price	= atoi(row_ptr);break;
					//case	26 :	D_type_store[store_type].item[12].item_type		= atoi(row_ptr);break;
					//case	27 :	D_type_store[store_type].item[12].item_plus_price	= atoi(row_ptr);break;
					//case	28 :	D_type_store[store_type].item[13].item_type		= atoi(row_ptr);break;
					//case	29 :	D_type_store[store_type].item[13].item_plus_price	= atoi(row_ptr);break;
					//case	30 :	D_type_store[store_type].item[14].item_type		= atoi(row_ptr);break;
					//case	31 :	D_type_store[store_type].item[14].item_plus_price	= atoi(row_ptr);break;
					//case	32 :	D_type_store[store_type].item[15].item_type		= atoi(row_ptr);break;
					//case	33 :	D_type_store[store_type].item[15].item_plus_price	= atoi(row_ptr);break;
					//case	34 :	D_type_store[store_type].item[16].item_type		= atoi(row_ptr);break;
					//case	35 :	D_type_store[store_type].item[16].item_plus_price	= atoi(row_ptr);break;
					//case	36 :	D_type_store[store_type].item[17].item_type		= atoi(row_ptr);break;
					//case	37 :	D_type_store[store_type].item[17].item_plus_price	= atoi(row_ptr);break;
					//case	38 :	D_type_store[store_type].item[18].item_type		= atoi(row_ptr);break;
					//case	39 :	D_type_store[store_type].item[18].item_plus_price	= atoi(row_ptr);break;
					//case	40 :	D_type_store[store_type].item[19].item_type		= atoi(row_ptr);break;
					//case	41 :	D_type_store[store_type].item[19].item_plus_price	= atoi(row_ptr);break;
					//case	42 :	D_type_store[store_type].item[20].item_type		= atoi(row_ptr);break;
					//case	43 :	D_type_store[store_type].item[20].item_plus_price	= atoi(row_ptr);break;
					//case	44 :	D_type_store[store_type].item[21].item_type		= atoi(row_ptr);break;
					//case	45 :	D_type_store[store_type].item[21].item_plus_price	= atoi(row_ptr);break;
					//case	46 :	D_type_store[store_type].item[22].item_type		= atoi(row_ptr);break;
					//case	47 :	D_type_store[store_type].item[22].item_plus_price	= atoi(row_ptr);break;
					//case	48 :	D_type_store[store_type].item[23].item_type		= atoi(row_ptr);break;
					//case	49 :	D_type_store[store_type].item[23].item_plus_price	= atoi(row_ptr);break;
					//case	50 :	D_type_store[store_type].item[24].item_type		= atoi(row_ptr);break;
					//case	51 :	D_type_store[store_type].item[24].item_plus_price	= atoi(row_ptr);break;
					//case	52 :	D_type_store[store_type].item[25].item_type		= atoi(row_ptr);break;
					//case	53 :	D_type_store[store_type].item[25].item_plus_price	= atoi(row_ptr);break;
					//case	54 :	D_type_store[store_type].item[26].item_type		= atoi(row_ptr);break;
					//case	55 :	D_type_store[store_type].item[26].item_plus_price	= atoi(row_ptr);break;
					//case	56 :	D_type_store[store_type].item[27].item_type		= atoi(row_ptr);break;
					//case	57 :	D_type_store[store_type].item[27].item_plus_price	= atoi(row_ptr);break;
					//case	58 :	D_type_store[store_type].item[28].item_type		= atoi(row_ptr);break;
					//case	59 :	D_type_store[store_type].item[28].item_plus_price	= atoi(row_ptr);break;
					//case	60 :	D_type_store[store_type].item[29].item_type		= atoi(row_ptr);break;
					//case	61 :	D_type_store[store_type].item[29].item_plus_price	= atoi(row_ptr);break;
					//case	62 :	D_type_store[store_type].item[30].item_type		= atoi(row_ptr);break;
					//case	63 :	D_type_store[store_type].item[30].item_plus_price	= atoi(row_ptr);break;
					//case	64 :	D_type_store[store_type].item[31].item_type		= atoi(row_ptr);break;
					//case	65 :	D_type_store[store_type].item[31].item_plus_price	= atoi(row_ptr);break;
					//case	66 :	D_type_store[store_type].item[32].item_type		= atoi(row_ptr);break;
					//case	67 :	D_type_store[store_type].item[32].item_plus_price	= atoi(row_ptr);break;
					//case	68 :	D_type_store[store_type].item[33].item_type		= atoi(row_ptr);break;
					//case	69 :	D_type_store[store_type].item[33].item_plus_price	= atoi(row_ptr);break;
					//case	70 :	D_type_store[store_type].item[34].item_type		= atoi(row_ptr);break;
					//case	71 :	D_type_store[store_type].item[34].item_plus_price	= atoi(row_ptr);break;
					//case	72 :	D_type_store[store_type].item[35].item_type		= atoi(row_ptr);break;
					//case	73 :	D_type_store[store_type].item[35].item_plus_price	= atoi(row_ptr);break;
					//case	74 :	D_type_store[store_type].item[36].item_type		= atoi(row_ptr);break;
					//case	75 :	D_type_store[store_type].item[36].item_plus_price	= atoi(row_ptr);break;
					//case	76 :	D_type_store[store_type].item[37].item_type		= atoi(row_ptr);break;
					//case	77 :	D_type_store[store_type].item[37].item_plus_price	= atoi(row_ptr);break;
					//case	78 :	D_type_store[store_type].item[38].item_type		= atoi(row_ptr);break;
					//case	79 :	D_type_store[store_type].item[38].item_plus_price	= atoi(row_ptr);break;
					//case	80 :	D_type_store[store_type].item[39].item_type		= atoi(row_ptr);break;
					//case	81 :	D_type_store[store_type].item[39].item_plus_price	= atoi(row_ptr);break;
					//case	82 :	D_type_store[store_type].item[40].item_type		= atoi(row_ptr);break;
					//case	83 :	D_type_store[store_type].item[40].item_plus_price	= atoi(row_ptr);break;
					//case	84 :	D_type_store[store_type].durability			= atoi(row_ptr);break;
					//case	85 :	D_type_store[store_type].durability_plus_price		= atoi(row_ptr);break;
				}

				line_row_cnt ++;
			}

			//if( line_row_cnt != comment_row_cnt )
			//{
			//	dprintf(0, "[ DB ]Error D_set_type_store() file(%d), line_row_cnt(%d, %d) \n"
			//		, (i + 1), line_row_cnt, comment_row_cnt );
			//	return (-1);
			//}

			if( D_chk_type_store(store_type) )
			{
				dprintf(0, "[ DB ]Error D_set_type_store() D_chk_type_warp(%d) \n", store_type);
				return (-1);
			}

			if( D_type_store_cnt < store_type )
			{
				dprintf(0, "[ DB ]Error D_set_type_store() store_cnt(%d, %d) \n", D_type_store_cnt, (store_type - 1) );
				return (-1);
			}

			store_type ++;
		}
	}


	for( j=0; j<body1.file_cnt; j++ )
		free(buf_ptr[j]);


	if( D_type_store_cnt != (store_type - 1) )
	{
		dprintf(0, "[ DB ]Error D_set_type_store() total_store_cnt(%d, %d) \n", D_type_store_cnt, (store_type - 1) );
		return (-1);
	}


	dprintf(0, "\t[ DB ]Success D_set_type_store(%d)\n", D_type_store_cnt);
	return (0);
}


static inline int	D_chk_type_store( const int type_store_id )
{

	return (0);
}


static inline int	D_set_type_mob_skill( const int initdb_fd )
{

	struct header_special				header;
	struct body1_SW_TYPE_MOB_SKILL_FILES_RESULT	body1;
	struct body2_SW_TYPE_MOB_SKILL_FILES_RESULT	body2;
	int						i, j, comment_row_cnt, line_row_cnt, skill_type;
	char						*buf_ptr[MAX_SCRIPT_FILE_CNT];
	char						*line_ptr, *row_ptr;


	D_type_mob_skill_cnt	= 0;

	header.command		= WS_TYPE_MOB_SKILL_FILES;
	header.body_size	= sizeof(struct body_WS_TYPE_MOB_SKILL_FILES);
	header.fd		= 0;
	header.session		= 0;
	header.sec_active	= 0;

	if( U_SendToServer(initdb_fd, SIZEOF_HEADER_SPECIAL, (char *)&header, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_type_mob_skill() U_SendToServer() \n");
		return (-1);
	}

	if( U_RecvFromServer(initdb_fd, SIZEOF_HEADER_SPECIAL, (char *)&header, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_type_mob_skill() U_RecvFromServer() \n");
		return (-1);
	}

	if( header.command != SW_TYPE_MOB_SKILL_FILES_RESULT )
	{
		dprintf(0, "[ DB ]Error D_set_type_mob_skill() header.command(%x, %x) \n", header.command, SW_TYPE_MOB_SKILL_FILES_RESULT );
		return (-1);
	}

	if( U_RecvFromServer(initdb_fd, sizeof(struct body1_SW_TYPE_MOB_SKILL_FILES_RESULT), (char *)&body1, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_type_mob_skill() U_RecvFromServer() \n");
		return (-1);
	}


	for( i=0; i<body1.file_cnt; i++ )
	{

		if( U_RecvFromServer(initdb_fd, sizeof(struct body2_SW_TYPE_MOB_SKILL_FILES_RESULT), (char *)&body2, 3) )
		{
			dprintf(0, "[ DB ]Error D_set_type_mob_skill() U_RecvFromServer() \n");
			return (-1);
		}

		if( body2.file_size < 1 )
		{
			dprintf(0, "[ DB ]Error D_set_type_mob_skill() file_size(%d) \n", body2.file_size );
			return (-1);
		}

		if( !(buf_ptr[i] = (char *)calloc((body2.file_size + 1), SIZEOF_CHAR)) )
		{
			dprintf(0, "[ DB ]Error D_set_type_mob_skill() calloc() : %s\n", strerror(errno));
			return (-1);
		}

		if( U_RecvFromServer(initdb_fd, body2.file_size, buf_ptr[i], 3) )
		{
			dprintf(0, "[ DB ]Error D_set_type_mob_skill() U_RecvFromServer() \n");
			return (-1);
		}

		if( (skill_type = U_get_line(buf_ptr[i], body2.file_size)) < 0 )
		{
			dprintf(0, "[ DB ]Error D_set_type_mob_skill() file(%d), skill_type_cnt(%d) \n", i, skill_type);
			return (-1);
		} 

		// comment is first's line
		skill_type 		-= 1;

		D_type_mob_skill_cnt	+= skill_type;
	}


	if( !(D_type_mob_skill = (struct type_mob_skill *)calloc((D_type_mob_skill_cnt + 1), sizeof(struct type_mob_skill))) )
	{
		dprintf(0, "[ DB ]Error D_set_type_mob_skill() calloc() : %s\n", strerror(errno));
		return (-1);
	}


	skill_type	= 1;

	for( i=0; i<body1.file_cnt; i++ )
	{

		//dprintf(0, "\t[ DB ]Loading ........... type_mob_skill \n");

		// first line( not data )
		line_ptr = strtok( buf_ptr[i], "\n" );
		comment_row_cnt	= 0;
		while( (row_ptr = strsep(&line_ptr, "\t")) != NULL )
			comment_row_cnt ++;


		while( (line_ptr = strtok( NULL, "\n" )) != NULL )
		{
			line_row_cnt	= 0;

			while( (row_ptr = strsep(&line_ptr, "\t")) != NULL )
			{
                                switch( line_row_cnt )
                                {
					//case	0 :	D_type_mob_skill[skill_type].index		= atoi(row_ptr);	break;
					//case	1 :	strncpy( D_type_mob_skill[skill_type].name, 	row_ptr, 255 );		break;
					case	2 :	D_type_mob_skill[skill_type].order[0].function	= atoi(row_ptr);	break;
					case	3 :	D_type_mob_skill[skill_type].order[1].function	= atoi(row_ptr);	break;
					case	4 :	D_type_mob_skill[skill_type].order[2].function	= atoi(row_ptr);	break;
					case	5 :	D_type_mob_skill[skill_type].order[0].damage	= atoi(row_ptr);	break;
					case	6 :	D_type_mob_skill[skill_type].order[1].damage	= atoi(row_ptr);	break;
					case	7 :	D_type_mob_skill[skill_type].order[2].damage	= atoi(row_ptr);	break;
					case	8 :	D_type_mob_skill[skill_type].order[0].sec	= atoi(row_ptr);	break;
					case	9 :	D_type_mob_skill[skill_type].order[1].sec	= atoi(row_ptr);	break;
					case	10 :	D_type_mob_skill[skill_type].order[2].sec	= atoi(row_ptr);	break;
					case	11 :	D_type_mob_skill[skill_type].action_cnt		= atoi(row_ptr);	break;
					case	12 :	D_type_mob_skill[skill_type].action_millisec	= atoi(row_ptr);	break;
					case	13 :	D_type_mob_skill[skill_type].use_ap		= atoi(row_ptr);	break;
					case	14 :	D_type_mob_skill[skill_type].target_object	= atoi(row_ptr);	break;
					case	15 :	D_type_mob_skill[skill_type].target_apply	= atoi(row_ptr);	break;
					case	16 :	D_type_mob_skill[skill_type].target_distance	= atoi(row_ptr);	break;
					case	17 :	D_type_mob_skill[skill_type].range_type		= atoi(row_ptr);	break;
					case	18 :	D_type_mob_skill[skill_type].range_x		= atoi(row_ptr);	break;
					case	19 :	D_type_mob_skill[skill_type].range_z		= atoi(row_ptr);	break;
					case	20 :	D_type_mob_skill[skill_type].casting_millisec	= atoi(row_ptr);	break;
					case	21 :	D_type_mob_skill[skill_type].ai_condition	= atoi(row_ptr);	break;
					//case	22 :	D_type_mob_skill[skill_type].motion_casting	= atoi(row_ptr);	break;
					//case	23 :	D_type_mob_skill[skill_type].motion_active	= atoi(row_ptr);	break;
					//case	24 :	D_type_mob_skill[skill_type].sound_hit		= atoi(row_ptr);	break;
					//case	25 :	strncpy( D_type_mob_skill[skill_type].monster,	row_ptr, 255 );		break;
					//case	26 :	strncpy( D_type_mob_skill[skill_type].scripts, 	row_ptr, 255 );		break;
				}

				line_row_cnt ++;
			}

			if( line_row_cnt != comment_row_cnt )
			{
				dprintf(0, "[ DB ]Error D_set_type_mob_skill() line_row_cnt(%d, %d) \n", line_row_cnt, comment_row_cnt );
				return (-1);
			}

			if( D_chk_type_mob_skill(skill_type) )
			{
				dprintf(0, "[ DB ]Error D_set_type_mob_skill() D_chk_type_mob_skill(%d) \n", skill_type );
				return (-1);
			}

			if( D_type_mob_skill_cnt < skill_type )
			{
				dprintf(0, "[ DB ]Error D_set_type_mob_skill() skill_cnt(%d, %d) \n",D_type_mob_skill_cnt, (skill_type - 1) );
				return (-1);
			}

			skill_type ++;
		}
	}
			

	for( j=0; j<body1.file_cnt; j++ )
		free(buf_ptr[j]);


	if( D_type_mob_skill_cnt != (skill_type - 1) )
	{
		dprintf(0, "[ DB ]Error D_set_type_mob_skill() total_skill_cnt(%d, %d) \n", D_type_mob_skill_cnt, (skill_type - 1) );
		return (-1);
	}


	dprintf(0, "\t[ DB ]Success D_set_type_mob_skill(%d)\n", D_type_mob_skill_cnt);
	return (0);
}


static inline int	D_chk_type_mob_skill( const int type )
{

	// check use_ap
	if( D_type_mob_skill[type].use_ap < 0 ) 
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob_skill() type(%d), use_ap(%d) \n"
			, type, D_type_mob_skill[type].use_ap );
		return (-1);
	}

	// check target_object
	if( (D_type_mob_skill[type].target_object < SKILL_TARGET_OBJECT_MYSELF) 
		|| (D_type_mob_skill[type].target_object > MAX_SKILL_TARGET_OBJECT) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob_skill() type(%d), target_object(%d) \n"
			, type, D_type_mob_skill[type].target_object );
		return (-1);
	}

	if( (D_type_mob_skill[type].target_object == SKILL_TARGET_OBJECT_MYSELF) && (D_type_mob_skill[type].target_distance) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob_skill() type(%d), SKILL_TARGET_OBJECT_MYSELF's target_distance(%d) \n"
			, type, D_type_mob_skill[type].target_distance );
		return (-1);
	}

	// check target_apply
	if( (D_type_mob_skill[type].target_apply < SKILL_TARGET_APPLY_MYSELF) 
		|| (D_type_mob_skill[type].target_apply > MAX_SKILL_TARGET_APPLY) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob_skill() type(%d), target_apply(%d) \n"
			, type, D_type_mob_skill[type].target_apply );
		return (-1);
	}

	// check target_distance
	if( (D_type_mob_skill[type].target_distance < 0) || (D_type_mob_skill[type].target_distance > (MAX_Z_TILE/2 + 1)) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob_skill() type(%d), target_distance(%d) \n"
			, type, D_type_mob_skill[type].target_distance );
		return (-1);
	}

	// check range_type
	if( (D_type_mob_skill[type].range_type < 0) || (D_type_mob_skill[type].range_type > MAX_SKILL_RANGE_TYPE) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob_skill() type(%d), range_type(%d) \n"
			, type, D_type_mob_skill[type].range_type );
		return (-1);
	}

	// check range_x, range_z
	if( D_type_mob_skill[type].range_type )
	{
		if( (D_type_mob_skill[type].range_x < 0) || (D_type_mob_skill[type].range_x > (MAX_X_TILE/2 + 1)) )
		{
			dprintf(0, "[ DB ]Error D_chk_type_mob_skill() type(%d), range_x(%d) \n"
				, type, D_type_mob_skill[type].range_x );
			return (-1);
		}

		if( (D_type_mob_skill[type].range_z < 0) || (D_type_mob_skill[type].range_z > (MAX_Z_TILE/2 + 1)) )
		{
			dprintf(0, "[ DB ]Error D_chk_type_mob_skill() type(%d), range_z(%d) \n"
				, type, D_type_mob_skill[type].range_z );
			return (-1);
		}
	}
	else
	{
		if( D_type_mob_skill[type].range_x )
		{
			dprintf(0, "[ DB ]Error D_chk_type_mob_skill() type(%d), range_x(%d) \n"
				, type, D_type_mob_skill[type].range_x );
			return (-1);
		}

		if( D_type_mob_skill[type].range_z )
		{
			dprintf(0, "[ DB ]Error D_chk_type_mob_skill() type(%d), range_z(%d) \n"
				, type, D_type_mob_skill[type].range_z );
			return (-1);
		}
	}

	// check casting_millisec
	if( (D_type_mob_skill[type].casting_millisec < 0) || (D_type_mob_skill[type].casting_millisec > MILLISEC_TO_DAY) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob_skill() type(%d), casting_millisec(%d) \n"
			, type, D_type_mob_skill[type].casting_millisec );
		return (-1);
	}

	// check ai_condition
	if( (D_type_mob_skill[type].ai_condition < 0) || (D_type_mob_skill[type].ai_condition > MAX_SKILL_AI_COND) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob_skill() type(%d), ai_condition(%d) \n", type, D_type_mob_skill[type].ai_condition );
		return (-1);
	}

	// check action_cnt
	if( (D_type_mob_skill[type].action_cnt < 1) || (D_type_mob_skill[type].action_cnt > 256) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob_skill() type(%d), action_cnt(%d) \n", type, D_type_mob_skill[type].action_cnt );
		return (-1);
	}

	// check action_millisec
	if( (D_type_mob_skill[type].action_millisec < 100) || (D_type_mob_skill[type].action_millisec > (MILLISEC_TO_DAY)) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob_skill() type(%d), action_millisec(%d) \n"
			, type, D_type_mob_skill[type].action_millisec );
		return (-1);
	}

	// set pattern
	D_type_mob_skill[type].pattern	|= (D_typemobskill_range_type(type)    << 0);
	D_type_mob_skill[type].pattern	|= (D_typemobskill_target_apply(type)  << 4);
	D_type_mob_skill[type].pattern	|= (D_typemobskill_target_object(type) << 8);

	switch( D_type_mob_skill[type].pattern )
	{
		case	SKILL_PATTERN_MY_MY_SINGLE :
		case	SKILL_PATTERN_MY_CHAR_CIRCLE :
		case	SKILL_PATTERN_MY_MOB_CIRCLE :
		case	SKILL_PATTERN_CHAR_CHAR_SINGLE :
		case	SKILL_PATTERN_CHAR_CHAR_CIRCLE :
		case	SKILL_PATTERN_CHAR_CHAR_SPRAY :
		case	SKILL_PATTERN_MOB_MOB_SINGLE :
		case	SKILL_PATTERN_MOB_MOB_CIRCLE :
		case	SKILL_PATTERN_MOB_MOB_SPRAY :
			break;
		default :
			dprintf(0, "[ DB ]Error D_chk_type_mob_skill() type(%d), not defined pattern(%x) \n"
				, type, D_type_mob_skill[type].pattern );
			return (-1);
	}

	return (0);
}


static inline int	D_set_type_mob( const int initdb_fd )
{
	struct header_special				header;
	struct body1_SW_TYPE_MOB_FILES_RESULT		body1;
	struct body2_SW_TYPE_MOB_FILES_RESULT		body2;
	int						i, j, comment_row_cnt, line_row_cnt, mob_type;
	char						*buf_ptr[MAX_SCRIPT_FILE_CNT];
	char						*line_ptr, *row_ptr;


	D_type_mob_cnt		= 0;

	header.command		= WS_TYPE_MOB_FILES;
	header.body_size	= sizeof(struct body_WS_TYPE_MOB_FILES);
	header.fd		= 0;
	header.session		= 0;
	header.sec_active	= 0;

	if( U_SendToServer(initdb_fd, SIZEOF_HEADER_SPECIAL, (char *)&header, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_type_mob() U_SendToServer() \n");
		return (-1);
	}

	if( U_RecvFromServer(initdb_fd, SIZEOF_HEADER_SPECIAL, (char *)&header, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_type_mob() U_RecvFromServer() \n");
		return (-1);
	}

	if( header.command != SW_TYPE_MOB_FILES_RESULT )
	{
		dprintf(0, "[ DB ]Error D_set_type_mob() header.command(%x, %x) \n", header.command, SW_TYPE_MOB_FILES_RESULT );
		return (-1);
	}

	if( U_RecvFromServer(initdb_fd, sizeof(struct body1_SW_TYPE_MOB_FILES_RESULT), (char *)&body1, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_type_mob() U_RecvFromServer() \n");
		return (-1);
	}

	if( (body1.file_cnt < 1) || (body1.file_cnt > (MAX_SCRIPT_FILE_CNT - 1)) )
	{
		dprintf(0, "[ DB ]Error D_set_type_mob() type_mob_file_cnt(%d) \n", body1.file_cnt);
		return (-1);
	}


	for( i=0; i<body1.file_cnt; i++ )
	{

		if( U_RecvFromServer(initdb_fd, sizeof(struct body2_SW_TYPE_MOB_FILES_RESULT), (char *)&body2, 3) )
		{
			dprintf(0, "[ DB ]Error D_set_type_mob() U_RecvFromServer() \n");
			return (-1);
		}

		if( body2.file_size < 1 )
		{
			dprintf(0, "[ DB ]Error D_set_type_mob() file_size(%d) \n", body2.file_size );
			return (-1);
		}

		if( !(buf_ptr[i] = (char *)calloc((body2.file_size + 1), SIZEOF_CHAR)) )
		{
			dprintf(0, "[ DB ]Error D_set_type_mob() calloc() : %s\n", strerror(errno));
			return (-1);
		}

		if( U_RecvFromServer(initdb_fd, body2.file_size, buf_ptr[i], 3) )
		{
			dprintf(0, "[ DB ]Error D_set_type_mob() U_RecvFromServer() \n");
			return (-1);
		}

		if( (mob_type = U_get_line(buf_ptr[i], body2.file_size)) < 0 )
		{
			dprintf(0, "[ DB ]Error D_set_type_mob() file(%d), line(%d) \n", i, mob_type);
			return (-1);
		} 

		// comment is first's line
		mob_type 	-= 1;

		D_type_mob_cnt += mob_type;
	}


	if( !(D_type_mob = (struct type_mob *)calloc((D_type_mob_cnt + 1), sizeof(struct type_mob))) )
	{
		dprintf(0, "[ DB ]Error D_set_type_mob() calloc() : %s\n", strerror(errno));
		return (-1);
	}


	mob_type = 1;

	for( i=0; i<body1.file_cnt; i++ )
	{

		//dprintf(0, "\t[ DB ]Loading ........... %d's typemob \n", (i + 1) );


		// first line( not data )
		line_ptr = strtok( buf_ptr[i], "\n" );
		comment_row_cnt	= 0;
		while( (row_ptr = strsep(&line_ptr, "\t")) != NULL )
			comment_row_cnt ++;


		while( (line_ptr = strtok( NULL, "\n" )) != NULL )
		{
			line_row_cnt	= 0;

			while( (row_ptr = strsep(&line_ptr, "\t")) != NULL )
			{
                                switch( line_row_cnt )
                                {
					//case	 0 :	D_type_mob[mob_type].index		= atoi(row_ptr);	break;	
					//case	 1 :	strncpy( D_type_mob[mob_type].name	, row_ptr, 255);	break;	
					//case	 2 :	D_type_mob[mob_type].level		= atoi(row_ptr);	break;
					case	 3 :	D_type_mob[mob_type].hp_max		= atoi(row_ptr);	break;
					case	 4 :	D_type_mob[mob_type].ap_max		= atoi(row_ptr);	break;
					//case	 5 :	D_type_mob[mob_type].exp		= atoi(row_ptr);	break;
					//case	 6 :	D_type_mob[mob_type].att_basic		= atoi(row_ptr);	break;
					//case	 7 :	D_type_mob[mob_type].def_basic		= atoi(row_ptr);	break;
					//case	 8 :	D_type_mob[mob_type].att2_basic		= atoi(row_ptr);	break;
					//case	 9 :	D_type_mob[mob_type].def2_basic		= atoi(row_ptr);	break;
					//case	10 :	D_type_mob[mob_type].eva_basic		= atoi(row_ptr);	break;
					//case	11 :	D_type_mob[mob_type].impact_short_def	= atoi(row_ptr);	break;
					//case	12 :	D_type_mob[mob_type].impact_long_def	= atoi(row_ptr);	break;
					//case	13 :	D_type_mob[mob_type].impact_att		= atoi(row_ptr);	break;
					//case	14 :	D_type_mob[mob_type].m_def_basic	= atoi(row_ptr);	break;
					//case	15 :	D_type_mob[mob_type].cri_basic		= atoi(row_ptr);	break;
					case	16 :	D_type_mob[mob_type].hp_recover		= atoi(row_ptr);	break;
					case	17 :	D_type_mob[mob_type].ap_recover		= atoi(row_ptr);	break;
					case	18 :	D_type_mob[mob_type].move_speed[0]	= atof(row_ptr);	break;
					case	19 :	D_type_mob[mob_type].move_speed[1]	= atof(row_ptr);	break;
					case	20 :	D_type_mob[mob_type].move_speed[2]	= atof(row_ptr);	break;
					case	21 :	D_type_mob[mob_type].milli_delay_attack	= atoi(row_ptr);	break;
					case	22 :	D_type_mob[mob_type].area_attack	= atoi(row_ptr);	break;
					//case	23 :	D_type_mob[mob_type].def_intensity	= atoi(row_ptr);	break;
					//case	24 :	D_type_mob[mob_type].att_intensity	= atoi(row_ptr);	break;
					case	25 :	D_type_mob[mob_type].skill[0].type	= atoi(row_ptr);	break;
					case	26 :	D_type_mob[mob_type].skill[0].rate	= atoi(row_ptr);	break;
					case	27 :	D_type_mob[mob_type].skill[1].type	= atoi(row_ptr);	break;
					case	28 :	D_type_mob[mob_type].skill[1].rate	= atoi(row_ptr);	break;
					case	29 :	D_type_mob[mob_type].skill[2].type	= atoi(row_ptr);	break;
					case	30 :	D_type_mob[mob_type].skill[2].rate	= atoi(row_ptr);	break;
					case	31 :	D_type_mob[mob_type].skill[3].type	= atoi(row_ptr);	break;
					case	32 :	D_type_mob[mob_type].skill[3].rate	= atoi(row_ptr);	break;
					case	33 :	D_type_mob[mob_type].skill[4].type	= atoi(row_ptr);	break;
					case	34 :	D_type_mob[mob_type].skill[4].rate	= atoi(row_ptr);	break;
					case	35 :	D_type_mob[mob_type].area_wander_w	= atoi(row_ptr);	break;
					case	36 :	D_type_mob[mob_type].area_wander_h	= atoi(row_ptr);	break;
					case	37 :	D_type_mob[mob_type].attack_type	= atoi(row_ptr);	break;
					case	38 :	D_type_mob[mob_type].area_vision	= atoi(row_ptr);	break;
					case	39 :	D_type_mob[mob_type].area_persuit	= atoi(row_ptr);	break;
					case	40 :	D_type_mob[mob_type].escape_hp		= atoi(row_ptr);	break;
					case	41 :	D_type_mob[mob_type].milli_delay_regen 	= atoi(row_ptr);	break;
					case	42 :	D_type_mob[mob_type].milli_delay_death	= atoi(row_ptr);	break;
					//case	43 :	D_type_mob[mob_type].elements		= atoi(row_ptr);	break;
					case	44 :	D_type_mob[mob_type].summon_cnt		= atoi(row_ptr);	break;
					//case	45 :	strcpy(D_type_mob[mob_type].talk	, row_ptr);		break;
					//case	46 :	D_type_mob[mob_type].graphic		= atoi(row_ptr);	break;	
					//case	47 :	D_type_mob[mob_type].scale		= atof(row_ptr);	break;
					//case	48 :	D_type_mob[mob_type].drop_type		= atoi(row_ptr);	break;
					//case	49 :	D_type_mob[mob_type].drop_money_rate	= atoi(row_ptr);	break;
					//case	50 :	D_type_mob[mob_type].drop_money_min	= atoi(row_ptr);	break;
					//case	51 :	D_type_mob[mob_type].drop_money_max	= atoi(row_ptr);	break;
					//case	52 :	D_type_mob[mob_type].quest_item_num	= atoi(row_ptr);	break;
					//case	53 :	D_type_mob[mob_type].quest_num		= atoi(row_ptr);	break;
					//case	54 :	D_type_mob[mob_type].quest_drop_rate	= atoi(row_ptr);	break;
					case	55 :	D_type_mob[mob_type].npc_type		= atoi(row_ptr);	break;
					//case	56 :	D_type_mob[mob_type].map_icon		= atoi(row_ptr);	break;
					//case	57 :	strcpy(D_type_mob[mob_type].regen_effect, atoi(row_ptr));	break;
					//case	58 :	D_type_mob[mob_type].mastery		= atoi(row_ptr);	break;
					//case	59 :	D_type_mob[mob_type].quest_list[0]	= atoi(row_ptr);	break;
					//case	60 :	D_type_mob[mob_type].quest_list[1]	= atoi(row_ptr);	break;
					//case	61 :	D_type_mob[mob_type].quest_list[2]	= atoi(row_ptr);	break;
					//case	62 :	D_type_mob[mob_type].quest_list[3]	= atoi(row_ptr);	break;
					//case	63 :	D_type_mob[mob_type].quest_list[4]	= atoi(row_ptr);	break;
					//case	64 :	D_type_mob[mob_type].quest_list[5]	= atoi(row_ptr);	break;
					//case	65 :	D_type_mob[mob_type].quest_list[6]	= atoi(row_ptr);	break;
					//case	66 :	D_type_mob[mob_type].quest_list[7]	= atoi(row_ptr);	break;
					//case	67 :	D_type_mob[mob_type].quest_list[8]	= atoi(row_ptr);	break;
					//case	68 :	D_type_mob[mob_type].quest_list[9]	= atoi(row_ptr);	break;
					//case	69 :	D_type_mob[mob_type].quest_list[10]	= atoi(row_ptr);	break;
					//case	70 :	D_type_mob[mob_type].quest_list[11]	= atoi(row_ptr);	break;
					//case	71 :	D_type_mob[mob_type].quest_list[12]	= atoi(row_ptr);	break;
					//case	72 :	D_type_mob[mob_type].quest_list[13]	= atoi(row_ptr);	break;
					//case	73 :	D_type_mob[mob_type].quest_list[14]	= atoi(row_ptr);	break;
				}

				line_row_cnt ++;
			}

			if( line_row_cnt != comment_row_cnt )
			{
				dprintf(0, "[ DB ]Error D_set_type_mob() type_mob file(%d), line_row_cnt(%d, %d) \n"
					, (i + 1), line_row_cnt, comment_row_cnt );
				return (-1);
			}

			if( D_chk_type_mob(mob_type) )
			{
				dprintf(0, "[ DB ]Error D_set_type_mob() D_chk_type_mob(%d)\n", mob_type);
				return (-1);
			}

			if( D_type_mob_cnt < mob_type )
			{
				dprintf(0, "[ DB ]Error D_set_type_mob() type_mob_cnt(%d, %d) \n", D_type_mob_cnt, (mob_type - 1) );
				return (-1);
			}

			mob_type ++;
		}
	}


	for( j=0; j<body1.file_cnt; j++ )
		free(buf_ptr[j]);


	if( D_type_mob_cnt != (mob_type - 1) )
	{
		dprintf(0, "[ DB ]Error D_set_type_mob() total_type_mob_cnt(%d, %d) \n", D_type_mob_cnt, (mob_type - 1) );
		return (-1);
	}

	dprintf(0, "\t[ DB ]Success D_set_type_mob(%d)\n", D_type_mob_cnt);
	return (0);
}


static inline int	D_chk_type_mob( const int mob_type )
{
	int		i, temp_skill_type, temp_skill_rate, temp_ai_condition;


	// hp_max
	if( (D_type_mob[mob_type].hp_max < 1) || (D_type_mob[mob_type].hp_max > 100000000) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), hp_max(%d) \n", mob_type, D_type_mob[mob_type].hp_max );
		return (-1);
	}

	// ap_max
	if( (D_type_mob[mob_type].ap_max < 0) || (D_type_mob[mob_type].ap_max > 100000000) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), ap_max(%d) \n", mob_type, D_type_mob[mob_type].ap_max );
		return (-1);
	}

	// hp_recover
	if( (D_type_mob[mob_type].hp_recover < 0) || (D_type_mob[mob_type].hp_recover > D_type_mob[mob_type].hp_max) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), hp_recover(%d) \n", mob_type, D_type_mob[mob_type].hp_recover );
		return (-1);
	}

	// ap_recover
	if( (D_type_mob[mob_type].ap_recover < 0) || (D_type_mob[mob_type].ap_recover > D_type_mob[mob_type].hp_max) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), ap_recover(%d) \n", mob_type, D_type_mob[mob_type].ap_recover );
		return (-1);
	}

	// speed_move_wander
	if( (D_type_mob[mob_type].move_speed[0] < 0.0f) || (D_type_mob[mob_type].move_speed[0] > 100.0f) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), move_speed grade_0(%f) \n"
			, mob_type, D_type_mob[mob_type].move_speed[0] );
		return (-1);
	}

	// speed_move_persuit
	if( (D_type_mob[mob_type].move_speed[1] < 0.0f) || (D_type_mob[mob_type].move_speed[1] > 100.0f) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), move_speed grade_1(%f) \n"
			, mob_type, D_type_mob[mob_type].move_speed[1] );
		return (-1);
	}

	// speed_move_escape
	if( (D_type_mob[mob_type].move_speed[2] < 0.0f) || (D_type_mob[mob_type].move_speed[2] > 100.0f) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), move_speed grade_2(%f) \n"
			, mob_type, D_type_mob[mob_type].move_speed[2] );
		return (-1);
	}

	// milli_delay_attack
	if( (D_type_mob[mob_type].milli_delay_attack < 800) || (D_type_mob[mob_type].milli_delay_attack > 10000) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), milli_delay_attack(%d) \n"
			, mob_type, D_type_mob[mob_type].milli_delay_attack );
		return (-1);
	}

	// area_attack
	if( (D_type_mob[mob_type].area_attack < 0) || (D_type_mob[mob_type].area_attack > 10) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), area_attack(%d) \n"
			, mob_type, D_type_mob[mob_type].area_attack );
		return (-1);
	}

	for( i=0; i<MAX_MOB_HAVE_SKILL_CNT; i++ )
	{
		// skill_type
		if( (D_type_mob[mob_type].skill[i].type < 0) || (D_type_mob[mob_type].skill[i].type > D_type_mob_skill_cnt) )
		{
			dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), skill_type_%d(%d) \n"
				, mob_type, i, D_type_mob[mob_type].skill[i].type );
			return (-1);
		}

		// set have_skill
		if( D_type_mob[mob_type].skill[i].type )
		{
			temp_skill_type		= D_type_mob[mob_type].skill[i].type;
			temp_skill_rate		= D_type_mob[mob_type].skill[i].rate;

			// skill rate
			if( (temp_skill_rate < 1 ) || (temp_skill_rate > 100) )
			{
				dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), skill_type(%d), rate(%d) \n"
					, mob_type, temp_skill_type, temp_skill_rate );
				return (-1);
			}

			// skill use_ap
			if( D_type_mob[mob_type].ap_max < D_type_mob_skill[temp_skill_type].use_ap )
			{
				dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), skill_type(%d), ap_max(%d), use_ap(%d) \n"
					, mob_type, temp_skill_type, D_type_mob[mob_type].ap_max
					, D_type_mob_skill[temp_skill_type].use_ap );
				return (-1);
			}

			// skill ai_conditon
			temp_ai_condition	= D_type_mob_skill[temp_skill_type].ai_condition;

			if( (temp_ai_condition < 1) || (temp_ai_condition > MAX_SKILL_AI_COND) )
			{
				dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), ai_condition(%d) \n"
					, mob_type, temp_ai_condition );
				return (-1);
			}

			D_type_mob[mob_type].have_skill[temp_ai_condition]	= 1;
		}
	}

	// area_wander_w
	if( (D_type_mob[mob_type].area_wander_w < 0) || (D_type_mob[mob_type].area_wander_w > (MAX_X_TILE/2 + 1)) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), area_wander_w(%d) \n"
			, mob_type, D_type_mob[mob_type].area_wander_w );
		return (-1);
	}

	// area_wander_h
	if( (D_type_mob[mob_type].area_wander_h < 0) || (D_type_mob[mob_type].area_wander_h > (MAX_Z_TILE/2 + 1)) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), area_wander_h(%d) \n"
			, mob_type, D_type_mob[mob_type].area_wander_h );
		return (-1);
	}

	// attack_type
	if( (D_type_mob[mob_type].attack_type < 0) || (D_type_mob[mob_type].attack_type > 100) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), attack_type(%d) \n"
			, mob_type, D_type_mob[mob_type].attack_type );
		return (-1);
	}

	// area_vision
	if( (D_type_mob[mob_type].area_vision < 0) || (D_type_mob[mob_type].area_vision > 8) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), area_vision(%d) \n"
			, mob_type, D_type_mob[mob_type].area_vision );
		return (-1);
	}

	// area_persuit
	if( (D_type_mob[mob_type].area_persuit < 0) || (D_type_mob[mob_type].area_persuit > (MAX_X_TILE/2 + 1)) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), area_persuit(%d) \n"
			, mob_type, D_type_mob[mob_type].area_persuit );
		return (-1);
	}

	// area_check
	if( D_type_mob[mob_type].area_persuit < D_type_mob[mob_type].area_vision )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), area_persuit(%d), vision(%d) \n"
			, mob_type, D_type_mob[mob_type].area_persuit, D_type_mob[mob_type].area_vision );
		return (-1);
	}

	// area_check
	if( D_type_mob[mob_type].area_persuit < D_type_mob[mob_type].area_wander_h )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), area_persuit(%d), wander_h(%d) \n"
			, mob_type, D_type_mob[mob_type].area_persuit, D_type_mob[mob_type].area_wander_h );
		return (-1);
	}

	// area_check
	if( D_type_mob[mob_type].area_persuit < D_type_mob[mob_type].area_wander_w )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), area_persuit(%d), wander_w(%d) \n"
			, mob_type, D_type_mob[mob_type].area_persuit, D_type_mob[mob_type].area_wander_w );
		return (-1);
	}

	// escape_hp
	if( (D_type_mob[mob_type].escape_hp < 0) || (D_type_mob[mob_type].escape_hp >= D_type_mob[mob_type].hp_max) )
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), escape_hp(%d), hp_max(%d) \n"
			, mob_type, D_type_mob[mob_type].area_persuit, D_type_mob[mob_type].hp_max );
		return (-1);
	}

	// milli_delay_regen
	if( (D_type_mob[mob_type].milli_delay_regen < 1000) || (D_type_mob[mob_type].milli_delay_regen > 100000000) ) 
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), milli_delay_regen(%d) \n"
			, mob_type, D_type_mob[mob_type].milli_delay_regen );
		return (-1);
	}

	// milli_delay_death
	if( (D_type_mob[mob_type].milli_delay_death < 1000) || (D_type_mob[mob_type].milli_delay_death > 100000000) ) 
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), milli_delay_death(%d) \n"
			, mob_type, D_type_mob[mob_type].milli_delay_death );
		return (-1);
	}

	// summon_cnt
	if( (D_type_mob[mob_type].summon_cnt < 0) || (D_type_mob[mob_type].summon_cnt > 5) ) 
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), summon_cnt(%d) \n"
			, mob_type, D_type_mob[mob_type].summon_cnt );
		return (-1);
	}

	// npc_type
	if( (D_type_mob[mob_type].npc_type < NPC_TYPE_MONSTER) || (D_type_mob[mob_type].npc_type > NPC_TYPE_CNT) ) 
	{
		dprintf(0, "[ DB ]Error D_chk_type_mob() mob_type(%d), npc_type(%d) \n"
			, mob_type, D_type_mob[mob_type].npc_type );
		return (-1);
	}

	return (0);
}


static inline int	D_set_mob( const int initdb_fd )
{
	struct header_special			header;
	struct body1_SW_MOB_FILES_RESULT	body1;
	struct body2_SW_MOB_FILES_RESULT	body2;
	int					i, j, line_row_cnt, map_mob_cnt, map_typemob_cnt, line_mob_cnt, line_mob_type, temp_mob_cnt;
	int					channel, regen_mob, regen_map, regen_x, regen_z, regen_dir, table_idx, mob_id;
	char					*buf_ptr[MAX_SCRIPT_FILE_CNT];
	char					*chk_ptr[MAX_SCRIPT_FILE_CNT];
	char					*line_ptr, *row_ptr;


	regen_x = regen_z = regen_dir = table_idx = 0;

	D_mob_cnt[0]		= 0;
	D_mob_cnt[1]		= 0;

	header.command		= WS_MOB_FILES;
	header.body_size	= sizeof(struct body_WS_MOB_FILES);
	header.fd		= 0;
	header.session		= 0;
	header.sec_active	= 0;

	if( U_SendToServer(initdb_fd, SIZEOF_HEADER_SPECIAL, (char *)&header, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_mob() U_SendToServer() \n");
		return (-1);
	}

	if( U_RecvFromServer(initdb_fd, SIZEOF_HEADER_SPECIAL, (char *)&header, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_mob() U_RecvFromServer() \n");
		return (-1);
	}

	if( header.command != SW_MOB_FILES_RESULT )
	{
		dprintf(0, "[ DB ]Error D_set_mob() header.command(%x, %x) \n", header.command, SW_MOB_FILES_RESULT );
		return (-1);
	}

	if( U_RecvFromServer(initdb_fd, sizeof(struct body1_SW_MOB_FILES_RESULT), (char *)&body1, 3) )
	{
		dprintf(0, "[ DB ]Error D_set_mob() U_RecvFromServer() \n");
		return (-1);
	}

	if( body1.file_cnt != D_map_cnt )
	{
		dprintf(0, "[ DB ]Error D_set_mob() mob_file_cnt(%d), D_map_cnt(%d) \n", body1.file_cnt, D_map_cnt);
		return (-1);
	}

	// normal_channel mob
	for( i=0; i<body1.file_cnt; i++ )
	{

		regen_map	= (i + 1);

		if( U_RecvFromServer(initdb_fd, sizeof(struct body2_SW_MOB_FILES_RESULT), (char *)&body2, 3) )
		{
			dprintf(0, "[ DB ]Error D_set_mob() U_RecvFromServer() \n");
			return (-1);
		}

		if( body2.file_size < 1 )
		{
			dprintf(0, "[ DB ]Error D_set_mob() file_size(%d) \n", body2.file_size );
			return (-1);
		}

		if( !(chk_ptr[i] = (char *)calloc((body2.file_size + 1), SIZEOF_CHAR)) )
		{
			dprintf(0, "[ DB ]Error D_set_mob() calloc() : %s\n", strerror(errno));
			return (-1);
		}

		if( !(buf_ptr[i] = (char *)calloc((body2.file_size + 1), SIZEOF_CHAR)) )
		{
			dprintf(0, "[ DB ]Error D_set_mob() calloc() : %s\n", strerror(errno));
			return (-1);
		}

		if( U_RecvFromServer(initdb_fd, body2.file_size, chk_ptr[i], 3) )
		{
			dprintf(0, "[ DB ]Error D_set_mob() U_RecvFromServer() \n");
			return (-1);
		}

		memcpy( buf_ptr[i], chk_ptr[i], body2.file_size + 1 );


		if( (line_ptr = strtok(chk_ptr[i], "\n")) == NULL )
		{
			dprintf(0, "[ DB ]Error D_set_mob() read map_mob_cnt \n" );
			return (-1);
		}

		map_mob_cnt	= atoi(line_ptr);

		D_map[regen_map].start_mob_id[0]	= (D_mob_cnt[0] + 1);
		D_mob_cnt[0]				+= map_mob_cnt;
		D_map[regen_map].start_summon_mob_id[0]	= (D_mob_cnt[0] + 1);
		D_mob_cnt[0]				+= MAP_SUMMON_MOB_CNT;
		D_map[regen_map].end_mob_id[0]		= (D_mob_cnt[0]);
	}

	// TODO :
	// ins_channel mob
	for( i=0; i<0; i++ )
	{
		regen_map	= (D_ins_start_map + i);

		D_map[regen_map].start_mob_id[1]	= -1;
		D_map[regen_map].start_summon_mob_id[1]	= -2;
		D_map[regen_map].end_mob_id[1]		= -3;
	}

	for( i=0; i<0; i++ )
	{
		regen_map	= (D_ins_start_map + i);

		D_map[regen_map].start_mob_id[1]	= (D_mob_cnt[1] + 1);
		D_mob_cnt[1]				+= 0;
		D_map[regen_map].start_summon_mob_id[1]	= (D_mob_cnt[1] + 1);
		D_mob_cnt[1]				+= MAP_SUMMON_MOB_CNT;
		D_map[regen_map].end_mob_id[1]		= (D_mob_cnt[1]);
	}


	if( (D_mob_cnt[0] < 1 || D_mob_cnt[0] > 65534) || (D_mob_cnt[1] < 0 || D_mob_cnt[1] > 65534) )
	{
		dprintf(0, "[ DB ]Error D_mob_cnt(%d, %d)\n", D_mob_cnt[0], D_mob_cnt[1] );
		return (-1);
	}


	// calloc
	for( i=1; i<=TOTAL_CHANNEL_CNT; i++ ) 
	{
		if( (D_mob[i] = calloc((D_mob_cnt[G_channel_ins[i]] + 1), sizeof(struct mob))) == NULL )
		{
			dprintf(0, "[ DB ]Error D_set_mob() calloc() : %s\n", strerror(errno));
			return (-1);
		}
	}


	// set normal_mob
	for( i=0; i<body1.file_cnt; i++ )
	{

		// map_mob_cnt
		if( (line_ptr = strtok( buf_ptr[i], "\n" )) == NULL )
		{
			dprintf(0, "[ DB ]Error D_set_mob() read map_mob_cnt \n" );
			return (-1);
		}

		if( (map_mob_cnt = atoi(line_ptr)) == 0 )
			continue;


		regen_map	= (i + 1);
		regen_mob 	= D_map[regen_map].start_mob_id[0];

		// map_typemob_cnt
		if( (line_ptr = strtok( NULL, "\n" )) == NULL )
		{
			dprintf(0, "[ DB ]Error D_set_mob() read map_typemob_cnt \n");
			return (-1);
		}

		if( (map_typemob_cnt = atoi(line_ptr)) == 0 )
		{
			dprintf(0, "\t[ DB ]Error D_set_mob() map_typemob_cnt=0 \n" );
			return (-1);
		}


		while( (line_ptr= strtok( NULL, "\n" )) != NULL )
		{

			// line_mob_type
			if( (row_ptr = strsep(&line_ptr, "\t")) == NULL )
			{
				dprintf(0, "[ DB ]Error D_set_mob() read line_mob_type_cnt\n" );
				return (-1);
			}

			line_mob_type	= atoi(row_ptr);

			if( (line_mob_type < 1) || (line_mob_type > D_type_mob_cnt) )
			{
				dprintf(0, "[ DB ]Error D_set_mob() line_mob_type_cnt(%d), D_type_mob_cnt(%d) \n"
					, line_mob_type, D_type_mob_cnt );
				return (-1);
			}


			// line_mob_cnt
			if( (row_ptr = strsep(&line_ptr, "\t")) == NULL )
			{
				dprintf(0, "[ DB ]Error D_set_mob() read line_mob_cnt \n");
				return (-1);
			}

			line_mob_cnt	= atoi(row_ptr);

			if( (line_mob_cnt < 1) || (line_mob_cnt > 1024) )
			{
				dprintf(0, "[ DB ]Error D_set_mob() line_mob_cnt(%d) \n", line_mob_cnt );
				return (-1);
			}


			temp_mob_cnt	= 0;
			line_row_cnt	= 0;

			while( (row_ptr = strsep(&line_ptr, "\t")) != NULL )
			{
                                switch( line_row_cnt )
                                {
					case	0 :	regen_x		= atoi(row_ptr);		break;
					case	1 :	regen_z		= atoi(row_ptr);		break;
					case	2 :	regen_dir	= atoi(row_ptr);		break;
					case	3 :	table_idx	= atoi(row_ptr);		break;
				}

				if( (++line_row_cnt % 4) != 0 )
					continue;

				line_row_cnt    = 0;

				for( channel=1; channel<=NORMAL_CHANNEL_CNT; channel++ )
				{
					if( !D_set_mob_regen(channel, regen_mob, line_mob_type, regen_map, regen_x, regen_z
						, regen_dir, table_idx) )
					{
						dprintf(0, "[ DB ]Error D_set_mob() map(%d), mob_type(%d), table_idx(%d)\n"
							, regen_map, line_mob_type, table_idx);
						return (-1);
					}
				}

				if( D_map[regen_map].end_mob_id[0] < regen_mob )
				{
					dprintf(0, "[ DB ]Error D_set_mob() end_mob_id(%d), regen_mob(%d) \n"
						, D_map[regen_map].end_mob_id[0], regen_mob);
					return (-1);
				}

				map_mob_cnt --;
				temp_mob_cnt ++;
				regen_mob ++;

			}// end while !!

			if( line_row_cnt != 0 )
			{
				dprintf(0, "[ DB ]Error D_set_mob() line_row_cnt(%d) \n", line_row_cnt );
				return (-1);
			}

			if( temp_mob_cnt != line_mob_cnt )
			{
				dprintf(0, "[ DB ]Error D_set_mob() line_mob_cnt(%d), temp_mob_cnt(%d) \n", line_mob_cnt, temp_mob_cnt );
				return (-1);
			}

			map_typemob_cnt --;

		}// end while !!


		if( map_mob_cnt != 0 )
		{
			dprintf(0, "[ DB ]Error D_set_mob() map_mob_cnt(%d) \n", map_mob_cnt );
			return (-1);
		}

		if( map_typemob_cnt != 0 )
		{
			dprintf(0, "[ DB ]Error D_set_mob() map_typemob_cnt(%d) \n", map_typemob_cnt );
			return (-1);
		}

		// set summon_mob
		for( channel=1; channel<=TOTAL_CHANNEL_CNT; channel++ )
		{
			if( D_check_invalid_link(channel, regen_map) )
				continue;

			for( mob_id=D_map_summon_start_mob(channel, regen_map);
						mob_id<=D_map_end_mob(channel, regen_map); mob_id++ )
				D_SetMap_empty_link_insert( channel, regen_map, mob_id );
		}

	}// end for !!


	for( j=0; j<body1.file_cnt; j++ )
	{
		free(buf_ptr[j]);
		free(chk_ptr[j]);
	}


	dprintf(0, "\t[ DB ]Success D_set_mob(%d, %d)\n", D_mob_cnt[0], D_mob_cnt[1]);
	return (0);
}


static inline int	D_chk_all_script( void )
{
	int	type, order;

	// check type_mob_skill table
	for( type=1; type<=D_type_mob_skill_cnt; type ++ )
	{
		for( order=0; order<MAX_SKILL_ORDER_CNT; order ++ )
		{
			if( (D_type_mob_skill[type].order[order].function < 0) 
				|| (D_type_mob_skill[type].order[order].function > MAX_SKILL_FUNC_TYPE) )
			{
				dprintf(0, "[ DB ]Error D_chk_all_script() type(%d), function(%d) \n"
					, type, D_type_mob_skill[type].order[order].function );
				return (-1);
			}

			switch( D_type_mob_skill[type].order[order].function )
			{
				case	SKILL_FUNC_SUMMON :
					if( (D_type_mob_skill[type].order[order].damage < 1) 
						|| (D_type_mob_skill[type].order[order].damage > D_type_mob_cnt) ) 
					{
						dprintf(0, "[ DB ]Error D_chk_all_script() type(%d), function_%d's damage(%d)\n"
							, type, order, D_type_mob_skill[type].order[order].damage );
						return (-1);
					}
					D_type_mob_skill[type].summon	= 1;
					break;
			}
		}
	}

	return (0);
}


static inline int	D_set_mob_regen( const int channel, const int mob_id, const int mob_type, const int regen_map
			, const int regen_x, const int regen_z, const int regen_dir, const int table_idx )
{

	D_mob[channel][mob_id].type		= mob_type;
	D_mob[channel][mob_id].regen_map	= regen_map;
	D_mob[channel][mob_id].map		= D_mob[channel][mob_id].regen_map;
	D_mob[channel][mob_id].regen_x		= regen_x;
	D_mob[channel][mob_id].regen_z		= regen_z;
	D_mob[channel][mob_id].regen_dir	= regen_dir;
	D_mob[channel][mob_id].table_idx	= table_idx;


        if( (D_mob[channel][mob_id].type < 1) || (D_mob[channel][mob_id].type > D_type_mob_cnt) )
        {
                dprintf(0, "[ DB ]Error D_set_mob_regen() mob_id(%d), type(%d), D_type_mob_cnt(%d) \n"
                        , mob_id, D_mob[channel][mob_id].regen_map, D_type_mob_cnt );
                return (0);
        }

        if( (D_mob[channel][mob_id].regen_map < 1) || (D_mob[channel][mob_id].regen_map > D_map_cnt) )
        {
                dprintf(0, "[ DB ]Error D_set_mob_regen() mob_id(%d), regen_map(%d), D_map_cnt(%d) \n"
                        , mob_id, D_mob[channel][mob_id].regen_map, D_map_cnt );
                return (0);
        }

        if( (D_mob[channel][mob_id].regen_x < 2) || (D_mob[channel][mob_id].regen_x > (MAX_X_TILE - 3)) )
        {
                dprintf(0, "[ DB ]Error D_set_mob_regen() regen_x mob_id(%d), regen_x(%d) \n"
                        , mob_id, D_mob[channel][mob_id].regen_x );
                return (0);
        }

        if( (D_mob[channel][mob_id].regen_z < 2) || (D_mob[channel][mob_id].regen_z > (MAX_Z_TILE - 3)) )
        {
                dprintf(0, "[ DB ]Error D_set_mob_regen() mob_id(%d), regen_z(%d) \n"
                        , mob_id, D_mob[channel][mob_id].regen_z );
                return (0);
        }

        if( (D_mob[channel][mob_id].regen_dir < 0) || (D_mob[channel][mob_id].regen_dir >= 360) )
        {
                dprintf(0, "[ DB ]Error D_set_mob_regen() mob_id(%d), regen_dir(%d) \n"
                        , mob_id, D_mob[channel][mob_id].regen_dir );
                return (0);
        }

        if( (D_mob[channel][mob_id].collission = D_GetMap_collission(D_mob[channel][mob_id].regen_map
		, D_mob[channel][mob_id].regen_x, D_mob[channel][mob_id].regen_z)) < 0 )
        {
                dprintf(0, "[ DB ]Error D_set_mob_regen() mob_id(%d), regen_map(%d), regen_xz(%d, %d), collission(%d) \n"
                        , mob_id, D_mob[channel][mob_id].regen_map, D_mob[channel][mob_id].regen_x
			, D_mob[channel][mob_id].regen_z, D_mob[channel][mob_id].collission );
                return (0);
        }


        switch( D_GetMob_npc_type(channel, mob_id) )
        {
                case    NPC_TYPE_WARP :
                        if( (D_mob[channel][mob_id].table_idx < 1) || (D_mob[channel][mob_id].table_idx > D_type_warp_cnt) )
                        {
                                dprintf(0, "[ DB ]Error D_set_mob_regen() mob_id(%d), table_idx(%d), D_type_warp_cnt(%d) \n"
                                        , mob_id, D_mob[channel][mob_id].table_idx, D_type_warp_cnt );
                		return (0);
                        }
                        break;
                case    NPC_TYPE_STORE :
                        if( (D_mob[channel][mob_id].table_idx < 1) || (D_mob[channel][mob_id].table_idx > D_type_store_cnt) )
                        {
                                dprintf(0, "[ DB ]Error D_set_mob_regen() mob_id(%d), table_idx(%d), D_type_store_cnt(%d) \n"
                                        , mob_id, D_mob[channel][mob_id].table_idx, D_type_store_cnt );
                		return (0);
                        }
                        break;
                case    NPC_TYPE_NORMAL_NPC :
                case    NPC_TYPE_MONSTER :
                        break;
                default :
                        dprintf(0, "[ DB ]Error D_set_mob_regen() mob_id(%d), npc_type(%d) \n", mob_id, D_GetMob_npc_type(channel, mob_id) );
                	return (0);
        }


        if( !D_set_mob_move_area(channel, mob_id) )
        {
                dprintf(0, "[ DB ]Error D_set_mob_move_area() channel(%d), mob_id(%d) \n", channel, mob_id );
               	return (0);
        }

	return (1);
}


static inline int	D_set_mob_move_area( const int channel, const int mob_id )
{
        // set area_wander
        D_mob[channel][mob_id].area_wander_sx    = (D_mob[channel][mob_id].regen_x - D_typemob_area_wander_w(D_mob[channel][mob_id].type));
        D_mob[channel][mob_id].area_wander_sz    = (D_mob[channel][mob_id].regen_z - D_typemob_area_wander_h(D_mob[channel][mob_id].type));
        D_mob[channel][mob_id].area_wander_ex    = (D_mob[channel][mob_id].regen_x + D_typemob_area_wander_w(D_mob[channel][mob_id].type));
        D_mob[channel][mob_id].area_wander_ez    = (D_mob[channel][mob_id].regen_z + D_typemob_area_wander_h(D_mob[channel][mob_id].type));

        D_SetMap_limit_xz( D_mob[channel][mob_id].map, &D_mob[channel][mob_id].area_wander_sx, &D_mob[channel][mob_id].area_wander_sz
                , &D_mob[channel][mob_id].area_wander_ex, &D_mob[channel][mob_id].area_wander_ez );

        // set area_persuit
        D_mob[channel][mob_id].area_persuit_sx   = (D_mob[channel][mob_id].regen_x - D_typemob_area_persuit(D_mob[channel][mob_id].type));
        D_mob[channel][mob_id].area_persuit_sz   = (D_mob[channel][mob_id].regen_z - D_typemob_area_persuit(D_mob[channel][mob_id].type));
        D_mob[channel][mob_id].area_persuit_ex   = (D_mob[channel][mob_id].regen_x + D_typemob_area_persuit(D_mob[channel][mob_id].type));
        D_mob[channel][mob_id].area_persuit_ez   = (D_mob[channel][mob_id].regen_z + D_typemob_area_persuit(D_mob[channel][mob_id].type));

        D_SetMap_limit_xz( D_mob[channel][mob_id].map, &D_mob[channel][mob_id].area_persuit_sx, &D_mob[channel][mob_id].area_persuit_sz
                , &D_mob[channel][mob_id].area_persuit_ex, &D_mob[channel][mob_id].area_persuit_ez );

	return (1);
}





//______________________________________________________________________________________
// type_warp 
static inline int	D_typewarp_start_sec( const int warp_id )
{
	return (D_type_warp[warp_id].start_sec);
}


static inline int	D_typewarp_end_sec( const int warp_id )
{
	return (D_type_warp[warp_id].end_sec);
}





//______________________________________________________________________________________
// type_mob_skill
static inline int	D_typemobskill_ai_condition( const int type )
{
	return (D_type_mob_skill[type].ai_condition);
}


static inline int	D_typemobskill_casting_millisec( const int type )
{
	return (D_type_mob_skill[type].casting_millisec);
}


static inline int	D_typemobskill_use_ap( const int type )
{
	return (D_type_mob_skill[type].use_ap);
}


static inline int	D_typemobskill_range_type( const int type )
{
	return (D_type_mob_skill[type].range_type);
}


static inline int	D_typemobskill_range_x( const int type )
{
	return (D_type_mob_skill[type].range_x);
}


static inline int	D_typemobskill_range_z( const int type )
{
	return (D_type_mob_skill[type].range_z);
}


static inline int	D_typemobskill_target_object( const int type )
{
	return (D_type_mob_skill[type].target_object);
}


static inline int	D_typemobskill_target_apply( const int type )
{
	return (D_type_mob_skill[type].target_apply);
}


static inline int	D_typemobskill_target_distance( const int type )
{
	return (D_type_mob_skill[type].target_distance);
}


static inline int	D_typemobskill_pattern( const int type )
{
	return (D_type_mob_skill[type].pattern);
}


static inline int	D_typemobskill_action_cnt( const int type )
{
	return (D_type_mob_skill[type].action_cnt);
}


static inline int	D_typemobskill_action_millisec( const int type )
{
	return (D_type_mob_skill[type].action_millisec);
}


static inline int	D_typemobskill_func_summon( const int type )
{
	return (D_type_mob_skill[type].summon);
}


static inline int	D_typemobskill_function( const int type, const int order )
{
	return (D_type_mob_skill[type].order[order].function);
}


static inline int	D_typemobskill_damage( const int type, const int order )
{
	return (D_type_mob_skill[type].order[order].damage);
}





//______________________________________________________________________________________
// typemob
static inline int	D_typemob_attack_type( const int type )
{
	return (D_type_mob[type].attack_type);
}


static inline int	D_typemob_hp_max( const int type )
{
	return (D_type_mob[type].hp_max);
}


static inline int	D_typemob_ap_max( const int type )
{
	return (D_type_mob[type].ap_max);
}


static inline int	D_typemob_hp_recover( const int type )
{
	return (D_type_mob[type].hp_recover);
}


static inline int	D_typemob_ap_recover( const int type )
{
	return (D_type_mob[type].ap_recover);
}


static inline int	D_typemob_area_attack( const int type )
{
	return (D_type_mob[type].area_attack);
}


static inline int	D_typemob_area_vision( const int type )
{
	return (D_type_mob[type].area_vision);
}


static inline int	D_typemob_area_wander_w( const int type )
{
	return (D_type_mob[type].area_wander_w);
}


static inline int	D_typemob_area_wander_h( const int type )
{
	return (D_type_mob[type].area_wander_h);
}


static inline int	D_typemob_area_persuit( const int type )
{
	return (D_type_mob[type].area_persuit);
}


static inline int	D_typemob_delay_regen( const int type )
{
	return (D_type_mob[type].milli_delay_regen);
}


static inline int	D_typemob_delay_attack( const int type )
{
	return (D_type_mob[type].milli_delay_attack);
}


static inline int	D_typemob_delay_death( const int type )
{
	return (D_type_mob[type].milli_delay_death);
}


static inline float	D_typemob_move_speed( const int type, const int status )
{
	return (D_type_mob[type].move_speed[status]);
}


static inline int	D_typemob_escape_hp( const int type )
{
	return (D_type_mob[type].escape_hp);
}


static inline int	D_typemob_skill_type( const int type, const int skill_order )
{
	return (D_type_mob[type].skill[skill_order].type);
}


static inline int	D_typemob_skill_rate( const int type, const int skill_order )
{
	return (D_type_mob[type].skill[skill_order].rate);
}


static inline int	D_typemob_npc_type( const int type )
{
	return (D_type_mob[type].npc_type);
}


static inline int	D_typemob_summon_cnt( const int type )
{
	return (D_type_mob[type].summon_cnt);
}


static inline int	D_typemob_have_skill_ai_condition( const int type, const int ai_condition )
{
	return (D_type_mob[type].have_skill[ai_condition]);
}





//______________________________________________________________________________________
// type_warp 
char *	D_get_gameserv_ip( void ) 
{
	return (D_gameserv.ipaddress);
}


int	D_get_map_cnt( void )
{
	return (D_map_cnt);
}


int	D_get_type_mob_cnt( void )
{
	return (D_type_mob_cnt);
}


int	D_get_mob_cnt( const int channel )
{
	return (D_mob_cnt[G_channel_ins[channel]]);
}


int	D_map_start_mob( const int channel, const int map )
{
	return (D_map[map].start_mob_id[G_channel_ins[channel]]);
}


int	D_map_end_mob( const int channel, const int map )
{
	return (D_map[map].end_mob_id[G_channel_ins[channel]]);
}


int	D_map_summon_start_mob( const int channel, const int map )
{
	return (D_map[map].start_summon_mob_id[G_channel_ins[channel]]);
}


int	D_check_invalid_link( const int channel, const int map )
{
	if( G_channel_ins[channel] && (map < D_ins_start_map) )
		return (1);

	if( !G_channel_ins[channel] && (map >= D_ins_start_map) )
		return (1);

	return (0);
}




//______________________________________________________________________________________
// activity
void	D_set_channel_activity( const int channel )
{
	dprintf(channel, "[ DB ]start D_set_channel_activity(%d) \n", channel );

	D_channel_char_activity( channel );
	D_channel_mob_activity( channel );

	dprintf(channel, "[ DB ]complete D_set_channel_activity(%d) \n", channel );
}


void	D_channel_mob_activity( const int channel )
{
	int	map, x, z, mob_id;


	for( map=1; map<=D_get_map_cnt(); map++ )
	{

		if( D_check_invalid_link(channel, map) )
			continue;

		// set link_delete
		for( z=0; z<D_map[map].height; z++ )
			for( x=0; x<D_map[map].width; x++ )
				while( (mob_id = D_GetMap_mob_link(channel, map, x, z)) )
					D_SetMap_mob_link_delete( channel, mob_id );

		dprintf(channel, "[ DB ]complete free mob_link\n");

		// normal_mob
		for( mob_id=D_map_start_mob(channel, map); mob_id<D_map_summon_start_mob(channel, map); mob_id++ )
		{
			switch( D_GetMob_npc_type(channel, mob_id) )
			{
				case 	NPC_TYPE_MONSTER :
					D_SetMob_status( channel, mob_id, MOB_STATUS_REGEN );
					D_SetMob_action_millisec( channel, mob_id, 0 );
					break;
				case 	NPC_TYPE_NORMAL_NPC :
				case 	NPC_TYPE_STORE :
				case 	NPC_TYPE_WARP :
					D_SetMob_status( channel, mob_id, MOB_STATUS_NPC_REGEN );
					D_SetMob_action_millisec( channel, mob_id, 0 );
					break;
			}
		}

		dprintf(channel, "[ DB ]complete mob activity\n");

		// summon_mob
		while( (mob_id = D_GetMap_summon_link(channel, map)) )
		{
			D_SetMap_summon_link_delete( channel, mob_id );
			D_SetMap_empty_link_insert( channel, map, mob_id );
		}

		dprintf(channel, "[ DB ]complete free summ_link\n");
	}
}


int	D_summon_mob_activity( const int channel, const int map, const int parent_id, const int mob_type, const float x, const float z )
{
	int	mob_id;


	if( D_check_invalid_link(channel, map) )
	{
		dprintf(channel, "[ DB ]Error D_summon_mob_activity() wrong_link channel(%d), map(%d) \n", channel, map );
		return (0);
	}

	if( (mob_id = D_GetMap_empty_link(channel, map)) )
	{
		if( D_set_mob_regen(channel, mob_id, mob_type, map, x, z, ((parent_id < 1) ? 0 : D_mob[channel][parent_id].dir), 0) )
		{
			dprintf(channel, "[ DB ]D_summon_mob_activity() channel(%d), map(%d), parent_id(%d), mob_type(%d) \n"
				, channel, map, parent_id, mob_type );

			D_SetMob_parent_id( channel, mob_id, parent_id );

			D_SetMap_empty_link_delete( channel, mob_id );
			D_SetMap_summon_link_insert( channel, map, mob_id );
	
			switch( D_GetMob_npc_type(channel, mob_id) )
			{
				case 	NPC_TYPE_MONSTER :
					D_SetMob_status( channel, mob_id, MOB_STATUS_REGEN );
					D_SetMob_action_millisec( channel, mob_id, 0 );
					break;
				case 	NPC_TYPE_NORMAL_NPC :
				case 	NPC_TYPE_STORE :
				case 	NPC_TYPE_WARP :
					D_SetMob_status( channel, mob_id, MOB_STATUS_NPC_REGEN );
					D_SetMob_action_millisec( channel, mob_id, 0 );
					break;
			}

			return (mob_id);
		}
	}

	return (0);
}


void	D_channel_mob_recover( const int channel )
{
	int	map, mob_id;

	for( map=1; map<=D_get_map_cnt(); map++ )
	{
		if( D_check_invalid_link(channel, map) )
			continue;

		// normal_mob
		for( mob_id=D_map_start_mob(channel, map); mob_id<D_map_summon_start_mob(channel, map); mob_id++ )
			D_SetMob_regular_recover( channel, mob_id );

		// summon_mob
		mob_id = D_GetMap_summon_link( channel, map );

		while( mob_id )
		{
			D_SetMob_regular_recover( channel, mob_id );

			mob_id = D_GetMap_summon_link_next( channel, mob_id );
		}
	}
}


void	D_channel_char_activity( const int channel )
{
	int	char_id;


	for( char_id=0; char_id<MAX_CHAR_CNT; char_id++ )
	{
		if( D_char[channel][char_id].link_add )
			D_SetMap_char_link_delete( channel, char_id );

		D_SetChar_init( channel, char_id );
	}

	dprintf(channel, "[ DB ]complete free character\n");
}





//______________________________________________________________________________________
// map
static inline int	D_GetMap_collission( const int map, const int x, const int z )
{
	return (D_map[map].collission[x][z]);
}


static inline void	D_SetMap_limit_xz( const int map, int *sx, int *sz, int *ex, int *ez )
{
	if( *sx < 2 )				*sx	= 2;
	if( *sz < 2 )				*sz	= 2;
	if( *ex > (D_map[map].width  - 3) )	*ex	= (D_map[map].width  - 3);
	if( *ez > (D_map[map].height - 3) )	*ez	= (D_map[map].height - 3);
}


static inline int	D_GetMap_char_link( const int channel, const int map, const int x, const int z )
{
	return (D_map[map].map_link[channel].char_link[x][z]);
}


static inline int	D_GetMap_char_link_next( const int channel, const int char_id )
{
	return (D_char[channel][char_id].link_next);
}


static inline void	D_SetMap_char_link_insert( const int channel, const int map, const int char_id )
{
	int	x, z;

	x	= (int)(D_char[channel][char_id].x);
	z	= (int)(D_char[channel][char_id].z);

	D_char[channel][char_id].link_next	= D_map[map].map_link[channel].char_link[x][z];
	D_char[channel][char_id].link_prev	= &(D_map[map].map_link[channel].char_link[x][z]);

	if( D_char[channel][char_id].link_next )
		D_char[channel][(D_char[channel][char_id].link_next)].link_prev	= &D_char[channel][char_id].link_next;

	D_map[map].map_link[channel].char_link[x][z]	= char_id;

	D_char[channel][char_id].link_add	= 1;
}


static inline void	D_SetMap_char_link_delete( const int channel, const int char_id )
{
	*(D_char[channel][char_id].link_prev)	= D_char[channel][char_id].link_next;

	if( D_char[channel][char_id].link_next )
		D_char[channel][(D_char[channel][char_id].link_next)].link_prev	= D_char[channel][char_id].link_prev;

	D_char[channel][char_id].link_add	= 0;
}


static inline int	D_GetMap_mob_link( const int channel, const int map, const int x, const int z )
{
	return (D_map[map].map_link[channel].mob_link[x][z]);
}


static inline int	D_GetMap_mob_link_next( const int channel, const int mob_id )
{
	return (D_mob[channel][mob_id].link_next);
}


static inline void	D_SetMap_mob_link_insert( const int channel, const int map, const int mob_id )
{
	int	x, z;

	x	= (int)D_mob[channel][mob_id].x;
	z	= (int)D_mob[channel][mob_id].z;

	D_mob[channel][mob_id].link_next	= D_map[map].map_link[channel].mob_link[x][z];
	D_mob[channel][mob_id].link_prev	= &(D_map[map].map_link[channel].mob_link[x][z]);

	if( D_mob[channel][mob_id].link_next )
		D_mob[channel][(D_mob[channel][mob_id].link_next)].link_prev	= &D_mob[channel][mob_id].link_next;

	D_map[map].map_link[channel].mob_link[x][z]	= mob_id;
}


static inline void	D_SetMap_mob_link_delete( const int channel, const int mob_id )
{
	*(D_mob[channel][mob_id].link_prev)	= D_mob[channel][mob_id].link_next;

	if( D_mob[channel][mob_id].link_next )
		D_mob[channel][(D_mob[channel][mob_id].link_next)].link_prev	= D_mob[channel][mob_id].link_prev;
}


int	D_GetMap_summon_link( const int channel, const int map )
{
	return (D_map[map].map_link[channel].summon_link);
}


int	D_GetMap_summon_link_next( const int channel, const int mob_id )
{
	return (D_mob[channel][mob_id].summon_link_next);
}


static inline void	D_SetMap_summon_link_insert( const int channel, const int map, const int mob_id )
{
	D_mob[channel][mob_id].summon_link_next		= D_map[map].map_link[channel].summon_link;
	D_mob[channel][mob_id].summon_link_prev		= &(D_map[map].map_link[channel].summon_link);

	if( D_mob[channel][mob_id].summon_link_next )
		D_mob[channel][(D_mob[channel][mob_id].summon_link_next)].summon_link_prev	= &D_mob[channel][mob_id].summon_link_next;

	D_map[map].map_link[channel].summon_link	= mob_id;
}


static inline void	D_SetMap_summon_link_delete( const int channel, const int mob_id )
{
	*(D_mob[channel][mob_id].summon_link_prev)	= D_mob[channel][mob_id].summon_link_next;

	if( D_mob[channel][mob_id].summon_link_next )
		D_mob[channel][(D_mob[channel][mob_id].summon_link_next)].summon_link_prev	= D_mob[channel][mob_id].summon_link_prev;
}


static inline int	D_GetMap_empty_link( const int channel, const int map )
{
	return (D_map[map].map_link[channel].empty_link);
}


static inline void	D_SetMap_empty_link_insert( const int channel, const int map, const int mob_id )
{
	D_mob[channel][mob_id].empty_link_next		= D_map[map].map_link[channel].empty_link;
	D_mob[channel][mob_id].empty_link_prev		= &(D_map[map].map_link[channel].empty_link);

	if( D_mob[channel][mob_id].empty_link_next )
		D_mob[channel][(D_mob[channel][mob_id].empty_link_next)].empty_link_prev	= &D_mob[channel][mob_id].empty_link_next;

	D_map[map].map_link[channel].empty_link		= mob_id;
}


static inline void	D_SetMap_empty_link_delete( const int channel, const int mob_id )
{
	*(D_mob[channel][mob_id].empty_link_prev)	= D_mob[channel][mob_id].empty_link_next;

	if( D_mob[channel][mob_id].empty_link_next )
		D_mob[channel][(D_mob[channel][mob_id].empty_link_next)].empty_link_prev	= D_mob[channel][mob_id].empty_link_prev;
}





//______________________________________________________________________________________
// mob
void	D_SetMob_regen( const int const channel, int mob_id, struct body_MG_MOB_REGEN *send_body )
{

	dprintf(channel, "[ DB ]D_SetMob_regen() channel(%d), mob_id(%d), type(%d), parent_id(%d), npc_type(%d), table_idx(%d) \n"
		, channel, mob_id, D_mob[channel][mob_id].type, D_GetMob_parent_id(channel, mob_id)
		, D_GetMob_npc_type(channel, mob_id), D_mob[channel][mob_id].table_idx );

	// set attack_type
	D_SetMob_attack_type( channel, mob_id );

	// init buff
	D_SetMob_init_buff( channel, mob_id );

	// init summon_cnt
	D_SetMob_summon_cnt( channel, mob_id, D_typemob_summon_cnt(D_mob[channel][mob_id].type) );

	// init escape_cnt
	D_SetMob_escape_cnt( channel, mob_id, 1 );

	// init regen_xz
	D_SetMob_regen_xz( channel, mob_id );

	// link_insert
	D_SetMap_mob_link_insert( channel, D_mob[channel][mob_id].map, mob_id );

	send_body->mob_id		= mob_id;
	send_body->map			= D_mob[channel][mob_id].map;
	send_body->mob_type		= D_mob[channel][mob_id].type;
	send_body->table_idx		= D_mob[channel][mob_id].table_idx;
	send_body->dir			= D_mob[channel][mob_id].dir;
	send_body->regen_x		= D_mob[channel][mob_id].x;
	send_body->regen_z		= D_mob[channel][mob_id].z;
	send_body->regen_y		= 0;
}


void	D_SetMob_awake( const int channel, const int mob_id, struct body_MG_MOB_MOVE *send_body )
{

	dprintf(channel, "[ DB ]D_SetMob_awake() channel(%d), mob_id(%d), map(%d), xz(%.01f, %.01f), collission(%d) \n"
		, channel, mob_id, D_mob[channel][mob_id].map, D_mob[channel][mob_id].x
		, D_mob[channel][mob_id].z, D_mob[channel][mob_id].collission );

	// init combat
	D_SetMob_init_combat( channel, mob_id );

	send_body->mob_id	= mob_id;
	send_body->status	= _MOB_WAIT1;
	send_body->dir		= D_mob[channel][mob_id].dir;
	send_body->speed	= D_GetMob_move_speed(channel, mob_id, 0);
	send_body->x 		= D_mob[channel][mob_id].x;
	send_body->z	 	= D_mob[channel][mob_id].z;
	send_body->y 		= 0;
}


int	D_SetMob_death( const int channel, const int mob_id )
{

	if( D_mob[channel][mob_id].status >= MOB_STATUS_DEATH )
	{
		dprintf(channel, "[ DB ]Error D_SetMob_death() status(%d) \n", mob_id, D_mob[channel][mob_id].status);
		return (1);
	}

	if( D_mob[channel][mob_id].hp_cur < 1 )
	{
		dprintf(channel, "[ DB ]D_SetMob_death() mob_id(%d), map(%d), xz(%.01f, %.01f) \n"
			, mob_id, D_mob[channel][mob_id].map, D_mob[channel][mob_id].x, D_mob[channel][mob_id].z );
		return (1);
	}

	return (0);
}


void	D_SetMob_extinct( const int channel, const int mob_id, struct body_MG_MOB_EXTINCT *send_body )
{
	// link_delete
	D_SetMap_mob_link_delete( channel, mob_id );

	send_body->mob_id	= mob_id;
}


int	D_SetMob_warp( const int channel, const int mob_id, int *p_add_warp_millisec )
{
	int		start_sec, end_sec;
	int		warp_open_sec, warp_close_sec;

	start_sec	= D_typewarp_start_sec(D_mob[channel][mob_id].table_idx);
	end_sec		= D_typewarp_end_sec(D_mob[channel][mob_id].table_idx);


	dprintf(channel, "[ DB ]D_SetMob_warp() mob_id(%d), map(%d), npc_type(%d), table_idx(%d), start_sec(%d), end_sec(%d) \n"
		, mob_id, D_mob[channel][mob_id].map, D_GetMob_npc_type(channel, mob_id)
		, D_mob[channel][mob_id].table_idx, start_sec, end_sec );

	if( (start_sec < 0) || (start_sec == end_sec) )
		return (RET_WARP_CONTINUE);

	warp_open_sec	= T_dayoftime( start_sec );
	warp_close_sec	= T_dayoftime( end_sec );

	if( start_sec < end_sec )
	{
		if( warp_open_sec > 0 )
		{
			*p_add_warp_millisec	= (warp_open_sec * 1000);
			return (RET_WARP_WAIT_OPEN);
		}
		else
		{
			if( warp_close_sec > 0 )
			{
				*p_add_warp_millisec	= (warp_close_sec * 1000);
				return (RET_WARP_NOW_OPEN_WAIT_CLOSE); 
			}
			else
			{
				warp_open_sec	= T_dayoftime(DAY) + start_sec;

				*p_add_warp_millisec	= (warp_open_sec * 1000);
				return (RET_WARP_WAIT_OPEN);
			}
		}
	}
	else
	{
		if( warp_close_sec > 0 )
		{
			*p_add_warp_millisec	= (warp_close_sec * 1000);
			return (RET_WARP_NOW_OPEN_WAIT_CLOSE); 
		}
		else
		{
			if( warp_open_sec > 0 )
			{
				*p_add_warp_millisec	= (warp_open_sec * 1000);
				return (RET_WARP_WAIT_OPEN);
			}
			else
			{
				warp_close_sec	= T_dayoftime(DAY) + end_sec;

				*p_add_warp_millisec	= (warp_close_sec * 1000);
				return (RET_WARP_NOW_OPEN_WAIT_CLOSE); 
			}
		}
	}


	return (RET_WARP_CONTINUE);
}


void	D_SetMob_perfect_recover( const int channel, const int mob_id, struct body_MG_MOB_PERFECT_RECOVER *send_body )
{

	dprintf(channel, "[ DB ]D_SetMob_perfect_recover() mob_id(%d) \n", mob_id );

	// init combat
	D_SetMob_init_combat( channel, mob_id );


	send_body->mob_id	= mob_id;
}


void	D_SetMob_regular_recover( const int channel, const int mob_id )
{

	if( D_mob[channel][mob_id].status < MOB_STATUS_DEATH )
	{
		D_SetMob_hp_cur( channel, mob_id, D_GetMob_hp_cur(channel, mob_id) 
				+ D_typemob_hp_recover(D_mob[channel][mob_id].type)
				+ D_GetMob_buff_value(channel, mob_id, _BUFF_POTION_RECOVER_HP)
				- D_GetMob_buff_value(channel, mob_id, _BUFF_DE_ACTIVE_POISONING)
				- D_GetMob_buff_value(channel, mob_id, _BUFF_DE_ACTIVE_FIREPOISONING) );

		if( D_GetMob_hp_cur(channel, mob_id) < 1 )
			D_SetMob_hp_cur( channel, mob_id, 1 );

		D_SetMob_ap_cur( channel, mob_id, D_GetMob_ap_cur(channel, mob_id) 
				+ D_typemob_ap_recover(D_mob[channel][mob_id].type)
				+ D_GetMob_buff_value(channel, mob_id, _BUFF_POTION_RECOVER_AP) );

		if( D_GetMob_ap_cur(channel, mob_id) < 1 )
			D_SetMob_ap_cur( channel, mob_id, 1 );
	}
}


int	D_SetMob_escape( const int channel, const int mob_id )
{

	if( (D_mob[channel][mob_id].hp_cur < D_GetMob_escape_hp(channel, mob_id)) && D_GetMob_escape_cnt(channel, mob_id) ) 
	{
		dprintf(channel, "[ DB ]D_SetMob_escape() mob_id(%d), max_hp(%d), cur_hp(%d), escape_hp(%d) \n"
			, mob_id, D_GetMob_hp_max(channel, mob_id), D_GetMob_hp_cur(channel, mob_id)
			, D_GetMob_escape_hp(channel, mob_id) );

		D_SetMob_escape_cnt( channel, mob_id, D_GetMob_escape_cnt(channel, mob_id) - 1 );

		if( D_ChkMob_have_skill_ai_condition(channel, mob_id, SKILL_AI_COND_ESCAPE_HP) )
			return (1);

		if( D_SetMob_random_xz(channel, mob_id, D_mob[channel][mob_id].map, 0.0f, 0.0f) )
			D_SetMob_status( channel, mob_id, MOB_STATUS_ESCAPE );
	}

	return (0);
}


int	D_GetMob_npc_type( const int channel, const int mob_id )
{
	return (D_typemob_npc_type(D_mob[channel][mob_id].type));
}


int	D_ChkMob_map_collission( const int channel, const int mob_id, const int x, const int z )
{
	if( D_mob[channel][mob_id].collission != D_GetMap_collission(D_mob[channel][mob_id].map, x, z) )
		return (1);

	return (0);
}


void	D_SetMob_status( const int channel, const int mob_id, const int value )
{
	D_mob[channel][mob_id].status = value;
}


int	D_GetMob_status( const int channel, const int mob_id )
{
	return (D_mob[channel][mob_id].status);
}


void	D_SetMob_action_millisec( const int channel, const int mob_id, const int add_millisec )
{
	D_mob[channel][mob_id].action_millisec	= (G_MilliSec + add_millisec);
}


double	D_GetMob_action_millisec( const int channel, const int mob_id )
{
	return (D_mob[channel][mob_id].action_millisec);
}


void	D_SetMob_action_sec( const int channel, const int mob_id, const int arr )
{
	D_mob[channel][mob_id].action_sec[arr] = G_Sec;
}


time_t	D_GetMob_action_sec( const int channel, const int mob_id, const int arr )
{
	return (D_mob[channel][mob_id].action_sec[arr]);
}


void	D_SetMob_skill_sec( const int channel, const int mob_id, const int arr )
{
	D_mob[channel][mob_id].skill_sec[arr] = G_Sec;
}


time_t	D_GetMob_skill_sec( const int channel, const int mob_id, const int arr )
{
	return (D_mob[channel][mob_id].skill_sec[arr]);
}


void	D_SetMob_skill_num( const int channel, const int mob_id, const int arr, const int value )
{
	D_mob[channel][mob_id].skill_num[arr] = value;
}


int	D_GetMob_skill_num( const int channel, const int mob_id, const int arr )
{
	return (D_mob[channel][mob_id].skill_num[arr]);
}


int	D_GetMob_delay_regen( const int channel, const int mob_id )
{
	return (D_typemob_delay_regen(D_mob[channel][mob_id].type));
}


int	D_GetMob_delay_death( const int channel, const int mob_id )
{
	return (D_typemob_delay_death(D_mob[channel][mob_id].type));
}


int	D_GetMob_delay_attack( const int channel, const int mob_id )
{
	return (  (float)D_typemob_delay_attack(D_mob[channel][mob_id].type)
		/ ((100 + 0) / 100)
		* ((100 + D_GetMob_buff_value(channel, mob_id, _BUFF_DE_ACTIVE_ATT_SPEED)) / 100)  );
}


int	D_GetMob_skill_casting_millisec( const int channel, const int mob_id )
{
	return (D_mob[channel][mob_id].skill_casting_millisec);
}


int	D_GetMob_skill_action_millisec( const int channel, const int mob_id )
{
	return (D_mob[channel][mob_id].skill_action_millisec);
}


static inline int	D_ChkMob_possible_move( const int channel,  const int mob_id )
{
	if( D_GetMob_move_speed(channel, mob_id, 0) > 0.0f )
		return (1);

	return (0);
}


static inline float	D_GetMob_move_speed( const int channel, const int mob_id, const int grade )
{
	return (  (float)D_typemob_move_speed(D_mob[channel][mob_id].type, grade) 
		/ ((100 + D_GetMob_buff_value(channel, mob_id, _BUFF_DE_ACTIVE_MOVE_SPEED)) / 100) 
		* ((100 + D_GetMob_buff_value(channel, mob_id, _BUFF_ACTIVE_MOVE_SPEED)) / 100) );
}


static inline int	D_GetMob_hp_max( const int channel, const int mob_id )
{
	return (D_typemob_hp_max(D_mob[channel][mob_id].type)
		+ D_GetMob_buff_value(channel, mob_id, _BUFF_PASSIVE_HP_MAX) );
}


static inline int	D_GetMob_ap_max( const int channel, const int mob_id )
{
	return (D_typemob_ap_max(D_mob[channel][mob_id].type)
		+ D_GetMob_buff_value(channel, mob_id, _BUFF_PASSIVE_AP_MAX) );
}


static inline int	D_GetMob_hp_cur( const int channel, const int mob_id )
{
	return (D_mob[channel][mob_id].hp_cur);
}


static inline void	D_SetMob_hp_cur( const int channel, const int mob_id, const int value )
{
	if( value < 0 )
		D_mob[channel][mob_id].hp_cur	= 0;

	else if( value > D_GetMob_hp_max(channel, mob_id) )
		D_mob[channel][mob_id].hp_cur	= D_GetMob_hp_max(channel, mob_id); 

	else
		D_mob[channel][mob_id].hp_cur	= value;
}


static inline int	D_GetMob_ap_cur( const int channel, const int mob_id )
{
	return (D_mob[channel][mob_id].ap_cur);
}


static inline void	D_SetMob_ap_cur( const int channel, const int mob_id, const int value )
{
	if( value < 0 )
		D_mob[channel][mob_id].ap_cur	= 0;

	else if( value > D_GetMob_ap_max(channel, mob_id) )
		D_mob[channel][mob_id].ap_cur	= D_GetMob_ap_max(channel, mob_id); 

	else
		D_mob[channel][mob_id].ap_cur	= value;
}


static inline int	D_GetMob_hp_cur_percent( const int channel, const int mob_id )
{
	return (int)((float)D_mob[channel][mob_id].hp_cur / D_GetMob_hp_max(channel, mob_id) * 100);
}


static inline int	D_GetMob_escape_hp( const int channel, const int mob_id )
{
	return (D_typemob_escape_hp(D_mob[channel][mob_id].type));
}


static inline void	D_SetMob_escape_cnt( const int channel, const int mob_id, const int value )
{
	if( value < 0 )
		D_mob[channel][mob_id].escape_cnt = 0;
	else
		D_mob[channel][mob_id].escape_cnt = value;
}


static inline int	D_GetMob_escape_cnt( const int channel, const int mob_id )
{
	return (D_mob[channel][mob_id].escape_cnt);
}


static inline int	D_GetMob_summon_cnt( const int channel, const int mob_id )
{
	return (D_mob[channel][mob_id].summon_cnt);
}


static inline void	D_SetMob_summon_cnt( const int channel, const int mob_id, const int value )
{
	if( value < 0 )
		D_mob[channel][mob_id].summon_cnt = 0;
	else
		D_mob[channel][mob_id].summon_cnt = value;
}


static inline int	D_GetMob_parent_id( const int channel, const int mob_id )
{
	return (D_mob[channel][mob_id].parent_id);
}


static inline void	D_SetMob_parent_id( const int channel, const int mob_id, const int value )
{
	D_mob[channel][mob_id].parent_id	= value;
}


static inline void	D_SetMob_attack_type( const int channel, const int mob_id )
{
	if( (U_get_random(100) + 1) <= D_typemob_attack_type(D_mob[channel][mob_id].type) )
		D_mob[channel][mob_id].attack_type	= 1;
	else
		D_mob[channel][mob_id].attack_type	= 0;

}


static inline int	D_GetMob_attack_type( const int channel, const int mob_id )
{
	return (D_mob[channel][mob_id].attack_type);
}


static inline int	D_GetMob_combat_start( const int channel, const int mob_id )
{
	return (D_mob[channel][mob_id].combat_start);
}


static inline int	D_GetMob_attack_start( const int channel, const int mob_id )
{
	return (D_mob[channel][mob_id].attack_start);
}


static inline void	D_SetMob_attack_start( const int channel, const int mob_id, const int value )
{
	D_mob[channel][mob_id].attack_start	= value;
}


static inline int	D_ChkMob_have_skill_ai_condition( const int channel, const int mob_id, const int skill_ai_condition )
{
	return (D_typemob_have_skill_ai_condition(D_mob[channel][mob_id].type, skill_ai_condition));
}


static inline int	D_GetMob_current_skill_type( const int channel, const int mob_id )
{
	return (D_mob[channel][mob_id].current_skill_type);
}


static inline int	D_GetMob_skill_target_object_id( const int channel, const int mob_id )
{
	return (D_mob[channel][mob_id].skill_target_object_id);
}


int	D_GetMob_skill_action_cnt( const int channel, const int mob_id )
{
	return (D_mob[channel][mob_id].skill_action_cnt);
}


static inline void	D_SetMob_skill_action_cnt( const int channel, const int mob_id, const int value )
{
	D_mob[channel][mob_id].skill_action_cnt	= value;
}


static inline void	D_GetMob_area_vision( const int channel, const int mob_id, int *p_x1, int *p_z1, int *p_x2, int *p_z2 )
{
	int		area_vision;

	area_vision	= D_typemob_area_vision(D_mob[channel][mob_id].type);
	*p_x1		= D_mob[channel][mob_id].x - area_vision; 
	*p_z1		= D_mob[channel][mob_id].z - area_vision;
	*p_x2		= D_mob[channel][mob_id].x + area_vision;
	*p_z2		= D_mob[channel][mob_id].z + area_vision;

	D_SetMap_limit_xz( D_mob[channel][mob_id].map, p_x1, p_z1, p_x2, p_z2 );
}


static inline int	D_ChkMob_over_area_vision( const int channel, const int mob_id, const float x, const float z )
{
	int	chk_sx, chk_sz, chk_ex, chk_ez;

	D_GetMob_area_vision( channel, mob_id, &chk_sx, &chk_sz, &chk_ex, &chk_ez );

	if( ((x < chk_sx) || (z < chk_sz)) || ((x > chk_ex) || (z > chk_ez)) )
		return (1);

	return (0);
}


static inline int	D_ChkMob_over_area_8_vision( const int channel, const int mob_id, const float x, const float z )
{
	int		x1, z1, x2, z2;

	x1		= D_mob[channel][mob_id].x - 8; 
	z1		= D_mob[channel][mob_id].z - 8;
	x2		= D_mob[channel][mob_id].x + 8;
	z2		= D_mob[channel][mob_id].z + 8;

	D_SetMap_limit_xz( D_mob[channel][mob_id].map, &x1, &z1, &x2, &z2 );

	if( (x < x1) || (z < z1) || (x > x2) || (z > x2) )
		return (1);
	
	return (0);
}


static inline void	D_GetMob_area_wander( const int channel, const int mob_id, int *p_x1, int *p_z1, int *p_x2, int *p_z2 )
{
	(*p_x1) 	= D_mob[channel][mob_id].area_wander_sx;
	(*p_z1) 	= D_mob[channel][mob_id].area_wander_sz;
	(*p_x2) 	= D_mob[channel][mob_id].area_wander_ex;
	(*p_z2) 	= D_mob[channel][mob_id].area_wander_ez;
}


static inline int	D_ChkMob_over_area_wander( const int channel, const int mob_id, const float x, const float z )
{
	int		chk_sx, chk_sz, chk_ex, chk_ez;

	D_GetMob_area_wander( channel, mob_id, &chk_sx, &chk_sz, &chk_ex, &chk_ez );

	if( ((x < chk_sx) || (z < chk_sz)) || ((x > chk_ex) || (z > chk_ez)) )
		return (1);

	return (0);
}


static inline void	D_GetMob_area_persuit( const int channel, const int mob_id, int *p_x1, int *p_z1, int *p_x2, int *p_z2 )
{
	(*p_x1) 	= D_mob[channel][mob_id].area_persuit_sx;
	(*p_z1) 	= D_mob[channel][mob_id].area_persuit_sz;
	(*p_x2) 	= D_mob[channel][mob_id].area_persuit_ex;
	(*p_z2) 	= D_mob[channel][mob_id].area_persuit_ez;
}


int	D_ChkMob_over_area_persuit( const int channel, const int mob_id, const float x, const float z )
{
	int		chk_sx, chk_sz, chk_ex, chk_ez;

	D_GetMob_area_persuit( channel, mob_id, &chk_sx, &chk_sz, &chk_ex, &chk_ez );

	if( ((x < chk_sx) || (z < chk_sz)) || ((x > chk_ex) || (z > chk_ez)) )
		return (1);

	return (0);
}


static inline void	D_GetMob_area_attack( const int channel, const int mob_id, int *p_x1, int *p_z1, int *p_x2, int *p_z2 )
{
	int		area_attack;

	area_attack	=  D_typemob_area_attack(D_mob[channel][mob_id].type);

	(*p_x1) 	= D_mob[channel][mob_id].x - area_attack;
	(*p_z1) 	= D_mob[channel][mob_id].z - area_attack;
	(*p_x2) 	= D_mob[channel][mob_id].x + area_attack;
	(*p_z2) 	= D_mob[channel][mob_id].z + area_attack;

	D_SetMap_limit_xz( D_mob[channel][mob_id].map, p_x1, p_z1, p_x2, p_z2 );
}


int	D_ChkMob_over_area_attack( const int channel, const int mob_id, const int char_id )
{
	int		chk_sx, chk_sz, chk_ex, chk_ez;

	D_GetMob_area_attack( channel, mob_id, &chk_sx, &chk_sz, &chk_ex, &chk_ez );
	
	if( ((D_char[channel][char_id].x < chk_sx) || (D_char[channel][char_id].z < chk_sz)) 
		|| (((int)D_char[channel][char_id].x > chk_ex) || ((int)D_char[channel][char_id].z > chk_ez)) )
		return (1);

	return (0);
}


static inline void	D_GetMob_area_skill_distance( const int channel, const int mob_id, const int skill_type
			, int *p_x1, int *p_z1, int *p_x2, int *p_z2 )
{
	int		area_skill_distance;

	area_skill_distance	=  D_typemobskill_target_distance(skill_type);

	(*p_x1) 	= D_mob[channel][mob_id].x - area_skill_distance;
	(*p_z1) 	= D_mob[channel][mob_id].z - area_skill_distance;
	(*p_x2) 	= D_mob[channel][mob_id].x + area_skill_distance;
	(*p_z2) 	= D_mob[channel][mob_id].z + area_skill_distance;

	D_SetMap_limit_xz( D_mob[channel][mob_id].map, p_x1, p_z1, p_x2, p_z2 );
}


static inline int	D_ChkMob_over_area_skill_distance( const int channel, const int mob_id, const int skill_type
			, const int x, const int z )
{
	int		target_distance, gap;

	target_distance	= D_typemobskill_target_distance(skill_type);
	gap		= GetLength(D_mob[channel][mob_id].x, D_mob[channel][mob_id].z, x, z);

	if( gap > target_distance )
		return (1); 

	return (0);
}


static inline void	D_GetMob_area_skill( const int channel, const int mob_id, const int skill_type, const int middle_x, const int middle_z
			, int *x1, int *z1, int *x2, int *z2 )
{
	int		range_max;

	if( D_typemobskill_range_x(skill_type) > D_typemobskill_range_z(skill_type) )
		range_max	= D_typemobskill_range_x(skill_type);
	else
		range_max	= D_typemobskill_range_z(skill_type);

	*x1	= middle_x - range_max;
	*z1	= middle_z - range_max;
	*x2	= middle_x + range_max;
	*z2	= middle_z + range_max;

	D_SetMap_limit_xz( D_mob[channel][mob_id].map, x1, z1, x2, z2 );
}


static inline void	D_SetMob_regen_xz( const int channel, const int mob_id )
{
	int		tmp_regen_x, tmp_regen_z;


	tmp_regen_x	= D_mob[channel][mob_id].regen_x;
	tmp_regen_z	= D_mob[channel][mob_id].regen_z;

	switch( U_get_random(4) )
	{
		case	0 :
			tmp_regen_x += U_get_random(D_typemob_area_wander_w(D_mob[channel][mob_id].type));
			tmp_regen_z += U_get_random(D_typemob_area_wander_h(D_mob[channel][mob_id].type));
			break;
		case	1 :
			tmp_regen_x += U_get_random(D_typemob_area_wander_w(D_mob[channel][mob_id].type));
			tmp_regen_z -= U_get_random(D_typemob_area_wander_h(D_mob[channel][mob_id].type));
			break;
		case	2 :
			tmp_regen_x -= U_get_random(D_typemob_area_wander_w(D_mob[channel][mob_id].type));
			tmp_regen_z += U_get_random(D_typemob_area_wander_h(D_mob[channel][mob_id].type));
			break;
		case	3 :
			tmp_regen_x -= U_get_random(D_typemob_area_wander_w(D_mob[channel][mob_id].type));
			tmp_regen_z -= U_get_random(D_typemob_area_wander_h(D_mob[channel][mob_id].type));
			break;
	}

	if( D_GetMob_npc_type(channel, mob_id)
		|| D_GetMob_parent_id(channel, mob_id) 
		|| D_ChkMob_map_collission(channel, mob_id, tmp_regen_x, tmp_regen_z) 
		|| D_ChkMob_over_area_wander(channel, mob_id, tmp_regen_x, tmp_regen_z) )
	{
		D_mob[channel][mob_id].x	= D_mob[channel][mob_id].regen_x;
		D_mob[channel][mob_id].z	= D_mob[channel][mob_id].regen_z;
	}
	else
	{
		D_mob[channel][mob_id].x	= tmp_regen_x;
		D_mob[channel][mob_id].z	= tmp_regen_z;
	}

	D_mob[channel][mob_id].map		= D_mob[channel][mob_id].regen_map;
	D_mob[channel][mob_id].dir		= D_mob[channel][mob_id].regen_dir;

	D_mob[channel][mob_id].wander		= 0;
	D_mob[channel][mob_id].wander_dir	= D_mob[channel][mob_id].dir;
}



static inline int	D_SetMob_move_xz( const int channel, const int mob_id, const float sx, const float sz
			, const int dir, const float to_move_xz )
{
	float		to_move_x, to_move_z, to_x, to_z;


	if( (dir < 0) && (dir >= 360) )
	{
		dprintf(channel, "[ DB ]Error D_SetMob_move_xz() dir(%d) \n", dir );
		return (0);
	}
	
	GetCirclePos( to_move_xz, ((270 + dir) % 360), &to_move_x, &to_move_z );
	to_x 	= (sx + to_move_x);
	to_z 	= (sz + to_move_z);

	if( D_ChkMob_over_area_persuit(channel, mob_id, to_x, to_z) ) 
	{
		//dprintf(channel, "[ DB ]Error D_SetMob_move_xz() over_persuit_area mob_id(%d) to(%f, %f) \n"
		//	, mob_id, to_x, to_z );
		return (0);
	}

	if( D_mob[channel][mob_id].status == MOB_STATUS_WANDER ) 
	{
		if( D_ChkMob_over_area_wander(channel, mob_id, to_x, to_z) ) 
		{
			//dprintf(channel, "[ DB ]Error D_SetMob_move_xz() over_wander_area mob_id(%d) to(%f, %f) \n"
			//	, mob_id, to_x, to_z );
			return (0);
		}

		if( D_ChkMob_map_collission(channel, mob_id, (int)to_x, (int)to_z) ) 
			return (0);
	}

	// link_delete
	D_SetMap_mob_link_delete( channel, mob_id );

	D_mob[channel][mob_id].x	= to_x;
	D_mob[channel][mob_id].z	= to_z;
	D_mob[channel][mob_id].dir	= dir;

	// link_insert
	D_SetMap_mob_link_insert( channel, D_mob[channel][mob_id].map, mob_id );


	return (1);
}


static inline int	D_SetMob_impact_xz( const int channel, const int mob_id, const int char_id, const int attack_dir )
{
	float		to_move_x, to_move_z, to_x, to_z;


	GetCirclePos( 1.0f, ((270 + attack_dir) % 360), &to_move_x, &to_move_z );

	to_x 	= (D_mob[channel][mob_id].x + to_move_x);
	to_z 	= (D_mob[channel][mob_id].z + to_move_z);

	if( D_ChkMob_map_collission(channel, mob_id, (int)to_x, (int)to_z) ) 
		return (0);

	if( D_ChkMob_over_area_persuit(channel, mob_id, to_x, to_z) ) 
		return (0);

	// link_delete
	D_SetMap_mob_link_delete( channel, mob_id );

	D_mob[channel][mob_id].x	= to_x;
	D_mob[channel][mob_id].z	= to_z;
	D_mob[channel][mob_id].dir	= (180 + attack_dir) % 360;

	// link_insert
	D_SetMap_mob_link_insert( channel, D_mob[channel][mob_id].map, mob_id );


	return (1);
}


static inline int	D_GetMob_wander_move_xz( const int channel, const int mob_id, const int map, const float to_move_xz
			, float *to_x, float *to_z )
{
	int		i;


	*to_x		= (D_mob[channel][mob_id].x);
	*to_z		= (D_mob[channel][mob_id].z);

	if( !D_mob[channel][mob_id].wander )
	{
		D_mob[channel][mob_id].wander_dir	= U_get_random(32) * 45;

		if( D_mob[channel][mob_id].wander_dir < 360 )
			D_mob[channel][mob_id].wander		= 1;
	}
	else
	{
		if( U_get_random(100) < 50 )
		{
			D_mob[channel][mob_id].wander		= 0;
			return (0);
		}
	}

	switch( D_mob[channel][mob_id].wander_dir )
	{
		case	DIR_N  :	for( i=0; i<to_move_xz; i ++ ){
						if( D_ChkMob_map_collission(channel, mob_id, (int)(*to_x), (int)((*to_z)+1)) ) 
							break;
						(*to_z) ++;
					}
					break;
		case	DIR_NW :	for( i=0; i<to_move_xz; i ++ ){
						if( D_ChkMob_map_collission(channel, mob_id, (int)((*to_x)-1), (int)((*to_z)+1)) ) 
							break;
						(*to_x) --, (*to_z) ++;
					}
					break;
		case	DIR_W  :	for( i=0; i<to_move_xz; i ++ ){
						if( D_ChkMob_map_collission(channel, mob_id, (int)((*to_x)-1), (int)((*to_z))) ) 
							break;
						(*to_x) --;
					}
					break;
		case	DIR_SW :	for( i=0; i<to_move_xz; i ++ ){
						if( D_ChkMob_map_collission(channel, mob_id, (int)((*to_x)-1), (int)((*to_z)-1)) ) 
							break;
						(*to_x) --, (*to_z) --;
					}
					break;
		case	DIR_S  :	for( i=0; i<to_move_xz; i ++ ){
						if( D_ChkMob_map_collission(channel, mob_id, (int)((*to_x)), (int)((*to_z))-1) ) 
							break;
						(*to_z) --;
					}
					break;
		case	DIR_SE :	for( i=0; i<to_move_xz; i ++ ){
						if( D_ChkMob_map_collission(channel, mob_id, (int)((*to_x)+1), (int)((*to_z)-1)) ) 
							break;
						(*to_x) ++, (*to_z) --;
					}
					break;
		case	DIR_E  :	for( i=0; i<to_move_xz; i ++ ){
						if( D_ChkMob_map_collission(channel, mob_id, (int)((*to_x)+1), (int)((*to_z))) ) 
							break;
						(*to_x) ++;
					}
					break;
		case	DIR_NE :	for( i=0; i<to_move_xz; i ++ ){
						if( D_ChkMob_map_collission(channel, mob_id, (int)((*to_x)+1), (int)((*to_z)+1)) ) 
							break;
						(*to_x) ++, (*to_z) ++;
					}
					break;
	}

	if( (D_mob[channel][mob_id].x == *to_x) && (D_mob[channel][mob_id].z == *to_z) )
	{
		D_mob[channel][mob_id].wander		= 0;
		return (0);
	}

	if( D_ChkMob_over_area_wander(channel, mob_id, *to_x, *to_z) )
	{
		D_mob[channel][mob_id].wander		= 0;
		return (0);
	}

	return (1);
}


static inline int	D_SetMob_random_xz( const int channel, const int mob_id, const int map, const float x, const float z )
{
	float		to_x, to_z;
	char		chk_cnt = 10;


	while( chk_cnt-- )
	{
		if( (x == 0.0f) || (z == 0.0f) )
		{
			switch( U_get_random(8) )
			{
				case	0 :
						to_x	= D_mob[channel][mob_id].x + (5 + U_get_random(5));
						to_z	= D_mob[channel][mob_id].z + (5 + U_get_random(5));
						break;
				case	1 :
						to_x	= D_mob[channel][mob_id].x - (5 + U_get_random(5));
						to_z	= D_mob[channel][mob_id].z - (5 + U_get_random(5));
						break;
				case	2 :
						to_x	= D_mob[channel][mob_id].x + (5 + U_get_random(5));
						to_z	= D_mob[channel][mob_id].z - (5 + U_get_random(5));
						break;
				case	3 :
						to_x	= D_mob[channel][mob_id].x - (5 + U_get_random(5));
						to_z	= D_mob[channel][mob_id].z + (5 + U_get_random(5));
						break;
				case	4 :
						to_x	= D_mob[channel][mob_id].x - (5 + U_get_random(5));
						to_z	= D_mob[channel][mob_id].z;
						break;
				case	5 :
						to_x	= D_mob[channel][mob_id].x;
						to_z	= D_mob[channel][mob_id].z + (5 + U_get_random(5));
						break;
				case	6 :
						to_x	= D_mob[channel][mob_id].x + (5 + U_get_random(5));
						to_z	= D_mob[channel][mob_id].z;
						break;
				case	7 :
						to_x	= D_mob[channel][mob_id].x;
						to_z	= D_mob[channel][mob_id].z - (5 + U_get_random(5));
						break;
				default :
						to_x	= D_mob[channel][mob_id].x;
						to_z	= D_mob[channel][mob_id].z;
						break;
			}

			if( !D_ChkMob_map_collission(channel, mob_id, (int)to_x, (int)to_z) 
					&& !D_ChkMob_over_area_persuit(channel, mob_id, to_x, to_z) ) 
			{
				D_mob[channel][mob_id].random_x	= (int)to_x + 0.5f;
				D_mob[channel][mob_id].random_z	= (int)to_z + 0.5f;
				return (1);
			}
		}
		else
		{
			switch( U_get_random(4) )
			{
				case	0 :
						to_x	= x + U_get_random(5);
						to_z	= z + U_get_random(5);
						break;
				case	1 :
						to_x	= x - U_get_random(5);
						to_z	= z - U_get_random(5);
						break;
				case	2 :
						to_x	= x + U_get_random(5);
						to_z	= z - U_get_random(5);
						break;
				case	3 :
						to_x	= x - U_get_random(5);
						to_z	= z + U_get_random(5);
						break;
				default :
						to_x	= x;
						to_z	= z;
						break;
			}

			if( !D_ChkMob_map_collission(channel, mob_id, (int)to_x, (int)to_z) 
					&& !D_ChkMob_over_area_wander(channel, mob_id, to_x, to_z) ) 
			{
				D_mob[channel][mob_id].random_x	= (int)to_x + 0.5f;
				D_mob[channel][mob_id].random_z	= (int)to_z + 0.5f;
				return (1);
			}
		}
	}

	D_mob[channel][mob_id].random_x	= 0.0f;
	D_mob[channel][mob_id].random_z	= 0.0f;
	return (0);
}


int 	D_GetMob_move_wander( const int channel, const int mob_id, struct body_MG_MOB_MOVE *send_body, int *p_add_move_millisec )
{
	float			to_x, to_z, to_move_xz, to_length;
	int			isMove;


	*p_add_move_millisec	= 0;
	send_body->mob_id	= mob_id;
	send_body->status	= _MOB_WAIT1; 
	send_body->speed	= D_GetMob_move_speed(channel, mob_id, 0);
	send_body->dir		= D_mob[channel][mob_id].dir;
	send_body->x 		= D_mob[channel][mob_id].x;
	send_body->z 		= D_mob[channel][mob_id].z;
	send_body->y 		= 0;

	if( !D_ChkMob_possible_move(channel, mob_id) )
		return (RET_MOVE_STAY);


	to_move_xz	= send_body->speed;

	if( D_GetMob_wander_move_xz(channel, mob_id, D_mob[channel][mob_id].map, SP_round(to_move_xz, 0.5), &to_x, &to_z) )
	{
		to_length = GetLength( send_body->x, send_body->z, to_x, to_z );

		if( to_length < to_move_xz )
		{
			*p_add_move_millisec	= (MILLISEC_TO_NEXT_MOVE * (to_length/to_move_xz)) 
						- MILLISEC_TO_NEXT_MOVE ;
			to_move_xz		= to_length;
		}

		isMove	= D_SetMob_move_xz( channel, mob_id, D_mob[channel][mob_id].x, D_mob[channel][mob_id].z
					  , GetDirection((to_x - D_mob[channel][mob_id].x), (to_z - D_mob[channel][mob_id].z))
					  , to_move_xz );
		if( isMove )
		{
			send_body->status	= _MOB_WALK;
			send_body->dir		= D_mob[channel][mob_id].dir;
			send_body->x 		= D_mob[channel][mob_id].x;
			send_body->z 		= D_mob[channel][mob_id].z;
			return (RET_MOVE_XZ);
		}
		else
			*p_add_move_millisec	= 0;
	}
	
	return (RET_MOVE_NOT_FOUND);
}


int 	D_GetMob_move_persuit( const int tidx, const int channel, const int mob_id, struct body_MG_MOB_MOVE *send_body
				, int *p_add_move_millisec )
{
	float			to_x, to_z, to_move_xz, to_length;
	int			isPath, isMove, area_attack;


	*p_add_move_millisec	= 0;
	send_body->mob_id	= mob_id;
	send_body->status	= _MOB_WAIT1;
	send_body->speed	= D_GetMob_move_speed(channel ,mob_id, 1);
	send_body->dir		= D_mob[channel][mob_id].dir;
	send_body->x 		= D_mob[channel][mob_id].x;
	send_body->z 		= D_mob[channel][mob_id].z;
	send_body->y 		= 0;

	if( !D_ChkMob_possible_move(channel, mob_id) )
		return (RET_MOVE_STAY);

        area_attack		= D_typemob_area_attack(D_mob[channel][mob_id].type);
	to_x			= D_char[channel][D_GetMob_target_char(channel, mob_id)].x;
	to_z			= D_char[channel][D_GetMob_target_char(channel, mob_id)].z;
        to_move_xz		= send_body->speed;

	if( D_ChkMob_over_area_persuit(channel, mob_id, to_x, to_z) )
		return (RET_MOVE_AREA_OVER);


	if( (isPath = SP_Find_Path(tidx, channel, mob_id, D_mob[channel][mob_id].map, (int)D_mob[channel][mob_id].x
		, (int)D_mob[channel][mob_id].z, (int)to_x, (int)to_z)) > 1 )
	{
		to_length  	= GetLength( D_mob[channel][mob_id].x, D_mob[channel][mob_id].z, (SP_to_x(tidx) + 0.5f)
				, (SP_to_z(tidx) + 0.5f) );

		if( (int)to_x == SP_to_x(tidx) && (int)to_z == SP_to_z(tidx) )
			to_length 	-= area_attack;

		*p_add_move_millisec	= (MILLISEC_TO_NEXT_MOVE * (to_length/to_move_xz)) - MILLISEC_TO_NEXT_MOVE;
		to_move_xz		= to_length;

		if( *p_add_move_millisec < (10 - MILLISEC_TO_NEXT_MOVE) )
			*p_add_move_millisec	= -500;

		isMove	= D_SetMob_move_xz( channel, mob_id, D_mob[channel][mob_id].x, D_mob[channel][mob_id].z
					  , GetDirection( ((SP_to_x(tidx) + 0.5f) - D_mob[channel][mob_id].x)
							, ((SP_to_z(tidx) + 0.5f) - D_mob[channel][mob_id].z))
					  , to_move_xz );
		if( isMove )
		{
	//dprintf(channel, "[ DB ]D_GetMob_move_persuit() mob_id(%d), isPath(%d, %d), delay(%.0f), from(%.01f, %.01f) to(%.01f, %.01f) \n"
	//			, mob_id, isPath, SP_history_cnt(tidx), SP_delay(tidx), send_body->x, send_body->z, to_x, to_z );

			send_body->status	= _MOB_RUN;
			send_body->dir 		= D_mob[channel][mob_id].dir;
			send_body->x 		= D_mob[channel][mob_id].x;
			send_body->z 		= D_mob[channel][mob_id].z;

			D_SetMob_attack_start( channel, mob_id, 0 );
			return (RET_MOVE_XZ);
		}
	}

	*p_add_move_millisec	= 0;
	//dprintf(channel, "[ DB ]Error D_GetMob_move_persuit() mob_id(%d), isPath(%d), delay(%.0f), from(%.01f, %.01f) to(%.01f, %.01f) \n"
	//	, mob_id, isPath, SP_delay(tidx), send_body->x, send_body->z, to_x, to_z );
	return (RET_MOVE_NOT_FOUND);
}


int 	D_GetMob_move_regen_area( const int tidx, const int channel, const int mob_id, struct body_MG_MOB_MOVE *send_body
				, int *p_add_move_millisec )
{
	float			to_x, to_z, to_move_xz, to_length;
	int			isPath, isMove;


	*p_add_move_millisec	= 0;
	send_body->mob_id	= mob_id;
	send_body->status	= _MOB_WAIT1;
	if( D_mob[channel][mob_id].status == MOB_STATUS_PERFECT )
		send_body->speed	= D_GetMob_move_speed(channel, mob_id, 2);
	else
		send_body->speed	= D_GetMob_move_speed(channel, mob_id, 1);
	send_body->dir		= D_mob[channel][mob_id].dir;
	send_body->x 		= D_mob[channel][mob_id].x;
	send_body->z 		= D_mob[channel][mob_id].z;
	send_body->y 		= 0;

	if( !D_ChkMob_possible_move(channel, mob_id) )
		return (RET_MOVE_STAY);

	to_move_xz		= send_body->speed;
	to_x			= D_mob[channel][mob_id].regen_x;
	to_z			= D_mob[channel][mob_id].regen_z;

	if( !D_ChkMob_over_area_wander(channel, mob_id, D_mob[channel][mob_id].x, D_mob[channel][mob_id].z) )
		return (RET_MOVE_AREA_OVER);


	if( (isPath = SP_Find_Path(tidx, channel, mob_id, D_mob[channel][mob_id].map, (int)D_mob[channel][mob_id].x
		, (int)D_mob[channel][mob_id].z, (int)to_x, (int)to_z)) > 1 )
	{
		to_length 	= GetLength( D_mob[channel][mob_id].x, D_mob[channel][mob_id].z
				, (SP_to_x(tidx) + 0.5f), (SP_to_z(tidx) + 0.5f) );

		if( to_length < to_move_xz )
		{
			*p_add_move_millisec	= (MILLISEC_TO_NEXT_MOVE * (to_length/to_move_xz)) - MILLISEC_TO_NEXT_MOVE;
			to_move_xz		= to_length;
		}

		isMove	= D_SetMob_move_xz( channel, mob_id, D_mob[channel][mob_id].x, D_mob[channel][mob_id].z
					  , GetDirection( ((SP_to_x(tidx) + 0.5f) - D_mob[channel][mob_id].x)
					  		, ((SP_to_z(tidx) + 0.5f) - D_mob[channel][mob_id].z))
				 	  , to_move_xz );
		if( isMove )
		{
	//dprintf(channel, "[ DB ]D_GetMob_move_regen() mob_id(%d), isPath(%d, %d), delay(%.0f), from(%.01f, %.01f) to(%.01f, %.01f) \n"
	//			, mob_id, isPath, SP_history_cnt(tidx), SP_delay(tidx), send_body->x, send_body->z, to_x, to_z );

			if( D_mob[channel][mob_id].status == MOB_STATUS_PERFECT )
				send_body->status	= _MOB_QUICK_RUN;
			else
				send_body->status	= _MOB_RUN;

			send_body->dir		= D_mob[channel][mob_id].dir;
			send_body->x 		= D_mob[channel][mob_id].x;
			send_body->z 		= D_mob[channel][mob_id].z;
			return (RET_MOVE_XZ);
		}
	}

	*p_add_move_millisec	= 0;
	//dprintf(channel, "[ DB ]Error D_GetMob_move_regen_area() mob_id(%d), isPath(%d), delay(%.0f), from(%.01f, %.01f) to(%.01f, %.01f) \n"
	//	, mob_id, isPath, SP_delay(tidx), send_body->x, send_body->z, to_x, to_z );
	return (RET_MOVE_NOT_FOUND);
}


int 	D_GetMob_move_random_xz( const int tidx, const int channel, const int mob_id, struct body_MG_MOB_MOVE *send_body
				, int *p_add_move_millisec )
{
	float			to_x, to_z, to_move_xz, to_length;
	int			isPath, isMove;


	*p_add_move_millisec	= 0;
	send_body->mob_id	= mob_id;
	send_body->status	= _MOB_WAIT1;
	send_body->speed	= D_GetMob_move_speed(channel, mob_id, 1);
	send_body->dir		= D_mob[channel][mob_id].dir;
	send_body->x 		= D_mob[channel][mob_id].x;
	send_body->z 		= D_mob[channel][mob_id].z;
	send_body->y 		= 0;

	if( !D_ChkMob_possible_move(channel, mob_id) )
		return (RET_MOVE_STAY);

	to_move_xz		= send_body->speed;
	to_x			= D_mob[channel][mob_id].random_x;
	to_z			= D_mob[channel][mob_id].random_z;

	if( (to_x == 0.0f) || (to_z == 0.0f) )
		return (RET_MOVE_AREA_OVER);

	if( (to_length = GetLength(D_mob[channel][mob_id].x, D_mob[channel][mob_id].z, to_x, to_z)) < 2 )
		return (RET_MOVE_AREA_OVER);


	if( (isPath = SP_Find_Path(tidx, channel, mob_id, D_mob[channel][mob_id].map, (int)D_mob[channel][mob_id].x
		, (int)D_mob[channel][mob_id].z, (int)to_x, (int)to_z)) > 1 )
	{
		to_length 	= GetLength( D_mob[channel][mob_id].x, D_mob[channel][mob_id].z
				, (SP_to_x(tidx) + 0.5f), (SP_to_z(tidx) + 0.5f) );

		if( to_length < to_move_xz )
		{
			*p_add_move_millisec	= (MILLISEC_TO_NEXT_MOVE * (to_length/to_move_xz)) - MILLISEC_TO_NEXT_MOVE;
			to_move_xz		= to_length;
		}

		isMove 	= D_SetMob_move_xz( channel, mob_id, D_mob[channel][mob_id].x, D_mob[channel][mob_id].z
					  , GetDirection( ((SP_to_x(tidx) + 0.5f) - D_mob[channel][mob_id].x)
					 		, ((SP_to_z(tidx) + 0.5f) - D_mob[channel][mob_id].z))
					  , to_move_xz );
		if( isMove )
		{
	//dprintf(channel, "[ DB ]D_GetMob_move_random() mob_id(%d), isPath(%d, %d), delay(%.0f), from(%.01f, %.01f) to(%.01f, %.01f) \n"
	//			, mob_id, isPath, SP_history_cnt(tidx), SP_delay(tidx), send_body->x, send_body->z, to_x, to_z );

			send_body->status	= _MOB_RUN;
			send_body->dir		= D_mob[channel][mob_id].dir;
			send_body->x 		= D_mob[channel][mob_id].x;
			send_body->z 		= D_mob[channel][mob_id].z;
			return (RET_MOVE_XZ);
		}
	}

	*p_add_move_millisec	= 0;
	//dprintf(channel, "[ DB ]Error D_GetMob_move_random_xz() mob_id(%d), isPath(%d), delay(%.0f), from(%.01f, %.01f) to(%.01f, %.01f) \n"
	//	, mob_id, isPath, SP_delay(tidx), send_body->x, send_body->z, to_x, to_z );
	return (RET_MOVE_NOT_FOUND);
}


static inline int	D_GetMob_area_skill_distance_mob_list( const int channel, const int mob_id, const int skill_type, char *p_mob_list )
{
	int		in, x, z, chk_sx, chk_sz, chk_ex, chk_ez, temp_mob, mob_cnt, isfull, link_cnt;


	D_GetMob_area_skill_distance( channel, mob_id, skill_type, &chk_sx, &chk_sz, &chk_ex, &chk_ez );
	mob_cnt		= 0;
	isfull		= 0;

	for( x=chk_sx; x<=chk_ex; x++ )
	{
		for( z=chk_sz; z<=chk_ez; z++ )
		{
			link_cnt = 0;
			temp_mob = D_GetMap_mob_link( channel, D_mob[channel][mob_id].map, x, z );

			while( temp_mob )
			{
				if( temp_mob != mob_id )
				{
					in = InCircle( D_mob[channel][mob_id].x, D_mob[channel][mob_id].z, 0
							, D_typemobskill_target_distance(skill_type)
							, D_typemobskill_target_distance(skill_type)
							, D_mob[channel][temp_mob].x, D_mob[channel][temp_mob].z );

					if( in && D_ChkMob_action_to_mob(channel, mob_id, temp_mob) )
					{
						//dprintf(channel, "-> target_mob_idx(%d), target_mob_id(%d), gap(%f) \n"
						//	, mob_cnt, temp_mob	
						//	, GetLength(D_mob[channel][mob_id].x, D_mob[channel][mob_id].z
						//	, D_mob[channel][temp_mob].x, D_mob[channel][temp_mob].z) );
						memcpy( p_mob_list + (mob_cnt * SIZEOF_INT), &temp_mob, SIZEOF_INT );
						mob_cnt ++;

						if( mob_cnt > 9 )
						{
							isfull	= 1;
							break;
						}
					}
				}

				temp_mob = D_GetMap_mob_link_next( channel, temp_mob );

				if( ++link_cnt > 50 )
				{
					dprintf(channel, "[ DB ]Error mob_link(%d, %d) over channel(%d), map(%d), mob_id(%d), link_next(%d)\n"
						, x, z, channel, D_mob[channel][temp_mob].map, temp_mob
						, D_mob[channel][temp_mob].link_next );
					isfull	= 1;
					break;
				}
			}

			if( isfull )	break;
		}

		if( isfull )	break;
	}

	dprintf(channel, "[ DB ]D_GetMob_area_skill_distance_mob_list() mob_id(%d), skill_type(%d), area(%d, %d, %d, %d), mob_cnt(%d) \n"
		, mob_id, skill_type, chk_sx, chk_sz, chk_ex, chk_ez, mob_cnt );
	return (mob_cnt);
}


static inline int	D_GetMob_area_my_circle_mob_list( const int channel, const int mob_id, const int skill_type
			, const int dir, char *p_mob_list )
{
	int		in, x, z, chk_sx, chk_sz, chk_ex, chk_ez, temp_mob, mob_cnt, isfull, link_cnt;


	D_GetMob_area_skill( channel, mob_id, skill_type, D_mob[channel][mob_id].x, D_mob[channel][mob_id].z
			, &chk_sx, &chk_sz, &chk_ex, &chk_ez );
	mob_cnt		= 0;
	isfull		= 0;

	for( x=chk_sx; x<=chk_ex; x++ )
	{
		for( z=chk_sz; z<=chk_ez; z++ )
		{
			link_cnt = 0;
			temp_mob = D_GetMap_mob_link( channel, D_mob[channel][mob_id].map, x, z );

			while( temp_mob )
			{
				if( temp_mob != mob_id )
				{
					in = InCircle( D_mob[channel][mob_id].x, D_mob[channel][mob_id].z, ((270 + dir) % 360)
							, D_typemobskill_range_x(skill_type), D_typemobskill_range_z(skill_type)
							, D_mob[channel][temp_mob].x, D_mob[channel][temp_mob].z );

					if( in && D_ChkMob_action_to_mob(channel, mob_id, temp_mob) )
					{
						//dprintf(channel, "-> target_mob_idx(%d), target_mob_id(%d), gap(%f) \n"
						//	, mob_cnt, temp_mob	
						//	, GetLength(D_mob[channel][mob_id].x, D_mob[channel][mob_id].z
						//	, D_mob[channel][temp_mob].x, D_mob[channel][temp_mob].z) );
						memcpy( p_mob_list + (mob_cnt * SIZEOF_INT), &temp_mob, SIZEOF_INT );
						mob_cnt ++;

						if( mob_cnt > 9 )
						{
							isfull	= 1;
							break;
						}
					}
				}

				temp_mob = D_GetMap_mob_link_next( channel, temp_mob );

				if( ++link_cnt > 50 )
				{
					dprintf(channel, "[ DB ]Error mob_link(%d, %d) over channel(%d), map(%d), mob_id(%d), link_next(%d)\n"
						, x, z, channel, D_mob[channel][temp_mob].map, temp_mob
						, D_mob[channel][temp_mob].link_next );
					isfull	= 1;
					break;
				}
			}

			if( isfull )	break;
		}

		if( isfull )	break;
	}

	dprintf(channel, "[ DB ]D_GetMob_area_my_circle_mob_list() mob_id(%d), skill_type(%d), area(%d, %d, %d, %d), mob_cnt(%d) \n"
		, mob_id, skill_type, chk_sx, chk_sz, chk_ex, chk_ez, mob_cnt );
	return (mob_cnt);
}


static inline int	D_GetMob_area_my_spray_mob_list( const int channel, const int mob_id, const int skill_type, const int dir
			, char *p_mob_list, float *p_target_x, float *p_target_z )
{
	int		in, x, z, chk_sx, chk_sz, chk_ex, chk_ez, temp_mob, mob_cnt, isfull, link_cnt;
	float		middle_x, middle_z, move_x, move_z;


	GetCirclePos( D_typemobskill_target_distance(skill_type), ((270 + dir) % 360), &move_x, &move_z );
	middle_x	= D_mob[channel][mob_id].x + move_x;
	middle_z	= D_mob[channel][mob_id].z + move_z;

	D_GetMob_area_skill( channel, mob_id, skill_type, middle_x, middle_z, &chk_sx, &chk_sz, &chk_ex, &chk_ez );
	mob_cnt		= 0;
	isfull		= 0;

	for( x=chk_sx; x<=chk_ex; x++ )
	{
		for( z=chk_sz; z<=chk_ez; z++ )
		{
			link_cnt = 0;
			temp_mob = D_GetMap_mob_link( channel, D_mob[channel][mob_id].map, x, z );

			while( temp_mob )
			{
				if( temp_mob != mob_id )
				{
					in = InCircle( middle_x, middle_z, ((270 + dir) % 360)
							, D_typemobskill_range_x(skill_type), D_typemobskill_range_z(skill_type)
							, D_mob[channel][temp_mob].x, D_mob[channel][temp_mob].z );

					if( in && D_ChkMob_action_to_mob(channel, mob_id, temp_mob) )
					{
						//dprintf(channel, "-> target_mob_idx(%d), target_mob_id(%d), gap(%f) \n"
						//	, mob_cnt, temp_mob	
						//	, GetLength(D_mob[channel][mob_id].x, D_mob[channel][mob_id].z
						//	, D_mob[channel][temp_mob].x, D_mob[channel][temp_mob].z) );
						memcpy( p_mob_list + (mob_cnt * SIZEOF_INT), &temp_mob, SIZEOF_INT );
						mob_cnt ++;

						if( mob_cnt > 9 )
						{
							isfull	= 1;
							break;
						}
					}
				}

				temp_mob = D_GetMap_mob_link_next( channel, temp_mob );

				if( ++link_cnt > 50 )
				{
					dprintf(channel, "[ DB ]Error mob_link(%d, %d) over channel(%d), map(%d), mob_id(%d), link_next(%d)\n"
						, x, z, channel, D_mob[channel][temp_mob].map, temp_mob
						, D_mob[channel][temp_mob].link_next );
					isfull	= 1;
					break;
				}
			}

			if( isfull )	break;
		}

		if( isfull )	break;
	}

	*p_target_x	= middle_x;
	*p_target_z	= middle_z;
	dprintf(channel, "[ DB ]D_GetMob_area_my_spray_mob_list() mob_id(%d), skill_type(%d), area(%d, %d, %d, %d), mob_cnt(%d) \n"
		, mob_id, skill_type, chk_sx, chk_sz, chk_ex, chk_ez, mob_cnt );
	return (mob_cnt);
}


static inline int	D_GetMob_area_other_circle_mob_list( const int channel, const int mob_id, const int skill_type
			, const int middle_x, const int middle_z, char *p_mob_list )
{
	int		in, x, z, dir, chk_sx, chk_sz, chk_ex, chk_ez, temp_mob, mob_cnt, isfull, link_cnt;


	dir		= GetDirection( (middle_x - D_mob[channel][mob_id].x), (middle_z - D_mob[channel][mob_id].z) );

	D_GetMob_area_skill( channel, mob_id, skill_type, middle_x, middle_z, &chk_sx, &chk_sz, &chk_ex, &chk_ez );
	mob_cnt		= 0;
	isfull		= 0;

	for( x=chk_sx; x<=chk_ex; x++ )
	{
		for( z=chk_sz; z<=chk_ez; z++ )
		{
			link_cnt = 0;
			temp_mob = D_GetMap_mob_link( channel, D_mob[channel][mob_id].map, x, z );

			while( temp_mob )
			{
				if( temp_mob != mob_id )
				{
					in = InCircle( middle_x, middle_z, ((270 + dir) % 360)
							, D_typemobskill_range_x(skill_type), D_typemobskill_range_z(skill_type)
							, D_mob[channel][temp_mob].x, D_mob[channel][temp_mob].z );

					if( in && D_ChkMob_action_to_mob(channel, mob_id, temp_mob) )
					{
						//dprintf(channel, "-> target_mob_idx(%d), target_mob_id(%d), gap(%f) \n"
						//	, mob_cnt, temp_mob	
						//	, GetLength(D_mob[channel][mob_id].x, D_mob[channel][mob_id].z
						//	, D_mob[channel][temp_mob].x, D_mob[channel][temp_mob].z) );
						memcpy( p_mob_list + (mob_cnt * SIZEOF_INT), &temp_mob, SIZEOF_INT );
						mob_cnt ++;

						if( mob_cnt > 9 )
						{
							isfull	= 1;
							break;
						}
					}
				}

				temp_mob = D_GetMap_mob_link_next( channel, temp_mob );

				if( ++link_cnt > 50 )
				{
					dprintf(channel, "[ DB ]Error mob_link(%d, %d) over channel(%d), map(%d), mob_id(%d), link_next(%d)\n"
						, x, z, channel, D_mob[channel][temp_mob].map, temp_mob
						, D_mob[channel][temp_mob].link_next );
					isfull	= 1;
					break;
				}
			}

			if( isfull )	break;
		}

		if( isfull )	break;
	}

	dprintf(channel, "[ DB ]D_GetMob_area_other_circle_mob_list() mob_id(%d), skill_type(%d), area(%d, %d, %d, %d), mob_cnt(%d) \n"
		, mob_id, skill_type, chk_sx, chk_sz, chk_ex, chk_ez, mob_cnt );
	return (mob_cnt);
}


static inline int	D_GetMob_area_vision_char( const int channel, const int mob_id )
{
	int		x, z, chk_sx, chk_sz, chk_ex, chk_ez, temp_char, temp_distance, target_char_id, target_distance;


	target_char_id	= 0;
	target_distance = (MAX_X_TILE * MAX_Z_TILE);

	D_GetMob_area_vision( channel, mob_id, &chk_sx, &chk_sz, &chk_ex, &chk_ez );

	for( x=(chk_sx + 1); x<chk_ex; x++ )
	{
		for( z=(chk_sz + 1); z<chk_ez; z++ )
		{
			if( (temp_char = D_GetMap_char_link(channel, D_mob[channel][mob_id].map, x, z)) )
			{
				if( D_char[channel][temp_char].status >= _USER_DEAD )
					continue;

				if( channel != D_char[channel][temp_char].channel )
					continue;

				if( D_mob[channel][mob_id].map != D_char[channel][temp_char].map )
					continue;

				if( D_ChkMob_map_collission(channel, mob_id, (D_char[channel][temp_char].x)
					, (D_char[channel][temp_char].z)) )
					continue;

				if( D_ChkMob_over_area_persuit(channel, mob_id, D_char[channel][temp_char].x
					, D_char[channel][temp_char].z) )
					continue;

				if( D_ChkMob_over_area_vision(channel, mob_id, D_char[channel][temp_char].x
					, D_char[channel][temp_char].z) )
					continue;

				temp_distance 	= abs((int)(D_char[channel][temp_char].x - D_mob[channel][mob_id].x)) 
						+ abs((int)(D_char[channel][temp_char].z - D_mob[channel][mob_id].z));

				if( temp_distance < target_distance )
				{
					target_char_id	= temp_char;
					target_distance	= temp_distance;
				}	
			}
		}
	}

	return (target_char_id);
}


static inline int	D_GetMob_area_my_circle_char_list( const int channel, const int mob_id, const int skill_type
			, const int dir, char *p_char_list )
{
	int		in, x, z, temp_char, chk_sx, chk_sz, chk_ex, chk_ez, char_cnt, isfull, link_cnt;


	D_GetMob_area_skill( channel, mob_id, skill_type, D_mob[channel][mob_id].x, D_mob[channel][mob_id].z
			, &chk_sx, &chk_sz, &chk_ex, &chk_ez );
	char_cnt	= 0;
	isfull		= 0;

	for( x=chk_sx; x<=chk_ex; x ++ )
	{
		for( z=chk_sz; z<=chk_ez; z ++ )
		{
			link_cnt = 0;
			temp_char = D_GetMap_char_link( channel, D_mob[channel][mob_id].map, x, z );

			while( temp_char )
			{
				in = InCircle( D_mob[channel][mob_id].x, D_mob[channel][mob_id].z, ((270 + dir) % 360)
						, D_typemobskill_range_x(skill_type), D_typemobskill_range_z(skill_type)
						, D_char[channel][temp_char].x, D_char[channel][temp_char].z );

				if( in && D_ChkMob_action_to_char(channel, mob_id, temp_char) )
				{
					//dprintf(channel, "-> target_char_idx(%d), target_char_id(%d), gap(%f) \n"
					//	, char_cnt, temp_char
					//	, GetLength(D_mob[channel][mob_id].x, D_mob[channel][mob_id].z
					//	, D_char[channel][temp_char].x, D_char[channel][temp_char].z) );
					memcpy( p_char_list + (char_cnt * SIZEOF_INT), &temp_char, SIZEOF_INT );
					char_cnt ++;

					if( char_cnt > 9 )
					{
						isfull	= 1;
						break;
					}
				}

				temp_char = D_GetMap_char_link_next( channel, temp_char );

				if( ++link_cnt > 50 )
				{
					dprintf(channel, "[ DB ]Error char_link(%d, %d) over \n", x, z);
					isfull	= 1;
					break;
				}
			}

			if( isfull )	break;
		}

		if( isfull )	break;
	}

	dprintf(channel, "[ DB ]D_GetMob_area_my_circle_char_list() mob_id(%d), skill_type(%d), area(%d, %d, %d, %d), char_cnt(%d) \n"
		, mob_id, skill_type, chk_sx, chk_sz, chk_ex, chk_ez, char_cnt );
	return (char_cnt);
}


static inline int	D_GetMob_area_my_spray_char_list( const int channel, const int mob_id, const int skill_type
			, const int dir, char *p_char_list, float *p_target_x, float *p_target_z )
{
	int		in, x, z, temp_char, chk_sx, chk_sz, chk_ex, chk_ez, char_cnt, isfull, link_cnt;
	float		middle_x, middle_z, move_x, move_z;


	GetCirclePos( D_typemobskill_target_distance(skill_type), ((270 + dir) % 360), &move_x, &move_z );
	middle_x	= D_mob[channel][mob_id].x + move_x;
	middle_z	= D_mob[channel][mob_id].z + move_z;

	D_GetMob_area_skill( channel, mob_id, skill_type, middle_x, middle_z, &chk_sx, &chk_sz, &chk_ex, &chk_ez );
	char_cnt	= 0;
	isfull		= 0;

	for( x=chk_sx; x<=chk_ex; x ++ )
	{
		for( z=chk_sz; z<=chk_ez; z ++ )
		{
			link_cnt = 0;
			temp_char = D_GetMap_char_link( channel, D_mob[channel][mob_id].map, x, z );

			while( temp_char )
			{
				in = InCircle( middle_x, middle_z, ((270 + dir) % 360)
						, D_typemobskill_range_x(skill_type), D_typemobskill_range_z(skill_type)
						, D_char[channel][temp_char].x, D_char[channel][temp_char].z );

				if( in && D_ChkMob_action_to_char(channel, mob_id, temp_char) )
				{
					//dprintf(channel, "-> target_char_idx(%d), target_char_id(%d), gap(%f) \n"
					//	, char_cnt, temp_char
					//	, GetLength(D_mob[channel][mob_id].x, D_mob[channel][mob_id].z
					//	, D_char[channel][temp_char].x, D_char[channel][temp_char].z) );
					memcpy( p_char_list + (char_cnt * SIZEOF_INT), &temp_char, SIZEOF_INT );
					char_cnt ++;

					if( char_cnt > 9 )
					{
						isfull	= 1;
						break;
					}
				}

				temp_char = D_GetMap_char_link_next( channel, temp_char );

				if( ++link_cnt > 50 )
				{
					dprintf(channel, "[ DB ]Error char_link(%d, %d) over \n", x, z);
					isfull	= 1;
					break;
				}
			}

			if( isfull ) 	break;
		}

		if( isfull ) 	break;
	}

	*p_target_x	= middle_x;
	*p_target_z	= middle_z;
	dprintf(channel, "[ DB ]D_GetMob_area_my_spray_char_list() mob_id(%d), skill_type(%d), area(%d, %d, %d, %d), char_cnt(%d) \n"
		, mob_id, skill_type, chk_sx, chk_sz, chk_ex, chk_ez, char_cnt );
	return (char_cnt);
}


static inline int	D_GetMob_area_other_circle_char_list( const int channel, const int mob_id, const int skill_type
			, const int middle_x, const int middle_z, char *p_char_list )
{
	int		in, x, z, dir, temp_char, chk_sx, chk_sz, chk_ex, chk_ez, char_cnt, isfull, link_cnt;


	dir		= GetDirection( (middle_x - D_mob[channel][mob_id].x), (middle_z - D_mob[channel][mob_id].z) );

	D_GetMob_area_skill( channel, mob_id, skill_type, middle_x, middle_z, &chk_sx, &chk_sz, &chk_ex, &chk_ez );
	char_cnt	= 0;
	isfull		= 0;

	for( x=chk_sx; x<=chk_ex; x ++ )
	{
		for( z=chk_sz; z<=chk_ez; z ++ )
		{
			link_cnt = 0;
			temp_char = D_GetMap_char_link( channel, D_mob[channel][mob_id].map, x, z );

			while( temp_char )
			{
				in = InCircle( middle_x, middle_z, ((270 + dir) % 360)
						, D_typemobskill_range_x(skill_type), D_typemobskill_range_z(skill_type)
						, D_char[channel][temp_char].x, D_char[channel][temp_char].z );

				if( in && D_ChkMob_action_to_char(channel, mob_id, temp_char) )
				{
					//dprintf(channel, "-> target_char_idx(%d), target_char_id(%d), gap(%f) \n"
					//	, char_cnt, temp_char
					//	, GetLength(D_mob[channel][mob_id].x, D_mob[channel][mob_id].z
					//	, D_char[channel][temp_char].x, D_char[channel][temp_char].z) );
					memcpy( p_char_list + (char_cnt * SIZEOF_INT), &temp_char, SIZEOF_INT );
					char_cnt ++;

					if( char_cnt > 9 )
					{
						isfull	= 1;
						break;
					}
				}

				temp_char = D_GetMap_char_link_next( channel, temp_char );

				if( ++link_cnt > 50 )
				{
					dprintf(channel, "[ DB ]Error char_link(%d, %d) over \n", x, z);
					isfull	= 1;
					break;
				}
			}

			if( isfull ) 	break;
		}

		if( isfull )	break;
	}

	dprintf(channel, "[ DB ]D_GetMob_area_other_circle_char_list() mob_id(%d), skill_type(%d), area(%d, %d, %d, %d), char_cnt(%d) \n"
		, mob_id, skill_type, chk_sx, chk_sz, chk_ex, chk_ez, char_cnt );
	return (char_cnt);
}


int	D_SetMob_search_target_char( const int channel, const int mob_id )
{
	int	target_char;

	if( D_GetMob_attack_type(channel, mob_id) )
	{
		if( (target_char = D_GetMob_area_vision_char(channel, mob_id)) )
		{
			D_SetMob_target_list( channel, mob_id, target_char, 0, 0 );
			D_SetMob_status( channel, mob_id, MOB_STATUS_ATTACK );

			// set mob_combat_start
			if( !D_GetMob_combat_start(channel, mob_id) )
				D_SetMob_combat_start( channel, mob_id );

			return (target_char);
		}
	}

	return (0);
}


int	D_GetMob_target_char( const int channel, const int mob_id )
{
	return (D_mob[channel][mob_id].target_list[D_mob[channel][mob_id].current_target_idx].char_id);
}


int	D_GetMob_target_list( const int channel, const int mob_id, char *p_buf, int *p_list_cnt )
{
	int					i;
	struct body1_MG_MOB_DAMAGE_LIST		*body1;
	struct body2_MG_MOB_DAMAGE_LIST		*body2;

	body1	= (struct body1_MG_MOB_DAMAGE_LIST *)(p_buf);
	body2	= (struct body2_MG_MOB_DAMAGE_LIST *)(p_buf + sizeof(struct body1_MG_MOB_DAMAGE_LIST));


	*p_list_cnt	= 0;

	for( i=0; i<D_mob[channel][mob_id].target_cnt; i++ )
	{
		if( D_mob[channel][mob_id].target_list[i].damage == 0 )
			continue;

		body2[*p_list_cnt].char_id	= D_mob[channel][mob_id].target_list[i].char_id;
		body2[*p_list_cnt].damage	= D_mob[channel][mob_id].target_list[i].damage;

		*p_list_cnt	+= 1;
	}

	body1->mob_id	= mob_id;
	body1->list_cnt	= *p_list_cnt;

	return (0);
}


static inline void	D_SetMob_init_target_list( const int channel, const int mob_id )
{
	D_mob[channel][mob_id].target_cnt		= 0;
	D_mob[channel][mob_id].current_target_idx	= 0;

	D_mob[channel][mob_id].target_list[0].char_id	= 0;
	D_mob[channel][mob_id].target_list[0].damage	= 0;
}


static inline void	D_SetMob_target_list( const int channel, const int mob_id, const int target_char_id
			, const int damage, const int damage_type )
{
	int	i, is_found, target_idx;


	// check exist
	for( i=0, is_found=0; i<D_mob[channel][mob_id].target_cnt; i ++ )
	{
		if( D_mob[channel][mob_id].target_list[i].char_id == target_char_id ) 
		{
			D_mob[channel][mob_id].target_list[i].damage	+= damage;
			target_idx	= i;
			is_found	= 1;
			break;
		}
	}

	// set new target
	if( !is_found )
	{
		if( D_mob[channel][mob_id].target_cnt > (TARGET_LIST_CNT - 1) )
		{
			dprintf(channel, "[ DB ]Error D_SetMob_target_list() target_cnt over mob_id(%d), char_id(%d), damage(%d) \n"
				, mob_id, target_char_id, damage );
			return;
		}

		D_mob[channel][mob_id].target_list[D_mob[channel][mob_id].target_cnt].char_id	= target_char_id;
		D_mob[channel][mob_id].target_list[D_mob[channel][mob_id].target_cnt].damage	= damage;

		target_idx = D_mob[channel][mob_id].target_cnt;
		D_mob[channel][mob_id].target_cnt ++;
	}

	if( D_mob[channel][mob_id].target_cnt <= 1 )
	{
		D_mob[channel][mob_id].current_target_idx	= 0;
		return;
	}

	if( damage_type & _DAMAGE_PROVOCATION )
	{
		D_mob[channel][mob_id].current_target_idx	= target_idx;
		return;
	}

	if( D_mob[channel][mob_id].target_list[(D_mob[channel][mob_id].current_target_idx)].char_id != target_char_id )
	{
		if( damage_type > _DAMAGE_NORMAL )
		{
			if( U_get_random(100) < 10 )
				D_mob[channel][mob_id].current_target_idx	= target_idx;
		}
		else
		{
			if( U_get_random(100) < 5 )
				D_mob[channel][mob_id].current_target_idx	= target_idx;
		}
	}
}


static inline void	D_SetMob_delete_target_char( const int channel, const int mob_id, const int remove_char_id )
{
	int	i, remove_idx=-1;


	// check target_list's remove_char_id
	for( i=0; i<D_mob[channel][mob_id].target_cnt; i ++ )
	{
		if( D_mob[channel][mob_id].target_list[i].char_id == remove_char_id )
		{
			remove_idx	= i;
			break;
		}
	}
	if( remove_idx == -1 )		
		return;

	// init target_list
	if( D_mob[channel][mob_id].target_cnt <= 1 )
	{
		D_SetMob_init_target_list( channel, mob_id );
		return;
	}

	// shift target_list
	memmove( &(D_mob[channel][mob_id].target_list[remove_idx]), &(D_mob[channel][mob_id].target_list[remove_idx + 1])
		, (D_mob[channel][mob_id].target_cnt - remove_idx - 1) * sizeof(struct target_info) );
	D_mob[channel][mob_id].target_cnt --;

	if( remove_idx <= D_mob[channel][mob_id].current_target_idx )
		D_SetMob_change_current_target_idx( channel, mob_id );
}


static inline void	D_SetMob_change_current_target_idx( const int channel, const int mob_id )
{
	int	i, temp_distance, target_distance, temp_char, target_list_idx;


	target_list_idx	= -1;
	target_distance	= (MAX_X_TILE * MAX_Z_TILE);

	for( i=0; i<D_mob[channel][mob_id].target_cnt; i ++ )
	{
		temp_char	= D_mob[channel][mob_id].target_list[i].char_id;

		if( !D_ChkMob_action_to_char(channel, mob_id, temp_char) )
			continue;

		temp_distance	= abs((int)(D_char[channel][temp_char].x - D_mob[channel][mob_id].x)) 
				+ abs((int)(D_char[channel][temp_char].z - D_mob[channel][mob_id].z));

		if( temp_distance < target_distance )
		{
			target_list_idx	= i;
			target_distance	= temp_distance;
		}	
	}

	if( target_list_idx > -1 )
		D_mob[channel][mob_id].current_target_idx	= target_list_idx;
}


static inline void	D_SetMob_init_combat( const int channel, const int mob_id )
{

	D_mob[channel][mob_id].combat_start	= 0;

	// init target_list
	D_SetMob_init_target_list( channel, mob_id );

	// init action_sec
	D_SetMob_action_sec( channel, mob_id, ACTION_SEC_WANDER_ANIMATION );
	D_SetMob_action_sec( channel, mob_id, ACTION_SEC_SEARCH_TARGET );

	// init skill_sec
	D_SetMob_skill_sec( channel, mob_id, SKILL_AI_COND_WANDER );
	D_SetMob_skill_sec( channel, mob_id, SKILL_AI_COND_MOB_HP_DOWN );
	D_SetMob_skill_sec( channel, mob_id, SKILL_AI_COND_MY_HP_DOWN );
	D_SetMob_skill_sec( channel, mob_id, SKILL_AI_COND_COMBAT_START );
	D_SetMob_skill_sec( channel, mob_id, SKILL_AI_COND_ESCAPE_HP );
	D_SetMob_skill_sec( channel, mob_id, SKILL_AI_COND_ESCAPE_END );

	// init action_num
	D_SetMob_skill_num( channel, mob_id, SKILL_AI_COND_WANDER, 1 );
	D_SetMob_skill_num( channel, mob_id, SKILL_AI_COND_MOB_HP_DOWN, 1 );
	D_SetMob_skill_num( channel, mob_id, SKILL_AI_COND_MY_HP_DOWN, 1 );
	D_SetMob_skill_num( channel, mob_id, SKILL_AI_COND_COMBAT_START, 1 );
	D_SetMob_skill_num( channel, mob_id, SKILL_AI_COND_ESCAPE_HP, 1 );
	D_SetMob_skill_num( channel, mob_id, SKILL_AI_COND_ESCAPE_END, 1 );

	// init hp, ap
	D_SetMob_hp_cur( channel, mob_id, D_GetMob_hp_max(channel, mob_id) );
	D_SetMob_ap_cur( channel, mob_id, D_GetMob_ap_max(channel, mob_id) );
}


static inline void	D_SetMob_combat_start( const int channel, const int mob_id )
{

	D_mob[channel][mob_id].combat_start	= 1;
	D_mob[channel][mob_id].attack_start	= 0;

	// init skill_sec
	D_SetMob_skill_sec( channel, mob_id, SKILL_AI_COND_COMBAT );
	D_SetMob_skill_sec( channel, mob_id, SKILL_AI_COND_BOMB_3 );
	D_SetMob_skill_sec( channel, mob_id, SKILL_AI_COND_EVERY_30 );
	D_SetMob_skill_sec( channel, mob_id, SKILL_AI_COND_LIMIT_3_60 );
	D_SetMob_skill_sec( channel, mob_id, SKILL_AI_COND_LIMIT_5_60 );
	D_SetMob_skill_sec( channel, mob_id, SKILL_AI_COND_LIMIT_8_60 );

	// init action_num
	D_SetMob_skill_num( channel, mob_id, SKILL_AI_COND_COMBAT, 1 );
	D_SetMob_skill_num( channel, mob_id, SKILL_AI_COND_BOMB_3, 1 );
	D_SetMob_skill_num( channel, mob_id, SKILL_AI_COND_EVERY_30, 1 );
	D_SetMob_skill_num( channel, mob_id, SKILL_AI_COND_LIMIT_3_60, 1 );
	D_SetMob_skill_num( channel, mob_id, SKILL_AI_COND_LIMIT_5_60, 1 );
	D_SetMob_skill_num( channel, mob_id, SKILL_AI_COND_LIMIT_8_60, 1 );
}


int	D_ChkMob_straight_target( const int channel, const int mob_id, const int char_id )
{
	int		dir, length, step;
	float		step_x, step_z, to_move_x, to_move_z;
	

	dir		= GetDirection( (D_char[channel][char_id].x - D_mob[channel][mob_id].x)
			, (D_char[channel][char_id].z - D_mob[channel][mob_id].z) );
	length		= GetLength( D_mob[channel][mob_id].x, D_mob[channel][mob_id].z
			, D_char[channel][char_id].x, D_char[channel][char_id].z );

	step_x		= D_mob[channel][mob_id].x;
	step_z		= D_mob[channel][mob_id].z;
	GetCirclePos( 1, ((270 + dir) % 360), &to_move_x, &to_move_z );

	for( step=1; step<length; step++ )
	{
		step_x	+= to_move_x;
		step_z	+= to_move_z;

		if( D_ChkMob_map_collission(channel, mob_id, step_x, step_z) )
		{
			//dprintf(channel, "[ DB ]D_ChkMob_straight_target() collission mob_id(%d), area_attack(%d), length(%.d) \n"
			//	, mob_id, D_typemob_area_attack(D_mob[channel][mob_id].type), length );
			return (0);
		}
	}

	return (1);
}


int	D_ChkMob_action_to_char( const int channel, const int mob_id, const int char_id )
{

	if( D_char[channel][char_id].status >= _USER_DEAD )
		return (0);

	if( channel != D_char[channel][char_id].channel )
	{
		dprintf(channel, "[ DB ]D_ChkMob_action_to_char() wrong_channel mob_id(%d), char_id(%d), channel(%d, %d)\n"
			, mob_id, char_id, channel, D_char[channel][char_id].channel );
		return (0);
	}

	if( D_mob[channel][mob_id].map != D_char[channel][char_id].map )
	{
		dprintf(channel, "[ DB ]D_ChkMob_action_to_char() wrong_map mob_id(%d), char_id(%d), map(%d, %d)\n"
			, mob_id, char_id, D_mob[channel][mob_id].map, D_char[channel][char_id].map );
		return (0);
	}

	if( D_ChkMob_map_collission(channel, mob_id, (int)(D_char[channel][char_id].x), (int)(D_char[channel][char_id].z)) )
	{
		//dprintf(channel, "[ DB ]D_ChkMob_action_to_char() collission mob_id(%d), char_id(%d)\n"
		//	, mob_id, char_id );
		return (0);
	}

	if( D_ChkMob_over_area_persuit(channel, mob_id, D_char[channel][char_id].x, D_char[channel][char_id].z) )
	{
		//dprintf(channel, "[ DB ]D_ChkMob_action_to_char() over_area_persuit mob_id(%d), char_id(%d)\n"
		//	, mob_id, char_id );
		return (0);
	}

	if( D_ChkMob_over_area_8_vision(channel, mob_id, D_char[channel][char_id].x, D_char[channel][char_id].z) )
	{
		//dprintf(channel, "[ DB ]D_ChkMob_action_to_char() over_area_vision(%d), mob_id(%d), char_id(%d)\n"
		//	, D_typemob_area_vision(D_mob[channel][mob_id].type), mob_id, char_id );
		return (0);
	}

	return (char_id);
}


static inline int	D_ChkMob_action_to_mob( const int channel, const int action_mob_id, const int recv_mob_id )
{

	if( D_GetMob_npc_type(channel, recv_mob_id) != NPC_TYPE_MONSTER )
		return (0);

	if( D_mob[channel][recv_mob_id].status >= MOB_STATUS_DEATH )
		return (0);

	if( D_ChkMob_map_collission(channel, action_mob_id, (int)D_mob[channel][recv_mob_id].x
		, (int)D_mob[channel][recv_mob_id].z) )
		return (0);

	return (1);
}


int	D_SetMob_attack_to_char( const int channel, const int mob_id, struct body_MG_MOB_ATTACK_CHAR *send_body )
{
	int		target_char;


	if( !D_GetMob_attack_start(channel, mob_id) )
	{
		D_SetMob_attack_start( channel, mob_id, 1 );
		return (0);
	}

	target_char		= D_GetMob_target_char(channel, mob_id);
	send_body->mob_id	= mob_id;
	send_body->char_id	= target_char;
	send_body->dir		= D_mob[channel][mob_id].dir;

	dprintf(channel, "[ DB ]D_SetMob_attack_to_char() mob_id(%d), char_id(%d), area_attack(%d), gap(%.01f)\n"
		, send_body->mob_id, send_body->char_id
		, D_typemob_area_attack(D_mob[channel][mob_id].type)
		, GetLength(D_mob[channel][mob_id].x, D_mob[channel][mob_id].z, D_char[channel][target_char].x
		, D_char[channel][target_char].z) );

	return (1);
}


static inline int	D_ChkMob_skill_casting( const int channel, const int mob_id )
{
	return	( (D_mob[channel][mob_id].current_skill_type && D_mob[channel][mob_id].skill_casting_millisec) 
			|| D_GetMob_skill_action_cnt(channel, mob_id) ); 
}


void	D_SetMob_init_current_skill( const int channel, const int mob_id )
{
	D_mob[channel][mob_id].current_skill_type 		= 0;
	D_mob[channel][mob_id].skill_casting_millisec 		= 0;
	D_mob[channel][mob_id].skill_target_object_id		= 0;
	D_mob[channel][mob_id].skill_action_cnt			= 0;
	D_mob[channel][mob_id].skill_action_millisec		= 0;
}


static inline void	D_SetMob_current_skill( const int channel, const int mob_id, const int skill_type, const int casting_millisec
			, const int target_object_id, const int action_cnt, const int action_millisec )
{
	D_mob[channel][mob_id].current_skill_type 		= skill_type;
	D_mob[channel][mob_id].skill_casting_millisec 		= casting_millisec;
	D_mob[channel][mob_id].skill_target_object_id		= target_object_id;
	D_mob[channel][mob_id].skill_action_cnt			= action_cnt;
	D_mob[channel][mob_id].skill_action_millisec		= action_millisec;
}


int	D_SetMob_casting_skill( const int channel, const int mob_id, char *p_buf, int *p_target_cnt )
{
	int		skill_type = D_GetMob_current_skill_type(channel, mob_id);

	if( D_ChkMob_skill_casting(channel, mob_id) )
	{
		if( D_SetMob_do_skill_action(channel, mob_id, skill_type, p_buf, p_target_cnt) )
		{
			dprintf(channel, "[ DB ]D_SetMob_casting_skill() mob_id(%d), condition(%d), SKILL_ACTION(%d, %d, %d) \n"
				, mob_id, D_typemobskill_ai_condition(skill_type), skill_type
				, D_GetMob_skill_casting_millisec(channel, mob_id)
				, D_GetMob_skill_action_millisec(channel, mob_id) );
			return (RET_SKILL_ACTION);
		}

		D_SetMob_cancel_skill( channel, mob_id, p_buf );
		return (RET_SKILL_CANCEL);
	}

	return (RET_SKILL_EMPTY);
}


int	D_SetMob_new_skill( const int channel, const int mob_id, const int skill_ai_condition, char *p_buf, int *p_target_cnt )
{
	int	i, skill_type, ap;


	// check casting_skill
	if( D_ChkMob_skill_casting(channel, mob_id) )
		return (RET_SKILL_EMPTY);

	// check skill_ai_condition
	if( !D_ChkMob_have_skill_ai_condition(channel, mob_id, skill_ai_condition) )
		return (RET_SKILL_NO_HAVE);


	// do skill
	for( i=0; i<MAX_MOB_HAVE_SKILL_CNT; i ++ )
	{

		// check skill_type
		if( (skill_type	= D_typemob_skill_type(D_mob[channel][mob_id].type, i)) < 1 )
			continue;

		// check skill_ai_condition
		if( skill_ai_condition != D_typemobskill_ai_condition(skill_type) )
			continue;

		// check summon
		if( D_typemobskill_func_summon(skill_type) && (D_GetMob_summon_cnt(channel, mob_id) < 1) )
			continue;

		// check skill_rate
		if( D_typemob_skill_rate(D_mob[channel][mob_id].type, i) < (U_get_random(100) + 1) )
			continue;

		// check use_ap
		if( (ap = D_GetMob_ap_cur(channel, mob_id) - D_typemobskill_use_ap(skill_type)) < 0 )
			continue;

		// check use skill
		switch( D_SetMob_do_skill_ready(channel, mob_id, skill_type, p_buf) )
		{
			case	RET_SKILL_CASTING :
				dprintf(channel, "[ DB ]D_SetMob_new_skill() mob_id(%d), condition(%d), SKILL_CASTING(%d, %d, %d) \n"
					, mob_id, D_typemobskill_ai_condition(skill_type)
					, skill_type, D_GetMob_skill_casting_millisec(channel, mob_id)
					, D_GetMob_skill_action_millisec(channel, mob_id) );
	 			D_SetMob_ap_cur( channel, mob_id, ap );
				return (RET_SKILL_READY);

			case	RET_SKILL_NO_CASTING :
				if( D_SetMob_do_skill_action(channel, mob_id, skill_type, p_buf, p_target_cnt) )
				{
				dprintf(channel, "[ DB ]D_SetMob_new_skill() mob_id(%d), condition(%d), SKILL_ACTION(%d, %d, %d) \n"
						, mob_id, D_typemobskill_ai_condition(skill_type)
						, skill_type, D_GetMob_skill_casting_millisec(channel, mob_id)
						, D_GetMob_skill_action_millisec(channel, mob_id) );
					D_SetMob_ap_cur( channel, mob_id, ap );
					return (RET_SKILL_ACTION);
				}
		}

		// init temp_skill
		D_SetMob_init_current_skill( channel, mob_id );
	}

	return (RET_SKILL_EMPTY);
}


int	D_ChkMob_cancel_skill( const int channel, const int mob_id, const int damage_type )
{
	if( (damage_type >= _DAMAGE_IMPACT) && D_ChkMob_skill_casting(channel, mob_id) )
		return (1);

	return (0);
}


void	D_SetMob_cancel_skill( const int channel, const int mob_id, char *p_buf )
{
	struct body_MG_MOB_SKILL_CANCEL		*res_body;


	dprintf(channel, "[ DB ]D_SetMob_cancel_skill() mob_id(%d), SKILL_CANCEL(%d, %d, %d) \n"
		, mob_id, D_GetMob_current_skill_type(channel, mob_id)
		, D_GetMob_skill_casting_millisec(channel, mob_id)
		, D_GetMob_skill_action_millisec(channel, mob_id) );

	res_body		= (struct body_MG_MOB_SKILL_CANCEL *)(p_buf);
	res_body->mob_id	= mob_id;
}


static inline int	D_SetMob_do_skill_ready( const int channel, const int mob_id, const int skill_type, char *p_buf )
{
	struct body_MG_MOB_SKILL_READY		*res_body;
	int					target_object_id;


	switch( D_typemobskill_pattern(skill_type) )
	{
		// target_object myself
		case	SKILL_PATTERN_MY_MY_SINGLE :
			if( !(target_object_id = D_SetMob_skill_ready_my_my_single(channel, mob_id, skill_type)) )
				return (RET_SKILL_NO_TARGET);
			break;
		case	SKILL_PATTERN_MY_CHAR_CIRCLE :
			if( !(target_object_id = D_SetMob_skill_ready_my_char_circle(channel, mob_id, skill_type)) )
				return (RET_SKILL_NO_TARGET);
			break;
		case	SKILL_PATTERN_MY_MOB_CIRCLE :
			if( !(target_object_id = D_SetMob_skill_ready_my_mob_circle(channel, mob_id, skill_type)) )
				return (RET_SKILL_NO_TARGET);
			break;

		// target_object char
		case	SKILL_PATTERN_CHAR_CHAR_SINGLE :
			if( !(target_object_id = D_SetMob_skill_ready_char_char_single(channel, mob_id, skill_type)) )
				return (RET_SKILL_NO_TARGET);
			break;
		case	SKILL_PATTERN_CHAR_CHAR_CIRCLE :
			if( !(target_object_id = D_SetMob_skill_ready_char_char_circle(channel, mob_id, skill_type)) )
				return (RET_SKILL_NO_TARGET);
			break;
		case	SKILL_PATTERN_CHAR_CHAR_SPRAY :
			if( !(target_object_id = D_SetMob_skill_ready_char_char_spray(channel, mob_id, skill_type)) )
				return (RET_SKILL_NO_TARGET);
			break;

		// target_object mob
		case	SKILL_PATTERN_MOB_MOB_SINGLE :
			if( !(target_object_id = D_SetMob_skill_ready_mob_mob_single(channel, mob_id, skill_type)) )
				return (RET_SKILL_NO_TARGET);
			break;
		case	SKILL_PATTERN_MOB_MOB_CIRCLE :
			if( !(target_object_id = D_SetMob_skill_ready_mob_mob_circle(channel, mob_id, skill_type)) )
				return (RET_SKILL_NO_TARGET);
			break;
		case	SKILL_PATTERN_MOB_MOB_SPRAY :
			if( !(target_object_id = D_SetMob_skill_ready_mob_mob_spray(channel, mob_id, skill_type)) )
				return (RET_SKILL_NO_TARGET);
			break;

		default :
			dprintf(channel, "[ DB ]Error D_SetMob_do_skill_ready() type(%d), pattern(%x) \n"
				, skill_type, D_typemobskill_pattern(skill_type) );
			return (0);
	}


	D_SetMob_current_skill( channel, mob_id, skill_type, D_typemobskill_casting_millisec(skill_type), target_object_id
		, D_typemobskill_action_cnt(skill_type), D_typemobskill_action_millisec(skill_type) );

	if( D_GetMob_skill_casting_millisec(channel, mob_id) )
	{
		res_body		= (struct body_MG_MOB_SKILL_READY *)(p_buf);
		res_body->mob_id	= mob_id;
		res_body->skill_type	= skill_type;
		res_body->dir		= D_mob[channel][mob_id].dir;
		return (RET_SKILL_CASTING);
	}

	return (RET_SKILL_NO_CASTING);
}


static inline int	D_SetMob_skill_ready_my_my_single( const int channel, const int mob_id, const int skill_type )
{

	switch( D_typemobskill_ai_condition(skill_type) )
	{
		case	SKILL_AI_COND_MY_HP_DOWN :
			if( D_GetMob_hp_cur_percent(channel, mob_id) > 50 )
				return (0);
	}

	return (mob_id);
}


static inline int	D_SetMob_skill_ready_my_char_circle( const int channel, const int mob_id, const int skill_type )
{
	char		buf[4096];

	if( D_GetMob_area_my_circle_char_list(channel, mob_id, skill_type, D_mob[channel][mob_id].dir, buf) < 1 )
		return (0);

	return (mob_id);
}


static inline int	D_SetMob_skill_ready_my_mob_circle( const int channel, const int mob_id, const int skill_type )
{
	char		buf[4096];

	if( D_GetMob_area_my_circle_mob_list(channel, mob_id, skill_type, D_mob[channel][mob_id].dir, buf) < 1 )
		return (0);

	return (mob_id);
}


static inline int	D_SetMob_skill_ready_char_char_single( const int channel, const int mob_id, const int skill_type )
{
	int		target_object_id;


	if( !(target_object_id = D_GetMob_target_char(channel, mob_id)) )
		return (0);

	if( !D_ChkMob_action_to_char(channel, mob_id, target_object_id) )
		return (0);

	if( D_ChkMob_over_area_skill_distance(channel, mob_id, skill_type, D_char[channel][target_object_id].x
		, D_char[channel][target_object_id].z) )
		return (0);

	D_mob[channel][mob_id].dir 	= GetDirection( (D_char[channel][target_object_id].x-D_mob[channel][mob_id].x)
					, (D_char[channel][target_object_id].z-D_mob[channel][mob_id].z) );

	return (target_object_id);
}


static inline int	D_SetMob_skill_ready_char_char_circle( const int channel, const int mob_id, const int skill_type )
{
	int		target_object_id;


	if( !(target_object_id = D_GetMob_target_char(channel, mob_id)) )
		return (0);

	if( !D_ChkMob_action_to_char(channel, mob_id, target_object_id) )
		return (0);

	if( D_ChkMob_over_area_skill_distance(channel, mob_id, skill_type, D_char[channel][target_object_id].x
		, D_char[channel][target_object_id].z) )
		return (0);

	D_mob[channel][mob_id].dir 	= GetDirection( (D_char[channel][target_object_id].x-D_mob[channel][mob_id].x)
					, (D_char[channel][target_object_id].z-D_mob[channel][mob_id].z) );

	return (target_object_id);
}


static inline int	D_SetMob_skill_ready_char_char_spray( const int channel, const int mob_id, const int skill_type )
{
	int		target_object_id;


	if( !(target_object_id = D_GetMob_target_char(channel, mob_id)) )
		return (0);

	if( !D_ChkMob_action_to_char(channel, mob_id, target_object_id) )
		return (0);

	if( D_ChkMob_over_area_skill_distance(channel, mob_id, skill_type, D_char[channel][target_object_id].x
		, D_char[channel][target_object_id].z) )
		return (0);

	D_mob[channel][mob_id].dir 	= GetDirection( (D_char[channel][target_object_id].x-D_mob[channel][mob_id].x)
					, (D_char[channel][target_object_id].z-D_mob[channel][mob_id].z) );

	return (target_object_id);
}


static inline int	D_SetMob_skill_ready_mob_mob_single( const int channel, const int mob_id, const int skill_type )
{
	int		i, target_object_id, target_cnt, temp_object_id;
	char		buf[4096];


	if( !(target_cnt = D_GetMob_area_skill_distance_mob_list(channel, mob_id, skill_type, buf)) )
		return (0);

	target_object_id	= 0;

	switch( D_typemobskill_ai_condition(skill_type) )
	{
		case	SKILL_AI_COND_MOB_HP_DOWN :
			for( i=0; i<target_cnt; i ++ )
			{
				temp_object_id	= *((int *)(buf + (SIZEOF_INT * i)));

				if( D_GetMob_hp_cur_percent(channel, temp_object_id) < 50 )
				{
					target_object_id = temp_object_id;
					break;
				}
			}
			break;
		default :
			temp_object_id	= *((int *)buf);
			break;
	}
	
	if( target_object_id )
	{
		D_mob[channel][mob_id].dir 	= GetDirection( (D_char[channel][target_object_id].x - D_mob[channel][mob_id].x)
						, (D_char[channel][target_object_id].z - D_mob[channel][mob_id].z) );
	}

	return (target_object_id);
}


static inline int	D_SetMob_skill_ready_mob_mob_circle( const int channel, const int mob_id, const int skill_type )
{
	int		i, target_object_id, target_cnt, temp_object_id;
	char		buf[4096];


	if( !(target_cnt = D_GetMob_area_skill_distance_mob_list(channel, mob_id, skill_type, buf)) )
		return (0);

	target_object_id	= 0;

	switch( D_typemobskill_ai_condition(skill_type) )
	{
		case	SKILL_AI_COND_MOB_HP_DOWN :
			for( i=0; i<target_cnt; i ++ )
			{
				temp_object_id	= *((int *)(buf + (SIZEOF_INT * i)));

				if( D_GetMob_hp_cur_percent(channel, temp_object_id) < 50 )
				{
					target_object_id = temp_object_id;
					break;
				}
			}
			break;
		default :
			temp_object_id	= *((int *)buf);
			break;
	}
	
	if( target_object_id )
	{
		D_mob[channel][mob_id].dir 	= GetDirection( (D_char[channel][target_object_id].x - D_mob[channel][mob_id].x)
						, (D_char[channel][target_object_id].z - D_mob[channel][mob_id].z) );
	}

	return (target_object_id);
}


static inline int	D_SetMob_skill_ready_mob_mob_spray( const int channel, const int mob_id, const int skill_type )
{
	int		i, target_object_id, target_cnt, temp_object_id;
	char		buf[4096];


	if( !(target_cnt = D_GetMob_area_skill_distance_mob_list(channel, mob_id, skill_type, buf)) )
		return (0);

	target_object_id	= 0;

	switch( D_typemobskill_ai_condition(skill_type) )
	{
		case	SKILL_AI_COND_MOB_HP_DOWN :
			for( i=0; i<target_cnt; i ++ )
			{
				temp_object_id	= *((int *)(buf + (SIZEOF_INT * i)));

				if( D_GetMob_hp_cur_percent(channel, temp_object_id) < 50 )
				{
					target_object_id = temp_object_id;
					break;
				}
			}
			break;
		default :
			temp_object_id	= *((int *)buf);
			break;
	}
	
	if( target_object_id )
	{
		D_mob[channel][mob_id].dir 	= GetDirection( (D_char[channel][target_object_id].x - D_mob[channel][mob_id].x)
						, (D_char[channel][target_object_id].z - D_mob[channel][mob_id].z) );
	}

	return (target_object_id);
}


static inline int	D_SetMob_do_skill_action( const int channel, const int mob_id, const int skill_type, char *p_buf, int *p_target_cnt )
{
	struct body1_MG_MOB_SKILL_ACTION	*res_body1;
	float					target_x, target_z;


	*p_target_cnt		= 0;

	switch( D_typemobskill_pattern(skill_type) )
	{
		// target_object myself
		case	SKILL_PATTERN_MY_MY_SINGLE :
			if( !D_SetMob_skill_action_my_my_single(channel, mob_id, skill_type, &target_x, &target_z
				, (p_buf + sizeof(struct body1_MG_MOB_SKILL_ACTION)), p_target_cnt) )
				return (0);
			break;
		case	SKILL_PATTERN_MY_CHAR_CIRCLE :
			if( !D_SetMob_skill_action_my_char_circle(channel, mob_id, skill_type, &target_x, &target_z
				, (p_buf + sizeof(struct body1_MG_MOB_SKILL_ACTION)), p_target_cnt) )
				return (0);
			break;
		case	SKILL_PATTERN_MY_MOB_CIRCLE :
			if( !D_SetMob_skill_action_my_mob_circle(channel, mob_id, skill_type, &target_x, &target_z
				, (p_buf + sizeof(struct body1_MG_MOB_SKILL_ACTION)), p_target_cnt) )
				return (0);
			break;

		// target_object char
		case	SKILL_PATTERN_CHAR_CHAR_SINGLE :
			if( !D_SetMob_skill_action_char_char_single(channel, mob_id, skill_type, &target_x, &target_z
				, (p_buf + sizeof(struct body1_MG_MOB_SKILL_ACTION)), p_target_cnt) )
				return (0);
			break;
		case	SKILL_PATTERN_CHAR_CHAR_CIRCLE :
			if( !D_SetMob_skill_action_char_char_circle(channel, mob_id, skill_type, &target_x, &target_z
				, (p_buf + sizeof(struct body1_MG_MOB_SKILL_ACTION)), p_target_cnt) )
				return (0);
			break;
		case	SKILL_PATTERN_CHAR_CHAR_SPRAY :
			if( !D_SetMob_skill_action_char_char_spray(channel, mob_id, skill_type, &target_x, &target_z
				, (p_buf + sizeof(struct body1_MG_MOB_SKILL_ACTION)), p_target_cnt) )
				return (0);
			break;

		// target_object mob
		case	SKILL_PATTERN_MOB_MOB_SINGLE :
			if( !D_SetMob_skill_action_mob_mob_single(channel, mob_id, skill_type, &target_x, &target_z
				, (p_buf + sizeof(struct body1_MG_MOB_SKILL_ACTION)), p_target_cnt) )
				return (0);
			break;
		case	SKILL_PATTERN_MOB_MOB_CIRCLE :
			if( !D_SetMob_skill_action_mob_mob_circle(channel, mob_id, skill_type, &target_x, &target_z
				, (p_buf + sizeof(struct body1_MG_MOB_SKILL_ACTION)), p_target_cnt) )
				return (0);
			break;
		case	SKILL_PATTERN_MOB_MOB_SPRAY :
			if( !D_SetMob_skill_action_mob_mob_spray(channel, mob_id, skill_type, &target_x, &target_z
				, (p_buf + sizeof(struct body1_MG_MOB_SKILL_ACTION)), p_target_cnt) )
				return (0);
			break;


		default :
			dprintf(channel, "[ DB ]Error D_SetMob_do_skill_action() type(%d), pattern(%x) \n"
				, skill_type, D_typemobskill_pattern(skill_type) );
			return (0);
	}

	res_body1			= (struct body1_MG_MOB_SKILL_ACTION *)(p_buf);
	res_body1->mob_id		= mob_id;
	res_body1->skill_type		= D_GetMob_current_skill_type(channel, mob_id);
	res_body1->action_cnt		= D_GetMob_skill_action_cnt(channel, mob_id);
	res_body1->dir			= D_mob[channel][mob_id].dir; 
	res_body1->target_x		= target_x;
	res_body1->target_y		= 0;
	res_body1->target_z		= target_z;
	res_body1->target_cnt		= *p_target_cnt;

	D_SetMob_skill_action_cnt( channel, mob_id, (D_GetMob_skill_action_cnt(channel, mob_id) - 1) );

	return (1);
}


static inline int	D_SetMob_skill_action_my_my_single( const int channel, const int mob_id, const int skill_type
			, float *p_target_x, float *p_target_z, char *p_target_list, int *p_target_cnt )
{
	int		target_object_id, order;


	if( (target_object_id = D_GetMob_skill_target_object_id(channel, mob_id)) != mob_id )
		return (0);

	// check function
	for( order=0; order<MAX_SKILL_ORDER_CNT; order ++ )
	{
		switch( D_typemobskill_function(skill_type, order) )
		{
			case	SKILL_FUNC_SUMMON :
				D_summon_mob_activity( channel, D_mob[channel][mob_id].map, mob_id	
					, D_typemobskill_damage(skill_type, order), D_mob[channel][mob_id].x, D_mob[channel][mob_id].z );
				D_SetMob_summon_cnt( channel, mob_id, (D_GetMob_summon_cnt(channel, mob_id) - 1) );
				break;
		}
	}

	*p_target_x	= D_mob[channel][mob_id].x;
	*p_target_z	= D_mob[channel][mob_id].z;
	*p_target_cnt	= 1;
	memcpy( p_target_list, &target_object_id, SIZEOF_INT );
	return (1);
}


static inline int	D_SetMob_skill_action_my_char_circle( const int channel, const int mob_id, const int skill_type
			, float *p_target_x, float *p_target_z, char *p_target_list, int *p_target_cnt )
{
	int		target_char;


	if( D_GetMob_skill_target_object_id(channel, mob_id) != mob_id )
		return (0);

	if( (target_char = D_GetMob_target_char(channel, mob_id)) )
		D_mob[channel][mob_id].dir 	= GetDirection( (D_char[channel][target_char].x-D_mob[channel][mob_id].x)
						, (D_char[channel][target_char].z-D_mob[channel][mob_id].z) );

	*p_target_x	= D_mob[channel][mob_id].x;
	*p_target_z	= D_mob[channel][mob_id].z;
	*p_target_cnt	= D_GetMob_area_my_circle_char_list( channel, mob_id, skill_type, D_mob[channel][mob_id].dir, p_target_list );
	return (1);
}


static inline int	D_SetMob_skill_action_my_mob_circle( const int channel, const int mob_id, const int skill_type
			, float *p_target_x, float *p_target_z, char *p_target_list, int *p_target_cnt )
{
	int		order, temp_mob_id, target_char, i;


	if( D_GetMob_skill_target_object_id(channel, mob_id) != mob_id )
		return (0);

	*p_target_x	= D_mob[channel][mob_id].x;
	*p_target_z	= D_mob[channel][mob_id].z;
	*p_target_cnt	= D_GetMob_area_my_circle_mob_list( channel, mob_id, skill_type, D_mob[channel][mob_id].dir, p_target_list );

	// check function
	for( order=0; order<MAX_SKILL_ORDER_CNT; order ++ )
	{
		switch( D_typemobskill_function(skill_type, order) )
		{
			case	SKILL_FUNC_HELP_1 :
				if( (target_char = D_GetMob_target_char(channel, mob_id)) )
				{
					for( i=0; i<*p_target_cnt; i ++ )
					{
						temp_mob_id	= *((int *)(p_target_list + (i * SIZEOF_INT)));

						D_SetMob_target_list( channel, temp_mob_id, target_char, 0, _DAMAGE_PROVOCATION );
		
						if( D_mob[channel][temp_mob_id].status <= MOB_STATUS_WANDER )
							D_SetMob_status( channel, temp_mob_id, MOB_STATUS_ATTACK );
					}
				}
				break;
			case	SKILL_FUNC_HELP_2 :
				if( (target_char = D_GetMob_target_char(channel, mob_id)) )
				{
					for( i=0; i<*p_target_cnt; i ++ )
					{
						temp_mob_id	= *((int *)(p_target_list + (i * SIZEOF_INT)));

						D_SetMob_target_list( channel, temp_mob_id, target_char, 0, 0 );
		
						if( D_mob[channel][temp_mob_id].status <= MOB_STATUS_WANDER )
							D_SetMob_status( channel, temp_mob_id, MOB_STATUS_ATTACK );
					}
				}
				break;
			case	SKILL_FUNC_GATHER :
				for( i=0; i<*p_target_cnt; i ++ )
				{
					temp_mob_id	= *((int *)(p_target_list + (i * SIZEOF_INT)));

					if( D_mob[channel][temp_mob_id].status <= MOB_STATUS_WANDER
						&& !D_ChkMob_over_area_wander(channel, temp_mob_id, D_mob[channel][mob_id].x
										, D_mob[channel][mob_id].z)
						&& D_SetMob_random_xz(channel, temp_mob_id, D_mob[channel][mob_id].map
										, D_mob[channel][mob_id].x, D_mob[channel][mob_id].z) )
					{
						D_SetMob_status( channel, temp_mob_id, MOB_STATUS_GATHER );
					}
				}
				break;
		}
	}

	return (1);
}


static inline int	D_SetMob_skill_action_char_char_single( const int channel, const int mob_id, const int skill_type
			, float *p_target_x, float *p_target_z, char *p_target_list, int *p_target_cnt )
{
	int		target_object_id;


	if( !(target_object_id = D_GetMob_skill_target_object_id(channel, mob_id)) )
		return (0);

	if( !D_ChkMob_action_to_char(channel, mob_id, target_object_id) )
		return (0);

	if( D_ChkMob_over_area_skill_distance(channel, mob_id, skill_type, D_char[channel][target_object_id].x
		, D_char[channel][target_object_id].z) )
		return (0);

	D_mob[channel][mob_id].dir 	= GetDirection( (D_char[channel][target_object_id].x-D_mob[channel][mob_id].x)
					, (D_char[channel][target_object_id].z-D_mob[channel][mob_id].z) );

	*p_target_x	= D_char[channel][target_object_id].x;
	*p_target_z	= D_char[channel][target_object_id].z;
	*p_target_cnt	= 1;
	memcpy( p_target_list, &target_object_id, SIZEOF_INT );
	return (1);
}


static inline int	D_SetMob_skill_action_char_char_circle( const int channel, const int mob_id, const int skill_type
			, float *p_target_x, float *p_target_z, char *p_target_list, int *p_target_cnt )
{
	int		target_object_id;


	if( !(target_object_id = D_GetMob_skill_target_object_id(channel, mob_id)) )
		return (0);

	if( !D_ChkMob_action_to_char(channel, mob_id, target_object_id) )
		return (0);

	if( D_ChkMob_over_area_skill_distance(channel, mob_id, skill_type, D_char[channel][target_object_id].x
		, D_char[channel][target_object_id].z) )
		return (0);

	D_mob[channel][mob_id].dir 	= GetDirection( (D_char[channel][target_object_id].x-D_mob[channel][mob_id].x)
					, (D_char[channel][target_object_id].z-D_mob[channel][mob_id].z) );

	*p_target_x	= D_char[channel][target_object_id].x;
	*p_target_z	= D_char[channel][target_object_id].z;
	*p_target_cnt	= D_GetMob_area_other_circle_char_list( channel, mob_id, skill_type, *p_target_x, *p_target_z, p_target_list );
	return (1);
}


static inline int	D_SetMob_skill_action_char_char_spray( const int channel, const int mob_id, const int skill_type
			, float *p_target_x, float *p_target_z, char *p_target_list, int *p_target_cnt )
{
	int		target_object_id;


	if( !(target_object_id = D_GetMob_skill_target_object_id(channel, mob_id)) )
		return (0);

	if( !D_ChkMob_action_to_char(channel, mob_id, target_object_id) )
		return (0);

	if( D_ChkMob_over_area_skill_distance(channel, mob_id, skill_type, D_char[channel][target_object_id].x
		, D_char[channel][target_object_id].z) )
		return (0);

	D_mob[channel][mob_id].dir 	= GetDirection( (D_char[channel][target_object_id].x-D_mob[channel][mob_id].x)
					, (D_char[channel][target_object_id].z-D_mob[channel][mob_id].z) );

	//*p_target_x	= D_char[channel][target_object_id].x;
	//*p_target_z	= D_char[channel][target_object_id].z;
	*p_target_cnt	= D_GetMob_area_my_spray_char_list( channel, mob_id, skill_type, D_mob[channel][mob_id].dir
			, p_target_list, p_target_x, p_target_z );
	return (1);
}


static inline int	D_SetMob_skill_action_mob_mob_single( const int channel, const int mob_id, const int skill_type
			, float *p_target_x, float *p_target_z, char *p_target_list, int *p_target_cnt )
{
	int		target_object_id;


	if( !(target_object_id = D_GetMob_skill_target_object_id(channel, mob_id)) )
		return (0);

	if( !D_ChkMob_action_to_mob(channel, mob_id, target_object_id) )
		return (0);

	if( D_ChkMob_over_area_skill_distance(channel, mob_id, skill_type, D_mob[channel][target_object_id].x
		, D_mob[channel][target_object_id].z) )
		return (0);

	D_mob[channel][mob_id].dir 	= GetDirection( (D_mob[channel][target_object_id].x-D_mob[channel][mob_id].x)
					, (D_mob[channel][target_object_id].z-D_mob[channel][mob_id].z) );

	*p_target_x	= D_mob[channel][target_object_id].x;
	*p_target_z	= D_mob[channel][target_object_id].z;
	*p_target_cnt	= 1;
	memcpy( p_target_list, &target_object_id, SIZEOF_INT );
	return (1);
}


static inline int	D_SetMob_skill_action_mob_mob_circle( const int channel, const int mob_id, const int skill_type
			, float *p_target_x, float *p_target_z, char *p_target_list, int *p_target_cnt )
{
	int		target_object_id;


	if( !(target_object_id = D_GetMob_skill_target_object_id(channel, mob_id)) )
		return (0);

	if( !D_ChkMob_action_to_mob(channel, mob_id, target_object_id) )
		return (0);

	if( D_ChkMob_over_area_skill_distance(channel, mob_id, skill_type, D_mob[channel][target_object_id].x
		, D_mob[channel][target_object_id].z) )
		return (0);

	D_mob[channel][mob_id].dir 	= GetDirection( (D_mob[channel][target_object_id].x-D_mob[channel][mob_id].x)
					, (D_mob[channel][target_object_id].z-D_mob[channel][mob_id].z) );

	*p_target_x	= D_mob[channel][target_object_id].x;
	*p_target_z	= D_mob[channel][target_object_id].z;
	*p_target_cnt	= D_GetMob_area_other_circle_mob_list( channel, mob_id, skill_type, *p_target_x, *p_target_z, p_target_list );
	return (1);
}


static inline int	D_SetMob_skill_action_mob_mob_spray( const int channel, const int mob_id, const int skill_type
			, float *p_target_x, float *p_target_z, char *p_target_list, int *p_target_cnt )
{
	int		target_object_id;


	if( !(target_object_id = D_GetMob_skill_target_object_id(channel, mob_id)) )
		return (0);

	if( !D_ChkMob_action_to_mob(channel, mob_id, target_object_id) )
		return (0);

	if( D_ChkMob_over_area_skill_distance(channel, mob_id, skill_type, D_mob[channel][target_object_id].x
		, D_mob[channel][target_object_id].z) )
		return (0);

	D_mob[channel][mob_id].dir 	= GetDirection( (D_mob[channel][target_object_id].x-D_mob[channel][mob_id].x)
					, (D_mob[channel][target_object_id].z-D_mob[channel][mob_id].z) );

	//*p_target_x	= D_mob[target_object_id].x;
	//*p_target_z	= D_mob[target_object_id].z;
	*p_target_cnt	= D_GetMob_area_my_spray_mob_list( channel, mob_id, skill_type, D_mob[channel][mob_id].dir
			, p_target_list, p_target_x, p_target_z );
	return (1);
}


static inline void	D_SetMob_init_buff( const int channel, const int mob_id )
{
	memset( D_mob[channel][mob_id].buff, 0x00, sizeof(D_mob[channel][mob_id].buff) );
}


void	D_SetMob_buff( const int channel, const int mob_id, const int buff_series, const int buff_value )
{
	if( (buff_series < 0) || (buff_series > (BUFF_SERIES_CNT - 1)) )
	{
		dprintf(channel, "[ DB ]D_SetMob_buff() mob_id(%d), buff_series(%d), buff_value(%d) \n"
			, mob_id, buff_series, buff_value );
		return;
	}

	D_mob[channel][mob_id].buff[buff_series].value	= buff_value;

	switch( buff_series )
	{
		case	_BUFF_DE_ACTIVE_STUN :
		case	_BUFF_DE_ACTIVE_SLEEP :
			if( buff_value )
				D_SetMob_action_millisec( channel, mob_id, MILLISEC_TO_DAY );
			else
				D_SetMob_action_millisec( channel, mob_id, 0 );
			break;
	}
}


static inline int	D_GetMob_buff_value( const int channel, const int mob_id, const int buff_series )
{
	if( (buff_series < 0) || (buff_series > (BUFF_SERIES_CNT - 1)) )
	{
		dprintf(channel, "[ DB ]Error D_GetMob_buff_value() mob_id(%d), buff_series(%d) \n", mob_id, buff_series );
		return (0);
	}

	return (D_mob[channel][mob_id].buff[buff_series].value);
}


static inline int	D_SetMob_recv_impact( const int channel, const int mob_id, const int char_id, const int attack_dir
			, const int damage_type, char *p_buf )
{
	struct body_MG_MOB_MOVE		*res_body;
	int				is_send_move = 0;


	if( D_GetMob_buff_value(channel, mob_id, _BUFF_DE_ACTIVE_STUN) )
		return (0);

	// check impact
	if( damage_type & _DAMAGE_IMPACT )
	{
		if( D_SetMob_impact_xz(channel, mob_id, char_id, attack_dir) )
		{
			D_SetMob_action_millisec( channel, mob_id, 500 );

			res_body		= (struct body_MG_MOB_MOVE *)p_buf;
			res_body->mob_id	= mob_id;
			res_body->status	= _MOB_IMPACT;
			res_body->speed		= D_GetMob_move_speed(channel, mob_id, 2);
			res_body->dir		= D_mob[channel][mob_id].dir;
			res_body->x 		= D_mob[channel][mob_id].x;
			res_body->z 		= D_mob[channel][mob_id].z;
			res_body->y 		= 0;

			is_send_move		= 1;
		}
	}

	return (is_send_move);
}





//______________________________________________________________________________________
// char
int	D_set_char( const int initdb_fd )
{
	int	i, j;

	for( i=1; i<=TOTAL_CHANNEL_CNT; i++ )
	{
		if(!(D_char[i] = (struct character *)calloc((MAX_CHAR_CNT + 1), sizeof(struct character))))
		{
			dprintf(0, "[ DB ]Error D_char calloc() : %s\n", strerror(errno));
			return (-1);
		}

		for( j=0; j<MAX_CHAR_CNT; j++ )
			D_SetChar_init(i, j);
	}


	dprintf(0, "\t[ DB ]Success D_set_char(%d) \n", MAX_CHAR_CNT);
	return (0);
}


static inline void	D_SetChar_init( const int channel, const int char_id )
{
	D_char[channel][char_id].status		= _USER_DEAD;

	D_char[channel][char_id].channel	= 0;
	D_char[channel][char_id].map		= 0;
	D_char[channel][char_id].x		= 0.0f;
	D_char[channel][char_id].y		= 0.0f;
	D_char[channel][char_id].z		= 0.0f;

	memset( D_char[channel][char_id].buff, 0x00, sizeof(D_char[channel][char_id].buff) );
}


void	D_SetChar_channel_in( const int channel, const int char_id, const struct body2_GM_CHAR_CHANNEL_IN char_data )
{

	if( (char_id < 1) || (char_id > (MAX_CHAR_CNT - 1)) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_channel_in() wrong char_id(%d) \n", char_id );
		return;
	}

	if( D_char[channel][char_id].channel ) 
	{
		dprintf(channel, "[ DB ]Error D_SetChar_channel_in() in channel(%d), char_id(%d) \n"
			, D_char[channel][char_id].channel, char_id );
		return;
	}

	if( D_char[channel][char_id].map || D_char[channel][char_id].x || D_char[channel][char_id].z )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_channel_in() in map(%d), xz(%f, %f) char_id(%d) \n"
			, D_char[channel][char_id].map, D_char[channel][char_id].x, D_char[channel][char_id].z, char_id );
		return;
	}

	if( D_char[channel][char_id].link_add )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_channel_in() in link char_id(%d) xz(%f, %f) \n"
			, char_id, D_char[channel][char_id].x, D_char[channel][char_id].z );
		return;
	}

	if( (char_data.map < 1) || (char_data.map > D_get_map_cnt()) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_channel_in() wrong map(%d) \n", char_data.map );
		return;
	}

	if( D_check_invalid_link(channel, char_data.map) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_channel_in() wrong_link channel(%d), map(%d) \n"
			, channel, char_data.map );
		return;
	}

	D_char[channel][char_id].status		= char_data.status;
	D_char[channel][char_id].channel	= channel;
	D_char[channel][char_id].map		= char_data.map;
	D_char[channel][char_id].x		= (int)char_data.x + 0.5f;
	D_char[channel][char_id].y		= char_data.y;
	D_char[channel][char_id].z		= (int)char_data.z + 0.5f;

	D_SetMap_char_link_insert( D_char[channel][char_id].channel, D_char[channel][char_id].map, char_id );
}


void	D_SetChar_channel_out( const int channel, const int char_id )
{
	int		mob_id;


	if( (char_id < 1) || (char_id > (MAX_CHAR_CNT - 1)) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_channel_out() wrong char_id(%d) \n", char_id );
		return;
	}

	if( D_char[channel][char_id].channel != channel )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_channel_out() wrong channel(%d, %d) \n"
			, channel, D_char[channel][char_id].channel );
		return;
	}

	if( (D_char[channel][char_id].map < 1) || (D_char[channel][char_id].map > D_get_map_cnt()) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_channel_out() wrong map(%d) \n", D_char[channel][char_id].map );
		return;
	}

	if( !(D_char[channel][char_id].link_add) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_channel_out() not link_add char_id(%d) map(%d), xz(%f, %f) \n"
			, char_id, D_char[channel][char_id].map, D_char[channel][char_id].x, D_char[channel][char_id].z );
		return;
	}

	if( D_check_invalid_link(D_char[channel][char_id].channel, D_char[channel][char_id].map) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_channel_out() wrong_link channel(%d), map(%d) \n"
			, channel, D_char[channel][char_id].map );
		return;
	}


	D_SetMap_char_link_delete( channel, char_id );

	// normal_mob
	for( mob_id=D_map_start_mob(channel, D_char[channel][char_id].map);
				mob_id<D_map_summon_start_mob(channel, D_char[channel][char_id].map); mob_id++ )
		D_SetMob_delete_target_char( channel, mob_id, char_id );

	// summon_mob
	mob_id = D_GetMap_summon_link( channel, D_char[channel][char_id].map );

	while( mob_id )
	{
		D_SetMob_delete_target_char( channel, mob_id, char_id );

		mob_id = D_GetMap_summon_link_next( channel, mob_id );
	}

	// init char	
	D_SetChar_init( channel, char_id );
}


void	D_SetChar_map_change( const int channel, const int char_id, const struct body_GM_CHAR_MAP_CHANGE char_data )
{

	if( (char_id < 1) || (char_id > (MAX_CHAR_CNT - 1)) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_map_change() wrong char_id(%d) \n", char_id );
		return;
	}

	if( D_char[channel][char_id].channel != channel )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_map_change() wrong channel(%d, %d) \n"
			, channel, D_char[channel][char_id].channel );
		return;
	}

	if( (char_data.map < 1) || (char_data.map > D_get_map_cnt()) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_map_change() wrong map(%d) \n", char_data.map );
		return;
	}

	if( !(D_char[channel][char_id].link_add) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_map_change() not link_add char_id(%d) map(%d), xz(%f, %f) \n"
			, char_id, D_char[channel][char_id].map, D_char[channel][char_id].x, D_char[channel][char_id].z );
		return;
	}

	if( D_check_invalid_link(D_char[channel][char_id].channel, char_data.map) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_map_change() wrong_link channel(%d), map(%d) \n"
			, channel, char_data.map );
		return;
	}


	D_SetChar_channel_out( channel, char_id );

	D_char[channel][char_id].channel	= channel;
	D_char[channel][char_id].status		= char_data.status;
	D_char[channel][char_id].map		= char_data.map;
	D_char[channel][char_id].x		= (int)char_data.x + 0.5f;
	D_char[channel][char_id].y		= char_data.y;
	D_char[channel][char_id].z		= (int)char_data.z + 0.5f;

	D_SetMap_char_link_insert( channel, D_char[channel][char_id].map, char_id );
}


void	D_SetChar_move( const int channel, const int char_id, const int status, const int x, const int y, const int z )
{
	int	mob_id;


	if( (char_id < 1) || (char_id > (MAX_CHAR_CNT - 1)) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_move() wrong char_id(%d) \n", char_id );
		return;
	}

	if( D_char[channel][char_id].channel != channel )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_move() wrong channel(%d, %d) \n"
			, channel, D_char[channel][char_id].channel );
		return;
	}

	if( (D_char[channel][char_id].map < 1) || (D_char[channel][char_id].map > D_get_map_cnt()) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_move() wrong map(%d) \n", D_char[channel][char_id].map );
		return;
	}

	if( !(D_char[channel][char_id].link_add) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_move() not link_add char_id(%d) \n", char_id );
		return;
	}

	if( D_check_invalid_link(D_char[channel][char_id].channel, D_char[channel][char_id].map) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_move() wrong_link channel(%d), map(%d) \n"
			, channel, D_char[channel][char_id].map );
		return;
	}


	D_SetMap_char_link_delete( channel, char_id );

	D_char[channel][char_id].status	= status;
	D_char[channel][char_id].x	= x + 0.5f;
	D_char[channel][char_id].y	= y;
	D_char[channel][char_id].z	= z + 0.5f;

	D_SetMap_char_link_insert( channel, D_char[channel][char_id].map, char_id );


	// check death
	if( status == _USER_DEAD )
	{
		dprintf(channel, "[ DB ]D_SetChar_death() char_id(%d), map(%d), xz(%.01f, %.01f) \n"
			, char_id, D_char[channel][char_id].map, D_char[channel][char_id].x, D_char[channel][char_id].z );

		// normal_mob
		for( mob_id=D_map_start_mob(channel, D_char[channel][char_id].map); 
					mob_id<D_map_summon_start_mob(channel, D_char[channel][char_id].map); mob_id++ )
			D_SetMob_delete_target_char( channel, mob_id, char_id );

		// summon_mob
		mob_id = D_GetMap_summon_link( channel, D_char[channel][char_id].map );

		while( mob_id )
		{
			D_SetMob_delete_target_char( channel, mob_id, char_id );

			mob_id = D_GetMap_summon_link_next( channel, mob_id );
		}
	}
}


int	D_SetChar_attack_to_mob( const int channel, const int char_id, const int mob_id, const int attack_dir, const int damage_type
		, const int damage, const int mob_hp, char *p_buf )
{
	int		ret;


	if( (char_id < 1) || (char_id > (MAX_CHAR_CNT - 1)) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_attack_to_mob() wrong char_id(%d) \n", char_id );
		return (RET_ATTACKED_ERROR);
	}

	if( D_char[channel][char_id].channel != channel )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_attack_to_mob() wrong channel(%d, %d) \n"
			, channel, D_char[channel][char_id].channel );
		return (RET_ATTACKED_ERROR);
	}

	if( (D_char[channel][char_id].map < 1) || (D_char[channel][char_id].map > D_get_map_cnt()) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_attack_to_mob() wrong map(%d) \n", D_char[channel][char_id].map );
		return (RET_ATTACKED_ERROR);
	}

	if( !(D_char[channel][char_id].link_add) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_attack_to_mob() not link_add char_id(%d) map(%d), xz(%f, %f) \n"
			, char_id, D_char[channel][char_id].map, D_char[channel][char_id].x, D_char[channel][char_id].z );
		return (RET_ATTACKED_ERROR);
	}

	if( (mob_id < D_map_start_mob(channel, D_char[channel][char_id].map)) 
		|| (mob_id > D_map_end_mob(channel, D_char[channel][char_id].map)) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_attack_to_mob() channel(%d), mob_id(%d), charge_mob(%d, %d) \n"
			, channel, mob_id, D_map_start_mob(channel, D_char[channel][char_id].map)
			, D_map_end_mob(channel, D_char[channel][char_id].map) );
		return (0);
	}

	if( (attack_dir < 0) || (attack_dir >= 360) )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_attack_to_mob() wrong dir(%d) \n", attack_dir );
		return (RET_ATTACKED_ERROR);
	}

	if( D_GetMob_npc_type(channel, mob_id) != NPC_TYPE_MONSTER )
	{
		dprintf(channel, "[ DB ]Error D_SetChar_attack_to_mob() wrong npc_type(%d) \n"
			, D_GetMob_npc_type(channel, mob_id) );
		return (RET_ATTACKED_ERROR);
	}

	//dprintf(channel, "[ DB ]D_SetChar_attack_to_mob() char_id(%d), mob_id(%d), damage(%d), gap(%.01f) \n"
	//	, char_id, mob_id, damage, GetLength(D_char[channel][char_id].x, D_char[channel][char_id].z
	//	, D_mob[channel][mob_id].x, D_mob[channel][mob_id].z) );

	// set target_list
	D_SetMob_target_list( channel, mob_id, char_id, damage, damage_type );

	// set mob_hp
	D_SetMob_hp_cur( channel, mob_id, mob_hp );

	// chk death
	if( D_SetMob_death(channel, mob_id) )
		return (RET_ATTACKED_DEATH);

	// set mob_status attack
	if( D_mob[channel][mob_id].status <= MOB_STATUS_WANDER )
		D_SetMob_status( channel, mob_id, MOB_STATUS_ATTACK );

	// set mob_combat_start_skill
	if( !D_GetMob_combat_start(channel, mob_id) )
	{
		D_SetMob_combat_start( channel, mob_id );
		ret	= RET_ATTACKED_COMBAT_START_SKILL;
	}
	else
		ret	= RET_ATTACKED_NORMAL;

	// set buff
	if( D_SetMob_recv_impact(channel, mob_id, char_id, attack_dir, damage_type, p_buf) )
		return (RET_ATTACKED_IMPACT_MOVE);

	// set escape and escape_skill
	if( D_SetMob_escape(channel, mob_id) )
		return (RET_ATTACKED_ESCAPE_HP_SKILL);

	return (ret);
}
