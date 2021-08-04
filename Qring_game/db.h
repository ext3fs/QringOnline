/*____________________________________________________________

	Project name : Qring Online
	File name    : db.h
	Description  : data structure
	Date         : February 14 2005
_____________________________________________________________*/

#ifdef	G
#undef	G
#endif

#ifdef	_DB_H_
#define	G
#else
#define	G	extern
#endif


G	int	D_SetDb_init(void);

G	int	D_GetMax_world(void);
G	char	*D_GetWorld_name(const int);
G	int	D_GetWorld_channel_cnt(const int);
G	void	D_SetWorld_channel_user_plus(const int, const int, const int);
G	int	D_GetWorld_channel_user(const int, const int);
G	char	*D_GetWorld_machine_outer_ip(const int, const int);
G	char	*D_GetWorld_machine_dbagent_ip(const int, const int);
G	char	*D_GetWorld_machine_mobserv_ip(const int, const int);
G	int	D_GetMax_map(void);
G	int	D_GetMax_type_mob(void);
G	int	D_GetMax_mob(void);
G	int	D_GetMax_type_mob_skill(void);

G	char	D_GetMap_attr(const int, const int, const int);
G	int	D_GetMapBlock_item_link(const int, const int, const int);
G	void	D_SetMapBlock_insert_item(const int);
G	void	D_SetMapBlock_delete_item(const int);
G	int	D_GetMapBlock_mob_link(const int, const int, const int);
G	void	D_SetMapBlock_insert_mob(const int, const int);
G	void	D_SetMapBlock_delete_mob(const int, const int);
G	int	D_GetMapBlock_char_link(const int, const int, const int);
G	void	D_SetMapBlock_insert_char(const int);
G	void	D_SetMapBlock_delete_char(const int);

G	int	D_GetItem_block_next(const int);
G	time_t	D_GetItem_drop_time(const int);
G	int	D_GetItem_channel_link(const int);
G	int	D_GetItem_map(const int);
G	int	D_GetItem_block_x(const int);
G	void	D_SetItem_clear(const int);
G	void	D_GetItem_appear_data(struct body2_GC_ITEM_APPEAR *, const int);

G	int	D_GetMob_block_next(const int, const int);
G	int	D_GetMob_map(const int, const int); 
G	void	D_SetMob_map(const int, const int, const int);
G	int	D_GetMob_block_x(const int, const int); 
G	void	D_SetMob_block_x(const int, const int, const int);
G	int	D_GetMob_dir(const int, const int);
G	void	D_SetMob_dir(const int, const int, const int);
G	int	D_GetMob_status(const int, const int);
G	void	D_SetMob_status(const int, const int, const int);
G	void	D_GetMob_xyz(const int, const int, float *, float *, float *);
G	void	D_SetMob_xyz(const int, const int, const float, const float, const float);
G	int	D_GetMob_hp(const int, const int);
G	int	D_GetMob_buff_value(const int, const int, const int);
G	time_t	D_GetMob_buff_end_sec(const int, const int, const int);
G	void	D_SetMob_clear(const int, const int);
G	void	D_SetMob_buff(const int, const int, const int, const int, const int);
G	void	D_SetMob_buff_clear(const int, const int, const int);
G	void	D_SetMob_recover(const int, const int);
G	void	D_SetMob_perfect_recover(const int, const int);
G	int	D_SetMob_regen(const int, const int, const int, const int, const int, const int, const int, const int, const int);
G	void	D_GetMob_appear_data(struct body2_GC_MOB_APPEAR *, const int, const int);

