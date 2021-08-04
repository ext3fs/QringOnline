
#include	"global.h"
#include	"util.h"
#include	"db.h"

struct mysql_connect
{
	MYSQL		member;
	MYSQL		world;
};

struct item_slot
{
	int		item_slot;

	int		item1;
	int		item2;
	int		item3;
	int		item4;

	char		serial[ITEM_SERIAL_LEN];
};

struct channel
{
	int		user_cnt;
};




// --------------------------------------------------------------------------
// db variable
static struct mysql_connect	D_mysql[MAX_RECVT_CNT];
static pthread_mutex_t		D_lock;
static struct server		D_server[MAX_RECVT_CNT];

static int			D_channel[1000][1000];





// --------------------------------------------------------------------------
// db function
static inline int	D_Mysql_CheckIDX	( const int, const int );
static inline int	D_Mysql_Init		( const int, const int );
static inline int	D_Mysql_Close		( const int, const int );

static inline int	D_Mysql_ItemInsert	( const int, const char *, const char *, const struct item_slot *, int * );






// --------------------------------------------------------------------------
// script
int	D_check_version( void )
{
	int					initdb_fd;
	char					initdb_ipaddress[SIZEOF_IP + 1];
	char					buf[SIZEOF_HEADER_DBAGENT + SIZEOF_MAX_BODY + 1];
	struct header_special			send_header;
	struct body_WS_INIT			init_body;
	struct body_SW_INIT_RESULT		*init_ptr;

	
	if( U_get_value("common.ini", "script", "SCRIPTSERV", initdb_ipaddress) )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_version() read scriptserv ipaddress \n");
		return FAIL;
	}

	if( (initdb_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_version() socket create() : %s \n", strerror(errno) );
		return FAIL;
	}

	if( U_ConnectToServer(initdb_fd, initdb_ipaddress, SCRIPTSERV_PORT, 3) )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_version() U_ConnectToServer() \n");
		close(initdb_fd);
		return FAIL;
	}

	// init
	send_header.body_size	= sizeof(struct body_WS_INIT);
	send_header.command	= WS_INIT;
	send_header.fd		= 0;
	send_header.sec_active	= 0;
	send_header.session	= 0;

	strncpy( init_body.version, VERSION, (VERSION_LEN + 1) );

	memcpy( buf				, &send_header	, SIZEOF_HEADER_DBAGENT );
	memcpy( buf + SIZEOF_HEADER_DBAGENT	, &init_body	, sizeof(struct body_WS_INIT) );

	if( U_SendToServer(initdb_fd, SIZEOF_HEADER_DBAGENT + sizeof(struct body_WS_INIT), buf, 3) )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_version() U_SendToServer() \n");
		close(initdb_fd);
		return FAIL;
	}

	if( U_RecvFromServer(initdb_fd, SIZEOF_HEADER_DBAGENT + sizeof(struct body_SW_INIT_RESULT), buf, 3) )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_version() U_RecvFromServer() \n");
		close(initdb_fd);
		return FAIL;
	}

	init_ptr	= (struct body_SW_INIT_RESULT *)(buf + SIZEOF_HEADER_DBAGENT);

	if( init_ptr->result_code )
	{
		dprintf(M_LOG, "[ DB ]Error D_check_version() result_code(%d) \n", init_ptr->result_code );
		close(initdb_fd);
		return FAIL;
	}

	close(initdb_fd);

	dprintf(M_LOG, "\t[ DB ]Success D_check_version(%s)\n", VERSION );
	return SUCC;
}





// --------------------------------------------------------------------------
// Server List 
void	D_ProSL_init( void )
{
	int	i;

	pthread_mutex_init( &D_lock, NULL );

	for( i=0; i<MAX_RECVT_CNT; i++ )
	{
		D_SetSL_login( i, 0 );
		D_SetSL_fd( i, FD_IS_EMPTY );
		D_SetSL_live_send( i, 0 );
	}

	memset( D_channel, 0x00, sizeof(D_channel) );
}


void	D_SetSL_fd( const int TIDX, const int FD )
{
	D_server[TIDX].fd	= FD;
}


int	D_GetSL_fd( const int TIDX )
{
	return D_server[TIDX].fd;
}


void	D_SetSL_ipaddress( const int TIDX, const char *ipaddress )
{
        strncpy( D_server[TIDX].ipaddress, ipaddress, (SIZEOF_IP + 1) );
}


char *  D_GetSL_ipaddress( const int TIDX )
{
	return D_server[TIDX].ipaddress;
}


void	D_SetSL_login( const int TIDX, const int value )
{
	D_server[TIDX].login	= value;
}


int	D_GetSL_login( const int TIDX )
{
	return D_server[TIDX].login;
}


void	D_SetSL_live_send( const int TIDX, const int value )
{
	D_server[TIDX].live	= value;
}


int	D_GetSL_live_send( const int TIDX )
{
	return D_server[TIDX].live;
}


void	D_SetSL_last_recvtime( const int TIDX )
{
	time(&D_server[TIDX].sec_live);
}


time_t	D_GetSL_last_recvtime( const int TIDX )
{
	return D_server[TIDX].sec_live;
}


int	D_SLMysql_Init( const int TIDX )
{
	if( D_Mysql_Init(TIDX, 0) )
		return FAIL;

	if( D_Mysql_Init(TIDX, 1) )
		return FAIL;

	return SUCC;
}


int	D_SLMysql_Close( const int TIDX )
{
	D_Mysql_Close( TIDX, 0 );
	D_Mysql_Close( TIDX, 1 );

	return SUCC;
}





// --------------------------------------------------------------------------
//Mysql
static inline int	D_Mysql_CheckIDX( const int TIDX, const int flag )
{

	if( (TIDX < 0) || (TIDX > (MAX_RECVT_CNT - 1)) )
	{
		dprintf(TIDX, "[ DB ]Error argument tidx(%d) \n", TIDX );
		return FAIL;
	}

	switch( flag )
	{
		case	0 :
			if( mysql_ping(&D_mysql[TIDX].member) )
			{
				D_Mysql_Close( TIDX, 0 );

				if( D_Mysql_Init(TIDX, 0) )
					return FAIL;
			}
			break;

		case	1 :
			if( mysql_ping(&D_mysql[TIDX].world) )
			{
				D_Mysql_Close( TIDX, 1 );

				if( D_Mysql_Init(TIDX, 1) )
					return FAIL;
			}
			break;
	}

	return SUCC;
}


