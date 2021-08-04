/*____________________________________________________________

	Project name : Qring Online
	File name    : db.c
	Description  : data structure
	Date         : February 14 2005
_____________________________________________________________*/


#include	"global.h"
#include	"util.h"
#include	"mk_packet.h"
#define	_DB_H_
#include	"db.h"


struct	machine_object
{
	char			ip[SIZEOF_IP+1];
	char			outer_ip[SIZEOF_IP+1];
	char			dbagent_ip[SIZEOF_IP+1];
	char			mobserv_ip[SIZEOF_IP+1];
};


struct	world
{
	char			name[WORLD_NAME_LEN+1];

	int			machine_cnt;
	struct	machine_object	machine[MAX_MACHINE+1];
	
	int			channel_cnt;
	int			channel_user[999];
};


struct	view_block
{
	int			char_link[CHANNEL_PER_MACHINE+1];
	int			mob_link[CHANNEL_PER_MACHINE+1];
	int			item_link[CHANNEL_PER_MACHINE+1];
};


struct	map
{
	char			attr[MAP_X_TILE][MAP_Z_TILE];
	struct	view_block	block[VIEW_BLOCK_CNT];
};


struct	item_object
{
	unsigned	short	type;

	union
	{
		unsigned	short	enchant[MAX_ENCHANT_CNT];
		unsigned	short	stack_num;
	};

	union
	{
		unsigned	int	durability;
		time_t			use_end_sec;
	};

	unsigned	int	repair;

	char			serial[ITEM_SERIAL_LEN+1];
};
	

struct	skill_ability
{
	int	damage_type;
	int	attr_attack;
	int	attr_damage;
	int	add_att1;
	float	add_att2;
	int	add_i_att;
	int	add_provocation;
	int	hp_heal;
};


struct  level
{
        int			index;
	int			hp_max;
	int			ap_max;
	int			exp;
	int			att1;
	int			def1;
	int			is_def;
	int			il_def;
	int			i_att;
	int			m_def;
	int			cri;
	float			move_speed1;
	float			move_speed2;
	int			recover_hp;
	int			recover_ap;
};


//struct mob_have_skill
//{
//	int			type;
//	int			rate;
//};


struct type_mob
{
	int			index;
	//char			name;
	int			level;
	int			hp_max;
	int			ap_max;
	int			exp;
	int			att1;
	int			def1;
	int			att2;
	int			def2;
	int			eva;
	int			is_def;
	int			il_def;
	int			i_att;
	int			m_def;	
	int			cri;
	int			recover_hp;
	int			recover_ap;
	float			move_speed1;
	float			move_speed2;
	float			move_speed3;
	int			milli_delay_attack;
	int			att_area;
	int			def_intensity;
	int			att_intensity;
	//struct	mob_have_skill	skill[5];
	//int			area_wander_w;
	//int			area_wander_h;
	//int			attack_type;
	//int			area_vision;
	//int			area_persuit;
	//int			escape_hp;
	//int			milli_delay_regen;
	//int			milli_delay_death;
	int			elements;
	//int			summon;
	//char			talk;
	//char			graphic;
	//float			scale;
	int			drop_type;
	int			money_drop_prob;
	int			money_min;
	int			money_max;
	int			quest_item;
	int			quest;
	int			quest_item_drop_prob;
	int			npc_type;
	//char			regen_effect;
	int			mastery;
	//int			map_icon;
};


struct	type_item
{
	int			index;
	//char			name;
	//char			description;
	int			class;
	int			level;
	int			stack_num;
	int			trade;
	int			buy_price;
	int			sell_price;
	//int			icon_id;
	int			equip_id;
	//int			mesh_id;
	//int			drop_mesh_id;
	//int			shader_id;
	//int			effect_id;
	int			data[15];
};


struct	drop_item
{
	int			item_type;
	int			drop_prob;
};


struct	type_drop
{
	int			index;
	//char			name;
	struct	drop_item	group[3][20];
	int			enchant_drop_prob;
	int			enchant_equip_bitmask;
	int			enchant_option_bitmask;
	int			enchant_level;
	int			enchant_grade_prob[9];	

	int			enchant_equip_group_num;
	int			enchant_equip_group[32];
	int			enchant_option_group_num;
	int			enchant_option_group[32];
};


struct	type_warp
{
	int	index;
	//char	name;
	int	req_quest;
	int	req_level;
	int	req_item;
	//int	open_sec;
	//int	close_sec;
	int	dest_map;
	int	dest_x;
	int	dest_z;
	//int	minimap_start_x;
	//int	minimap_start_z;
	//int	minimap_end_x;
	//int	minimap_end_z;
};


struct	type_store
{
	int	index;
	//char	name;
	int	repair;
	//int	tab_id;
	int	item_type[STORE_ITEM_SLOT_CNT];	
};


struct type_skill_order
{
	int			function;
	int			damage;
	int			sec;
};


struct	type_char_skill
{
	int				index;
	//char				name;	
	struct	type_skill_order	order[MAX_SKILL_ORDER_CNT];
	int				method;
	int				req_equip_slot;
	int				req_equip_class;
	int				req_material;
	//int				req_action;
	int				req_special;
	int				max_level;
	int				action_cnt;
	int				action_millisec;
	int				use_ap;
	int				req_buff_level;
	int				target_object;
	int				target_apply;
	int				target_distance;
	int				range_type;
	int				range_x;
	int				range_z;
	int				learn_req_quest;
	int				learn_req_skill;
	int				learn_req_skill_level;
	int				learn_req_skill_point;
	int				learn_req_level;
	int				casting_millisec;
	int				cool_millisec;
	int				cool_serise;
	//char				scripts;
};


struct	equip_passive_char_skill
{
	int				skill_cnt;
	int				skill_type[CHAR_SKILL_CNT];
};


struct	type_mob_skill
{
	int				index;
	//char				name;
	struct	type_skill_order	order[MAX_SKILL_ORDER_CNT];
	int				action_cnt;
	//int				action_millisec;
	int				use_ap;
	int				target_object;
	int				target_apply;
	int				target_distance;
	//int				range_type;
	int				range_x;
	int				range_z;
	int				casting_millisec;
	//int				ai_condition;
	//int				casting_motion;
	//int				active_motion;
	//int				hit_sound;
	//char				scripts;
	//char				monster;
};


struct	item
{
	int			id_next;

	int			block_x;
	int			block_next;
	int			*block_prev;		

	int			channel_link;

	int			map;
	float			x;
	float			y;
	float			z;	
	
	struct	item_object	item;	
	volatile	time_t	drop_time;
	char			own_char_name[CHAR_NAME_LEN+1];	
};


struct	mob
{
	int			type;
	int			stair;
	int			level;
	int			table_idx;
	
	int			block_x;
	int			block_next;
	int			*block_prev;
	
	int			map;
	float			x;
	float			y;
	float			z;
	
	int			dir;
	volatile	int	status;

	int			hp;
	int			ap;

	int			buff_value[BUFF_SERIES_CNT];	
	volatile	time_t	buff_end_sec[BUFF_SERIES_CNT];	
	
	int			hp_max;
	int			ap_max;
	int			m_def;
	int			att1;
	int			att2;
	int			def1;
	int			def2;
	int			eva;
	int			cri;
	int			i_att;
	int			is_def;
	int			il_def;
	int			recover_hp;
	int			recover_ap;
	int			att_area;
	int			att_milli_speed;
	float			move_speed1;
	float			move_speed2;
	float			move_speed3;
};


struct	client
{
	int			sec_active;	
	int			session;			

	int			billing_idx;
	int			use_billing_idx;
	int			dbagent_idx;
	int			use_dbagent_idx;
	int			login_dbagent_idx;
	int			use_login_dbagent_idx;

	int			mobserv_charge_channel_link;

	char			ip[SIZEOF_IP+1];
	char			lock_user_id[USER_ID_LEN+1];	
	char			user_id[USER_ID_LEN+1];	
	char			char_name[CHAR_NAME_LEN+1];

	int			channel_link;

	int			dir;
	volatile	int	status;
	
	int			block_x;
	int			block_next;
	int			*block_prev;
	
	int			map;
	float			x;
	float			y;
	float			z;
	int			regen_map;
	int			regen_x;
	int			regen_y;
	int			regen_z;
	
	int			sex;
	int			hair;
	int			face;
	int			level;
	int			exp;
	int			hp;
	int			ap;
	int			load_cnt;
	int			char_load_sec;
	
	int			money;
	struct	item_object	item_slot[ITEM_SLOT_CNT];

	int			skill_point;
	int			skill_slot[CHAR_SKILL_CNT];

	time_t			cool_end_sec[MAX_COOL_SERIES+1];
	
	int			buff_value[BUFF_SERIES_CNT];	
	volatile	time_t	buff_end_sec[BUFF_SERIES_CNT];	

	int			hp_max;
	int			hp_max_equip;
	int			ap_max;
	int			ap_max_equip;
	int			att_land_equip;
	int			att_water_equip;
	int			att_fire_equip;
	int			att_wind_equip;
	int			m_def;
	int			m_def_equip;
	int			att1;
	int			att1_equip;
	int			att2_equip;
	int			def1;
	int			def1_equip;
	int			def2_equip;
	int			eva_equip;
	int			cri;
	int			cri_equip;
	int			i_att;
	int			i_att_equip;
	int			is_def;
	int			is_def_equip;
	int			il_def;
	int			il_def_equip;
	int			recover_hp;
	int			recover_hp_equip;
	int			recover_ap;
	int			recover_ap_equip;
	int			att_area;
	int			att_milli_speed;
	float			move_speed1;
	float			move_speed2;

	int			last_attack_type; 
	int			transform_type;
	int			action_skill;
	int			remain_action_skill;
};


static	inline	int	D_set_myinfo(const int);
static	inline	int	D_set_world_name(const int);
static	inline	int	D_set_machine_ip(const int);
static	inline	int	D_set_machine_dbagent_ip(const int);
static	inline	int	D_set_machine_mobserv_ip(const int);
static	inline	int	D_set_map(const int);
static	inline	int	D_set_level(const int);
static	inline	int	D_set_type_warp(const int);
static	inline	int	D_set_type_mob(const int);
static	inline	int	D_set_mob(const int);
static	inline	int	D_set_type_item(const int);
static	inline	int	D_set_type_drop(const int);
static	inline	int	D_set_drop_enchant(void);
static	inline	int	D_set_type_store(const int);
static	inline	int	D_set_type_char_skill(const int);
static	inline	int	D_set_equip_passive_char_skill(void);
static	inline	int	D_set_type_mob_skill(const int);

static	inline	int	D_get_mob_hp_max(const int, const int);
static	inline	int	D_get_mob_ap_max(const int, const int);
static	inline	int	D_get_mob_att_land(const int, const int);
static	inline	int	D_get_mob_att_water(const int, const int);
static	inline	int	D_get_mob_att_fire(const int, const int);
static	inline	int	D_get_mob_att_wind(const int, const int);
static	inline	int	D_get_mob_m_def(const int, const int);
static	inline	int	D_get_mob_att1(const int, const int);
static	inline	int	D_get_mob_att2(const int, const int);
static	inline	int	D_get_mob_def1(const int, const int);
static	inline	int	D_get_mob_def2(const int, const int);
static	inline	int	D_get_mob_eva(const int, const int);
static	inline	int	D_get_mob_cri(const int, const int);
static	inline	int	D_get_mob_i_att(const int, const int);
static	inline	int	D_get_mob_is_def(const int, const int);
static	inline	int	D_get_mob_il_def(const int, const int);
static	inline	int	D_get_mob_recover_hp(const int, const int);
static	inline	int	D_get_mob_recover_ap(const int, const int);
static	inline	int	D_get_mob_att_area(const int, const int);
static	inline	int	D_get_mob_att_milli_speed(const int, const int);
static	inline	float	D_get_mob_move_speed1(const int, const int);
static	inline	float	D_get_mob_move_speed2(const int, const int);
static	inline	float	D_get_mob_move_speed3(const int, const int);

static	int	D_get_client_hp_max(const int);
static	int	D_get_client_ap_max(const int);
static	int	D_get_client_att_land(const int);
static	int	D_get_client_att_water(const int);
static	int	D_get_client_att_fire(const int);
static	int	D_get_client_att_wind(const int);
static	int	D_get_client_m_def(const int);
static	int	D_get_client_att1(const int);
static	int	D_get_client_att2(const int);
static	int	D_get_client_def1(const int);
static	int	D_get_client_def2(const int);
static	int	D_get_client_eva(const int);
static	int	D_get_client_cri(const int);
static	int	D_get_client_i_att(const int);
static	int	D_get_client_is_def(const int);
static	int	D_get_client_il_def(const int);
static	int	D_get_client_recover_hp(const int);
static	int	D_get_client_recover_ap(const int);
static	int	D_get_client_att_area(const int);
static	int	D_get_client_att_milli_speed(const int);
static	float	D_get_client_move_speed1(const int);
static	float	D_get_client_move_speed2(const int);

static	void	D_make_item(struct item_object *, const int, const int, const char *);
static	int	D_get_item_id(void);
static	void	D_return_item_id(const int);
static	int	D_set_item_drop(const int, const int, const int, const int, const int, const int, const struct item_object *, const char *);

static	int	D_check_char_attack_type(const int, const int, float *, int *);
static	int	D_compute_damage(const int, const int, const int, const int, const int, const float);
static	int	D_check_evasion(const int, const int, const int, const int);
static	int	D_check_critical(const int, const int, const int, const int, const int);
static	int	D_check_impact(const int, const int, const int, const int, const int);
static	int	D_check_stun(const int, const int, const int, const int, const int);
static	int	D_check_provocation(const int, const int, const int, const int, const int);

static	void	D_set_client_enchant(const int, const int, const int);
static	void	D_set_client_clear_enchant(const int, const int, const int);
static	int	D_set_client_item(const int, const int, const struct item_object *, int *);
static	void	D_set_client_clear_item(const int, const int, int *, int *);
static	void	D_set_client_passive_buff(const int, const int, int *);
static	void	D_set_client_clear_passive_buff(const int, const int, int *);
static	void	D_set_client_level_up(const int);

static	void	D_get_skill_ability(const int, const int, int *, int *, int *, int *, struct skill_ability *);
static	void	D_compute_skill_damage(const int, const int, const int, const int, struct skill_ability *, int *, int *);
static	int	D_compute_skill_attr_damage(const int, const int, const int, const int, const int, const int);


static	const	int	D_max_int	= 0x7fffffff;			
static	const	int	D_min_int	= 0x80000000;
static	const	int	D_max_ushort	= 0xffff;
static	const	int	D_max_uchar	= 0xff;

static	int	D_max_world;
static	int	D_max_map;
static	int	D_max_level;
static	int	D_max_type_item;
static	int	D_max_type_drop;
static	int	D_max_type_warp;
static	int	D_max_type_mob;
static	int	D_max_mob;
static	int	D_max_type_store;
static	int	D_max_type_char_skill_level;
static	int	D_max_type_mob_skill;

static	int	D_item_id_link;

static	struct	world				D_world[999];
static	struct	map				*D_map;
static	struct	level				*D_level;
static	struct	type_item			*D_type_item;
static	struct	type_drop			*D_type_drop;
static	struct	type_warp			*D_type_warp;
static	struct	type_mob			*D_type_mob;
static	struct	type_store			*D_type_store;
static	struct	type_char_skill			(*D_type_char_skill)[CHAR_SKILL_CNT+1];
static	struct	type_mob_skill			*D_type_mob_skill;
static	struct	item				D_item[MAX_DROP_ITEM+1];
static	struct	mob				(*D_mob)[CHANNEL_PER_MACHINE+1];
static	struct	client				D_client[MAX_FD+1];
static	int					(*D_drop_enchant)[MAX_ENCHANT_EQUIP_GROUP+1][MAX_ENCHANT_OPTION_GROUP+1][MAX_ENCHANT_GRADE+1];
static	struct	equip_passive_char_skill	D_equip_passive_char_skill[MAX_ITEM_CLASS+1];



//________________________________________________________________
//
int	D_SetDb_init(void)
{
	int				sock_fd, n, i, j, use_billing_idx, use_dbagent_idx, use_login_dbagent_idx;
	struct	sockaddr_in		addr;
	char				buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	struct	body_SW_INIT_RESULT	*body;

	
	//connect script  
	if( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		U_Printf("[DB]Error socket() : %s\n", strerror(errno));
    		return	(-1);
	}

	bzero(&addr, sizeof(addr));
  	addr.sin_family		= AF_INET;
  	addr.sin_port		= htons(SCRIPT_PORT);
	
	if( inet_pton(AF_INET, G_Script_ip, &addr.sin_addr) <= 0 )
	{
		U_Printf("[DB]Error inet_pton(%s)\n", G_Script_ip);
    		return	(-1);
	}

	if( U_Nonb_connect(sock_fd, (struct sockaddr *)&addr, 3) )
	{
		U_Printf("[DB]Error U_Nonb_connect()\n");
    		return	(-1);
	}


	//send	WS_INIT
	n	= MK_GetPacket_WS_INIT(buf);
	
	if( U_Nonb_send(sock_fd, buf, n, 3) )
	{
		U_Printf("[DB]Error U_Nonb_send()\n");
    		return	(-1);
	}

	//recv SW_INIT_RESULT
	if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL +  sizeof(struct body_SW_INIT_RESULT), 10) )
	{
		U_Printf("[DB]Error U_Nonb_recv\n");
    		return	(-1);
	}

	//check result_code	
	body	= (struct body_SW_INIT_RESULT *)(buf + SIZEOF_HEADER_SPECIAL);

	if( body->result_code )
	{
		U_Printf("[DB]Error bad result_code(%d)\n", body->result_code);
    		return	(-1);
	}

	//set G_Game, G_World, G_Server_code
	if( D_set_myinfo(sock_fd) )
	{
		U_Printf("[DB]Error D_set_myinfo()\n");
		return	(-1);
	}

	//set D_max_world, D_world.name
	if( D_set_world_name(sock_fd) )
	{
		U_Printf("[DB]Error D_set_world_name()\n");
		return	(-1);
	}	
	
	//set D_world.machine_cnt, D_world.channel_cnt, D_world.machine.ip, D_world.machine.outer_ip
	if( D_set_machine_ip(sock_fd) )
	{
		U_Printf("[DB]Error D_set_machine_ip()\n");
		return	(-1);
	}	

	//set D_world.machine.dbagent_ip
	if( D_set_machine_dbagent_ip(sock_fd) )
	{
		U_Printf("[DB]Error D_set_machine_dbagent_ip()\n");
		return	(-1);
	}
	
	//set D_world.machine.mobserv_ip
	if( D_set_machine_mobserv_ip(sock_fd) )
	{
		U_Printf("[DB]Error D_set_machine_mobserv_ip()\n");
    		return	(-1);
	}

	//set D_max_map, D_map
	if( D_set_map(sock_fd) )
	{
		U_Printf("[DB]Error D_set_map()\n");
    		return	(-1);
	}

	//set D_max_level, D_level
	if( D_set_level(sock_fd) )
	{
		U_Printf("[DB]Error D_set_level()\n");
    		return	(-1);
	}

	//set D_max_type_item, D_type_item
	if( D_set_type_item(sock_fd) )
	{
		U_Printf("[DB]Error D_set_type_item()\n");
    		return	(-1);
	}
		
	//set D_drop_enchant
	if( D_set_drop_enchant() )
	{
		U_Printf("[DB]Error D_set_drop_enchant()\n");
    		return	(-1);
	}
	
	//set D_max_type_drop, D_type_drop
	if( D_set_type_drop(sock_fd) )
	{
		U_Printf("[DB]Error D_set_type_drop()\n");
    		return	(-1);
	}

	//set D_max_type_warp, D_type_warp	
	if( D_set_type_warp(sock_fd) )
	{
		U_Printf("[DB]Error D_set_type_warp()\n");
    		return	(-1);
	}
 
	//set D_max_type_store, D_type_store
	if( D_set_type_store(sock_fd) )
	{
		U_Printf("[DB]Error D_set_type_store()\n");
    		return	(-1);
	}

	//set D_max_type_mob, D_type_mob
	if( D_set_type_mob(sock_fd) )
	{
		U_Printf("[DB]Error D_set_type_mob()\n");
    		return	(-1);
	}
	
	//set D_max_mob, D_mob
	if( D_set_mob(sock_fd) )
	{
		U_Printf("[DB]Error D_set_mob()\n");
    		return	(-1);
	}

	//set D_max_type_char_skill_level, D_type_char_skill
	if( D_set_type_char_skill(sock_fd) )
	{
		U_Printf("[DB]Error D_set_type_char_skill()\n");
    		return	(-1);
	}
	
	//set D_equip_passive_char_skill
	if( D_set_equip_passive_char_skill() )
	{
		U_Printf("[DB]Error D_set_equip_passive_char_skill()\n");
    		return	(-1);
	}
	
	//set D_max_mob_skill, D_type_mob_skill
	if( D_set_type_mob_skill(sock_fd) )
	{
		U_Printf("[DB]Error D_set_type_mob_skill()\n");
    		return	(-1);
	}

	//alloc memory
	if( (G_Buff_memory = calloc(D_max_map+1, SIZEOF_INT*(CHANNEL_PER_MACHINE+1)*VIEW_BLOCK_CNT)) == NULL )
	{
		U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    		return	(-1);
	}
	
	if( (G_Tick_mob_memory = calloc((D_max_mob+1)*(CHANNEL_PER_MACHINE+1)*2, sizeof(time_t))) == NULL )
	{
		U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    		return	(-1);
	}


	//init D_item_id_link, D_item
	D_item_id_link	= 1;
	
	for( i=1; i<MAX_DROP_ITEM; i++ )
		D_item[i].id_next	= i+1;	


	//init D_client
	use_billing_idx		= 1;
	use_dbagent_idx		= 1;
	use_login_dbagent_idx	= 1;

	for( i=0; i<=MAX_FD; i++ )
	{
		D_client[i].use_billing_idx		= use_billing_idx;
		use_billing_idx				= ( (use_billing_idx == CONNECTION_BILLING) ? 1 : use_billing_idx+1);
		
		D_client[i].use_dbagent_idx		= use_dbagent_idx;
		use_dbagent_idx				= ( (use_dbagent_idx == CONNECTION_DBAGENT) ? 1 : use_dbagent_idx+1);
		
		D_client[i].use_login_dbagent_idx	= use_login_dbagent_idx;
		use_login_dbagent_idx			= ( (use_login_dbagent_idx == CONNECTION_LOGIN_DBAGENT) ? 1 : use_login_dbagent_idx+1);
	
		D_client[i].status		= _USER_EXTINCT;
	}


	//print D_world
	for( i=1; i<=D_max_world; i++ )
	{
		U_Printf("[DB]D_world[%d] name(%s) machine_cnt(%d) channel_cnt(%d)\n",
						i, D_world[i].name, D_world[i].machine_cnt, D_world[i].channel_cnt);
		
		for( j=1; j<=D_world[i].machine_cnt; j++ )
			U_Printf("[DB] - machine(%d) ip(%s) outer_ip(%s) dbagent_ip(%s) mobserv_ip(%s)\n", j, D_world[i].machine[j].ip,
						D_world[i].machine[j].outer_ip, D_world[i].machine[j].dbagent_ip, D_world[i].machine[j].mobserv_ip);
	}

	close(sock_fd);
	return	(0);
} 


static	int	D_set_myinfo(const int sock_fd)
{
	char				buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	int				n;
	struct	body_SW_MYINFO_RESULT	body;
		

	//send WS_MYINFO
	n	= MK_GetPacket_WS_MYINFO(buf);

	if( U_Nonb_send(sock_fd, buf, n, 3) )
	{
		U_Printf("[DB]Error U_Nonb_send()\n");
    		return	(-1);
	}

	//recv header
	if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}

	//recv body	
	if( U_Nonb_recv(sock_fd, (char *)&body, sizeof(struct body_SW_MYINFO_RESULT), 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}
	
	//check G_Game, G_World	
	if( (body.game < 1) || (body.game >= 999) || (body.world < 1) || (body.world >= 999) )
	{
		U_Printf("[DB]Error bad game(%d), world(%d)\n", body.game, body.world);
    		return	(-1);
	}
	
	//set G_Game, G_World, G_Server_code
	G_Game		= body.game;
	G_World		= body.world;
	G_Server_code	= G_Game*1000000 + G_World*1000 + G_Machine;
	
	U_Printf("[DB]complete G_Game(%d), G_World(%d), G_Server_code(%d)\n", G_Game, G_World, G_Server_code);
	return	(0);
}


