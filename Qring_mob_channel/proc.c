
#include	"global.h"
#include	"util.h"
#include	"db.h"
#include	"sp.h"
#include	"recv.h"
#include	"send.h"
#include	"proc.h"





// ----------------------------------------------------------------------------
// proc variable
static struct connection	*P_connect;
static struct recv_q		**P_proc_q;





// ----------------------------------------------------------------------------
// proc function
static inline void	P_proc_mob			( const int, const int, const int );
static inline void	P_send_queue_in			( const int, const int, const unsigned short, const char * );

static inline void	P_INTER_CONN_FD			( const int, const int );
static inline void	P_INTER_CLOSE_FD		( const int, const int );
static inline void	P_INTER_MOB_RECOVER		( const int, const int );

static inline void	P_GM_CHAR_CHANNEL_IN		( const int, const int );
static inline void	P_GM_CHAR_CHANNEL_OUT		( const int, const int );
static inline void	P_GM_CHAR_MAP_CHANGE		( const int, const int );
static inline void	P_GM_CHAR_MOVE			( const int, const int );
static inline void	P_GM_CHAR_ATTACK_MOB		( const int, const int );
static inline void	P_GM_CHAR_SKILL_ACTION		( const int, const int );
static inline void	P_GM_MOB_SUMMON			( const int, const int );
static inline void	P_GM_MOB_BUFF			( const int, const int );

static inline void	P_MOB_STATUS_DEATH		( const int, const int );
static inline void	P_MOB_STATUS_EXTINCT		( const int, const int );
static inline void	P_MOB_STATUS_REGEN		( const int, const int );
static inline void	P_MOB_STATUS_AWAKE		( const int, const int );
static inline void	P_MOB_STATUS_WANDER		( const int, const int );
static inline void	P_MOB_STATUS_MOVE_RANDOM_XYZ	( const int, const int, const int );
static inline void	P_MOB_STATUS_MOVE_REGEN_AREA	( const int, const int, const int );
static inline void	P_MOB_STATUS_ATTACK		( const int, const int, const int );
static inline void	P_MOB_ACTION_WANDER		( const int, const int );
static inline void	P_MOB_ACTION_PERSUIT		( const int, const int, const int );
static inline void	P_MOB_ACTION_ATTACK		( const int, const int );
static inline void	P_MOB_STATUS_NPC_REGEN		( const int, const int );
static inline void	P_MOB_STATUS_NPC_CONTINUE	( const int, const int );
static inline void	P_MOB_STATUS_NPC_OPEN		( const int, const int );
static inline void	P_MOB_STATUS_NPC_CLOSE		( const int, const int );

static inline int	P_mob_search_target		( const int, const int, const int );
static inline int	P_mob_new_skill			( const int, const int, const int, const int );
static inline int	P_mob_casting_skill		( const int, const int );
static inline void	P_mob_cancel_skill		( const int, const int, const int );

static inline void	P_char_attack_to_mob		( const int, const int, const int, const int, const int
							, const int, const int );



