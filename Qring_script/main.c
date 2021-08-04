
#include	"global.h"
#include	"util.h"
#include	"db.h"
#include	"accept.h"
#include	"recv.h"


// ----------------------------------------------------------------------------
// global variable
volatile sig_atomic_t		G_run;
volatile int			G_daemon;
volatile int			G_connection;
char				G_version[VERSION_LEN + 1];
struct database			G_script_db;

int				G_tcp_fd;
int				G_inner_acpt_fd[2];
int				G_inner_recv_fd[MAX_RECVT_CNT][2];

int				G_MapFileCnt;
struct map_info *		G_MapList;
struct script_file *		G_MobList;
int				G_TypeMobFileCnt;
struct script_file *		G_MobTypeList;
int				G_LevelFileCnt;
struct script_file *		G_LevelList;
int				G_CharSkillFileCnt;
struct script_file *		G_CharSkillList;
int				G_MobSkillFileCnt;
struct script_file *		G_MobSkillList;
int				G_ItemFileCnt;
struct script_file *		G_ItemList;
int				G_DropFileCnt;
struct script_file *		G_DropList;
int				G_WarpFileCnt;
struct script_file *		G_WarpList;
int				G_StoreFileCnt;
struct script_file *		G_StoreList;



// ----------------------------------------------------------------------------
// main variable
static pthread_t		M_acpt_pid;
static pthread_t		M_recv_pids		[MAX_RECVT_CNT];
static pid_t			M_pid;



// ----------------------------------------------------------------------------
// inner function
static void	M_server_release			( void );
static void     M_signal_segv                           ( int );
static void     M_signal_fpe                            ( int );
static int	M_server_daemon				( void );
static int	M_set_script_db				( void );
static int	M_set_sock				( void );




int main( int argc, char **argv )
{

	int			i, signo;
	sigset_t                blockmask, waitmask;
	struct  sigaction       segv_act, fpe_act;
	int			inner_msg_shutdown = INTER_EXIT_THREAD;

	

	dprintf(M_LOG, "\n\n\t==================================================== \n");

	// set script_db
	if( M_set_script_db() )
	{
		dprintf(M_LOG, "[MAIN]Error M_set_script_db() \n");
		exit(-1);
	}

	// checking script
	if( D_check_script() )
	{
		dprintf(M_LOG, "[MAIN]Error D_check_script() \n");
		exit(-1);
	}

	// loading script
	if( D_set_script() )
	{
		dprintf(M_LOG, "[MAIN]Error D_load_script() \n");
		exit(-1);
	}

	// set socket
	if( M_set_sock() )
	{
		dprintf(M_LOG, "[MAIN]Error M_set_sock() \n");
		exit(-1);
	}

	// initialization serverlist
	if( D_ProSL_init() )
	{
		dprintf(M_LOG, "[MAIN]Error D_ProSL_init() \n");
		exit(-1);
	}

	// set gameversion
	if( D_set_version() )
	{
		dprintf(M_LOG, "[MAIN]Error D_set_version() \n");
		exit(-1);
	}

	dprintf(M_LOG, "\t==================================================== \n\n\n\n");



	while( (i = getopt(argc, argv, "d")) != EOF )
	{

		switch( i )
		{

			case 'd' :
					G_daemon	= 1;

					if( M_server_daemon() )
					        exit(-1);
					break;

			default :
					break;
		}
	}


	M_pid	= getpid();


	signal(SIGPIPE, SIG_IGN);

	segv_act.sa_handler     = M_signal_segv;
	sigfillset(&segv_act.sa_mask);
	segv_act.sa_flags       = SA_RESTART;
	sigaction(SIGSEGV, &segv_act, NULL);

	fpe_act.sa_handler      = M_signal_fpe;
	sigfillset(&fpe_act.sa_mask);
	fpe_act.sa_flags        = SA_RESTART;
	sigaction(SIGFPE, &fpe_act, NULL);

	sigfillset(&blockmask);
	sigdelset(&blockmask, SIGSEGV);
	sigdelset(&blockmask, SIGFPE);

	if( pthread_sigmask(SIG_BLOCK, &blockmask, NULL) )
	{
		dprintf(M_LOG, "pthread_sigmask() : %s \n", strerror(errno) );
		exit(0);
	}


	G_run	= 1;

	if( pthread_create(&M_acpt_pid, NULL, A_pro_main, (void*)0) )
	{
		dprintf(M_LOG, "[MAIN]Error pthread_create() accept_thread : %s \n", strerror(errno) );
		exit(-1);
	}

	for( i=0; i<MAX_RECVT_CNT; i++ )
	{
		if( pthread_create(&M_recv_pids[i], NULL, R_pro_main, (void*)i) )
		{
			dprintf(M_LOG, "[MAIN]Error pthread_create() recv_thread : %s \n", strerror(errno) );
			exit(-1);
		}
	}

        dprintf(M_LOG, "=======================================================\n");
	dprintf(M_LOG,  "[MAIN]Script Server Start !! \n");

	//wait
	sigemptyset(&waitmask);
	sigaddset(&waitmask, SIGINT);
	sigaddset(&waitmask, SIGTERM);
	sigwait(&waitmask, &signo);

        G_run	= 0;


	send( G_inner_acpt_fd[0], (char *)&inner_msg_shutdown, SIZEOF_INT, 0 );
	pthread_join( M_acpt_pid, NULL );
	while(--i>-1) pthread_join( M_recv_pids[i], NULL );

	M_server_release();
	
	dprintf(M_LOG,  "[MAIN]Script Server Stop !! \n");
        dprintf(M_LOG, "=======================================================\n");


	return 0;
}