static	int	D_set_world_name(const int sock_fd)
{
	char					buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	int					n, i;
	struct	body1_SW_WORLD_INFO_RESULT	body1;
	struct	body2_SW_WORLD_INFO_RESULT	body2;


	//send WS_WORLD_INFO
	n	= MK_GetPacket_WS_WORLD_INFO(buf, G_Game);

	if( U_Nonb_send(sock_fd, buf, n, 3) )
	{
		U_Printf("[DB]Error U_Nonb_send()\n");
    		return	(-1);
	}

	//recv header
	if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}

	//recv body1	
	if( U_Nonb_recv(sock_fd, (char *)&body1, sizeof(struct body1_SW_WORLD_INFO_RESULT), 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}
	
	//check info_cnt
	if( (body1.info_cnt < 1) || (body1.info_cnt >= 999) )
	{
		U_Printf("[DB]Error bad info_cnt(%d)\n", body1.info_cnt);
    		return	(-1);
	}
		
	//set D_max_world
	D_max_world	= body1.info_cnt;

	//recv info	
	for( i=1; i<=body1.info_cnt; i++ )
	{
		//recv body2
		if( U_Nonb_recv(sock_fd, (char *)&body2, sizeof(struct body2_SW_WORLD_INFO_RESULT), 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
    			return	(-1);
		}

		//check body2.world
		if( (body2.world < 1) || (body2.world > D_max_world) )
		{
			U_Printf("[DB]Error bad world(%d)\n", body2.world);
    			return	(-1);
		}

		//check body2.name
		if( (body2.name[0] == 0) || (body2.name[WORLD_NAME_LEN] != 0) )
		{
			body2.name[WORLD_NAME_LEN]	= 0;
			U_Printf("[DB]Error world(%d) bad name(%s)\n", body2.world, body2.name);
    			return	(-1);
		}
	
		//check D_world.name	
		if( D_world[body2.world].name[0] )
		{
			U_Printf("[DB]Error world(%d) name(%s) dup name(%s)\n", body2.world, D_world[body2.world].name, body2.name);
    			return	(-1);
		}	
		
		//set D_world.name
		strncpy(D_world[body2.world].name, body2.name, WORLD_NAME_LEN+1);
	}

	U_Printf("[DB]complete D_max_world(%d)\n", D_max_world);
	return	(0);
}


static	int	D_set_machine_ip(const int sock_fd)
{
	char					buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	int					n, i, j;
	struct	body1_SW_GAMESERV_INFO_RESULT	body1;
	struct	body2_SW_GAMESERV_INFO_RESULT	body2;
		

	for( i=1; i<=D_max_world; i++ )
	{
		//send WS_GAMESERV_INFO
		n	= MK_GetPacket_WS_GAMESERV_INFO(buf, G_Game, i);

		if( U_Nonb_send(sock_fd, buf, n, 3) )
		{
			U_Printf("[DB]Error U_Nonb_send()\n");
    			return	(-1);
		}

		//recv header
		if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
    			return	(-1);
		}

		//recv body1	
		if( U_Nonb_recv(sock_fd, (char *)&body1, sizeof(struct body1_SW_GAMESERV_INFO_RESULT), 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
    			return	(-1);
		}
	
		//check info_cnt
		if( (body1.info_cnt < 1) || (body1.info_cnt > MAX_MACHINE) )
		{
			U_Printf("[DB]Error MAX_MACHINE(%d) bad info_cnt(%d)\n", MAX_MACHINE, body1.info_cnt);
    			return	(-1);
		}
		
		//set D_world.machine_cnt 
		D_world[i].machine_cnt	= body1.info_cnt;
		
		//set D_world.channel_cnt 
		D_world[i].channel_cnt	= CHANNEL_PER_MACHINE*D_world[i].machine_cnt;
		
		for( j=1; j<=body1.info_cnt; j++ )
		{
			//recv body2
			if( U_Nonb_recv(sock_fd, (char *)&body2, sizeof(struct body2_SW_GAMESERV_INFO_RESULT), 3) )
			{
				U_Printf("[DB]Error U_Nonb_recv()\n");
	    			return	(-1);
			}
			
			//check ip
			if( (body2.ip[0] == 0) || (body2.ip[SIZEOF_IP] != 0) )
			{
				body2.ip[SIZEOF_IP]	= 0;
				U_Printf("[DB]Error world(%d) machine(%d) bad ip(%s)\n", i, j, body2.ip);
	    			return	(-1);
			}
			
			//check outer_ip
			if( (body2.outer_ip[0] == 0) || (body2.outer_ip[SIZEOF_IP] != 0) )
			{
				body2.outer_ip[SIZEOF_IP]	= 0;
				U_Printf("[DB]Error world(%d) machine(%d) bad outer_ip(%s)\n", i, j, body2.outer_ip);
	    			return	(-1);
			}

			//set D_world.machine.ip
			strncpy(D_world[i].machine[j].ip, body2.ip, SIZEOF_IP+1);
			
			//set D_world.machine.outer_ip
			strncpy(D_world[i].machine[j].outer_ip, body2.outer_ip, SIZEOF_IP+1);
		}
	}

	U_Printf("[DB]complete machine ip\n");
	return	(0);
}


static	int	D_set_machine_dbagent_ip(const int sock_fd)
{
	char					buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	int					n, i, j;
	struct	body1_SW_DBAGENT_INFO_RESULT	body1;
	struct	body2_SW_DBAGENT_INFO_RESULT	body2;
	
	
	for( i=1; i<=D_max_world; i++ )
	{
		//send WS_DBAGENT_INFO
		n	= MK_GetPacket_WS_DBAGENT_INFO(buf, G_Game, i);

		if( U_Nonb_send(sock_fd, buf, n, 3) )
		{
			U_Printf("[DB]Error U_Nonb_send()\n");
    			return	(-1);
		}

		//recv header
		if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
    			return	(-1);
		}

		//recv body1	
		if( U_Nonb_recv(sock_fd, (char *)&body1, sizeof(struct body1_SW_DBAGENT_INFO_RESULT), 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
    			return	(-1);
		}
	
		//check info_cnt
		if( body1.info_cnt != D_world[i].machine_cnt )
		{
			U_Printf("[DB]Error world(%d) machine_cnt(%d) bad info_cnt(%d)\n", i, D_world[i].machine_cnt, body1.info_cnt);
    			return	(-1);
		}
		
		for( j=1; j<=body1.info_cnt; j++ )
		{
			//recv body2
			if( U_Nonb_recv(sock_fd, (char *)&body2, sizeof(struct body2_SW_DBAGENT_INFO_RESULT), 3) )
			{
				U_Printf("[DB]Error U_Nonb_recv()\n");
    				return	(-1);
			}
			
			//check ip
			if( (body2.ip[0] == 0) || (body2.ip[SIZEOF_IP] != 0) )
			{
				body2.ip[SIZEOF_IP]	= 0;
				U_Printf("[DB]Error world(%d) machine(%d) bad ip(%s)\n", i, j, body2.ip);
	    			return	(-1);
			}
		
			//set D_world.machine.dbagent_ip
			strncpy(D_world[i].machine[j].dbagent_ip, body2.ip, SIZEOF_IP+1);
		}
	}

	U_Printf("[DB]complete machine dbagent_ip\n");
	return	(0);
}


static	int	D_set_machine_mobserv_ip(const int sock_fd)
{
	char					buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	int					n, i, j;
	struct	body1_SW_MOBSERV_INFO_RESULT	body1;
	struct	body2_SW_MOBSERV_INFO_RESULT	body2;


	for( i=1; i<=D_max_world; i++ )
	{
		//send WS_MOBSERV_INFO
		n	= MK_GetPacket_WS_MOBSERV_INFO(buf, G_Game, i);

		if( U_Nonb_send(sock_fd, buf, n, 3) )
		{
			U_Printf("[DB]Error U_Nonb_send()\n");
    			return	(-1);
		}

		//recv header
		if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
    			return	(-1);
		}

		//recv body1	
		if( U_Nonb_recv(sock_fd, (char *)&body1, sizeof(struct body1_SW_MOBSERV_INFO_RESULT), 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
    			return	(-1);
		}
		
		//check info_cnt
		if( body1.info_cnt != D_world[i].machine_cnt )
		{
			U_Printf("[DB]Error world(%d) machine_cnt(%d) bad info_cnt(%d)\n", i, D_world[i].machine_cnt, body1.info_cnt);
    			return	(-1);
		}

		for( j=1; j<=body1.info_cnt; j++ )
		{
			//recv body2
			if( U_Nonb_recv(sock_fd, (char *)&body2, sizeof(struct body2_SW_MOBSERV_INFO_RESULT), 3) )
			{
				U_Printf("[DB]Error U_Nonb_recv()\n");
    				return	(-1);
			}
			
			//check ip
			if( (body2.ip[0] == 0) || (body2.ip[SIZEOF_IP] != 0) )
			{
				body2.ip[SIZEOF_IP]	= 0;
				U_Printf("[DB]Error world(%d) machine(%d) bad ip(%s)\n", i, j, body2.ip);
		    		return	(-1);
			}
			
			//set D_world.machine.mobserv_ip
			strncpy(D_world[i].machine[j].mobserv_ip, body2.ip, SIZEOF_IP+1);
		}
	}

	U_Printf("[DB]complete machine mobserv_ip\n");
	return	(0);
}


static	int	D_set_map(const int sock_fd)
{
	int					i, n, x, z, attr;
	char					buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	struct	body1_SW_MAP_FILES_RESULT	body1;
	struct	body2_SW_MAP_FILES_RESULT	body2;	
	char					map_file[40 + MAP_X_TILE*MAP_Z_TILE];


	//send WS_MAP_FILES
	n	= MK_GetPacket_WS_MAP_FILES(buf);
	
	if( U_Nonb_send(sock_fd, buf, n, 3) )
	{
		U_Printf("[DB]Error U_Nonb_send()\n");
    		return	(-1);
	}

	//recv header
	if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}
 
	//recv body1	
	if( U_Nonb_recv(sock_fd, (char *)&body1, sizeof(struct body1_SW_MAP_FILES_RESULT), 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}

	//check file_cnt
	if( (body1.file_cnt < 1) || (body1.file_cnt > MAX_SCRIPT_FILE_CNT)  )
	{
		U_Printf("[DB]Error bad file_cnt(%d)\n", body1.file_cnt);
    		return	(-1);
	}

	//alloc D_map
	if( (D_map = calloc(body1.file_cnt+1, sizeof(struct map))) == NULL )
	{
		U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    		return	(-1);
	}

	//load D_map
	for( i=1; i<=body1.file_cnt; i++ )
	{
		//recv file_size
		if( U_Nonb_recv(sock_fd, (char *)(&body2), sizeof(struct body2_SW_MAP_FILES_RESULT), 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
    			return	(-1);
		}

		//check file_size
		if( body2.file_size != sizeof(map_file)  )
		{
			U_Printf("[DB]Error bad body2.file_size(%d)\n", body2.file_size);
    			return	(-1);
		}
	
		//recv file 
		if( U_Nonb_recv(sock_fd, map_file, body2.file_size, 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
    			return	(-1);
		}

		//set attr
		attr	= 40;
	
		for( z=0; z<MAP_Z_TILE; z++ )
			for( x=0; x<MAP_X_TILE; x++ )
				D_map[i].attr[x][z]	= map_file[attr++]; 
	}
	
	//set D_max_map
	D_max_map	= body1.file_cnt;
	
	U_Printf("[DB]complete D_max_map(%d)\n", D_max_map);
	return	(0);
}


static	int	D_set_level(const int sock_fd)
{
	int					n, i, j, line, row, column;
	char					buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	char					*row_ptr, *column_ptr, *file[MAX_SCRIPT_FILE_CNT+1];
	struct	body1_SW_LEVEL_FILES_RESULT	body1;
	struct	body2_SW_LEVEL_FILES_RESULT	body2;


	//send WS_LEVEL_FILES
	n	= MK_GetPacket_WS_LEVEL_FILES(buf);

	if( U_Nonb_send(sock_fd, buf, n, 3) )
	{
		U_Printf("[DB]Error U_Nonb_send()\n");
    		return	(-1);
	}

	//recv header
	if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}
 
	//recv body1	
	if( U_Nonb_recv(sock_fd, (char *)&body1, sizeof(struct body1_SW_LEVEL_FILES_RESULT), 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}

	//check file_cnt
	if( (body1.file_cnt < 1) || (body1.file_cnt > MAX_SCRIPT_FILE_CNT) )
	{
		U_Printf("[DB]Error bad file_cnt(%d)\n", body1.file_cnt);
    		return	(-1);
	}

	//recv files
	for( i=1; i<=body1.file_cnt; i++ )
	{
		//recv file_size
		if( U_Nonb_recv(sock_fd, (char *)(&body2), sizeof(struct body2_SW_LEVEL_FILES_RESULT), 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
   	 		return	(-1);
		}

		//check file_size
		if( body2.file_size < 1 )
		{
			U_Printf("[DB]Error bad file_size(%d)\n", body2.file_size);
    			return	(-1);
		}
		
		//alloc file
		if( (file[i] = calloc(1, body2.file_size)) == NULL )
		{
			U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    			return	(-1);
		}
		
		//recv file 
		if( U_Nonb_recv(sock_fd, file[i], body2.file_size, 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
   	 		return	(-1);
		}

		//count line
		line	= 0;

		for( j=0; j<body2.file_size; j++ )
		{
			if( file[i][j] == 0 )
			{
				U_Printf("[DB]Error file(%d) bad data\n", i);
    				return	(-1);
			}			

			if( file[i][j] == '\n' )
				++line;
		}

		if( file[i][body2.file_size-1] == '\n' )
		{
			--line;
			file[i][body2.file_size-1] = 0; 
		}
	
		//check line
		if( line < 1 )
		{
			U_Printf("[DB]Error file(%d) bad line(%d)\n", i, line);
    			return	(-1);
		}

		//set D_max_level
		D_max_level	+= line;
	}
		
	//alloc D_level
	if( (D_level = calloc(D_max_level+1, sizeof(struct level))) == NULL )
	{
		U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    		return	(-1);
	}

	//load D_level
	row	= 0;

	for( i=1; i<=body1.file_cnt; i++ )
	{
		//first line
		row_ptr	= strtok(file[i], "\n");

		//other line
		line	= 0;
		
		while( (row_ptr = strtok(NULL, "\n")) )
		{
			++row;
			++line;
			column	= 0;
		
			//set column
			while( (column_ptr = strsep(&row_ptr, "\t")) )
			{
				++column;

				switch( column ) 
				{
					case	1:	
						if( (D_level[row].index = strtol(column_ptr, NULL, 10)) != row )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad index(%d)\n", i, line, D_level[row].index);
							return	(-1);
						}
						break;
					case	2:
						if( (D_level[row].hp_max = strtol(column_ptr, NULL, 10)) < 1 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad hp_max(%d)\n", i, line, D_level[row].hp_max);
							return	(-1);
						}
						break;
					case	3:
						if( (D_level[row].ap_max = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad ap_max(%d)\n", i, line, D_level[row].ap_max);
							return	(-1);
						}
						break;
					case	4:
						if( (D_level[row].exp = strtol(column_ptr, NULL, 10)) < 1 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad exp(%d)\n", i, line, D_level[row].exp);
							return	(-1);
						}
						break;
					case	5:
						if( (D_level[row].att1 = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad att1(%d)\n", i, line, D_level[row].att1);
							return	(-1);
						}
						break;
					case	6:
						if( (D_level[row].def1 = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad def1(%d)\n", i, line, D_level[row].def1);
							return	(-1);
						}
						break;
					case	7:
						if( (D_level[row].is_def = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad is_def(%d)\n", i, line, D_level[row].is_def);
							return	(-1);
						}
						break;
					case	8:
						if( (D_level[row].il_def = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad il_def(%d)\n", i, line, D_level[row].il_def);
							return	(-1);
						}
						break;
					case	9:
						if( (D_level[row].i_att = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad i_att(%d)\n", i, line, D_level[row].i_att);
							return	(-1);
						}
						break;
					case	10:
						if( (D_level[row].m_def = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad m_def(%d)\n", i, line, D_level[row].m_def);
							return	(-1);
						}
						break;
					case	11:
						if( (D_level[row].cri = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad cri(%d)\n", i, line, D_level[row].cri);
							return	(-1);
						}
						break;
					case	12:
						if( (D_level[row].move_speed1 = strtof(column_ptr, NULL)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad move_speed1(%f)\n",
													i, line, D_level[row].move_speed1);
							return	(-1);
						}
						break;
					case	13:
						if( (D_level[row].move_speed2 = strtof(column_ptr, NULL)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad move_speed2(%f)\n",
													i, line, D_level[row].move_speed2);
							return	(-1);
						}
						break;
					case	14:
						if( (D_level[row].recover_hp = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad recover_hp(%d)\n",
									i, line, D_level[row].recover_hp);
							return	(-1);
						}
						break;
					case	15:
						if( (D_level[row].recover_ap = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad recover_ap(%d)\n",
									i, line, D_level[row].recover_ap);
							return	(-1);
						}
						break;
					default	:	
						U_Printf("[DB]Error file(%d) line(%d) bad column(%d)\n", i, line, column);	
						return	(-1);
				}
			}

			//check column
			if( column != 15 )
			{
				U_Printf("[DB]Error file(%d) line(%d) bad column(%d)\n", i, line, column);
				return	(-1);
			}
		
		}//end while
	}//end for

	//free files
	for( i=1; i<=body1.file_cnt; i++ )
		free(file[i]);

	U_Printf("[DB]complete D_max_level(%d)\n", D_max_level);
	return	(0);
}


static	int	D_set_type_store(const int sock_fd)
{
	int						n, i, j, line, idx, row, column;
	char						buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	char						*row_ptr, *column_ptr, *file[MAX_SCRIPT_FILE_CNT+1];
	struct	body1_SW_TYPE_STORE_FILES_RESULT	body1;
	struct	body2_SW_TYPE_STORE_FILES_RESULT	body2;
	

	//send WS_TYPE_STORE_FILES
	n	= MK_GetPacket_WS_TYPE_STORE_FILES(buf);

	if( U_Nonb_send(sock_fd, buf, n, 3) )
	{
		U_Printf("[DB]Error U_Nonb_send()\n");
    		return	(-1);
	}

	//recv header
	if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}
 
	//recv body1	
	if( U_Nonb_recv(sock_fd, (char *)&body1, sizeof(struct body1_SW_TYPE_STORE_FILES_RESULT), 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}

	//check file_cnt
	if( (body1.file_cnt < 1) || (body1.file_cnt > MAX_SCRIPT_FILE_CNT) )
	{
		U_Printf("[DB]Error bad file_cnt(%d)\n", body1.file_cnt);
    		return	(-1);
	}

	//recv files
	for( i=1; i<=body1.file_cnt; i++ )
	{
		//recv file_size
		if( U_Nonb_recv(sock_fd, (char *)(&body2), sizeof(struct body2_SW_TYPE_STORE_FILES_RESULT), 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
   	 		return	(-1);
		}

		//check file_size
		if( body2.file_size < 1 )
		{
			U_Printf("[DB]Error bad file_size(%d)\n", body2.file_size);
    			return	(-1);
		}
		
		//alloc file
		if( (file[i] = calloc(1, body2.file_size)) == NULL )
		{
			U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    			return	(-1);
		}
		
		//recv file 
		if( U_Nonb_recv(sock_fd, file[i], body2.file_size, 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
   	 		return	(-1);
		}

		//count line
		line	= 0;

		for( j=0; j<body2.file_size; j++ )
		{
			if( file[i][j] == 0 )
			{
				U_Printf("[DB]Error file(%d) bad data\n", i);
    				return	(-1);
			}			
			
			if( file[i][j] == '\n' )
				++line;
		}

		if( file[i][body2.file_size-1] == '\n' )
		{
			--line;
			file[i][body2.file_size-1] = 0; 
		}
	
		//check line
		if( line < 1 )
		{
			U_Printf("[DB]Error file(%d) bad line(%d)\n", i, line);
    			return	(-1);
		}

		//set D_max_type_store
		D_max_type_store	+= line;
	}

	//alloc D_type_store
	if( (D_type_store = calloc(D_max_type_store+1, sizeof(struct type_store))) == NULL )
	{
		U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    		return	(-1);
	}

	//load D_type_store
	row	= 0;
	
	for( i=1; i<=body1.file_cnt; i++ )
	{
		//first line
		row_ptr	= strtok(file[i], "\n");
		
		//other line
		line	= 0;
		
		while( (row_ptr = strtok(NULL, "\n")) )
		{
			++row;
			++line;
			column	= 0;
			
			//set column
			while( (column_ptr = strsep(&row_ptr, "\t")) )
			{
				++column;

				if( (column >= 5) && (column <= 124) )
				{
					idx	= column-5;
					if( ((D_type_store[row].item_type[idx] = strtol(column_ptr, NULL, 10)) < 0)
									|| (D_type_store[row].item_type[idx] > D_max_type_item) )
					{
						U_Printf("[DB]Error file(%d) line(%d) bad item_type[%d](%d)\n",
										i, line, idx, D_type_store[row].item_type[idx]);
						return	(-1);
					}
					continue;
				}

				switch( column ) 
				{
					case	1:	
						if( (D_type_store[row].index = strtol(column_ptr, NULL, 10)) != row )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad index(%d)\n", i, line, D_type_store[row].index);
							return	(-1);
						}
						break;
					case	2:
						break;
					case	3:
						D_type_store[row].repair		= strtol(column_ptr, NULL, 10); 
						break;
					case	4:
						break;
					default	:	
						U_Printf("[DB]Error file(%d) line(%d) bad column(%d)\n", i, line, column);	
						return	(-1);
				}
			}

			//check column
			if( column != 124 )
			{
				U_Printf("[DB]Error file(%d) line(%d) bad column(%d)\n", i, line, column);
				return	(-1);
			}
		
		}//end while
	}//end for

	//free files
	for( i=1; i<=body1.file_cnt; i++ )
		free(file[i]);

	U_Printf("[DB]complete D_max_type_store(%d)\n", D_max_type_store);
	return	(0);
}


static	int	D_set_type_warp(const int sock_fd)
{
	int					n, i, j, line, row, column;
	char					buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	char					*row_ptr, *column_ptr, *file[MAX_SCRIPT_FILE_CNT+1];
	struct	body1_SW_TYPE_WARP_FILES_RESULT	body1;
	struct	body2_SW_TYPE_WARP_FILES_RESULT	body2;
	

	//send WS_TYPE_WARP_FILES
	n	= MK_GetPacket_WS_TYPE_WARP_FILES(buf);

	if( U_Nonb_send(sock_fd, buf, n, 3) )
	{
		U_Printf("[DB]Error U_Nonb_send()\n");
    		return	(-1);
	}

	//recv header
	if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}
 
	//recv body1	
	if( U_Nonb_recv(sock_fd, (char *)&body1, sizeof(struct body1_SW_TYPE_WARP_FILES_RESULT), 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}

	//check file_cnt
	if( (body1.file_cnt < 1) || (body1.file_cnt > MAX_SCRIPT_FILE_CNT) )
	{
		U_Printf("[DB]Error bad file_cnt(%d)\n", body1.file_cnt);
    		return	(-1);
	}

	//recv files
	for( i=1; i<=body1.file_cnt; i++ )
	{
		//recv file_size
		if( U_Nonb_recv(sock_fd, (char *)(&body2), sizeof(struct body2_SW_TYPE_WARP_FILES_RESULT), 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
   	 		return	(-1);
		}

		//check file_size
		if( body2.file_size < 1 )
		{
			U_Printf("[DB]Error bad file_size(%d)\n", body2.file_size);
    			return	(-1);
		}
		
		//alloc file
		if( (file[i] = calloc(1, body2.file_size)) == NULL )
		{
			U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    			return	(-1);
		}
		
		//recv file 
		if( U_Nonb_recv(sock_fd, file[i], body2.file_size, 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
   	 		return	(-1);
		}

		//count line
		line	= 0;

		for( j=0; j<body2.file_size; j++ )
		{
			if( file[i][j] == 0 )
			{
				U_Printf("[DB]Error file(%d) bad data\n", i);
    				return	(-1);
			}			
		
			if( file[i][j] == '\n' )
				++line;
		}


		if( file[i][body2.file_size-1] == '\n' )
		{
			--line;
			file[i][body2.file_size-1] = 0; 
		}
	
		//check line
		if( line < 1 )
		{
			U_Printf("[DB]Error file(%d) bad line(%d)\n", i, line);
    			return	(-1);
		}

		//set D_max_type_warp
		D_max_type_warp	+= line;
	}

	//alloc D_type_warp
	if( (D_type_warp = calloc(D_max_type_warp+1, sizeof(struct type_warp))) == NULL )
	{
		U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    		return	(-1);
	}

	//load D_type_warp
	row	= 0;

	for( i=1; i<=body1.file_cnt; i++ )
	{
		//first line
		row_ptr	= strtok(file[i], "\n");
		
		//other line
		line	= 0;
		
		while( (row_ptr = strtok(NULL, "\n")) )
		{
			++row;
			++line;
			column	= 0;
			
			//set column
			while( (column_ptr = strsep(&row_ptr, "\t")) )
			{
				++column;

				switch( column ) 
				{
					case	1:	
						if( (D_type_warp[row].index = strtol(column_ptr, NULL, 10)) != row )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad index(%d)\n", i, line, D_type_warp[row].index);
							return	(-1);
						}
						break;
					case	2:	
						break;
					case	3:
						if( (D_type_warp[row].req_quest = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad req_quest(%d)\n",
											i, line, D_type_warp[row].req_quest);
							return	(-1);
						}
						break;
					case	4:
						if( ((D_type_warp[row].req_level = strtol(column_ptr, NULL, 10)) < 0)
										|| (D_type_warp[row].req_level > D_max_level)  )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad req_level(%d)\n",
											i, line, D_type_warp[row].req_level);
							return	(-1);
						}
						break;
					case	5:
						if( ((D_type_warp[row].req_item = strtol(column_ptr, NULL, 10)) < 0)
										|| (D_type_warp[row].req_item > D_max_type_item)  )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad req_item(%d)\n",
											i, line, D_type_warp[row].req_item);
							return	(-1);
						}
						break;
					case	6:
					case	7:
						break;
					case	8:
						if( ((D_type_warp[row].dest_map = strtol(column_ptr, NULL, 10)) < 1)
										|| (D_type_warp[row].dest_map > D_max_map)  )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad dest_map(%d)\n",
											i, line, D_type_warp[row].dest_map);
							return	(-1);
						}
						break;
					case	9:
						if( ((D_type_warp[row].dest_x = strtol(column_ptr, NULL, 10)) < 1)
										|| (D_type_warp[row].dest_x > (MAP_X_TILE-2)) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad dest_x(%d)\n",
											i, line, column, D_type_warp[row].dest_x);
							return	(-1);
						}
						break;
					case	10:	
						if( ((D_type_warp[row].dest_z = strtol(column_ptr, NULL, 10)) < 1)
										|| (D_type_warp[row].dest_z > (MAP_Z_TILE-2)) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad dest_z(%d)\n",
											i, line, column, D_type_warp[row].dest_z);
							return	(-1);
						}
						break;
					case	11:
					case	12:
					case	13:
					case	14:
						break;
					default	:	
						U_Printf("[DB]Error file(%d) line(%d) bad column(%d)\n", i, line, column);	
						return	(-1);
				}
			}

			//check column
			if( column != 14 )
			{
				U_Printf("[DB]Error file(%d) line(%d) bad column(%d)\n", i, line, column);
				return	(-1);
			}

			//check attr
			if( D_map[D_type_warp[row].dest_map].attr[D_type_warp[row].dest_x][D_type_warp[row].dest_z] == MAP_ATTR_COLLISION )
			{
				U_Printf("[DB]Error file(%d) line(%d) dest_map(%d) dest_x(%d), dest_z(%d) bad attr(%d)\n",
						i, line, D_type_warp[row].dest_map, D_type_warp[row].dest_x, D_type_warp[row].dest_z,
						D_map[D_type_warp[row].dest_map].attr[D_type_warp[row].dest_x][D_type_warp[row].dest_z]);
				return	(-1);
			}
		
		}//end while
	}//end for

	//free files
	for( i=1; i<=body1.file_cnt; i++ )
		free(file[i]);

	U_Printf("[DB]complete D_max_type_warp(%d)\n", D_max_type_warp);
	return	(0);
}


static	int	D_set_type_mob(const int sock_fd)
{
	int					n, i, j, line, row, column;
	char					buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	char					*row_ptr, *column_ptr, *file[MAX_SCRIPT_FILE_CNT+1];
	struct	body1_SW_TYPE_MOB_FILES_RESULT	body1;
	struct	body2_SW_TYPE_MOB_FILES_RESULT	body2;


	//send WS_TYPE_MOB_FILES
	n	= MK_GetPacket_WS_TYPE_MOB_FILES(buf);

	if( U_Nonb_send(sock_fd, buf, n, 3) )
	{
		U_Printf("[DB]Error U_Nonb_send()\n");
    		return	(-1);
	}

	//recv header
	if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}
 
	//recv body1	
	if( U_Nonb_recv(sock_fd, (char *)&body1, sizeof(struct body1_SW_TYPE_MOB_FILES_RESULT), 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}

	//check file_cnt
	if( (body1.file_cnt < 1) || (body1.file_cnt > MAX_SCRIPT_FILE_CNT) )
	{
		U_Printf("[DB]Error bad file_cnt(%d)\n", body1.file_cnt);
    		return	(-1);
	}

	//recv files
	for( i=1; i<=body1.file_cnt; i++ )
	{
		//recv file_size
		if( U_Nonb_recv(sock_fd, (char *)(&body2), sizeof(struct body2_SW_TYPE_MOB_FILES_RESULT), 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
   	 		return	(-1);
		}

		//check file_size
		if( body2.file_size < 1 )
		{
			U_Printf("[DB]Error bad file_size(%d)\n", body2.file_size);
    			return	(-1);
		}
		
		//alloc file
		if( (file[i] = calloc(1, body2.file_size)) == NULL )
		{
			U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    			return	(-1);
		}
		
		//recv file 
		if( U_Nonb_recv(sock_fd, file[i], body2.file_size, 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
   	 		return	(-1);
		}

		//count line
		line	= 0;

		for( j=0; j<body2.file_size; j++ )
		{	
			if( file[i][j] == 0 )
			{
				U_Printf("[DB]Error file(%d) bad data\n", i);
    				return	(-1);
			}
				
			if( file[i][j] == '\n' )
				++line;
		}

		if( file[i][body2.file_size-1] == '\n' )
		{
			--line;
			file[i][body2.file_size-1] = 0; 
		}
	
		//check line
		if( line < 1 )
		{
			U_Printf("[DB]Error file(%d) bad line(%d)\n", i, line);
    			return	(-1);
		}

		//set D_max_type_mob
		D_max_type_mob	+= line;
	}
		
	//alloc D_type_mob
	if( (D_type_mob = calloc(D_max_type_mob+1, sizeof(struct type_mob))) == NULL )
	{
		U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    		return	(-1);
	}

	//load D_type_mob
	row	= 0;

	for( i=1; i<=body1.file_cnt; i++ )
	{
		//first line
		row_ptr	= strtok(file[i], "\n");
		
		//other line
		line	= 0;
				
		
		while( (row_ptr = strtok(NULL, "\n")) )
		{
			++row;
			++line;
			column	= 0;
		
			//set column
			while( (column_ptr = strsep(&row_ptr, "\t")) )
			{
				++column;

				switch( column ) 
				{
					case	1:	
						if( (D_type_mob[row].index = strtol(column_ptr, NULL, 10)) != row )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad index(%d)\n", i, line, D_type_mob[row].index);
							return	(-1);
						}
						break;
					case	2:	
						break;
					case	3:
						if( (D_type_mob[row].level = strtol(column_ptr, NULL, 10)) < 1 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad level(%d)\n", i, line, D_type_mob[row].level);
							return	(-1);
						}
						break;
					case	4:
						if( (D_type_mob[row].hp_max = strtol(column_ptr, NULL, 10)) < 1 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad hp_max(%d)\n", i, line, D_type_mob[row].hp_max);
							return	(-1);
						}
						break;
					case	5:
						if( (D_type_mob[row].ap_max = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad ap_max(%d)\n", i, line, D_type_mob[row].ap_max);
							return	(-1);
						}
						break;
					case	6:
						if( (D_type_mob[row].exp = strtol(column_ptr, NULL, 10)) < 0  )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad exp(%d)\n", i, line, D_type_mob[row].exp);
							return	(-1);
						}
						break;
					case	7:
						if( (D_type_mob[row].att1 = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad att1(%d)\n", i, line, D_type_mob[row].att1);
							return	(-1);
						}
						break;
					case	8:
						if( (D_type_mob[row].def1 = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad def1(%d)\n", i, line, D_type_mob[row].def1);
							return	(-1);
						}
						break;
					case	9:
						if( (D_type_mob[row].att2 = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad att2(%d)\n", i, line, D_type_mob[row].att2);
							return	(-1);
						}
						break;
					case	10:
						if( (D_type_mob[row].def2 = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad def2(%d)\n", i, line, D_type_mob[row].def2);
							return	(-1);
						}
						break;
					case	11:
						if( (D_type_mob[row].eva = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad eva(%d)\n", i, line, D_type_mob[row].eva);
							return	(-1);
						}
						break;
					case	12:
						if( (D_type_mob[row].is_def = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad is_def(%d)\n", i, line, D_type_mob[row].is_def);
							return	(-1);
						}
						break;
					case	13:
						if( (D_type_mob[row].il_def = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad il_def(%d)\n", i, line, D_type_mob[row].il_def);
							return	(-1);
						}
						break;
					case	14:
						if( (D_type_mob[row].i_att = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad i_att(%d)\n", i, line, D_type_mob[row].i_att);
							return	(-1);
						}
						break;
					case	15:
						if( (D_type_mob[row].m_def = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad m_def(%d)\n", i, line, D_type_mob[row].m_def);
							return	(-1);
						}
						break;
					case	16:
						if( (D_type_mob[row].cri = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad cri(%d)\n", i, line, D_type_mob[row].cri);
							return	(-1);
						}
						break;
					case	17:
						if( (D_type_mob[row].recover_hp = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad recover_hp(%d)\n",
									i, line, D_type_mob[row].recover_hp);
							return	(-1);
						}
						break;
					case	18:
						if( (D_type_mob[row].recover_ap = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad recover_ap(%d)\n",
									i, line, D_type_mob[row].recover_ap);
							return	(-1);
						}
						break;
					case	19:
						if( (D_type_mob[row].move_speed1 = strtof(column_ptr, NULL)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad move_speed1(%f)\n",
											i, line, D_type_mob[row].move_speed1);
							return	(-1);
						}
						break;
					case	20:
						if( (D_type_mob[row].move_speed2 = strtof(column_ptr, NULL)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad move_speed2(%f)\n",
											i, line, D_type_mob[row].move_speed2);
							return	(-1);
						}
						break;
					case	21:
						if( (D_type_mob[row].move_speed3 = strtof(column_ptr, NULL)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad move_speed3(%f)\n",
											i, line, D_type_mob[row].move_speed3);
							return	(-1);
						}
						break;
					case	22:
						if( (D_type_mob[row].milli_delay_attack = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad milli_delay_attack(%d)\n",
									i, line, D_type_mob[row].milli_delay_attack);
							return	(-1);
						}
						break;
					case	23:
						if( (D_type_mob[row].att_area = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad att_area(%d)\n",
											i, line, D_type_mob[row].att_area);
							return	(-1);
						}
						break;
					case	24:
						if( (D_type_mob[row].def_intensity = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad def_intensity(%d)\n",
									i, line, D_type_mob[row].def_intensity);
							return	(-1);
						}
						break;
					case	25:
						if( (D_type_mob[row].att_intensity = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad att_intensity(%d)\n",
									i, line, D_type_mob[row].att_intensity);
							return	(-1);
						}
						break;
					case	26:
					case	27:
					case	28:
					case	29:
					case	30:
					case	31:
					case	32:
					case	33:
					case	34:
					case	35:
					case	36:
					case	37:
					case	38:
					case	39:
					case	40:
					case	41:
					case	42:
					case	43:
						break;
					case	44:
						if( ((D_type_mob[row].elements = strtol(column_ptr, NULL, 10)) < 0)
							|| (D_type_mob[row].elements > 4))
						{
							U_Printf("[DB]Error file(%d) line(%d) bad elements(%d)\n",
											i, line, D_type_mob[row].elements);
							return	(-1);
						}
						break;
					case	45:
					case	46:
					case	47:
					case	48:
						break;
					case	49:
						if( ((D_type_mob[row].drop_type = strtol(column_ptr, NULL, 10)) < 0) 
							|| (D_type_mob[row].drop_type > D_max_type_drop) )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad drop_type(%d)\n",
										i, line, D_type_mob[row].drop_type);
							return	(-1);
						}
						break;
					case	50:
						if( (D_type_mob[row].money_drop_prob = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad money_drop_prob(%d)\n",
										i, line, D_type_mob[row].money_drop_prob);
							return	(-1);
						}
						break;
					case	51:
						if( ((D_type_mob[row].money_min = strtol(column_ptr, NULL, 10)) < 0) 
							|| (D_type_mob[row].money_min > D_max_ushort) )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad money_min(%d)\n",
										i, line, D_type_mob[row].money_min);
							return	(-1);
						}
						break;
					case	52:
						if( ((D_type_mob[row].money_max = strtol(column_ptr, NULL, 10)) < D_type_mob[row].money_min) 
							|| (D_type_mob[row].money_max > D_max_ushort) )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad money_max(%d)\n",
										i, line, D_type_mob[row].money_max);
							return	(-1);
						}
						break;
					case	53:
						if( (D_type_mob[row].quest_item = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad quest_item(%d)\n",
										i, line, D_type_mob[row].quest_item);
							return	(-1);
						}
						break;
					case	54:
						if( (D_type_mob[row].quest = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad quest(%d)\n", i, line, D_type_mob[row].quest);
							return	(-1);
						}
						break;
					case	55:
						if( (D_type_mob[row].quest_item_drop_prob = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad quest_item_drop_prob(%d)\n",
									i, line, D_type_mob[row].quest_item_drop_prob);
							return	(-1);
						}
						break;
					case	56:
						if( ((D_type_mob[row].npc_type = strtol(column_ptr, NULL, 10)) < 0) 
							|| (D_type_mob[row].npc_type > 3) )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad npc_type(%d)\n",
											i, line, D_type_mob[row].npc_type);
							return	(-1);
						}
						break;
					case	57:
						break;
					case	58:
						if( (D_type_mob[row].mastery = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad mastery(%d)\n", i, line, D_type_mob[row].mastery);
							return	(-1);
						}
						break;
					case	59:
						break;
					default	:	
						U_Printf("[DB]Error file(%d) line(%d) bad column(%d)\n", i, line, column);	
						return	(-1);
				}
			}

			//check column
			if( column != 59 )
			{
				U_Printf("[DB]Error file(%d) line(%d) bad column(%d)\n", i, line, column);
				return	(-1);
			}
		
		}//end while
	}//end for

	//free files
	for( i=1; i<=body1.file_cnt; i++ )
		free(file[i]);

	U_Printf("[DB]complete D_max_type_mob(%d)\n", D_max_type_mob);
	return	(0);
}


static	int	D_set_mob(const int sock_fd)
{
	int					n, i, j, line, column, first_line, second_line, file_mob_cnt;
	int					mob_type, mob_cnt, dir, x, z, table_idx;
	char					buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	char					*row_ptr, *column_ptr, *file[MAX_SCRIPT_FILE_CNT+1];
	struct	body1_SW_MOB_FILES_RESULT	body1;
	struct	body2_SW_MOB_FILES_RESULT	body2;


	//send WS_MOB_FILES
	n	= MK_GetPacket_WS_MOB_FILES(buf);

	if( U_Nonb_send(sock_fd, buf, n, 3) )
	{
		U_Printf("[DB]Error U_Nonb_send()\n");
    		return	(-1);
	}

	//recv header
	if( U_Nonb_recv(sock_fd, (char *)buf, SIZEOF_HEADER_SPECIAL, 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}
 
	//recv body1	
	if( U_Nonb_recv(sock_fd, (char *)&body1, sizeof(struct body1_SW_MOB_FILES_RESULT), 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}

	//check file_cnt
	if( (body1.file_cnt < 1) || (body1.file_cnt > MAX_SCRIPT_FILE_CNT) || (body1.file_cnt != D_max_map) )
	{
		U_Printf("[DB]Error bad file_cnt(%d)\n", body1.file_cnt);
    		return	(-1);
	}

	//recv files	
	for( i=1; i<=body1.file_cnt; i++ )
	{
		//recv file_size
		if( U_Nonb_recv(sock_fd, (char *)(&body2), sizeof(struct body2_SW_MOB_FILES_RESULT), 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
    			return	(-1);
		}

		//check file_size
		if( body2.file_size < 1 )
		{
			U_Printf("[DB]Error bad file_size(%d)\n", body2.file_size);
    			return	(-1);
		}
		
		//alloc file
		if( (file[i] = calloc(1, body2.file_size)) == NULL )
		{
			U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    			return	(-1);
		}
		
		//recv file 
		if( U_Nonb_recv(sock_fd, file[i], body2.file_size, 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
    			return	(-1);
		}

		//count line
		line	= 0;

		for( j=0; j<body2.file_size; j++ )
		{	
			if( file[i][j] == 0 )
			{
				U_Printf("[DB]Error file(%d) bad data\n", i);
    				return	(-1);
			}
	
			if( file[i][j] == '\n' )
				++line;
		}

		if( file[i][body2.file_size-1] == '\n' )
			file[i][body2.file_size-1] = 0; 
		else
			++line;
			
		//check line
		if( line < 2 )
		{
			U_Printf("[DB]Error file(%d) bad line(%d)\n", i, line);
    			return	(-1);
		}
	}
	
	//check file
	for( i=1; i<=body1.file_cnt; i++ )
	{
		//first line
		row_ptr	= strtok(file[i], "\n");

		if( (first_line = strtol(row_ptr, NULL, 10)) < 0 )
		{
			U_Printf("[DB]Error file(%d) bad frist_line(%d) \n", i, first_line);
			return	(-1);
		}	

		//second line
		row_ptr = strtok(NULL, "\n");
		
		if( (second_line = strtol(row_ptr, NULL, 10)) < 0 )
		{
			U_Printf("[DB]Error file(%d) bad second_line(%d) \n", i, second_line);
			return	(-1);
		}	
				
		//other line
		line		= 0;
		file_mob_cnt	= 0;
	
		while( (row_ptr = strtok(NULL, "\n")) )
		{
			++line;
			column		= 0;
			mob_type	= 0;
			mob_cnt		= 0;
		
			while( (column_ptr = strsep(&row_ptr, "\t")) )
			{
				++column;

				if( column == 1 )
				{
					if( ((mob_type = strtol(column_ptr, NULL, 10)) < 1) || (mob_type > D_max_type_mob) )
					{
						U_Printf("[DB]Error file(%d) line(%d) bad type(%d)\n", i, line, mob_type);
						return	(-1);
					}
					continue;
				}

				if( column == 2 )
				{
					if( (mob_cnt = strtol(column_ptr, NULL, 10)) < 1 )
					{
						U_Printf("[DB]Error file(%d) line(%d) bad mob_cnt(%d) \n", i, line, mob_cnt);
						return	(-1);
					}
					continue;
				}

				switch( ((column - 3) % 4) ) 
				{
					case	0:	
						if( ((x = strtol(column_ptr, NULL, 10)) < 1) || (x > (MAP_X_TILE-2)) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad x(%d)\n", i, line, column, x);
							return	(-1);
						}
						break;
					case	1:	
						if( ((z = strtol(column_ptr, NULL, 10)) < 1) || (z > (MAP_Z_TILE-2)) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad z(%d)\n", i, line, column, z);
							return	(-1);
						}
						break;
					case	2:
						if( ((dir = strtol(column_ptr, NULL, 10)) < 0) || (dir > 359) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad dir(%d)\n", i, line, column, dir);
							return	(-1);
						}
						break;
					case	3:	
						table_idx	= strtol(column_ptr, NULL, 10);
						
						if( (D_type_mob[mob_type].npc_type == NPC_TYPE_STORE) 
							&& ((table_idx < 1) || (table_idx > D_max_type_store)) )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad npc_type(%d) table_idx(%d) D_max_type_store(%d)\n",
								i, line, D_type_mob[mob_type].npc_type, table_idx, D_max_type_store);
							return	(-1);
						}

						if( (D_type_mob[mob_type].npc_type == NPC_TYPE_WARP) 
							&& ((table_idx < 1) || (table_idx > D_max_type_warp)) )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad npc_type(%d) table_idx(%d) D_max_type_warp(%d)\n",
								i, line, D_type_mob[mob_type].npc_type, table_idx, D_max_type_warp);
							return	(-1);
						}
						break;
				}//end switch 
			}//end while
				
			//check column
			if( column != (2 + mob_cnt*4) )
			{
				U_Printf("[DB]Error file(%d) line(%d) bad column(%d) mob_cnt(%d)\n", i, line, column, mob_cnt);
				return	(-1);
			}

			file_mob_cnt	+= mob_cnt;

		}//end while

		//check first_line
		if( first_line != file_mob_cnt ) 
		{
			U_Printf("[DB]Error file(%d) first_line(%d) bad file_mob_cnt(%d) \n", i, first_line, file_mob_cnt);
			return	(-1);
		}
		
		//check second_line
		if( second_line != line ) 
		{
			U_Printf("[DB]Error file(%d) second_line(%d) bad line(%d) \n", i, second_line, line);
			return	(-1);
		}
		
		//set D_max_mob
		D_max_mob	+= file_mob_cnt;

	}//end for

	//set D_max_mob
	D_max_mob	+= D_max_map*MAP_SUMMON_MOB_CNT;
	
	//alloc D_mob
	if( (D_mob = calloc((D_max_mob+1)*(CHANNEL_PER_MACHINE+1), sizeof(struct mob))) == NULL )
	{
		U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    		return	(-1);
	}
	
	//set D_mob.status
	for( i=0; i<=D_max_mob; i++ )
		for( j=0; j<CHANNEL_PER_MACHINE+1; j++ )
			D_mob[i][j].status	= _MOB_EXTINCT; 

	//free files
	for( i=1; i<=body1.file_cnt; i++ )
		free(file[i]);

	U_Printf("[DB]complete D_max_mob(%d)(%d)\n", D_max_mob, CHANNEL_PER_MACHINE+1);
	return	(0);
}


static	int	D_set_type_item(const int sock_fd)
{
	int					n, i, j, line, row, column, idx;
	char					buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	char					*row_ptr, *column_ptr, *file[MAX_SCRIPT_FILE_CNT+1];
	struct	body1_SW_TYPE_ITEM_FILES_RESULT	body1;
	struct	body2_SW_TYPE_ITEM_FILES_RESULT	body2;


	//send WS_TYPE_ITEM_FILES
	n	= MK_GetPacket_WS_TYPE_ITEM_FILES(buf);

	if( U_Nonb_send(sock_fd, buf, n, 3) )
	{
		U_Printf("[DB]Error U_Nonb_send()\n");
    		return	(-1);
	}

	//recv header
	if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}
 
	//recv body1	
	if( U_Nonb_recv(sock_fd, (char *)&body1, sizeof(struct body1_SW_TYPE_ITEM_FILES_RESULT), 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}

	//check file_cnt
	if( (body1.file_cnt < 1) || (body1.file_cnt > MAX_SCRIPT_FILE_CNT) )
	{
		U_Printf("[DB]Error bad file_cnt(%d)\n", body1.file_cnt);
    		return	(-1);
	}

	//recv files
	for( i=1; i<=body1.file_cnt; i++ )
	{
		//recv file_size
		if( U_Nonb_recv(sock_fd, (char *)(&body2), sizeof(struct body2_SW_TYPE_ITEM_FILES_RESULT), 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
   	 		return	(-1);
		}

		//check file_size
		if( body2.file_size < 1 )
		{
			U_Printf("[DB]Error bad file_size(%d)\n", body2.file_size);
    			return	(-1);
		}
		
		//alloc file
		if( (file[i] = calloc(1, body2.file_size)) == NULL )
		{
			U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    			return	(-1);
		}
		

		//recv file 
		if( U_Nonb_recv(sock_fd, file[i], body2.file_size, 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
   	 		return	(-1);
		}

		//count line
		line	= 0;

		for( j=0; j<body2.file_size; j++ )
		{	
			if( file[i][j] == 0 )
			{
				U_Printf("[DB]Error file(%d) bad data\n", i);
    				return	(-1);
			}

			if( file[i][j] == '\n' )
				++line;
		}

		if( file[i][body2.file_size-1] == '\n' )
		{
			--line;
			file[i][body2.file_size-1] = 0; 
		}
	
		//check line
		if( line < 1 )
		{
			U_Printf("[DB]Error file(%d) bad line(%d)\n", i, line);
    			return	(-1);
		}

		//set D_max_type_item
		D_max_type_item	+= line;
	}
		
	//alloc D_type_item
	if( (D_type_item = calloc(D_max_type_item+1, sizeof(struct type_item))) == NULL )
	{
		U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    		return	(-1);
	}

	//load D_type_item
	row	= 0;

	for( i=1; i<=body1.file_cnt; i++ )
	{
		//first line
		row_ptr	= strtok(file[i], "\n");
		
		//other line
		line	= 0;
			
		while( (row_ptr = strtok(NULL, "\n")) )
		{
			++row;
			++line;
			column	= 0;
	
			//set column
			while( (column_ptr = strsep(&row_ptr, "\t")) )
			{
				++column;

				switch( column ) 
				{
					case	1:	
						if( (D_type_item[row].index = strtol(column_ptr, NULL, 10)) != row )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad index(%d)\n", i, line, D_type_item[row].index);
							return	(-1);
						}
						break;
					case	2:	
					case	3:	
						break;
					case	4:	
						if( ((D_type_item[row].class = strtol(column_ptr, NULL, 10)) < 1)
							|| (D_type_item[row].class > MAX_ITEM_CLASS) )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad class(%d)\n", i, line, D_type_item[row].class);
							return	(-1);
						}
						break;
					case	5:	
						if( ((D_type_item[row].level = strtol(column_ptr, NULL, 10)) < 1)
							|| (D_type_item[row].level > D_max_level) )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad level(%d)\n",
									i, line, D_type_item[row].level);
							return	(-1);
						}
						break;
					case	6:	
						if( ((D_type_item[row].stack_num = strtol(column_ptr, NULL, 10)) < 0)
							|| (D_type_item[row].stack_num > D_max_ushort) )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad stack_num(%d)\n",
									i, line, D_type_item[row].stack_num);
							return	(-1);
						}
						break;
					case	7:	
						if( (D_type_item[row].trade = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad trade(%d)\n",
									i, line, D_type_item[row].trade);
							return	(-1);
						}
						break;
					case	8:	
						if( ((D_type_item[row].buy_price = strtol(column_ptr, NULL, 10)) < 0)
							|| (D_type_item[row].buy_price*D_type_item[row].stack_num < 0) )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad buy_price(%d)\n",
									i, line, D_type_item[row].buy_price);
							return	(-1);
						}
						break;
					case	9:	
						if( ((D_type_item[row].sell_price = strtol(column_ptr, NULL, 10)) < 0)
							|| (D_type_item[row].sell_price*D_type_item[row].stack_num <0) )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad sell_price(%d)\n",
									i, line, D_type_item[row].sell_price);
							return	(-1);
						}
						break;
					case	10:	
						break;
					case	11:
						if( ((D_type_item[row].equip_id = strtol(column_ptr, NULL, 10)) < 0)
							|| (D_type_item[row].equip_id > EQUIP_SLOT_CNT) )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad equip_id(%d)\n",
									i, line, D_type_item[row].equip_id);
							return	(-1);
						}
						break;
					case	12:	
					case	13:	
					case	14:	
					case	15:
						break;	
					case	16:	
					case	17:	
					case	18:	
					case	19:	
					case	20:	
					case	21:	
					case	22:	
					case	23:	
					case	24:	
					case	25:	
					case	26:	
					case	27:	
					case	28:	
					case	29:	
					case	30:	
						idx	= column -16;
						if( (D_type_item[row].data[idx] = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad data[%d](%d)\n",
									i, line, idx, D_type_item[row].data[idx]);
							return	(-1);
						}
						break;
					default	:	
						U_Printf("[DB]Error file(%d) line(%d) bad column(%d)\n", i, line, column);	
						return	(-1);
				}//end switch 
			}//end while

			//check column
			if( column != 30 )
			{
				U_Printf("[DB]Error file(%d) line(%d) bad column(%d)\n", i, line, column);
				return	(-1);
			}
				
			//check data
			switch( D_type_item[row].class )
			{
				case	ITEM_CLASS_MONEY:
					if( row != ITEM_TYPE_MONEY )
					{
						U_Printf("[DB]Error file(%d) line(%d) bad money type\n", i, line);
						return	(-1);
					}
					if( D_type_item[row].stack_num != D_max_ushort )
					{
						U_Printf("[DB]Error file(%d) line(%d) bad stack_num(%d)\n", i, line, D_type_item[row].stack_num);
						return	(-1);
					}
					if( D_type_item[row].equip_id != EQUIP_SLOT_CNT )
					{
						U_Printf("[DB]Error file(%d) line(%d) bad equip_id(%d)\n", i, line, D_type_item[row].equip_id);
						return	(-1);
					}
					break;				
				case	ITEM_CLASS_ONEHAND_SWORD:
				case	ITEM_CLASS_TWOHAND_SWORD:
				case	ITEM_CLASS_BOW:
				case	ITEM_CLASS_ARROW:
					if( D_type_item[row].stack_num )
					{
						U_Printf("[DB]Error file(%d) line(%d) bad stack_num(%d)\n", i, line, D_type_item[row].stack_num);
						return	(-1);
					}				
					if( D_type_item[row].data[0] < 0 )//intensity
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[0](%d)\n", i, line, D_type_item[row].data[0]);
						return	(-1);
					}
					if( D_type_item[row].data[1] < 0 )//durability
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[1](%d)\n", i, line, D_type_item[row].data[1]);
						return	(-1);
					}
					if( D_type_item[row].data[2] < 0 )//repair_cost
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[2](%d)\n", i, line, D_type_item[row].data[2]);
						return	(-1);
					}
					if( D_type_item[row].data[3] < 0 )//att1
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[3](%d)\n", i, line, D_type_item[row].data[3]);
						return	(-1);
					}
					if( D_type_item[row].data[4] < 0 )//milli_attack_delay
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[4](%d)\n", i, line, D_type_item[row].data[4]);
						return	(-1);
					}
					if( D_type_item[row].data[5] < 0 )//attack_area
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[5](%d)\n", i, line, D_type_item[row].data[5]);
						return	(-1);
					}
					if( (D_type_item[row].data[6] < 0) || (D_type_item[row].data[6] > 100) )//cri
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[6](%d)\n", i, line, D_type_item[row].data[6]);
						return	(-1);
					}
					if( (D_type_item[row].data[7] < 0) || (D_type_item[row].data[7] > 100) )//i_att
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[7](%d)\n", i, line, D_type_item[row].data[7]);
						return	(-1);
					}
					if( (D_type_item[row].data[8] < 0) || (D_type_item[row].data[8] > MAX_ENCHANT_CNT) )//slot_num
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[8](%d)\n", i, line, D_type_item[row].data[8]);
						return	(-1);
					}
					if( D_type_item[row].data[9] < 0 )//att2
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[9](%d)\n", i, line, D_type_item[row].data[9]);
						return	(-1);
					}
					break;
				case	ITEM_CLASS_HAT:
				case	ITEM_CLASS_JACKET:
				case	ITEM_CLASS_PANTS:
				case	ITEM_CLASS_SHOES:
				case	ITEM_CLASS_SHIELD:
					if( D_type_item[row].stack_num )
					{
						U_Printf("[DB]Error file(%d) line(%d) bad stack_num(%d)\n", i, line, D_type_item[row].stack_num);
						return	(-1);
					}				
					if( D_type_item[row].data[0] < 1 )//intensity
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[0](%d)\n", i, line, D_type_item[row].data[0]);
						return	(-1);
					}
					if( D_type_item[row].data[1] < 1 )//durability
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[1](%d)\n", i, line, D_type_item[row].data[1]);
						return	(-1);
					}
					if( D_type_item[row].data[2] < 1 )//repair_cost
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[2](%d)\n", i, line, D_type_item[row].data[2]);
						return	(-1);
					}
					if( D_type_item[row].data[3] < 0 )//def1
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[3](%d)\n", i, line, D_type_item[row].data[3]);
						return	(-1);
					}
					if( (D_type_item[row].data[4] < 0) || (D_type_item[row].data[4] > 100) )//is_def
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[4](%d)\n", i, line, D_type_item[row].data[4]);
						return	(-1);
					}
					if( (D_type_item[row].data[5] < 0) || (D_type_item[row].data[5] > 100) )//il_def
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[5](%d)\n", i, line, D_type_item[row].data[5]);
						return	(-1);
					}
					if( (D_type_item[row].data[6] < 0) || (D_type_item[row].data[6] > MAX_ENCHANT_CNT) )//slot_num
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[6](%d)\n", i, line, D_type_item[row].data[6]);
						return	(-1);
					}
					if( (D_type_item[row].data[7] < 0) || (D_type_item[row].data[7] > 2) )//armor type
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[7](%d)\n", i, line, D_type_item[row].data[7]);
						return	(-1);
					}
					if( D_type_item[row].data[8] < 0 )//def2
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[8](%d)\n", i, line, D_type_item[row].data[8]);
						return	(-1);
					}
					if( D_type_item[row].data[9] < 0 )//m_def
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[9](%d)\n", i, line, D_type_item[row].data[9]);
						return	(-1);
					}
					break;
				case	ITEM_CLASS_EARRING:
				case	ITEM_CLASS_LEFT_RING:
				case	ITEM_CLASS_RIGHT_RING:
					if( D_type_item[row].stack_num )
					{
						U_Printf("[DB]Error file(%d) line(%d) bad stack_num(%d)\n",i, line, D_type_item[row].stack_num);
						return	(-1);
					}				
					if( D_type_item[row].data[0] < 1 )//intensity
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[0](%d)\n", i, line, D_type_item[row].data[0]);
						return	(-1);
					}
					if( D_type_item[row].data[1] < 1 )//durability
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[1](%d)\n", i, line, D_type_item[row].data[1]);
						return	(-1);
					}
					if( D_type_item[row].data[2] < 1 )//repair_cost
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[2](%d)\n", i, line, D_type_item[row].data[2]);
						return	(-1);
					}
					if( (D_type_item[row].data[3] < 0) || (D_type_item[row].data[3] > MAX_ENCHANT_CNT) )//slot_num
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[3](%d)\n", i, line, D_type_item[row].data[3]);
						return	(-1);
					}
					break;
				case	ITEM_CLASS_QUEST:
					if( D_type_item[row].stack_num )
					{
						U_Printf("[DB]Error file(%d) line(%d) bad stack_num(%d)\n", i, line, D_type_item[row].stack_num);
						return	(-1);
					}				
					if( D_type_item[row].equip_id != EQUIP_SLOT_CNT )
					{
						U_Printf("[DB]Error file(%d) line(%d) bad equip_id(%d)\n", i, line, D_type_item[row].equip_id);
						return	(-1);
					}
					break;
				case	ITEM_CLASS_RESTORE_POTION:		
					if( D_type_item[row].equip_id != EQUIP_SLOT_CNT )
					{
						U_Printf("[DB]Error file(%d) line(%d) bad equip_id(%d)\n", i, line, D_type_item[row].equip_id);
						return	(-1);
					}
					if( D_type_item[row].data[1] < 0 )//hp restoration
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[1](%d)\n", i, line, D_type_item[row].data[1]);
						return	(-1);
					}
					if( D_type_item[row].data[2] < 0 )//ap restoration
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[2](%d)\n", i, line, D_type_item[row].data[2]);
						return	(-1);
					}
					if( D_type_item[row].data[3] < 0 )//celerity time
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[3](%d)\n", i, line, D_type_item[row].data[3]);
						return	(-1);
					}
					if( (D_type_item[row].data[4] < 0) || (D_type_item[row].data[4] > MAX_COOL_SERIES) )//cool series
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[4](%d)\n", i, line, D_type_item[row].data[4]);
						return	(-1);
					}
					if( D_type_item[row].data[5] < 0 )//milli cool time
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[5](%d)\n", i, line, D_type_item[row].data[5]);
						return	(-1);
					}
					break;
				//case	ITEM_CLASS_BUFF_POTION:
				case	ITEM_CLASS_PORTAL:
					break;
				case	ITEM_CLASS_RESURRECTION:
					break;
				//case	ITEM_CLASS_MATERIAL:
				case	ITEM_CLASS_ENCHANT:
					if( D_type_item[row].equip_id != EQUIP_SLOT_CNT )
					{
						U_Printf("[DB]Error file(%d) line(%d) bad equip_id(%d)\n", i, line, D_type_item[row].equip_id);
						return	(-1);
					}
					if( D_type_item[row].stack_num )
					{
						U_Printf("[DB]Error file(%d) line(%d) bad stack_num(%d)\n", i, line, D_type_item[row].stack_num);
						return	(-1);
					}				
					if( (D_type_item[row].data[0] < 1) || (D_type_item[row].data[0] > MAX_ENCHANT_EQUIP_GROUP) )//equip_group
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[0](%d)\n", i, line, D_type_item[row].data[0]);
						return	(-1);
					}
					if( (D_type_item[row].data[1] < 1) || (D_type_item[row].data[1] > MAX_ENCHANT_OPTION_GROUP) )//option_group
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[1](%d)\n", i, line, D_type_item[row].data[1]);
						return	(-1);
					}
					if( (D_type_item[row].data[2] < 1) || (D_type_item[row].data[2] > MAX_ITEM_CLASS) )//enchant_class
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[2](%d)\n", i, line, D_type_item[row].data[2]);
						return	(-1);
					}
					if( (D_type_item[row].data[3] < 1) || (D_type_item[row].data[3] > MAX_ENCHANT_OPTION) )//option_id 
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[3](%d)\n", i, line, D_type_item[row].data[3]);
						return	(-1);
					}
					if( (D_type_item[row].data[4] < 0) || (D_type_item[row].data[4] > 1) )//unique_effect_id
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[4](%d)\n", i, line, D_type_item[row].data[4]);
						return	(-1);
					}
					if( (D_type_item[row].data[5] < 1) || (D_type_item[row].data[5] > MAX_ENCHANT_GRADE) )//grade
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[5](%d)\n", i, line, D_type_item[row].data[5]);
						return	(-1);
					}
					if( D_type_item[row].data[6] < 1 )//grade_data
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[6](%d)\n", i, line, D_type_item[row].data[6]);
						return	(-1);
					}
					break;
				case	ITEM_CLASS_TRANSFORM_BROW_CRYSTAL:
					if( D_type_item[row].equip_id != EQUIP_SLOT_CNT )
					{
						U_Printf("[DB]Error file(%d) line(%d) bad equip_id(%d)\n", i, line, D_type_item[row].equip_id);
						return	(-1);
					}
					if( D_type_item[row].stack_num )
					{
						U_Printf("[DB]Error file(%d) line(%d) bad stack_num(%d)\n", i, line, D_type_item[row].stack_num);
						return	(-1);
					}
					if( (D_type_item[row].data[0] < 1) || (D_type_item[row].data[0] > MAX_TRANSFORM_TYPE) )//transform type
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[0](%d)\n", i, line, D_type_item[row].data[0]);
						return	(-1);
					}
					if( (D_type_item[row].data[1] < 0) || (D_type_item[row].data[1] > 0) )//quest
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[1](%d)\n", i, line, D_type_item[row].data[1]);
						return	(-1);
					}
					if( (D_type_item[row].data[2] < 0) || (D_type_item[row].data[2] > 0) )//location
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[2](%d)\n", i, line, D_type_item[row].data[2]);
						return	(-1);
					}
					if( (D_type_item[row].data[3] < 0) || (D_type_item[row].data[3] > MAX_COOL_SERIES) )//cool series
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[3](%d)\n", i, line, D_type_item[row].data[3]);
						return	(-1);
					}
					if( D_type_item[row].data[4] < 0 )//milli cool time
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[4](%d)\n", i, line, D_type_item[row].data[4]);
						return	(-1);
					}
					if( (D_type_item[row].data[5] < 0) || (D_type_item[row].data[5] > TRANSFORM_SKILL_CNT) )//transform skill1
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[5](%d)\n", i, line, D_type_item[row].data[5]);
						return	(-1);
					}
					if( (D_type_item[row].data[6] < 0) || (D_type_item[row].data[6] > TRANSFORM_SKILL_CNT) )//transfrom skill2
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[6](%d)\n", i, line, D_type_item[row].data[6]);
						return	(-1);
					}
					break;
				case	ITEM_CLASS_CASH_COSTUME_ONEHAND_SWORD:
				case	ITEM_CLASS_CASH_COSTUME_TWOHAND_SWORD:
				case	ITEM_CLASS_CASH_COSTUME_BOW:
				case	ITEM_CLASS_CASH_COSTUME_SHIELD:
				case	ITEM_CLASS_CASH_COSTUME_HAT:
				case	ITEM_CLASS_CASH_COSTUME_FACE:
				case	ITEM_CLASS_CASH_COSTUME_JACKET:
				case	ITEM_CLASS_CASH_COSTUME_PANTS:
				case	ITEM_CLASS_CASH_COSTUME_ONEPIECE:
				case	ITEM_CLASS_CASH_COSTUME_CLOAK:
				case	ITEM_CLASS_CASH_COSTUME_SHOES:
				case	ITEM_CLASS_CASH_COSTUME_NECKLACE:
					if( D_type_item[row].stack_num )
					{
						U_Printf("[DB]Error file(%d) line(%d) bad stack_num(%d)\n", i, line, D_type_item[row].stack_num);
						return	(-1);
					}				
					if( D_type_item[row].data[0] < 0 )//hour of validity
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[0](%d)\n", i, line, D_type_item[row].data[0]);
						return	(-1);
					}
					if( (D_type_item[row].data[1] < 0) || (D_type_item[row].data[1] > EQUIP_SLOT_CNT) )//equip_id
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[1](%d)\n", i, line, D_type_item[row].data[1]);
						return	(-1);
					}
					if( (D_type_item[row].data[2] < 0) || (D_type_item[row].data[2] > MAX_ITEM_CLASS) )//class_id
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[2](%d)\n", i, line, D_type_item[row].data[2]);
						return	(-1);
					}
					if( (D_type_item[row].data[3] < 0) || (D_type_item[row].data[3] > (MAX_CHAR_SEX+1)) )//sex
					{
						U_Printf("[DB]Error file(%d) line(%d) bad data[3](%d)\n", i, line, D_type_item[row].data[3]);
						return	(-1);
					}
					break;	
				default	:	
					U_Printf("[DB]Error file(%d) line(%d) bad item class(%d)\n", i, line, D_type_item[row].class);	
					return	(-1);
			}//end switch  
		}//end while
	}//end for

	//free files
	for( i=1; i<=body1.file_cnt; i++ )
		free(file[i]);

	U_Printf("[DB]complete D_max_type_item(%d)\n", D_max_type_item);
	return	(0);
}