void	*P_pro_main( void *agr )
{

	int			isExit=0;
	struct thread_args*	args = (struct thread_args *)agr;
	int			tidx, start_channel, end_channel, channel, map, mob_id;
	int			recvq_tail_idx;


	tidx			= args->tidx;
	start_channel		= args->charge_start_channel;
	end_channel		= args->charge_end_channel;

	dprintf(0, "[PROC]Thread(%d), charge_channel(%d, %d) Start !! \n", tidx, start_channel, end_channel );


	while( !isExit )
	{

		usleep(0);


		recvq_tail_idx	= U_recv_queue_tail(tidx);


		// Msg Proc
		while( 1 )
		{
			if( recvq_tail_idx == U_recv_queue_head(tidx) )
				break;

			if( (P_proc_q[tidx] = U_recv_queue_out(tidx)) == NULL )
				break;

			if( (P_proc_q[tidx]->channel < start_channel) || (P_proc_q[tidx]->channel > end_channel) )
			{
				dprintf(0, "[PROC]Error P_pro_main() tidx(%d), charge(%d, %d), proc_q->channel(%d)\n"
					, tidx, start_channel, end_channel, P_proc_q[tidx]->channel );
				isExit	= 1;
				break;
			}

			switch( P_proc_q[tidx]->command )
			{
				case	INTER_EXIT_THREAD :
					isExit	= 1;
					continue;
				case	INTER_CONN_FD :
					P_INTER_CONN_FD(tidx, P_proc_q[tidx]->channel);
					continue;
				case	INTER_CLOSE_FD :
					P_INTER_CLOSE_FD(tidx, P_proc_q[tidx]->channel);
					continue;
				case	INTER_CHANNEL_LIVE :
					//dprintf(P_proc_q[tidx]->channel, "[PROC]P_pro_main() tidx(%d), CHANNEL_LIVE(%d) \n"
					//	, tidx, P_proc_q[tidx]->channel );
					continue;
			}

			if( !P_connect[channel].fd )
				continue;

			switch( P_proc_q[tidx]->command )
			{
				case	INTER_MOB_RECOVER :
					P_INTER_MOB_RECOVER(tidx, P_proc_q[tidx]->channel);
					break;
				case	GM_CHAR_CHANNEL_IN :
					P_GM_CHAR_CHANNEL_IN(tidx, P_proc_q[tidx]->channel);
					break;
				case	GM_CHAR_CHANNEL_OUT :
					P_GM_CHAR_CHANNEL_OUT(tidx, P_proc_q[tidx]->channel);
					break;
				case	GM_CHAR_MAP_CHANGE :
					P_GM_CHAR_MAP_CHANGE(tidx, P_proc_q[tidx]->channel);
					break;
				case	GM_CHAR_MOVE :
					P_GM_CHAR_MOVE(tidx, P_proc_q[tidx]->channel);
					break;
				case	GM_CHAR_ATTACK_MOB :
					P_GM_CHAR_ATTACK_MOB(tidx, P_proc_q[tidx]->channel);
					break;
				case	GM_CHAR_SKILL_ACTION :
					P_GM_CHAR_SKILL_ACTION(tidx, P_proc_q[tidx]->channel);
					break;
				case	GM_MOB_SUMMON :
					P_GM_MOB_SUMMON(tidx, P_proc_q[tidx]->channel);
					break;
				case	GM_MOB_BUFF :
					P_GM_MOB_BUFF(tidx, P_proc_q[tidx]->channel);
					break;

				default :
					dprintf(P_proc_q[tidx]->channel, "[PROC](%d)Unknown Type(%x) \n"
						, P_proc_q[tidx]->channel, P_proc_q[tidx]->command );
					break;
			}

		}// end Msg Proc !!


		// proc monster ai
		for( channel=start_channel; channel<=end_channel; channel ++ )
		{
			// check alive
			if( !P_connect[channel].fd )
				continue;

			for( map=1; map<=D_get_map_cnt(); map ++ )
			{
				if( D_check_invalid_link(channel, map) )
					continue;

				// normal mob
				for( mob_id=D_map_start_mob(channel, map); mob_id<D_map_summon_start_mob(channel, map); mob_id++ )
					P_proc_mob( tidx, channel, mob_id );

				// summon mob
				mob_id = D_GetMap_summon_link( channel, map );

				while( mob_id )
				{
					P_proc_mob( tidx, channel, mob_id );

					mob_id = D_GetMap_summon_link_next( channel, mob_id );
				}
			}
		}

	}// end while() !!


	dprintf(0, "[PROC]Thread(%d) End !! \n", tidx);

	return NULL;
}


static inline void	P_proc_mob( const int tidx, const int channel, const int mob_id )
{

	// check action_millisec
	if( D_GetMob_action_millisec(channel, mob_id) > G_MilliSec )
		return;

	// check casting_skill
	if( P_mob_casting_skill(channel, mob_id) )
		return;

	// proc mob
	switch( D_GetMob_status(channel, mob_id) )
	{
		case	MOB_STATUS_WANDER :
			P_MOB_STATUS_WANDER(channel, mob_id);
			return;
		case	MOB_STATUS_PERSUIT :
		case	MOB_STATUS_ATTACK :
			P_MOB_STATUS_ATTACK(tidx, channel, mob_id);
			return;
		case	MOB_STATUS_ESCAPE :
		case	MOB_STATUS_GATHER :
			P_MOB_STATUS_MOVE_RANDOM_XYZ(tidx, channel, mob_id);
			return;
		case	MOB_STATUS_PERFECT :
		case	MOB_STATUS_MOVE_REGEN :
			P_MOB_STATUS_MOVE_REGEN_AREA(tidx, channel, mob_id);
			return;
		case	MOB_STATUS_DEATH :
			P_MOB_STATUS_DEATH(channel, mob_id);
			return;
		case	MOB_STATUS_EXTINCT :
			P_MOB_STATUS_EXTINCT(channel, mob_id);
			return;
		case	MOB_STATUS_REGEN :
			P_MOB_STATUS_REGEN(channel, mob_id);
			return;
		case	MOB_STATUS_AWAKE :
			P_MOB_STATUS_AWAKE(channel, mob_id);
			return;
		case	MOB_STATUS_NPC_REGEN :
			P_MOB_STATUS_NPC_REGEN(channel, mob_id);
			return;
		case	MOB_STATUS_NPC_CONTINUE :
			P_MOB_STATUS_NPC_CONTINUE(channel, mob_id);
			return;
		case	MOB_STATUS_NPC_OPEN :
			P_MOB_STATUS_NPC_OPEN(channel, mob_id);
			return;
		case	MOB_STATUS_NPC_CLOSE :
			P_MOB_STATUS_NPC_CLOSE(channel, mob_id);
			return;
		default :
			dprintf(channel, "[PROC]Error P_proc_mob() ai_status(%d) channel(%d), mob_id(%d) \n"
				, channel, D_GetMob_status(channel, mob_id), mob_id );
			return;
	}
}





