
#ifndef	__db_h__
#define	__db_h__


#ifdef __cplusplus
extern "C" {
#endif


//______________________________________________________________________________________
// script
int	D_set_script				( void );

char *	D_get_gameserv_ip			( void );

int	D_get_map_cnt				( void );
int	D_get_type_mob_cnt			( void );
int	D_get_mob_cnt				( const int );
int	D_map_start_mob				( const int, const int );
int	D_map_end_mob				( const int, const int );
int	D_map_summon_start_mob			( const int, const int );
int	D_check_invalid_link			( const int, const int );

void	D_set_channel_activity			( const int );
void	D_channel_mob_activity			( const int );
int	D_summon_mob_activity			( const int, const int, const int, const int, const float, const float );
void	D_channel_mob_recover			( const int );
void	D_channel_char_activity			( const int );

//______________________________________________________________________________________
// map
int	D_GetMap_summon_link			( const int, const int );
int	D_GetMap_summon_link_next		( const int, const int );

//______________________________________________________________________________________
// mob 
void	D_SetMob_regen				( const int, const int, struct body_MG_MOB_REGEN * );
void	D_SetMob_awake				( const int, const int, struct body_MG_MOB_MOVE * );
int	D_SetMob_death				( const int, const int );
void	D_SetMob_extinct			( const int, const int, struct body_MG_MOB_EXTINCT * );
int	D_SetMob_warp				( const int, const int, int * );
void	D_SetMob_perfect_recover		( const int, const int, struct body_MG_MOB_PERFECT_RECOVER * );
void	D_SetMob_regular_recover		( const int, const int );
int	D_SetMob_escape				( const int, const int );

int	D_GetMob_npc_type			( const int, const int );

int	D_ChkMob_map_collission			( const int, const int, const int, const int );

void	D_SetMob_status				( const int, const int, const int );
int	D_GetMob_status				( const int, const int );
void	D_SetMob_action_millisec		( const int, const int, const int );
double	D_GetMob_action_millisec		( const int, const int );
void	D_SetMob_action_sec			( const int, const int, const int );
time_t	D_GetMob_action_sec			( const int, const int, const int );
void	D_SetMob_skill_sec			( const int, const int, const int );
time_t	D_GetMob_skill_sec			( const int, const int, const int );
void	D_SetMob_skill_num			( const int, const int, const int, const int );
int	D_GetMob_skill_num			( const int, const int, const int );

int	D_GetMob_delay_regen			( const int, const int );
int	D_GetMob_delay_attack			( const int, const int );
int	D_GetMob_delay_death			( const int, const int );
int	D_GetMob_skill_casting_millisec		( const int, const int );
int	D_GetMob_skill_action_millisec		( const int, const int );
int	D_GetMob_skill_action_cnt		( const int, const int );

int	D_ChkMob_over_area_persuit		( const int, const int, const float, const float );
int	D_ChkMob_over_area_attack		( const int, const int, const int );

int 	D_GetMob_move_wander			( const int, const int, struct body_MG_MOB_MOVE *, int * );
int 	D_GetMob_move_persuit			( const int, const int, const int, struct body_MG_MOB_MOVE *, int * );
int 	D_GetMob_move_regen_area		( const int, const int, const int, struct body_MG_MOB_MOVE *, int * );
int 	D_GetMob_move_random_xz			( const int, const int, const int, struct body_MG_MOB_MOVE *, int * );

int	D_SetMob_search_target_char		( const int, const int );
int	D_GetMob_target_char			( const int, const int );
int	D_GetMob_target_list			( const int, const int, char *, int * );

int	D_ChkMob_straight_target		( const int, const int, const int );
int	D_ChkMob_action_to_char			( const int, const int, const int );
int	D_SetMob_attack_to_char			( const int, const int, struct body_MG_MOB_ATTACK_CHAR * );

void	D_SetMob_init_current_skill		( const int, const int );
int	D_SetMob_casting_skill			( const int, const int, char *, int * );
int	D_SetMob_new_skill			( const int, const int, const int, char *, int * );
int	D_ChkMob_cancel_skill			( const int, const int, const int );
void	D_SetMob_cancel_skill			( const int, const int, char * );
void	D_SetMob_buff				( const int, const int, const int, const int );


//______________________________________________________________________________________
// char
int	D_set_char				( const int );

void	D_SetChar_channel_in			( const int, const int, const struct body2_GM_CHAR_CHANNEL_IN );
void	D_SetChar_channel_out			( const int, const int );
void	D_SetChar_map_change			( const int, const int, const struct body_GM_CHAR_MAP_CHANGE );
void	D_SetChar_move				( const int, const int, const int, const int, const int, const int );
int	D_SetChar_attack_to_mob			( const int, const int, const int, const int, const int, const int
						, const int, char * );


#ifdef __cplusplus
}
#endif	



#endif
