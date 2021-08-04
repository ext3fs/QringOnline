/*____________________________________________________________

	Project name : Qring online
	File name    : global.h
	Description  : global value 
	Date         : February 14 2005
_____________________________________________________________*/


//_____________________________________________________________
//version
#define	VERSION					"20041216"



//_____________________________________________________________
//thread
#define	TOTAL_THREAD_CNT			(6)



//_____________________________________________________________
//port
#define	BASE_LISTEN_PORT			(3818)
#define	SCRIPT_PORT				(1819)
#define	DBAGENT_PORT				(1820)



//_____________________________________________________________
//dbagent
#define	DBAGENT_CONN_CNT			(1)



//_____________________________________________________________
//socket
#define	MAX_FD					(4096)
#define	SOCKET_BUF_SIZE				(8192*20)
#define	APP_BUF_SIZE				(8192*20)



//_____________________________________________________________
//sizeof
#define	SIZEOF_INT				(4)
#define	SIZEOF_HEADER_NORMAL			(8)
#define	SIZEOF_HEADER_SPECIAL			(16)
#define	SIZEOF_MAX_PACKET_BODY			(1008)



//_____________________________________________________________
//heartbeat
#define	SEC_TO_CLOSE				(400)
#define	SEC_HEARTBEAT				(300)



//_____________________________________________________________
//queue
#define	QUEUE_CNT				(30000)



//_____________________________________________________________
//rand
#define	RAND_CNT				(30000)



//_____________________________________________________________
//script
#define	MAX_SCRIPT_FILE_CNT			(16)



//_____________________________________________________________
//friend
#define	MAX_FRIEND_CNT				(1)



//_____________________________________________________________
//pc
#define	USER_ID_LEN				(16)
#define	PASSWORD_LEN				(16)
#define	CHAR_NAME_LEN				(16)



//_____________________________________________________________
//Send Type
#define	SEND_EXIT_THREAD			(1)
#define	SEND_INTER_CONN_FD			(2)
#define	SEND_INTER_CLOSE_FD			(3)
#define	SEND_INTER_BLOCK_INSERT_PC		(4)
#define	SEND_INTER_BLOCK_DELETE_PC		(5)
#define	SEND_UNICAST				(6)
#define	SEND_BROADCAST				(7)
#define	SEND_BLOCK_ALLCAST			(8)
#define	SEND_BLOCK_OTHERCAST			(9)



//_____________________________________________________________
//include
#define _REENTRANT

#include	<errno.h>
#include	<stdio.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/time.h>
#include	<sys/socket.h>
#include	<stdlib.h>
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
#define	__USE_XOPEN
#include	<poll.h>

#pragma	pack(1)
#include	"header.h"
#pragma	pack()


struct	send_buf
{
	pthread_mutex_t	mutex;
	int		active;
	int		size;
	char		buf[APP_BUF_SIZE];
};


extern	pid_t				G_Pid;
extern	volatile	sig_atomic_t	G_Run;
extern	volatile	time_t		G_Sec;
extern	unsigned	short		G_Game;
extern	unsigned	short		G_World;
extern	unsigned	short		G_Demon;
extern	char				G_Script_ip[SIZEOF_IP+1];
extern	unsigned	short		G_Script_port;
extern	int				G_Epoll_handler;
extern	int				G_Unix_close_fd[2];
extern	int				G_Unix_live_fd[2];
extern  int				G_Unix_dbagent_idx[2];
extern	int				G_Udp_fd;
extern	int				G_Listen_fd;
extern	volatile	int		G_Connection_number;
extern	volatile	int		G_Login_number;
extern	struct	send_buf		G_Send_buf[MAX_FD];