// ----------------------------------------------------------------------------------------
//
int	P_channel_init( void )
{
	int		channel;


	if( (P_proc_q = calloc((G_proc_thr_cnt + 1), sizeof(struct recv_q))) == NULL )
	{
		dprintf(channel, "[PROC]Error P_channel_init() calloc() : %s \n", strerror(errno) );
		return (-1);
	}

	if( (P_connect = calloc((TOTAL_CHANNEL_CNT + 1), sizeof(struct connection))) == NULL )
	{
		dprintf(channel, "[PROC]Error P_channel_init() calloc() : %s \n", strerror(errno) );
		return (-1);
	}

	for( channel=1; channel<=TOTAL_CHANNEL_CNT; channel++ )
	{
		P_connect[channel].fd		= 0;
		P_connect[channel].channel	= 0;
	}

	return (0);
}


static inline void	P_send_queue_in( const int channel, const int command, const unsigned short body_size, const char *body )
{
	struct 	header_normal	header;
	char			buf[SIZEOF_HEADER_NORMAL + SIZEOF_MAX_BODY + 1];

	//dprintf(channel, "[PROC]P_send_queue_in() channel(%d), command(%x), body_size(%d) \n", channel, command, body_size );

	header.command		= command;
	header.body_size	= body_size;
	header.seq		= 0;

	memcpy( buf				, &header	, SIZEOF_HEADER_NORMAL );
	memcpy( buf + SIZEOF_HEADER_NORMAL	, body		, body_size );

	S_channel_sendbuf_in( channel, buf, (SIZEOF_HEADER_NORMAL + body_size) );
}





// ----------------------------------------------------------------------------------------
//
static inline void	P_INTER_CONN_FD( const int tidx, const int channel )
{

	dprintf(channel, "[PROC]P_INTER_CONN_FD() channel(%d) \n", channel );

	if( P_connect[channel].fd )
	{
		dprintf(channel, "[PROC]Error P_INTER_CONN_FD() running channel(%d) \n", channel );
		return;
	}

	memcpy( &P_connect[channel], P_proc_q[tidx]->buf, P_proc_q[tidx]->buf_size );

	D_set_channel_activity( channel );
}


static inline void	P_INTER_CLOSE_FD( const int tidx, const int channel )
{

	dprintf(channel, "[PROC]P_INTER_CLOSE_FD() channel(%d) \n", channel );

	if( !P_connect[channel].fd )
	{
		dprintf(channel, "[PROC]Error P_INTER_CLOSE_FD() closed channel(%d) \n", channel );
		return;
	}

	memcpy( &P_connect[channel], P_proc_q[tidx]->buf, P_proc_q[tidx]->buf_size );
}


static inline void	P_INTER_MOB_RECOVER( const int tidx, const int channel )
{

	if( !P_connect[channel].fd )
		return;

	D_channel_mob_recover( channel );
}





// ----------------------------------------------------------------------------------------
//
static inline void	P_GM_CHAR_CHANNEL_IN( const int tidx, const int channel )
{
	struct  body1_GM_CHAR_CHANNEL_IN	*req_body1;
	struct  body2_GM_CHAR_CHANNEL_IN	*req_body2=NULL;
	int				i;


	req_body1	= (struct body1_GM_CHAR_CHANNEL_IN *)(P_proc_q[tidx]->buf);

	if( req_body1->char_cnt )
		req_body2	= (struct body2_GM_CHAR_CHANNEL_IN *)((P_proc_q[tidx]->buf) + sizeof(struct body1_GM_CHAR_CHANNEL_IN));

	for( i=0; i<req_body1->char_cnt; i++ )
	{
		dprintf(channel, "[PROC]P_GM_CHAR_CHANNEL_IN() total_cnt(%d), char_cnt(%d), channel(%d), map(%d), char_id(%d) \n"
			, req_body1->char_cnt, (i + 1), channel, req_body2[i].map, req_body2[i].char_id );
		D_SetChar_channel_in( channel, req_body2[i].char_id, req_body2[i] );
	}
}


static inline void	P_GM_CHAR_CHANNEL_OUT( const int tidx, const int channel )
{
	struct  body1_GM_CHAR_CHANNEL_OUT	*req_body1;
	struct  body2_GM_CHAR_CHANNEL_OUT	*req_body2=NULL;
	int				i;


	req_body1	= (struct body1_GM_CHAR_CHANNEL_OUT *)(P_proc_q[tidx]->buf);

	if( req_body1->char_cnt )
		req_body2	= (struct body2_GM_CHAR_CHANNEL_OUT *)((P_proc_q[tidx]->buf) + sizeof(struct body1_GM_CHAR_CHANNEL_OUT));

	for( i=0; i<req_body1->char_cnt; i++ )
	{
		dprintf(channel, "[PROC]P_GM_CHAR_CHANNEL_OUT() channel(%d), char_id(%d) \n"
			, channel, req_body2[i].char_id );
		D_SetChar_channel_out( channel, req_body2[i].char_id );
	}
}


static inline void	P_GM_CHAR_MAP_CHANGE( const int tidx, const int channel )
{
	struct  body_GM_CHAR_MAP_CHANGE	*req_body;


	req_body	= (struct body_GM_CHAR_MAP_CHANGE *)(P_proc_q[tidx]->buf);

	dprintf(channel, "[PROC]P_GM_CHAR_MAP_CHANGE() channel(%d), char_id(%d), map(%d) \n"
		, channel, req_body->char_id, req_body->map );

	D_SetChar_map_change( channel, req_body->char_id, *req_body );
}


