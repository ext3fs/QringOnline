
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
int				G_world;
struct database			G_member_db;
struct database			G_world_db;

int				G_tcp_fd;
int				G_inner_acpt_fd[2];
int				G_inner_recv_fd[MAX_RECVT_CNT][2];




// ----------------------------------------------------------------------------
// main variable
static pthread_t		M_acpt_pid;
static pthread_t		M_recv_pids[MAX_RECVT_CNT];
static pid_t			M_pid;




// ----------------------------------------------------------------------------
// inner function
static void	M_server_release			( void );
static void     M_signal_segv                           ( int );
static void     M_signal_fpe                            ( int );
static int	M_server_daemon				( void );
static int	M_set_world_db				( void );
static int	M_set_sock				( void );





int main( int argc, char **argv )
{

	int			i, signo;
	sigset_t                blockmask, waitmask;
	struct  sigaction       segv_act, fpe_act;
	int			inner_msg_shutdown = INTER_EXIT_THREAD;



	dprintf(M_LOG, "\n\n\t==================================================== \n");


	// check version
	if( D_check_version() )
	{
		dprintf(M_LOG, "[MAIN]Error D_check_version() \n");
		exit(-1);
	}


	// set world_db
	if( M_set_world_db() )
	{
		dprintf(M_LOG, "[MAIN]Error M_set_world_db() \n");
		exit(-1);
	}


	// set socket
	if( M_set_sock() )
	{
		dprintf(M_LOG, "[MAIN]Error M_set_sock() \n");
		exit(-1);
	}


	D_ProSL_init();


	dprintf(M_LOG, "\t==================================================== \n\n\n\n");



	// process deamon
	while( (i=getopt(argc, argv, "d")) != EOF )
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

	for( i=0; i<MAX_RECVT_CNT; i++ )
	{
		if( pthread_create(&M_recv_pids[i], NULL, R_pro_main, (void*)i) )
		{
			dprintf(M_LOG, "[MAIN]Error pthread_create() recv_thread : %s \n", strerror(errno) );
			exit(-1);
		}
	}

	if( pthread_create(&M_acpt_pid, NULL, A_pro_main, (void*)0) )
	{
		dprintf(M_LOG, "[MAIN]Error pthread_create() accept_thread : %s \n", strerror(errno) );
		exit(-1);
	}

        dprintf(M_LOG, "=======================================================\n");
	dprintf(M_LOG,  "[MAIN]Agent Server Start !! \n");

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
	
	dprintf(M_LOG,  "[MAIN]Agent Server Stop !! \n");
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


static int	M_set_world_db( void )
{
	char	tmpBuf[256], tmpWorld[256];


	// world
	if( U_get_value("agent_serv.ini", "basic", "world", tmpBuf) )
	{
		dprintf(M_LOG, "[MAIN]Error read G_world \n");
		return FAIL;
	}

	if( (G_world = atoi(tmpBuf)) < 1 )
	{
		dprintf(M_LOG, "[MAIN]Error G_world(%d)\n", G_world );
		return FAIL;
	}

	sprintf( tmpWorld, "world_%d", G_world );


	// world_db
	if( U_get_value("common.ini", tmpWorld, "IP", G_world_db.ipaddress) )
	{
		dprintf(M_LOG, "[MAIN]Error read %d's world_db ipaddress\n", G_world);
		return FAIL;
	}

	if( U_get_value("common.ini", tmpWorld, "NAME", G_world_db.name) )
	{
		dprintf(M_LOG, "[MAIN]Error read %d's world_db name\n", G_world);
		return FAIL;
	}

	if( U_get_value("common.ini", tmpWorld, "ID", G_world_db.id) )
	{
		dprintf(M_LOG, "[MAIN]Error read %d's world_db id\n", G_world);
		return FAIL;
	}

	if( U_get_value("common.ini", tmpWorld, "PWD", G_world_db.pwd) )
	{
		dprintf(M_LOG, "[MAIN]Error read %d's world_db pwd\n", G_world);
		return FAIL;
	}


	// member_db
	if( U_get_value("common.ini", "member", "IP", G_member_db.ipaddress) )
	{
		dprintf(M_LOG, "[MAIN]Error read %d's member_db ipaddress\n", G_world);
		return FAIL;
	}

	if( U_get_value("common.ini", "member", "NAME", G_member_db.name) )
	{
		dprintf(M_LOG, "[MAIN]Error read %d's member_db name\n", G_world);
		return FAIL;
	}

	if( U_get_value("common.ini", "member", "ID", G_member_db.id) )
	{
		dprintf(M_LOG, "[MAIN]Error read %d's member_db id\n", G_world);
		return FAIL;
	}

	if( U_get_value("common.ini", "member", "PWD", G_member_db.pwd) )
	{
		dprintf(M_LOG, "[MAIN]Error read %d's member_db pwd\n", G_world);
		return FAIL;
	}

	dprintf(M_LOG, "\t[MAIN]Success M_set_world_db(%d)\n", G_world);
	return SUCC;
}


static int	M_set_sock( void )
{

	int			i;
	struct sockaddr_in	server_addr;
	struct linger		ling;
	int			opt, buf_size;
	int			flags;


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

	flags = fcntl( G_tcp_fd, F_GETFL, 0 );
	fcntl( G_tcp_fd, F_SETFL, flags | O_NONBLOCK );

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


	dprintf(M_LOG, "\t[MAIN]Success M_set_sock(%d) \n", G_tcp_fd);
	return SUCC;
}


