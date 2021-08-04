/*____________________________________________________________

	Project name : Qring Online
	File name    : proc.c
	Description  : proc module
	Date         : February 14 2005
_____________________________________________________________*/


#include	"global.h"
#include	"util.h"
#include	"db.h"
#include	"mk_packet.h"
#define	_PROC_H_
#include	"proc.h"


static	void	P_remove_client(const int);

/*
static	void	P_proc_BW_GAMESTART_SUCC(const int);
static	void	P_proc_BW_GAMESTART_FAIL(const int);
static	void	P_proc_BW_GAMESTOP_SUCC(const int);
static	void	P_proc_BW_GAMESTOP_FAIL(const int);
static	void	P_proc_BW_USER_CHECK(const int);
static	void	P_proc_BW_DATABASE_ERROR(const int);
static	void	P_proc_BW_SYSTEM_ERROR(const int);
static	void	P_proc_BW_REGSERVER_SUCC(const int);
static	void	P_proc_BW_REGSERVER_FAIL(const int);
static	void	P_proc_BW_ALIVECHECK(const int);
*/

//static	void	P_proc_INTER_CONN_BILLING_FD(const int);
static	void	P_proc_INTER_CONN_LOGIN_DBAGENT_FD(const int);
static	void	P_proc_INTER_CONN_DBAGENT_FD(const int);
static	void	P_proc_INTER_CONN_CLIENT_FD(const int);
static	void	P_proc_INTER_CLOSE_FD(const int);
static	void	P_proc_INTER_TICK(const int);

static	void	P_proc_AW_INIT_RESULT(const int);
static	void	P_proc_AW_LIVE(const int);
static	void	P_proc_AW_CHAR_LIST_RESULT(const int);
static	void	P_proc_AW_CHAR_CREATE_RESULT(const int);
static	void	P_proc_AW_CHAR_DELETE_RESULT(const int);
static	void	P_proc_AW_LOGIN_RESULT(const int);
static	void	P_proc_AW_GET_USER_RESULT(const int);

static	void	P_proc_CL_LOGIN(const int);
static	void	P_proc_CL_LIVE(const int);
static	void	P_proc_CL_WORLD_LIST(const int);
static	void	P_proc_CL_CHANNEL_LIST(const int);
static	void	P_proc_CL_CHAR_LIST(const int);
static	void	P_proc_CL_CHAR_CREATE(const int);
static	void	P_proc_CL_CHAR_DELETE(const int);
static	void	P_proc_CL_CHANNEL_CONNECT(const int);


static	struct	recv_q	*P_recv_q;

static	int		P_session;
static	char		P_send_buf[SIZEOF_HEADER_SPECIAL+SIZEOF_MAX_SERV_PACKET_BODY];
static	int		P_send_size;
static	int		P_billing_fd[CONNECTION_BILLING+1];
static	int		P_login_dbagent_fd[CONNECTION_LOGIN_DBAGENT+1];
static	int		P_dbagent_fd[999];