static inline void	P_GM_CHAR_MOVE( const int tidx, const int channel )
{
	struct  body_GM_CHAR_MOVE	*req_body;


	req_body	= (struct body_GM_CHAR_MOVE *)(P_proc_q[tidx]->buf);

	D_SetChar_move( channel, req_body->char_id, req_body->status, req_body->x, req_body->y, req_body->z );
}


static inline void	P_GM_CHAR_ATTACK_MOB( const int tidx, const int channel )
{
	struct  body_GM_CHAR_ATTACK_MOB		*req_body;


	req_body	= (struct body_GM_CHAR_ATTACK_MOB *)(P_proc_q[tidx]->buf);

	dprintf(channel, "[PROC]P_GM_CHAR_ATTACK_MOB() char_id(%d), target_mob(%d) \n", req_body->char_id, req_body->mob_id );

	D_SetChar_move( channel, req_body->char_id, _USER_STAND_ATTACK1, req_body->x, req_body->y, req_body->z );

	P_char_attack_to_mob( channel, req_body->mob_id, req_body->char_id, req_body->damage, req_body->damage_type
		, req_body->dir, req_body->mob_hp );
}


static inline void	P_GM_CHAR_SKILL_ACTION( const int tidx, const int channel )
{
	struct  body1_GM_CHAR_SKILL_ACTION	*req_body1;
	struct  body2_GM_CHAR_SKILL_ACTION	*req_body2;
	int					i;


	req_body1	= (struct body1_GM_CHAR_SKILL_ACTION *)(P_proc_q[tidx]->buf);

	dprintf(channel, "[PROC]P_GM_CHAR_SKILL_ACTION() char_id(%d), target_mob_cnt(%d) \n"
		, req_body1->char_id, req_body1->target_mob_cnt );

	D_SetChar_move( channel, req_body1->char_id, _USER_STAND_ATTACK1, req_body1->x, req_body1->y, req_body1->z );


	for( i=0; i<req_body1->target_mob_cnt; i ++ )
	{
		req_body2	= (struct body2_GM_CHAR_SKILL_ACTION *)(P_proc_q[tidx]->buf 	
				+ sizeof(struct body1_GM_CHAR_SKILL_ACTION) + i * sizeof(struct body2_GM_CHAR_SKILL_ACTION) );

		P_char_attack_to_mob( channel, req_body2->target_id, req_body1->char_id, req_body2->damage, req_body2->damage_type
			, req_body1->dir, req_body2->target_hp );
	}
}


static inline void	P_GM_MOB_SUMMON( const int tidx, const int channel )
{
	struct body_GM_MOB_SUMMON	*req_body;


	req_body	= (struct body_GM_MOB_SUMMON *)(P_proc_q[tidx]->buf);

	dprintf(channel, "[PROC]P_GM_MOB_SUMMON() mob_type(%d)\n", req_body->mob_type );

	D_summon_mob_activity( channel, req_body->map, -1, req_body->mob_type, req_body->x, req_body->z );
}


static inline void	P_GM_MOB_BUFF( const int tidx, const int channel )
{
	struct body_GM_MOB_BUFF		*req_body;


	req_body	= (struct body_GM_MOB_BUFF *)(P_proc_q[tidx]->buf);

	dprintf(channel, "[PROC]P_GM_MOB_BUFF() mob_id(%d), series(%d), value(%d), sec(%d) \n"
		, req_body->mob_id, req_body->buff_series, req_body->buff_value, req_body->buff_sec );

	D_SetMob_buff( channel, req_body->mob_id, req_body->buff_series, req_body->buff_value );
}	





// ----------------------------------------------------------------------------------------
//
static inline void	P_MOB_STATUS_DEATH( const int channel, const int mob_id )
{
	// set_mob_status extinct
	D_SetMob_status( channel, mob_id, MOB_STATUS_EXTINCT );
	D_SetMob_action_millisec( channel, mob_id, MILLISEC_TO_EXTINCT );
}


static inline void	P_MOB_STATUS_EXTINCT( const int channel, const int mob_id )
{
	char		buf[SIZEOF_MAX_BODY + 1];


	D_SetMob_extinct( channel, mob_id, (struct body_MG_MOB_EXTINCT *)buf );
	P_send_queue_in( channel, MG_MOB_EXTINCT, sizeof(struct body_MG_MOB_EXTINCT), buf );

	// set_mob_status regen
	D_SetMob_status( channel, mob_id, MOB_STATUS_REGEN );
	D_SetMob_action_millisec( channel, mob_id, D_GetMob_delay_regen(channel, mob_id) );
}


static inline void	P_MOB_STATUS_REGEN( const int channel, const int mob_id )
{
	char		buf[SIZEOF_MAX_BODY + 1];


	D_SetMob_regen( channel, mob_id, (struct body_MG_MOB_REGEN *)buf );
	P_send_queue_in( channel, MG_MOB_REGEN, sizeof(struct body_MG_MOB_REGEN), buf );

	// set_mob_status awake
	D_SetMob_status( channel, mob_id, MOB_STATUS_AWAKE );
	D_SetMob_action_millisec( channel, mob_id, MILLISEC_TO_AWAKE );
}