static	int	D_set_type_drop(const int sock_fd)
{
	int					n, i, j, k, line, row, column, group_idx, idx;
	char					buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	char					*row_ptr, *column_ptr, *file[MAX_SCRIPT_FILE_CNT+1];
	struct	body1_SW_TYPE_DROP_FILES_RESULT	body1;
	struct	body2_SW_TYPE_DROP_FILES_RESULT	body2;


	//send WS_TYPE_DROP_FILES
	n	= MK_GetPacket_WS_TYPE_DROP_FILES(buf);

	if( U_Nonb_send(sock_fd, buf, n, 3) )
	{
		U_Printf("[DB]Error U_Nonb_send()\n");
    		return	(-1);
	}

	//recv header
	if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}
 
	//recv body1	
	if( U_Nonb_recv(sock_fd, (char *)&body1, sizeof(struct body1_SW_TYPE_DROP_FILES_RESULT), 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}

	//check file_cnt
	if( (body1.file_cnt < 1) || (body1.file_cnt > MAX_SCRIPT_FILE_CNT) )
	{
		U_Printf("[DB]Error bad file_cnt(%d)\n", body1.file_cnt);
    		return	(-1);
	}

	//recv files
	for( i=1; i<=body1.file_cnt; i++ )
	{
		//recv file_size
		if( U_Nonb_recv(sock_fd, (char *)(&body2), sizeof(struct body2_SW_TYPE_DROP_FILES_RESULT), 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
   	 		return	(-1);
		}

		//check file_size
		if( body2.file_size < 1 )
		{
			U_Printf("[DB]Error bad file_size(%d)\n", body2.file_size);
    			return	(-1);
		}
		
		//alloc file
		if( (file[i] = calloc(1, body2.file_size)) == NULL )
		{
			U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    			return	(-1);
		}
		
		//recv file 
		if( U_Nonb_recv(sock_fd, file[i], body2.file_size, 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
   	 		return	(-1);
		}

		//count line
		line	= 0;

		for( j=0; j<body2.file_size; j++ )
		{	
			if( file[i][j] == 0 )
			{
				U_Printf("[DB]Error file(%d) bad data\n", i);
    				return	(-1);
			}
		
			if( file[i][j] == '\n' )
				++line;
		}

		if( file[i][body2.file_size-1] == '\n' )
		{
			--line;
			file[i][body2.file_size-1] = 0; 
		}
	
		//check line
		if( line < 1 )
		{
			U_Printf("[DB]Error file(%d) bad line(%d)\n", i, line);
    			return	(-1);
		}

		//set D_max_type_drop
		D_max_type_drop	+= line;
	}
		
	//alloc D_type_drop
	if( (D_type_drop = calloc(D_max_type_drop+1, sizeof(struct type_drop))) == NULL )
	{
		U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    		return	(-1);
	}

	//load D_type_drop
	row	= 0;

	for( i=1; i<=body1.file_cnt; i++ )
	{
		//first line
		row_ptr	= strtok(file[i], "\n");

		//other line
		line	= 0;
		
		while( (row_ptr = strtok(NULL, "\n")) )
		{
			++row;
			++line;
			column	= 0;
		
			//set column
			while( (column_ptr = strsep(&row_ptr, "\t")) )
			{
				++column;

				//group
				if( (column >= 3) && (column <= 122) )
				{
					if( (column % 2) )
					{
						group_idx	= (column - 3)/40;
						idx		= (column - group_idx*40)/2 -1;	
						if( ((D_type_drop[row].group[group_idx][idx].item_type = strtol(column_ptr, NULL, 10)) < 0) 
							|| (D_type_drop[row].group[group_idx][idx].item_type > D_max_type_item) )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad gruop[%d][%d].item_type(%d)\n",
									i, line, group_idx, idx, D_type_drop[row].group[group_idx][idx].item_type);
							return	(-1);
						}
					}
					else
					{
						group_idx	= (column - 4)/40;
						idx		= (column - group_idx*40 -1)/2 -1;	
						if( (D_type_drop[row].group[group_idx][idx].drop_prob = strtol(column_ptr, NULL, 10)) < 0 ) 
						{
							U_Printf("[DB]Error file(%d) line(%d) bad gruop[%d][%d].drop_prob(%d)\n",
									i, line, group_idx, idx, D_type_drop[row].group[group_idx][idx].drop_prob);
							return	(-1);
						}
					}

					continue;
				}
				
				switch( column ) 
				{
					case	1:	
						if( (D_type_drop[row].index = strtol(column_ptr, NULL, 10)) != row )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad index(%d)\n", i, line, D_type_drop[row].index);
							return	(-1);
						}
						break;
					case	2:
						if( *column_ptr == 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad name\n", i, line);
							return	(-1);
						}
						break;
					case	123:
						if( (D_type_drop[row].enchant_drop_prob = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad enchant_drop_prob(%d)\n",
									i, line, D_type_drop[row].enchant_drop_prob);
							return	(-1);
						}
						break;
					case	124:
						D_type_drop[row].enchant_equip_bitmask = strtol(column_ptr, NULL, 10);
						break;
					case	125:
						D_type_drop[row].enchant_option_bitmask = strtol(column_ptr, NULL, 10);
						break;
					case	126:
						if( ((D_type_drop[row].enchant_level = strtol(column_ptr, NULL, 10)) < 0) 
								|| (D_type_drop[row].enchant_level > D_max_level) )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad enchant_level(%d)\n",
									i, line, D_type_drop[row].enchant_level);
							return	(-1);
						}
						break;
					case	127:
					case	128:
					case	129:
					case	130:
					case	131:
					case	132:
					case	133:
					case	134:
					case	135:
						idx	= (column - 127);	
						if( (D_type_drop[row].enchant_grade_prob[idx] = strtol(column_ptr, NULL, 10)) < 0) 
						{
							U_Printf("[DB]Error file(%d) line(%d) bad enchant_grade_prob[%d](%d)\n",
									i, line, idx, D_type_drop[row].enchant_grade_prob[idx]);
							return	(-1);
						}
						break;

					default	:	
						U_Printf("[DB]Error file(%d) line(%d) bad column(%d)\n", i, line, column);	
						return	(-1);
				}
			}

			//check column
			if( column != 135 )
			{
				U_Printf("[DB]Error file(%d) line(%d) bad column(%d)\n", i, line, column);
				return	(-1);
			}
		}//end while
	}//end for

	//free files
	for( i=1; i<=body1.file_cnt; i++ )
		free(file[i]);
	
	//compute drop_prob 
	for( i=1; i<=D_max_type_drop; i++ )
	{
		//group
		for( j=0; j<3; j++ )
		{
			for( k=1; k<20; k++ )
			{
				if( D_type_drop[i].group[j][k].item_type )
					D_type_drop[i].group[j][k].drop_prob	+= D_type_drop[i].group[j][k-1].drop_prob;	
				else
					D_type_drop[i].group[j][k].drop_prob	= 100000000;	
			}
		}
	
		//enchant_drop_prob
		if( D_type_drop[i].enchant_drop_prob )
		{
			//enchant_grade_prob
			for( j=1; j<9; j++ )
				D_type_drop[i].enchant_grade_prob[j]	+= D_type_drop[i].enchant_grade_prob[j-1];	
		
			if( D_type_drop[i].enchant_grade_prob[8] != 100000000 )
			{
				U_Printf("[DB]Error bad D_type_drop[%d].enchant_grade_drop[8](%d)\n", i, D_type_drop[i].enchant_grade_prob[8]);
				return	(-1);
			}

			//enchant_equip_bitmask
			for( j=0; j<32; j++ )
				if( (D_type_drop[i].enchant_equip_bitmask >> j) & 0x00000001 )
					D_type_drop[i].enchant_equip_group[(D_type_drop[i].enchant_equip_group_num)++]		= j+1;

			//enchant_option_bitmask
			for( j=0; j<32; j++ )
				if( (D_type_drop[i].enchant_option_bitmask >> j) & 0x00000001 )
					D_type_drop[i].enchant_option_group[(D_type_drop[i].enchant_option_group_num)++]	= j+1;
		}
	}

	U_Printf("[DB]complete D_max_type_drop(%d)\n", D_max_type_drop);
	return	(0);
}


static	int	D_set_drop_enchant(void)
{
	int	i, equip_group, option_group, level, grade;


	//alloc D_drop_enchant
	if( (D_drop_enchant
		= calloc((D_max_level+1)*(MAX_ENCHANT_EQUIP_GROUP+1)*(MAX_ENCHANT_OPTION_GROUP+1)*(MAX_ENCHANT_GRADE+1), SIZEOF_INT)) == NULL )
	{
		U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    		return	(-1);
	}
	
	//set D_drop_enchant
	for( i=1; i<=D_max_type_item; i++ )
	{
		if( D_type_item[i].class == ITEM_CLASS_ENCHANT )		
		{
			equip_group	= D_type_item[i].data[0];
			option_group	= D_type_item[i].data[1];
			grade		= D_type_item[i].data[5];
			level		= D_type_item[i].level;

			D_drop_enchant[level][equip_group][option_group][grade]	= i;
		}
	}
	
	U_Printf("[DB]complete D_drop_enchant(%d)(%d)(%d)(%d)\n",
					D_max_level, MAX_ENCHANT_EQUIP_GROUP, MAX_ENCHANT_OPTION_GROUP, MAX_ENCHANT_GRADE);
	return	(0);
}


static	int	D_set_type_char_skill(const int sock_fd)
{
	int						n, i, j, line, row, column, idx;
	char						buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	char						*row_ptr, *column_ptr, *file[MAX_SCRIPT_FILE_CNT+1];
	struct	body1_SW_TYPE_SKILL_FILES_RESULT	body1;
	struct	body2_SW_TYPE_SKILL_FILES_RESULT	body2;


	//send WS_TYPE_SKILL_FILES
	n	= MK_GetPacket_WS_TYPE_SKILL_FILES(buf);

	if( U_Nonb_send(sock_fd, buf, n, 3) )
	{
		U_Printf("[DB]Error U_Nonb_send()\n");
    		return	(-1);
	}

	//recv header
	if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}
 
	//recv body1	
	if( U_Nonb_recv(sock_fd, (char *)&body1, sizeof(struct body1_SW_TYPE_SKILL_FILES_RESULT), 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}

	//check file_cnt
	if( (body1.file_cnt < 1) || (body1.file_cnt > MAX_SCRIPT_FILE_CNT) )
	{
		U_Printf("[DB]Error bad file_cnt(%d)\n", body1.file_cnt);
    		return	(-1);
	}

	//recv files
	for( i=1; i<=body1.file_cnt; i++ )
	{
		//recv file_size
		if( U_Nonb_recv(sock_fd, (char *)(&body2), sizeof(struct body2_SW_TYPE_SKILL_FILES_RESULT), 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
   	 		return	(-1);
		}

		//check file_size
		if( body2.file_size < 1 )
		{
			U_Printf("[DB]Error bad file_size(%d)\n", body2.file_size);
    			return	(-1);
		}
		
		//alloc file
		if( (file[i] = calloc(1, body2.file_size)) == NULL )
		{
			U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    			return	(-1);
		}
		
		//recv file 
		if( U_Nonb_recv(sock_fd, file[i], body2.file_size, 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
   	 		return	(-1);
		}
		
		//count line
		line	= 0;

		for( j=0; j<body2.file_size; j++ )
		{	
			if( file[i][j] == 0 )
			{
				U_Printf("[DB]Error file(%d) bad data\n", i);
    				return	(-1);
			}
	
			if( file[i][j] == '\n' )
				++line;
		}

		if( file[i][body2.file_size-1] == '\n' )
		{
			--line;
			file[i][body2.file_size-1] = 0; 
		}
	
		//check line
		if( line != CHAR_SKILL_CNT )
		{
			U_Printf("[DB]Error file(%d) bad line(%d)\n", i, line);
    			return	(-1);
		}
	}

	//set D_max_type_char_skill_level
	D_max_type_char_skill_level	= body1.file_cnt;

	//alloc D_type_char_skill
	if( (D_type_char_skill = calloc((D_max_type_char_skill_level+1)*(CHAR_SKILL_CNT+1), sizeof(struct type_char_skill))) == NULL )
	{
		U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    		return	(-1);
	}
	
	for( i=1; i<=body1.file_cnt; i++ )
	{
		//first line
		row_ptr	= strtok(file[i], "\n");
		
		//other line
		line	= 0;
		row	= 0;
		
		while( (row_ptr = strtok(NULL, "\n")) )
		{
			++row;
			++line;
			column	= 0;
			
			//set column
			while( (column_ptr = strsep(&row_ptr, "\t")) )
			{
				++column;

				switch( column ) 
				{
					case	1:	
						if( (D_type_char_skill[i][row].index = strtol(column_ptr, NULL, 10)) != row )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad index(%d)\n",
										i, line, D_type_char_skill[i][row].index);
							return	(-1);
						}
						break;
					case	2:
						break;
					case	3:
					case	4:
					case	5:
						idx	= column - 3;
						if( ((D_type_char_skill[i][row].order[idx].function = strtol(column_ptr, NULL, 10)) < 0) 
							|| (D_type_char_skill[i][row].order[idx].function > MAX_SKILL_FUNCTION) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) (%d)function(%d)\n",
										i, line, column, idx, D_type_char_skill[i][row].order[idx].function);
							return	(-1);
						}
						break;
					case	6:
					case	7:
					case	8:
						idx	= column - 6;
						D_type_char_skill[i][row].order[idx].damage	= strtol(column_ptr, NULL, 10);
						break;
					case	9:
					case	10:
					case	11:
						idx	= column - 9;
						if( (D_type_char_skill[i][row].order[idx].sec = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) (%d)sec(%d)\n",
										i, line, column, idx, D_type_char_skill[i][row].order[idx].sec);
							return	(-1);
						}
						break;
					case	12:
						if( ((D_type_char_skill[i][row].method = strtol(column_ptr, NULL, 10)) < 1) 
							|| (D_type_char_skill[i][row].method > 8) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad method(%d)\n",
										i, line, column, D_type_char_skill[i][row].method);
							return	(-1);
						}
						break;
					case	13:
						if( ((D_type_char_skill[i][row].req_equip_slot = strtol(column_ptr, NULL, 10)) < 0) 
									|| (D_type_char_skill[i][row].req_equip_slot >= EQUIP_SLOT_CNT) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad req_equip_slot(%d)\n",
										i, line, column, D_type_char_skill[i][row].req_equip_slot);
							return	(-1);
						}
						break;
					case	14:
						if( ((D_type_char_skill[i][row].req_equip_class = strtol(column_ptr, NULL, 10)) < 0) 
							|| (D_type_char_skill[i][row].req_equip_class > MAX_ITEM_CLASS) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad req_equip_class(%d)\n",
										i, line, column, D_type_char_skill[i][row].req_equip_class);
							return	(-1);
						}
						break;
					case	15:
						if( ((D_type_char_skill[i][row].req_material = strtol(column_ptr, NULL, 10)) < 0) 
							|| (D_type_char_skill[i][row].req_material > 0) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad req_material(%d)\n",
										i, line, column, D_type_char_skill[i][row].req_material);
							return	(-1);
						}
						break;
					case	16:
						break;
					case	17:
						if( ((D_type_char_skill[i][row].req_special = strtol(column_ptr, NULL, 10)) < 0) 
							|| (D_type_char_skill[i][row].req_special > 0) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad req_special(%d)\n",
										i, line, column, D_type_char_skill[i][row].req_special);
							return	(-1);
						}
						break;
					case	18:
						if( ((D_type_char_skill[i][row].max_level = strtol(column_ptr, NULL, 10)) < 1) 
							|| (D_type_char_skill[i][row].max_level > D_max_type_char_skill_level) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad max_level(%d)\n",
										i, line, column, D_type_char_skill[i][row].max_level);
							return	(-1);
						}
						break;
					case	19:
						if( ((D_type_char_skill[i][row].action_cnt = strtol(column_ptr, NULL, 10)) < 1)
							|| (D_type_char_skill[i][row].action_cnt > D_max_uchar) ) 
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad action_cnt(%d)\n",
										i, line, column, D_type_char_skill[i][row].action_cnt);
							return	(-1);
						}
						break;
					case	20:
						if( (D_type_char_skill[i][row].action_millisec = strtol(column_ptr, NULL, 10)) < 0 ) 
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad action_millisec(%d)\n",
										i, line, column, D_type_char_skill[i][row].action_millisec);
							return	(-1);
						}
						break;
					case	21:
						if( (D_type_char_skill[i][row].use_ap = strtol(column_ptr, NULL, 10)) < 0 ) 
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad use_ap(%d)\n",
										i, line, column, D_type_char_skill[i][row].use_ap);
							return	(-1);
						}
						break;
					case	22:
						if( ((D_type_char_skill[i][row].req_buff_level = strtol(column_ptr, NULL, 10)) < 0) 
							|| (D_type_char_skill[i][row].req_buff_level > D_max_level) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad req_buff_level(%d)\n",
										i, line, column, D_type_char_skill[i][row].req_buff_level);
							return	(-1);
						}
						break;
					case	23:
						if( ((D_type_char_skill[i][row].target_object = strtol(column_ptr, NULL, 10)) < 1) 
							|| (D_type_char_skill[i][row].target_object > 5) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad target_object(%d)\n",
										i, line, column, D_type_char_skill[i][row].target_object);
							return	(-1);
						}
						break;
					case	24:
						if( ((D_type_char_skill[i][row].target_apply = strtol(column_ptr, NULL, 10)) < 1) 
							|| (D_type_char_skill[i][row].target_apply > 5) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad target_apply(%d)\n",
										i, line, column, D_type_char_skill[i][row].target_apply);
							return	(-1);
						}
						break;
					case	25:
						if( ((D_type_char_skill[i][row].target_distance = strtol(column_ptr, NULL, 10)) < 0) 
							|| (D_type_char_skill[i][row].target_distance > 10) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad target_distance(%d)\n",
										i, line, column, D_type_char_skill[i][row].target_distance);
							return	(-1);
						}
						break;
					case	26:
						if( ((D_type_char_skill[i][row].range_type = strtol(column_ptr, NULL, 10)) < 0) 
								|| (D_type_char_skill[i][row].range_type > 2) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad range_type(%d)\n",
										i, line, column, D_type_char_skill[i][row].range_type);
							return	(-1);
						}
						break;
					case	27:
						if( ((D_type_char_skill[i][row].range_x = strtol(column_ptr, NULL, 10)) < 0) 
								|| (D_type_char_skill[i][row].range_x > 10) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad range_x(%d)\n",
										i, line, column, D_type_char_skill[i][row].range_x);
							return	(-1);
						}
						break;
					case	28:
						if( ((D_type_char_skill[i][row].range_z = strtol(column_ptr, NULL, 10)) < 0) 
								|| (D_type_char_skill[i][row].range_z > 10) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad range_z(%d)\n",
										i, line, column, D_type_char_skill[i][row].range_z);
							return	(-1);
						}
						break;
					case	29:
						if( ((D_type_char_skill[i][row].learn_req_quest = strtol(column_ptr, NULL, 10)) < 0) 
											|| (D_type_char_skill[i][row].learn_req_quest > 0) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad learn_req_quest(%d)\n",
									i, line, column, D_type_char_skill[i][row].learn_req_quest);
							return	(-1);
						}
						break;
					case	30:
						if( ((D_type_char_skill[i][row].learn_req_skill = strtol(column_ptr, NULL, 10)) < 0) 
										|| (D_type_char_skill[i][row].learn_req_skill > CHAR_SKILL_CNT) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad learn_req_skill(%d)\n",
									i, line, column, D_type_char_skill[i][row].learn_req_skill);
							return	(-1);
						}
						break;
					case	31:
						if( ((D_type_char_skill[i][row].learn_req_skill_level = strtol(column_ptr, NULL, 10)) < 0) 
								|| (D_type_char_skill[i][row].learn_req_skill_level > D_max_type_char_skill_level) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad learn_req_skill_level(%d)\n",
									i, line, column, D_type_char_skill[i][row].learn_req_skill_level);
							return	(-1);
						}
						break;
					case	32:
						if( (D_type_char_skill[i][row].learn_req_skill_point = strtol(column_ptr, NULL, 10)) < 0 )  
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad learn_req_skill_point(%d)\n",
									i, line, column, D_type_char_skill[i][row].learn_req_skill_point);
							return	(-1);
						}
						break;
					case	33:
						if( ((D_type_char_skill[i][row].learn_req_level = strtol(column_ptr, NULL, 10)) < 0) 
									|| (D_type_char_skill[i][row].learn_req_level > D_max_level) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad learn_req_level(%d)\n",
									i, line, column, D_type_char_skill[i][row].learn_req_level);
							return	(-1);
						}
						break;
					case	34:
						if( (D_type_char_skill[i][row].casting_millisec = strtol(column_ptr, NULL, 10)) < 0 )  
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad casting_millisec(%d)\n",
									i, line, column, D_type_char_skill[i][row].casting_millisec);
							return	(-1);
						}
						break;
					case	35:
						if( (D_type_char_skill[i][row].cool_millisec = strtol(column_ptr, NULL, 10)) < 0 )  
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad cool_millisec(%d)\n",
									i, line, column, D_type_char_skill[i][row].cool_millisec);
							return	(-1);
						}
						break;
					case	36:
						if( ((D_type_char_skill[i][row].cool_serise = strtol(column_ptr, NULL, 10)) < 0) 
									|| (D_type_char_skill[i][row].cool_serise > MAX_COOL_SERIES) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad cool_serise(%d)\n",
									i, line, column, D_type_char_skill[i][row].cool_serise);
							return	(-1);
						}
						break;
					case	37:
						break;
					default	:	
						U_Printf("[DB]Error file(%d) line(%d) bad column(%d)\n", i, line, column);	
						return	(-1);
				}
			}

			//check column
			if( column != 37 )
			{
				U_Printf("[DB]Error file(%d) line(%d) bad column(%d)\n", i, line, column);
				return	(-1);
			}

			//check method
			if( (D_type_char_skill[i][row].method == 2) && (D_type_char_skill[i][row].req_equip_class == 0) )
			{
				U_Printf("[DB]Error file(%d) line(%d) method(%d) bad req_equip_class(%d)\n",
							i, line, D_type_char_skill[i][row].method, D_type_char_skill[i][row].req_equip_class);
				return	(-1);
			}
		
		}//end while
	}//end for


	//free files
	for( i=1; i<=body1.file_cnt; i++ )
		free(file[i]);

	U_Printf("[DB]complete D_max_type_char_skill_level(%d) CHAR_SKILL_CNT(%d)\n", D_max_type_char_skill_level, CHAR_SKILL_CNT);
	return	(0);
}