static void	M_server_release( void )
{
	int	i;
	close(G_tcp_fd);
	close(G_inner_acpt_fd[0]);
	for( i=0; i<MAX_RECVT_CNT; i++ )
		close(G_inner_recv_fd[i][0]);
}


static void     M_signal_segv( int sig )
{
	signal( SIGSEGV, SIG_IGN );

	dprintf(M_LOG, "[MAIN]recv SIGSEGV \n");
	M_server_release();

	signal( SIGSEGV, SIG_DFL );
}


static  void    M_signal_fpe( int sig )
{
	signal(SIGFPE, SIG_IGN);

	dprintf(M_LOG, "[MAIN]recv SIGFPE \n");
	M_server_release();

	signal(SIGFPE, SIG_DFL);
}


static int	M_server_daemon( void )
{
	pid_t   pid;
	int     i;

	if((pid = U_pro_fork())){
		exit(FAIL);
	}

	setsid();
	signal(SIGHUP, SIG_IGN);

	if((pid = U_pro_fork())){
		exit(FAIL);
	}

	i = open("/dev/null", O_RDWR | O_CREAT | O_TRUNC, 0);

	dup2(i, 0);
	dup2(i, 1);
	dup2(i, 2);

	close(i);

	return SUCC;
}


static int	M_set_script_db( void )
{

	if( U_get_value("common.ini", "script", "IP", G_script_db.ipaddress) )
	{
		dprintf(M_LOG, "[MAIN]Error M_set_script_db() script_db.ipaddress \n");
		return FAIL;
	}
	
	if( U_get_value("common.ini", "script", "NAME", G_script_db.name) )
	{
		dprintf(M_LOG, "[MAIN]Error M_set_script_db() script_db.name \n");
		return FAIL;
	}

	if( U_get_value("common.ini", "script", "ID", G_script_db.id) )
	{
		dprintf(M_LOG, "[MAIN]Error M_set_script_db() script_db.id \n");
		return FAIL;
	}

	if( U_get_value("common.ini", "script", "PWD", G_script_db.pwd) )
	{
		dprintf(M_LOG, "[MAIN]Error M_set_script_db() script_db.pwd \n");
		return FAIL;
	}

	return SUCC;
}


static int	M_set_sock( void )
{

	int			i, opt, buf_size;
	struct sockaddr_in	server_addr;
	struct linger		ling;


	// inner sock( Main <-> Accept )
	if( socketpair(AF_LOCAL, SOCK_STREAM, 0, G_inner_acpt_fd) )
	{
		dprintf(M_LOG, "[MAIN]Error socketpair() : %s \n", strerror(errno) );
		return FAIL;
	}


	// inner sock( Recv <-> Accept )
	for( i=0; i<MAX_RECVT_CNT; i++ )
	{
		if( socketpair(AF_LOCAL, SOCK_STREAM, 0, G_inner_recv_fd[i]) )
		{
			dprintf(M_LOG, "[MAIN]Error socketpair() : %s \n", strerror(errno) );
			return FAIL;
		}
	}


	// tcp socket
	if( (G_tcp_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		dprintf(M_LOG, "[MAIN]Error create tcp socket() : %s \n", strerror(errno) );
		return FAIL;
	}

	ling.l_onoff = ling.l_linger = 0;
	setsockopt( G_tcp_fd, SOL_SOCKET, SO_LINGER, (char *)&ling, sizeof(struct linger) );

	opt	= 1;
	setsockopt( G_tcp_fd, SOL_SOCKET, SO_REUSEADDR, &opt, SIZEOF_INT );

	buf_size	= SIZEOF_SNDBUF;
	setsockopt( G_tcp_fd, SOL_SOCKET, SO_SNDBUF, &buf_size, SIZEOF_INT );
	buf_size	= SIZEOF_RCVBUF;
	setsockopt( G_tcp_fd, SOL_SOCKET, SO_RCVBUF, &buf_size, SIZEOF_INT );

	bzero( (char *)&server_addr, sizeof(server_addr) );
	server_addr.sin_family		= AF_INET;
	server_addr.sin_addr.s_addr	= htonl(INADDR_ANY);
	server_addr.sin_port		= htons(ACCEPT_PORT);

	if( bind(G_tcp_fd, (struct sockaddr *)&server_addr, sizeof(server_addr) ) < 0 )
	{
		dprintf(M_LOG, "[MAIN]Error tcp socket bind() : %s \n", strerror(errno) );
		return FAIL;
	}

	listen( G_tcp_fd, MAX_RECVT_CNT );

	G_connection = 0;


	dprintf(M_LOG, "\t[MAIN]Success M_set_sock() \n");
	return SUCC;
}


