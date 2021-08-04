
#ifndef	__global_h__
#define	__global_h__



//_____________________________________________________________
//version
#define	VERSION					"20041216"



//_____________________________________________________________
//port
//#define	SCRIPTSERV_PORT				(1819)
//#define	GAMESERV_PORT				(2828)
#define	SCRIPTSERV_PORT				(7024)
#define	GAMESERV_PORT				(8024)



//_____________________________________________________________
//channel
#define	TOTAL_CHANNEL_CNT			(6)

#define	NORMAL_CHANNEL_CNT			(6)
#define	PROC_CHARGE_NORMAL_CHANNEL		(1)

#define	INS_CHANNEL_CNT				(TOTAL_CHANNEL_CNT - NORMAL_CHANNEL_CNT)
#define	PROC_CHARGE_INS_CHANNEL			(INS_CHANNEL_CNT + 1)



//_____________________________________________________________
//map
#define	MAX_X_TILE				(1024)
#define	MAX_Z_TILE				(128)

#define	MAX_MAP_SIZE				(MAX_X_TILE * MAX_Z_TILE + 40)
#define	MAX_PATH_CNT				(MAX_X_TILE * MAX_Z_TILE)
#define	MAX_HISTORY_CNT				(500)



//_____________________________________________________________
//script
#define	MAX_SCRIPT_FILE_CNT			(20)



//_____________________________________________________________
//queue
#define	MAX_QUEUE_CNT				(100)



//_____________________________________________________________
//char
#define	MAX_CHAR_CNT				(4096)



//_____________________________________________________________
//target
#define	TARGET_LIST_CNT				(10)



//_____________________________________________________________
//date
#define	MINUTE					(60)
#define	HOUR					(60 *MINUTE)
#define	DAY					(24 *HOUR)
#define	YEAR					(365*DAY)



//_____________________________________________________________
//sizeof
#define	SIZEOF_CHAR				(1)
#define	SIZEOF_SHORT				(2)
#define	SIZEOF_INT				(4)
#define	SIZEOF_FLOAT				(4)
#define	SIZEOF_DOUBLE				(8)

#define	SIZEOF_HEADER_NORMAL			(8)
#define	SIZEOF_HEADER_SPECIAL			(16)
#define	SIZEOF_MAX_BODY				(1024)
#define	SIZEOF_MAX_PACKET_NORMAL		(SIZEOF_HEADER_NORMAL +SIZEOF_MAX_BODY)
#define	SIZEOF_MAX_PACKET_SPECIAL		(SIZEOF_HEADER_SPECIAL+SIZEOF_MAX_BODY)

#define	SIZEOF_RCVBUF				(128*1024)
#define	SIZEOF_SNDBUF				(128*1024)



//_____________________________________________________________
//live check
#define	LIMIT_SEC_TO_PROC			(5)
#define	LIMIT_SEC_TO_WARNING			(60)
#define	LIMIT_SEC_TO_CLOSE			(LIMIT_SEC_TO_WARNING+(30))



//_____________________________________________________________
//millisec
#define	MILLISEC_TO_EXTINCT			(5000)
#define	MILLISEC_TO_AWAKE			(0)
#define	MILLISEC_TO_NEXT_MOVE			(1000)
#define	MILLISEC_TO_DAY				(1000 * DAY)



//_____________________________________________________________
//action_sec
#define	MAX_ACTION_SEC_CNT			(2)
#define	ACTION_SEC_WANDER_ANIMATION		(0)
#define	ACTION_SEC_SEARCH_TARGET		(1)



//_____________________________________________________________
//return code
#define	RET_MOVE_STAY				(0)
#define	RET_MOVE_XZ				(1)
#define	RET_MOVE_AREA_OVER			(2)
#define	RET_MOVE_NOT_FOUND			(3)

#define	RET_SKILL_NO_HAVE			(-1)
#define	RET_SKILL_EMPTY				(0)
#define	RET_SKILL_READY				(1)
#define	RET_SKILL_ACTION			(2)
#define	RET_SKILL_CANCEL			(3)
#define	RET_SKILL_NO_TARGET			(4)
#define	RET_SKILL_CASTING			(5)
#define	RET_SKILL_NO_CASTING			(6)