static	int	D_set_equip_passive_char_skill(void)
{
	int	i, req_equip_class;


	for( i=1; i<=CHAR_SKILL_CNT; i++ )
	{
		if( D_type_char_skill[1][i].method == 2 )
		{
			req_equip_class	= D_type_char_skill[1][i].req_equip_class;
			D_equip_passive_char_skill[req_equip_class].skill_type[(D_equip_passive_char_skill[req_equip_class].skill_cnt)++]	= i;
		}
	}
	
	return	(0);
}


static	int	D_set_type_mob_skill(const int sock_fd)
{
	int						n, i, j, line, row, column, idx;
	char						buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	char						*row_ptr, *column_ptr, *file[MAX_SCRIPT_FILE_CNT+1];
	struct	body1_SW_TYPE_MOB_SKILL_FILES_RESULT	body1;
	struct	body2_SW_TYPE_MOB_SKILL_FILES_RESULT	body2;


	//send WS_TYPE_MOB_SKILL_FILES
	n	= MK_GetPacket_WS_TYPE_MOB_SKILL_FILES(buf);

	if( U_Nonb_send(sock_fd, buf, n, 3) )
	{
		U_Printf("[DB]Error U_Nonb_send()\n");
    		return	(-1);
	}

	//recv header
	if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}
 
	//recv body1	
	if( U_Nonb_recv(sock_fd, (char *)&body1, sizeof(struct body1_SW_TYPE_MOB_SKILL_FILES_RESULT), 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}

	//check file_cnt
	if( (body1.file_cnt < 1) || (body1.file_cnt > MAX_SCRIPT_FILE_CNT) )
	{
		U_Printf("[DB]Error bad file_cnt(%d)\n", body1.file_cnt);
    		return	(-1);
	}
	
	//recv files
	for( i=1; i<=body1.file_cnt; i++ )
	{
		//recv file_size
		if( U_Nonb_recv(sock_fd, (char *)(&body2), sizeof(struct body2_SW_TYPE_MOB_SKILL_FILES_RESULT), 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
   	 		return	(-1);
		}

		//check file_size
		if( body2.file_size < 1 )
		{
			U_Printf("[DB]Error bad file_size(%d)\n", body2.file_size);
    			return	(-1);
		}
		
		//alloc file
		if( (file[i] = calloc(1, body2.file_size)) == NULL )
		{
			U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    			return	(-1);
		}
		
		//recv file 
		if( U_Nonb_recv(sock_fd, file[i], body2.file_size, 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
   	 		return	(-1);
		}
		
		//count line
		line	= 0;

		for( j=0; j<body2.file_size; j++ )
		{			
			if( file[i][j] == 0 )
			{
				U_Printf("[DB]Error file(%d) bad data\n", i);
    				return	(-1);
			}

			if( file[i][j] == '\n' )
				++line;
		}

		if( file[i][body2.file_size-1] == '\n' )
		{
			--line;
			file[i][body2.file_size-1] = 0; 
		}
	
		//check line
		if( line < 1 )
		{
			U_Printf("[DB]Error file(%d) bad line(%d)\n", i, line);
    			return	(-1);
		}

		//set D_max_type_mob_skill
		D_max_type_mob_skill	+= line;
	}
		
	//alloc D_type_mob_skill
	if( (D_type_mob_skill = calloc(D_max_type_mob_skill+1, sizeof(struct type_mob_skill))) == NULL )
	{
		U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    		return	(-1);
	}

	//load D_type_mob_skill
	row	= 0;

	for( i=1; i<=body1.file_cnt; i++ )
	{
		//first line
		row_ptr	= strtok(file[i], "\n");
		
		//other line
		line	= 0;
		
		while( (row_ptr = strtok(NULL, "\n")) )
		{
			++row;
			++line;
			column	= 0;
		
			//set column
			while( (column_ptr = strsep(&row_ptr, "\t")) )
			{
				++column;

				switch( column ) 
				{
					case	1:	
						if( (D_type_mob_skill[row].index = strtol(column_ptr, NULL, 10)) != row )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad index(%d)\n",
										i, line, D_type_mob_skill[row].index);
							return	(-1);
						}
						break;
					case	2:
						break;	
					case	3:
					case	4:
					case	5:
						idx	= column - 3;
						if( ((D_type_mob_skill[row].order[idx].function = strtol(column_ptr, NULL, 10)) < 0) 
							|| (D_type_mob_skill[row].order[idx].function > MAX_SKILL_FUNCTION) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) (%d)function(%d)\n",
										i, line, column, idx, D_type_mob_skill[row].order[idx].function);
							return	(-1);
						}
						break;
					case	6:
					case	7:
					case	8:
						idx	= column - 6;
						D_type_mob_skill[row].order[idx].damage = strtol(column_ptr, NULL, 10);
						break;
					case	9:
					case	10:
					case	11:
						idx	= column - 9;
						if( (D_type_mob_skill[row].order[idx].sec = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) (%d)sec(%d)\n",
										i, line, column, idx, D_type_mob_skill[row].order[idx].sec);
							return	(-1);
						}
						break;
					case	12:
						if( ((D_type_mob_skill[row].action_cnt = strtol(column_ptr, NULL, 10)) < 1)
										|| (D_type_mob_skill[row].action_cnt > D_max_uchar) ) 
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad action_cnt(%d)\n",
										i, line, column, D_type_mob_skill[row].action_cnt);
							return	(-1);
						}
						break;
					case	13:
						break;
					case	14:
						if( (D_type_mob_skill[row].use_ap = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) use_ap(%d)\n",
										i, line, column, D_type_mob_skill[row].use_ap);
							return	(-1);
						}
						break;
					case	15:
						if( ((D_type_mob_skill[row].target_object = strtol(column_ptr, NULL, 10)) < 1) 
							|| (D_type_mob_skill[row].target_object > 4) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad target_object(%d)\n",
										i, line, column, D_type_mob_skill[row].target_object);
							return	(-1);
						}
						break;
					case	16:
						if( ((D_type_mob_skill[row].target_apply = strtol(column_ptr, NULL, 10)) < 1) 
							|| (D_type_mob_skill[row].target_apply > 3) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad target_apply(%d)\n",
										i, line, column, D_type_mob_skill[row].target_apply);
							return	(-1);
						}
						break;
					case	17:
						if( (D_type_mob_skill[row].target_distance = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) target_distance(%d)\n",
										i, line, column, D_type_mob_skill[row].target_distance);
							return	(-1);
						}
						break;
					case	18:
						break;
					case	19:
						if( ((D_type_mob_skill[row].range_x = strtol(column_ptr, NULL, 10)) < 0) 
								|| (D_type_mob_skill[row].range_x > 10) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad range_x(%d)\n",
										i, line, column, D_type_mob_skill[row].range_x);
							return	(-1);
						}
						break;
					case	20:
						if( ((D_type_mob_skill[row].range_z = strtol(column_ptr, NULL, 10)) < 0) 
								|| (D_type_mob_skill[row].range_z > 10) )
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad range_z(%d)\n",
										i, line, column, D_type_mob_skill[row].range_z);
							return	(-1);
						}
						break;
					case	21:
						if( (D_type_mob_skill[row].casting_millisec = strtol(column_ptr, NULL, 10)) < 0 )  
						{
							U_Printf("[DB]Error file(%d) line(%d) column(%d) bad casting_millisec(%d)\n",
									i, line, column, D_type_mob_skill[row].casting_millisec);
							return	(-1);
						}
						break;
					case	22:
					case	23:
					case	24:
					case	25:
					case	26:
					case	27:
						break;
					default	:	
						U_Printf("[DB]Error file(%d) line(%d) bad column(%d)\n", i, line, column);	
						return	(-1);
				}
			}

			//check column
			if( column != 27 )
			{
				U_Printf("[DB]Error file(%d) line(%d) bad column(%d)\n", i, line, column);
				return	(-1);
			}
		
		}//end while
	}//end for


	//free files
	for( i=1; i<=body1.file_cnt; i++ )
		free(file[i]);

	U_Printf("[DB]complete D_max_type_mob_skill(%d)\n", D_max_type_mob_skill);
	return	(0);

}



//______________________________________________________________
//
static	inline	int	D_get_mob_hp_max(const int channel_link, const int mob_id)
{
	return	(D_mob[mob_id][channel_link].hp_max + D_mob[mob_id][channel_link].buff_value[_BUFF_PASSIVE_HP_MAX]);
}


static	inline	int	D_get_mob_ap_max(const int channel_link, const int mob_id)
{
	return	(D_mob[mob_id][channel_link].ap_max + D_mob[mob_id][channel_link].buff_value[_BUFF_PASSIVE_AP_MAX]);
}


static	inline	int	D_get_mob_att_land(const int channel_link, const int mob_id)
{
	return	(0);
}


static	inline	int	D_get_mob_att_water(const int channel_link, const int mob_id)
{
	return	(0);
}


static	inline	int	D_get_mob_att_fire(const int channel_link, const int mob_id)
{
	return	(0);
}


static	inline	int	D_get_mob_att_wind(const int channel_link, const int mob_id)
{
	return	(0);
}

static	inline	int	D_get_mob_m_def(const int channel_link, const int mob_id)
{
	return	(D_mob[mob_id][channel_link].m_def + D_mob[mob_id][channel_link].buff_value[_BUFF_ACTIVE_MAGIC_DEF]);
}


static	inline	int	D_get_mob_att1(const int channel_link, const int mob_id)
{
	return	(D_mob[mob_id][channel_link].att1 + D_mob[mob_id][channel_link].buff_value[_BUFF_ACTIVE_ATT1]);
}


static	inline	int	D_get_mob_att2(const int channel_link, const int mob_id)
{
	return	(D_mob[mob_id][channel_link].att2);
}


static	inline	int	D_get_mob_def1(const int channel_link, const int mob_id)
{
	return	(D_mob[mob_id][channel_link].def1 + D_mob[mob_id][channel_link].buff_value[_BUFF_ACTIVE_DEF1]
							- D_mob[mob_id][channel_link].buff_value[_BUFF_DE_ACTIVE_DEF1]);
}


static	inline	int	D_get_mob_def2(const int channel_link, const int mob_id)
{
	return	(D_mob[mob_id][channel_link].def2);
}


static	inline	int	D_get_mob_eva(const int channel_link, const int mob_id)
{
	return	(D_mob[mob_id][channel_link].eva);
}	


static	inline	int	D_get_mob_cri(const int channel_link, const int mob_id)
{
	return	(D_mob[mob_id][channel_link].cri);
}


static	inline	int	D_get_mob_i_att(const int channel_link, const int mob_id)
{
	return	(D_mob[mob_id][channel_link].i_att);
}


static	inline	int	D_get_mob_is_def(const int channel_link, const int mob_id)
{
	return	(D_mob[mob_id][channel_link].is_def);
}


static	inline	int	D_get_mob_il_def(const int channel_link, const int mob_id)
{
	return	(D_mob[mob_id][channel_link].il_def);
}


static	inline	int	D_get_mob_recover_hp(const int channel_link, const int mob_id)
{
	return	(D_mob[mob_id][channel_link].recover_hp
			+ D_mob[mob_id][channel_link].buff_value[_BUFF_PASSIVE_HP_RECOVER]
	- D_mob[mob_id][channel_link].buff_value[_BUFF_DE_ACTIVE_POISONING] - D_mob[mob_id][channel_link].buff_value[_BUFF_DE_ACTIVE_FIREPOISONING]);
}


static	inline	int	D_get_mob_recover_ap(const int channel_link, const int mob_id)
{
	return	(D_mob[mob_id][channel_link].recover_ap + D_mob[mob_id][channel_link].buff_value[_BUFF_PASSIVE_AP_RECOVER]);
}


static	inline	int	D_get_mob_att_area(const int channel_link, const int mob_id)
{
	return	(D_mob[mob_id][channel_link].att_area);
}


static	inline	int	D_get_mob_att_milli_speed(const int channel_link, const int mob_id)
{
	return	((D_mob[mob_id][channel_link].att_milli_speed)
				/((100 + 0)/100.0f)*((100 + D_mob[mob_id][channel_link].buff_value[_BUFF_DE_ACTIVE_ATT_SPEED])/100.0f));
}


static	inline	float	D_get_mob_move_speed1(const int channel_link, const int mob_id)
{
	return	((D_mob[mob_id][channel_link].move_speed1)
				/((100 + D_mob[mob_id][channel_link].buff_value[_BUFF_DE_ACTIVE_MOVE_SPEED])/100)
				*((100 + D_mob[mob_id][channel_link].buff_value[_BUFF_ACTIVE_MOVE_SPEED])/100));
}


static	inline	float	D_get_mob_move_speed2(const int channel_link, const int mob_id)
{
	return	((D_mob[mob_id][channel_link].move_speed2)
				/((100 + D_mob[mob_id][channel_link].buff_value[_BUFF_DE_ACTIVE_MOVE_SPEED])/100)
				*((100 + D_mob[mob_id][channel_link].buff_value[_BUFF_ACTIVE_MOVE_SPEED])/100));
}


static	inline	float	D_get_mob_move_speed3(const int channel_link, const int mob_id)
{
	return	((D_mob[mob_id][channel_link].move_speed3)
				/((100 + D_mob[mob_id][channel_link].buff_value[_BUFF_DE_ACTIVE_MOVE_SPEED])/100)
				*((100 + D_mob[mob_id][channel_link].buff_value[_BUFF_ACTIVE_MOVE_SPEED])/100));
}



//______________________________________________________________
//
static	inline	int	D_get_client_hp_max(const int fd)
{
	return	(D_client[fd].hp_max + D_client[fd].hp_max_equip + D_client[fd].buff_value[_BUFF_PASSIVE_HP_MAX]);
}


static	inline	int	D_get_client_ap_max(const int fd)
{
	return	(D_client[fd].ap_max + D_client[fd].ap_max_equip + D_client[fd].buff_value[_BUFF_PASSIVE_AP_MAX]);
}


static	inline	int	D_get_client_att_land(const int fd)
{
	return	(D_client[fd].att_land_equip);
}


static	inline	int	D_get_client_att_water(const int fd)
{
	return	(D_client[fd].att_water_equip);
}


static	inline	int	D_get_client_att_fire(const int fd)
{
	return	(D_client[fd].att_fire_equip);
}


static	inline	int	D_get_client_att_wind(const int fd)
{
	return	(D_client[fd].att_wind_equip);
}


static	inline	int	D_get_client_m_def(const int fd)
{
	return	(D_client[fd].m_def + D_client[fd].m_def_equip + D_client[fd].buff_value[_BUFF_ACTIVE_MAGIC_DEF]);	
}


static	inline	int	D_get_client_att1(const int fd)
{
	return	(D_client[fd].att1 + D_client[fd].att1_equip
			+ D_client[fd].buff_value[_BUFF_ACTIVE_ATT1]);
}


static	inline	int	D_get_client_att2(const int fd)
{
	return	(D_client[fd].att2_equip);
}


static	inline	int	D_get_client_def1(const int fd)
{
	return	(D_client[fd].def1 + D_client[fd].def1_equip
			+ D_client[fd].buff_value[_BUFF_ACTIVE_DEF1]
			- D_client[fd].buff_value[_BUFF_DE_ACTIVE_DEF1]);
}


static	inline	int	D_get_client_def2(const int fd)
{
	return	(D_client[fd].def2_equip);
}


static	inline	int	D_get_client_eva(const int fd)
{
	return	(D_client[fd].eva_equip);
}	


static	inline	int	D_get_client_cri(const int fd)
{
	return	(D_client[fd].cri + D_client[fd].cri_equip + D_client[fd].buff_value[_BUFF_EQUIP_PASSIVE_CRI]);
}


static	inline	int	D_get_client_i_att(const int fd)
{
	return	(D_client[fd].i_att + D_client[fd].i_att_equip + D_client[fd].buff_value[_BUFF_EQUIP_PASSIVE_I_ATT]);
}


static	inline	int	D_get_client_is_def(const int fd)
{
	return	(D_client[fd].is_def + D_client[fd].is_def_equip + D_client[fd].buff_value[_BUFF_EQUIP_PASSIVE_IS_DEF]);
}


static	inline	int	D_get_client_il_def(const int fd)
{
	return	(D_client[fd].il_def + D_client[fd].il_def_equip + D_client[fd].buff_value[_BUFF_EQUIP_PASSIVE_IL_DEF]);
}


static	inline	int	D_get_client_recover_hp(const int fd)
{
	return	(D_client[fd].recover_hp + D_client[fd].recover_hp_equip
			+ D_client[fd].buff_value[_BUFF_PASSIVE_HP_RECOVER]
			- D_client[fd].buff_value[_BUFF_DE_ACTIVE_POISONING] - D_client[fd].buff_value[_BUFF_DE_ACTIVE_FIREPOISONING]);
}


static	inline	int	D_get_client_recover_ap(const int fd)
{
	return	(D_client[fd].recover_ap + D_client[fd].recover_ap_equip
			+ D_client[fd].buff_value[_BUFF_PASSIVE_AP_RECOVER]);
}
	

static	inline	int	D_get_client_att_area(const int fd)
{
	return	(D_client[fd].att_area + D_client[fd].buff_value[_BUFF_EQUIP_PASSIVE_ATT_AREA]);
}


static	inline	int	D_get_client_att_milli_speed(const int fd)
{
	return	((D_client[fd].att_milli_speed)
				/((100 + 0)/100.0f)*((100 + D_client[fd].buff_value[_BUFF_DE_ACTIVE_ATT_SPEED])/100.0f));
}


static	inline	float	D_get_client_move_speed1(const int fd)
{
	return	((D_client[fd].move_speed1)
			/((100 + D_client[fd].buff_value[_BUFF_DE_ACTIVE_MOVE_SPEED])/100)
			*((100 + D_client[fd].buff_value[_BUFF_ACTIVE_MOVE_SPEED])/100));
}


static	inline	float	D_get_client_move_speed2(const int fd)
{
	return	((D_client[fd].move_speed2)
			/((100 + D_client[fd].buff_value[_BUFF_DE_ACTIVE_MOVE_SPEED])/100)
			*((100 + D_client[fd].buff_value[_BUFF_ACTIVE_MOVE_SPEED])/100));
}



//______________________________________________________________
//
static	void	D_make_item(struct item_object *item, const int item_type, const int stack_num, const char *char_name)
{
	item->type	= item_type;

	switch	( D_type_item[item_type].class )
	{
		case	ITEM_CLASS_MONEY:
			item->stack_num		= stack_num;
			break;
		case	ITEM_CLASS_ONEHAND_SWORD:
		case	ITEM_CLASS_TWOHAND_SWORD:
		case	ITEM_CLASS_BOW:
		case	ITEM_CLASS_ARROW:
		case	ITEM_CLASS_HAT:
		case	ITEM_CLASS_JACKET:
		case	ITEM_CLASS_PANTS:
		case	ITEM_CLASS_SHOES:
		case	ITEM_CLASS_SHIELD:
		case	ITEM_CLASS_EARRING:
		case	ITEM_CLASS_LEFT_RING:	
		case	ITEM_CLASS_RIGHT_RING:
			item->durability	= D_type_item[item_type].data[1];	
			U_MakeSerial(item->serial, char_name, item_type);
			break;
		case	ITEM_CLASS_QUEST:
			U_MakeSerial(item->serial, char_name, item_type);
			break;
		case	ITEM_CLASS_RESTORE_POTION:
			item->stack_num		= stack_num;
			break;
		//case	ITEM_CLASS_BUFF_POTION:
		case	ITEM_CLASS_PORTAL:
			U_Printf("ITEM_CLASS_PORTAL\n");
			break;
		case	ITEM_CLASS_RESURRECTION:
			U_Printf("ITEM_CLASS_RESURRECTION\n");
			break;
		//case	ITEM_CLASS_MATERIAL:
		case	ITEM_CLASS_ENCHANT:
			U_MakeSerial(item->serial, char_name, item_type);
			break;
		//case	ITEM_CLASS_TRANSFORM:
		case	ITEM_CLASS_CASH_COSTUME_ONEHAND_SWORD:
		case	ITEM_CLASS_CASH_COSTUME_TWOHAND_SWORD:
		case	ITEM_CLASS_CASH_COSTUME_BOW:
		case	ITEM_CLASS_CASH_COSTUME_SHIELD:
		case	ITEM_CLASS_CASH_COSTUME_HAT:
		case	ITEM_CLASS_CASH_COSTUME_FACE:
		case	ITEM_CLASS_CASH_COSTUME_JACKET:
		case	ITEM_CLASS_CASH_COSTUME_PANTS:
		case	ITEM_CLASS_CASH_COSTUME_ONEPIECE:
		case	ITEM_CLASS_CASH_COSTUME_CLOAK:
		case	ITEM_CLASS_CASH_COSTUME_SHOES:
		case	ITEM_CLASS_CASH_COSTUME_NECKLACE:
			item->use_end_sec	= D_type_item[item_type].data[0];	
			U_MakeSerial(item->serial, char_name, item_type);
			break;
	}
}


static	int	D_get_item_id(void)
{
	int	item_id;


	item_id	= D_item_id_link;

	D_item_id_link	= D_item[item_id].id_next;

	return	(item_id);
}


static	void	D_return_item_id(const int item_id)
{
	D_item[item_id].id_next	= D_item_id_link;
	D_item_id_link		= item_id;
}


static	int	D_set_item_drop(const int item_id, const int channel_link, const int map, const int x, const int y, const int z,
										const struct item_object *item, const char *own_char_name)
{
	D_item[item_id].channel_link	= channel_link;
	D_item[item_id].map		= map;
	D_item[item_id].x		= x + U_GetRand_val(3) -1 + 0.5f;
	D_item[item_id].y		= 0;
	D_item[item_id].z		= z + U_GetRand_val(3) -1 + 0.5f;	

	//check attr
	if( D_map[map].attr[(int)(D_item[item_id].x)][(int)(D_item[item_id].z)] == MAP_ATTR_COLLISION )
		return	(-1);

	memcpy(&(D_item[item_id].item), item, ITEM_DATA_LEN+ITEM_SERIAL_LEN);

	if( own_char_name )
		strncpy(D_item[item_id].own_char_name, own_char_name, CHAR_NAME_LEN+1);
	
	D_item[item_id].block_x		= (((int)(D_item[item_id].x)) / VIEW_BLOCK_TILE) + 2; 
	D_item[item_id].drop_time	= G_Sec;

	return	(0);
}


//______________________________________________________________
//
static	int	D_check_char_attack_type(const int fd, const int attack_type, float *add_att2, int *add_i_att)
{
	*add_att2		= 0;
	*add_i_att		= 0;

	switch( attack_type )
	{
		case	_ATTACK_JUMP:
			switch( D_type_item[D_client[fd].item_slot[EQUIP_SLOT_WEAPON1].type].class )
			{
				case	0:
					break;
				case	ITEM_CLASS_ONEHAND_SWORD:
					*add_att2	= D_get_client_att2(fd)*-0.2f;
					break;
				case	ITEM_CLASS_TWOHAND_SWORD:
					*add_att2	= D_get_client_att2(fd)*-0.5f;
					break;
				case	ITEM_CLASS_BOW:
					*add_att2	= D_get_client_att2(fd)*-0.5f;
					break;
			}
			break;
		case	_ATTACK_NORMAL:
			break;
		case	_ATTACK_COMBO1:
			if( D_client[fd].last_attack_type != _ATTACK_NORMAL )
			{
				U_Printf("[DB]Error fd(%d) last_attack_type(%d) attack_type(%d)\n",
								fd, D_client[fd].last_attack_type, attack_type);	
				return	(-1);
			}
			switch( D_type_item[D_client[fd].item_slot[EQUIP_SLOT_WEAPON1].type].class )
			{
				case	0:
					break;
				case	ITEM_CLASS_ONEHAND_SWORD:
					*add_att2	= D_get_client_att2(fd)*0.1f;
					break;
				case	ITEM_CLASS_TWOHAND_SWORD:
					*add_att2	= D_get_client_att2(fd)*0.1f;
					break;
				case	ITEM_CLASS_BOW:
					break;
			}
			break;
		case	_ATTACK_COMBO2:
			if( D_client[fd].last_attack_type != _ATTACK_COMBO1 )
			{
				U_Printf("[DB]Error fd(%d) last_attack_type(%d) attack_type(%d)\n",
								fd, D_client[fd].last_attack_type, attack_type);	
				return	(-1);
			}
			switch( D_type_item[D_client[fd].item_slot[EQUIP_SLOT_WEAPON1].type].class )
			{
				case	0:
					break;
				case	ITEM_CLASS_ONEHAND_SWORD:
					*add_att2
						= D_get_client_att2(fd)*0.2f + D_client[fd].buff_value[_BUFF_EQUIP_PASSIVE_COND_COMBO2];
					*add_i_att	= 90;
					break;
				case	ITEM_CLASS_TWOHAND_SWORD:
					*add_att2
						= D_get_client_att2(fd)*0.2f + D_client[fd].buff_value[_BUFF_EQUIP_PASSIVE_COND_COMBO2];
					*add_i_att	= 90;
					break;
				case	ITEM_CLASS_BOW:
					break;
					*add_att2
						= D_get_client_att2(fd)*0.1f + D_client[fd].buff_value[_BUFF_EQUIP_PASSIVE_COND_COMBO2];
					break;
			}
			break;
		case	_ATTACK_PIERCE:
			switch( D_type_item[D_client[fd].item_slot[EQUIP_SLOT_WEAPON1].type].class )
			{
				case	0:
					break;
				case	ITEM_CLASS_ONEHAND_SWORD:
					*add_att2	= D_get_client_att2(fd)*0.2f;
					*add_i_att	= 90;
					break;
				case	ITEM_CLASS_TWOHAND_SWORD:
					*add_att2	= D_get_client_att2(fd)*0.2f;
					*add_i_att	= 90;
					break;
				case	ITEM_CLASS_BOW:
					*add_att2	= D_get_client_att2(fd)*-3.0f;
					*add_i_att	= 90;
					break;
			}
			break;
	}

	return	(0);
}


static	int	D_compute_damage(const int channel_link, const int type, 
						const int att_id, const int def_id, const int add_att1, const float add_att2)
{
	int	damage;
	float	att1, att2, def1, def2;


	switch( type )
	{
		case	CHAR_ATTACK_MOB:
			att1			= D_get_client_att1(att_id);
			att2			= D_get_client_att2(att_id);
			def1			= D_get_mob_def1(channel_link, def_id);
			def2			= D_get_mob_def2(channel_link, def_id);
			break;

		case	MOB_ATTACK_CHAR:
			att1			= D_get_mob_att1(channel_link, att_id);
			att2			= D_get_mob_att2(channel_link, att_id);
			def1			= D_get_client_def1(def_id);
			def2			= D_get_client_def2(def_id);
			break;
		default:
			att1			= 0;
			att2			= 0;
			def1			= 0;
			def2			= 0;
	}//end switch

	damage		= (att1 + add_att1 - def1) * ((att2 + add_att2 +100)/100) * ((100-def2)/100);
	
	if( damage < 6 )
		damage	= U_GetRand_val(5) + 1;	
	else
		damage	+= damage * (U_GetRand_val(21)-10)/100.0f;

	return	(damage);
}


	
static	int	D_check_evasion(const int channel_link, const int type, const int att_id, const int def_id) 
{
	int	eva;

	
	switch( type )
	{
		case	CHAR_ATTACK_MOB:
			eva		= D_get_mob_eva(channel_link, def_id);
			break;

		case	MOB_ATTACK_CHAR:
			eva		= D_get_client_eva(def_id);
			break;
		default:
			eva		= 0;
	}
	
	if( eva > U_GetRand_val(100) )
		return	(1);

	return	(0);		
}


static	int	D_check_critical(const int channel_link, const int type, const int att_id, const int def_id, const int add_cri)
{
	int	cri, att_level, def_level;

	
	switch( type )
	{
		case	CHAR_ATTACK_MOB:
			cri		= D_get_client_cri(att_id);
			att_level	= D_client[att_id].level;
			def_level	= D_mob[def_id][channel_link].level;
			break;

		case	MOB_ATTACK_CHAR:
			cri		= D_get_mob_cri(channel_link, att_id);
			att_level	= D_mob[att_id][channel_link].level;
			def_level	= D_client[def_id].level;
			break;
		default:	
			cri		= 0;
			att_level	= 0;
			def_level	= 0;
	}
	
	if( (cri + add_cri + att_level - def_level) > U_GetRand_val(100) )	
		return	(1);

	return	(0);		
}

	
static	int	D_check_impact(const int channel_link, const int type, const int att_id, const int def_id, const int add_i_att)
{
	int	i_att, att_level, i_def, def_level;


	switch( type )
	{
		case	CHAR_ATTACK_MOB:
			i_att		= D_get_client_i_att(att_id);
			att_level	= D_client[att_id].level;
			
			if( D_get_client_att_area(att_id) < 3 )
				i_def		= D_get_mob_is_def(channel_link, def_id);
			else
				i_def		= D_get_mob_il_def(channel_link, def_id);	
			
			def_level	= D_mob[def_id][channel_link].level;	
			break;

		case	MOB_ATTACK_CHAR:
			i_att		= D_get_mob_i_att(channel_link, att_id);
			att_level	= D_mob[att_id][channel_link].level;	
			
			if( D_get_mob_att_area(channel_link, att_id) < 3 )
				i_def		= D_get_client_is_def(def_id);
			else
				i_def		= D_get_client_il_def(def_id);	
			
			def_level	= D_client[def_id].level;	
			break;
		default:
			i_att		= 0;
			att_level	= 0;
			i_def		= 0;
			def_level	= 0;
			
	}

	if( (i_att + add_i_att - i_def + att_level - def_level) > U_GetRand_val(100) )
		return	(1);

	return	(0);
}