//____________________________________________________________
//
void *P_Proc_main(void *arg)
{
	U_Printf("\t[PROC]Proc Thread Start !\n");
	
	while(1)
	{
		P_recv_q	= U_Recv_queue_wait();

		switch ( P_recv_q->command )
		{
			/*
			//BW
			case	BW_GAMESTART_SUCC:
				P_proc_BW_GAMESTART_SUCC(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	BW_GAMESTART_FAIL:		
				P_proc_BW_GAMESTART_FAIL(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	BW_GAMESTOP_SUCC:
				P_proc_BW_GAMESTOP_SUCC(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	BW_GAMESTOP_FAIL:
				P_proc_BW_GAMESTOP_FAIL(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	BW_USER_CHECK:
				P_proc_BW_USER_CHECK(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	BW_DATABASE_ERROR:
				P_proc_BW_DATABASE_ERROR(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	BW_SYSTEM_ERROR:
				P_proc_BW_SYSTEM_ERROR(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	BW_REGSERVER_SUCC:
				P_proc_BW_REGSERVER_SUCC(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	BW_REGSERVER_FAIL:
				P_proc_BW_REGSERVER_FAIL(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	BW_ALIVECHECK:
				P_proc_BW_ALIVECHECK(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			*/
			//INTER
			case	INTER_EXIT_THREAD:
				U_Recv_queue_proc_end();
				goto Exit;
			//case	INTER_CONN_BILLING_FD:
			//	P_proc_INTER_CONN_BILLING_FD(P_recv_q->fd);
			//	U_Recv_queue_proc_end();
			//	break;
			case	INTER_CONN_LOGIN_DBAGENT_FD:
				P_proc_INTER_CONN_LOGIN_DBAGENT_FD(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	INTER_CONN_DBAGENT_FD:
				P_proc_INTER_CONN_DBAGENT_FD(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	INTER_CONN_CLIENT_FD:
				P_proc_INTER_CONN_CLIENT_FD(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	INTER_CLOSE_FD:
				P_proc_INTER_CLOSE_FD(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	INTER_TICK:
				P_proc_INTER_TICK(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			//AW
			case	AW_INIT_RESULT:
				P_proc_AW_INIT_RESULT(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	AW_LIVE:
				P_proc_AW_LIVE(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	AW_CHAR_LIST_RESULT:
				P_proc_AW_CHAR_LIST_RESULT(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	AW_CHAR_CREATE_RESULT:
				P_proc_AW_CHAR_CREATE_RESULT(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	AW_CHAR_DELETE_RESULT:
				P_proc_AW_CHAR_DELETE_RESULT(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	AW_LOGIN_RESULT:
				P_proc_AW_LOGIN_RESULT(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	AW_GET_USER_RESULT:
				P_proc_AW_GET_USER_RESULT(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			//CL
			case	CL_LOGIN:
				P_proc_CL_LOGIN(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CL_LIVE:
				P_proc_CL_LIVE(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CL_WORLD_LIST:
				P_proc_CL_WORLD_LIST(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CL_CHANNEL_LIST:
				P_proc_CL_CHANNEL_LIST(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CL_CHAR_LIST:
				P_proc_CL_CHAR_LIST(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CL_CHAR_CREATE:
				P_proc_CL_CHAR_CREATE(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CL_CHAR_DELETE:
				P_proc_CL_CHAR_DELETE(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;
			case	CL_CHANNEL_CONNECT:	
				P_proc_CL_CHANNEL_CONNECT(P_recv_q->fd);
				U_Recv_queue_proc_end();
				break;

			default:
				U_Printf("[PROC]Hack fd(%d) command(%x) user_id(%s)\n",
						P_recv_q->fd, P_recv_q->command, D_GetClient_user_id(P_recv_q->fd));
				P_proc_INTER_CLOSE_FD(P_recv_q->fd);
				U_Recv_queue_proc_end();
		}//end switch
	}//end while  		

Exit:
	U_Printf("\t[PROC]Proc Thread End !\n");
				
	//send LC_SERVER_STOP
	U_Printf("[PROC]send_queue_in BROADCAST LC_SERVER_TROUBLE SERVER_TROUBLE_INSPECT_TIME\n");
	P_send_size	= MK_GetPacket_LC_SERVER_TROUBLE(P_send_buf, SERVER_TROUBLE_INSPECT_TIME);
	U_Send_queue_in(SEND_BROADCAST, 0, 0, 0, P_send_size, P_send_buf);

	U_Send_queue_in(SEND_EXIT_THREAD, 0, 0, 0, 0, NULL);
	return	(NULL);
}


//_________________________________________________________
//
static	void	P_remove_client(const int fd)
{
	int	billing_idx, use_billing_fd, use_login_dbagent_fd;


	//billing
	if( (billing_idx = D_GetClient_billing_idx(fd)) )
	{
		P_billing_fd[billing_idx]	= 0;
		D_SetClient_clear(fd);
		U_Send_queue_in(SEND_INTER_CLOSE_FD, fd, 0, 0, 0, NULL);

		//for( i=1; i<=MAX_FD; i++ )
		//{
		//	if( (D_GetClient_sec_active(i)) && (*D_GetClient_user_id(i)) && (D_GetClient_use_billing_idx(i)) )
		//	{
				//send LC_SERVER_STOP
		//		U_Printf("[PROC]send_queue_in UNICAST(%d) LC_SERVER_TROUBLE SERVER_TROUBLE_BILLING\n", i);
		//		P_send_size	= MK_GetPacket_LC_SERVER_TROUBLE(P_send_buf, SERVER_TROUBLE_BILLING);
		//		U_Send_queue_in(SEND_UNICAST, i, 0, 0, P_send_size, P_send_buf);

		//		P_remove_client(i);
		//	}
		//}

		return;
	}
	
	//login_dbagent
	if( D_GetClient_login_dbagent_idx(fd) )
	{
		P_login_dbagent_fd[D_GetClient_login_dbagent_idx(fd)]	= 0;
		D_SetClient_clear(fd);
		U_Send_queue_in(SEND_INTER_CLOSE_FD, fd, 0, 0, 0, NULL);
		return;
	}

	//dbagent
	if( D_GetClient_dbagent_idx(fd) )
	{
		P_dbagent_fd[D_GetClient_dbagent_idx(fd)]	= 0;
		D_SetClient_clear(fd);
		U_Send_queue_in(SEND_INTER_CLOSE_FD, fd, 0, 0, 0, NULL);
		return;
	}

	use_billing_fd		= P_billing_fd[D_GetClient_use_billing_idx(fd)];
	use_login_dbagent_fd	= P_login_dbagent_fd[D_GetClient_use_login_dbagent_idx(fd)]; 

	//login
	if( *D_GetClient_user_id(fd) )
	{
		--G_Login_number;

		//logout
		if( use_login_dbagent_fd  )
		{
			//send WB_GAMESTOP
			//U_Printf("[PROC]send_queue_in UNICAST(%d) WB_GAMESTOP fd(%d) user_id(%s)\n", use_billing_fd, fd, D_GetClient_user_id(fd));
			//P_send_size	= MK_GetPacket_WB_GAMESTOP(P_send_buf, fd, D_GetClient_user_id(fd), D_GetClient_ip(fd));
			//U_Send_queue_in(SEND_UNICAST, use_billing_fd, 0, 0, P_send_size, P_send_buf);
			
			//send WA_LOGOUT
			U_Printf("[PROC]send_queue_in UNICAST(%d) WB_LOGOUT fd(%d) user_id(%s)\n",
								 use_login_dbagent_fd, fd, D_GetClient_user_id(fd));
			P_send_size	= MK_GetPacket_WA_LOGOUT(P_send_buf, fd, D_GetClient_sec_active(fd), D_GetClient_session(fd), D_GetClient_user_id(fd));
			U_Send_queue_in(SEND_UNICAST, use_login_dbagent_fd, 0, 0, P_send_size, P_send_buf);
		}
		else
		{
			//do noting.. 
			U_Printf("[PROC]do noting logout fd(%d) user_id(%s)\n", fd, D_GetClient_user_id(fd));
		}
	}
	
	D_SetClient_clear(fd);
	U_Send_queue_in(SEND_INTER_CLOSE_FD, fd, 0, 0, 0, NULL);
}


//____________________________________________________________
//INTER
static	void	P_proc_INTER_TICK(const int tick)
{
	int	i;


	if( P_login_dbagent_fd[1] )
	{
		for( i=1; i<=D_GetWorld_channel_cnt(1); i++)
		{
			//send WA_GET_USER
			U_Printf("[PROC]send_queue_in UNICAST(%d) WA_GET_USER world(%d) channel(%d)\n", P_login_dbagent_fd[1], 1, i);
			P_send_size	= MK_GetPacket_WA_GET_USER(P_send_buf, 1, i);
			U_Send_queue_in(SEND_UNICAST, P_login_dbagent_fd[1], 0, 0, P_send_size, P_send_buf);
		}	
	}
}


//static	void	P_proc_INTER_CONN_BILLING_FD(const int billing_fd)
//{
//	D_SetClient_sec_active(billing_fd, G_Sec);
//	D_SetClient_session(billing_fd, ++P_session);
//	D_SetClient_billing_idx(billing_fd, P_recv_q->fd_or_seq);
//	D_SetClient_ip(dbagent_fd, P_recv_q->data);
//	U_Send_queue_in(SEND_INTER_CONN_FD, billing_fd, 0, 0, 0, NULL);
	
	//send	WB_REGSERVER
//	U_Printf("[PROC]send_queue_in UNICAST(%d) WB_REGSERVER\n", billing_fd);
//	P_send_size	= MK_GetPacket_WB_REGSERVER(P_send_buf);	
//	U_Send_queue_in(SEND_UNICAST, billing_fd, 0, 0, P_send_size, P_send_buf);
//}


static	void	P_proc_INTER_CONN_LOGIN_DBAGENT_FD(const int login_dbagent_fd)
{
	D_SetClient_sec_active(login_dbagent_fd, G_Sec);
	D_SetClient_session(login_dbagent_fd, ++P_session);
	D_SetClient_login_dbagent_idx(login_dbagent_fd, P_recv_q->fd_or_seq);
	D_SetClient_ip(login_dbagent_fd, P_recv_q->data);
	U_Send_queue_in(SEND_INTER_CONN_FD, login_dbagent_fd, 0, 0, 0, NULL);
	
	//send	WA_INIT	
	P_send_size	= MK_GetPacket_WA_INIT(P_send_buf, 0);	
	U_Send_queue_in(SEND_UNICAST, login_dbagent_fd, 0, 0, P_send_size, P_send_buf);
}


static	void	P_proc_INTER_CONN_DBAGENT_FD(const int dbagent_fd)
{
	D_SetClient_sec_active(dbagent_fd, G_Sec);
	D_SetClient_session(dbagent_fd, ++P_session);
	D_SetClient_dbagent_idx(dbagent_fd, P_recv_q->fd_or_seq);
	D_SetClient_ip(dbagent_fd, P_recv_q->data);
	U_Send_queue_in(SEND_INTER_CONN_FD, dbagent_fd, 0, 0, 0, NULL);
	
	//send	WA_INIT	
	P_send_size	= MK_GetPacket_WA_INIT(P_send_buf, 0);	
	U_Send_queue_in(SEND_UNICAST, dbagent_fd, 0, 0, P_send_size, P_send_buf);
}


static	void	P_proc_INTER_CONN_CLIENT_FD(const int fd)
{
	D_SetClient_sec_active(fd, G_Sec);
	D_SetClient_session(fd, ++P_session);
	D_SetClient_ip(fd, P_recv_q->data);
	U_Send_queue_in(SEND_INTER_CONN_FD, fd, 0, 0, 0, NULL);
}

	
static	void	P_proc_INTER_CLOSE_FD(const int fd)
{
	if( D_GetClient_sec_active(fd) )
		P_remove_client(fd);
}

/*
//______________________________________________________________
//BW
static	void	P_proc_BW_GAMESTART_SUCC(const int billing_fd)
{
	struct	body_BW_GAMESTART_SUCC	*body	= (struct body_BW_GAMESTART_SUCC *)(P_recv_q->data);
	
	
	if( D_GetClient_sec_active(billing_fd) == 0 )	
		return;

	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_BW_GAMESTART_SUCC) )
	{
		U_Printf("[PROC]Error billing_fd(%d) BW_GAMESTART_SUCC(%d) bad body_size(%d)\n",
				billing_fd, sizeof(struct body_BW_GAMESTART_SUCC), P_recv_q->data_size);
		P_remove_client(billing_fd);
		return;
	}

	//check id
	if( (body->id < 1) || (body->id > MAX_FD) )
	{
		U_Printf("[PROC]Error billing_fd(%d) BW_GAMESTART_SUCC bad id(%d)\n", billing_fd, body->id);
		P_remove_client(billing_fd);
		return;
	}

	//check user_id	
	if( (body->user_id[0] == 0) || (body->user_id[USER_ID_LEN] != 0) )
	{
		body->user_id[USER_ID_LEN]	= 0;
		U_Printf("[PROC]Error billing_fd(%d) BW_GAMESTART_SUCC bad user_id(%s)\n", billing_fd, body->user_id);
		P_remove_client(billing_fd);
		return;
	}
	
	//check user_ip
	if( (body->user_ip[0] == 0) || (body->user_ip[SIZEOF_IP] != 0) )
	{
		body->user_ip[SIZEOF_IP]	= 0;
		U_Printf("[PROC]Error billing_fd(%d) BW_GAMESTART_SUCC user_id(%s) bad user_ip(%s)\n", billing_fd, body->user_id, body->user_ip);
		P_remove_client(billing_fd);
		return;
	}
	
	//check session
	if( strncmp(D_GetClient_lock_user_id(body->id), body->user_id, USER_ID_LEN+1) 
									|| strncmp(D_GetClient_ip(body->id), body->user_ip, SIZEOF_IP+1)  )
	{
		U_Printf("[PROC]Discord billing_fd(%d) BW_GAMESTART_SUCC fd(%d) lock_user_id(%s) user_id(%s) user_ip(%s)\n",
						billing_fd, body->id, D_GetClient_lock_user_id(body->id), body->user_id, body->user_ip);
		//send WB_GAMESTOP
		U_Printf("[PROC]send_queue_in UNICAST(%d) WB_GAMESTOP  user_id(%s) user_ip(%s)\n", billing_fd, body->user_id, body->user_ip);
		P_send_size	= MK_GetPacket_WB_GAMESTOP(P_send_buf, body->id, body->user_id, body->user_ip);
		U_Send_queue_in(SEND_UNICAST, billing_fd, 0, 0, P_send_size, P_send_buf);
		return;
	}
	
	//check already login
	if( *D_GetClient_user_id(body->id) )
	{
		U_Printf("[PROC]Error billing_fd:%d BW_GAMESTART already login user_id(%s) body->user_id(%s)\n",
				billing_fd, D_GetClient_user_id(body->id), body->user_id);
		P_remove_client(billing_fd);
		return;
	}

	switch( body->result )
	{
		case	0:
			body->result	= RESULT_SUCC;
			break;
		case	1:
			body->result	= RESULT_FAIL_PAY_INFO;
			break;
		case	2:
			body->result	= RESULT_FAIL_LOGIN;
			break;
		case	3:
			body->result	= RESULT_FAIL_BLOCK_USERID;
			break;
		case	4:
			body->result	= RESULT_FAIL_SQL;
			break;
		case	5:
			body->result	= RESULT_FAIL_OTHER_GAME;
			break;
		default	:
			body->result	= RESULT_FAIL_BILLING;
	}

	//send LC_LOGIN_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) LC_LOGIN_RESULT result_code(%d) user_id(%s)\n", body->id, body->result, body->user_id);
	P_send_size	= MK_GetPacket_LC_LOGIN_RESULT(P_send_buf, body->result);
	U_Send_queue_in(SEND_UNICAST, body->id, 0, 0, P_send_size, P_send_buf);

	if( body->result )
	{
		//remove client	
		P_remove_client(body->id);	
	}
	else
	{
		//set G_Login_number
		++G_Login_number;

		//set user_id
		D_SetClient_user_id(body->id, body->user_id);

		U_Printf("[PROC]Login success fd(%d) user_id(%s) user_ip(%s) G_Login_number(%d) G_Connection_number(%d)\n",
								body->id, body->user_id, body->user_ip, G_Login_number, G_Connection_number);
	}
}


static	void	P_proc_BW_GAMESTART_FAIL(const int billing_fd)
{
	struct	body_BW_GAMESTART_FAIL	*body	= (struct body_BW_GAMESTART_FAIL *)(P_recv_q->data);

	
	if( D_GetClient_sec_active(billing_fd) == 0 )	
		return;

	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_BW_GAMESTART_FAIL) )
	{
		U_Printf("[PROC]Error billing_fd(%d) BW_GAMESTART_FAIL(%d) bad body_size(%d)\n",
				billing_fd, sizeof(struct body_BW_GAMESTART_FAIL), P_recv_q->data_size);
		P_remove_client(billing_fd);
		return;
	}

	//check id
	if( (body->id < 1) || (body->id > MAX_FD) )
	{
		U_Printf("[PROC]Error billing_fd(%d) BW_GAMESTART_FAIL bad id(%d)\n", billing_fd, body->id);
		P_remove_client(billing_fd);
		return;
	}

	//check user_id	
	if( (body->user_id[0] == 0) || (body->user_id[USER_ID_LEN] != 0) )
	{
		body->user_id[USER_ID_LEN]	= 0;
		U_Printf("[PROC]Error billing_fd(%d) BW_GAMESTART_FAIL bad user_id(%s)\n", billing_fd, body->user_id);
		P_remove_client(billing_fd);
		return;
	}
	
	//check session
	if( strncmp(D_GetClient_lock_user_id(body->id), body->user_id, 16) )
	{
		U_Printf("[PROC]Discord billing_fd(%d) BW_GAMESTART_SUCC fd(%d) lock_user_id(%s) user_id(%s)\n",
							billing_fd, body->id, D_GetClient_lock_user_id(body->id), body->user_id);
		return;
	}
	
	//check already login
	if( *D_GetClient_user_id(body->id) )
	{
		U_Printf("[PROC]Error billing_fd:%d BW_GAMESTOP already login user_id(%s) body->user_id(%s)\n",
							billing_fd, D_GetClient_user_id(body->id), body->user_id);
		P_remove_client(billing_fd);
		return;
	}

	//send LC_LOGIN_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) LC_LOGIN_RESULT RESULT_FAIL_BILLING user_id(%s)\n", body->id, body->user_id);
	P_send_size	= MK_GetPacket_LC_LOGIN_RESULT(P_send_buf, RESULT_FAIL_BILLING);
	U_Send_queue_in(SEND_UNICAST, body->id, 0, 0, P_send_size, P_send_buf);
	
	//remove client	
	P_remove_client(body->id);	
}


static	void	P_proc_BW_GAMESTOP_SUCC(const int billing_fd)
{
	if( D_GetClient_sec_active(billing_fd) == 0 )
		return;
}


static	void	P_proc_BW_GAMESTOP_FAIL(const int billing_fd)
{
	if( D_GetClient_sec_active(billing_fd) == 0 )
		return;
}


static	void	P_proc_BW_USER_CHECK(const int billing_fd)
{
	struct	body_BW_USER_CHECK	*body	= (struct body_BW_USER_CHECK *)(P_recv_q->data);
	

	if( D_GetClient_sec_active(billing_fd) == 0 )
		return;

	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_BW_USER_CHECK) )
	{
		U_Printf("[PROC]Error billing_fd(%d) BW_USER_CHECK(%d) bad body_size(%d)\n",
				billing_fd, sizeof(struct body_BW_USER_CHECK), P_recv_q->data_size);
		P_remove_client(billing_fd);
		return;
	}

	//check id
	if( (body->id < 1) || (body->id > MAX_FD) )
	{
		U_Printf("[PROC]Error billing_fd(%d) BW_USER_CHECK bad id(%d)\n", billing_fd, body->id);
		P_remove_client(billing_fd);
		return;
	}

	//check user_id	
	if( (body->user_id[0] == 0) || (body->user_id[USER_ID_LEN] != 0) )
	{
		body->user_id[USER_ID_LEN]	= 0;
		U_Printf("[PROC]Error billing_fd(%d) BW_USER_CHECK bad user_id(%s)\n", billing_fd, body->user_id);
		P_remove_client(billing_fd);
		return;
	}
	
	//check session
	if( strncmp(D_GetClient_user_id(body->id), body->user_id, USER_ID_LEN+1) )
	{
		//send WB_USER_CHECK_FAIL
		U_Printf("[PROC]send_queue_in UNICAST(%d) WB_USER_CHECK_FAIL  user_id(%s)\n", billing_fd, body->user_id);
		P_send_size	= MK_GetPacket_WB_USER_CHECK_FAIL(P_send_buf, body->id, body->user_id);
		U_Send_queue_in(SEND_UNICAST, billing_fd, 0, 0, P_send_size, P_send_buf);
		return;
	}
	else
	{
		//send WB_USER_CHECK_SUCC
		U_Printf("[PROC]send_queue_in UNICAST(%d) WB_USER_CHECK_SUCC  user_id(%s)\n", billing_fd, body->user_id);
		P_send_size	= MK_GetPacket_WB_USER_CHECK_SUCC(P_send_buf, body->id, body->user_id);
		U_Send_queue_in(SEND_UNICAST, billing_fd, 0, 0, P_send_size, P_send_buf);
		return;
	}
}


static	void	P_proc_BW_DATABASE_ERROR(const int billing_fd)
{
	struct	body_BW_DATABASE_ERROR	*body	= (struct body_BW_DATABASE_ERROR *)(P_recv_q->data);
	

	if( D_GetClient_sec_active(billing_fd) == 0 )
		return;

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_BW_DATABASE_ERROR) )
	{
		U_Printf("[PROC]Error billing_fd(%d) BW_DATABASE_ERROR(%d) bad body_size(%d)\n",
				billing_fd, sizeof(struct body_BW_DATABASE_ERROR), P_recv_q->data_size);
		P_remove_client(billing_fd);
		return;
	}

	U_Printf("[PROC]billing_fd(%d) BW_DATABASE_ERROR(%d)\n", billing_fd, body->id);
}


static	void	P_proc_BW_SYSTEM_ERROR(const int billing_fd)
{
	struct	body_BW_SYSTEM_ERROR	*body	= (struct body_BW_SYSTEM_ERROR *)(P_recv_q->data);
	

	if( D_GetClient_sec_active(billing_fd) == 0 )
		return;

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_BW_SYSTEM_ERROR) )
	{
		U_Printf("[PROC]Error billing_fd(%d) BW_SYSTEM_ERROR(%d) bad body_size(%d)\n",
				billing_fd, sizeof(struct body_BW_SYSTEM_ERROR), P_recv_q->data_size);
		P_remove_client(billing_fd);
		return;
	}

	U_Printf("[PROC]billing_fd(%d) BW_SYSTEM_ERROR(%d)\n", billing_fd, body->id);
}


static	void	P_proc_BW_REGSERVER_SUCC(const int billing_fd)
{
	if( D_GetClient_sec_active(billing_fd) == 0 )
		return;

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_BW_REGSERVER_SUCC) )
	{
		U_Printf("[PROC]Error billing_fd(%d) BW_REGSERVER_SUCC(%d) bad body_size(%d)\n",
				billing_fd, sizeof(struct body_BW_REGSERVER_SUCC), P_recv_q->data_size);
		P_remove_client(billing_fd);
		return;
	}

	P_billing_fd[D_GetClient_billing_idx(billing_fd)]	= billing_fd;
	U_Printf("[PROC]billing_fd(%d) BW_REGSERVER_SUCC\n", billing_fd);
}


static	void	P_proc_BW_REGSERVER_FAIL(const int billing_fd)
{
	if( D_GetClient_sec_active(billing_fd) == 0 )
		return;

	U_Printf("[PROC]billing_fd(%d) BW_REGSERVER_FAIL\n", billing_fd);
	P_remove_client(billing_fd);
}


static	void	P_proc_BW_ALIVECHECK(const int billing_fd)
{
	if( D_GetClient_sec_active(billing_fd) == 0 )
		return;
}
*/

//______________________________________________________________
//AW
static	void	P_proc_AW_INIT_RESULT(const int dbagent_fd)
{
	struct	body_AW_INIT_RESULT	*body	= (struct body_AW_INIT_RESULT *)(P_recv_q->data);


	if( D_GetClient_sec_active(dbagent_fd) == 0 )
		return;

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_AW_INIT_RESULT) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_INIT_RESULT(%d) bad body_size(%d)\n"
			, dbagent_fd, sizeof(struct body_AW_INIT_RESULT), P_recv_q->data_size);
		P_remove_client(dbagent_fd);
		return;
	}

	//check result_code
	if( body->result_code )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_INIT_RESULT result_code(%d)\n", dbagent_fd, body->result_code);
		P_remove_client(dbagent_fd);
		return;
	}

	if( D_GetClient_dbagent_idx(dbagent_fd) )
	{
		P_dbagent_fd[D_GetClient_dbagent_idx(dbagent_fd)]		= dbagent_fd;
		U_Printf("[PROC]dbagent_fd(%d) AW_INIT_RESULT success\n", dbagent_fd);
	}
	else
	{
		P_login_dbagent_fd[D_GetClient_login_dbagent_idx(dbagent_fd)]	= dbagent_fd;
		U_Printf("[PROC]login_dbagent_fd(%d) AW_INIT_RESULT success\n", dbagent_fd);
	}
}


static	void	P_proc_AW_LIVE(const int dbagent_fd)
{
	if( D_GetClient_sec_active(dbagent_fd) == 0 )
		return;

	P_send_size	= MK_GetPacket_WA_LIVE(P_send_buf);
	U_Send_queue_in(SEND_UNICAST, dbagent_fd, 0, 0, P_send_size, P_send_buf);
}

				
static	void	P_proc_AW_CHAR_LIST_RESULT(const int dbagent_fd)
{
	int					i;
	struct	body1_AW_CHAR_LIST_RESULT	*body1	= (struct body1_AW_CHAR_LIST_RESULT *)(P_recv_q->data);
	struct	body2_AW_CHAR_LIST_RESULT	*body2
				= (struct body2_AW_CHAR_LIST_RESULT *)(P_recv_q->data + sizeof(struct body1_AW_CHAR_LIST_RESULT));


	if( D_GetClient_sec_active(dbagent_fd) == 0 )
		return;
	
	//check body1_size 
	if( P_recv_q->data_size < sizeof(struct body1_AW_CHAR_LIST_RESULT) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_LIST_RESULT(%d) bad body_size(%d)\n",
					dbagent_fd, sizeof(struct body1_AW_CHAR_LIST_RESULT), P_recv_q->data_size);
		P_remove_client(dbagent_fd);
		return;
	}

	//check char_cnt
	if( (body1->char_cnt < 0) || (body1->char_cnt > MAX_CHAR_CNT) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_LIST_RESULT bad char_cnt(%d)\n", dbagent_fd, body1->char_cnt);
		P_remove_client(dbagent_fd);
		return;
	}

	//check body2_size 
	if( P_recv_q->data_size != sizeof(struct body1_AW_CHAR_LIST_RESULT) + (body1->char_cnt)*sizeof(struct body2_AW_CHAR_LIST_RESULT) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_LIST_RESULT(%d) bad body_size(%d)\n", dbagent_fd,
			sizeof(struct body1_AW_CHAR_LIST_RESULT)+(body1->char_cnt)*sizeof(struct body2_AW_CHAR_LIST_RESULT), P_recv_q->data_size);
		P_remove_client(dbagent_fd);
		return;
	}

	//check fd
	if( (P_recv_q->fd_or_seq < 1) || (P_recv_q->fd_or_seq > MAX_FD) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_LIST_RESULT bad fd_or_seq(%d)\n", dbagent_fd, P_recv_q->fd_or_seq);
		P_remove_client(dbagent_fd);
		return;
	}

	//check user_id
	if( (body1->user_id[0] == 0) || (body1->user_id[USER_ID_LEN] != 0) )
	{
		body1->user_id[USER_ID_LEN]	= 0;
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_LIST_RESULT bad user_id(%s)\n", dbagent_fd, body1->user_id);
		P_remove_client(dbagent_fd);
		return;
	}

	//check session
	if( (P_recv_q->sec_active != D_GetClient_sec_active(P_recv_q->fd_or_seq)) || (P_recv_q->session != D_GetClient_session(P_recv_q->fd_or_seq))
		|| strncmp(body1->user_id, D_GetClient_user_id(P_recv_q->fd_or_seq), USER_ID_LEN+1) )
	{
		U_Printf("[PROC]Discord dbagent_fd(%d) AW_CHAR_LIST_RESULT fd_or_seq(%d) sec_active(%d) session(%d) user_id(%s)\n",
				dbagent_fd, P_recv_q->fd_or_seq, P_recv_q->sec_active, P_recv_q->session, body1->user_id);
		return;
	}

	//check char info
	for( i=0; i<body1->char_cnt; i++ )
	{
		//char name
		if( (body2->char_name[0] == 0) || (body2->char_name[CHAR_NAME_LEN] != 0) )
		{
			body2->char_name[CHAR_NAME_LEN]	= 0;
			U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_LIST_RESULT char_cnt(%d) bad char_name(%s)\n",
										dbagent_fd, i, body2->char_name);
			P_remove_client(dbagent_fd);
			return;
		}
	
		//sex, hair, face, level
		if( (body2->sex < 0) || (body2->sex > MAX_CHAR_SEX) 
			|| (body2->hair < 0) || (body2->hair > MAX_CHAR_HAIR)
			|| (body2->face < 0) || (body2->face > MAX_CHAR_FACE) 
			|| (body2->level < 1) || (body2->level > D_GetMax_level()) )  
		{
			U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_LIST_RESULT char_cnt(%d) char_name(%s) bad sex(%d) hair(%d) face(%d) level(%d)\n",
								dbagent_fd, i, body2->char_name, body2->sex, body2->hair, body2->face, body2->level);
			body1->result_code	= RESULT_FAIL_DB;
		}

		++body2;	
	}

	//set load_world_idx, load_char_cnt	
	if( body1->result_code == RESULT_SUCC )
	{
		D_SetClient_load_world_idx(P_recv_q->fd_or_seq, D_GetClient_dbagent_idx(dbagent_fd));
		D_SetClient_load_char_cnt(P_recv_q->fd_or_seq, body1->char_cnt);
	}

	//send LC_CHAR_LIST_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) LC_CHAR_LIST_RESULT result_code(%d) user_id(%s) char_cnt(%d)\n",
						P_recv_q->fd_or_seq, body1->result_code, body1->user_id, body1->char_cnt);
	P_send_size	= MK_GetPacket_LC_CHAR_LIST_RESULT(P_send_buf, body1->result_code, body1->char_cnt,
						(struct body2_AW_CHAR_LIST_RESULT *)(P_recv_q->data + sizeof(struct body1_AW_CHAR_LIST_RESULT)));
	U_Send_queue_in(SEND_UNICAST, P_recv_q->fd_or_seq, 0, 0, P_send_size, P_send_buf);
}


static	void	P_proc_AW_CHAR_CREATE_RESULT(const int dbagent_fd)
{
	struct	body_AW_CHAR_CREATE_RESULT	*body	= (struct body_AW_CHAR_CREATE_RESULT *)(P_recv_q->data);
	

	if( D_GetClient_sec_active(dbagent_fd) == 0 )
		return;

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_AW_CHAR_CREATE_RESULT) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_CREATE_RESULT(%d) bad body_size(%d)\n", dbagent_fd,
			sizeof(struct body_AW_CHAR_CREATE_RESULT), P_recv_q->data_size);
		P_remove_client(dbagent_fd);
		return;
	}

	//check fd
	if( (P_recv_q->fd_or_seq < 1) || (P_recv_q->fd_or_seq > MAX_FD) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_CREATE_RESULT bad fd_or_seq(%d)\n", dbagent_fd, P_recv_q->fd_or_seq);
		P_remove_client(dbagent_fd);
		return;
	}

	//check user_id
	if( (body->user_id[0] == 0) || (body->user_id[USER_ID_LEN] != 0) )
	{
		body->user_id[USER_ID_LEN]	= 0;
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_CREATE_RESULT bad user_id(%s)\n", dbagent_fd, body->user_id);
		P_remove_client(dbagent_fd);
		return;
	}
	
	//check char_name
	if( (body->char_name[0] == 0) || (body->char_name[CHAR_NAME_LEN] != 0) )
	{
		body->char_name[CHAR_NAME_LEN]	= 0;
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_CREATE_RESULT bad char_name(%s)\n", dbagent_fd, body->char_name);
		P_remove_client(dbagent_fd);
		return;
	}

	//check session
	if( (P_recv_q->sec_active != D_GetClient_sec_active(P_recv_q->fd_or_seq)) || (P_recv_q->session != D_GetClient_session(P_recv_q->fd_or_seq))
		|| strncmp(body->user_id, D_GetClient_user_id(P_recv_q->fd_or_seq), USER_ID_LEN+1) )
	{
		U_Printf("[PROC]Discord dbagent_fd(%d) AW_CHAR_CREATE_RESULT fd_or_seq(%d) sec_active(%d) session(%d), user_id(%s), char_name(%s)\n",
				dbagent_fd, P_recv_q->fd_or_seq, P_recv_q->sec_active, P_recv_q->session, body->user_id, body->char_name);
		return;
	}

	//check world_idx
	if( D_GetClient_dbagent_idx(dbagent_fd) != D_GetClient_load_world_idx(P_recv_q->fd_or_seq) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_CREATE_RESULT bad world_idx(%d) load_world_idx(%d)\n",
			dbagent_fd, D_GetClient_dbagent_idx(dbagent_fd), D_GetClient_load_world_idx(P_recv_q->fd_or_seq));
		P_remove_client(dbagent_fd);
		return;
	}

	//set char_load_cnt
	if( body->result_code == RESULT_SUCC )	
		D_SetClient_load_char_cnt(P_recv_q->fd_or_seq, D_GetClient_load_char_cnt(P_recv_q->fd_or_seq)+1);

	//send LC_CHAR_CREATE_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) LC_CHAR_CREATE_RESULT result_code(%d) user_id(%s) char_name(%s)\n",
			P_recv_q->fd_or_seq, body->result_code, body->user_id, body->char_name);
	P_send_size	= MK_GetPacket_LC_CHAR_CREATE_RESULT(P_send_buf, body->result_code, body->char_name);
	U_Send_queue_in(SEND_UNICAST, P_recv_q->fd_or_seq, 0, 0, P_send_size, P_send_buf);
}


static	void	P_proc_AW_CHAR_DELETE_RESULT(const int dbagent_fd)
{
	struct	body_AW_CHAR_DELETE_RESULT	*body	= (struct body_AW_CHAR_DELETE_RESULT *)(P_recv_q->data);
	

	if( D_GetClient_sec_active(dbagent_fd) == 0 )
		return;

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_AW_CHAR_DELETE_RESULT) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_DELETE_RESULT(%d) bad body_size(%d)\n", dbagent_fd,
			sizeof(struct body_AW_CHAR_DELETE_RESULT), P_recv_q->data_size);
		P_remove_client(dbagent_fd);
		return;
	}

	//check fd
	if( (P_recv_q->fd_or_seq < 1) || (P_recv_q->fd_or_seq > MAX_FD) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_DELETE_RESULT bad fd_or_seq(%d)\n", dbagent_fd, P_recv_q->fd_or_seq);
		P_remove_client(dbagent_fd);
		return;
	}

	//check user_id
	if( (body->user_id[0] == 0) || (body->user_id[USER_ID_LEN] != 0) )
	{
		body->user_id[USER_ID_LEN]	= 0;
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_DELETE_RESULT bad user_id(%s)\n", dbagent_fd, body->user_id);
		P_remove_client(dbagent_fd);
		return;
	}
	
	//check char_name
	if( (body->char_name[0] == 0) || (body->char_name[CHAR_NAME_LEN] != 0) )
	{
		body->char_name[CHAR_NAME_LEN]	= 0;
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_DELETE_RESULT bad char_name(%s)\n", dbagent_fd, body->char_name);
		P_remove_client(dbagent_fd);
		return;
	}

	//check session
	if( (P_recv_q->sec_active != D_GetClient_sec_active(P_recv_q->fd_or_seq)) || (P_recv_q->session != D_GetClient_session(P_recv_q->fd_or_seq))
		|| strncmp(body->user_id, D_GetClient_user_id(P_recv_q->fd_or_seq), USER_ID_LEN+1) )
	{
		U_Printf("[PROC]Discord dbagent_fd(%d) AW_CHAR_DELETE_RESULT fd_or_seq(%d) sec_active(%d) session(%d), user_id(%s), char_name(%s)\n",
				dbagent_fd, P_recv_q->fd_or_seq, P_recv_q->sec_active, P_recv_q->session, body->user_id, body->char_name);
		return;
	}
	
	//check world_idx
	if( D_GetClient_dbagent_idx(dbagent_fd) != D_GetClient_load_world_idx(P_recv_q->fd_or_seq) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_CHAR_DELETE_RESULT bad world_idx(%d) load_world_idx(%d)\n",
			dbagent_fd, D_GetClient_dbagent_idx(dbagent_fd), D_GetClient_load_world_idx(P_recv_q->fd_or_seq));
		P_remove_client(dbagent_fd);
		return;
	}

	//set char_load_cnt
	if( body->result_code == RESULT_SUCC )	
		D_SetClient_load_char_cnt(P_recv_q->fd_or_seq, D_GetClient_load_char_cnt(P_recv_q->fd_or_seq)-1);
	
	
	//send LC_CHAR_DELETE_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) LC_CHAR_DELETE_RESULT result_code(%d) user_id(%s) char_name(%s)\n",
			P_recv_q->fd_or_seq, body->result_code, body->user_id, body->char_name);
	P_send_size	= MK_GetPacket_LC_CHAR_DELETE_RESULT(P_send_buf, body->result_code, body->char_name);
	U_Send_queue_in(SEND_UNICAST, P_recv_q->fd_or_seq, 0, 0, P_send_size, P_send_buf);
}


