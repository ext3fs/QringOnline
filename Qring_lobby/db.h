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
G	int	D_GetWorld_channel_user(const int, const int);
G	void	D_SetWorld_channel_user(const int, const int, const int);
G	char	*D_GetWorld_machine_outer_ip(const int, const int);
G	char	*D_GetWorld_machine_dbagent_ip(const int, const int);
G	int	D_GetMax_level(void);
G	int	D_GetLevel_hp_max(const int);
G	int	D_GetLevel_ap_max(const int);

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
G	char	*D_GetClient_ip(const int);
G	void	D_SetClient_ip(const int, const char *);
G	char	*D_GetClient_lock_user_id(const int);
G	void	D_SetClient_lock_user_id(const int, const char *);
G	char	*D_GetClient_user_id(const int); 
G	void	D_SetClient_user_id(const int, const char *);
G	int	D_GetClient_load_world_idx(const int); 
G	void	D_SetClient_load_world_idx(const int, const int);
G	int	D_GetClient_load_char_cnt(const int);
G	void	D_SetClient_load_char_cnt(const int, const int);
G	void	D_SetClient_clear(const int);


#undef	G
#undef	_DB_H_