static	int	D_check_stun(const int channel_link, const int type, const int att_id, const int def_id, const int add_stun)
{
	int	att_level, def_level;


	switch( type )
	{
		case	CHAR_ATTACK_MOB:
			att_level	= D_client[att_id].level;
			def_level	= D_mob[def_id][channel_link].level;	
			break;
		case	MOB_ATTACK_CHAR:
			att_level	= D_mob[att_id][channel_link].level;	
			def_level	= D_client[def_id].level;	
			break;
		default:
			att_level	= 0;
			def_level	= 0;
	}
	
	if( (add_stun + att_level - def_level) > U_GetRand_val(100) )
		return	(1);
		
	return	(0);
}
				

static	int	D_check_provocation(const int channel_link, const int type, const int att_id, const int def_id, const int add_provocation)
{
	int	att_level, def_level;


	switch( type )
	{
		case	CHAR_ATTACK_MOB:
			att_level	= D_client[att_id].level;
			def_level	= D_mob[def_id][channel_link].level;	
			break;
		case	MOB_ATTACK_CHAR:
			att_level	= D_mob[att_id][channel_link].level;	
			def_level	= D_client[def_id].level;	
			break;
		default:
			att_level	= 0;
			def_level	= 0;
	}
	
	if( (add_provocation + att_level - def_level) > U_GetRand_val(100) )
		return	(1);
		
	return	(0);
}



//_________________________________________________________________________________
//
static	void	D_set_client_enchant(const int fd, const int option, const int option_data)
{
	switch( option )
	{
		case	0:
			return;
		case	ENCHANT_OPTION_ATT1:
			D_client[fd].att1_equip		+= option_data;
			return;
		case	ENCHANT_OPTION_ATT2:
			D_client[fd].att2_equip		+= option_data;
			return;
		case	ENCHANT_OPTION_CRI:
			D_client[fd].cri_equip		+= option_data;
			return;
		case	ENCHANT_OPTION_I_ATT:
			D_client[fd].i_att_equip	+= option_data;
			return;
		case	ENCHANT_OPTION_ATT_LAND:
			D_client[fd].att_land_equip	+= option_data;
			return;
		case	ENCHANT_OPTION_ATT_WATER:
			D_client[fd].att_water_equip	+= option_data;
			return;
		case	ENCHANT_OPTION_ATT_FIRE:
			D_client[fd].att_fire_equip	+= option_data;
			return;
		case	ENCHANT_OPTION_ATT_WIND:
			D_client[fd].att_wind_equip	+= option_data;
			return;
		case	ENCHANT_OPTION_DEF1:
			D_client[fd].def1_equip		+= option_data;
			return;
		case	ENCHANT_OPTION_DEF2:
			D_client[fd].def2_equip		+= option_data;
			return;
		case	ENCHANT_OPTION_EVA:
			D_client[fd].eva_equip		+= option_data;
			return;
		case	ENCHANT_OPTION_IL_DEF:
			D_client[fd].il_def_equip	+= option_data;
			return;
		case	ENCHANT_OPTION_IS_DEF:
			D_client[fd].is_def_equip	+= option_data;
			return;
		case	ENCHANT_OPTION_HP_MAX:
			D_client[fd].hp_max_equip	+= option_data;
			return;
		case	ENCHANT_OPTION_AP_MAX:
			D_client[fd].ap_max_equip	+= option_data;
			return;
		case	ENCHANT_OPTION_RECOVER_HP:
			D_client[fd].recover_hp_equip	+= option_data;
			return;
		case	ENCHANT_OPTION_RECOVER_AP:
			D_client[fd].recover_ap_equip	+= option_data;
			return;
	}
}


static	void	D_set_client_clear_enchant(const int fd, const int option, const int option_data)
{
	switch( option )
	{
		case	0:
			return;
		case	ENCHANT_OPTION_ATT1:
			D_client[fd].att1_equip		-= option_data;
			return;
		case	ENCHANT_OPTION_ATT2:
			D_client[fd].att2_equip		-= option_data;
			return;
		case	ENCHANT_OPTION_CRI:
			D_client[fd].cri_equip		-= option_data;
			return;
		case	ENCHANT_OPTION_I_ATT:
			D_client[fd].i_att_equip	-= option_data;
			return;
		case	ENCHANT_OPTION_ATT_LAND:
			D_client[fd].att_land_equip	-= option_data;
			return;
		case	ENCHANT_OPTION_ATT_WATER:
			D_client[fd].att_water_equip	-= option_data;
			return;
		case	ENCHANT_OPTION_ATT_FIRE:
			D_client[fd].att_fire_equip	-= option_data;
			return;
		case	ENCHANT_OPTION_ATT_WIND:
			D_client[fd].att_wind_equip	-= option_data;
			return;
		case	ENCHANT_OPTION_DEF1:
			D_client[fd].def1_equip		-= option_data;
			return;
		case	ENCHANT_OPTION_DEF2:
			D_client[fd].def2_equip		-= option_data;
			return;
		case	ENCHANT_OPTION_EVA:
			D_client[fd].eva_equip		-= option_data;
			return;
		case	ENCHANT_OPTION_IL_DEF:
			D_client[fd].il_def_equip	-= option_data;
			return;
		case	ENCHANT_OPTION_IS_DEF:
			D_client[fd].is_def_equip	-= option_data;
			return;
		case	ENCHANT_OPTION_HP_MAX:
			D_client[fd].hp_max_equip	-= option_data;
			return;
		case	ENCHANT_OPTION_AP_MAX:
			D_client[fd].ap_max_equip	-= option_data;
			return;
		case	ENCHANT_OPTION_RECOVER_HP:
			D_client[fd].recover_hp_equip	-= option_data;
			return;
		case	ENCHANT_OPTION_RECOVER_AP:
			D_client[fd].recover_ap_equip	-= option_data;
			return;
	}
}


static	int	D_set_client_item(const int fd, const int item_slot, const struct item_object *item, int *change_buff)
{
	int	i, stack_num, equip_item_type, item_class;


	//money
	if( D_type_item[item->type].class == ITEM_CLASS_MONEY )
	{
		if( (D_client[fd].money + item->stack_num) < 0 )
			D_client[fd].money	= D_max_int;			
		else
			D_client[fd].money	+= item->stack_num;		

		return	(0);
	}

	if( item_slot >= EQUIP_SLOT_CNT )
	{
		//stack
		if( D_client[fd].item_slot[item_slot].type ) 
		{
			if( (D_client[fd].item_slot[item_slot].type != item->type) || (D_type_item[item->type].stack_num == 0) )
			{
				U_Printf("[DB]Error item_type(%d) item_slot_type(%d) stack_num(%d)\n",
						item->type, D_client[fd].item_slot[item_slot].type, D_type_item[item->type].stack_num);
				return	(-1);
			}
			
			if( (stack_num = D_client[fd].item_slot[item_slot].stack_num + item->stack_num) > D_type_item[item->type].stack_num )
				stack_num	= D_type_item[item->type].stack_num;

			D_client[fd].item_slot[item_slot].stack_num	= stack_num;
			return	(0);
		}
		
		//set item
		memcpy(D_client[fd].item_slot + item_slot, item, ITEM_DATA_LEN+ITEM_SERIAL_LEN);	
		return	(0);
	}

	//check equip_id
	if( D_type_item[item->type].equip_id != item_slot )
	{
		U_Printf("[DB]Error item_type(%d) equip_id(%d) bad item_slot(%d)\n", item->type, D_type_item[item->type].equip_id, item_slot);
		return	(-1);
	}
	
	//check level
	if( D_type_item[item->type].level > D_client[fd].level )
	{
		U_Printf("[DB]Error item_type(%d) level(%d) bad level(%d)\n", item->type, D_type_item[item->type].level, D_client[fd].level);
		return	(-1);
	}		


	switch( (item_class = D_type_item[item->type].class) )
	{
		case	ITEM_CLASS_ONEHAND_SWORD:
			equip_item_type	= D_client[fd].item_slot[EQUIP_SLOT_WEAPON2].type;
			if( equip_item_type && (D_type_item[equip_item_type].class != ITEM_CLASS_SHIELD) )
			{
				U_Printf("[DB]Error item_type(%d) equip_item_type(%d)\n", item->type, equip_item_type);
				return	(-1);
			}
			D_client[fd].cri_equip		+= D_type_item[item->type].data[6];
			D_client[fd].i_att_equip	+= D_type_item[item->type].data[7];
			D_client[fd].att1_equip		+= D_type_item[item->type].data[3];
			D_client[fd].att2_equip		+= D_type_item[item->type].data[9];
			for( i=0; i<D_type_item[item->type].data[8]; i++ )
				D_set_client_enchant(fd, D_type_item[item->enchant[i]].data[2], D_type_item[item->enchant[i]].data[6]);
			break;
		case	ITEM_CLASS_TWOHAND_SWORD:
			equip_item_type	= D_client[fd].item_slot[EQUIP_SLOT_WEAPON2].type;
			if( equip_item_type )
			{
				U_Printf("[DB]Error item_type(%d) equip_item_type(%d)\n", item->type, equip_item_type);
				return	(-1);
			}
			D_client[fd].cri_equip		+= D_type_item[item->type].data[6];
			D_client[fd].i_att_equip	+= D_type_item[item->type].data[7];
			D_client[fd].att1_equip		+= D_type_item[item->type].data[3];
			D_client[fd].att2_equip		+= D_type_item[item->type].data[9];
			for( i=0; i<D_type_item[item->type].data[8]; i++ )
				D_set_client_enchant(fd, D_type_item[item->enchant[i]].data[2], D_type_item[item->enchant[i]].data[6]);
			break;
		case	ITEM_CLASS_BOW:
			equip_item_type	= D_client[fd].item_slot[EQUIP_SLOT_WEAPON2].type;
			if( equip_item_type && (D_type_item[equip_item_type].class != ITEM_CLASS_ARROW) )
			{
				U_Printf("[DB]Error item_type(%d) equip_item_type(%d)\n", item->type, equip_item_type);
				return	(-1);
			}
			D_client[fd].att_milli_speed	= D_type_item[item->type].data[4];
			D_client[fd].att_area		= D_type_item[item->type].data[5];
			D_client[fd].cri_equip		+= D_type_item[item->type].data[6];
			D_client[fd].i_att_equip	+= D_type_item[item->type].data[7];
			D_client[fd].att1_equip		+= D_type_item[item->type].data[3];
			D_client[fd].att2_equip		+= D_type_item[item->type].data[9];
			for( i=0; i<D_type_item[item->type].data[8]; i++ )
				D_set_client_enchant(fd, D_type_item[item->enchant[i]].data[2], D_type_item[item->enchant[i]].data[6]);
			break;
		case	ITEM_CLASS_ARROW:
			equip_item_type	= D_client[fd].item_slot[EQUIP_SLOT_WEAPON1].type;
			if( equip_item_type && (D_type_item[equip_item_type].class != ITEM_CLASS_BOW) )
			{
				U_Printf("[DB]Error item_type(%d) equip_item_type(%d)\n", item->type, equip_item_type);
				return	(-1);
			}
			break;
		case	ITEM_CLASS_HAT:
		case	ITEM_CLASS_JACKET:
		case	ITEM_CLASS_PANTS:
		case	ITEM_CLASS_SHOES:
			D_client[fd].is_def_equip	+= D_type_item[item->type].data[4];
			D_client[fd].il_def_equip	+= D_type_item[item->type].data[5];
			D_client[fd].def1_equip		+= D_type_item[item->type].data[3];
			D_client[fd].def2_equip		+= D_type_item[item->type].data[8];
			D_client[fd].m_def_equip	+= D_type_item[item->type].data[9];
			for( i=0; i<D_type_item[item->type].data[6]; i++ )
				D_set_client_enchant(fd, D_type_item[item->enchant[i]].data[2], D_type_item[item->enchant[i]].data[6]);
			break;
		case	ITEM_CLASS_SHIELD:
			equip_item_type	= D_client[fd].item_slot[EQUIP_SLOT_WEAPON1].type;
			if( equip_item_type && (D_type_item[equip_item_type].class != ITEM_CLASS_ONEHAND_SWORD) )
			{
				U_Printf("[DB]Error item_type(%d) equip_item_type(%d)\n", item->type, equip_item_type);
				return	(-1);
			}
			D_client[fd].is_def_equip	+= D_type_item[item->type].data[4];
			D_client[fd].il_def_equip	+= D_type_item[item->type].data[5];
			D_client[fd].def1_equip		+= D_type_item[item->type].data[3];
			D_client[fd].def2_equip		+= D_type_item[item->type].data[8];
			D_client[fd].m_def_equip	+= D_type_item[item->type].data[9];
			for( i=0; i<D_type_item[item->type].data[6]; i++ )
				D_set_client_enchant(fd, D_type_item[item->enchant[i]].data[2], D_type_item[item->enchant[i]].data[6]);
			break;
		case	ITEM_CLASS_EARRING:
		case	ITEM_CLASS_LEFT_RING:
		case	ITEM_CLASS_RIGHT_RING:
			for( i=0; i<D_type_item[item->type].data[3]; i++ )
				D_set_client_enchant(fd, D_type_item[item->enchant[i]].data[2], D_type_item[item->enchant[i]].data[6]);
			break;
		case	ITEM_CLASS_CASH_COSTUME_ONEHAND_SWORD:
		case	ITEM_CLASS_CASH_COSTUME_TWOHAND_SWORD:
		case	ITEM_CLASS_CASH_COSTUME_BOW:
		case	ITEM_CLASS_CASH_COSTUME_SHIELD:
		case	ITEM_CLASS_CASH_COSTUME_HAT:
		case	ITEM_CLASS_CASH_COSTUME_FACE:
		case	ITEM_CLASS_CASH_COSTUME_JACKET:
			equip_item_type	= D_client[fd].item_slot[D_type_item[item->type].data[1]].type;
			if( D_type_item[item->type].data[2] && (D_type_item[item->type].data[2] != D_type_item[equip_item_type].class) )
			{
				U_Printf("[DB]Error item_type(%d) item_slot(%d) equip_item_type(%d)\n",
						item->type, D_type_item[item->type].data[1], equip_item_type);
				return	(-1);
			}
			if( (D_type_item[item->type].data[3] != (MAX_CHAR_SEX+1)) && (D_type_item[item->type].data[3] != D_client[fd].sex) )	
			{
				U_Printf("[DB]Error item_type(%d) char_sex(%d)\n", item->type, D_client[fd].sex);
				return	(-1);
			}
			break;
		case	ITEM_CLASS_CASH_COSTUME_PANTS:
			equip_item_type	= D_client[fd].item_slot[EQUIP_SLOT_COAT_COSTUME].type;
			if( equip_item_type && (D_type_item[equip_item_type].class == ITEM_CLASS_CASH_COSTUME_ONEPIECE) )
			{
				U_Printf("[DB]Error item_type(%d) equip_item_type(%d)\n", item->type, equip_item_type);
				return	(-1);
			}
			equip_item_type	= D_client[fd].item_slot[D_type_item[item->type].data[1]].type;
			if( D_type_item[item->type].data[2] && (D_type_item[item->type].data[2] != D_type_item[equip_item_type].class) )
			{
				U_Printf("[DB]Error item_type(%d) item_slot(%d) equip_item_type(%d)\n",
						item->type, D_type_item[item->type].data[1], equip_item_type);
				return	(-1);
			}
			if( (D_type_item[item->type].data[3] != (MAX_CHAR_SEX+1)) && (D_type_item[item->type].data[3] != D_client[fd].sex) )	
			{
				U_Printf("[DB]Error item_type(%d) char_sex(%d)\n", item->type, D_client[fd].sex);
				return	(-1);
			}
			break;
		case	ITEM_CLASS_CASH_COSTUME_ONEPIECE:
			equip_item_type	= D_client[fd].item_slot[EQUIP_SLOT_PANTS_COSTUME].type;
			if( equip_item_type )
			{
				U_Printf("[DB]Error item_type(%d) equip_item_type(%d)\n", item->type, equip_item_type);
				return	(-1);
			}
			equip_item_type	= D_client[fd].item_slot[D_type_item[item->type].data[1]].type;
			if( D_type_item[item->type].data[2] && (D_type_item[item->type].data[2] != D_type_item[equip_item_type].class) )
			{
				U_Printf("[DB]Error item_type(%d) item_slot(%d) equip_item_type(%d)\n",
						item->type, D_type_item[item->type].data[1], equip_item_type);
				return	(-1);
			}
			if( (D_type_item[item->type].data[3] != (MAX_CHAR_SEX+1)) && (D_type_item[item->type].data[3] != D_client[fd].sex) )	
			{
				U_Printf("[DB]Error item_type(%d) char_sex(%d)\n", item->type, D_client[fd].sex);
				return	(-1);
			}
			break;
		case	ITEM_CLASS_CASH_COSTUME_CLOAK:
		case	ITEM_CLASS_CASH_COSTUME_SHOES:
		case	ITEM_CLASS_CASH_COSTUME_NECKLACE:
			equip_item_type	= D_client[fd].item_slot[D_type_item[item->type].data[1]].type;
			if( D_type_item[item->type].data[2] && (D_type_item[item->type].data[2] != D_type_item[equip_item_type].class) )
			{
				U_Printf("[DB]Error item_type(%d) item_slot(%d) equip_item_type(%d)\n",
						item->type, D_type_item[item->type].data[1], equip_item_type);
				return	(-1);
			}
			if( (D_type_item[item->type].data[3] != (MAX_CHAR_SEX+1)) && (D_type_item[item->type].data[3] != D_client[fd].sex) )	
			{
				U_Printf("[DB]Error item_type(%d) char_sex(%d)\n", item->type, D_client[fd].sex);
				return	(-1);
			}
			break;
	}
	
	//set equip passive buff
	for( i=0; i<D_equip_passive_char_skill[item_class].skill_cnt; i++ )
	{
		if( D_client[fd].skill_slot[D_equip_passive_char_skill[item_class].skill_type[i] -1] )
			D_set_client_passive_buff(fd, D_equip_passive_char_skill[item_class].skill_type[i], change_buff);
	}

	//set item	
	memcpy(D_client[fd].item_slot + item_slot, item, ITEM_DATA_LEN+ITEM_SERIAL_LEN);	
	return	(0);
}



static	void	D_set_client_clear_item(const int fd, const int item_slot, int *costume_item_slot, int *change_buff)
{
	struct	item_object	*item;
	int			item_class, i;
	

	*costume_item_slot	= -1;

	if( D_client[fd].item_slot[item_slot].type == 0 )
		return;

	if( item_slot >= EQUIP_SLOT_CNT )
	{
		//clear item
		memset(D_client[fd].item_slot + item_slot, 0, ITEM_DATA_LEN+ITEM_SERIAL_LEN);	
		return;
	}

	item	= D_client[fd].item_slot + item_slot;

	switch( (item_class = D_type_item[item->type].class) )
	{
		case	ITEM_CLASS_ONEHAND_SWORD:
			D_client[fd].cri_equip		-= D_type_item[item->type].data[6];
			D_client[fd].i_att_equip	-= D_type_item[item->type].data[7];
			D_client[fd].att1_equip		-= D_type_item[item->type].data[3];
			D_client[fd].att2_equip		-= D_type_item[item->type].data[9];
			for( i=0; i<D_type_item[item->type].data[8]; i++ )
				D_set_client_clear_enchant(fd, D_type_item[item->enchant[i]].data[2], D_type_item[item->enchant[i]].data[6]);
			*costume_item_slot		= EQUIP_SLOT_WEAPON1_COSTUME;
			break;
		case	ITEM_CLASS_TWOHAND_SWORD:
			D_client[fd].cri_equip		-= D_type_item[item->type].data[6];
			D_client[fd].i_att_equip	-= D_type_item[item->type].data[7];
			D_client[fd].att1_equip		-= D_type_item[item->type].data[3];
			D_client[fd].att2_equip		-= D_type_item[item->type].data[9];
			for( i=0; i<D_type_item[item->type].data[8]; i++ )
				D_set_client_clear_enchant(fd, D_type_item[item->enchant[i]].data[2], D_type_item[item->enchant[i]].data[6]);
			*costume_item_slot	= EQUIP_SLOT_WEAPON1_COSTUME;
			break;
		case	ITEM_CLASS_BOW:
			D_client[fd].att_milli_speed	= BASE_CHAR_MILLI_DELAY_ATTACK;
			D_client[fd].att_area		= BASE_CHAR_AREA_ATTACK;
			D_client[fd].cri_equip		-= D_type_item[item->type].data[6];
			D_client[fd].i_att_equip	-= D_type_item[item->type].data[7];
			D_client[fd].att1_equip		-= D_type_item[item->type].data[3];
			D_client[fd].att2_equip		-= D_type_item[item->type].data[9];
			for( i=0; i<D_type_item[item->type].data[8]; i++ )
				D_set_client_clear_enchant(fd, D_type_item[item->enchant[i]].data[2], D_type_item[item->enchant[i]].data[6]);
			*costume_item_slot	= EQUIP_SLOT_WEAPON1_COSTUME;
			break;
		case	ITEM_CLASS_ARROW:
			*costume_item_slot	= EQUIP_SLOT_WEAPON2_COSTUME;
			break;
		case	ITEM_CLASS_HAT:
			D_client[fd].is_def_equip	-= D_type_item[item->type].data[4];
			D_client[fd].il_def_equip	-= D_type_item[item->type].data[5];
			D_client[fd].def1_equip		-= D_type_item[item->type].data[3];
			D_client[fd].def2_equip		-= D_type_item[item->type].data[8];
			D_client[fd].m_def_equip	-= D_type_item[item->type].data[9];
			for( i=0; i<D_type_item[item->type].data[6]; i++ )
				D_set_client_clear_enchant(fd, D_type_item[item->enchant[i]].data[2], D_type_item[item->enchant[i]].data[6]);
			*costume_item_slot	= EQUIP_SLOT_HEAD_COSTUME;
			break;
		case	ITEM_CLASS_JACKET:
			D_client[fd].is_def_equip	-= D_type_item[item->type].data[4];
			D_client[fd].il_def_equip	-= D_type_item[item->type].data[5];
			D_client[fd].def1_equip		-= D_type_item[item->type].data[3];
			D_client[fd].def2_equip		-= D_type_item[item->type].data[8];
			D_client[fd].m_def_equip	-= D_type_item[item->type].data[9];
			for( i=0; i<D_type_item[item->type].data[6]; i++ )
				D_set_client_clear_enchant(fd, D_type_item[item->enchant[i]].data[2], D_type_item[item->enchant[i]].data[6]);
			*costume_item_slot	= EQUIP_SLOT_COAT_COSTUME;
			break;
		case	ITEM_CLASS_PANTS:
			D_client[fd].is_def_equip	-= D_type_item[item->type].data[4];
			D_client[fd].il_def_equip	-= D_type_item[item->type].data[5];
			D_client[fd].def1_equip		-= D_type_item[item->type].data[3];
			D_client[fd].def2_equip		-= D_type_item[item->type].data[8];
			D_client[fd].m_def_equip	-= D_type_item[item->type].data[9];
			for( i=0; i<D_type_item[item->type].data[6]; i++ )
				D_set_client_clear_enchant(fd, D_type_item[item->enchant[i]].data[2], D_type_item[item->enchant[i]].data[6]);
			*costume_item_slot	= EQUIP_SLOT_PANTS_COSTUME;
			break;
		case	ITEM_CLASS_SHOES:
			D_client[fd].is_def_equip	-= D_type_item[item->type].data[4];
			D_client[fd].il_def_equip	-= D_type_item[item->type].data[5];
			D_client[fd].def1_equip		-= D_type_item[item->type].data[3];
			D_client[fd].def2_equip		-= D_type_item[item->type].data[8];
			D_client[fd].m_def_equip	-= D_type_item[item->type].data[9];
			for( i=0; i<D_type_item[item->type].data[6]; i++ )
				D_set_client_clear_enchant(fd, D_type_item[item->enchant[i]].data[2], D_type_item[item->enchant[i]].data[6]);
			*costume_item_slot	= EQUIP_SLOT_SHOES_COSTUME;
			break;
		case	ITEM_CLASS_SHIELD:
			D_client[fd].is_def_equip	-= D_type_item[item->type].data[4];
			D_client[fd].il_def_equip	-= D_type_item[item->type].data[5];
			D_client[fd].def1_equip		-= D_type_item[item->type].data[3];
			D_client[fd].def2_equip		-= D_type_item[item->type].data[8];
			D_client[fd].m_def_equip	-= D_type_item[item->type].data[9];
			for( i=0; i<D_type_item[item->type].data[6]; i++ )
				D_set_client_clear_enchant(fd, D_type_item[item->enchant[i]].data[2], D_type_item[item->enchant[i]].data[6]);
			*costume_item_slot	= EQUIP_SLOT_WEAPON2_COSTUME;
			break;
		case	ITEM_CLASS_EARRING:
		case	ITEM_CLASS_LEFT_RING:
		case	ITEM_CLASS_RIGHT_RING:
			for( i=0; i<D_type_item[item->type].data[3]; i++ )
				D_set_client_clear_enchant(fd, D_type_item[item->enchant[i]].data[2], D_type_item[item->enchant[i]].data[6]);
			break;
	}
	
	//clear equip passive buff
	for( i=0; i<D_equip_passive_char_skill[item_class].skill_cnt; i++ )
	{
		if( D_client[fd].skill_slot[D_equip_passive_char_skill[item_class].skill_type[i] -1] )
			D_set_client_clear_passive_buff(fd, D_equip_passive_char_skill[item_class].skill_type[i], change_buff);
	}
	
	//clear item
	memset(D_client[fd].item_slot + item_slot, 0, ITEM_DATA_LEN+ITEM_SERIAL_LEN);	
}


static	void	D_set_client_passive_buff(const int fd, const int skill_type, int *change_buff)
{
	int			i, buff_cnt, buff_serise[MAX_SKILL_ORDER_CNT], buff_value[MAX_SKILL_ORDER_CNT], buff_sec[MAX_SKILL_ORDER_CNT];
	struct	skill_ability	ability;


	//get skill ability	
	D_get_skill_ability(skill_type, D_client[fd].skill_slot[skill_type-1], &buff_cnt, buff_serise, buff_value, buff_sec, &ability);

	//set buff
	for( i=0; i<buff_cnt; i++ )
	{
		D_SetClient_buff(fd, buff_serise[i], buff_value[i], buff_sec[i]);
		change_buff[buff_serise[i]]	= 1;
	}
}


static	void	D_set_client_clear_passive_buff(const int fd, const int skill_type, int *change_buff)
{
	int			i, buff_cnt, buff_serise[MAX_SKILL_ORDER_CNT], buff_value[MAX_SKILL_ORDER_CNT], buff_sec[MAX_SKILL_ORDER_CNT];
	struct	skill_ability	ability;


	//get skill ability	
	D_get_skill_ability(skill_type, D_client[fd].skill_slot[skill_type-1], &buff_cnt, buff_serise, buff_value, buff_sec, &ability);

	//clear buff
	for( i=0; i<buff_cnt; i++ )
	{
		D_SetClient_buff_clear(fd, buff_serise[i]);
		change_buff[buff_serise[i]]	= 1;
	}
}


static	void	D_set_client_level_up(const int fd)
{
	++(D_client[fd].level);

	D_client[fd].hp_max		= D_level[D_client[fd].level].hp_max;
	D_client[fd].ap_max		= D_level[D_client[fd].level].ap_max;
	D_client[fd].att1		= D_level[D_client[fd].level].att1;
	D_client[fd].def1		= D_level[D_client[fd].level].def1;
	D_client[fd].is_def		= D_level[D_client[fd].level].is_def;
	D_client[fd].il_def		= D_level[D_client[fd].level].il_def;
	D_client[fd].i_att		= D_level[D_client[fd].level].i_att;
	D_client[fd].m_def		= D_level[D_client[fd].level].m_def;
	D_client[fd].cri		= D_level[D_client[fd].level].cri;
	D_client[fd].recover_hp		= D_level[D_client[fd].level].recover_hp;
	D_client[fd].recover_ap		= D_level[D_client[fd].level].recover_ap;
	D_client[fd].move_speed1	= D_level[D_client[fd].level].move_speed1;	
	D_client[fd].move_speed2	= D_level[D_client[fd].level].move_speed2;	

	if( (D_client[fd].skill_point += 3) < 0 )
		D_client[fd].skill_point	= D_max_int;	

	D_client[fd].hp		= D_get_client_hp_max(fd);
	D_client[fd].ap		= D_get_client_ap_max(fd);
}


//_________________________________________________________________________________
//
static	void	D_get_skill_ability(const int skill_type, const int skill_level,
						int *buff_cnt, int *buff_serise, int *buff_value, int *buff_sec, struct skill_ability *ability)
{
	int			i;
	struct	type_char_skill	*skill;


	if( skill_level )
		skill	= &(D_type_char_skill[skill_level][skill_type]);
	else
		skill	= (struct type_char_skill *)(&(D_type_mob_skill[skill_type]));

	*buff_cnt			= 0;
	ability->damage_type		= _DAMAGE_NOTING;
	ability->attr_attack		= 0;
	ability->attr_damage		= 0;
	ability->add_att1		= 0;
	ability->add_att2		= 0.0f;
	ability->add_i_att		= 0;
	ability->add_provocation	= 0;
	ability->hp_heal		= 0;

	for( i=0; i<MAX_SKILL_ORDER_CNT; i++ ) 
	{
		switch( skill->order[i].function )
		{
			case	0:
				break;
			case	4://att1 active buff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_ACTIVE_ATT1;
				break;
			case	12://att2 transform passive buff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_TRANSFORM_PASSIVE_ATT2;
				break;
			case	14://cri equip passive buff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_EQUIP_PASSIVE_CRI;
				break;
			case	20://i_att equip passive buff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_EQUIP_PASSIVE_I_ATT;
				break;
			case	26://is_def equip passive buff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_EQUIP_PASSIVE_IS_DEF;
				break;
			case	32://il_def equip passive buff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_EQUIP_PASSIVE_IL_DEF;
				break;
			case	40://def1 active buff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_ACTIVE_DEF1;
				break;
			case	41://def1 active debuff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_DE_ACTIVE_DEF1;
				break;
			case	52://move speed active buff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_ACTIVE_MOVE_SPEED;
				break;
			case	53://move speed active debuff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_DE_ACTIVE_MOVE_SPEED;
				break;
			case	56://att speed equip passive buff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_EQUIP_PASSIVE_ATT_SPEED;
				break;
			case	59://att speed active debuff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_DE_ACTIVE_ATT_SPEED;
				break;
			case	62://attack area equip passive buff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_EQUIP_PASSIVE_ATT_AREA;
				break;
			case	69://hp recover passive buff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_PASSIVE_HP_RECOVER;
				break;
			case	75://ap recover passive buff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_PASSIVE_AP_RECOVER;
				break;
			case	81://hp max passive buff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_PASSIVE_HP_MAX;
				break;
			case	84://hp max transform passive buff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_TRANSFORM_PASSIVE_HP_MAX;
				break;
			case	87://ap max passive buff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_PASSIVE_AP_MAX;
				break;
			case	94://m_def active buff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_ACTIVE_MAGIC_DEF;
				break;
			case	102://eva transform passive buff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_TRANSFORM_PASSIVE_EVA;
				break;
			case	103://att1
				ability->add_att1		= skill->order[i].damage;
				ability->damage_type		|=  _DAMAGE_NORMAL;
				break;
			case	104://att2
				ability->add_att2		= skill->order[i].damage;
				ability->damage_type		|=  _DAMAGE_NORMAL;
				break;
			case	105://attr land damage
				ability->attr_attack		=  1;
				ability->attr_damage		= skill->order[i].damage;
				ability->damage_type		|=  _DAMAGE_NORMAL;
				break;
			case	106://attr water damage
				ability->attr_attack		=  2;
				ability->attr_damage		= skill->order[i].damage;
				ability->damage_type		|=  _DAMAGE_NORMAL;
				break;
			case	107://attr fire damage
				ability->attr_attack		=  3;
				ability->attr_damage		= skill->order[i].damage;
				ability->damage_type		|=  _DAMAGE_NORMAL;
				break;
			case	108://attr wind damage
				ability->attr_attack		=  4;
				ability->attr_damage		= skill->order[i].damage;
				ability->damage_type		|=  _DAMAGE_NORMAL;
				break;
			case	109://i_att
				ability->add_i_att		= skill->order[i].damage;
				ability->damage_type		|=  _DAMAGE_IMPACT;
				break;
			case	113://poisoning 
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_DE_ACTIVE_POISONING;
				break;
			case	114://fire poisoning
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_DE_ACTIVE_FIREPOISONING;
				break;
			case	122://action
				break;
			case	133://stun
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_DE_ACTIVE_STUN;
				ability->damage_type		|=  _DAMAGE_STUN;
				break;
			case	135://multiple shoot
				break;
			case	148://hp heal
				ability->hp_heal		= skill->order[i].damage;
				ability->damage_type		|=  _DAMAGE_HP_HEAL;
				break;
			case	153://self destruction
				ability->damage_type		|=  _DAMAGE_NORMAL;
				break;
			case	155://mob summon
				break;
			case	156://att2 condition equip passive buff
				buff_value[*buff_cnt]		= skill->order[i].damage;
				buff_sec[*buff_cnt]		= skill->order[i].sec;
				buff_serise[(*buff_cnt)++]	= _BUFF_EQUIP_PASSIVE_COND_COMBO2;
				break;
		}
	}
}


void	D_compute_skill_damage(const int channel_link, const int type, const int att_id, const int def_id, struct skill_ability *ability,
														int *damage, int *damage_type)
{
	*damage			= 0;
	*damage_type		= _DAMAGE_NOTING;
	ability->add_att1	+= ability->attr_damage;	


	//damage
	if( ability->damage_type & _DAMAGE_NORMAL )
	{
		*damage_type	= _DAMAGE_NORMAL;
		*damage		= D_compute_damage(channel_link, type, att_id, def_id, ability->add_att1, ability->add_att2);
		if( ability->attr_attack )
			*damage	= D_compute_skill_attr_damage(channel_link, type, att_id, def_id, *damage, ability->attr_attack);

		//impact
		if( (ability->damage_type & _DAMAGE_IMPACT) )
		{
			if( D_check_impact(channel_link, type, att_id, def_id, ability->add_i_att) )
				*damage_type	|= _DAMAGE_IMPACT;
		}
	}

	//provocation
	if( ability->damage_type & _DAMAGE_PROVOCATION )
	{
		if( D_check_provocation(channel_link, type, att_id, def_id, ability->add_provocation) )
			*damage_type	|= _DAMAGE_PROVOCATION;
	}
}


static	int	D_compute_skill_attr_damage(const int channel_link, const int type, const int att_id, const int def_id,
											const int skill_damage, const int attr_attack)
{
	int	damage, def_elements, def_m_def, att_land, att_water, att_fire, att_wind;


	switch( type )
	{
		case	CHAR_ATTACK_MOB:
			def_elements	= D_type_mob[D_mob[def_id][channel_link].type].elements;
			def_m_def	= D_get_mob_m_def(channel_link, def_id);	
			att_land	= D_get_client_att_land(att_id);	
			att_water	= D_get_client_att_water(att_id);	
			att_fire	= D_get_client_att_fire(att_id);	
			att_wind	= D_get_client_att_wind(att_id);	
			break;
		case	MOB_ATTACK_CHAR:
			def_elements	= 0;
			def_m_def	= D_get_client_m_def(def_id);	
			att_land	= D_get_mob_att_land(channel_link, att_id);	
			att_water	= D_get_mob_att_water(channel_link, att_id);	
			att_fire	= D_get_mob_att_fire(channel_link, att_id);	
			att_wind	= D_get_mob_att_wind(channel_link, att_id);	
			break;
		default:
			def_elements	= 0;
			def_m_def	= 0;
			att_land	= 0;
			att_water	= 0;
			att_fire	= 0;
			att_wind	= 0;
	}
	
	switch( attr_attack )
	{
		case	0:
			damage	= skill_damage + att_land - def_m_def;
			break;
		case	1:
			damage	= skill_damage + att_land - def_m_def;
			if( def_elements == 4 )
				damage	+= damage*0.5f;
			break;
		case	2:
			damage	= skill_damage + att_water - def_m_def;
			if( def_elements == 3 )
				damage	+= damage*0.5f;
			break;
		case	3:
			damage	= skill_damage + att_fire - def_m_def;
			if( def_elements == 2 )
				damage	+= damage*0.5f;
			break;
		case	4:
			damage	= skill_damage + att_wind - def_m_def;
			if( def_elements == 1 )
				damage	+= damage*0.5f;
			break;
		default:
			damage	= 0;
	}

	return	(damage);
}