#define	RET_ATTACKED_ERROR			(-1)
#define	RET_ATTACKED_NORMAL			(0)
#define	RET_ATTACKED_DEATH			(1)
#define	RET_ATTACKED_IMPACT_MOVE		(2)
#define	RET_ATTACKED_COMBAT_START_SKILL		(3)
#define	RET_ATTACKED_ESCAPE_HP_SKILL		(4)

#define	RET_WARP_CONTINUE			(0)
#define	RET_WARP_WAIT_OPEN			(1)
#define	RET_WARP_NOW_OPEN_WAIT_CLOSE		(2)



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



//_____________________________________________________________
//mob type
#define	NPC_TYPE_CNT				(3)
#define	NPC_TYPE_MONSTER			(0)
#define	NPC_TYPE_STORE				(1)
#define	NPC_TYPE_WARP				(2)
#define	NPC_TYPE_NORMAL_NPC			(3)



//_____________________________________________________________
//mob status
#define	MOB_STATUS_MOVE_REGEN			(1)
#define	MOB_STATUS_WANDER			(2)
#define	MOB_STATUS_PERSUIT			(3)
#define	MOB_STATUS_ATTACK			(4)
#define	MOB_STATUS_ESCAPE			(5)
#define	MOB_STATUS_PERFECT			(6)
#define	MOB_STATUS_GATHER			(7)
#define	MOB_STATUS_DEATH			(8)
#define	MOB_STATUS_EXTINCT			(9)
#define	MOB_STATUS_REGEN			(10)
#define	MOB_STATUS_AWAKE			(11)
#define	MOB_STATUS_NPC_REGEN			(12)
#define	MOB_STATUS_NPC_CONTINUE			(13)
#define	MOB_STATUS_NPC_OPEN			(14)
#define	MOB_STATUS_NPC_CLOSE			(15)

#define	MOB_ACTION_WANDER			(1)
#define	MOB_ACTION_PERSUIT			(2)
#define	MOB_ACTION_ATTACK			(3)



//_____________________________________________________________
//mob skill
#define	MAX_MOB_HAVE_SKILL_CNT			(5)
#define	MAX_SKILL_ORDER_CNT			(3)

//ai_condition
#define	MAX_SKILL_AI_COND			(12)
#define	SKILL_AI_COND_WANDER			(1)
#define	SKILL_AI_COND_COMBAT_START		(2)
#define	SKILL_AI_COND_COMBAT			(3)
#define	SKILL_AI_COND_BOMB_3			(4)
#define	SKILL_AI_COND_EVERY_30			(5)
#define	SKILL_AI_COND_LIMIT_3_60		(6)
#define	SKILL_AI_COND_LIMIT_5_60		(7)
#define	SKILL_AI_COND_LIMIT_8_60		(8)
#define	SKILL_AI_COND_ESCAPE_HP			(9)
#define	SKILL_AI_COND_ESCAPE_END		(10)
#define	SKILL_AI_COND_MOB_HP_DOWN		(11)
#define	SKILL_AI_COND_MY_HP_DOWN		(12)

//target_object
#define	MAX_SKILL_TARGET_OBJECT			(4)
#define	SKILL_TARGET_OBJECT_MYSELF		(1)
#define	SKILL_TARGET_OBJECT_CHAR		(2)
#define	SKILL_TARGET_OBJECT_MOB			(3)
#define	SKILL_TARGET_OBJECT_XZ			(4)

//target_apply
#define	MAX_SKILL_TARGET_APPLY			(3)
#define	SKILL_TARGET_APPLY_MYSELF		(1)
#define	SKILL_TARGET_APPLY_CHAR			(2)
#define	SKILL_TARGET_APPLY_MOB			(3)

//range_type
#define	MAX_SKILL_RANGE_TYPE			(2)
#define	SKILL_RANGE_SINGLE			(0)
#define	SKILL_RANGE_CIRCLE			(1)
#define	SKILL_RANGE_SPRAY			(2)

//pattern
#define	SKILL_PATTERN_MY_MY_SINGLE		(0x110)
#define	SKILL_PATTERN_MY_MY_CIRCLE		(0x111)
#define	SKILL_PATTERN_MY_MY_SPRAY		(0x112)
#define	SKILL_PATTERN_MY_CHAR_SINGLE		(0x120)
#define	SKILL_PATTERN_MY_CHAR_CIRCLE		(0x121)
#define	SKILL_PATTERN_MY_CHAR_SPRAY		(0x122)
#define	SKILL_PATTERN_MY_MOB_SINGLE		(0x130)
#define	SKILL_PATTERN_MY_MOB_CIRCLE		(0x131)
#define	SKILL_PATTERN_MY_MOB_SPRAY		(0x132)

