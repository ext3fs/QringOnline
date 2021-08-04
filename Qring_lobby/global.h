/*____________________________________________________________

	Project name : Qring Online
	File name    : global.h
	Description  : global value 
	Date         : February 14 2005
_____________________________________________________________*/



//____________________________________________________________
//billing
#define	BILLING_GAME_CODE			"Qr"


//_____________________________________________________________
//port
/*
#define	LOBBY_PORT				(1818)
#define	GAME_PORT				(1828)
#define	MOB_PORT				(2828)
#define	SCRIPT_PORT				(1819)
#define	BILLING_PORT				(1818)
#define	DBAGENT_PORT				(1820)
*/

#define	LOBBY_PORT				(7024)
#define	GAME_PORT				(7024)
#define	MOB_PORT				(8024)
#define	SCRIPT_PORT				(7024)
#define	BILLING_PORT				(7024)
#define	DBAGENT_PORT				(7024)


//_____________________________________________________________
//socket
#define	MAX_FD					(1024)
#define	SOCKET_BUF_SIZE				(8192*20)
#define	APP_BUF_SIZE				(8192*20)


//_____________________________________________________________
//sizeof
#define	SIZEOF_SHORT				(2)
#define	SIZEOF_INT				(4)
#define	SIZEOF_HEADER_BILLING			(8)
#define	SIZEOF_HEADER_NORMAL			(8)
#define	SIZEOF_HEADER_SPECIAL			(16)
#define	SIZEOF_MAX_GAME_PACKET_BODY		(8192-SIZEOF_HEADER_NORMAL)
#define	SIZEOF_MAX_SERV_PACKET_BODY		(8192*2-SIZEOF_HEADER_SPECIAL)


//_____________________________________________________________
//heartbeat
#define	SEC_HEARTBEAT				(300)


//_____________________________________________________________
//queue
#define	QUEUE_CNT				(30000)


//_____________________________________________________________
//rand
#define	RAND_CNT				(30000)


//_____________________________________________________________
//channel
#define	CHANNEL_PER_MACHINE			(6)
#define	USER_PER_CHANNEL			(500)
#define	MAX_MACHINE				(999/CHANNEL_PER_MACHINE)


//_____________________________________________________________
//connection
#define	CONNECTION_BILLING				(1)
#define	CONNECTION_LOGIN_DBAGENT			(1)


//_____________________________________________________________
//script
#define	MAX_SCRIPT_FILE_CNT			(16)


//_____________________________________________________________
//map
#define	MAP_X_TILE				(1024)
#define	MAP_Z_TILE				(128)	
#define	VIEW_BLOCK_TILE				(128)

#if (MAP_X_TILE % VIEW_BLOCK_TILE) == 0
	#define	VIEW_BLOCK_CNT	(MAP_X_TILE/VIEW_BLOCK_TILE + 2)
#else
	#define	VIEW_BLOCK_CNT	(MAP_X_TILE/VIEW_BLOCK_TILE + 3)
#endif	



//_____________________________________________________________
//char
#define	MAX_CHAR_CNT				(3)

#define	MAX_CHAR_SEX				(1)
#define	MAX_CHAR_HAIR				(0)
#define	MAX_CHAR_FACE				(0)



//_____________________________________________________________
//send type
#define	SEND_EXIT_THREAD			(1)
#define	SEND_INTER_CONN_FD			(2)
#define	SEND_INTER_CLOSE_FD			(3)
#define	SEND_INTER_BLOCK_INSERT_PC		(4)
#define	SEND_INTER_BLOCK_DELETE_PC		(5)
#define	SEND_LOGIN				(6)
#define	SEND_UNICAST				(7)
#define	SEND_BROADCAST				(8)
#define	SEND_BLOCK_ALLCAST			(9)
#define	SEND_BLOCK_OTHERCAST			(10)



//_____________________________________________________________
//include
#define _REENTRANT

#include	<errno.h>
#include	<stdio.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/time.h>
#include	<sys/socket.h>
#include	<string.h>
#include	<pthread.h>
#include	<netinet/in.h>
#include	<signal.h>
#include	<pthread.h>
#include	<stdarg.h>
#include	<arpa/inet.h>
#include	<sys/epoll.h>
#include	<sys/resource.h>
#include	<fcntl.h>
#include	<stdlib.h>
#define	__USE_XOPEN
#include	<poll.h>

#include	"header.h"


struct	send_buf
{
	pthread_mutex_t		mutex;
	int			active;
	int			login;
	int			size;
	char			buf[APP_BUF_SIZE];
};  


extern	pid_t				G_Pid;
extern	volatile	sig_atomic_t	G_Run;
extern	volatile	time_t		G_Sec;
extern	int				G_Game;
extern	int				G_World;
extern	int				G_Lobby;
extern	int				G_Server_code;
extern	int				G_Demon;
extern	char				G_Script_ip[SIZEOF_IP+1];
extern	char				G_Billing_ip[SIZEOF_IP+1];
extern	char				G_Login_dbagent_ip[SIZEOF_IP+1];
extern	int				G_Epoll_handler;
extern	int				G_Unix_close_fd[2];
extern	int				G_Unix_live_fd[2];
extern	int				G_Unix_tick[2];
extern	int				G_Unix_login_dbagent_idx[2];
extern	int				G_Unix_dbagent_idx[2];
extern	int				G_Unix_billing_idx[2];
extern	int				G_Udp_fd;
extern	int				G_Listen_fd;
extern	volatile	int		G_Connection_number;
extern	volatile	int		G_Login_number;
extern	struct	send_buf		G_Send_buf[MAX_FD+1];