//______________________________________________________________
//
int	D_GetMax_world(void)
{
	return	(D_max_world);
}


char	*D_GetWorld_name(const int world)
{
	return	(D_world[world].name);
}


int	D_GetWorld_channel_cnt(const int world)
{
	return	(D_world[world].channel_cnt);
}


void	D_SetWorld_channel_user_plus(const int world, const int channel, const int value)
{
	D_world[world].channel_user[channel]	+= value;
}


int	D_GetWorld_channel_user(const int world, const int channel)
{
	return	(D_world[world].channel_user[channel]);
}	


char	*D_GetWorld_machine_outer_ip(const int world, const int machine)
{
	return	(D_world[world].machine[machine].outer_ip);
}	

	
char	*D_GetWorld_machine_dbagent_ip(const int world, const int machine)
{
	return	(D_world[world].machine[machine].dbagent_ip);
}


char	*D_GetWorld_machine_mobserv_ip(const int world, const int machine)
{
	return	(D_world[world].machine[machine].mobserv_ip);
}


int	D_GetMax_map(void)
{
	return	(D_max_map);
}


int	D_GetMax_type_mob(void)
{
	return	(D_max_type_mob);
}


int	D_GetMax_mob(void)
{
	return	(D_max_mob);
}


int	D_GetMax_type_mob_skill(void)
{
	return	(D_max_type_mob_skill);
}


//______________________________________________________________
//
char	D_GetMap_attr(const int map, const int x, const int z) 
{
	return	(D_map[map].attr[x][z]);
}


int	D_GetMapBlock_item_link(const int channel_link, const int map, const int block_x)
{
	return	(D_map[map].block[block_x].item_link[channel_link]);
} 


void	D_SetMapBlock_insert_item(const int item_id)
{
	D_item[item_id].block_next	= D_map[D_item[item_id].map].block[D_item[item_id].block_x].item_link[D_item[item_id].channel_link];
	D_item[item_id].block_prev	= &(D_map[D_item[item_id].map].block[D_item[item_id].block_x].item_link[D_item[item_id].channel_link]);

	if( D_item[item_id].block_next )	
		D_item[D_item[item_id].block_next].block_prev	= &(D_item[item_id].block_next);
	
	D_map[D_item[item_id].map].block[D_item[item_id].block_x].item_link[D_item[item_id].channel_link]	= item_id;
}


void	D_SetMapBlock_delete_item(const int item_id)
{
	*(D_item[item_id].block_prev)	= D_item[item_id].block_next;

	if( D_item[item_id].block_next )
		D_item[D_item[item_id].block_next].block_prev	= D_item[item_id].block_prev;
}


int	D_GetMapBlock_mob_link(const int channel_link, const int map, const int block_x)
{
	return	(D_map[map].block[block_x].mob_link[channel_link]);
} 


void	D_SetMapBlock_insert_mob(const int channel_link, const int mob_id)
{
	D_mob[mob_id][channel_link].block_next
			= D_map[D_mob[mob_id][channel_link].map].block[D_mob[mob_id][channel_link].block_x].mob_link[channel_link];
	D_mob[mob_id][channel_link].block_prev
			= &(D_map[D_mob[mob_id][channel_link].map].block[D_mob[mob_id][channel_link].block_x].mob_link[channel_link]);

	if( D_mob[mob_id][channel_link].block_next )	
		D_mob[D_mob[mob_id][channel_link].block_next][channel_link].block_prev	= &(D_mob[mob_id][channel_link].block_next);
	
	D_map[D_mob[mob_id][channel_link].map].block[D_mob[mob_id][channel_link].block_x].mob_link[channel_link]	= mob_id;
}


void	D_SetMapBlock_delete_mob(const int channel_link, const int mob_id)
{
	*(D_mob[mob_id][channel_link].block_prev)	= D_mob[mob_id][channel_link].block_next;

	if( D_mob[mob_id][channel_link].block_next )
		D_mob[D_mob[mob_id][channel_link].block_next][channel_link].block_prev	= D_mob[mob_id][channel_link].block_prev;
}


int	D_GetMapBlock_char_link(const int channel_link, const int map, const int block_x)
{
	return	(D_map[map].block[block_x].char_link[channel_link]);
} 


void	D_SetMapBlock_insert_char(const int fd)
{
	D_client[fd].block_next	= D_map[D_client[fd].map].block[D_client[fd].block_x].char_link[D_client[fd].channel_link];
	D_client[fd].block_prev	= &(D_map[D_client[fd].map].block[D_client[fd].block_x].char_link[D_client[fd].channel_link]);

	if( D_client[fd].block_next )	
		D_client[D_client[fd].block_next].block_prev	= &(D_client[fd].block_next);
	
	D_map[D_client[fd].map].block[D_client[fd].block_x].char_link[D_client[fd].channel_link]	= fd;
}


void	D_SetMapBlock_delete_char(const int fd)
{
	*(D_client[fd].block_prev)	= D_client[fd].block_next;

	if( D_client[fd].block_next )
		D_client[D_client[fd].block_next].block_prev	= D_client[fd].block_prev;
}



//______________________________________________________________
//
int	D_GetItem_block_next(const int item_id)
{
	return	(D_item[item_id].block_next);
}

time_t	D_GetItem_drop_time(const int item_id)
{
	return	(D_item[item_id].drop_time);
}

int	D_GetItem_channel_link(const int item_id)
{
	return	(D_item[item_id].channel_link);
}

int	D_GetItem_map(const int item_id)
{
	return	(D_item[item_id].map);
}	
	

int	D_GetItem_block_x(const int item_id)
{
	return	(D_item[item_id].block_x);
}	


void	D_SetItem_clear(const int item_id)
{
	D_item[item_id].drop_time	= 0;

	D_return_item_id(item_id);
}


void	D_GetItem_appear_data(struct body2_GC_ITEM_APPEAR *body, const int item_id)
{
	body->item_id	= item_id;
	memcpy(body->item, &(D_item[item_id].item), ITEM_DATA_LEN);
	body->x		= D_item[item_id].x;
	body->y		= D_item[item_id].y;
	body->z		= D_item[item_id].z;
	strncpy(body->own_char_name, D_item[item_id].own_char_name, CHAR_NAME_LEN+1);
}



//______________________________________________________________
//
int	D_GetMob_block_next(const int channel_link, const int mob_id)
{
	return	(D_mob[mob_id][channel_link].block_next);
}


int	D_GetMob_map(const int channel_link, const int mob_id) 
{
	return	(D_mob[mob_id][channel_link].map);
}


void	D_SetMob_map(const int channel_link, const int mob_id, const int value)
{
	D_mob[mob_id][channel_link].map	= value;
}


int	D_GetMob_block_x(const int channel_link, const int mob_id) 
{
	return	(D_mob[mob_id][channel_link].block_x);
}


void	D_SetMob_block_x(const int channel_link, const int mob_id, const int value)
{
	D_mob[mob_id][channel_link].block_x	= value;
}


int	D_GetMob_dir(const int channel_link, const int mob_id) 
{
	return	(D_mob[mob_id][channel_link].dir);
}


void	D_SetMob_dir(const int channel_link, const int mob_id, const int value)
{
	D_mob[mob_id][channel_link].dir	= value;
}


int	D_GetMob_status(const int channel_link, const int mob_id)
{
	return	(D_mob[mob_id][channel_link].status);
}


void	D_SetMob_status(const int channel_link, const int mob_id, const int value)
{
	D_mob[mob_id][channel_link].status	= value;
}


void	D_GetMob_xyz(const int channel_link, const int mob_id, float *x, float *y, float *z)
{
	*x	= D_mob[mob_id][channel_link].x;
	*y	= D_mob[mob_id][channel_link].y;
	*z	= D_mob[mob_id][channel_link].z;
}


void	D_SetMob_xyz(const int channel_link, const int mob_id, const float x, const float y, const float z)
{
	D_mob[mob_id][channel_link].x	= x;
	D_mob[mob_id][channel_link].y	= y;
	D_mob[mob_id][channel_link].z	= z;
}


int	D_GetMob_hp(const int channel_link, const int mob_id)
{
	return	(D_mob[mob_id][channel_link].hp);
}


int	D_GetMob_buff_value(const int channel_link, const int mob_id, const int buff_series)
{
	return	(D_mob[mob_id][channel_link].buff_value[buff_series]);
}


time_t	D_GetMob_buff_end_sec(const int channel_link, const int mob_id, const int buff_series)
{
	return	(D_mob[mob_id][channel_link].buff_end_sec[buff_series]);
}


void	D_SetMob_clear(const int channel_link, const int mob_id)
{
	D_mob[mob_id][channel_link].status	= _MOB_EXTINCT;
}


void	D_SetMob_buff(const int channel_link, const int mob_id, const int buff_series, const int buff_value, const int buff_sec)
{
	if( D_mob[mob_id][channel_link].buff_value[buff_series] <= buff_value )
	{
		D_mob[mob_id][channel_link].buff_value[buff_series]		= buff_value;
		D_mob[mob_id][channel_link].buff_end_sec[buff_series]	= G_Sec + buff_sec;
	}
}


void	D_SetMob_buff_clear(const int channel_link, const int mob_id, const int buff_series)
{
	D_mob[mob_id][channel_link].buff_value[buff_series]	= 0;
	D_mob[mob_id][channel_link].buff_end_sec[buff_series]	= 0;
}

	
void	D_SetMob_recover(const int channel_link, const int mob_id)
{
	//recover hp
	if( ( D_mob[mob_id][channel_link].hp += D_get_mob_recover_hp(channel_link, mob_id)) < 0 ) 
		D_mob[mob_id][channel_link].hp	= 1;
	else if( D_mob[mob_id][channel_link].hp > D_get_mob_hp_max(channel_link, mob_id) )
		D_mob[mob_id][channel_link].hp	= D_get_mob_hp_max(channel_link, mob_id);
		
	//recover ap
	if( (D_mob[mob_id][channel_link].ap += D_get_mob_recover_ap(channel_link, mob_id)) < 0)
		D_mob[mob_id][channel_link].ap	= 1;
	else if( D_mob[mob_id][channel_link].ap > D_get_mob_ap_max(channel_link, mob_id) )
		D_mob[mob_id][channel_link].ap	= D_get_mob_ap_max(channel_link, mob_id);
}


void	D_SetMob_perfect_recover(const int channel_link, const int mob_id)
{
	D_mob[mob_id][channel_link].hp	= D_get_mob_hp_max(channel_link, mob_id);
	D_mob[mob_id][channel_link].ap	= D_get_mob_ap_max(channel_link, mob_id);
}	


int	D_SetMob_regen(const int channel_link, const int mob_id, const int mob_type, const int table_idx,
					const int map, const int dir, const int regen_x, const int regen_y, const int regen_z)
{
	D_mob[mob_id][channel_link].type		= mob_type;
	D_mob[mob_id][channel_link].x			= regen_x; 
	D_mob[mob_id][channel_link].y			= 0; 
	D_mob[mob_id][channel_link].z			= regen_z; 
	D_mob[mob_id][channel_link].stair		= D_map[map].attr[regen_x][regen_z];
	D_mob[mob_id][channel_link].level		= D_type_mob[mob_type].level;
	D_mob[mob_id][channel_link].map			= map;
	D_mob[mob_id][channel_link].dir			= dir; 

	D_mob[mob_id][channel_link].hp			= D_type_mob[mob_type].hp_max;
	D_mob[mob_id][channel_link].ap			= D_type_mob[mob_type].ap_max;;
	
	D_mob[mob_id][channel_link].block_x		= (regen_x / VIEW_BLOCK_TILE) + 2; 

	memset(D_mob[mob_id][channel_link].buff_value, 0, SIZEOF_INT*BUFF_SERIES_CNT);
	memset((void *)(D_mob[mob_id][channel_link].buff_end_sec), 0, SIZEOF_INT*BUFF_SERIES_CNT);

	D_mob[mob_id][channel_link].hp_max		= D_type_mob[mob_type].hp_max;
	D_mob[mob_id][channel_link].ap_max		= D_type_mob[mob_type].ap_max;;
	
	D_mob[mob_id][channel_link].att1		= D_type_mob[mob_type].att1;
	D_mob[mob_id][channel_link].def1		= D_type_mob[mob_type].def1;
	D_mob[mob_id][channel_link].att2		= D_type_mob[mob_type].att2;
	D_mob[mob_id][channel_link].def2		= D_type_mob[mob_type].def2;
	D_mob[mob_id][channel_link].eva			= D_type_mob[mob_type].eva;
	D_mob[mob_id][channel_link].cri			= D_type_mob[mob_type].cri;
	D_mob[mob_id][channel_link].m_def		= D_type_mob[mob_type].m_def;
	D_mob[mob_id][channel_link].i_att		= D_type_mob[mob_type].i_att;
	D_mob[mob_id][channel_link].is_def		= D_type_mob[mob_type].is_def;
	D_mob[mob_id][channel_link].il_def		= D_type_mob[mob_type].il_def;
	D_mob[mob_id][channel_link].recover_hp		= D_type_mob[mob_type].recover_hp;
	D_mob[mob_id][channel_link].recover_ap		= D_type_mob[mob_type].recover_ap;
	D_mob[mob_id][channel_link].att_milli_speed	= D_type_mob[mob_type].milli_delay_attack;
	D_mob[mob_id][channel_link].att_area		= D_type_mob[mob_type].att_area;
	D_mob[mob_id][channel_link].move_speed1		= D_type_mob[mob_type].move_speed1;
	D_mob[mob_id][channel_link].move_speed2		= D_type_mob[mob_type].move_speed2;
	D_mob[mob_id][channel_link].move_speed3		= D_type_mob[mob_type].move_speed3;

	//check npc_type
	if( D_type_mob[mob_type].npc_type )
	{
		if( D_type_mob[mob_type].npc_type == NPC_TYPE_STORE )
		{
			if( (table_idx < 1) || (table_idx > D_max_type_store) )
			{
				U_Printf("[DB]Error mob_id(%d) mob_type(%d) npc_type(%d) bad table_idx(%d)\n",
								mob_id, mob_type, D_type_mob[mob_id].npc_type, table_idx);
				return	(-1);
			}
		}
		else
		{
			if( (table_idx < 1) || (table_idx > D_max_type_warp) )
			{
				U_Printf("[DB]Error mob_id(%d) mob_type(%d) npc_type(%d) bad table_idx(%d)\n",
								mob_id, mob_type, D_type_mob[mob_id].npc_type, table_idx);
				return	(-1);
			}
		}
			
		D_mob[mob_id][channel_link].table_idx	= table_idx;
		D_mob[mob_id][channel_link].status	= _MOB_WAIT1;
	}
	else
	{
		D_mob[mob_id][channel_link].table_idx	= 0;
		D_mob[mob_id][channel_link].status	= _MOB_REGEN; 
	}

	return	(0);
}	


void	D_GetMob_appear_data(struct body2_GC_MOB_APPEAR *body, const int channel_link, const int mob_id)
{
	body->mob_id	= mob_id;
	body->mob_type	= D_mob[mob_id][channel_link].type;
	body->table_idx	= D_mob[mob_id][channel_link].table_idx;
	body->mob_stair	= D_mob[mob_id][channel_link].stair;
	body->x		= D_mob[mob_id][channel_link].x;
	body->y		= D_mob[mob_id][channel_link].y;
	body->z		= D_mob[mob_id][channel_link].z;
	body->dir	= D_mob[mob_id][channel_link].dir;
	body->status	= D_mob[mob_id][channel_link].status;
	body->speed	= 0;
	body->hp	= D_mob[mob_id][channel_link].hp;
}


//______________________________________________________________
//
int	D_GetClient_sec_active(const int fd)
{
	return	(D_client[fd].sec_active);
}


void	D_SetClient_sec_active(const int fd, const int value)
{
	D_client[fd].sec_active		= value;
}


int	D_GetClient_session(const int fd)
{
	return	(D_client[fd].session);
}


void	D_SetClient_session(const int fd, const int value)
{
	D_client[fd].session		= value;
}


int	D_GetClient_billing_idx(const int fd) 
{
	return	(D_client[fd].billing_idx);
}


void	D_SetClient_billing_idx(const int fd, const int value)
{
	D_client[fd].billing_idx	= value;
}


int	D_GetClient_use_billing_idx(const int fd) 
{
	return	(D_client[fd].use_billing_idx);
}


int	D_GetClient_login_dbagent_idx(const int fd) 
{
	return	(D_client[fd].login_dbagent_idx);
}


void	D_SetClient_login_dbagent_idx(const int fd, const int value)
{
	D_client[fd].login_dbagent_idx	= value;
}


int	D_GetClient_use_login_dbagent_idx(const int fd) 
{
	return	(D_client[fd].use_login_dbagent_idx);
}


int	D_GetClient_dbagent_idx(const int fd) 
{
	return	(D_client[fd].dbagent_idx);
}


void	D_SetClient_dbagent_idx(const int fd, const int value)
{
	D_client[fd].dbagent_idx	= value;
}


int	D_GetClient_use_dbagent_idx(const int fd) 
{
	return	(D_client[fd].use_dbagent_idx);
}


int	D_GetClient_mobserv_charge_channel_link(const int fd) 
{
	return	(D_client[fd].mobserv_charge_channel_link);
}


void	D_SetClient_mobserv_charge_channel_link(const int fd, const int value)
{
	D_client[fd].mobserv_charge_channel_link	= value;
}


int	D_GetClient_block_next(const int fd)
{
	return	(D_client[fd].block_next);
}


int	D_GetClient_map(const int fd) 
{
	return	(D_client[fd].map);
}


void	D_SetClient_map(const int fd, const int value)
{
	D_client[fd].map	= value;
}


int	D_GetClient_block_x(const int fd) 
{
	return	(D_client[fd].block_x);
}


void	D_SetClient_block_x(const int fd, const int value)
{
	D_client[fd].block_x	= value;
}


int	D_GetClient_dir(const int fd) 
{
	return	(D_client[fd].dir);
}


void	D_SetClient_dir(const int fd, const int value)
{
	D_client[fd].dir	= value;
}


int	D_GetClient_status(const int fd)
{
	return	(D_client[fd].status);
}


void	D_SetClient_status(const int fd, const int value)
{
	D_client[fd].status	= value;
}


void	D_GetClient_xyz(const int fd, float *x, float *y, float *z)
{
	*x	= D_client[fd].x;
	*y	= D_client[fd].y;
	*z	= D_client[fd].z;
}


void	D_SetClient_xyz(const int fd, const float x, const float y, const float z)
{
	D_client[fd].x	= x;
	D_client[fd].y	= y;
	D_client[fd].z	= z;
}


int	D_GetClient_hp(const int fd)
{
	return	(D_client[fd].hp);
}


void	D_SetClient_hp(const int fd, const int value)
{
	D_client[fd].hp	= value;
}


int	D_GetClient_ap(const int fd)
{
	return	(D_client[fd].ap);
}


time_t	D_GetClient_char_load_sec(const int fd)
{
	return	(D_client[fd].char_load_sec);
}


int	D_GetClient_level(const int fd)
{
	return	(D_client[fd].level);
}


int	D_GetClient_exp(const int fd)
{
	return	(D_client[fd].exp);
}


void	D_SetClient_last_attack_type(const int fd, const int value)
{
	D_client[fd].last_attack_type	= value;	
}


int	D_GetClient_money(const int fd)
{
	return	(D_client[fd].money);
}


void	*D_GetClient_item_slot(const int fd, const int item_slot)
{
	return	(D_client[fd].item_slot + item_slot);
}


int	D_GetClient_buff_value(const int fd, const int buff_series)
{
	return	(D_client[fd].buff_value[buff_series]);
}


time_t	D_GetClient_buff_end_sec(const int fd, const int buff_series)
{
	return	(D_client[fd].buff_end_sec[buff_series]);
}


int	D_GetClient_skill_point(const int fd)
{
	return	(D_client[fd].skill_point);
}


int	D_GetClient_skill_slot(const int fd, const int skill_slot)
{
	return	(D_client[fd].skill_slot[skill_slot]);
}


char	*D_GetClient_ip(const int fd)
{
	return	(D_client[fd].ip);
}


void	D_SetClient_ip(const int fd, const char *string)
{
	strncpy(D_client[fd].ip, string, SIZEOF_IP+1);
}


char	*D_GetClient_lock_user_id(const int fd) 
{
	return	(D_client[fd].lock_user_id);
}


void	D_SetClient_lock_user_id(const int fd, const char *string)
{
	strncpy(D_client[fd].lock_user_id, string, USER_ID_LEN+1);
}


char	*D_GetClient_user_id(const int fd) 
{
	return	(D_client[fd].user_id);
}


void	D_SetClient_user_id(const int fd, const char *string)
{
	strncpy(D_client[fd].user_id, string, USER_ID_LEN+1);
}


char	*D_GetClient_char_name(const int fd) 
{
	return	(D_client[fd].char_name);
}


void	D_SetClient_char_name(const int fd, const char *string)
{
	strncpy(D_client[fd].char_name, string, CHAR_NAME_LEN+1);
}


void	D_SetClient_channel_link(const int fd, const int value)
{
	D_client[fd].channel_link	= value;
}


int	D_GetClient_channel_link(const int fd)
{
	return	(D_client[fd].channel_link);	
}


void	D_SetClient_buff(const int fd, const int buff_series, const int buff_value, const int buff_sec)
{
	if( D_client[fd].buff_value[buff_series] <= buff_value )
	{
		D_client[fd].buff_value[buff_series]	= buff_value;
		D_client[fd].buff_end_sec[buff_series]	= G_Sec + buff_sec;
	}
}


void	D_SetClient_buff_clear(const int fd, const int buff_series)
{
	D_client[fd].buff_value[buff_series]	= 0;
	D_client[fd].buff_end_sec[buff_series]	= 0;
}


void	D_SetClient_recover(const int fd, int *change)
{
	int	hp, ap;


	hp	= D_client[fd].hp;
	ap	= D_client[fd].ap;

	//recover hp
	if( (D_client[fd].hp += D_get_client_recover_hp(fd)) < 0 )
		D_client[fd].hp	= 1;
	else if( D_client[fd].hp > D_get_client_hp_max(fd) )
		D_client[fd].hp	= D_get_client_hp_max(fd);

	//recover ap
	if( (D_client[fd].ap += D_get_client_recover_ap(fd)) < 0 )
		D_client[fd].ap	= 1;
	else if( D_client[fd].ap > D_get_client_ap_max(fd) )
		D_client[fd].ap	= D_get_client_ap_max(fd);

	//check change
	if( (hp == D_client[fd].hp) && (ap == D_client[fd].ap) )
		*change	= 0;
	else
		*change	= 1;
}


void	D_SetClient_clear(const int fd)
{
	D_client[fd].sec_active				= 0;
	D_client[fd].billing_idx			= 0;
	D_client[fd].dbagent_idx			= 0;
	D_client[fd].login_dbagent_idx			= 0;
	D_client[fd].mobserv_charge_channel_link	= 0;
	D_client[fd].lock_user_id[0]			= 0;
	D_client[fd].user_id[0]				= 0;
	D_client[fd].char_name[0]			= 0;
	D_client[fd].status				= _USER_EXTINCT;
	D_client[fd].channel_link			= 0;
}


int	D_SetClient_from_db(const int fd, struct body1_AW_CHAR_LOAD_RESULT *body1, struct body2_AW_CHAR_LOAD_RESULT *body2, int *change_buff)
{
	int			i, j;
	struct	item_object	*item;


	memset(change_buff, 0, SIZEOF_INT*(PASSIVE_BUFF_CNT+EQUIP_PASSIVE_BUFF_CNT));
	
	//check map, x, y, z
	if( (body1->map < 1) || (body1->map > D_max_map)
		|| (body1->x < 1) || (body1->x > (MAP_X_TILE-2)) || (body1->z < 1) || (body1->z > (MAP_Z_TILE-2)) )
	{
		U_Printf("[DB]Error fd(%d) map(%d) x(%d) y(%d) z(%d)\n", fd, body1->map, body1->x, body1->y, body1->z);	
		return	(-1);
	}
	
	//check map_attr
	if( D_map[body1->map].attr[body1->x][body1->z] == MAP_ATTR_COLLISION )
	{
		U_Printf("[DB]Error fd(%d) map(%d) x(%d) y(%d) z(%d) attr(%d)\n",
					fd, body1->map, body1->x, body1->y, body1->z, D_map[body1->map].attr[body1->x][body1->z]);	
		return	(-1);
	}

	//check regen_map, regen_x, regen_y
	if( (body1->regen_map < 1) || (body1->regen_map > D_max_map)
		|| (body1->regen_x < 1) || (body1->regen_x > (MAP_X_TILE-2)) || (body1->regen_z < 1) || (body1->regen_z > (MAP_Z_TILE-2)) )
	{
		U_Printf("[DB]Error fd(%d) regen_map(%d) regen_x(%d) regen_y(%d) regen_z(%d)\n",
						fd, body1->regen_map, body1->regen_x, body1->regen_y, body1->regen_z);	
		return	(-1);
	}

	//check regen map_attr
	if( D_map[body1->regen_map].attr[body1->regen_x][body1->regen_z] == MAP_ATTR_COLLISION )
	{
		U_Printf("[DB]Error fd(%d) regen_map(%d) regen_x(%d) regen_y(%d) regen_z(%d) attr(%d)\n", fd, body1->regen_map,
				body1->regen_x, body1->regen_y, body1->regen_z, D_map[body1->regen_map].attr[body1->regen_x][body1->regen_z]);	
		return	(-1);
	}


	//check sex, hair, face
	if( (body1->sex < 0) || (body1->sex > MAX_CHAR_SEX)
		|| (body1->hair < 0) || (body1->hair > MAX_CHAR_HAIR)|| (body1->face < 0) || (body1->face > MAX_CHAR_FACE)) 
	{
		U_Printf("[DB]Error fd(%d) sex(%d) hair(%d) face(%d)\n", fd, body1->sex, body1->hair, body1->face);
		return	(-1);
	}
	
	//check level, hp, ap
	if( (body1->level < 1) || (body1->level > D_max_level) || (body1->hp < 0) || (body1->ap < 0) ) 
	{
		U_Printf("[DB]Error fd(%d) level(%d) hp(%d) ap(%d)\n", fd, body1->level, body1->hp, body1->ap);
		return	(-1);
	}

	//check money, skill_point
	if( (body1->money < 0) || (body1->skill_point < 0) )
	{
		U_Printf("[DB]Error fd(%d) money(%d) skill_point(%d)\n", fd, body1->money, body1->skill_point);
		return	(-1);
	}

	//set db data	
	D_client[fd].map		= body1->map;
	D_client[fd].x			= body1->x + 0.5f;
	D_client[fd].y			= 0;
	D_client[fd].z			= body1->z + 0.5f;
	D_client[fd].regen_map		= body1->regen_map;
	D_client[fd].regen_x		= body1->regen_x + 0.5f;
	D_client[fd].regen_y		= 0;
	D_client[fd].regen_z		= body1->regen_z + 0.5f;
	D_client[fd].sex		= body1->sex;
	D_client[fd].hair		= body1->hair;
	D_client[fd].face		= body1->face;
	D_client[fd].level		= body1->level;
	D_client[fd].exp		= body1->exp;
	D_client[fd].hp			= body1->hp;
	D_client[fd].ap			= body1->ap;
	D_client[fd].money		= body1->money;
	D_client[fd].skill_point	= body1->skill_point;
	D_client[fd].load_cnt		= body1->load_cnt;
	D_client[fd].char_load_sec	= G_Sec;

	//set buff 
	memset(D_client[fd].buff_value, 0, SIZEOF_INT*BUFF_SERIES_CNT);
	memset((void *)(D_client[fd].buff_end_sec), 0, SIZEOF_INT*BUFF_SERIES_CNT);


	//set cool time
	memset(D_client[fd].cool_end_sec, 0, SIZEOF_INT*(MAX_COOL_SERIES+1));

	
	//set ability	
	D_client[fd].hp_max			= D_level[D_client[fd].level].hp_max;
	D_client[fd].hp_max_equip		= 0;
	D_client[fd].ap_max			= D_level[D_client[fd].level].ap_max;
	D_client[fd].ap_max_equip		= 0;
	D_client[fd].att_land_equip		= 0;
	D_client[fd].att_water_equip		= 0;
	D_client[fd].att_fire_equip		= 0;
	D_client[fd].att_wind_equip		= 0;
	D_client[fd].m_def			= 0;
	D_client[fd].m_def_equip		= 0;
	D_client[fd].att1			= D_level[D_client[fd].level].att1;
	D_client[fd].att1_equip			= 0;
	D_client[fd].att2_equip			= 0;
	D_client[fd].def1			= D_level[D_client[fd].level].def1;
	D_client[fd].def1_equip			= 0;
	D_client[fd].def2_equip			= 0;
	D_client[fd].eva_equip			= 0;
	D_client[fd].cri			= D_level[D_client[fd].level].cri;
	D_client[fd].cri_equip			= 0;
	D_client[fd].i_att			= D_level[D_client[fd].level].i_att;
	D_client[fd].i_att_equip		= 0;
	D_client[fd].is_def			= D_level[D_client[fd].level].is_def;
	D_client[fd].is_def_equip		= 0;
	D_client[fd].il_def			= D_level[D_client[fd].level].il_def;
	D_client[fd].il_def_equip		= 0;
	D_client[fd].recover_hp			= D_level[D_client[fd].level].recover_hp;
	D_client[fd].recover_hp_equip		= 0;
	D_client[fd].recover_ap			= D_level[D_client[fd].level].recover_ap;
	D_client[fd].recover_ap_equip		= 0;
	D_client[fd].att_milli_speed		= BASE_CHAR_MILLI_DELAY_ATTACK;
	D_client[fd].att_area			= BASE_CHAR_AREA_ATTACK;
	D_client[fd].move_speed1		= D_level[fd].move_speed1;
	D_client[fd].move_speed2		= D_level[fd].move_speed2;
	
	//set skill_slot
	for( i=0; i<CHAR_SKILL_CNT; i++ )
	{
		if( (body1->skill_slot[i] < 0) || (body1->skill_slot[i] > D_type_char_skill[1][i+1].max_level) )
		{
			U_Printf("[DB]Error fd(%d) skill_slot(%d) bad level(%d) max_level(%d)\n",
						fd, i, body1->skill_slot[i], D_type_char_skill[1][i+1].max_level);
			return	(-1);
		}
		
		D_client[fd].skill_slot[i]	= body1->skill_slot[i];
	
		//set passive buff
		if( D_client[fd].skill_slot[i] && (D_type_char_skill[1][i+1].method == 3) ) 	
			D_set_client_passive_buff(fd, i+1, change_buff);
	}	
	
	//set item_slot
	for( i=0; i<ITEM_SLOT_CNT; i++ )
		memset(D_client[fd].item_slot + i, 0, ITEM_DATA_LEN+ITEM_SERIAL_LEN);		

	for( i=0; i<body1->item_slot_cnt; i++ )
	{
		item	= (struct item_object *)(body2->item);
		
		//check item_slot
		if( (body2->item_slot < 0) || (body2->item_slot >= ITEM_SLOT_CNT) )
		{
			U_Printf("[DB]Error fd(%d) bad item_slot(%d)\n", fd, body2->item_slot);
			return	(-1);
		}

		//check type
		if( (item->type < 1) || (item->type > D_max_type_item) )
		{
			U_Printf("[DB]Error fd(%d) item_slot(%d) bad type(%d) \n", fd, body2->item_slot, item->type);
			return	(-1);
		}

		//check class
		switch( D_type_item[item->type].class )
		{
			case	ITEM_CLASS_MONEY:
				U_Printf("[DB]Error fd(%d) item_slot(%d) bad item class(%d)\n",fd, body2->item_slot, D_type_item[item->type].class);
				return	(-1);
			case	ITEM_CLASS_ONEHAND_SWORD:
			case	ITEM_CLASS_TWOHAND_SWORD:
			case	ITEM_CLASS_BOW:
			case	ITEM_CLASS_ARROW:
			case	ITEM_CLASS_HAT:
			case	ITEM_CLASS_JACKET:
			case	ITEM_CLASS_PANTS:
			case	ITEM_CLASS_SHOES:
			case	ITEM_CLASS_SHIELD:
			case	ITEM_CLASS_EARRING:
			case	ITEM_CLASS_LEFT_RING:
			case	ITEM_CLASS_RIGHT_RING:
				//check enchant
				for( j=0; j<MAX_ENCHANT_CNT; j++ )
				{
					if( item->enchant[j] && ((item->enchant[j] > D_max_type_item)
							|| (D_type_item[item->enchant[j]].class != ITEM_CLASS_ENCHANT)) )
					{
						U_Printf("[DB]Error fd(%d) item_slot(%d) bad enchant[%d](%d) \n",
										fd, body2->item_slot, j, item->enchant[j]);
						return	(-1);
					}
				}	
				//check serial
				if( item->serial[0] == 0 )
				{
					U_Printf("[DB]Error fd(%d) item_slot(%d) bad serial\n", fd, body2->item_slot);
					return	(-1);
				}
				break;
			case	ITEM_CLASS_QUEST:
				U_Printf("[DB]Error fd(%d) item_slot(%d) bad item class(%d)\n",
							fd, body2->item_slot, D_type_item[item->type].class);
				return	(-1);
			case	ITEM_CLASS_RESTORE_POTION:
				if( (item->stack_num == 0) || (item->stack_num > D_type_item[item->type].stack_num) )
				{
					U_Printf("[DB]Error fd(%d) item_slot(%d) bad stack_num(%d)\n", fd, body2->item_slot, item->stack_num);
					return	(-1);
				}
				break;
			//case	ITEM_CLASS_BUFF_POTION:
			case	ITEM_CLASS_PORTAL:
				U_Printf("[DB]Error fd(%d) item_slot(%d) bad item class(%d)\n",
							fd, body2->item_slot, D_type_item[item->type].class);
				return	(-1);
			case	ITEM_CLASS_RESURRECTION:
				U_Printf("[DB]Error fd(%d) item_slot(%d) bad item class(%d)\n",
							fd, body2->item_slot, D_type_item[item->type].class);
				return	(-1);
			//case	ITEM_CLASS_MATERIAL:
			//case	ITEM_CLASS_ENCHANT:
			//case	ITEM_CLASS_TRANSFORM:
			case	ITEM_CLASS_CASH_COSTUME_ONEHAND_SWORD:
			case	ITEM_CLASS_CASH_COSTUME_TWOHAND_SWORD:
			case	ITEM_CLASS_CASH_COSTUME_BOW:
			case	ITEM_CLASS_CASH_COSTUME_SHIELD:
			case	ITEM_CLASS_CASH_COSTUME_HAT:
			case	ITEM_CLASS_CASH_COSTUME_FACE:
			case	ITEM_CLASS_CASH_COSTUME_JACKET:
			case	ITEM_CLASS_CASH_COSTUME_PANTS:
			case	ITEM_CLASS_CASH_COSTUME_ONEPIECE:
			case	ITEM_CLASS_CASH_COSTUME_CLOAK:
			case	ITEM_CLASS_CASH_COSTUME_SHOES:
			case	ITEM_CLASS_CASH_COSTUME_NECKLACE:
				//check serial
				if( item->serial[0] == 0 )
				{
					U_Printf("[DB]Error fd(%d) item_slot(%d) bad serial\n", fd, body2->item_slot);
					return	(-1);
				}
				break;
			default:
				U_Printf("[DB]Error fd(%d) item_slot(%d) bad item class(%d)\n",
								fd, body2->item_slot, D_type_item[item->type].class);
				return	(-1);
		}//end switch

		//set item
		memcpy(D_client[fd].item_slot + body2->item_slot, item, ITEM_DATA_LEN+ITEM_SERIAL_LEN);	
		
		U_Printf("[DB]load fd(%d) user_id(%s) char_name(%s) item_slot(%d) item_type(%d)\n",
			fd, D_client[fd].user_id, D_client[fd].char_name, body2->item_slot, D_client[fd].item_slot[body2->item_slot].type);
				
		++body2;
	}
		
	//set equip item
	for( i=0; i<EQUIP_SLOT_CNT; i++ )
	{
		if( (D_client[fd].item_slot[i].type) && (D_set_client_item(fd, i, D_client[fd].item_slot +i, change_buff)) )
		{
			U_Printf("[DB]Error fd(%d) D_set_client_item()\n", fd);
			return	(-1);
		}
	}

	//check hp
	if( D_client[fd].hp > D_get_client_hp_max(fd) )
		D_client[fd].hp	= D_get_client_hp_max(fd);

	//check ap
	if( D_client[fd].ap > D_get_client_ap_max(fd) )
		D_client[fd].ap	= D_get_client_ap_max(fd);


	//set temporary data
	D_client[fd].dir	= DIR_S;

	if( D_client[fd].hp )
		D_client[fd].status	= _USER_REGEN;
	else
		D_client[fd].status	= _USER_DEAD_REGEN;
		
	D_client[fd].block_x	= ( ((int)(D_client[fd].x)) / VIEW_BLOCK_TILE) + 2; 
	
	D_client[fd].last_attack_type		= 0;
	D_client[fd].transform_type		= 0;

	D_client[fd].action_skill		= 0;
	D_client[fd].remain_action_skill	= 0;
	
	//set char_name
	strncpy(D_client[fd].char_name, body1->char_name, CHAR_NAME_LEN+1);

	U_Printf("[DB]load fd(%d) user_id(%s) char_name(%s) map(%d) x(%f) z(%f)\n",
			fd, D_client[fd].user_id, D_client[fd].char_name, D_client[fd].map, D_client[fd].x, D_client[fd].z);

	return	(0);
}
	
	
void	D_SetClient_regen(const int fd)
{
	float	exp;


	D_client[fd].map	= D_client[fd].regen_map;
	D_client[fd].x		= D_client[fd].regen_x;
	D_client[fd].y		= 0;
	D_client[fd].z		= D_client[fd].regen_z;
	
	if( D_client[fd].level < 10 )
	{
		D_client[fd].hp		= D_get_client_hp_max(fd);
		D_client[fd].ap		= D_get_client_ap_max(fd);
	}
	else
	{
		exp	= D_client[fd].exp - D_client[fd].exp*0.05f;

		if( exp < D_min_int )	
			D_client[fd].exp	= D_min_int;	
		else
			D_client[fd].exp	= exp;		

		D_client[fd].hp	= D_get_client_hp_max(fd)*0.1f;
		D_client[fd].ap	= D_get_client_ap_max(fd)*0.1f;
	}

	D_client[fd].dir	= DIR_S;
	D_client[fd].status	= _USER_REGEN;
	D_client[fd].block_x	= ( ((int)(D_client[fd].x)) / VIEW_BLOCK_TILE ) + 2; 
}
 
	
void	D_SetClient_warp(const int fd, const int dest_map, const int dest_x, const int dest_z)
{
	D_client[fd].map	= dest_map;
	D_client[fd].x		= dest_x;
	D_client[fd].y		= 0;
	D_client[fd].z		= dest_z;
	D_client[fd].dir	= DIR_S;
	D_client[fd].status	= _USER_REGEN;
	D_client[fd].block_x	= ( ((int)(D_client[fd].x)) / VIEW_BLOCK_TILE ) + 2; 
}