static	void	P_proc_AW_LOGIN_RESULT(const int dbagent_fd)
{
	struct	body_AW_LOGIN_RESULT	*body	= (struct body_AW_LOGIN_RESULT *)(P_recv_q->data);


	if( D_GetClient_sec_active(dbagent_fd) == 0 )
		return;

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_AW_LOGIN_RESULT) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_LOGIN_RESULT(%d) bad body_size(%d)\n",
					dbagent_fd, sizeof(struct body_AW_LOGIN_RESULT), P_recv_q->data_size);
		P_remove_client(dbagent_fd);
		return;
	}

	//check fd_or_seq
	if( (P_recv_q->fd_or_seq < 1) || (P_recv_q->fd_or_seq > MAX_FD) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_LOGIN_RESULT bad fd_or_seq(%d)\n", dbagent_fd, P_recv_q->fd_or_seq);
		P_remove_client(dbagent_fd);
		return;
	}

	//check user_id
	if( (body->user_id[0] == 0) || (body->user_id[USER_ID_LEN] != 0) )
	{
		body->user_id[USER_ID_LEN]	= 0;
		U_Printf("[PROC]Error dbagent_fd(%d) AW_LOGIN_RESULT bad user_id(%s)\n", dbagent_fd, body->user_id);
		P_remove_client(dbagent_fd);
		return;
	}

	//check session
	if( (P_recv_q->sec_active != D_GetClient_sec_active(P_recv_q->fd_or_seq))
				|| (P_recv_q->session != D_GetClient_session(P_recv_q->fd_or_seq)) )
	{
		U_Printf("[PROC]Discord dbagent_fd(%d) AW_LOGIN_RESULT fd_or_seq(%d) sec_active(%d) session(%d) user_id(%s)\n",
								dbagent_fd, P_recv_q->fd_or_seq, P_recv_q->session, body->user_id);
		//send GA_LOGOUT
		U_Printf("[PROC]send_queue_in UNICAST(%d) WA_LOGOUT user_id(%s)\n", P_recv_q->fd_or_seq, body->user_id);
		P_send_size	= MK_GetPacket_WA_LOGOUT(P_send_buf, P_recv_q->fd_or_seq, P_recv_q->sec_active, P_recv_q->session, body->user_id);
		U_Send_queue_in(SEND_UNICAST, dbagent_fd, 0, 0, P_send_size, P_send_buf);
		return;		
	}
	
	//check already login
	if( *D_GetClient_user_id(P_recv_q->fd_or_seq) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_LOGIN_RESULT already login user_id(%s)(%s)\n",
							dbagent_fd, D_GetClient_user_id(P_recv_q->fd_or_seq), body->user_id);
		P_remove_client(dbagent_fd);
		return;
	}



	if( body->result_code )
	{
		//send LC_LOGIN_RESULT
		U_Printf("[PROC]send_queue_in UNICAST(%d) LC_LOGIN_RESULT result_code(%d) user_id(%s)\n",
								P_recv_q->fd_or_seq, body->result_code, body->user_id);
		P_send_size	= MK_GetPacket_LC_LOGIN_RESULT(P_send_buf, body->result_code);
		U_Send_queue_in(SEND_UNICAST, P_recv_q->fd_or_seq, 0, 0, P_send_size, P_send_buf);
	
		//remove client	
		P_remove_client(P_recv_q->fd_or_seq);	
	}
	else
	{
		//set G_Login_number
		++G_Login_number;

		//set user_id
		D_SetClient_user_id(P_recv_q->fd_or_seq, body->user_id);

		U_Printf("[PROC]Login success fd(%d) user_id(%s) user_ip(%s) G_Login_number(%d) G_Connection_number(%d)\n",
				P_recv_q->fd_or_seq, body->user_id, D_GetClient_ip(P_recv_q->fd_or_seq), G_Login_number, G_Connection_number);
		
		//send LC_LOGIN_RESULT
		U_Printf("[PROC]send_queue_in UNICAST(%d) LC_LOGIN_RESULT result_code(%d) user_id(%s)\n",
								P_recv_q->fd_or_seq, body->result_code, body->user_id);
		P_send_size	= MK_GetPacket_LC_LOGIN_RESULT(P_send_buf, body->result_code);
		U_Send_queue_in(SEND_LOGIN, P_recv_q->fd_or_seq, 0, 0, P_send_size, P_send_buf);
	}
}


