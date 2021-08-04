/*____________________________________________________________

	Project name : Qring online
	File name    : util.h
	Description  : utility
	Date         : February 14 2005 
_____________________________________________________________*/

#ifdef	G
#undef	G
#endif

#ifdef	_UTIL_H_
#define	G
#else
#define	G	extern
#endif


struct	recv_q
{
	unsigned	short	fd;
	unsigned	short	fd_or_seq;
	int			command;
	int			sec_active;
	int			session;
	unsigned	short	data_size;
	char			data[SIZEOF_MAX_PACKET_BODY];	
	unsigned	short	recv_cnt;
	int			next;
};

struct	send_q
{
	int			send_type;
	unsigned	short	id;
	unsigned	short	map;
	unsigned	short	block_x;
	unsigned	short	data_size;
	char			data[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_PACKET_BODY];
	int			next;
};

struct	rand_val
{
	int		val;
	int		next;
};


//_____________________________________________________________
//queue
G	void	U_Init_queue(void);
G	void	U_Recv_queue_in(const unsigned short, const unsigned short, const int, const int, const int,
			const unsigned short, const char *, const unsigned short);
G	struct recv_q	*U_Recv_queue_wait(void);
G	void	U_Recv_queue_proc_end(void);
G	void	U_Send_queue_in(const int, const unsigned short, const unsigned short, const unsigned short,
			const unsigned short, const char *);
G	struct send_q	*U_Send_queue_wait(void);
G	void	U_Send_queue_proc_end(void);

G	int	U_Curr_recv_q_command(void);
G	int	U_GetRecv_head(void);
G	int	U_GetRecv_tail(void);
G	int	U_GetSend_head(void);
G	int	U_GetSend_tail(void);


//_____________________________________________________________
//max fd
G	int	U_SetMax_fd(const int);


//_____________________________________________________________
//rand
G	void	U_Init_rand(void);
G	int	U_GetRand_val(const int);


//_____________________________________________________________
//log
G	void	U_Printf(const char *, ...);


//_____________________________________________________________
//nonblock
G 	int	U_Nonb_connect(const int, const struct sockaddr *, const int);
G 	int	U_Nonb_recv(const int, char *, const int, const int);
G	int	U_Nonb_send(const int, char *, const int, const int);

#undef	G
#undef	_UTIL_H_

