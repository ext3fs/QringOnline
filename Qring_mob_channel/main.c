
#include	"global.h"
#include	"util.h"
#include	"db.h"
#include	"sp.h"
#include	"mymath.h"
#include	"tick.h"
#include	"recv.h"
#include	"send.h"
#include	"proc.h"


// ----------------------------------------------------------------------------
// global variable
volatile sig_atomic_t		G_run;
volatile int			G_daemon;

volatile time_t			G_Sec;
volatile double			G_MilliSec;

volatile int			G_Input_fds;
int				G_inner_close_fd[2];
int				G_inner_send_fd[2];
int				G_inner_check_fd[2];

volatile int			G_proc_thr_cnt;
struct thread_args		*G_proc_thr_args;
volatile int			G_channel_proc_thr[TOTAL_CHANNEL_CNT + 1];
volatile int			G_channel_ins[TOTAL_CHANNEL_CNT + 1];




// ----------------------------------------------------------------------------
// main variable
static pid_t			M_pid;
static pthread_t		M_tids[1024];
static int			M_tids_idx;





// ----------------------------------------------------------------------------
// main function
static inline void	M_server_release		( void );
static inline int	M_server_daemon			( void );
static inline void	M_signal_segv			( int sig );
static inline void	M_signal_fpe			( int sig );
static inline int	M_thread_init			( void );
static inline int	M_set_sock			( void );
static inline int	M_set_channel			( void );