#define	SKILL_PATTERN_CHAR_MY_SINGLE		(0x210)
#define	SKILL_PATTERN_CHAR_MY_CIRCLE		(0x211)
#define	SKILL_PATTERN_CHAR_MY_SPRAY		(0x212)
#define	SKILL_PATTERN_CHAR_CHAR_SINGLE		(0x220)
#define	SKILL_PATTERN_CHAR_CHAR_CIRCLE		(0x221)
#define	SKILL_PATTERN_CHAR_CHAR_SPRAY		(0x222)
#define	SKILL_PATTERN_CHAR_MOB_SINGLE		(0x230)
#define	SKILL_PATTERN_CHAR_MOB_CIRCLE		(0x231)
#define	SKILL_PATTERN_CHAR_MOB_SPRAY		(0x232)

#define	SKILL_PATTERN_MOB_MY_SINGLE		(0x310)
#define	SKILL_PATTERN_MOB_MY_CIRCLE		(0x311)
#define	SKILL_PATTERN_MOB_MY_SPRAY		(0x312)
#define	SKILL_PATTERN_MOB_CHAR_SINGLE		(0x320)
#define	SKILL_PATTERN_MOB_CHAR_CIRCLE		(0x321)
#define	SKILL_PATTERN_MOB_CHAR_SPRAY		(0x322)
#define	SKILL_PATTERN_MOB_MOB_SINGLE		(0x330)
#define	SKILL_PATTERN_MOB_MOB_CIRCLE		(0x331)
#define	SKILL_PATTERN_MOB_MOB_SPRAY		(0x332)

#define	SKILL_PATTERN_XZ_MY_SINGLE		(0x410)
#define	SKILL_PATTERN_XZ_MY_CIRCLE		(0x411)
#define	SKILL_PATTERN_XZ_MY_SPRAY		(0x412)
#define	SKILL_PATTERN_XZ_CHAR_SINGLE		(0x420)
#define	SKILL_PATTERN_XZ_CHAR_CIRCLE		(0x421)
#define	SKILL_PATTERN_XZ_CHAR_SPRAY		(0x422)
#define	SKILL_PATTERN_XZ_MOB_SINGLE		(0x430)
#define	SKILL_PATTERN_XZ_MOB_CIRCLE		(0x431)
#define	SKILL_PATTERN_XZ_MOB_SPRAY		(0x432)

//function
#define	MAX_SKILL_FUNC_TYPE			(256)
#define	SKILL_FUNC_HELP_1			(150)
#define	SKILL_FUNC_HELP_2			(151)
#define	SKILL_FUNC_GATHER			(152)
#define	SKILL_FUNC_SUMMON			(155)



//_____________________________________________________________
//header
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<stdarg.h>
#include	<math.h>
#include	<signal.h>
#include	<pthread.h>
#include	<unistd.h>
#include	<netinet/in.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<sys/resource.h>
#include	<sys/fcntl.h>
#include	<sys/time.h>
#include	<sys/epoll.h>
#include	<sys/stat.h>
#include	<arpa/inet.h>
#include	<errno.h>
#include	"../Qring_common/header.h"



//_____________________________________________________________
//global structure
struct thread_args
{
	volatile int	tidx;
	volatile int	charge_start_channel;
	volatile int	charge_end_channel;
};

struct connection
{
	volatile int	channel;
	volatile int	fd;
};



//_____________________________________________________________
//global variable
extern volatile sig_atomic_t		G_run;
extern volatile int			G_daemon;

extern volatile time_t			G_Sec;
extern volatile double			G_MilliSec;

extern volatile int			G_Input_fds;
extern int				G_inner_close_fd[2];
extern int				G_inner_send_fd[2];
extern int				G_inner_check_fd[2];

extern volatile int			G_proc_thr_cnt;
extern struct thread_args		*G_proc_thr_args;
extern volatile int			G_channel_proc_thr[TOTAL_CHANNEL_CNT + 1];
extern volatile int			G_channel_ins[TOTAL_CHANNEL_CNT + 1];


#endif
