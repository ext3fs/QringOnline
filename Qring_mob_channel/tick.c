
#include	"global.h"
#include	"util.h"
#include	"db.h"
#include	"tick.h"


static struct timeval		T_timeval;



void	*T_pro_main( void *arg )
{

	int		channel;
	double		check_fd_sec, check_recover_sec, check_live_sec;


	dprintf(0, "[TICK]Thread Start !! \n");

	T_set_localtime();

	check_fd_sec		= G_Sec;
	check_recover_sec	= G_Sec;
	check_live_sec		= G_Sec;

	while( G_run )
	{
		usleep(0);

		T_set_localtime();

		// map connection
		if( G_Sec != check_fd_sec )
		{
			for( channel=1; channel<=TOTAL_CHANNEL_CNT; channel++ )
				send(G_inner_check_fd[0], &channel, SIZEOF_INT, 0);

			check_fd_sec	= G_Sec;
		}

		// channel live
		if( (G_Sec - check_live_sec) > 5 )
		{
			for( channel=1; channel<=TOTAL_CHANNEL_CNT; channel++ )
				U_recv_queue_in( G_channel_proc_thr[channel], channel, INTER_CHANNEL_LIVE, 0, NULL );

			check_live_sec	= G_Sec;
		}

		// mob recover
		if( (G_Sec - check_recover_sec) > 1 )
		{
			for( channel=1; channel<=TOTAL_CHANNEL_CNT; channel++ )
				U_recv_queue_in( G_channel_proc_thr[channel], channel, INTER_MOB_RECOVER, 0, NULL );

			check_recover_sec	= G_Sec;
		}
	}

	dprintf(0, "[TICK]Thread End !! \n");
	return (NULL);
}


void	T_set_localtime( void )
{
	gettimeofday( &T_timeval, NULL );
	G_Sec		= (T_timeval.tv_sec);
	G_MilliSec	= ((double)T_timeval.tv_sec * 1000) + ((double)T_timeval.tv_usec / 1000);
}


int	T_dayoftime( int day_sec )
{
	struct tm *	tm_ptr;
	tm_ptr		= localtime(&T_timeval.tv_sec);	
	return (day_sec - ((tm_ptr->tm_hour * HOUR) + (tm_ptr->tm_min * MINUTE) + (tm_ptr->tm_sec)));
}