static	void	P_proc_AW_GET_USER_RESULT(const int dbagent_fd)
{
	struct	body_AW_GET_USER_RESULT		*body	= (struct body_AW_GET_USER_RESULT *)(P_recv_q->data);


	if( D_GetClient_sec_active(dbagent_fd) == 0 )
		return;

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_AW_GET_USER_RESULT) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_GET_USER_RESULT(%d) bad body_size(%d)\n",
					dbagent_fd, sizeof(struct body_AW_GET_USER_RESULT), P_recv_q->data_size);
		P_remove_client(dbagent_fd);
		return;
	}

	//check msvr_code
	if( body->msvr_code != 1 )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_GET_USER_RESULT bad msvr_code(%d)\n", dbagent_fd, body->msvr_code);
		P_remove_client(dbagent_fd);
		return;
	}
	
	//check ssvr_code
	if( (body->ssvr_code < 1) || (body->ssvr_code > D_GetWorld_channel_cnt(1)) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_GET_USER_RESULT bad ssvr_code(%d)\n", dbagent_fd, body->ssvr_code);
		P_remove_client(dbagent_fd);
		return;
	}

	//check user
	if( body->user < 0 )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_GET_USER_RESULT bad user(%d)\n", dbagent_fd, body->user);
		P_remove_client(dbagent_fd);
		return;
	}

	//set channel user
	U_Printf("[PROC]dbagent_fd(%d) AW_GET_USER_RESULT msvr_code(%d) ssvr_code(%d) user(%d)\n",
							dbagent_fd, body->msvr_code, body->ssvr_code, body->user);

	if( (body->user > 0) && (body->user < 50) )
			body->user	= 50;
	D_SetWorld_channel_user(body->msvr_code, body->ssvr_code, body->user);
}