int main( int argc, char **argv )
{

	int			signo, opt, msg_type;
        sigset_t                blockmask, waitmask;
        struct sigaction	segv_act, fpe_act;


	dprintf(0, "\n\n\t==================================================== \n");


	T_set_localtime();


	if( M_set_channel() )
	{
		dprintf(0, "[MAIN]Error U_set_channel() \n");
		exit(-1);
	}

	if( U_set_random(1024 * 128) )
	{
		dprintf(0, "[MAIN]Error U_set_random() \n");
		exit(-1);
	}

	if( D_set_script() )
	{
		dprintf(0, "[MAIN]Error D_set_script() \n");
		exit(-1);
	}

	//if( U_set_logdir() )
	//{
	//	dprintf(0, "[MAIN]Error U_set_logdir() \n");
	//	exit(-1);
	//}

	if( U_set_queue() )
	{
		dprintf(0, "[MAIN]Error U_set_queue() \n");
		exit(-1);
	}

	if( M_set_sock() )
	{
		dprintf(0, "[MAIN]Error M_set_sock() \n");
		exit(-1);
	}

	if( P_channel_init() )
	{
		dprintf(0, "[MAIN]Error P_channel_init() \n");
		exit(-1);
	}

	if( R_channel_init() )
	{
		dprintf(0, "[MAIN]Error R_channel_init() \n");
		exit(-1);
	}

	if( S_channel_init() )
	{
		dprintf(0, "[MAIN]Error S_channel_init() \n");
		exit(-1);
	}

	if( SP_init() )
	{
		dprintf(0, "[MAIN]Error SP_init() \n");
		exit(-1);
	}

	if( CreateSines() )
	{
		dprintf(0, "[MAIN]Error CreateSines() \n");
		exit(-1);
	}


	dprintf(0, "\t==================================================== \n\n\n\n");
	dprintf(0, "[MAIN]AI Server Start !! \n\n\n");


	G_daemon = 0;

	while( (opt = getopt(argc, argv, "d")) != EOF )
	{
		switch( opt )
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

	M_pid		= getpid();


        signal(SIGPIPE, SIG_IGN);

        segv_act.sa_handler     = M_signal_segv;
        sigfillset(&segv_act.sa_mask);
        segv_act.sa_flags       = SA_RESTART;
        sigaction(SIGSEGV, &segv_act, NULL);

	fpe_act.sa_handler	= M_signal_fpe;
	sigfillset(&fpe_act.sa_mask);
	fpe_act.sa_flags	= SA_RESTART;
	sigaction(SIGFPE, &fpe_act, NULL);
	
        sigfillset(&blockmask);
        sigdelset(&blockmask, SIGSEGV);
        sigdelset(&blockmask, SIGFPE);

        if( pthread_sigmask(SIG_BLOCK, &blockmask, NULL) )
	{
		dprintf(0, "pthread_sigmask() : %s \n", strerror(errno) );
		exit(-1);
	}


	G_run	= 1;
	
	if( M_thread_init() )
	{
		dprintf(0, "Error M_thread_init() \n");
		exit(-1);
	}

        sigemptyset(&waitmask);
        sigaddset(&waitmask, SIGINT);
        sigaddset(&waitmask, SIGTERM);
        sigwait(&waitmask, &signo);

	G_run	= 0;

	msg_type = INTER_EXIT_THREAD;
	send( G_inner_close_fd[0], (char *)&msg_type, SIZEOF_INT, 0 );

	while( (--M_tids_idx) > -1 )
		pthread_join( M_tids[M_tids_idx], NULL );

	M_server_release();

	dprintf(0, "[MAIN]AI Server Stop !! \n");
	dprintf(0, "=======================================================\n\n\n\n");


	return (0);
}


static void	M_server_release()
{
	dprintf(0, "[MAIN]M_server_release() \n");

	UnSines();
}


static int	M_server_daemon()
{
	pid_t	pid;
	int	fd;

	if( (pid = U_pro_fork()) )
		exit(-1);

	setsid();
	signal( SIGHUP, SIG_IGN );

	if( (pid = U_pro_fork()) )
		exit(-1);

	fd = open( "/dev/null", O_RDWR | O_CREAT | O_TRUNC, 0 );

	dup2( fd, 0 );
	dup2( fd, 1 );
	dup2( fd, 2 );

	close( fd );

	return (0);
}


static void	M_signal_segv( int sig )
{
	signal( SIGSEGV, SIG_IGN );

	dprintf(0, "[MAIN]recv SIGSEGV \n");
	M_server_release();

	signal( SIGSEGV, SIG_DFL );
}


static void	M_signal_fpe( int sig )
{
	signal(SIGFPE, SIG_IGN);
	
	dprintf(0, "[MAIN]recv SIGFPE \n");
	M_server_release();
	
	signal(SIGFPE, SIG_DFL);
}


static int	M_thread_init()
{
	int	i;


	M_tids_idx = 0;


	// TickThread()
	if( pthread_create(&M_tids[M_tids_idx++], NULL, T_pro_main, (void*)M_tids_idx) )
	{
		dprintf(0, "[MAIN]Error pthread_create() : %s \n", strerror(errno) );
		return (-1);
	}

	// SendThread()
	if( pthread_create(&M_tids[M_tids_idx++], NULL, S_pro_main, (void*)M_tids_idx) )
	{
		dprintf(0, "[MAIN]Error pthread_create() : %s \n", strerror(errno) );
		return (-1);
	}

	// ProcThread()
	for( i=1; i<=G_proc_thr_cnt; i++ )
	{
		if( pthread_create(&M_tids[M_tids_idx++], NULL, P_pro_main, (void*)&G_proc_thr_args[i]) )
		{
			dprintf(0, "[MAIN]Error pthread_create() : %s \n", strerror(errno) );
			return (-1);
		}
	}

	// RecvThread()
	if( pthread_create(&M_tids[M_tids_idx++], NULL, R_pro_main, (void*)M_tids_idx) )
	{
		dprintf(0, "[MAIN]Error pthread_create() : %s \n", strerror(errno) );
		return (-1);
	}


	return (0);
}


static int	M_set_sock()
{
	struct epoll_event 	event;


	// epoll_create()
	if( (G_Input_fds = epoll_create(1024)) < 0 )
	{
		dprintf(0, "[MAIN]Error epoll_create() : %s \n", strerror(errno) );
		return (-1);
	}

	// close_fd
	if( socketpair(AF_LOCAL, SOCK_STREAM, 0, G_inner_close_fd) )
	{
		dprintf(0, "[MAIN]Error socketpair() : %s \n", strerror(errno) );
		return (-1);
	}

	event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
	event.data.fd	= G_inner_close_fd[1];
	if( epoll_ctl(G_Input_fds, EPOLL_CTL_ADD, G_inner_close_fd[1], &event ) < 0 )
	{
		dprintf(0, "[MAIN]Error epoll_ctl() : %s \n", strerror(errno) );
		return (-1);
	}

	// G_inner_send_fd
	if( socketpair(AF_LOCAL, SOCK_STREAM, 0, G_inner_send_fd) )
	{
		dprintf(0, "[MAIN]Error socketpair() : %s \n", strerror(errno) );
		return (-1);
	}

	event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
	event.data.fd	= G_inner_send_fd[1];
	if( epoll_ctl(G_Input_fds, EPOLL_CTL_ADD, G_inner_send_fd[1], &event ) < 0 )
	{
		dprintf(0, "[MAIN]Error epoll_ctl() : %s \n", strerror(errno) );
		return (-1);
	}

	// G_inner_check_fd
	if( socketpair(AF_LOCAL, SOCK_STREAM, 0, G_inner_check_fd) )
	{
		dprintf(0, "[MAIN]Error socketpair() : %s \n", strerror(errno) );
		return (-1);
	}

	event.events	= EPOLLIN|EPOLLERR|EPOLLHUP;
	event.data.fd	= G_inner_check_fd[1];
	if( epoll_ctl(G_Input_fds, EPOLL_CTL_ADD, G_inner_check_fd[1], &event ) < 0 )
	{
		dprintf(0, "[MAIN]Error epoll_ctl() : %s \n", strerror(errno) );
		return (-1);
	}

	dprintf(0, "\t[MAIN]Success M_set_sock() \n");

	return (0);
}


static inline int	M_set_channel( void )
{
	int		thr, ch, ins_thr, ins_start_thr, ins_start_channel;


	// normal_channel
	if( (NORMAL_CHANNEL_CNT % PROC_CHARGE_NORMAL_CHANNEL) )
		G_proc_thr_cnt	= ((NORMAL_CHANNEL_CNT/PROC_CHARGE_NORMAL_CHANNEL) + 1);
	else
		G_proc_thr_cnt	= (NORMAL_CHANNEL_CNT/PROC_CHARGE_NORMAL_CHANNEL);
	
	if( (INS_CHANNEL_CNT % PROC_CHARGE_INS_CHANNEL) )
		ins_thr		= ((INS_CHANNEL_CNT/PROC_CHARGE_INS_CHANNEL) + 1);
	else
		ins_thr		= (INS_CHANNEL_CNT/PROC_CHARGE_INS_CHANNEL);

	// ins_channel
	ins_start_thr		= G_proc_thr_cnt + 1;
	ins_start_channel	= NORMAL_CHANNEL_CNT + 1;
	G_proc_thr_cnt		+= ins_thr;

	// calloc()
	if( (G_proc_thr_args = calloc((G_proc_thr_cnt + 1), sizeof(struct thread_args))) == NULL )
	{
		dprintf(0, "[MAIN]Error M_thread_init() caloloc() : %s \n", strerror(errno) );
		return (-1);
	}

	// set charge channel
	for( thr=1; thr<=G_proc_thr_cnt; thr++ )
	{
		// set charge normal_channel
		if( thr < ins_start_thr )
		{
			G_proc_thr_args[thr].tidx			= (thr);
			G_proc_thr_args[thr].charge_start_channel	= 1 + ((thr - 1) * PROC_CHARGE_NORMAL_CHANNEL);
			G_proc_thr_args[thr].charge_end_channel		= G_proc_thr_args[thr].charge_start_channel 
									+ (PROC_CHARGE_NORMAL_CHANNEL-1);
	
			if( G_proc_thr_args[thr].charge_end_channel > NORMAL_CHANNEL_CNT )
				G_proc_thr_args[thr].charge_end_channel	= NORMAL_CHANNEL_CNT;

			for( ch=G_proc_thr_args[thr].charge_start_channel; ch<=G_proc_thr_args[thr].charge_end_channel; ch++ )
			{
				G_channel_proc_thr[ch]	= (thr);
				G_channel_ins[ch]	= 0;
			}

			//dprintf(0, "\t[MAIN] normal_channel proc_thr(%d), start(%d), end(%d) \n"
			//	, thr, G_proc_thr_args[thr].charge_start_channel, G_proc_thr_args[thr].charge_end_channel );
		}

		// set charge ins_channel
		else
		{
			G_proc_thr_args[thr].tidx			= (thr);
			G_proc_thr_args[thr].charge_start_channel	= ins_start_channel
									+ ((thr-ins_start_thr) * PROC_CHARGE_INS_CHANNEL);
			G_proc_thr_args[thr].charge_end_channel		= G_proc_thr_args[thr].charge_start_channel
									+ (PROC_CHARGE_INS_CHANNEL-1);

			if( G_proc_thr_args[thr].charge_end_channel > TOTAL_CHANNEL_CNT )
				G_proc_thr_args[thr].charge_end_channel	= TOTAL_CHANNEL_CNT;

			for( ch=G_proc_thr_args[thr].charge_start_channel; ch<=G_proc_thr_args[thr].charge_end_channel; ch++ )
			{
				G_channel_proc_thr[ch]	= (thr);
				G_channel_ins[ch]	= 1;
			}

			//dprintf(0, "\t[MAIN] ins_channel proc_thr(%d), start(%d), end(%d) \n"
			//	, thr, G_proc_thr_args[thr].charge_start_channel, G_proc_thr_args[thr].charge_end_channel );
		}
	}

	return (0);
}