static inline int	D_Mysql_Init( const int TIDX, const int flag )
{
	unsigned int	timeout;


	if( (TIDX < 0) || (TIDX > (MAX_RECVT_CNT - 1)) )
	{
		dprintf(TIDX, "[ DB ]Error argument idx(%d) \n", TIDX );
		return FAIL;
	}

	pthread_mutex_lock(&D_lock);

	switch( flag )
	{
		case	0 :
			mysql_init(&D_mysql[TIDX].member);

			timeout	= 3;
			mysql_options(&D_mysql[TIDX].member, MYSQL_OPT_CONNECT_TIMEOUT, (char *)&timeout);
			mysql_options(&D_mysql[TIDX].member, MYSQL_SET_CHARSET_NAME, "euckr");

			if( !mysql_real_connect(&D_mysql[TIDX].member, G_member_db.ipaddress, G_member_db.id, G_member_db.pwd
				, G_member_db.name, 0, MYSQL_UNIX_ADDR, 0) )
			{
				dprintf(TIDX, "[ DB ]Error mysql_real_connect() %s \n ", mysql_error(&D_mysql[TIDX].member) );
				pthread_mutex_unlock(&D_lock);
				return FAIL;
			}
			break;

		case	1 :
			mysql_init(&D_mysql[TIDX].world);

			timeout	= 3;
			mysql_options(&D_mysql[TIDX].world, MYSQL_OPT_CONNECT_TIMEOUT, (char *)&timeout);
			mysql_options(&D_mysql[TIDX].world, MYSQL_SET_CHARSET_NAME, "euckr");

			if( !mysql_real_connect(&D_mysql[TIDX].world, G_world_db.ipaddress, G_world_db.id, G_world_db.pwd
				, G_world_db.name, 0, MYSQL_UNIX_ADDR, 0) )
			{
				dprintf(TIDX, "[ DB ]Error mysql_real_connect() %s \n ", mysql_error(&D_mysql[TIDX].world) );
				pthread_mutex_unlock(&D_lock);
				return FAIL;
			}
			break;
	}

	pthread_mutex_unlock(&D_lock);

	return SUCC;
}


int	D_Mysql_Close( const int TIDX, const int flag )
{

	if( (TIDX < 0) || (TIDX > (MAX_RECVT_CNT - 1)) )
	{
		dprintf(TIDX, "[ DB ]Error argument idx(%d) \n", TIDX );
		return FAIL;
	}

	pthread_mutex_lock(&D_lock);

	switch( flag )
	{
                case    0 :
			mysql_close(&D_mysql[TIDX].member);
			break;

		case    1 :
			mysql_close(&D_mysql[TIDX].world);
			break;
	}

	pthread_mutex_unlock(&D_lock);

	return SUCC;
}


int	D_Mysql_all_logout( const int TIDX, const int record_lock )
{
	char		str_query[MAX_QUERY];
	int		n;


	if( D_Mysql_CheckIDX(TIDX, 0) )
	{
		dprintf(TIDX, "[ DB ]Error D_Mysql_CheckIDX() \n");
		return FAIL;
	}


	n = snprintf(str_query, MAX_QUERY, "update %s set record_lock=0, fd=0, sec_active=0, session=0 where record_lock=%d"
		, DB_TBL_MEMBER, record_lock );
	dprintf(TIDX, "[ DB ] %s\n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX].member, str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX].member) );
		return FAIL;
	}

	if( (n = mysql_affected_rows(&D_mysql[TIDX].member)) )
		dprintf(TIDX, "[ DB ]Success record_lock(%d), all_logout_cnt(%d) \n", record_lock, n );


	return SUCC;
}