static inline void	P_MOB_STATUS_AWAKE( const int channel, const int mob_id )
{
	char		buf[SIZEOF_MAX_BODY + 1];


	D_SetMob_awake( channel, mob_id, (struct body_MG_MOB_MOVE *)buf );
	P_send_queue_in( channel, MG_MOB_MOVE, sizeof(struct body_MG_MOB_MOVE), buf );

	if( P_mob_search_target(channel, mob_id, -1) )
		return;

	// set_mob_status wander
	D_SetMob_status( channel, mob_id, MOB_STATUS_WANDER );
	D_SetMob_action_millisec( channel, mob_id, MILLISEC_TO_NEXT_MOVE );
}


static inline void	P_MOB_STATUS_WANDER( const int channel, const int mob_id )
{
	char		buf[SIZEOF_MAX_BODY + 1];
	int		isProcess, add_move_millisec;


	// set target
	if( P_mob_search_target(channel, mob_id, -1) )
		return;

	// do wander_skill
	if( P_mob_new_skill(channel, mob_id, SKILL_AI_COND_WANDER, 30) )
		return;
	
	// do area_hill skill
	if( P_mob_new_skill(channel, mob_id, SKILL_AI_COND_MOB_HP_DOWN, 10) )
		return;

	// send move_wander
	isProcess	=  D_GetMob_move_wander( channel, mob_id, (struct body_MG_MOB_MOVE *)buf, &add_move_millisec ); 
	
	switch( isProcess )
	{
		case	RET_MOVE_XZ :
			P_send_queue_in( channel, MG_MOB_MOVE, sizeof(struct body_MG_MOB_MOVE), buf );
			break;

		case	RET_MOVE_STAY :
		case	RET_MOVE_AREA_OVER :
		case	RET_MOVE_NOT_FOUND :
			break;
	}

	D_SetMob_action_millisec( channel, mob_id, (MILLISEC_TO_NEXT_MOVE + add_move_millisec) );
}


static inline void	P_MOB_STATUS_MOVE_RANDOM_XYZ( const int tidx, const int channel, const int mob_id )
{
	char		buf[SIZEOF_MAX_BODY + 1];
	int		isProcess, add_move_millisec;


	isProcess 	= D_GetMob_move_random_xz( tidx, channel, mob_id, (struct body_MG_MOB_MOVE *)buf, &add_move_millisec );

	switch( isProcess )
	{
		case	RET_MOVE_XZ :
			P_send_queue_in( channel, MG_MOB_MOVE, sizeof(struct body_MG_MOB_MOVE), buf );
			break;

		case	RET_MOVE_STAY :
		case	RET_MOVE_AREA_OVER :
		case	RET_MOVE_NOT_FOUND :
			if( D_GetMob_status(channel, mob_id) == MOB_STATUS_GATHER )
			{
				D_SetMob_status( channel, mob_id, MOB_STATUS_WANDER );
				return;
			}

			D_SetMob_status( channel, mob_id, MOB_STATUS_MOVE_REGEN );
			if( P_mob_new_skill(channel, mob_id, SKILL_AI_COND_ESCAPE_END, -1) )
				return;
			break;
	}

	D_SetMob_action_millisec( channel, mob_id, (MILLISEC_TO_NEXT_MOVE + add_move_millisec) );
}


static inline void	P_MOB_STATUS_MOVE_REGEN_AREA( const int tidx, const int channel, const int mob_id )
{
	char		buf[SIZEOF_MAX_BODY + 1];
	int		isProcess, add_move_millisec;


	// set target
	if( P_mob_search_target(channel, mob_id, -1) )
		return;

	isProcess 	= D_GetMob_move_regen_area( tidx, channel, mob_id, (struct body_MG_MOB_MOVE *)buf, &add_move_millisec );

	switch( isProcess )
	{
		case	RET_MOVE_XZ :
			P_send_queue_in( channel, MG_MOB_MOVE, sizeof(struct body_MG_MOB_MOVE), buf );
			break;
		case	RET_MOVE_STAY :
		case	RET_MOVE_AREA_OVER :
		case	RET_MOVE_NOT_FOUND :
			D_SetMob_perfect_recover( channel, mob_id, (struct body_MG_MOB_PERFECT_RECOVER *)buf );
			P_send_queue_in( channel, MG_MOB_PERFECT_RECOVER, sizeof(struct body_MG_MOB_PERFECT_RECOVER), buf );
			D_SetMob_status( channel, mob_id, MOB_STATUS_WANDER );
			break;
	}

	D_SetMob_action_millisec( channel, mob_id, (MILLISEC_TO_NEXT_MOVE + add_move_millisec) );
}