//______________________________________________________________
//CL
static	void	P_proc_CL_LOGIN(const int fd)
{
	int			use_login_dbagent_fd;
	struct	body_CL_LOGIN	*body	= (struct body_CL_LOGIN *)(P_recv_q->data);


	if( D_GetClient_sec_active(fd) == 0 )
		return;
	
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CL_LOGIN) )
	{
		U_Printf("[PROC]Hack fd(%d) CL_LOGIN(%d) bad body_size(%d)\n", fd, sizeof(struct body_CL_LOGIN), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}

	//check first packet
	if( P_recv_q->recv_cnt != 1 )
	{
		U_Printf("[PROC]Hack fd(%d) CL_LOGIN bad recv_cnt(%d)\n", fd, P_recv_q->recv_cnt);
		P_remove_client(fd);
		return;
	}

	//check version
	if( (body->version[0] == 0) || (body->version[VERSION_LEN] != 0) || (strncmp(body->version, VERSION, VERSION_LEN+1)) )
	{
		body->version[VERSION_LEN]	= 0;
		U_Printf("[PROC]Error fd(%d) CL_LOGIN bad version(%s)\n", fd, body->version);
		P_remove_client(fd);
		return;
	}
	
	//check user_id
	if( (body->user_id[0] == 0) || (body->user_id[USER_ID_LEN] != 0) )
	{
		body->user_id[USER_ID_LEN]	= 0;
		U_Printf("[PROC]Error fd(%d) CL_LOGIN bad user_id(%s)\n", fd, body->user_id);
		P_remove_client(fd);
		return;
	}

	//check encodedstr
	if( (body->encodedstr[0] == 0) || (body->encodedstr[ENCODEDSTR_LEN] != 0) )
	{
		body->encodedstr[ENCODEDSTR_LEN]	= 0;
		U_Printf("[PROC]Error fd(%d) CL_LOGIN bad user_id(%s) encodedstr(%s)\n", fd, body->user_id, body->encodedstr);
		P_remove_client(fd);
		return;
	}


	/*
	use_billing_fd	= P_billing_fd[D_GetClient_use_billing_idx(fd)];
	
	if( use_billing_fd )
	{
		//set lock_user_id
		D_SetClient_lock_user_id(fd, body->user_id);		

		//send WB_GAMESTART
		U_Printf("[PROC]send_queue_in UNICAST(%d) WB_GAMESTART user_id(%s)\n", use_billing_fd, body->user_id);
		P_send_size	= MK_GetPacket_WB_GAMESTART(P_send_buf, fd, body->user_id);
		U_Send_queue_in(SEND_UNICAST, use_billing_fd, 0, 0, P_send_size, P_send_buf);
	}
	else
	{
		//send LC_LOGIN_RESULT
		U_Printf("[PROC]send_queue_in UNICAST(%d) LC_LOGIN_RESULT RESULT_FAILE_BILLING user_id(%s)\n", fd, body->user_id);
		P_send_size	= MK_GetPacket_LC_LOGIN_RESULT(P_send_buf, RESULT_FAIL_BILLING);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, P_send_size, P_send_buf);

		//remove client
		P_remove_client(fd);
	}
	*/

	use_login_dbagent_fd	= P_login_dbagent_fd[D_GetClient_use_login_dbagent_idx(fd)];

	if( use_login_dbagent_fd )
	{
		//send WA_LOGIN
		U_Printf("[PROC]send_queue_in UNICAST(%d) WA_LOGIN user_id(%s)\n", use_login_dbagent_fd, body->user_id);
		P_send_size	= MK_GetPacket_WA_LOGIN(P_send_buf, fd, body->user_id, body->encodedstr);
		U_Send_queue_in(SEND_UNICAST, use_login_dbagent_fd, 0, 0, P_send_size, P_send_buf);
	}
	else
	{
		//send LC_LOGIN_RESULT
		U_Printf("[PROC]send_queue_in UNICAST(%d) LC_LOGIN_RESULT result_code(%d) user_id(%s)\n", fd, RESULT_FAIL_DBAGENT, body->user_id);
		P_send_size	= MK_GetPacket_LC_LOGIN_RESULT(P_send_buf, RESULT_FAIL_DBAGENT);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, P_send_size, P_send_buf);

		//remove client
		P_remove_client(fd);
	}
	
	
	/*	
	//set G_Login_number
	++G_Login_number;

	//set user_id
	D_SetClient_user_id(fd, body->user_id);
		
	//send LC_LOGIN_RESULT
	U_Printf("[PROC]send_queue_in LOGIN(%d) LC_LOGIN_RESULT RESULT_SUCC user_id(%s)\n", fd, body->user_id);
	P_send_size	= MK_GetPacket_LC_LOGIN_RESULT(P_send_buf, RESULT_SUCC);
	U_Send_queue_in(SEND_LOGIN, fd, 0, 0, P_send_size, P_send_buf);
	*/
}


