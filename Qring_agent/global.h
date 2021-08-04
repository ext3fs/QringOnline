
#ifndef	__global_h__
#define	__global_h__




// ----------------------------------------------------------------
// version
#define	VERSION					"20041216"



// ----------------------------------------------------------------
// port
//#define	ACCEPT_PORT				(1820)
//#define	SCRIPTSERV_PORT				(1819)
#define	ACCEPT_PORT				(7024)
#define	SCRIPTSERV_PORT				(7024)



// ----------------------------------------------------------------
// recv thread
#define	MAX_RECVT_CNT				(10)



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

#define SIZEOF_HEADER_DBAGENT			(16)
#define SIZEOF_MAX_BODY				(8192 * 2)
#define SIZEOF_MAX_PACKET			(SIZEOF_HEADER_DBAGENT + SIZEOF_MAX_BODY)

#define	SIZEOF_SNDBUF				(2*1024*1024)
#define	SIZEOF_RCVBUF				(2*1024*1024)



// ----------------------------------------------------------------
// live checking
#define	LIMIT_SEC_TO_PROC			(5)
#define	LIMIT_SEC_TO_WARNING			(60)
#define LIMIT_SEC_TO_CLOSE                      (LIMIT_SEC_TO_WARNING + (10))



// ----------------------------------------------------------------
// exit status
#define	CONN_STATUS_NORMAL			(0)
#define	CONN_STATUS_ABNORMAL			(1)



// ----------------------------------------------------------------
// socket 
#define	FD_IS_EMPTY				(-1)



// ----------------------------------------------------------------
// DBMS(Mysql)
#define DB_TBL_VERSION				"version_table"
#define DB_TBL_SERVER_IDX			"svr_idx_table"

#define DB_TBL_MEMBER				"tblP_UserkeyInfo"
#define DB_TBL_CURRENT_USER			"tblP_CurrentUser"
#define DB_TBL_GAME_LOG				"tblP_GameLog"
#define DB_TBL_CHAR				"char_table"
#define DB_TBL_ITEM				"item_table"

#define	MAX_QUERY				(4096)


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
	char			ipaddress[SIZEOF_IP + 1];

	volatile int		login;	//GA_INIT
	volatile int		live;	//LIVE CHECK
	time_t			sec_live;

};





// ----------------------------------------------------------------
// global variable
extern volatile sig_atomic_t		G_run;
extern volatile int			G_daemon;
extern volatile int			G_connection;
extern struct database			G_member_db;
extern struct database			G_world_db;

extern int				G_tcp_fd;
extern int				G_inner_acpt_fd[2];
extern int				G_inner_recv_fd[MAX_RECVT_CNT][2];


#endif