static inline void	P_MOB_STATUS_ATTACK( const int tidx, const int channel, const int mob_id )
{
	int	target_char_id;


	if( !(target_char_id = D_GetMob_target_char(channel, mob_id)) )
	{
		P_MOB_ACTION_WANDER( channel, mob_id );
		return;
	}

	// check target
	if( !D_ChkMob_action_to_char(channel, mob_id, target_char_id) )
	{
		P_MOB_ACTION_WANDER( channel, mob_id );
		return;
	}

	// check straight
	if( !D_ChkMob_straight_target(channel, mob_id, target_char_id) )
	{
		P_MOB_ACTION_PERSUIT( tidx, channel, mob_id );
		return;
	}

	// do bomb_3_skill
	if( P_mob_new_skill(channel, mob_id, SKILL_AI_COND_BOMB_3, 2) )
		return;

	// do my_hill skill
	if( P_mob_new_skill(channel, mob_id, SKILL_AI_COND_MY_HP_DOWN, 10) )
		return;

	// do area_hill skill
	if( P_mob_new_skill(channel, mob_id, SKILL_AI_COND_MOB_HP_DOWN, 10) )
		return;

	// do limit_8_skill
	if( P_mob_new_skill(channel, mob_id, SKILL_AI_COND_LIMIT_8_60, (8 * 60)) )
		return;

	// do limit_5_skill
	if( P_mob_new_skill(channel, mob_id, SKILL_AI_COND_LIMIT_5_60, (5 * 60)) )
		return;

	// do limit_3_skill
	if( P_mob_new_skill(channel, mob_id, SKILL_AI_COND_LIMIT_3_60, (3 * 60)) )
		return;

	// do every_30_skill
	if( P_mob_new_skill(channel, mob_id, SKILL_AI_COND_EVERY_30, 30) )
		return;

	// do combat_skill
	if( P_mob_new_skill(channel, mob_id, SKILL_AI_COND_COMBAT, 0) )
		return;

	// do normal_attack
	if( !D_ChkMob_over_area_attack(channel, mob_id, target_char_id) )
	{
		P_MOB_ACTION_ATTACK( channel, mob_id );
		return; 
	}


	// do persuit
	P_MOB_ACTION_PERSUIT( tidx, channel, mob_id );
}


static inline void	P_MOB_ACTION_WANDER( const int channel, const int mob_id )
{
	// set_mob_status perfect
	D_SetMob_status( channel, mob_id, MOB_STATUS_PERFECT );
	D_SetMob_action_millisec( channel, mob_id, MILLISEC_TO_NEXT_MOVE );
}


static inline void	P_MOB_ACTION_PERSUIT( const int tidx, const int channel, const int mob_id )
{
	char		buf[SIZEOF_MAX_BODY + 1];
	int		isProcess, add_move_millisec;


	isProcess 	= D_GetMob_move_persuit( tidx, channel, mob_id, (struct body_MG_MOB_MOVE *)buf, &add_move_millisec );

	switch( isProcess )
	{
		case	RET_MOVE_XZ :
			P_send_queue_in( channel, MG_MOB_MOVE, sizeof(struct body_MG_MOB_MOVE), buf );
			D_SetMob_status( channel, mob_id, MOB_STATUS_PERSUIT );
			break;
		case	RET_MOVE_STAY :
			D_SetMob_status( channel, mob_id, MOB_STATUS_PERSUIT );
			break;
		case	RET_MOVE_AREA_OVER :
		case	RET_MOVE_NOT_FOUND :
			D_SetMob_status( channel, mob_id, MOB_STATUS_PERFECT );
			break;
	}

	D_SetMob_action_millisec( channel, mob_id, (MILLISEC_TO_NEXT_MOVE + add_move_millisec) );
}


static inline void	P_MOB_ACTION_ATTACK( const int channel, const int mob_id )
{
	char		buf[SIZEOF_MAX_BODY + 1];


	if( D_SetMob_attack_to_char( channel, mob_id, (struct body_MG_MOB_ATTACK_CHAR *)buf) )
	{
		P_send_queue_in( channel, MG_MOB_ATTACK_CHAR, sizeof(struct body_MG_MOB_ATTACK_CHAR), buf );
		D_SetMob_action_millisec( channel, mob_id, D_GetMob_delay_attack(channel, mob_id) );
		return;
	}

	D_SetMob_action_millisec( channel, mob_id, 300.0f );
}