int	D_Mysql_WA_LOGIN( const int TIDX, const char *user_id, const char *user_key, const int record_lock, const int fd
		, const int sec_active, const int session, int *p_fd, int *p_sec_active, int *p_session, int *p_record_lock
		, int *p_result_code )
{
	MYSQL_ROW	row;
	MYSQL_RES	*res;
	char		str_query[MAX_QUERY];
	int		n, record_lock_fail = 0;


	*p_result_code	= 0;


	if( D_Mysql_CheckIDX(TIDX, 0) )
	{
		dprintf(TIDX, "[ DB ]Error D_Mysql_CheckIDX() \n");
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( user_id[USER_ID_LEN] != 0 )
	{
		dprintf(TIDX, "[ DB ]Error WA_LOGIN : not string data( user_id ) \n");
		*p_result_code	= RESULT_FAIL_DB;
		return FAIL;
	}


	n = snprintf(str_query, MAX_QUERY, "update %s set record_lock=%d, fd=%d, sec_active=%d, session=%d, LastTime=now() where userno='%s' and record_lock=0"
		, DB_TBL_MEMBER, record_lock, fd, sec_active, session, user_id );
	dprintf(TIDX, "[ DB ] %s\n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX].member, str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX].member) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( mysql_affected_rows(&D_mysql[TIDX].member) != 1 )
		record_lock_fail	= 1;


	n = snprintf(str_query, MAX_QUERY, "select userkey, record_lock, fd, sec_active, session from %s where userno='%s'"
			, DB_TBL_MEMBER, user_id );
	dprintf(TIDX, "[ DB ] %s\n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX].member, str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX].member) );
		D_Mysql_RecordLockFree(TIDX, user_id, record_lock, fd, sec_active, session, p_result_code);
		*p_result_code = RESULT_FAIL_DB;
		return SUCC;
	}

	if( !(res = mysql_store_result(&D_mysql[TIDX].member)) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_store_result() : %s \n", mysql_error(&D_mysql[TIDX].member) );
		D_Mysql_RecordLockFree(TIDX, user_id, record_lock, fd, sec_active, session, p_result_code);
		*p_result_code = RESULT_FAIL_DB;
		return SUCC;
	}

	if( !(row = mysql_fetch_row(res)) )
	{
		mysql_free_result(res);
		dprintf(TIDX, "[ DB ]Error mysql_fetch_row() : %s \n", mysql_error(&D_mysql[TIDX].member) );
		D_Mysql_RecordLockFree(TIDX, user_id, record_lock, fd, sec_active, session, p_result_code);
		*p_result_code = RESULT_FAIL_DB;
		return SUCC;
	}


	*p_record_lock		= atoi(row[1]);
	*p_fd			= atoi(row[2]);
	*p_sec_active		= atoi(row[3]);
	*p_session		= atoi(row[4]);

	//record_lock
	if( record_lock_fail )
	{
		mysql_free_result(res);
		dprintf(TIDX, "[ DB ]Error login() record_lock_fail user_id(%s) \n", user_id );
		*p_result_code = RESULT_FAIL_DUP_LOGIN;
		return SUCC;
	}

	//userkey
	if( strncmp(user_key, row[0], ENCODEDSTR_LEN) )
	{
		dprintf(TIDX, "[ DB ]Error login() user_id(%s), user_key(%s), db_key(%s) \n", user_id, user_key, row[0] );
		D_Mysql_RecordLockFree(TIDX, user_id, record_lock, fd, sec_active, session, p_result_code);
		*p_result_code = RESULT_FAIL_USERID_OR_PW;
		return SUCC;
	}

	mysql_free_result(res);


	dprintf(TIDX, "[ DB ]Success login() user_id(%s) \n", user_id );
	return SUCC;
}


int	D_Mysql_RecordLockFree( const int TIDX, const char *user_id, const int record_lock, const int fd, const int sec_active
		, const int session, int *p_result_code )
{
	char	str_query[MAX_QUERY];
	int	row_cnt, n;

	
	*p_result_code	= 0;


	if( D_Mysql_CheckIDX(TIDX, 0) )
	{
		dprintf(TIDX, "[ DB ]Error D_Mysql_CheckIDX() \n");
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( user_id[USER_ID_LEN] != 0 ) 
	{
		dprintf(TIDX, "[ DB ]Error record_lock_free : not string data( user_id or char_name ) \n");
		*p_result_code	= RESULT_FAIL_DB;
		return FAIL;
	}

	
	n  = snprintf( str_query, MAX_QUERY, "update %s set record_lock=0 where userno='%s' and record_lock=%d and fd=%d and sec_active=%d and session=%d", DB_TBL_MEMBER, user_id, record_lock, fd, sec_active, session );
	dprintf(TIDX, "[ DB ] %s\n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX].member, str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX].member) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( (row_cnt = mysql_affected_rows(&D_mysql[TIDX].member)) != 1 )
	{
		dprintf(TIDX, "[ DB ]Error mysql_affected_rows(%d) : %s \n", row_cnt, mysql_error(&D_mysql[TIDX].member) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}
	

	dprintf(TIDX, "[ DB ]Success record_lock_free user_id(%s) \n", user_id );
	return SUCC;
}


int	D_Mysql_WA_LOGOUT( const int TIDX, const char *user_id, const int record_lock, const int fd, const int sec_active
		, const int session, int *p_result_code )
{
	char	str_query[MAX_QUERY];
	int	row_cnt, n;

	
	*p_result_code	= 0;


	if( D_Mysql_CheckIDX(TIDX, 0) )
	{
		dprintf(TIDX, "[ DB ]Error D_Mysql_CheckIDX() \n");
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( user_id[USER_ID_LEN] != 0 ) 
	{
		dprintf(TIDX, "[ DB ]Error WA_LOGOUT : not string data( user_id or char_name ) \n");
		*p_result_code	= RESULT_FAIL_DB;
		return FAIL;
	}

	
	n  = snprintf( str_query, MAX_QUERY, "update %s set record_lock=0, LastTime=now() where userno='%s' and record_lock=%d and fd=%d and sec_active=%d and session=%d", DB_TBL_MEMBER, user_id, record_lock, fd, sec_active, session );
	dprintf(TIDX, "[ DB ] %s\n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX].member, str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX].member) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( (row_cnt = mysql_affected_rows(&D_mysql[TIDX].member)) != 1 )
	{
		dprintf(TIDX, "[ DB ]Error mysql_affected_rows(%d) : %s \n", row_cnt, mysql_error(&D_mysql[TIDX].member) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}
	

	dprintf(TIDX, "[ DB ]Success logout user_id(%s) \n", user_id );
	return SUCC;
}


int	D_Mysql_WA_CURRENT_USER( const int TIDX, const struct body_WA_CURRENT_USER *req_body, int *p_result_code )
{
	char	str_query[MAX_QUERY];
	int	row_cnt, n;

	
	*p_result_code	= 0;


	if( D_Mysql_CheckIDX(TIDX, 0) )
	{
		dprintf(TIDX, "[ DB ]Error D_Mysql_CheckIDX() \n");
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( req_body->channel_ip[SIZEOF_IP] != 0 )
	{
		dprintf(TIDX, "[ DB ]Error WA_CURRENT_USER not string data in channel_ip \n");
		*p_result_code	= RESULT_FAIL_DB;
		return FAIL;
	}

	if( req_body->channel_name[CHANNEL_NAME_LEN] != 0 )
	{
		dprintf(TIDX, "[ DB ]Error WA_CURRENT_USER not string data in channel_name \n");
		*p_result_code	= RESULT_FAIL_DB;
		return FAIL;
	}

	if( (req_body->msvr_code < 1) || (req_body->msvr_code > 999) )
	{
		dprintf(TIDX, "[ DB ]Error WA_CURRENT_USER msvr_code(%d) \n", req_body->msvr_code );
		return FAIL;
	}

	if( (req_body->ssvr_code < 1) || (req_body->ssvr_code > 999) )
	{
		dprintf(TIDX, "[ DB ]Error WA_CURRENT_USER ssvr_code(%d) \n", req_body->ssvr_code );
		return FAIL;
	}

	if( req_body->user_cnt < 0 )
	{
		dprintf(TIDX, "[ DB ]Error WA_CURRENT_USER user_cnt(%d) \n", req_body->user_cnt );
		return FAIL;
	}

	D_channel[req_body->msvr_code][req_body->ssvr_code]	= req_body->user_cnt;


	n  = snprintf( str_query, MAX_QUERY, "insert into %s (GameCode, MsvrCode, SsvrCode, ip, port, SvrName, Cnt, AddTime) values (%d, %d, %d, '%s', %d, '%s', %d, now())"
		, DB_TBL_CURRENT_USER, req_body->game_code, req_body->msvr_code, req_body->ssvr_code, req_body->channel_ip, req_body->channel_port, req_body->channel_name, req_body->user_cnt );
	dprintf(TIDX, "[ DB ] %s\n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX].member, str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX].member) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( (row_cnt = mysql_affected_rows(&D_mysql[TIDX].member)) != 1 )
	{
		dprintf(TIDX, "[ DB ]Error mysql_affected_rows(%d) : %s \n", row_cnt, mysql_error(&D_mysql[TIDX].member) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}
	

	dprintf(TIDX, "[ DB ]Success update curent_user gamecode(%d), MsvrCode(%d), SsvrCode(%d) \n"
		, req_body->game_code, req_body->msvr_code, req_body->ssvr_code );
	return SUCC;
}


int	D_Mysql_WA_GAME_LOG( const int TIDX, const struct body_WA_GAME_LOG *req_body, int *p_result_code )
{
	char	str_query[MAX_QUERY];
	int	row_cnt, n;

	
	*p_result_code	= 0;


	if( D_Mysql_CheckIDX(TIDX, 0) )
	{
		dprintf(TIDX, "[ DB ]Error D_Mysql_CheckIDX() \n");
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( req_body->user_id[USER_ID_LEN] != 0 )
	{
		dprintf(TIDX, "[ DB ]Error WA_GAME_LOG : not string data( user_id ) \n");
		*p_result_code	= RESULT_FAIL_DB;
		return FAIL;
	}

	if( req_body->char_name[USER_ID_LEN] != 0 )
	{
		dprintf(TIDX, "[ DB ]Error WA_GAME_LOG : not string data( char_name ) \n");
		*p_result_code	= RESULT_FAIL_DB;
		return FAIL;
	}

	if( req_body->channel_name[CHANNEL_NAME_LEN] != 0 )
	{
		dprintf(TIDX, "[ DB ]Error WA_GAME_LOG : not string data in channel_name \n");
		*p_result_code	= RESULT_FAIL_DB;
		return FAIL;
	}

	n  = snprintf( str_query, MAX_QUERY, "insert into %s (gamecode, Userno, GameID, SvrName, UserIP, LogType, LoginTime, GameTime, Level, Exp) value (%d, '%s', '%s', '%s', '%s', '%d', now(), %d, %d, %d) "
		, DB_TBL_GAME_LOG
		, req_body->game_code
		, req_body->user_id
		, req_body->char_name
		, req_body->channel_name
		, req_body->user_ip
		, req_body->log_type
		, req_body->game_time
		, req_body->level
		, req_body->exp );
	dprintf(TIDX, "[ DB ] %s\n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX].member, str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX].member) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( (row_cnt = mysql_affected_rows(&D_mysql[TIDX].member)) != 1 )
	{
		dprintf(TIDX, "[ DB ]Error mysql_affected_rows(%d) : %s \n", row_cnt, mysql_error(&D_mysql[TIDX].member) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}
	

	dprintf(TIDX, "[ DB ]Success update game_log gamecode(%d), user_id(%s), char_name(%s)\n"
		, req_body->game_code, req_body->user_id, req_body->char_name );
	return SUCC;
}


int	D_Mysql_WA_GET_USER( const int TIDX, const struct body_WA_GET_USER *req_body, int *p_user_cnt )
{

	if( (req_body->msvr_code < 1) || (req_body->msvr_code > 999) )
	{
		dprintf(TIDX, "[ DB ]Error WA_CURRENT_USER msvr_code(%d) \n", req_body->msvr_code );
		return FAIL;
	}

	if( (req_body->ssvr_code < 1) || (req_body->ssvr_code > 999) )
	{
		dprintf(TIDX, "[ DB ]Error WA_CURRENT_USER ssvr_code(%d) \n", req_body->ssvr_code );
		return FAIL;
	}


	*p_user_cnt	= D_channel[req_body->msvr_code][req_body->ssvr_code];


	dprintf(TIDX, "[ DB ]Success get_user MsvrCode(%d), SsvrCode(%d), user_cnt(%d) \n"
		, req_body->msvr_code, req_body->ssvr_code, *p_user_cnt );
	return SUCC;
}


int	D_Mysql_WA_CHAR_LIST( const int TIDX, const char *user_id, int *p_char_cnt, struct body2_AW_CHAR_LIST_RESULT *res_body2
		, int *p_result_code )
{
	MYSQL_ROW	row;
	MYSQL_RES	*res;
	char		str_query[MAX_QUERY];
	int		n, i, row_cnt;


	*p_char_cnt	= 0;
	*p_result_code	= 0;


	if( D_Mysql_CheckIDX(TIDX, 1) )
	{
		dprintf(TIDX, "[ DB ]Error D_Mysql_CheckIDX() \n");
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( user_id[USER_ID_LEN] != 0 )
	{
		dprintf(TIDX, "[ DB ]Error WA_CHAR_LIST : not string data( user_id ) \n");
		*p_result_code	= RESULT_FAIL_DB;
		return FAIL;
	}


	n = snprintf( str_query, MAX_QUERY, "select char_name, sex, hair, face, level from %s where user_id = '%s'"
		, DB_TBL_CHAR, user_id );
	dprintf(TIDX, "[ DB ] %s\n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX].world, str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( !(res = mysql_store_result(&D_mysql[TIDX].world)) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_store_result() : %s \n", mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}
	
	if( ((row_cnt = mysql_num_rows(res)) < 0) || (row_cnt > 3) )
	{
		dprintf(TIDX, "[ DB ]Error row_cnt(%d) \n", row_cnt);
		mysql_free_result(res);
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}


	for( i=0; i<row_cnt; i++ )
	{
		if( !(row = mysql_fetch_row(res)) )
		{
			dprintf(TIDX, "[ DB ]Error mysql_fetch_row() \n");
			mysql_free_result(res);

			*p_char_cnt	= 0;
			*p_result_code	= RESULT_FAIL_DB;
			return SUCC;
		}

		strncpy( res_body2[i].char_name, row[0], (CHAR_NAME_LEN + 1) );
		res_body2[i].sex	= atoi(row[1]);
		res_body2[i].hair	= atoi(row[2]);
		res_body2[i].face	= atoi(row[3]);
		res_body2[i].level	= atoi(row[4]);
	}	

	*p_char_cnt	= row_cnt;
	mysql_free_result(res);

	dprintf(TIDX, "[ DB ]Success char_list user_id(%s), list_cnt(%d) \n", user_id, *p_char_cnt );
	return SUCC;
}


int	D_Mysql_WA_CHAR_CREATE( const int TIDX, const struct body_WA_CHAR_CREATE *req_body, int *p_result_code )
{
	char	str_query[MAX_QUERY];
	int	row_cnt, n;

	
	*p_result_code	= 0;


	if( D_Mysql_CheckIDX(TIDX, 1) )
	{
		dprintf(TIDX, "[ DB ]Error D_Mysql_CheckIDX() \n");
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( (req_body->user_id[USER_ID_LEN] != 0) || (req_body->char_name[CHAR_NAME_LEN] != 0) )
	{
		dprintf(TIDX, "[ DB ]Error WA_CHAR_CREATE : not string data( user_id or char_name ) \n");
		*p_result_code	= RESULT_FAIL_DB;
		return FAIL;
	}

	
	n  = snprintf( str_query, MAX_QUERY, "insert into %s (char_name, user_id, sex, hair, face, level, exp, hp, ap, map_num, x, y, z, regen_map, regen_x, regen_y, regen_z) values ('", DB_TBL_CHAR );
	n += U_change_escape_string( (str_query+n), req_body->char_name, strlen(req_body->char_name) );
	n += snprintf( (str_query+n), (MAX_QUERY-n), "', '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ,%d)"
		, req_body->user_id
		, req_body->sex
		, req_body->hair
		, req_body->face
		, req_body->level
		, req_body->exp
		, req_body->hp
		, req_body->ap
		, req_body->map
		, req_body->x
		, req_body->y
		, req_body->z
		, req_body->regen_map
		, req_body->regen_x
		, req_body->regen_y
		, req_body->regen_z );
	dprintf(TIDX, "[ DB ] %s\n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX].world, str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX].world) );

		if( mysql_errno(&D_mysql[TIDX].world) == 1062 )
			*p_result_code	= RESULT_FAIL_DUP_CHAR_NAME;
		else
			*p_result_code	= RESULT_FAIL_DB;

		return SUCC;
	}

	if( (row_cnt = mysql_affected_rows(&D_mysql[TIDX].world)) != 1 )
	{
		dprintf(TIDX, "[ DB ]Error mysql_affected_rows(%d) : %s \n", row_cnt, mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}
	

	dprintf(TIDX, "[ DB ]Success char_create user_id(%s), char_name(%s) \n", req_body->user_id, req_body->char_name );
	return SUCC;
}


int	D_Mysql_WA_CHAR_DELETE( const int TIDX, const char *user_id, const char *char_name, int *p_result_code )
{
	char	str_query[MAX_QUERY];
	int	n, row_cnt, delete_item_cnt;

	
	*p_result_code	= 0;


	if( D_Mysql_CheckIDX(TIDX, 1) )
	{
		dprintf(TIDX, "[ DB ]Error D_Mysql_CheckIDX() \n");
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( (user_id[USER_ID_LEN] != 0) || (char_name[CHAR_NAME_LEN] != 0) )
	{
		dprintf(TIDX, "[ DB ]Error WA_CHAR_DELETE : not string data( user_id or char_name ) \n");
		*p_result_code	= RESULT_FAIL_DB;
		return FAIL;
	}

	
	// char_delete 
	n  = snprintf( str_query, MAX_QUERY, "delete from %s where user_id='%s' and char_name='", DB_TBL_CHAR, user_id );
	n += U_change_escape_string( (str_query+n), char_name, strlen(char_name) );
	n += snprintf( (str_query+n), (MAX_QUERY-n), "' ");
	dprintf(TIDX, "[ DB ] %s\n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX].world, str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( (row_cnt = mysql_affected_rows(&D_mysql[TIDX].world)) != 1 )
	{
		dprintf(TIDX, "[ DB ]Error mysql_affected_rows(%d) : %s \n", row_cnt, mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}
	

	// item_delete
	n  = snprintf( str_query, MAX_QUERY, "delete from %s where user_id='%s' and char_name='", DB_TBL_ITEM, user_id );
	n += U_change_escape_string( (str_query+n), char_name, strlen(char_name) );
	n += snprintf( (str_query+n), (MAX_QUERY-n), "' " );
	dprintf(TIDX, "[ DB ] %s\n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX].world, str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( ((row_cnt = mysql_affected_rows(&D_mysql[TIDX].world)) < 0) || (row_cnt > (ITEM_SLOT_CNT - 1)) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_affected_rows(%d) : %s \n", row_cnt, mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	delete_item_cnt		= row_cnt;


	dprintf(TIDX, "[ DB ]Success char_delete user_id(%s), char_name(%s), delete_item_cnt(%d) \n"
		, user_id, char_name, delete_item_cnt );
	return SUCC;
}


int	D_Mysql_WA_CHAR_LOAD( const int TIDX, const char *user_id, const char *char_name, struct body1_AW_CHAR_LOAD_RESULT *p_res_body1
		, struct body2_AW_CHAR_LOAD_RESULT *p_res_body2, int *p_result_code )
{
	MYSQL_ROW				row;
	MYSQL_RES				*res;
	char					str_query[MAX_QUERY];
	int					i, n, row_cnt;

	struct item_slot			*p_item;
	int					delete_item_cnt;

	
	*p_result_code			= 0;
	p_res_body1->item_slot_cnt 	= 0;


	if( D_Mysql_CheckIDX(TIDX, 1) )
	{
		dprintf(TIDX, "[ DB ]Error D_Mysql_CheckIDX() \n");
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( (user_id[USER_ID_LEN] != 0) || (char_name[CHAR_NAME_LEN] != 0) )
	{
		dprintf(TIDX, "[ DB ]Error WA_CHAR_LOAD : not string data( user_id or char_name ) \n");
		*p_result_code	= RESULT_FAIL_DB;
		return FAIL;
	}


	// basic_info
	n  = snprintf( str_query, MAX_QUERY, "select load_cnt, map_num, x, y, z, regen_map, regen_x, regen_y, regen_z, sex, hair, face, level, exp, hp, ap, money, skill_point, skill1, skill2, skill3, skill4, skill5, skill6, skill7, skill8, skill9, skill10, skill11, skill12, skill13, skill14, skill15, skill16, skill17, skill18, skill19, skill20, skill21, skill22, skill23, skill24, skill25, skill26, skill27, skill28, skill29, skill30, skill31, skill32, skill33, skill34, skill35, skill36, skill37, skill38, skill39, skill40, skill41, skill42, skill43, skill44, skill45, skill46, skill47, skill48 from %s where user_id='%s' and char_name='"
		, DB_TBL_CHAR, user_id );
	n += U_change_escape_string( (str_query+n), char_name, strlen(char_name) );
	n += snprintf( (str_query+n), (MAX_QUERY-n), "' " );
	dprintf(TIDX, "[ DB ] %s\n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX].world, str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( !(res = mysql_store_result(&D_mysql[TIDX].world)) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_store_result() : %s \n", mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}
	
	if( (row_cnt = mysql_num_rows(res)) != 1 )
	{
		dprintf(TIDX, "[ DB ]Error row_cnt(%d) \n", row_cnt );
		mysql_free_result(res);
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( !(row = mysql_fetch_row(res)) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_fetch_row() : %s \n", mysql_error(&D_mysql[TIDX].world) );
		mysql_free_result(res);
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	p_res_body1->load_cnt		= atoi(row[0]);
	p_res_body1->map		= atoi(row[1]);
	p_res_body1->x			= atoi(row[2]);
	p_res_body1->y			= atoi(row[3]);
	p_res_body1->z			= atoi(row[4]);
	p_res_body1->regen_map		= atoi(row[5]);
	p_res_body1->regen_x		= atoi(row[6]);
	p_res_body1->regen_y		= atoi(row[7]);
	p_res_body1->regen_z		= atoi(row[8]);
	p_res_body1->sex		= atoi(row[9]);
	p_res_body1->hair		= atoi(row[10]);
	p_res_body1->face		= atoi(row[11]);
	p_res_body1->level		= atoi(row[12]);
	p_res_body1->exp		= atoi(row[13]);
	p_res_body1->hp			= atoi(row[14]);
	p_res_body1->ap			= atoi(row[15]);
	p_res_body1->money		= atoi(row[16]);
	p_res_body1->skill_point	= atoi(row[17]);
	p_res_body1->skill_slot[0]	= atoi(row[18]);
	p_res_body1->skill_slot[1]	= atoi(row[19]);
	p_res_body1->skill_slot[2]	= atoi(row[20]);
	p_res_body1->skill_slot[3]	= atoi(row[21]);
	p_res_body1->skill_slot[4]	= atoi(row[22]);
	p_res_body1->skill_slot[5]	= atoi(row[23]);
	p_res_body1->skill_slot[6]	= atoi(row[24]);
	p_res_body1->skill_slot[7]	= atoi(row[25]);
	p_res_body1->skill_slot[8]	= atoi(row[26]);
	p_res_body1->skill_slot[9]	= atoi(row[27]);
	p_res_body1->skill_slot[10]	= atoi(row[28]);
	p_res_body1->skill_slot[11]	= atoi(row[29]);
	p_res_body1->skill_slot[12]	= atoi(row[30]);
	p_res_body1->skill_slot[13]	= atoi(row[31]);
	p_res_body1->skill_slot[14]	= atoi(row[32]);
	p_res_body1->skill_slot[15]	= atoi(row[33]);
	p_res_body1->skill_slot[16]	= atoi(row[34]);
	p_res_body1->skill_slot[17]	= atoi(row[35]);
	p_res_body1->skill_slot[18]	= atoi(row[36]);
	p_res_body1->skill_slot[19]	= atoi(row[37]);
	p_res_body1->skill_slot[20]	= atoi(row[38]);
	p_res_body1->skill_slot[21]	= atoi(row[39]);
	p_res_body1->skill_slot[22]	= atoi(row[40]);
	p_res_body1->skill_slot[23]	= atoi(row[41]);
	p_res_body1->skill_slot[24]	= atoi(row[42]);
	p_res_body1->skill_slot[25]	= atoi(row[43]);
	p_res_body1->skill_slot[26]	= atoi(row[44]);
	p_res_body1->skill_slot[27]	= atoi(row[45]);
	p_res_body1->skill_slot[28]	= atoi(row[46]);
	p_res_body1->skill_slot[29]	= atoi(row[47]);
	p_res_body1->skill_slot[30]	= atoi(row[48]);
	p_res_body1->skill_slot[31]	= atoi(row[49]);
	p_res_body1->skill_slot[32]	= atoi(row[50]);
	p_res_body1->skill_slot[33]	= atoi(row[51]);
	p_res_body1->skill_slot[34]	= atoi(row[52]);
	p_res_body1->skill_slot[35]	= atoi(row[53]);
	p_res_body1->skill_slot[36]	= atoi(row[54]);
	p_res_body1->skill_slot[37]	= atoi(row[55]);
	p_res_body1->skill_slot[38]	= atoi(row[56]);
	p_res_body1->skill_slot[39]	= atoi(row[57]);
	p_res_body1->skill_slot[40]	= atoi(row[58]);
	p_res_body1->skill_slot[41]	= atoi(row[59]);
	p_res_body1->skill_slot[42]	= atoi(row[60]);
	p_res_body1->skill_slot[43]	= atoi(row[61]);
	p_res_body1->skill_slot[44]	= atoi(row[62]);
	p_res_body1->skill_slot[45]	= atoi(row[63]);
	p_res_body1->skill_slot[46]	= atoi(row[64]);
	p_res_body1->skill_slot[47]	= atoi(row[65]);
	p_res_body1->skill_slot[48]	= atoi(row[66]);

	mysql_free_result(res);


	// item_select
	n = snprintf( str_query, MAX_QUERY, "select position, serial, item1, item2, item3, item4 from %s where user_id='%s' and char_name='"
		, DB_TBL_ITEM, user_id );
	n += U_change_escape_string( (str_query+n), char_name, strlen(char_name) );
	n += snprintf( (str_query+n), (MAX_QUERY-n), "' " );
	dprintf(TIDX, "[ DB ] %s\n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX].world, str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( !(res = mysql_store_result(&D_mysql[TIDX].world)) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_store_result() : %s \n", mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}
	
	if( ((row_cnt = mysql_num_rows(res)) < 0) || (row_cnt > (ITEM_SLOT_CNT - 1)) )
	{
		dprintf(TIDX, "[ DB ]Error item_cnt(%d) \n", row_cnt);
		mysql_free_result(res);
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	for( i=0; i<row_cnt; i++ )
	{
		if( !(row = mysql_fetch_row(res)) )
		{
			dprintf(TIDX, "[ DB ]Error mysql_fetch_row() \n");
			mysql_free_result(res);

			*p_result_code	= RESULT_FAIL_DB;
			return SUCC;
		}

		p_item			= (struct item_slot *)((char *)p_res_body2 + (i * sizeof(struct body2_AW_CHAR_LOAD_RESULT)) );

		p_item->item_slot 	= atoi(row[0]);

		if( (p_item->item_slot < 0) || (p_item->item_slot > (ITEM_SLOT_CNT - 1)) )
		{
			dprintf(TIDX, "[ DB ]Error char_load user_id(%s), char_name(%s), item_slot(%d) \n"
				, user_id, char_name, p_item->item_slot); 
			continue;
		}

		if( row[1] != NULL )
			strncpy( p_item->serial, row[1], ITEM_SERIAL_LEN );
		else
			memset( p_item->serial, 0x00, ITEM_SERIAL_LEN );

		p_item->item1	= atoi(row[2]);
		p_item->item2	= atoi(row[3]);
		p_item->item3	= atoi(row[4]);
		p_item->item4	= atoi(row[5]);
	}

	p_res_body1->item_slot_cnt	= row_cnt;
	mysql_free_result(res);


	// item_delete
	n  = snprintf( str_query, MAX_QUERY, "delete from %s where user_id='%s' and char_name='", DB_TBL_ITEM, user_id );
	n += U_change_escape_string( (str_query+n), char_name, strlen(char_name) );
	n += snprintf( (str_query+n), (MAX_QUERY-n), "' " );
	dprintf(TIDX, "[ DB ] %s\n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX].world, str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( ((row_cnt = mysql_affected_rows(&D_mysql[TIDX].world)) < 0) || (row_cnt > (ITEM_SLOT_CNT - 1)) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_affected_rows(%d) : %s \n", row_cnt, mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	delete_item_cnt		= row_cnt;


	dprintf(TIDX, "[ DB ]Success char_load user_id(%s), char_name(%s), load_cnt(%d), item_slot_cnt(%d), delete_item_cnt(%d) \n"
		, user_id, char_name, p_res_body1->load_cnt, p_res_body1->item_slot_cnt, delete_item_cnt );
	return SUCC;
}



int	D_Mysql_WA_CHAR_SAVE( const int TIDX, const struct body1_WA_CHAR_SAVE *req_body1, const struct body2_WA_CHAR_SAVE *req_body2
		, int *p_result_code )
{
	int			i, row_cnt, n;
	char			str_query[MAX_QUERY];

	struct item_slot	*p_item;


	*p_result_code	= 0;


	if( D_Mysql_CheckIDX(TIDX, 1) )
	{
		dprintf(TIDX, "[ DB ]Error D_Mysql_CheckIDX() \n");
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( (req_body1->user_id[USER_ID_LEN] != 0) || (req_body1->char_name[CHAR_NAME_LEN] != 0) )
	{
		dprintf(TIDX, "[ DB ]Error WA_CHAR_SAVE : not string data( user_id or char_name ) \n");
		*p_result_code	= RESULT_FAIL_DB;
		return FAIL;
	}


	// basic_info
	n  = snprintf( str_query, MAX_QUERY, "update %s set load_cnt=%d, map_num=%d, x=%d, y=%d, z=%d, regen_map=%d, regen_x=%d, regen_y=%d, regen_z=%d, sex=%d, hair=%d, face=%d, level=%d, exp=%d, hp=%d, ap=%d, money=%d, skill_point=%d, skill1=%d, skill2=%d, skill3=%d, skill4=%d, skill5=%d, skill6=%d, skill7=%d, skill8=%d, skill9=%d, skill10=%d, skill11=%d, skill12=%d, skill13=%d, skill14=%d, skill15=%d, skill16=%d, skill17=%d, skill18=%d, skill19=%d, skill20=%d, skill21=%d, skill22=%d, skill23=%d, skill24=%d, skill25=%d, skill26=%d, skill27=%d, skill28=%d, skill29=%d, skill30=%d, skill31=%d, skill32=%d, skill33=%d, skill34=%d, skill35=%d, skill36=%d, skill37=%d, skill38=%d, skill39=%d, skill40=%d, skill41=%d, skill42=%d, skill43=%d, skill44=%d, skill45=%d, skill46=%d, skill47=%d, skill48=%d where user_id='%s' and char_name='"
		, DB_TBL_CHAR
		, req_body1->load_cnt
		, req_body1->map
		, req_body1->x
		, req_body1->y
		, req_body1->z
		, req_body1->regen_map
		, req_body1->regen_x
		, req_body1->regen_y
		, req_body1->regen_z
		, req_body1->sex
		, req_body1->hair
		, req_body1->face
		, req_body1->level
		, req_body1->exp
		, req_body1->hp
		, req_body1->ap
		, req_body1->money
		, req_body1->skill_point
		, req_body1->skill_slot[0]
		, req_body1->skill_slot[1]
		, req_body1->skill_slot[2]
		, req_body1->skill_slot[3]
		, req_body1->skill_slot[4]
		, req_body1->skill_slot[5]
		, req_body1->skill_slot[6]
		, req_body1->skill_slot[7]
		, req_body1->skill_slot[8]
		, req_body1->skill_slot[9]
		, req_body1->skill_slot[10]
		, req_body1->skill_slot[11]
		, req_body1->skill_slot[12]
		, req_body1->skill_slot[13]
		, req_body1->skill_slot[14]
		, req_body1->skill_slot[15]
		, req_body1->skill_slot[16]
		, req_body1->skill_slot[17]
		, req_body1->skill_slot[18]
		, req_body1->skill_slot[19]
		, req_body1->skill_slot[20]
		, req_body1->skill_slot[21]
		, req_body1->skill_slot[22]
		, req_body1->skill_slot[23]
		, req_body1->skill_slot[24]
		, req_body1->skill_slot[25]
		, req_body1->skill_slot[26]
		, req_body1->skill_slot[27]
		, req_body1->skill_slot[28]
		, req_body1->skill_slot[29]
		, req_body1->skill_slot[30]
		, req_body1->skill_slot[31]
		, req_body1->skill_slot[32]
		, req_body1->skill_slot[33]
		, req_body1->skill_slot[34]
		, req_body1->skill_slot[35]
		, req_body1->skill_slot[36]
		, req_body1->skill_slot[37]
		, req_body1->skill_slot[38]
		, req_body1->skill_slot[39]
		, req_body1->skill_slot[40]
		, req_body1->skill_slot[41]
		, req_body1->skill_slot[42]
		, req_body1->skill_slot[43]
		, req_body1->skill_slot[44]
		, req_body1->skill_slot[45]
		, req_body1->skill_slot[46]
		, req_body1->skill_slot[47]
		, req_body1->user_id );
	n += U_change_escape_string( (str_query+n), req_body1->char_name, strlen(req_body1->char_name) );
	n += snprintf( (str_query+n), (MAX_QUERY-n), "' " );
	dprintf(TIDX, "[ DB ] %s\n", str_query); 

	if( mysql_real_query(&D_mysql[TIDX].world, str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}

	if( (row_cnt = mysql_affected_rows(&D_mysql[TIDX].world)) != 1 )
	{
		dprintf(TIDX, "[ DB ]Error mysql_affected_rows(%d) : %s \n", row_cnt, mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return SUCC;
	}


	// item_insert
	for( i=0; i<req_body1->item_slot_cnt; i++ )
	{
		p_item	= (struct item_slot *)((char *)req_body2 + (i * sizeof(struct body2_WA_CHAR_SAVE)));

		if( D_Mysql_ItemInsert(TIDX, req_body1->user_id, req_body1->char_name, p_item, p_result_code) )
		{
			dprintf(TIDX, "[ DB ]Error D_Mysql_ItemInsert() user_id(%s), char_name(%s), item_slot(%d) \n"
				, req_body1->user_id, req_body1->char_name, p_item->item_slot );
		}
	}


	dprintf(TIDX, "[ DB ]Success char_save user_id(%s), char_name(%s), load_cnt(%d), item_slot_cnt(%d) \n"
		, req_body1->user_id, req_body1->char_name, req_body1->load_cnt, req_body1->item_slot_cnt );
	return SUCC;
}


static inline int	D_Mysql_ItemInsert( const int TIDX, const char *user_id, const char *char_name
			, const struct item_slot *p_item, int *p_result_code )
{
	char			str_query[MAX_QUERY];
	int			row_cnt, n;


	if( !(p_item->item1) && !(p_item->item2) && !(p_item->item3) && !(p_item->item4) )
		return SUCC;

	if( p_item->serial[0] == 0 )
	{
		n  = snprintf( str_query, MAX_QUERY, "insert into %s (char_name, user_id, position, item1, item2, item3, item4) values ('"
			, DB_TBL_ITEM );
		n += U_change_escape_string( (str_query+n), char_name, strlen(char_name) );
		n += snprintf( (str_query+n), (MAX_QUERY-n), "', '%s', %d, %d, %d, %d, %d )"
			, user_id, p_item->item_slot, p_item->item1, p_item->item2, p_item->item3, p_item->item4 );
	}
	else
	{
		n  = snprintf( str_query, MAX_QUERY, "insert into %s (char_name, user_id, position, serial, item1, item2, item3, item4) values ('"
			, DB_TBL_ITEM );
		n += U_change_escape_string( (str_query+n), char_name, strlen(char_name) );
		n += snprintf( (str_query+n), (MAX_QUERY-n), "', '%s', %d, '", user_id, p_item->item_slot );
		n += U_change_escape_string( (str_query+n), p_item->serial, ITEM_SERIAL_LEN );
		n += snprintf( (str_query+n), (MAX_QUERY-n), "', %d, %d, %d, %d)"	
			, p_item->item1, p_item->item2, p_item->item3, p_item->item4 );
	}

	dprintf(TIDX, "[ DB ] %s\n", str_query); 


	if( mysql_real_query(&D_mysql[TIDX].world, str_query, n) )
	{
		dprintf(TIDX, "[ DB ]Error mysql_real_query() : %s \n", mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return FAIL;
	}

	if( (row_cnt = mysql_affected_rows(&D_mysql[TIDX].world)) != 1 )
	{
		dprintf(TIDX, "[ DB ]Error mysql_affected_rows(%d) : %s \n", row_cnt, mysql_error(&D_mysql[TIDX].world) );
		*p_result_code	= RESULT_FAIL_DB;
		return FAIL;
	}

	return SUCC;
}