G	int	D_GetClient_sec_active(const int);
G	void	D_SetClient_sec_active(const int, const int);
G	int	D_GetClient_session(const int);
G	void	D_SetClient_session(const int, const int);
G	int	D_GetClient_billing_idx(const int);
G	void	D_SetClient_billing_idx(const int, const int);
G	int	D_GetClient_use_billing_idx(const int);
G	int	D_GetClient_login_dbagent_idx(const int); 
G	void	D_SetClient_login_dbagent_idx(const int, const int);
G	int	D_GetClient_use_login_dbagent_idx(const int);
G	int	D_GetClient_dbagent_idx(const int); 
G	void	D_SetClient_dbagent_idx(const int, const int);
G	int	D_GetClient_use_dbagent_idx(const int); 
G	int	D_GetClient_mobserv_charge_channel_link(const int);
G	void	D_SetClient_mobserv_charge_channel_link(const int, const int);
G	int	D_GetClient_block_next(const int);
G	int	D_GetClient_map(const int);
G	void	D_SetClient_map(const int, const int);
G	int	D_GetClient_block_x(const int);
G	void	D_SetClient_block_x(const int, const int);
G	int	D_GetClient_dir(const int);
G	void	D_SetClient_dir(const int, const int);
G	int	D_GetClient_status(const int);
G	void	D_SetClient_status(const int, const int);
G	void	D_GetClient_xyz(const int, float *, float *, float *);
G	void	D_SetClient_xyz(const int, const float, const float, const float);
G	int	D_GetClient_hp(const int);
G	void	D_SetClient_hp(const int, const int);
G	int	D_GetClient_ap(const int);
G	time_t	D_GetClient_char_load_sec(const int);
G	int	D_GetClient_level(const int);
G	int	D_GetClient_exp(const int);
G	void	D_SetClient_last_attack_type(const int, const int);
G	int	D_GetClient_money(const int);
G	void	*D_GetClient_item_slot(const int, const int);
G	int	D_GetClient_buff_value(const int, const int);
G	time_t	D_GetClient_buff_end_sec(const int, const int);
G	int	D_GetClient_skill_point(const int);
G	int	D_GetClient_skill_slot(const int, const int);
G	char	*D_GetClient_ip(const int);
G	void	D_SetClient_ip(const int, const char *);
G	char	*D_GetClient_lock_user_id(const int);
G	void	D_SetClient_lock_user_id(const int, const char *);
G	char	*D_GetClient_user_id(const int); 
G	void	D_SetClient_user_id(const int, const char *);
G	char	*D_GetClient_char_name(const int);
G	void	D_SetClient_char_name(const int, const char *);
G	void	D_SetClient_channel_link(const int, const int);
G	int	D_GetClient_channel_link(const int);
G	void	D_SetClient_recover(const int, int *);
G	void	D_SetClient_buff(const int, const int, const int, const int);
G	void	D_SetClient_buff_clear(const int, const int);
G	void	D_SetClient_clear(const int);
G	int	D_SetClient_from_db(const int, struct body1_AW_CHAR_LOAD_RESULT *, struct body2_AW_CHAR_LOAD_RESULT *, int *);
G	void	D_SetClient_regen(const int);
G	void	D_SetClient_warp(const int, const int, const int, const int);
G	void	D_GetClient_save_data(struct body1_WA_CHAR_SAVE *, struct body2_WA_CHAR_SAVE *, const int);
G	void	D_GetClient_owner_data(struct body1_GC_CHAR_LOAD_RESULT *, struct body2_GC_CHAR_LOAD_RESULT *, const int);
G	void	D_GetClient_appear_data(struct body2_GC_OTHER_CHAR_APPEAR *, const int);
G	void	D_GetClient_channelin_data(struct body2_GM_CHAR_CHANNEL_IN *, const int);
G	void	D_GetClient_mapchange_data(struct body_GM_CHAR_MAP_CHANGE *, const int);
G	void	D_GetClient_regen_data(struct body_GC_CHAR_REGEN_RESULT *, const int);
G	void	D_GetClient_warp_data(struct body_GC_CHAR_WARP_RESULT *, const int);
G	void	D_GetClient_channel_move_data(struct body_GC_CHAR_CHANNEL_MOVE_RESULT *, const int);
G	int	D_SetClient_item_move(const int, const int, const int, int *);
G	int	D_SetClient_item_pickup(const int, const int, const int, int *, int *, int *);
G	int	D_SetClient_item_drop(const int, const int, int *);
G	int	D_SetClient_item_enchant(const int, const int, const int);
G	int	D_SetClient_item_use(const int, const int, int *, int *);
G	int	D_SetClient_skill_ready(const int, const int);
G	void	D_SetClient_skill_cancel(const int);
G	int	D_SetClient_skill_learn(const int, const int, int *);
G	int	D_SetClient_transform(const int, const int);

G	int	D_Char_attack_mob(const int, const int, const int, int *, int *, int *);
G	int	D_Mob_attack_char(const int, const int, const int, int *, int *, int *);
G	void	D_Char_kill_mob(const int, const int, const int, int *, int *, int *);
G	int	D_Char_check_warp(const int,  const int, const int, int *, int *, int *, int *);
G	int	D_Char_buy_store_item(const int, const int, const int, const int, const int, int *);
G	int	D_Char_sell_store_item(const int, const int, const int, const int);
G	int	D_Char_skill_action(const int, struct body1_CG_CHAR_SKILL_ACTION *, char *, char *, struct skill_action_result *);
G	int	D_Mob_skill_action(const int, const int, struct body1_MG_MOB_SKILL_ACTION *, char *, struct skill_action_result *);

#undef	G
#undef	_DB_H_