static inline void	P_MOB_STATUS_NPC_REGEN( const int channel, const int mob_id )
{
	char		buf[SIZEOF_MAX_BODY + 1];
	int		npc_type, npc_status, add_warp_millisec;


	npc_type	= D_GetMob_npc_type( channel, mob_id );

	switch( npc_type )
	{
		case	NPC_TYPE_NORMAL_NPC :
		case	NPC_TYPE_STORE :

			D_SetMob_regen( channel, mob_id, (struct body_MG_MOB_REGEN *)buf );
			P_send_queue_in( channel, MG_MOB_REGEN, sizeof(struct body_MG_MOB_REGEN), buf );

			D_SetMob_status( channel, mob_id, MOB_STATUS_NPC_CONTINUE );
			D_SetMob_action_millisec( channel, mob_id, MILLISEC_TO_DAY );
			return;


		case	NPC_TYPE_WARP :

			npc_status = D_SetMob_warp( channel, mob_id, &add_warp_millisec );

			switch( npc_status )
			{
				case 	RET_WARP_NOW_OPEN_WAIT_CLOSE :
					D_SetMob_regen( channel, mob_id, (struct body_MG_MOB_REGEN *)buf );
					P_send_queue_in( channel, MG_MOB_REGEN, sizeof(struct body_MG_MOB_REGEN), buf );
					D_SetMob_status( channel, mob_id, MOB_STATUS_NPC_CLOSE );
					break;
				case	RET_WARP_WAIT_OPEN :
					D_SetMob_status( channel, mob_id, MOB_STATUS_NPC_OPEN );
					break;
				case	RET_WARP_CONTINUE :
					D_SetMob_regen( channel, mob_id, (struct body_MG_MOB_REGEN *)buf );
					P_send_queue_in( channel, MG_MOB_REGEN, sizeof(struct body_MG_MOB_REGEN), buf );
					D_SetMob_status( channel, mob_id, MOB_STATUS_NPC_CONTINUE );
					break;
			}

			D_SetMob_action_millisec( channel, mob_id, add_warp_millisec );
			return;


		default :
			dprintf(channel, "[PROC]Error P_MOB_STATUS_NPC_REGEN() mob_id(%d), npc_type(%d) \n"
				, mob_id, npc_type );
	}
}


static inline void	P_MOB_STATUS_NPC_CONTINUE( const int channel, const int mob_id )
{
	D_SetMob_action_millisec( channel, mob_id, MILLISEC_TO_DAY );
}


static inline void	P_MOB_STATUS_NPC_OPEN( const int channel, const int mob_id )
{
	char		buf[SIZEOF_MAX_BODY + 1];
	int		add_warp_millisec;


	dprintf(channel, "[PROC]P_MOB_STATUS_NPC_OPEN() : mob_id(%d) \n", mob_id );

	D_SetMob_regen( channel, mob_id, (struct body_MG_MOB_REGEN *)buf );
	P_send_queue_in( channel, MG_MOB_REGEN, sizeof(struct body_MG_MOB_REGEN), buf );

	D_SetMob_warp( channel, mob_id, &add_warp_millisec );

	D_SetMob_status( channel, mob_id, MOB_STATUS_NPC_CLOSE );
	D_SetMob_action_millisec( channel, mob_id, add_warp_millisec );
}


static inline void	P_MOB_STATUS_NPC_CLOSE( const int channel, const int mob_id )
{
	char		buf[SIZEOF_MAX_BODY + 1];
	int		add_warp_millisec;


	dprintf(channel, "[PROC]P_MOB_STATUS_NPC_CLOSE() : mob_id(%d) \n", mob_id );

	D_SetMob_extinct( channel, mob_id, (struct body_MG_MOB_EXTINCT *)buf );
	P_send_queue_in( channel, MG_MOB_EXTINCT, sizeof(struct body_MG_MOB_EXTINCT), buf );

	D_SetMob_warp( channel, mob_id, &add_warp_millisec );

	D_SetMob_status( channel, mob_id, MOB_STATUS_NPC_OPEN );
	D_SetMob_action_millisec( channel, mob_id, add_warp_millisec );
}






// ----------------------------------------------------------------------------------------
//
static inline int	P_mob_search_target( const int channel, const int mob_id, const int search_sec )
{
	if( (G_Sec - D_GetMob_action_sec(channel, mob_id, ACTION_SEC_SEARCH_TARGET)) > search_sec )
	{
		D_SetMob_action_sec( channel, mob_id, ACTION_SEC_SEARCH_TARGET );

		if( D_SetMob_search_target_char(channel, mob_id) )
			P_mob_new_skill( channel, mob_id, SKILL_AI_COND_COMBAT_START, -1 );
	}

	return (0);
}


static inline void	P_char_attack_to_mob( const int channel, const int mob_id, const int char_id, const int damage
			, const int damage_type, const int attack_dir, const int mob_hp )
{
	char		buf[SIZEOF_MAX_BODY + 1];
	int		isProcess, list_cnt;


	P_mob_cancel_skill( channel, mob_id, damage_type );


	isProcess	= D_SetChar_attack_to_mob( channel, char_id, mob_id, attack_dir, damage_type, damage, mob_hp, buf );

	switch( isProcess )
	{
		case	RET_ATTACKED_DEATH :
			D_GetMob_target_list( channel, mob_id, buf, &list_cnt );
			P_send_queue_in( channel, MG_MOB_DAMAGE_LIST, (sizeof(struct body1_MG_MOB_DAMAGE_LIST) 
				+ list_cnt * sizeof(struct body2_MG_MOB_DAMAGE_LIST)), buf );

			D_SetMob_status( channel, mob_id, MOB_STATUS_DEATH );
			D_SetMob_action_millisec( channel, mob_id, D_GetMob_delay_death(channel, mob_id) );
			return;

		case	RET_ATTACKED_IMPACT_MOVE :
			P_send_queue_in( channel, MG_MOB_MOVE, sizeof(struct body_MG_MOB_MOVE), buf );
			return;

		case	RET_ATTACKED_COMBAT_START_SKILL :
			P_mob_new_skill( channel, mob_id, SKILL_AI_COND_COMBAT_START, -1 );
			return;

		case	RET_ATTACKED_ESCAPE_HP_SKILL :
			P_mob_new_skill( channel, mob_id, SKILL_AI_COND_ESCAPE_HP, -1 );
			return;
	}
}