void	D_GetClient_save_data(struct body1_WA_CHAR_SAVE *body1, struct body2_WA_CHAR_SAVE *body2, const int fd)
{
	int	i;


	strncpy(body1->user_id, D_client[fd].user_id, USER_ID_LEN+1);
	strncpy(body1->char_name, D_client[fd].char_name, CHAR_NAME_LEN+1);
	body1->map		= D_client[fd].map;
	body1->x		= D_client[fd].x;
	body1->y		= 0;
	body1->z		= D_client[fd].z;
	body1->regen_map	= D_client[fd].regen_map;
	body1->regen_x		= D_client[fd].regen_x;
	body1->regen_y		= 0;
	body1->regen_z		= D_client[fd].regen_z;
	body1->sex		= D_client[fd].sex;
	body1->hair		= D_client[fd].hair;
	body1->face		= D_client[fd].face;
	body1->level		= D_client[fd].level;
	body1->exp		= D_client[fd].exp;
	body1->hp		= D_client[fd].hp;
	body1->ap		= D_client[fd].ap;
	body1->money		= D_client[fd].money;	
	body1->skill_point	= D_client[fd].skill_point;	
	body1->load_cnt		= D_client[fd].load_cnt +1;
	body1->item_slot_cnt	= 0;
	
	for( i=0; i<CHAR_SKILL_CNT; i++ )
		body1->skill_slot[i]	= D_client[fd].skill_slot[i];

	U_Printf("[DB]save fd(%d) user_id(%s) char_name(%s) money(%d) map(%d) x(%d) z(%d)\n",
				fd, D_client[fd].user_id, D_client[fd].char_name, D_client[fd].money, body1->map, body1->x, body1->z);
	for( i=0; i<ITEM_SLOT_CNT; i++ )
	{
		if( D_client[fd].item_slot[i].type )
		{
			++(body1->item_slot_cnt);
			body2->item_slot	= i;
			memcpy(body2->item, D_client[fd].item_slot + i, ITEM_DATA_LEN+ITEM_SERIAL_LEN);
			++body2;
			U_Printf("[DB]save fd(%d) user_id(%s) char_name(%s) item_slot(%d) item_type(%d)\n",
					fd, D_client[fd].user_id, D_client[fd].char_name, i, D_client[fd].item_slot[i].type);
		}
	}
}	

		
void	D_GetClient_owner_data(struct body1_GC_CHAR_LOAD_RESULT *body1, struct body2_GC_CHAR_LOAD_RESULT *body2, const int fd)
{
	int	i;


	strncpy(body1->char_name, D_client[fd].char_name, CHAR_NAME_LEN+1);
	body1->char_id		= fd;
	body1->map		= D_client[fd].map;
	body1->x		= D_client[fd].x;
	body1->y		= D_client[fd].y;
	body1->z		= D_client[fd].z;
	body1->sex		= D_client[fd].sex;
	body1->hair		= D_client[fd].hair;
	body1->face		= D_client[fd].face;
	body1->level		= D_client[fd].level;
	body1->exp		= D_client[fd].exp;
	body1->hp		= D_client[fd].hp;
	body1->ap		= D_client[fd].ap;
	body1->dir		= D_client[fd].dir;
	body1->status		= D_client[fd].status;
	body1->money		= D_client[fd].money;
	body1->skill_point	= D_client[fd].skill_point;
	body1->item_slot_cnt	= 0;

	for( i=0; i<CHAR_SKILL_CNT; i++ )
		body1->skill_slot[i]	= D_client[fd].skill_slot[i];
	
	for( i=0; i<ITEM_SLOT_CNT; i++ )
	{
		if( D_client[fd].item_slot[i].type )
		{
			++(body1->item_slot_cnt);
			body2->item_slot	= i;
			memcpy(body2->item, D_client[fd].item_slot + i, ITEM_DATA_LEN);
			++body2;
		}
	}
}


void	D_GetClient_appear_data(struct body2_GC_OTHER_CHAR_APPEAR *body, const int fd)
{	
	int	i;


	body->char_id		= fd;
	strncpy(body->char_name, D_client[fd].char_name, CHAR_NAME_LEN+1);
	body->x			= D_client[fd].x;
	body->y			= D_client[fd].y;
	body->z			= D_client[fd].z;
	body->sex		= D_client[fd].sex;
	body->hair		= D_client[fd].hair;
	body->face		= D_client[fd].face;
	body->dir		= D_client[fd].dir;
	body->status		= D_client[fd].status;
	body->speed		= 0;
	body->transform_type	= D_client[fd].transform_type;
	
	for( i=0; i<EQUIP_SLOT_CNT; i++ )
		memcpy(body->item_slot + i, D_client[fd].item_slot + i, ITEM_DATA_LEN);
}


void	D_GetClient_channelin_data(struct body2_GM_CHAR_CHANNEL_IN *body, const int fd)
{
	body->map		= D_client[fd].map;
	body->char_id		= fd;
	body->status		= D_client[fd].status;
	body->x			= D_client[fd].x;
	body->y			= D_client[fd].y;
	body->z			= D_client[fd].z;
}


void	D_GetClient_mapchange_data(struct body_GM_CHAR_MAP_CHANGE *body, const int fd)
{
	body->map		= D_client[fd].map;
	body->char_id		= fd;
	body->status		= D_client[fd].status;
	body->x			= D_client[fd].x;
	body->y			= D_client[fd].y;
	body->z			= D_client[fd].z;
}

	
void	D_GetClient_regen_data(struct body_GC_CHAR_REGEN_RESULT *body, const int fd)
{
	body->regen_map		= D_client[fd].map;
	body->regen_x		= D_client[fd].x;
	body->regen_y		= D_client[fd].y;
	body->regen_z		= D_client[fd].z;
	body->exp		= D_client[fd].exp;
	body->hp		= D_client[fd].hp;
	body->ap		= D_client[fd].ap;
	body->dir		= D_client[fd].dir;
	body->status		= D_client[fd].status;
}


void	D_GetClient_warp_data(struct body_GC_CHAR_WARP_RESULT *body, const int fd)
{
	body->regen_map		= D_client[fd].map;
	body->regen_x		= D_client[fd].x;
	body->regen_y		= D_client[fd].y;
	body->regen_z		= D_client[fd].z;
	body->hp		= D_client[fd].hp;
	body->ap		= D_client[fd].ap;
	body->dir		= D_client[fd].dir;
	body->status		= D_client[fd].status;
}


void	D_GetClient_channel_move_data(struct body_GC_CHAR_CHANNEL_MOVE_RESULT *body, const int fd)
{
	body->regen_map		= D_client[fd].map;
	body->regen_x		= D_client[fd].x;
	body->regen_y		= D_client[fd].y;
	body->regen_z		= D_client[fd].z;
	body->hp		= D_client[fd].hp;
	body->ap		= D_client[fd].ap;
	body->dir		= D_client[fd].dir;
	body->status		= D_client[fd].status;
}


int	D_SetClient_item_move(const int fd, const int item_slot1, const int item_slot2, int *change_buff) 
{
	int			costume_item_slot, temp_slot;
	struct	item_object	item1, item2, costume_item;


	memset(change_buff+PASSIVE_BUFF_CNT, 0, SIZEOF_INT*EQUIP_PASSIVE_BUFF_CNT);
	costume_item.type	= 0;
	item1			= D_client[fd].item_slot[item_slot1];	
	item2			= D_client[fd].item_slot[item_slot2];	
	U_Printf("[DB]fd(%d) item_slot1(%d) item_type(%d)\n", fd, item_slot1, item1.type);
	U_Printf("[DB]fd(%d) item_slot2(%d) item_type(%d)\n", fd, item_slot2, item2.type);


	//clear item
	D_set_client_clear_item(fd, item_slot1, &costume_item_slot, change_buff);
	D_set_client_clear_item(fd, item_slot2, &temp_slot, change_buff);

	//check costume item 	
	if( costume_item_slot != -1 )
	{
		costume_item	= D_client[fd].item_slot[costume_item_slot];	
		D_set_client_clear_item(fd, costume_item_slot, &temp_slot, change_buff);
	}

	//set item
	if( (item2.type) && (D_set_client_item(fd, item_slot1, &item2, change_buff)) )
	{
		memcpy(D_client[fd].item_slot + item_slot1, &item1, ITEM_DATA_LEN+ITEM_SERIAL_LEN);	
		memcpy(D_client[fd].item_slot + item_slot2, &item2, ITEM_DATA_LEN+ITEM_SERIAL_LEN);	
		if( costume_item.type )
			memcpy(D_client[fd].item_slot + costume_item_slot, &costume_item, ITEM_DATA_LEN+ITEM_SERIAL_LEN);	
		U_Printf("[DB]Error fd(%d) D_set_client_item() item_slot1(%d) item2.type(%d)\n", fd, item_slot1, item2.type);	
		return	(-1);
	}

	if( (item1.type) && (D_set_client_item(fd, item_slot2, &item1, change_buff)) )
	{
		memcpy(D_client[fd].item_slot + item_slot1, &item1, ITEM_DATA_LEN+ITEM_SERIAL_LEN);	
		memcpy(D_client[fd].item_slot + item_slot2, &item2, ITEM_DATA_LEN+ITEM_SERIAL_LEN);	
		if( costume_item.type )
			memcpy(D_client[fd].item_slot + costume_item_slot, &costume_item, ITEM_DATA_LEN+ITEM_SERIAL_LEN);	
		U_Printf("[DB]Error fd(%d) D_set_client_item() item_slot2(%d) item1.type(%d)\n", fd, item_slot2, item1.type);	
		return	(-1);
	}
	
	if( (costume_item.type) && (D_set_client_item(fd, costume_item_slot, &costume_item, change_buff)) )
	{
		memcpy(D_client[fd].item_slot + item_slot1, &item1, ITEM_DATA_LEN+ITEM_SERIAL_LEN);	
		memcpy(D_client[fd].item_slot + item_slot2, &item2, ITEM_DATA_LEN+ITEM_SERIAL_LEN);	
		if( costume_item.type )
			memcpy(D_client[fd].item_slot + costume_item_slot, &costume_item, ITEM_DATA_LEN+ITEM_SERIAL_LEN);	
		U_Printf("[DB]Error fd(%d) D_set_client_item() costume_item_slot(%d) costume_item.type(%d)\n",
										fd, costume_item_slot, costume_item.type);	
		return	(-1);
	}

	return	(0);
}

	
int	D_SetClient_item_pickup(const int fd, const int item_slot,
					const int item_id, int *item_class, int *acquire_money, int *change_buff)
{
	float	gab_x, gab_z;


	U_Printf("[DB]pickup item_id(%d) item_type(%d) fd(%d) item_slot(%d) item_type(%d)\n",
			item_id, D_item[item_id].item.type, fd, item_slot, D_client[fd].item_slot[item_slot].type);

	//check item drop_time
	if( D_item[item_id].drop_time == 0 )
	{
		*item_class	= 0;
		return	(0);
	}

	//check own_char_name
	//if( (D_item[item_id].own_char_name[0]) && (strncmp(D_item[item_id].own_char_name, D_client[fd].char_name, CHAR_NAME_LEN+1)) )
	//{	
	//	*item_class	= 0;
	//	return	(0);
	//}

	//check channel_link
	if( D_client[fd].channel_link != D_item[item_id].channel_link )
	{
		*item_class	= 0;
		return	(0);
	}

	//check map
	if( D_client[fd].map != D_item[item_id].map )
	{
		*item_class	= 0;
		return	(0);
	}

	//check pickup area
	gab_x	= D_client[fd].x - D_item[item_id].x;
	gab_z	= D_client[fd].z - D_item[item_id].z;
 
	if( (gab_x < -10) || (gab_x > 10) || (gab_z < -10) || (gab_z > 10) )
	{
		*item_class	= 0;
		return	(0);
	}

	//set item
	if( D_set_client_item(fd, item_slot, &(D_item[item_id].item), change_buff) )
	{
		U_Printf("[DB]Error fd(%d) D_set_client_item()\n", fd);
		return	(-1);
	}

	//item class
	*item_class	= D_type_item[D_item[item_id].item.type].class;

	if( *item_class == ITEM_CLASS_MONEY )
		*acquire_money	= D_item[item_id].item.stack_num;
	else
		*acquire_money	= 0;

	return	(0);
}

	
int	D_SetClient_item_drop(const int fd, const int item_slot, int *item_id) 
{
	//check item slot
	if( D_client[fd].item_slot[item_slot].type == 0 )
	{
		U_Printf("[DB]Error fd(%d) item_slot(%d) is null\n", fd, item_slot);	
		return	(-1);
	}

	//check trade	
	if( D_type_item[D_client[fd].item_slot[item_slot].type].trade )
		*item_id	= D_get_item_id();
	else
		*item_id	= 0;

	//drop item
	if( *item_id )
	{
		if( D_set_item_drop(*item_id, D_client[fd].channel_link, D_client[fd].map, D_client[fd].x, 0, D_client[fd].z,
												D_client[fd].item_slot + item_slot, NULL) )
		{
			D_return_item_id(*item_id);
			*item_id	= 0;
		}
	}

	//clear item_slot	
	memset(D_client[fd].item_slot + item_slot, 0, ITEM_DATA_LEN+ITEM_SERIAL_LEN);	
	
	return	(0);
}


int	D_SetClient_item_enchant(const int fd, const int item_slot1, const int item_slot2)
{
	struct	item_object	*item1, *item2;
	int			enchant_slot_num, enchant_slot;


	item1	= D_client[fd].item_slot + item_slot1;	
	item2	= D_client[fd].item_slot + item_slot2;	

	//check item1 class
	switch( D_type_item[item1->type].class )
	{
		case	ITEM_CLASS_ONEHAND_SWORD:
		case	ITEM_CLASS_TWOHAND_SWORD:
		case	ITEM_CLASS_BOW:
		case	ITEM_CLASS_ARROW:
			enchant_slot_num	= D_type_item[item1->type].data[8];
			break;
		case	ITEM_CLASS_HAT:
		case	ITEM_CLASS_JACKET:
		case	ITEM_CLASS_PANTS:
		case	ITEM_CLASS_SHOES:
		case	ITEM_CLASS_SHIELD:
			enchant_slot_num	= D_type_item[item1->type].data[6];
			break;
		case	ITEM_CLASS_EARRING:
		case	ITEM_CLASS_LEFT_RING:
		case	ITEM_CLASS_RIGHT_RING:
			enchant_slot_num	= D_type_item[item1->type].data[3];
			break;
		default:
			U_Printf("[DB]Error fd(%d) item1 type(%d) bad class(%d)\n", fd, item1->type, D_type_item[item1->type].class);	
			return	(-1);
	}
	
	//check item2 class	
	if( D_type_item[item2->type].class != ITEM_CLASS_ENCHANT )
	{
		U_Printf("[DB]Error fd(%d) item2 type(%d) bad class(%d)\n", fd, item2->type, D_type_item[item2->type].class);	
		return	(-1);
	}	

	//check item1 slot_num
	if( enchant_slot_num == 0 )
	{
		U_Printf("[DB]Error fd(%d) item1 type(%d) bad slot_num(%d)\n", fd, item1->type, enchant_slot_num);	
		return	(-1);
	}

	//check item2 enchant_class
	if( D_type_item[item1->type].class != D_type_item[item2->type].data[2] )
	{
		U_Printf("[DB]Error fd(%d) item1 class(%d) item2 type(%d) bad enchant_class(%d)\n",
				fd, D_type_item[item1->type].class, item2->type, D_type_item[item2->type].data[2]);	
		return	(-1);
	}

	//check level
	if( D_type_item[item1->type].level != D_type_item[item2->type].level )
	{
		U_Printf("[DB]Error fd(%d) item1 level(%d) item2 type(%d) bad level(%d)\n",
					D_type_item[item1->type].level, item2->type, D_type_item[item2->type].level);	
		return	(-1);
	}	

	//find enchant_slot
	enchant_slot	= 0;

	while( enchant_slot_num != enchant_slot )
	{
		if( item1->enchant[enchant_slot] == 0 )
			break;
		
		++enchant_slot;
	}

	if( enchant_slot_num == enchant_slot )
		enchant_slot	= U_GetRand_val(enchant_slot_num);

	//enchant
	D_client[fd].item_slot[item_slot1].enchant[enchant_slot]	= item2->type; 
	
	//clear item
	memset(D_client[fd].item_slot + item_slot2, 0, ITEM_DATA_LEN+ITEM_SERIAL_LEN);	

	return	(0);
}

	
int	D_SetClient_item_use(const int fd, const int item_slot, int *result, int *item_type)
{
	struct	item_object	*item;
	int			value;


	item		= D_client[fd].item_slot + item_slot;
	*item_type	= item->type;
	*result		= -2;

	switch( D_type_item[item->type].class )
	{
		case	ITEM_CLASS_RESTORE_POTION:

			//check cool time
			if( D_client[fd].cool_end_sec[D_type_item[item->type].data[4]] > G_Sec )
			{
				U_Printf("[DB]Error fd(%d) item_type(%d) bad cool_end_sec(%d) G_Sec(%d)\n", fd, item->type,
						D_client[fd].cool_end_sec[D_type_item[item->type].data[4]], G_Sec);	
				return	(-1);
			}
			
			//celerity time	
			if( D_type_item[item->type].data[3] )
			{
				if( D_type_item[item->type].data[1] )
				{
					if( D_type_item[item->type].data[2] )
					{
						D_SetClient_buff(fd, _BUFF_POTION_RECOVER_HP,
									D_type_item[item->type].data[1], D_type_item[item->type].data[3]);

						D_SetClient_buff(fd, _BUFF_POTION_RECOVER_AP,
									D_type_item[item->type].data[2], D_type_item[item->type].data[3]);
				
						*result	= -1;
					}
					else
					{
						D_SetClient_buff(fd, _BUFF_POTION_RECOVER_HP,
									D_type_item[item->type].data[1], D_type_item[item->type].data[3]);
					
						*result	= _BUFF_POTION_RECOVER_HP;
					}
							
				}
				else if( D_type_item[item->type].data[2] )
				{
					D_SetClient_buff(fd, _BUFF_POTION_RECOVER_AP,
								D_type_item[item->type].data[2], D_type_item[item->type].data[3]);

					*result	= _BUFF_POTION_RECOVER_AP;
				}
			}
			else
			{
				if( D_type_item[item->type].data[1] )
				{
					if( ((value = D_client[fd].hp + D_type_item[item->type].data[1]) < 0) || (value > D_client[fd].hp_max) )
						D_client[fd].hp	= D_client[fd].hp_max;
					else
						D_client[fd].hp = value;
				}
				else if( D_type_item[item->type].data[2] )
				{
					if( ((value = D_client[fd].ap + D_type_item[item->type].data[2]) < 0) || (value > D_client[fd].ap_max) )
						D_client[fd].ap	= D_client[fd].ap_max;
					else
						D_client[fd].ap = value;
				}
			}

			//clear item			
			if( (--D_client[fd].item_slot[item_slot].stack_num) == 0 )
				D_client[fd].item_slot[item_slot].type	= 0;
				
			//set cool_end_sec
			D_client[fd].cool_end_sec[D_type_item[item->type].data[4]]	= (D_type_item[item->type].data[5])/1000 + G_Sec;

			break;

		default:
			U_Printf("[DB]Error fd(%d) item type(%d) bad class(%d)\n", fd, item->type, D_type_item[item->type].class);	
			return	(-1);
	}
	
	return	(0);
}

	
int	D_SetClient_skill_ready(const int fd, const int skill_type)
{
	int	skill_level;


	//check skill level
	if( (skill_level = D_client[fd].skill_slot[skill_type-1]) == 0 )
	{
		U_Printf("[DB]Error fd(%d) bad skill_level(%d)\n", fd, D_client[fd].skill_slot[skill_type-1]);
		return	(-1);
	}

	//check action_skill
	if( D_client[fd].action_skill )
	{	
		U_Printf("[DB]Error fd(%d) bad action_skill(%d)\n", fd, D_client[fd].action_skill);
		return	(-1);
	}
	
	//check method
	if( D_type_char_skill[skill_level][skill_type].method < 5 )
	{
		U_Printf("[DB]Error fd(%d) skill_type(%d) bad method(%d)\n", fd, skill_type, D_type_char_skill[skill_level][skill_type].method);
		return	(-1);
	}

	//check casting millisec
	if( D_type_char_skill[skill_level][skill_type].casting_millisec == 0 )
	{
		U_Printf("[DB]Error fd(%d) skill_type(%d) bad casting_millisec(%d)\n",
						fd, skill_type, D_type_char_skill[skill_level][skill_type].casting_millisec);
		return	(-1);
	}
	
	return	(0);
}

	
void	D_SetClient_skill_cancel(const int fd)
{
	D_client[fd].action_skill		= 0;
	D_client[fd].remain_action_skill	= 0;
}


int	D_SetClient_skill_learn(const int fd, const int skill_type, int *change_buff)
{
	int	skill_level, learn_req_skill;


	memset(change_buff, 0, SIZEOF_INT*(PASSIVE_BUFF_CNT+EQUIP_PASSIVE_BUFF_CNT));
	skill_level = D_client[fd].skill_slot[skill_type-1];

	//check max_level
	if( D_type_char_skill[1][skill_type].max_level == skill_level )
	{
		U_Printf("[DB]Error fd(%d) skill_type(%d) max_level(%d) bad skill_level(%d)\n",
				fd, skill_type, D_type_char_skill[1][skill_type].max_level, skill_level);
		return	(-1);
	}

	//check learn_req_skill, learn_req_skill_level
	if( (learn_req_skill = D_type_char_skill[skill_level+1][skill_type].learn_req_skill) )
	{
		if( D_type_char_skill[skill_level+1][skill_type].learn_req_skill_level > D_client[fd].skill_slot[learn_req_skill-1] )
		{
		U_Printf("[DB]Error fd(%d) skill_type(%d) skill_level(%d) learn_req_skill(%d) learn_req_skill_level(%d) bad skill_level(%d)\n",
				fd, skill_type, skill_level, learn_req_skill,
				D_type_char_skill[skill_level+1][skill_type].learn_req_skill_level, D_client[fd].skill_slot[learn_req_skill-1]);
			return	(-1);
		}
	}

	//check learn_req_skill_point
	if( D_type_char_skill[skill_level+1][skill_type].learn_req_skill_point > D_client[fd].skill_point )
	{
		U_Printf("[DB]Error fd(%d) skill_type(%d) skill_level(%d) learn_req_skill_point(%d) bad skill_point(%d)\n",
			fd, skill_type, skill_level, D_type_char_skill[skill_level+1][skill_type].learn_req_skill_point, D_client[fd].skill_point);
		return	(-1);
	}

	//check learn_req_level
	if( D_type_char_skill[skill_level+1][skill_type].learn_req_level > D_client[fd].level )
	{
		U_Printf("[DB]Error fd(%d) skill_type(%d) skill_level(%d) learn_req_level(%d) bad level(%d)\n",
				fd, skill_type, skill_level, D_type_char_skill[skill_level+1][skill_type].learn_req_level, D_client[fd].level);
		return	(-1);
	}	

	//check learn_req_quest
	//......................

	//set skill_point, skill_level
	D_client[fd].skill_point	-= D_type_char_skill[skill_level+1][skill_type].learn_req_skill_point;	
	++(D_client[fd].skill_slot[skill_type-1]);


	//passive buff	
	if( D_type_char_skill[skill_level+1][skill_type].method == 3 )
	{
		D_set_client_passive_buff(fd, skill_type, change_buff);
	}
	else if( D_type_char_skill[skill_level+1][skill_type].method == 2 )
	{
		if( D_type_item[D_client[fd].item_slot[D_type_char_skill[skill_level+1][skill_type].req_equip_slot].type].class 
									== D_type_char_skill[skill_level+1][skill_type].req_equip_class )
			D_set_client_passive_buff(fd, skill_type, change_buff);
	}

	return	(0);
}


int	D_SetClient_transform(const int fd, const int transform_type)
{
	//check transform_type
	if( (D_client[fd].transform_type) && (transform_type) )
	{
		U_Printf("[DB]Error fd(%d) transform(%d) bad transform_type(%d)\n", fd, D_client[fd].transform_type, transform_type);	
		return	(-1);
	}	

	//set transform_type
	D_client[fd].transform_type	= transform_type;

	return	(0);
}


//______________________________________________________________
//
int	D_Char_attack_mob(const int char_id, const int mob_id, const int attack_type, int *mob_hp, int *damage_type, int *damage) 
{
	float	gab_x, gab_z, add_att2;
	int	channel_link, add_i_att, att_area;


	channel_link	= D_client[char_id].channel_link;

	//check map
	if( D_client[char_id].map != D_mob[mob_id][channel_link].map )
	{
		U_Printf("[DB]Error fd(%d) map(%d) mob_id(%d) map(%d)\n", char_id, D_client[char_id].map, mob_id, D_mob[mob_id][channel_link].map);
		return	(-1);
	}

	//check table_idx
	if( D_mob[mob_id][channel_link].table_idx )
	{
		U_Printf("[DB]Error fd(%d) mob_id(%d) table_idx(%d)\n", char_id, mob_id, D_mob[mob_id][channel_link].table_idx);
		return	(-1);
	}
	
	//check stair
	//if( D_map[D_client[char_id].map].attr[(int)(D_client[char_id].x)][(int)(D_client[char_id].z)] != D_mob[mob_id][channel_link].stair ) 
	//{
	//	U_Printf("[DB]Error fd(%d) map_attr(%d) mob_id(%d) stair(%d)\n",
	//				char_id, D_map[D_client[char_id].map].attr[(int)(D_client[char_id].x)][(int)(D_client[char_id].z)],
	//				mob_id, D_mob[mob_id][channel_link].stair);
	//	return	(-1);
	//}
	
	//check att area
	gab_x		= D_client[char_id].x - D_mob[mob_id][channel_link].x;
	gab_z		= D_client[char_id].z - D_mob[mob_id][channel_link].z;
	att_area	= D_get_client_att_area(char_id) + 10; 

	//if( (gab_x < -att_area) || (gab_x > att_area) || (gab_z < -att_area) || (gab_z > att_area) )
	//{
	//	U_Printf("[DB]Error fd(%d) att_area(%d) char_x(%f) char_z(%f) mob_id(%d) mob_x(%f) mob_z(%f)\n", char_id, att_area,
	//			D_client[char_id].x, D_client[char_id].z, mob_id, D_mob[mob_id][channel_link].x, D_mob[mob_id][channel_link].z);
	//	return	(-1);
	//}

	//check evasion
	if( D_check_evasion(channel_link, CHAR_ATTACK_MOB, char_id, mob_id) )
	{
		*mob_hp		= D_mob[mob_id][channel_link].hp;
		*damage_type	= _DAMAGE_MISS;
		*damage		= 0;
		return	(0);
	}

	//check attack type
	if( D_check_char_attack_type(char_id, attack_type, &add_att2, &add_i_att) )
	{
		U_Printf("[DB]Error fd(%d) D_check_char_attack_type()\n", char_id);	
		return	(-1);
	}

	//damage
	*damage_type	= _DAMAGE_NORMAL;	
	*damage		= D_compute_damage(channel_link, CHAR_ATTACK_MOB, char_id, mob_id, 0, add_att2);

	//check critical
	if( D_check_critical(channel_link, CHAR_ATTACK_MOB, char_id, mob_id, 0) )
	{
		*damage_type	|= _DAMAGE_CRITICAL;
		*damage		*= 2.0f; 
	}
		
	//check damage
	if( *damage > 9999 )
		*damage	= 9999;
	
	//check impact
	if( D_check_impact(channel_link, CHAR_ATTACK_MOB, char_id, mob_id, add_i_att) )
		*damage_type	|= _DAMAGE_IMPACT;

	//set hp, status
	if( (D_mob[mob_id][channel_link].hp -= *damage) < 1 )
	{
		D_mob[mob_id][channel_link].hp	= 0;	
		D_mob[mob_id][channel_link].status	= _MOB_DEATH;
	}
	
	*mob_hp	= D_mob[mob_id][channel_link].hp;
	
	return	(0);
}

		
int	D_Mob_attack_char(const int channel_link, const int mob_id, const int char_id, int *char_hp, int *damage_type, int *damage)
{
	float	gab_x, gab_z;
	int	att_area;


	//check channel_link
	if( D_client[char_id].channel_link != channel_link )
		return	(-1);

	//check map
	if( D_client[char_id].map != D_mob[mob_id][channel_link].map )
		return	(-1);
	
	//check stair
	if( D_map[D_client[char_id].map].attr[(int)(D_client[char_id].x)][(int)(D_client[char_id].z)] != D_mob[mob_id][channel_link].stair ) 
		return	(-1);
	
	//check area attack
	gab_x		= D_client[char_id].x - D_mob[mob_id][channel_link].x;
	gab_z		= D_client[char_id].z - D_mob[mob_id][channel_link].z;
	att_area	= D_get_mob_att_area(channel_link, mob_id) + 10; 

	if( (gab_x < -att_area) || (gab_x > att_area) || (gab_z < -att_area) || (gab_z > att_area) )
		return	(-1);

	//check evasion
	if( D_check_evasion(channel_link, MOB_ATTACK_CHAR, mob_id, char_id) )
	{	
		*char_hp	= D_client[char_id].hp;
		*damage_type	= _DAMAGE_MISS;
		*damage		= 0;
		return	(0);
	}		

	//damage
	*damage_type	= _DAMAGE_NORMAL;	
	*damage		= D_compute_damage(channel_link, MOB_ATTACK_CHAR, mob_id, char_id, 0, 0.0f);

	//check critical
	if( D_check_critical(channel_link, MOB_ATTACK_CHAR, mob_id, char_id, 0) )
	{
		*damage_type	|= _DAMAGE_CRITICAL;
		*damage		*= 2.0f; 
	}
		
	//check damage
	if( *damage > 9999 )
		*damage	= 9999;

	//check impact
	if( D_check_impact(channel_link, MOB_ATTACK_CHAR, mob_id, char_id, 0) )
		*damage_type	|= _DAMAGE_IMPACT;

	//set char hp, status, transform_type
	if( (D_client[char_id].hp -= *damage) < 1 )
	{
		D_client[char_id].hp			= 0;	
		D_client[char_id].status		= _USER_DEAD;
		D_client[char_id].transform_type	= 0;
	}
	
	*char_hp	= D_client[char_id].hp;

	return	(0);
}