static	void	P_proc_CL_LIVE(const int fd)
{
	if( D_GetClient_sec_active(fd) == 0 )
		return;

	P_send_size	= MK_GetPacket_LC_LIVE(P_send_buf);
	U_Send_queue_in(SEND_UNICAST, fd, 0, 0, P_send_size, P_send_buf);
}


static	void	P_proc_CL_WORLD_LIST(const int fd)
{
	if( D_GetClient_sec_active(fd) == 0 )
		return;

	//check login
	if( *D_GetClient_user_id(fd) == 0 )
	{
		U_Printf("[PROC]Hack fd(%d) CL_WORLD_LIST not login\n", fd);
		P_remove_client(fd);
		return;
	}
		
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CL_WORLD_LIST) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) CL_WORLD_LIST(%d) bad body_size(%d)\n",
			fd, D_GetClient_user_id(fd), sizeof(struct body_CL_WORLD_LIST), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}

	//send LC_WORLD_LIST_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) LC_WORLD_LIST_RESULT user_id(%s)\n", fd, D_GetClient_user_id(fd));
	P_send_size	= MK_GetPacket_LC_WORLD_LIST_RESULT(P_send_buf, RESULT_SUCC);
	U_Send_queue_in(SEND_UNICAST, fd, 0, 0, P_send_size, P_send_buf);
}