static inline int	P_mob_new_skill( const int channel, const int mob_id, const int skill_ai_condition, const int delay_sec )
{
	char		buf[SIZEOF_MAX_BODY + 1];
	int		target_cnt;


	if( D_GetMob_skill_num(channel, mob_id, skill_ai_condition) < 1 )
		return (0);
	
	if( (G_Sec - D_GetMob_skill_sec(channel, mob_id, skill_ai_condition)) < delay_sec )
		return (0);

	switch( skill_ai_condition )
	{
		case	SKILL_AI_COND_WANDER :
		case	SKILL_AI_COND_COMBAT :
		case	SKILL_AI_COND_BOMB_3 :
		case	SKILL_AI_COND_EVERY_30 :
		case	SKILL_AI_COND_MOB_HP_DOWN :
		case	SKILL_AI_COND_MY_HP_DOWN :
			{
				D_SetMob_skill_sec( channel, mob_id, skill_ai_condition );
				D_SetMob_skill_num( channel, mob_id, skill_ai_condition, 1 );
			}
			break;
		case	SKILL_AI_COND_COMBAT_START :
		case	SKILL_AI_COND_LIMIT_3_60 :
		case	SKILL_AI_COND_LIMIT_5_60 :
		case	SKILL_AI_COND_LIMIT_8_60 :
		case	SKILL_AI_COND_ESCAPE_HP	:
		case	SKILL_AI_COND_ESCAPE_END :
			{
				D_SetMob_skill_sec( channel, mob_id, skill_ai_condition );
				D_SetMob_skill_num( channel, mob_id, skill_ai_condition, 0 );
			}
			break;
		default :
			dprintf(channel, "[PROC]Error P_mob_new_skill() skill_ai_condition(%d) \n", skill_ai_condition );
			return (0);
	}


	switch( D_SetMob_new_skill(channel, mob_id, skill_ai_condition, buf, &target_cnt) )
	{
		case	RET_SKILL_NO_HAVE :
			D_SetMob_skill_num( channel, mob_id, skill_ai_condition, 0 );
			return (0);

		case	RET_SKILL_READY :
			P_send_queue_in( channel, MG_MOB_SKILL_READY, sizeof(struct body_MG_MOB_SKILL_READY), buf );
			D_SetMob_action_millisec( channel, mob_id, D_GetMob_skill_casting_millisec(channel, mob_id) );
			return (1);

		case	RET_SKILL_ACTION :
			P_send_queue_in( channel, MG_MOB_SKILL_ACTION, sizeof(struct body1_MG_MOB_SKILL_ACTION) 
					+ (target_cnt * sizeof(struct body2_MG_MOB_SKILL_ACTION)), buf );
			D_SetMob_action_millisec( channel, mob_id, D_GetMob_skill_action_millisec(channel, mob_id) );

			if( !D_GetMob_skill_action_cnt(channel, mob_id) )
				D_SetMob_init_current_skill( channel, mob_id );
			return (1);
	}

	return (0);
}


static inline int	P_mob_casting_skill( const int channel, const int mob_id )
{
	char		buf[SIZEOF_MAX_BODY + 1];
	int		target_cnt;


	switch( D_SetMob_casting_skill(channel, mob_id, buf, &target_cnt) )
	{
		case	RET_SKILL_ACTION :
			P_send_queue_in( channel, MG_MOB_SKILL_ACTION, sizeof(struct body1_MG_MOB_SKILL_ACTION) 
					+ (target_cnt * sizeof(struct body2_MG_MOB_SKILL_ACTION)), buf );
			D_SetMob_action_millisec( channel, mob_id, D_GetMob_skill_action_millisec(channel, mob_id) );

			if( !D_GetMob_skill_action_cnt(channel, mob_id) )
				D_SetMob_init_current_skill( channel, mob_id );
			return (1);

		case	RET_SKILL_CANCEL :
			P_send_queue_in( channel, MG_MOB_SKILL_CANCEL, sizeof(struct body_MG_MOB_SKILL_CANCEL), buf );
			D_SetMob_action_millisec( channel, mob_id, 0.0f );

			D_SetMob_init_current_skill( channel, mob_id );
			return (1);
	}

	return (0);
}


static inline void	P_mob_cancel_skill( const int channel, const int mob_id, const int damage_type )
{
	char		buf[SIZEOF_MAX_BODY + 1];


	if( D_ChkMob_cancel_skill(channel, mob_id, damage_type) )
	{
		D_SetMob_cancel_skill( channel, mob_id, buf );
		P_send_queue_in( channel, MG_MOB_SKILL_CANCEL, sizeof(struct body_MG_MOB_SKILL_CANCEL), buf );
		D_SetMob_init_current_skill( channel, mob_id );
		D_SetMob_action_millisec( channel, mob_id, 0.0f );
	}
}


