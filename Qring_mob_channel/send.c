
#include	"global.h"
#include	"util.h"
#include	"db.h"
#include	"send.h"


struct channel
{
	struct connection	connect;

	pthread_mutex_t		send_lock;
	char			send_buf[SIZEOF_SNDBUF];
	volatile int		send_pos;
};




// ----------------------------------------------------------------------------
// send	variable
static struct channel		*S_channel;





// ----------------------------------------------------------------------------
// send function 
static inline void	S_channel_send		( const int );






void	*S_pro_main( void *arg )
{
	int		i;


	dprintf(0, "[SEND]Thread Start !! \n");

	while( G_run )
	{
		usleep(0);

		for( i=1; i<=TOTAL_CHANNEL_CNT; i++ )
			S_channel_send(i);
	}

	dprintf(0, "[SEND]Thread End !! \n");
	return (NULL);
}


static inline void	S_channel_send( const int channel )
{
	int     nsend;

	pthread_mutex_lock( &S_channel[channel].send_lock );

		if( !S_channel[channel].connect.fd )
		{
			pthread_mutex_unlock( &S_channel[channel].send_lock );
			return;
		}

		if( !S_channel[channel].send_pos )
		{
			pthread_mutex_unlock( &S_channel[channel].send_lock );
			return;
		}

		nsend = send( S_channel[channel].connect.fd, S_channel[channel].send_buf, S_channel[channel].send_pos, 0 );

		if( nsend < 0 )
		{
			dprintf(0, "[SEND]Error send() : %s \n", strerror(errno) );
			S_channel[channel].send_pos		= 0;
			S_channel[channel].connect.fd		= 0;
			S_channel[channel].connect.channel	= 0;

			send( G_inner_send_fd[0], &channel, SIZEOF_INT, 0 );
			pthread_mutex_unlock( &S_channel[channel].send_lock );
			return;
		}

		memmove( S_channel[channel].send_buf, (S_channel[channel].send_buf + nsend), (S_channel[channel].send_pos - nsend) );
		S_channel[channel].send_pos -= nsend;

	pthread_mutex_unlock( &S_channel[channel].send_lock );
}


int	S_channel_init( void )
{
	int	channel;

	if( (S_channel = calloc((TOTAL_CHANNEL_CNT + 1), sizeof(struct channel))) == NULL )
	{
		dprintf(0, "[RECV]Error S_channel_init() calloc() : %s \n", strerror(errno) );
		return (-1);
	}

	for( channel=1; channel<=TOTAL_CHANNEL_CNT; channel++ )
	{
		pthread_mutex_init(&S_channel[channel].send_lock, NULL);

		S_channel_sendbuf_init( channel, 0 );
	}

	return (0);
}


void	S_channel_sendbuf_init( const int channel, const int channel_fd )
{
	pthread_mutex_lock( &S_channel[channel].send_lock );

		S_channel[channel].send_pos		= 0;

		S_channel[channel].connect.fd		= channel_fd;
		S_channel[channel].connect.channel	= channel;

	pthread_mutex_unlock( &S_channel[channel].send_lock );
}


void	S_channel_sendbuf_in( const int channel, const char *p_data, const unsigned short data_size )
{

	pthread_mutex_lock( &S_channel[channel].send_lock );

		if( !S_channel[channel].connect.fd )
		{
			pthread_mutex_unlock( &S_channel[channel].send_lock );
			return;
		}

		if( data_size > SIZEOF_MAX_PACKET_NORMAL )
		{
			dprintf(0, "[SEND]Error S_channel_sendbuf_in() send packet's size error data_size(%d) \n", data_size );
			S_channel[channel].send_pos		= 0;
			S_channel[channel].connect.fd		= 0;
			S_channel[channel].connect.channel	= 0;

			send( G_inner_send_fd[0], &channel, SIZEOF_INT, 0 );
			pthread_mutex_unlock( &S_channel[channel].send_lock );
			return;
		}

		if( (S_channel[channel].send_pos + data_size) > (SIZEOF_SNDBUF - 1) )
		{
			dprintf(0, "[SEND]Error S_channel_sendbuf_in() sendbuf full channel(%d) \n", channel );
			S_channel[channel].send_pos		= 0;
			S_channel[channel].connect.fd		= 0;
			S_channel[channel].connect.channel	= 0;

			send( G_inner_send_fd[0], &channel, SIZEOF_INT, 0 );
			pthread_mutex_unlock( &S_channel[channel].send_lock );
			return;
		}

		memcpy( (S_channel[channel].send_buf + S_channel[channel].send_pos), p_data, data_size );
		S_channel[channel].send_pos += data_size;

	pthread_mutex_unlock( &S_channel[channel].send_lock );
}
