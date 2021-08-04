
#ifndef	__global_h__
#define	__global_h__




// ----------------------------------------------------------------
// definition declare
//#define	ACCEPT_PORT				(1819)
#define	ACCEPT_PORT				(7024)



// ----------------------------------------------------------------
// recv thread
#define	MAX_RECVT_CNT				(2)	



// ----------------------------------------------------------------
// 
#define	SUCC					(0)
#define	FAIL					(1)



// ----------------------------------------------------------------
// 
#define	M_LOG					(999)



// ----------------------------------------------------------------
// RecvT Status
#define THREAD_STATUS_EMPTY			(0)
#define THREAD_STATUS_RUNNING			(1)
#define	THREAD_STATUS_ERROR			(2)



// ----------------------------------------------------------------
// SIZE DEFINITION
#define SIZEOF_CHAR				(1)
#define SIZEOF_SHORT				(2)
#define	SIZEOF_INT				(4)
#define	SIZEOF_FLOAT				(4)
#define SIZEOF_DOUBLE				(8)

#define SIZEOF_HEADER				(16)
#define SIZEOF_MAX_BODY				(1024)
#define SIZEOF_MAX_PACKET			(SIZEOF_HEADER+SIZEOF_MAX_BODY)

#define	SIZEOF_SNDBUF				(2*1024*1024)
#define	SIZEOF_RCVBUF				(2*1024*1024)



// ----------------------------------------------------------------
// packet check
#define LIMIT_SEC_TO_PROC			(5)



// ----------------------------------------------------------------
// exit status
#define	CONN_STATUS_NORMAL			(0)
#define	CONN_STATUS_ABNORMAL			(1)



// ----------------------------------------------------------------
// socket 
#define	FD_IS_EMPTY				(-1)



// ----------------------------------------------------------------
// DBMS(Mysql)
#define DB_TBL_ID				"id_table"
#define DB_TBL_CHAR				"char_table"
#define DB_TBL_VERSION				"version_table"
#define DB_TBL_SERVER_IDX			"svr_idx_table"
#define DB_TBL_WORLD				"world_name_table"
#define DB_TBL_MOBSERV				"mob_svr_table"
#define DB_TBL_DBAGENT				"dbagent_table"



// ----------------------------------------------------------------
// Map
#define MAP_X_TILE				(1024)
#define MAP_Y_TILE				(128)
#define MAP_SIZE				((MAP_X_TILE*MAP_Y_TILE) + 32 + 4 + 4 )



// ----------------------------------------------------------------
// pc
#define MAX_USER_CHAR_CNT			(3)





// ----------------------------------------------------------------
// header file
#include 	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<stdarg.h>
#include	<pthread.h>
#include 	<signal.h>
#include	<unistd.h>
#include	<netinet/in.h>
#include	<sys/socket.h>
#include	<sys/resource.h>
#include	<sys/fcntl.h>
#include	<sys/time.h>
#include	<errno.h>
#include	<arpa/inet.h>
#include	</home/db/include/mysql/mysql.h>
#include	<sys/epoll.h>

#pragma pack(1)
#include	"../Qring_common/header.h"







// ----------------------------------------------------------------
// STRUCTURE DEFINITION
struct database
{
	char		ipaddress[SIZEOF_IP + 1];
	char		name	[255];
	char		id	[255];
	char		pwd	[255];
};

struct server 
{
	volatile int		fd;
	volatile int		uniq;

	char			ipaddress[SIZEOF_IP + 1];
	volatile int		login;	//GA_INIT
};

struct map_info
{
	unsigned int		size;
	char			data[MAP_SIZE];
};

struct script_file
{
	unsigned int		line_cnt;

	unsigned int		size;
	char			*data;
};






// ----------------------------------------------------------------
// global variable
extern volatile sig_atomic_t		G_run;
extern volatile int			G_daemon;
extern volatile int			G_connection;
extern char				G_version[VERSION_LEN + 1];
extern struct database			G_script_db;

extern int				G_tcp_fd;
extern int				G_inner_acpt_fd[2];
extern int				G_inner_recv_fd[MAX_RECVT_CNT][2];

extern int				G_MapFileCnt;
extern struct map_info *		G_MapList;
extern struct script_file *		G_MobList;
extern int				G_TypeMobFileCnt;
extern struct script_file *		G_MobTypeList;
extern int				G_LevelFileCnt;
extern struct script_file *		G_LevelList;
extern int				G_CharSkillFileCnt;
extern struct script_file *		G_CharSkillList;
extern int				G_MobSkillFileCnt;
extern struct script_file *		G_MobSkillList;
extern int				G_ItemFileCnt;
extern struct script_file *		G_ItemList;
extern int				G_DropFileCnt;
extern struct script_file *		G_DropList;
extern int				G_WarpFileCnt;
extern struct script_file *		G_WarpList;
extern int				G_StoreFileCnt;
extern struct script_file *		G_StoreList;




#endif