static	void	P_proc_CL_CHANNEL_LIST(const int fd)
{
	struct	body_CL_CHANNEL_LIST	*body	= (struct body_CL_CHANNEL_LIST *)(P_recv_q->data);
	
	
	if( D_GetClient_sec_active(fd) == 0 )
		return;

	//check login
	if( *D_GetClient_user_id(fd) == 0 )
	{
		U_Printf("[PROC]Hack fd(%d) CL_CHANNEL_LIST not login\n", fd);
		P_remove_client(fd);
		return;
	}
		
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CL_CHANNEL_LIST) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) CL_CHANNEL_LIST(%d) bad body_size(%d)\n",
			fd, D_GetClient_user_id(fd), sizeof(struct body_CL_CHANNEL_LIST), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}

	//check world_idx
	if( (body->world_idx < 1) || (body->world_idx > D_GetMax_world()) ) 
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) CL_CHANNEL_LIST bad world_idx(%d)\n", fd, D_GetClient_user_id(fd), body->world_idx);
		P_remove_client(fd);
		return;
	}

	//send LC_CHANNEL_LIST_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) LC_CHANNEL_LIST_RESULT user_id(%s)\n", fd, D_GetClient_user_id(fd));
	P_send_size	= MK_GetPacket_LC_CHANNEL_LIST_RESULT(P_send_buf, RESULT_SUCC, body->world_idx);
	U_Send_queue_in(SEND_UNICAST, fd, 0, 0, P_send_size, P_send_buf);
}


static	void	P_proc_CL_CHAR_LIST(const int fd)
{
	struct	body_CL_CHAR_LIST	*body	= (struct body_CL_CHAR_LIST *)(P_recv_q->data);
	
	
	if( D_GetClient_sec_active(fd) == 0 )
		return;

	//check login
	if( *D_GetClient_user_id(fd) == 0 )
	{
		U_Printf("[PROC]Hack fd(%d) CL_CHAR_LIST not login\n", fd);
		P_remove_client(fd);
		return;
	}

	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CL_CHAR_LIST) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) CL_CHAR_LIST(%d) bad body_size(%d)\n",
			fd, D_GetClient_user_id(fd), sizeof(struct body_CL_CHAR_LIST), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}
	
	//check world_idx
	if( (body->world_idx < 1) || (body->world_idx > D_GetMax_world()) ) 
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) CL_CHAR_LIST bad world_idx(%d)\n", fd, D_GetClient_user_id(fd), body->world_idx);
		P_remove_client(fd);
		return;
	}

	if( P_dbagent_fd[body->world_idx] )
	{
		//send WA_CHAR_LIST
		U_Printf("[PROC]send_queue_in UNICAST(%d) WA_CHAR_LIST user_id(%s)\n", P_dbagent_fd[body->world_idx], D_GetClient_user_id(fd));
		P_send_size	= MK_GetPacket_WA_CHAR_LIST(P_send_buf, fd);
		U_Send_queue_in(SEND_UNICAST, P_dbagent_fd[body->world_idx], 0, 0, P_send_size, P_send_buf);
	}
	else
	{
		//send LC_CHAR_LIST_RESULT
		U_Printf("[PROC]send_queue_in UNICAST(%d) LC_CHAR_LIST_RESULT RESULT_FAIL_DBAGENT user_id(%s)\n", fd, D_GetClient_user_id(fd));
		P_send_size	= MK_GetPacket_LC_CHAR_LIST_RESULT(P_send_buf, RESULT_FAIL_DBAGENT, 0, NULL);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, P_send_size, P_send_buf);
	}
}