void	D_Char_kill_mob(const int channel_link, const int char_id, const int mob_id, int *level_up, int *acquire_exp, int *item_5id_list)
{
	int			i, j, mob_type, drop_type, rand_val, equip_group, option_group, grade, level;
	float			exp, level_penalty;
	struct	item_object	item;
	

	memset(item_5id_list, 0, SIZEOF_INT*5);
	memset(&item, 0, ITEM_DATA_LEN+ITEM_SERIAL_LEN);

	if( D_mob[mob_id][channel_link].level >= D_client[char_id].level )	
		level_penalty	= 1.0f;
	else if( (D_client[char_id].level - D_mob[mob_id][channel_link].level) < 10 )
		level_penalty	= (10 - (D_client[char_id].level - D_mob[mob_id][channel_link].level))/10.0f;
	else
		level_penalty	= 0.0f;

	mob_type	= D_mob[mob_id][channel_link].type;
	*level_up	= 0;
	*acquire_exp	= D_type_mob[mob_type].exp * level_penalty;
	exp		= D_client[char_id].exp + *acquire_exp;
	drop_type	= D_type_mob[mob_type].drop_type;

	
	while( (D_client[char_id].level < D_max_level) && (exp >= D_level[D_client[char_id].level].exp) )
	{
		
		++(*level_up);	
		exp	-=  D_level[D_client[char_id].level].exp;
		
		//level up
		D_set_client_level_up(char_id);
	}

	//set exp
	if( D_client[char_id].level == D_max_level )	
		D_client[char_id].exp	= 0;	
	else
		D_client[char_id].exp	= exp;

	
	//drop item group
	for( i=0; i<3; i++ )
	{
		rand_val	= U_GetRand_val(100000000);
		
		for( j=0; j<20; j++ )
		{
			if( (D_type_drop[drop_type].group[i][j].drop_prob * level_penalty) > rand_val )
			{   
				if( (D_type_drop[drop_type].group[i][j].item_type == 0) )
					break;
					
				if( (item_5id_list[i] = D_get_item_id()) == 0 )
					break;

				item.type	= D_type_drop[drop_type].group[i][j].item_type;

				memset(&item, 0, ITEM_DATA_LEN+ITEM_SERIAL_LEN+1);
				D_make_item(&item, D_type_drop[drop_type].group[i][j].item_type, 1, D_client[char_id].char_name);
			
				if( D_set_item_drop(item_5id_list[i], channel_link, D_mob[mob_id][channel_link].map,
					D_mob[mob_id][channel_link].x, 0, D_mob[mob_id][channel_link].z, &item, D_client[char_id].char_name) )
				{
					D_return_item_id(item_5id_list[i]);
					item_5id_list[i]	= 0;
				}	
				break;
			}
		}
	}
	
	//drop enchant 
	if( (D_type_drop[drop_type].enchant_drop_prob * level_penalty) > U_GetRand_val(100000000) )
	{
		rand_val	= U_GetRand_val(100000000);
	
		for( i=0; i<=9; i++ )	
		{
			if( (D_type_drop[drop_type].enchant_grade_prob[i] > rand_val) )
			{
				level		= D_type_drop[drop_type].enchant_level;
				equip_group
				= D_type_drop[drop_type].enchant_equip_group[U_GetRand_val(D_type_drop[drop_type].enchant_equip_group_num)];
				option_group
				= D_type_drop[drop_type].enchant_option_group[U_GetRand_val(D_type_drop[drop_type].enchant_option_group_num)];
				grade		= i+1;

				if( D_drop_enchant[level][equip_group][option_group][grade] == 0 )
					break;	
				
				if( (item_5id_list[3] = D_get_item_id()) == 0 )
					break;

				memset(&item, 0, ITEM_DATA_LEN+ITEM_SERIAL_LEN+1);
				D_make_item(&item, D_drop_enchant[level][equip_group][option_group][grade], 0, D_client[char_id].char_name);
				if( D_set_item_drop(item_5id_list[3], channel_link, D_mob[mob_id][channel_link].map,
					D_mob[mob_id][channel_link].x, 0, D_mob[mob_id][channel_link].z, &item, D_client[char_id].char_name) )
				{
					D_return_item_id(item_5id_list[3]);
					item_5id_list[3]	= 0;
				}
				break;
			}	
		}
	}

	//drop money
	if( (D_type_mob[D_mob[mob_id][channel_link].type].money_drop_prob * level_penalty) > U_GetRand_val(100000000) )
	{
		if( (item_5id_list[4] = D_get_item_id()) )
		{
			memset(&item, 0, ITEM_DATA_LEN+ITEM_SERIAL_LEN+1);
			D_make_item(&item, ITEM_TYPE_MONEY,
				D_type_mob[mob_type].money_min + U_GetRand_val(D_type_mob[mob_type].money_max - D_type_mob[mob_type].money_min +1),
				D_client[char_id].char_name);
			if( D_set_item_drop(item_5id_list[4], channel_link, D_mob[mob_id][channel_link].map, D_mob[mob_id][channel_link].x, 0,
										D_mob[mob_id][channel_link].z, &item, D_client[char_id].char_name) )
			{
					D_return_item_id(item_5id_list[4]);
					item_5id_list[4]	= 0;
			}
		}
	}
}


int	D_Char_check_warp(const int char_id,  const int mob_id, const int item_slot, int *dest_map, int *dest_x, int *dest_z, int *warp_num)
{
	float	gab_x, gab_z;
	int	table_idx, channel_link;


	channel_link		= D_client[char_id].channel_link;
	table_idx		= D_mob[mob_id][channel_link].table_idx;

	//check npc_type
	if( D_type_mob[D_mob[mob_id][channel_link].type].npc_type != NPC_TYPE_WARP )
	{
		U_Printf("[DB]Error fd(%d) mob_id(%d) npc_type(%d)\n", char_id, mob_id, D_type_mob[D_mob[mob_id][channel_link].type].npc_type);
		return	(-1);
	}

	//check map
	if( D_client[char_id].map != D_mob[mob_id][channel_link].map )
	{
		U_Printf("[DB]Error fd(%d) map(%d) mob_id(%d) map(%d)\n", char_id, D_client[char_id].map, mob_id, D_mob[mob_id][channel_link].map);
		return	(-1);
	}
	
	//check area
	gab_x	= D_client[char_id].x - D_mob[mob_id][channel_link].x;
	gab_z	= D_client[char_id].z - D_mob[mob_id][channel_link].z;
 
	if( (gab_x < -10) || (gab_x > 10) || (gab_z < -10) || (gab_z > 10) )
	{
		U_Printf("[DB]Error fd(%d) char_x(%f) char_z(%f) mob_id(%d) mob_x(%f) mob_z(%f)\n", char_id,
				D_client[char_id].x, D_client[char_id].z, mob_id, D_mob[mob_id][channel_link].x, D_mob[mob_id][channel_link].z);
		return	(-1);
	}
	
	//check quest
	//....................

	//check level
	if( D_type_warp[table_idx].req_level && (D_type_warp[table_idx].req_level < D_client[char_id].level) )
	{
		U_Printf("[DB]Error fd(%d) level(%d) table_idx(%d) req_level(%d)\n",
				char_id, D_client[char_id].level, table_idx, D_type_warp[table_idx].req_level);	
		return	(-1);
	}

	//check item
	if( D_type_warp[table_idx].req_item && (D_type_warp[table_idx].req_item != D_client[char_id].item_slot[item_slot].type) )
	{
		U_Printf("[DB]Error fd(%d) item_slot(%d) type(%d) table_idx(%d) req_item(%d)\n",
				char_id, item_slot, D_client[char_id].item_slot[item_slot].type, table_idx, D_type_warp[table_idx].req_item);	
		return	(-1);
	}

	*dest_map	= D_type_warp[table_idx].dest_map;
	*dest_x		= D_type_warp[table_idx].dest_x;
	*dest_z		= D_type_warp[table_idx].dest_z;
	*warp_num	= table_idx;	

	return	(0);	
}

	
int	D_Char_buy_store_item(const int char_id, const int mob_id,
				const int store_item_slot, const int item_cnt, const int char_item_slot, int *change_buff)
{
	float			gab_x, gab_z;
	int			channel_link, item_type, buy_price;
	struct	item_object	item;

	
	channel_link	= D_client[char_id].channel_link;

	//check npc
	if( D_type_mob[D_mob[mob_id][channel_link].type].npc_type != NPC_TYPE_STORE )
	{
		U_Printf("[DB]Error fd(%d) mob_id(%d) npc_type(%d)\n", char_id, mob_id, D_type_mob[D_mob[mob_id][channel_link].type].npc_type);
		return	(-1);
	}

	//check map
	if( D_client[char_id].map != D_mob[mob_id][channel_link].map )
	{
		U_Printf("[DB]Error fd(%d) map(%d) mob_id(%d) map(%d)\n", char_id, D_client[char_id].map, mob_id, D_mob[mob_id][channel_link].map);
		return	(-1);
	}
	
	//check area
	gab_x	= D_client[char_id].x - D_mob[mob_id][channel_link].x;
	gab_z	= D_client[char_id].z - D_mob[mob_id][channel_link].z;
 
	if( (gab_x < -10) || (gab_x > 10) || (gab_z < -10) || (gab_z > 10) )
	{
		U_Printf("[DB]Error fd(%d) char_x(%f) char_z(%f) mob_id(%d) mob_x(%f) mob_z(%f)\n", char_id,
			D_client[char_id].x, D_client[char_id].z, mob_id, D_mob[mob_id][channel_link].x, D_mob[mob_id][channel_link].z);
		return	(-1);
	}

	//check store_item
	if( (item_type = D_type_store[D_mob[mob_id][channel_link].table_idx].item_type[store_item_slot]) == 0 )
	{
		U_Printf("[DB]Error fd(%d) mob_id(%d) table_idx(%d) bad item_type(%d)\n",
						char_id, mob_id, D_mob[mob_id][channel_link].table_idx, item_type);
		return	(-1);
	}

	//check item_cnt
	if( D_type_item[item_type].stack_num )
	{
		if( (item_cnt < 1) || (item_cnt > D_type_item[item_type].stack_num) )
		{
			U_Printf("[DB]Error fd(%d) mob_id(%d) item_type(%d) stack_num(%d) bad item_cnt(%d)\n",
						char_id, mob_id, item_type, D_type_item[item_type].stack_num, item_cnt);
			return	(-1);
		}
	}
	else
	{
		if( item_cnt != 1 )
		{
			U_Printf("[DB]Error fd(%d) mob_id(%d) item_type(%d) stack_num(%d) bad item_cnt(%d)\n",
						char_id, mob_id, item_type, D_type_item[item_type].stack_num, item_cnt);
			return	(-1);
		}	
	}

	//check money
	buy_price	= D_type_item[item_type].buy_price*item_cnt;
	
	if( (D_client[char_id].money - buy_price) < 0 )
	{
		U_Printf("[DB]Error fd(%d) mob_id(%d) table_idx(%d) item_type(%d) item_cnt(%d) buy_price(%d) bad money(%d)\n",
				char_id, mob_id, D_mob[mob_id][channel_link].table_idx, item_type, item_cnt, buy_price, D_client[char_id].money);
		return	(-1);
	}

	//set item
	memset(&item, 0, ITEM_DATA_LEN+ITEM_SERIAL_LEN+1);
	D_make_item(&item, item_type, item_cnt, D_client[char_id].char_name);
	
	if( D_set_client_item(char_id, char_item_slot, &item, change_buff) )
	{
		U_Printf("[DB]Error fd(%d) D_set_client_item()\n", char_id);
		return	(-1);
	}
	
	//set money
	D_client[char_id].money	-= buy_price;	 
	
	return	(0);
}
	

int	D_Char_sell_store_item(const int char_id, const int mob_id, const int item_slot, const int item_cnt)
{
	float			gab_x, gab_z;
	int			channel_link, item_type, sell_price;


	channel_link	= D_client[char_id].channel_link;

	//check npc_type
	if( D_type_mob[D_mob[mob_id][channel_link].type].npc_type != NPC_TYPE_STORE )
	{
		U_Printf("[DB]Error fd(%d) mob_id(%d) npc_type(%d)\n", char_id, mob_id, D_type_mob[D_mob[mob_id][channel_link].type].npc_type);
		return	(-1);
	}

	//check map
	if( D_client[char_id].map != D_mob[mob_id][channel_link].map )
	{
		U_Printf("[DB]Error fd(%d) map(%d) mob_id(%d) map(%d)\n", char_id, D_client[char_id].map, mob_id, D_mob[mob_id][channel_link].map);
		return	(-1);
	}
	
	//check area
	gab_x	= D_client[char_id].x - D_mob[mob_id][channel_link].x;
	gab_z	= D_client[char_id].z - D_mob[mob_id][channel_link].z;
 
	if( (gab_x < -10) || (gab_x > 10) || (gab_z < -10) || (gab_z > 10) )
	{
		U_Printf("[DB]Error fd(%d) char_x(%f) char_z(%f) mob_id(%d) mob_x(%f) mob_z(%f)\n",
			char_id, D_client[char_id].x, D_client[char_id].z, mob_id, D_mob[mob_id][channel_link].x, D_mob[mob_id][channel_link].z);
		return	(-1);
	}

	//check item_type
	if( (item_type = D_client[char_id].item_slot[item_slot].type) == 0  )
	{
		U_Printf("[DB]Error fd(%d) mob_id(%d) item_type(%d)\n", char_id, mob_id, item_type);
		return	(-1);
	}
	
	//check item_cnt
	if( D_type_item[item_type].stack_num )
	{
		if( (item_cnt < 1) || (item_cnt > D_type_item[item_type].stack_num) )
		{
			U_Printf("[DB]Error fd(%d) mob_id(%d) item_type(%d) stack_num(%d) bad item_cnt(%d)\n",
						char_id, mob_id, item_type, D_type_item[item_type].stack_num, item_cnt);
			return	(-1);
		}
	}
	else
	{
		if( item_cnt != 1 )
		{
			U_Printf("[DB]Error fd(%d) mob_id(%d) item_type(%d) stack_num(%d) bad item_cnt(%d)\n",
						char_id, mob_id, item_type, D_type_item[item_type].stack_num, item_cnt);
			return	(-1);
		}	
	}

	//clear item
	if( D_type_item[item_type].stack_num )
	{
		if( D_client[char_id].item_slot[item_slot].stack_num < item_cnt )
		{
			U_Printf("[DB]Error fd(%d) mob_id(%d) item_cnt(%d) item_type(%d) stack_num(%d)\n",
					char_id, mob_id, item_cnt, item_type, D_client[char_id].item_slot[item_slot].stack_num);
			return	(-1);
		}

		if( (D_client[char_id].item_slot[item_slot].stack_num -= item_cnt) == 0 )
			memset(D_client[char_id].item_slot + item_slot, 0, ITEM_DATA_LEN+ITEM_SERIAL_LEN);	
	}
	else
	{
		memset(D_client[char_id].item_slot + item_slot, 0, ITEM_DATA_LEN+ITEM_SERIAL_LEN);	
	}
	
	
	//set  money
	sell_price	= D_type_item[item_type].sell_price*item_cnt;
	
	if( (D_client[char_id].money + sell_price) < 0 )
		D_client[char_id].money	= D_max_int;	
	else
		D_client[char_id].money	+= sell_price;		

	return	(0);
}

	
int	D_Char_skill_action(const int char_id, struct body1_CG_CHAR_SKILL_ACTION *body1, char *client_send_buf,
										char *mobserv_send_buf, struct skill_action_result *result)
{
	int					channel_link, skill_level, i, j, att_area;
	int					buff_value[MAX_SKILL_ORDER_CNT], buff_sec[MAX_SKILL_ORDER_CNT];
	int					damage, damage_type;
	float					gab_x, gab_z;
	struct	skill_ability			ability;
	struct	body2_CG_CHAR_SKILL_ACTION	*body2;
	struct	body1_GC_CHAR_SKILL_ACTION	*client_send_body1;
	struct	body2_GC_CHAR_SKILL_ACTION	*client_send_body2;
	struct	body1_GM_CHAR_SKILL_ACTION	*mobserv_send_body1;
	struct	body2_GM_CHAR_SKILL_ACTION	*mobserv_send_body2;


	channel_link	= D_client[char_id].channel_link;

	//check skill level
	if( (skill_level = D_client[char_id].skill_slot[body1->skill_type-1]) == 0 )
	{
		U_Printf("[DB]Error fd(%d) bad skill_level(%d)\n", char_id, D_client[char_id].skill_slot[body1->skill_type-1]);
		return	(-1);
	}

	//check method
	if( D_type_char_skill[skill_level][body1->skill_type].method < 4 )
	{
		U_Printf("[DB]Error fd(%d) skill_type(%d) bad method(%d)\n",
					char_id, body1->skill_type, D_type_char_skill[skill_level][body1->skill_type].method);
		return	(-1);
	}

	//check action_cnt
	if( (body1->action_cnt == 0) || (body1->action_cnt > D_type_char_skill[skill_level][body1->skill_type].action_cnt) )
	{
		U_Printf("[DB]Error fd(%d) skill_type(%d) action_cnt(%d) bad action_cnt(%d)\n",
				char_id, body1->skill_type, D_type_char_skill[skill_level][body1->skill_type].action_cnt, body1->action_cnt);
		return (-1);
	}

	//check req_equip_class
	if( D_type_char_skill[skill_level][body1->skill_type].req_equip_class )
	{
		if( D_type_item[D_client[char_id].item_slot[D_type_char_skill[skill_level][body1->skill_type].req_equip_slot].type].class
									!= D_type_char_skill[skill_level][body1->skill_type].req_equip_class )
		{
			U_Printf("[DB]Error fd(%d) skill_type(%d) req_equip_slot(%d) req_equip_class(%d) bad item_class(%d)\n",
				char_id, body1->skill_type, D_type_char_skill[skill_level][body1->skill_type].req_equip_slot,
				D_type_char_skill[skill_level][body1->skill_type].req_equip_class,
			D_type_item[D_client[char_id].item_slot[D_type_char_skill[skill_level][body1->skill_type].req_equip_slot].type].class);
			return	(-1);
		}
	}
	
	//check target distance
	gab_x		= D_client[char_id].x - body1->target_x;
	gab_z		= D_client[char_id].z - body1->target_z;
	att_area	= D_type_char_skill[skill_level][body1->skill_type].target_distance +2;	

	//if( (gab_x < -att_area) || (gab_x > att_area) || (gab_z < -att_area) || (gab_z > att_area) )
	//{
	//	U_Printf("[DB]Error fd(%d) skill_type(%d) att_area(%d) char_x(%f) char_z(%f) target_x(%f) target_z(%f)\n",
	//		char_id, body1->skill_type, att_area, D_client[char_id].x, D_client[char_id].z, body1->target_x, body1->target_z);	
	//	return	(-1);
	//}

	//check target_apply
	if( D_type_char_skill[skill_level][body1->skill_type].target_apply == 2 )
	{
		if( body1->target_char_cnt )
		{
			U_Printf("[DB]Error fd(%d) skill_type(%d) target_apply(%d) bad target_mob_cnt(%d) target_char_cnt(%d)\n",
					char_id, body1->skill_type, D_type_char_skill[skill_level][body1->skill_type].target_apply,
					body1->target_mob_cnt, body1->target_char_cnt);
			return (-1);
		}	
	}
	else
	{
		if( body1->target_mob_cnt )
		{
			U_Printf("[DB]Error fd(%d) skill_type(%d) target_apply(%d) bad target_mob_cnt(%d) target_char_cnt(%d)\n",
					char_id, body1->skill_type, D_type_char_skill[skill_level][body1->skill_type].target_apply,
					body1->target_mob_cnt, body1->target_char_cnt);
			return (-1);
		}	
	}
		
	//check action_skill
	if( D_client[char_id].action_skill )
	{
		//check action skill
		if( D_client[char_id].action_skill != body1->skill_type )
		{
			U_Printf("[DB]Error fd(%d) action_skill(%d) bad skill_type(%d)\n",
						char_id, D_client[char_id].action_skill, body1->skill_type);
			return (-1);
		}

		//check remain_action_skill
		if( body1->action_cnt != D_client[char_id].remain_action_skill )
		{
			U_Printf("[DB]Error fd(%d) skill_type(%d) remain_aciton_skill(%d) bad action_cnt(%d)\n",
						char_id, body1->skill_type, D_client[char_id].remain_action_skill, body1->action_cnt);
			return (-1);
		}
	
		//set action_skill, remain_action skill
		if( --(D_client[char_id].remain_action_skill) == 0 )
			D_client[char_id].action_skill	= 0;
	}
	else
	{
		//set action_skill, remain_action skill
		D_client[char_id].action_skill		= body1->skill_type;
		D_client[char_id].remain_action_skill	= D_type_char_skill[skill_level][body1->skill_type].action_cnt;

		//check cool time
		if( D_client[char_id].cool_end_sec[D_type_char_skill[skill_level][body1->skill_type].cool_serise] > G_Sec )
		{
			U_Printf("[DB]Error fd(%d) skill_type(%d) bad cool_end_sec(%d) G_Sec(%d)\n", char_id, body1->skill_type,
					D_client[char_id].cool_end_sec[D_type_char_skill[skill_level][body1->skill_type].cool_serise], G_Sec);
			return	(-1);
		}

		//check ap
		if( D_client[char_id].ap < D_type_char_skill[skill_level][body1->skill_type].use_ap )
		{
			U_Printf("[DB]Error fd(%d) skill_type(%d) use_ap(%d) bad ap(%d)\n",
					char_id, body1->skill_type, D_type_char_skill[skill_level][body1->skill_type].use_ap, D_client[char_id].ap);
			return	(-1);
		}
	
		//set ap, cool_end_sec, action_skill, remain_action_skill
		D_client[char_id].ap	-= D_type_char_skill[skill_level][body1->skill_type].use_ap;
		D_client[char_id].cool_end_sec[D_type_char_skill[skill_level][body1->skill_type].cool_serise]
							= (D_type_char_skill[skill_level][body1->skill_type].cool_millisec)/1000 + G_Sec;
		if( --(D_client[char_id].remain_action_skill) == 0 )
			D_client[char_id].action_skill	= 0;
	}


	//get skill ability
	D_get_skill_ability(body1->skill_type, skill_level, &(result->buff_cnt), result->buff_serise, buff_value, buff_sec, &ability);

	//skill action
	result->buff_char_cnt	= 0;
	result->buff_mob_cnt	= 0;
	body2			= (struct body2_CG_CHAR_SKILL_ACTION *)(body1 + 1);
	client_send_body1	= (struct body1_GC_CHAR_SKILL_ACTION *)(client_send_buf + SIZEOF_HEADER_NORMAL);
	client_send_body2
		= (struct body2_GC_CHAR_SKILL_ACTION *)(client_send_buf + SIZEOF_HEADER_NORMAL + sizeof(struct body1_GC_CHAR_SKILL_ACTION));	
	mobserv_send_body1	= (struct body1_GM_CHAR_SKILL_ACTION *)(mobserv_send_buf + SIZEOF_HEADER_NORMAL);
	mobserv_send_body2
		= (struct body2_GM_CHAR_SKILL_ACTION *)(mobserv_send_buf + SIZEOF_HEADER_NORMAL + sizeof(struct body1_GM_CHAR_SKILL_ACTION));	

	//set client_send_body1
	client_send_body1->char_id		= char_id;
	client_send_body1->char_ap		= D_client[char_id].ap;
	client_send_body1->attack_id		= body1->attack_id;
	client_send_body1->skill_type		= body1->skill_type;
	client_send_body1->action_cnt		= body1->action_cnt;
	client_send_body1->dir			= body1->dir;
	client_send_body1->speed		= body1->speed;
	client_send_body1->x			= body1->x;
	client_send_body1->y			= body1->y;
	client_send_body1->z			= body1->z;
	client_send_body1->target_x		= body1->target_x;
	client_send_body1->target_y		= body1->target_y;
	client_send_body1->target_z		= body1->target_z;
	client_send_body1->target_mob_cnt	= 0;
	client_send_body1->target_char_cnt	= 0;

	//set mobserv_send_body1
	mobserv_send_body1->char_id		= char_id;
	mobserv_send_body1->dir			= body1->dir;
	mobserv_send_body1->x			= body1->x;
	mobserv_send_body1->y			= body1->y;
	mobserv_send_body1->z			= body1->z;
	mobserv_send_body1->target_mob_cnt	= 0;


	//mob
	for( i=0; i<body1->target_mob_cnt; i++ )
	{
		//check target_id
		if( (body2->target_id < 1) || (body2->target_id > D_max_mob) )
		{
			U_Printf("[DB]Error fd(%d) bad mob_id(%d)\n", char_id, body2->target_id);
			return	(-1);
		}	

		//check status
		if( D_mob[body2->target_id][channel_link].status >= _MOB_DEATH )
			continue;

		//check map
		if( D_client[char_id].map != D_mob[body2->target_id][channel_link].map )
		{
			U_Printf("[DB]Error fd(%d) map(%d) mob_id(%d) map(%d)\n",
						char_id, D_client[char_id].map, body2->target_id, D_mob[body2->target_id][channel_link].map);
			return	(-1);
		}
	
		//check table_idx
		if( D_mob[body2->target_id][channel_link].table_idx )
		{
			U_Printf("[DB]Error fd(%d) mob_id(%d) table_idx(%d)\n",
							char_id, body2->target_id, D_mob[body2->target_id][channel_link].table_idx);
			return	(-1);
		}

		//check area attack  
		gab_x		= D_mob[body2->target_id][channel_link].x - body1->target_x;
		gab_z		= D_mob[body2->target_id][channel_link].z - body1->target_z;

		if( D_type_char_skill[skill_level][body1->skill_type].range_x > D_type_char_skill[skill_level][body1->skill_type].range_z )
			att_area	= D_type_char_skill[skill_level][body1->skill_type].range_x + 10;	
		else
			att_area	= D_type_char_skill[skill_level][body1->skill_type].range_z + 10;	
			
		//if( (gab_x < -att_area) || (gab_x > att_area) || (gab_z < -att_area) || (gab_z > att_area) )
		//{
		//	U_Printf("[DB]Error fd(%d) skill_type(%d) mob_id(%d) x(%f) z(%f) target_x(%f) target_z(%f)\n",
		//				char_id, body1->skill_type, body2->target_id, D_mob[body2->target_id][channel_link].x,
		//				D_mob[body2->target_id][channel_link].z, body1->target_x, body1->target_z);	
		//	return	(-1);
		//}	
			
		//damage type
		if( ability.damage_type )
		{
			//compute damge
			D_compute_skill_damage(channel_link, CHAR_ATTACK_MOB, char_id, body2->target_id, &ability, &damage, &damage_type);
				
			//check damage
			if( damage > 9999 )
				damage	= 9999;
		
			//set mob hp, status
			if( (D_mob[body2->target_id][channel_link].hp -= damage) < 1 )
			{
				D_mob[body2->target_id][channel_link].hp	= 0;
				D_mob[body2->target_id][channel_link].status	= _MOB_DEATH;
			}
				
			++(client_send_body1->target_mob_cnt);
				
			//set client_send_body2
			client_send_body2->target_id	= body2->target_id;
			client_send_body2->target_hp	= D_mob[body2->target_id][channel_link].hp;
			client_send_body2->damage_type	= damage_type;
			client_send_body2->damage	= damage;	
				
			++(mobserv_send_body1->target_mob_cnt);
		
			//set mobserv_send_body2
			mobserv_send_body2->target_id	= body2->target_id;
			mobserv_send_body2->target_hp	= D_mob[body2->target_id][channel_link].hp;
			mobserv_send_body2->damage_type	= damage_type;
			mobserv_send_body2->damage	= damage;
		
			++client_send_body2;
			++mobserv_send_body2;
		}
			
		//set mob buff
		if( (result->buff_cnt) && (D_mob[body2->target_id][channel_link].status != _MOB_DEATH) )
		{
			result->buff_mob_list[(result->buff_mob_cnt)++]	= body2->target_id;
			
			for( j=0; j<(result->buff_cnt); j++ )
			{
				//stun
				if( result->buff_serise[j] == _BUFF_DE_ACTIVE_STUN )
				{
					if( D_check_stun(channel_link, CHAR_ATTACK_MOB, char_id, body2->target_id, buff_value[j]) )
						(client_send_body2 - 1)->damage_type	|= _DAMAGE_STUN;
					else
						continue;
				}

				D_SetMob_buff(channel_link, body2->target_id, result->buff_serise[j], buff_value[j], buff_sec[j]);
			}
		}
		
		++body2;
	}

	//char
	for( i=0; i<body1->target_char_cnt; i++ )
	{
		//check target_id
		if( (body2->target_id < 1) || (body2->target_id >= MAX_FD) )
		{
			U_Printf("[DB]Error fd(%d) bad char_id(%d)\n", char_id, body2->target_id);
			return	(-1);
		}	

		//check status
		if( D_client[body2->target_id].status >= _USER_DEAD )
			continue;

		//check channel_link
		if( channel_link != D_client[body2->target_id].channel_link )
			continue;

		//check map
		if( D_client[char_id].map != D_client[body2->target_id].map )
			continue;

		//check area attack  
		gab_x		= D_client[body2->target_id].x - body1->target_x;
		gab_z		= D_client[body2->target_id].z - body1->target_z;

		if( D_type_char_skill[skill_level][body1->skill_type].range_x > D_type_char_skill[skill_level][body1->skill_type].range_z )
			att_area	= D_type_char_skill[skill_level][body1->skill_type].range_x + 10;	
		else
			att_area	= D_type_char_skill[skill_level][body1->skill_type].range_z + 10;	
			
		if( (gab_x < -att_area) || (gab_x > att_area) || (gab_z < -att_area) || (gab_z > att_area) )
			continue;
			
		//damage type
		if( ability.damage_type & _DAMAGE_HP_HEAL )
		{
			damage_type	= _DAMAGE_HP_HEAL;
			damage		= ability.hp_heal;	

			if( ((D_client[body2->target_id].hp += damage) < 0)
					|| (D_client[body2->target_id].hp > D_get_client_hp_max(body2->target_id)) )
				D_client[body2->target_id].hp	= D_get_client_hp_max(body2->target_id);
				
			++(client_send_body1->target_char_cnt);
				
			client_send_body2->target_id	= body2->target_id;
			client_send_body2->target_hp	= D_client[body2->target_id].hp;
			client_send_body2->damage_type	= damage_type;
			client_send_body2->damage	= damage;	
				
			++client_send_body2;
		}

		//set char buff
		if( result->buff_cnt )
		{
			result->buff_char_list[(result->buff_char_cnt)++]	= body2->target_id;
				
			for( j=0; j<(result->buff_cnt); j++ )
				D_SetClient_buff(body2->target_id, result->buff_serise[j], buff_value[j], buff_sec[j]);
		}

		++body2;
	}

	return	(0);
}	
	

int	D_Mob_skill_action(const int channel_link, const int mob_id,
					struct body1_MG_MOB_SKILL_ACTION *body1, char *client_send_buf, struct skill_action_result *result)
{
	int					i, j, att_area;
	int					buff_value[MAX_SKILL_ORDER_CNT], buff_sec[MAX_SKILL_ORDER_CNT];
	int					damage, damage_type;
	float					gab_x, gab_z;
	struct	skill_ability			ability;
	struct	body2_MG_MOB_SKILL_ACTION	*body2;
	struct	body1_GC_MOB_SKILL_ACTION	*client_send_body1;
	struct	body2_GC_MOB_SKILL_ACTION	*client_send_body2;


	//set ap
	if( D_type_mob_skill[body1->skill_type].action_cnt == body1->action_cnt )
	{
		if( (D_mob[mob_id][channel_link].ap -= D_type_mob_skill[body1->skill_type].use_ap) < 0 )
			D_mob[mob_id][channel_link].ap	= 0;
	}	

	//check target distance
	gab_x		= D_mob[mob_id][channel_link].x - body1->target_x;
	gab_z		= D_mob[mob_id][channel_link].z - body1->target_z;
	att_area	= D_type_mob_skill[body1->skill_type].target_distance +2;	

	if( (gab_x < -att_area) || (gab_x > att_area) || (gab_z < -att_area) || (gab_z > att_area) )
	{
		U_Printf("[DB]Error mob_id(%d) skill_type(%d) att_area(%d) mob_x(%f) mob_z(%f) target_x(%f) target_z(%f)\n",
		mob_id, body1->skill_type, att_area, D_mob[mob_id][channel_link].x, D_mob[mob_id][channel_link].z, body1->target_x, body1->target_z);
		return	(-1);
	}

	//get skill ability
	D_get_skill_ability(body1->skill_type, 0, &(result->buff_cnt), result->buff_serise, buff_value, buff_sec, &ability);

	//skill action
	result->buff_char_cnt		= 0;
	result->buff_mob_cnt		= 0;
	result->dead_char_cnt		= 0;
	body2			= (struct body2_MG_MOB_SKILL_ACTION *)(body1 + 1);
	client_send_body1	= (struct body1_GC_MOB_SKILL_ACTION *)(client_send_buf + SIZEOF_HEADER_NORMAL);
	client_send_body2
		= (struct body2_GC_MOB_SKILL_ACTION *)(client_send_buf + SIZEOF_HEADER_NORMAL + sizeof(struct body1_GC_MOB_SKILL_ACTION));	

	//set client_send_body1
	client_send_body1->mob_id		= mob_id;
	client_send_body1->skill_type		= body1->skill_type;
	client_send_body1->action_cnt		= body1->action_cnt;
	client_send_body1->dir			= body1->dir;
	client_send_body1->speed		= 0;
	client_send_body1->x			= D_mob[mob_id][channel_link].x;
	client_send_body1->y			= D_mob[mob_id][channel_link].y;
	client_send_body1->z			= D_mob[mob_id][channel_link].z;
	client_send_body1->target_x		= body1->target_x;
	client_send_body1->target_y		= body1->target_y;
	client_send_body1->target_z		= body1->target_z;
	client_send_body1->target_mob_cnt	= 0;
	client_send_body1->target_char_cnt	= 0;


	if( D_type_mob_skill[body1->skill_type].target_apply == 2 )
	{
		//char
		for( i=0; i<body1->target_cnt; i++ )
		{
			//check target_id
			if( (body2->target_id < 1) || (body2->target_id >= MAX_FD) )
			{
				U_Printf("[DB]Error mob_id(%d) bad char_id(%d)\n", channel_link, mob_id, body2->target_id);
				return	(-1);
			}	
			
			//check channel_link
			if( D_client[body2->target_id].channel_link != channel_link )
				continue;

			//check status
			if( D_client[body2->target_id].status >= _USER_DEAD )
				continue;

			//check map
			if( D_mob[mob_id][channel_link].map != D_client[body2->target_id].map )
				continue;

			//check area attack  
			gab_x		= D_client[body2->target_id].x - body1->target_x;
			gab_z		= D_client[body2->target_id].z - body1->target_z;
			
			if( D_type_mob_skill[body1->skill_type].range_x > D_type_mob_skill[body1->skill_type].range_z )
				att_area	= D_type_mob_skill[body1->skill_type].range_x + 10;	
			else
				att_area	= D_type_mob_skill[body1->skill_type].range_z + 10;	
				
			if( (gab_x < -att_area) || (gab_x > att_area) || (gab_z < -att_area) || (gab_z > att_area) )
				continue;

			//damage type
			if( ability.damage_type )
			{
				//compute damge
				D_compute_skill_damage(channel_link, MOB_ATTACK_CHAR, mob_id, body2->target_id, &ability, &damage, &damage_type);
					
				//check damage
				if( damage > 9999 )
					damage	= 9999;
			
				//set client hp, status
				if( (D_client[body2->target_id].hp -= damage) < 1 )
				{
					D_client[body2->target_id].hp		= 0;
					D_client[body2->target_id].status	= _USER_DEAD;
				
					result->dead_char_list[(result->dead_char_cnt)++]	= body2->target_id;
				}
				
				++(client_send_body1->target_char_cnt);
				
				//set client_send_body2
				client_send_body2->target_id	= body2->target_id;
				client_send_body2->target_hp	= D_client[body2->target_id].hp;
				client_send_body2->damage_type	= damage_type;
				client_send_body2->damage	= damage;	
		
				++client_send_body2;
			}
			
			//set char buff
			if( (result->buff_cnt) && (D_client[body2->target_id].status != _USER_DEAD) )
			{
				result->buff_char_list[(result->buff_char_cnt)++]	= body2->target_id;
				
				for( j=0; j<result->buff_cnt; j++ )
				{
					//stun
					if( (result->buff_serise[j] == _BUFF_DE_ACTIVE_STUN) )
					{
						if( D_check_stun(channel_link, MOB_ATTACK_CHAR, mob_id, body2->target_id, buff_value[j]) )
							(client_send_body2 -1)->damage_type	|= _DAMAGE_STUN;
						else
							continue;
					}
					
					D_SetClient_buff(body2->target_id, result->buff_serise[j], buff_value[j], buff_sec[j]);
				}
			}
		
			++body2;
		}
	
		return	(0);
	}

	//mob
	for( i=0; i<body1->target_cnt; i++ )
	{
		//check target_id
		if( (body2->target_id < 1) || (body2->target_id > D_max_mob) )
		{
			U_Printf("[DB]Error mob_id(%d) bad mob_id(%d)\n", channel_link, mob_id, body2->target_id);
			return	(-1);
		}	

		//check status
		if( D_mob[body2->target_id][channel_link].status >= _MOB_DEATH )
			continue;

		//check map
		if( D_mob[mob_id][channel_link].map != D_mob[body2->target_id][channel_link].map )
			continue;

		//check area attack  
		gab_x		= D_mob[body2->target_id][channel_link].x - body1->target_x;
		gab_z		= D_mob[body2->target_id][channel_link].z - body1->target_z;

		if( D_type_mob_skill[body1->skill_type].range_x > D_type_mob_skill[body1->skill_type].range_z )
			att_area	= D_type_mob_skill[body1->skill_type].range_x + 10;	
		else
			att_area	= D_type_mob_skill[body1->skill_type].range_z + 10;	
			
		if( (gab_x < -att_area) || (gab_x > att_area) || (gab_z < -att_area) || (gab_z > att_area) )
			continue;

		//set mob buff
		if( result->buff_cnt )
		{
			result->buff_mob_list[(result->buff_mob_cnt)++]	= body2->target_id;
			
			for( j=0; j<result->buff_cnt; j++ )
				D_SetMob_buff(channel_link, body2->target_id, result->buff_serise[j], buff_value[j], buff_sec[j]);
		}
			
		++body2;
	}

	return	(0);
}