static	void	P_proc_CL_CHAR_CREATE(const int fd)
{
	struct	body_CL_CHAR_CREATE	*body	= (struct body_CL_CHAR_CREATE *)(P_recv_q->data);


	if( D_GetClient_sec_active(fd) == 0 )
		return;

	//check login
	if( *D_GetClient_user_id(fd) == 0 )
	{
		U_Printf("[PROC]Hack fd(%d) CL_CHAR_CREATE not login\n", fd);
		P_remove_client(fd);
		return;
	}
	
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CL_CHAR_CREATE) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) CL_CHAR_CREATE(%d) bad body_size(%d)\n",
			fd, D_GetClient_user_id(fd), sizeof(struct body_CL_CHAR_CREATE), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}
	
	//check world_idx
	if( (body->world_idx < 1) || (body->world_idx > D_GetMax_world()) ) 
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) CL_CHAR_CREATE bad world_idx(%d)\n", fd, D_GetClient_user_id(fd), body->world_idx);
		P_remove_client(fd);
		return;
	}

	//check load_world_idx
	if( D_GetClient_load_world_idx(fd) != body->world_idx )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) CL_CHAR_CREATE bad world_idx(%d) load_world_idx(%d)\n",
				fd, D_GetClient_user_id(fd), body->world_idx, D_GetClient_load_world_idx(fd));
		P_remove_client(fd);
		return;
	}

	//check load_char_cnt
	if( D_GetClient_load_char_cnt(fd) >= MAX_CHAR_CNT )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) CL_CHAR_CREATE bad load_char_cnt(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_load_char_cnt(fd));
		P_remove_client(fd);
		return;
	}

	//check char_name
	if( (body->char_name[0] == 0) || (body->char_name[CHAR_NAME_LEN] != 0) || (U_Check_string(body->char_name, CHAR_NAME_LEN)) )
	{
		body->char_name[CHAR_NAME_LEN]	= 0;
		U_Printf("[PROC]Hack fd(%d) user_id(%s) CL_CHAR_CREATE bad char_name(%s)\n", fd, D_GetClient_user_id(fd), body->char_name);
		P_remove_client(fd);
		return;
	}

	//check sex, hair, face
	if( (body->sex > MAX_CHAR_SEX) || (body->hair > MAX_CHAR_HAIR)|| (body->face > MAX_CHAR_FACE)) 
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) bad sex(%d) hair(%d) face(%d)\n"
				,fd, D_GetClient_user_id(fd), body->sex, body->hair, body->face);
		P_remove_client(fd);
		return;
	}

	if( P_dbagent_fd[body->world_idx] )
	{
		//send WA_CHAR_CREATE
		U_Printf("[PROC]send_queue_in UNICAST(%d) WA_CHAR_CREATE user_id(%s) char_name(%s)\n",
				P_dbagent_fd[body->world_idx], D_GetClient_user_id(fd), body->char_name);
		P_send_size	= MK_GetPacket_WA_CHAR_CREATE(P_send_buf, fd, body->char_name, body->sex, body->hair, body->face);
		U_Send_queue_in(SEND_UNICAST, P_dbagent_fd[body->world_idx], 0, 0, P_send_size, P_send_buf);
	}
	else
	{
		//send LC_CHAR_CREATE_RESULT
		U_Printf("[PROC]send_queue_in UNICAST(%d) LC_CHAR_CREATE_RESULT RESULT_FAIL_DBAGENT user_id(%s) char_name(%s)\n",
											fd, D_GetClient_user_id(fd), body->char_name);
		P_send_size	= MK_GetPacket_LC_CHAR_CREATE_RESULT(P_send_buf, RESULT_FAIL_DBAGENT, body->char_name);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, P_send_size, P_send_buf);
	}
}


static	void	P_proc_CL_CHAR_DELETE(const int fd)
{
	struct	body_CL_CHAR_DELETE	*body	= (struct body_CL_CHAR_DELETE *)(P_recv_q->data);


	if( D_GetClient_sec_active(fd) == 0 )
		return;

	//check login
	if( *D_GetClient_user_id(fd) == 0 )
	{
		U_Printf("[PROC]Hack fd(%d) CL_CHAR_DELETE not login\n", fd);
		P_remove_client(fd);
		return;
	}
	
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CL_CHAR_DELETE) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) CL_CHAR_DELETE(%d) bad body_size(%d)\n",
			fd, D_GetClient_user_id(fd), sizeof(struct body_CL_CHAR_DELETE), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}
	
	//check world_idx
	if( (body->world_idx < 1) || (body->world_idx > D_GetMax_world()) ) 
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) CL_CHAR_DELETE bad world_idx(%d)\n", fd, D_GetClient_user_id(fd), body->world_idx);
		P_remove_client(fd);
		return;
	}
	
	//check load_world_idx
	if( D_GetClient_load_world_idx(fd) != body->world_idx )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) CL_CHAR_DELETE bad world_idx(%d) load_world_idx(%d)\n",
				fd, D_GetClient_user_id(fd), body->world_idx, D_GetClient_load_world_idx(fd));
		P_remove_client(fd);
		return;
	}
	
	//check load_char_cnt
	if( D_GetClient_load_char_cnt(fd) < 1 )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) CL_CHAR_DELETE bad load_char_cnt(%d)\n",
				fd, D_GetClient_user_id(fd), D_GetClient_load_char_cnt(fd));
		P_remove_client(fd);
		return;
	}
	
	//check char_name
	if( (body->char_name[0] == 0) || (body->char_name[CHAR_NAME_LEN] != 0) || (U_Check_string(body->char_name, CHAR_NAME_LEN))  )
	{
		body->char_name[CHAR_NAME_LEN]	= 0;
		U_Printf("[PROC]Hack fd(%d) user_id(%s) CL_CHAR_DELETE bad char_name(%s)\n", fd, D_GetClient_user_id(fd), body->char_name);
		P_remove_client(fd);
		return;
	}

	if( P_dbagent_fd[body->world_idx] )
	{
		//send WA_CHAR_DELETE
		U_Printf("[PROC]send_queue_in UNICAST(%d) WA_CHAR_DELETE user_id(%s) char_name(%s)\n",
					P_dbagent_fd[body->world_idx], D_GetClient_user_id(fd), body->char_name);
		P_send_size	= MK_GetPacket_WA_CHAR_DELETE(P_send_buf, fd, body->char_name);
		U_Send_queue_in(SEND_UNICAST, P_dbagent_fd[body->world_idx], 0, 0, P_send_size, P_send_buf);
	}
	else
	{
		//send LC_CHAR_DELETE_RESULT
		U_Printf("[PROC]send_queue_in UNICAST(%d) LC_CHAR_DELETE_RESULT RESULT_FAIL_DBAGENT user_id(%s) char_name(%s)\n",
					fd, D_GetClient_user_id(fd), body->char_name);
		P_send_size	= MK_GetPacket_LC_CHAR_DELETE_RESULT(P_send_buf, RESULT_FAIL_DBAGENT, body->char_name);
		U_Send_queue_in(SEND_UNICAST, fd, 0, 0, P_send_size, P_send_buf);
	}
}
				

static	void	P_proc_CL_CHANNEL_CONNECT(const int fd)
{
	int				result_code;
	struct	body_CL_CHANNEL_CONNECT	*body	= (struct body_CL_CHANNEL_CONNECT *)(P_recv_q->data);


	if( D_GetClient_sec_active(fd) == 0 )
		return;

	//check login
	if( *D_GetClient_user_id(fd) == 0 )
	{
		U_Printf("[PROC]Hack fd(%d) CL_CHANNEL_CONNECT not login\n", fd);
		P_remove_client(fd);
		return;
	}
	
	//check body_size
	if( P_recv_q->data_size != sizeof(struct body_CL_CHANNEL_CONNECT) )
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) CL_CHANNEL_CONNECT(%d) bad body_size(%d)\n",
			fd, D_GetClient_user_id(fd), sizeof(struct body_CL_CHANNEL_CONNECT), P_recv_q->data_size);
		P_remove_client(fd);
		return;
	}
	
	//check world_idx
	if( (body->world_idx < 1) || (body->world_idx > D_GetMax_world()) ) 
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) CL_CHANNEL_CONNECT bad world_idx(%d)\n", fd, D_GetClient_user_id(fd), body->world_idx);
		P_remove_client(fd);
		return;
	}
	
	//check channel_idx
	if( (body->channel_idx < 1) || (body->channel_idx > D_GetWorld_channel_cnt(body->world_idx)) ) 
	{
		U_Printf("[PROC]Hack fd(%d) user_id(%s) CL_CHANNEL_CONNECT bad channel_idx(%d)\n", fd, D_GetClient_user_id(fd), body->channel_idx);
		P_remove_client(fd);
		return;
	}

	//check user full
	if( D_GetWorld_channel_user(body->world_idx, body->channel_idx) >= USER_PER_CHANNEL )
		result_code	= RESULT_FAIL_CHANNEL_FULL;
	else
		result_code	= RESULT_SUCC;
		
	//send LC_CHANNEL_CONNECT_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) LC_CHANNEL_CONNECT_RESULT user_id(%s) result_code(%d) world_idx(%d) channel_idx(%d)\n",
			fd, D_GetClient_user_id(fd), result_code, body->world_idx, body->channel_idx);
	P_send_size	= MK_GetPacket_LC_CHANNEL_CONNECT_RESULT(P_send_buf, result_code, body->world_idx, body->channel_idx);
	U_Send_queue_in(SEND_UNICAST, fd, 0, 0, P_send_size, P_send_buf);
}
