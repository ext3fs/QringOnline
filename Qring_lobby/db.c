/*____________________________________________________________

	Project name : Qring Online
	File name    : db.c
	Description  : data structure
	Date         : February 14 2005
_____________________________________________________________*/


#include	"global.h"
#include	"util.h"
#include	"mk_packet.h"
#define	_DB_H_
#include	"db.h"


struct	machine_object
{
	char			ip[SIZEOF_IP+1];
	char			outer_ip[SIZEOF_IP+1];
	char			dbagent_ip[SIZEOF_IP+1];
};

struct	world
{
	char			name[WORLD_NAME_LEN+1];

	int			machine_cnt;
	struct	machine_object	machine[MAX_MACHINE+1];
	
	int			channel_cnt;
	int			channel_user[999];
};

struct  level
{
        int	index;
	int	hp_max;
	int	ap_max;
	int	exp;
	int	att1;
	int	def1;
	int	is_def;
	int	il_def;
	int	i_att;
	int	m_def;
	int	cri;
	//float	speed_move_1;
	//float	speed_move_2;
	int	recover_hp;
	int	recover_ap;
};

struct	client
{
	int	sec_active;	
	int	session;			

	int	billing_idx;
	int	use_billing_idx;
	int	login_dbagent_idx;
	int	use_login_dbagent_idx;
	int	dbagent_idx;

	char	ip[SIZEOF_IP+1];
	char	lock_user_id[USER_ID_LEN+1];
	char	user_id[USER_ID_LEN+1];	

	int	load_world_idx;
	int	load_char_cnt;
};


static	int	D_set_myinfo(const int);
static	int	D_set_world_name(const int);
static	int	D_set_machine_ip(const int);
static	int	D_set_machine_dbagent_ip(const int);
static	int	D_set_level(const int);


static	int		D_max_world;
static	int		D_max_level;
static	struct	world	D_world[999];
static	struct	level	*D_level;
static	struct	client	D_client[MAX_FD+1];



//________________________________________________________________
//
int	D_SetDb_init(void)
{
	int				sock_fd, n, i, j, use_billing_idx, use_login_dbagent_idx;
	struct	sockaddr_in		addr;
	char				buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	struct	body_SW_INIT_RESULT	*body;

	
	//connect script  
	if( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		U_Printf("[DB]Error socket() : %s\n", strerror(errno));
    		return	(-1);
	}

	bzero(&addr, sizeof(addr));
  	addr.sin_family		= AF_INET;
  	addr.sin_port		= htons(SCRIPT_PORT);
	
	if( inet_pton(AF_INET, G_Script_ip, &addr.sin_addr) <= 0 )
	{
		U_Printf("[DB]Error inet_pton(%s)\n", G_Script_ip);
    		return	(-1);
	}

	if( U_Nonb_connect(sock_fd, (struct sockaddr *)&addr, 3) )
	{
		U_Printf("[DB]Error U_Nonb_connect()\n");
    		return	(-1);
	}


	//send	WS_INIT
	n	= MK_GetPacket_WS_INIT(buf);
	
	if( U_Nonb_send(sock_fd, buf, n, 3) )
	{
		U_Printf("[DB]Error U_Nonb_send()\n");
    		return	(-1);
	}

	//recv SW_INIT_RESULT
	if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL +  sizeof(struct body_SW_INIT_RESULT), 10) )
	{
		U_Printf("[DB]Error U_Nonb_recv\n");
    		return	(-1);
	}

	//check result_code	
	body	= (struct body_SW_INIT_RESULT *)(buf + SIZEOF_HEADER_SPECIAL);

	if( body->result_code )
	{
		U_Printf("[DB]Error bad result_code(%d)\n", body->result_code);
    		return	(-1);
	}

	//set G_Game, G_World, G_Server_code
	if( D_set_myinfo(sock_fd) )
	{
		U_Printf("[DB]Error D_set_myinfo()\n");
		return	(-1);
	}

	//set D_max_world, D_world.name
	if( D_set_world_name(sock_fd) )
	{
		U_Printf("[DB]Error D_set_world_name()\n");
		return	(-1);
	}	
	
	//set D_world.machine_cnt, D_world.channel_cnt, D_world.machine.ip, D_world.machine.outer_ip
	if( D_set_machine_ip(sock_fd) )
	{
		U_Printf("[DB]Error D_set_machine_ip()\n");
		return	(-1);
	}	

	//set D_world.machine.dbagent_ip
	if( D_set_machine_dbagent_ip(sock_fd) )
	{
		U_Printf("[DB]Error D_set_machine_dbagent_ip()\n");
		return	(-1);
	}

	//set D_max_level, D_level
	if( D_set_level(sock_fd) )
	{
		U_Printf("[DB]Error D_set_level()\n");
		return	(-1);
	}	
	
	//init D_client
	use_billing_idx		= 1;
	use_login_dbagent_idx	= 1;

	for( i=0; i<=MAX_FD; i++ )
	{
		D_client[i].use_billing_idx		= use_billing_idx;
		use_billing_idx				= ( (use_billing_idx == CONNECTION_BILLING) ? 1 : use_billing_idx+1);
		
		D_client[i].use_login_dbagent_idx	= use_login_dbagent_idx;
		use_login_dbagent_idx			= ( (use_login_dbagent_idx == CONNECTION_LOGIN_DBAGENT) ? 1 : use_login_dbagent_idx+1);
	}

	//print D_world
	for( i=1; i<=D_max_world; i++ )
	{
		U_Printf("[DB]D_world[%d] name(%s) machine_cnt(%d) channel_cnt(%d)\n",
						i, D_world[i].name, D_world[i].machine_cnt, D_world[i].channel_cnt);
		
		for( j=1; j<=D_world[i].machine_cnt; j++ )
			U_Printf("[DB] - machine(%d) ip(%s) outer_ip(%s) dbagent_ip(%s)\n",
				j, D_world[i].machine[j].ip, D_world[i].machine[j].outer_ip, D_world[i].machine[j].dbagent_ip);
	}

	close(sock_fd);
	return	(0);
} 


static	int	D_set_myinfo(const int sock_fd)
{
	char				buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	int				n;
	struct	body_SW_MYINFO_RESULT	body;
		

	//send WS_MYINFO
	n	= MK_GetPacket_WS_MYINFO(buf);

	if( U_Nonb_send(sock_fd, buf, n, 3) )
	{
		U_Printf("[DB]Error U_Nonb_send()\n");
    		return	(-1);
	}

	//recv header
	if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}

	//recv body	
	if( U_Nonb_recv(sock_fd, (char *)&body, sizeof(struct body_SW_MYINFO_RESULT), 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}
	
	//check G_Game, G_World	
	if( (body.game < 1) || (body.game >= 999) || (body.world != 999) )
	{
		U_Printf("[DB]Error bad game(%d), world(%d)\n",body.game, body.world);
    		return	(-1);
	}
	
	//set G_Game, G_World, G_Server_code
	G_Game		= body.game;
	G_World		= body.world;
	G_Server_code	= G_Game*1000000 + G_World*1000 + G_Lobby;
	
	U_Printf("[DB]complete G_Game(%d), G_World(%d), G_Server_code(%d)\n", G_Game, G_World, G_Server_code);
	return	(0);
}

static	int	D_set_world_name(const int sock_fd)
{
	char					buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	int					n, i;
	struct	body1_SW_WORLD_INFO_RESULT	body1;
	struct	body2_SW_WORLD_INFO_RESULT	body2;


	//send WS_WORLD_INFO
	n	= MK_GetPacket_WS_WORLD_INFO(buf, G_Game);

	if( U_Nonb_send(sock_fd, buf, n, 3) )
	{
		U_Printf("[DB]Error U_Nonb_send()\n");
    		return	(-1);
	}

	//recv header
	if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}

	//recv body1	
	if( U_Nonb_recv(sock_fd, (char *)&body1, sizeof(struct body1_SW_WORLD_INFO_RESULT), 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}
	
	//check info_cnt
	if( (body1.info_cnt < 1) || (body1.info_cnt >= 999) )
	{
		U_Printf("[DB]Error bad info_cnt(%d)\n", body1.info_cnt);
    		return	(-1);
	}
		
	//set D_max_world
	D_max_world	= body1.info_cnt;

	//recv info	
	for( i=1; i<=body1.info_cnt; i++ )
	{
		//recv body2
		if( U_Nonb_recv(sock_fd, (char *)&body2, sizeof(struct body2_SW_WORLD_INFO_RESULT), 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
    			return	(-1);
		}

		//check body2.world
		if( (body2.world < 1) || (body2.world > D_max_world) )
		{
			U_Printf("[DB]Error bad world(%d)\n", body2.world);
    			return	(-1);
		}

		//check body2.name
		if( (body2.name[0] == 0) || (body2.name[WORLD_NAME_LEN] != 0) )
		{
			body2.name[WORLD_NAME_LEN]	= 0;
			U_Printf("[DB]Error world(%d) bad name(%s)\n", body2.world, body2.name);
    			return	(-1);
		}
	
		//check D_world.name	
		if( D_world[body2.world].name[0] )
		{
			U_Printf("[DB]Error world(%d) name(%s) dup name(%s)\n", body2.world, D_world[body2.world].name, body2.name);
    			return	(-1);
		}	
		
		//set D_world.name
		strncpy(D_world[body2.world].name, body2.name, WORLD_NAME_LEN+1);
	}

	U_Printf("[DB]complete D_max_world(%d)\n", D_max_world);
	return	(0);
}


static	int	D_set_machine_ip(const int sock_fd)
{
	char					buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	int					n, i, j;
	struct	body1_SW_GAMESERV_INFO_RESULT	body1;
	struct	body2_SW_GAMESERV_INFO_RESULT	body2;
		

	for( i=1; i<=D_max_world; i++ )
	{
		//send WS_GAMESERV_INFO
		n	= MK_GetPacket_WS_GAMESERV_INFO(buf, G_Game, i);

		if( U_Nonb_send(sock_fd, buf, n, 3) )
		{
			U_Printf("[DB]Error U_Nonb_send()\n");
    			return	(-1);
		}

		//recv header
		if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
    			return	(-1);
		}

		//recv body1	
		if( U_Nonb_recv(sock_fd, (char *)&body1, sizeof(struct body1_SW_GAMESERV_INFO_RESULT), 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
    			return	(-1);
		}
	
		//check info_cnt
		if( (body1.info_cnt < 1) || (body1.info_cnt > MAX_MACHINE) )
		{
			U_Printf("[DB]Error MAX_MACHINE(%d) bad info_cnt(%d)\n", MAX_MACHINE, body1.info_cnt);
    			return	(-1);
		}
		
		//set D_world.machine_cnt 
		D_world[i].machine_cnt	= body1.info_cnt;
		
		//set D_world.channel_cnt 
		D_world[i].channel_cnt	= CHANNEL_PER_MACHINE*D_world[i].machine_cnt;
		
		for( j=1; j<=body1.info_cnt; j++ )
		{
			//recv body2
			if( U_Nonb_recv(sock_fd, (char *)&body2, sizeof(struct body2_SW_GAMESERV_INFO_RESULT), 3) )
			{
				U_Printf("[DB]Error U_Nonb_recv()\n");
	    			return	(-1);
			}
			
			//check ip
			if( (body2.ip[0] == 0) || (body2.ip[SIZEOF_IP] != 0) )
			{
				body2.ip[SIZEOF_IP]	= 0;
				U_Printf("[DB]Error world(%d) machine(%d) bad ip(%s)\n", i, j, body2.ip);
	    			return	(-1);
			}
			
			//check outer_ip
			if( (body2.outer_ip[0] == 0) || (body2.outer_ip[SIZEOF_IP] != 0) )
			{
				body2.outer_ip[SIZEOF_IP]	= 0;
				U_Printf("[DB]Error world(%d) machine(%d) bad outer_ip(%s)\n", i, j, body2.outer_ip);
	    			return	(-1);
			}

			//set D_world.machine.ip
			strncpy(D_world[i].machine[j].ip, body2.ip, SIZEOF_IP+1);
			
			//set D_world.machine.outer_ip
			strncpy(D_world[i].machine[j].outer_ip, body2.outer_ip, SIZEOF_IP+1);
		}
	}

	U_Printf("[DB]complete machine ip\n");
	return	(0);
}


static	int	D_set_machine_dbagent_ip(const int sock_fd)
{
	char					buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	int					n, i, j;
	struct	body1_SW_DBAGENT_INFO_RESULT	body1;
	struct	body2_SW_DBAGENT_INFO_RESULT	body2;
	
	
	for( i=1; i<=D_max_world; i++ )
	{
		//send WS_DBAGENT_INFO
		n	= MK_GetPacket_WS_DBAGENT_INFO(buf, G_Game, i);

		if( U_Nonb_send(sock_fd, buf, n, 3) )
		{
			U_Printf("[DB]Error U_Nonb_send()\n");
    			return	(-1);
		}

		//recv header
		if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
    			return	(-1);
		}

		//recv body1	
		if( U_Nonb_recv(sock_fd, (char *)&body1, sizeof(struct body1_SW_DBAGENT_INFO_RESULT), 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
    			return	(-1);
		}
	
		//check info_cnt
		if( body1.info_cnt != D_world[i].machine_cnt )
		{
			U_Printf("[DB]Error world(%d) machine_cnt(%d) bad info_cnt(%d)\n", i, D_world[i].machine_cnt, body1.info_cnt);
    			return	(-1);
		}
		
		for( j=1; j<=body1.info_cnt; j++ )
		{
			//recv body2
			if( U_Nonb_recv(sock_fd, (char *)&body2, sizeof(struct body2_SW_DBAGENT_INFO_RESULT), 3) )
			{
				U_Printf("[DB]Error U_Nonb_recv()\n");
    				return	(-1);
			}
			
			//check ip
			if( (body2.ip[0] == 0) || (body2.ip[SIZEOF_IP] != 0) )
			{
				body2.ip[SIZEOF_IP]	= 0;
				U_Printf("[DB]Error world(%d) machine(%d) bad ip(%s)\n", i, j, body2.ip);
	    			return	(-1);
			}
		
			//set D_world.machine.dbagent_ip
			strncpy(D_world[i].machine[j].dbagent_ip, body2.ip, SIZEOF_IP+1);
		}
	}

	U_Printf("[DB]complete machine dbagent_ip\n");
	return	(0);
}

static	int	D_set_level(const int sock_fd)
{
	int					n, i, j, line, row, column;
	char					buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_SERV_PACKET_BODY];
	char					*row_ptr, *column_ptr, *file[MAX_SCRIPT_FILE_CNT+1];
	struct	body1_SW_LEVEL_FILES_RESULT	body1;
	struct	body2_SW_LEVEL_FILES_RESULT	body2;


	//send WS_LEVEL_FILES
	n	= MK_GetPacket_WS_LEVEL_FILES(buf);

	if( U_Nonb_send(sock_fd, buf, n, 3) )
	{
		U_Printf("[DB]Error U_Nonb_send()\n");
    		return	(-1);
	}

	//recv header
	if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}
 
	//recv body1	
	if( U_Nonb_recv(sock_fd, (char *)&body1, sizeof(struct body1_SW_LEVEL_FILES_RESULT), 3) )
	{
		U_Printf("[DB]Error U_Nonb_recv()\n");
    		return	(-1);
	}

	//check file_cnt
	if( (body1.file_cnt < 1) || (body1.file_cnt > MAX_SCRIPT_FILE_CNT) )
	{
		U_Printf("[DB]Error bad file_cnt(%d)\n", body1.file_cnt);
    		return	(-1);
	}

	//recv files
	for( i=1; i<=body1.file_cnt; i++ )
	{
		//recv file_size
		if( U_Nonb_recv(sock_fd, (char *)(&body2), sizeof(struct body2_SW_LEVEL_FILES_RESULT), 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
   	 		return	(-1);
		}

		//check file_size
		if( body2.file_size < 1 )
		{
			U_Printf("[DB]Error bad file_size(%d)\n", body2.file_size);
    			return	(-1);
		}
		
		//alloc file
		if( (file[i] = calloc(1, body2.file_size)) == NULL )
		{
			U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    			return	(-1);
		}
		
		//recv file 
		if( U_Nonb_recv(sock_fd, file[i], body2.file_size, 3) )
		{
			U_Printf("[DB]Error U_Nonb_recv()\n");
   	 		return	(-1);
		}

		//count line
		line	= 0;

		for( j=0; j<body2.file_size; j++ )
			if( file[i][j] == '\n' )
				++line;

		if( file[i][body2.file_size-1] == '\n' )
		{
			--line;
			file[i][body2.file_size-1] = 0; 
		}
	
		//check line
		if( line < 1 )
		{
			U_Printf("[DB]Error file(%d) bad line(%d)\n", i, line);
    			return	(-1);
		}

		//set D_max_level
		D_max_level	+= line;
	
		//check D_max_level
		if( D_max_level < 1 )
		{
			U_Printf("[DB]Error file(%d) bad D_max_level(%d)\n", i, D_max_level);
    			return	(-1);
		}	
	}
		
	//alloc D_level
	if( (D_level = calloc(D_max_level+1, sizeof(struct level))) == NULL )
	{
		U_Printf("[DB]Error calloc() : %s\n", strerror(errno));
    		return	(-1);
	}

	//load D_level
	row	= 0;

	for( i=1; i<=body1.file_cnt; i++ )
	{
		//first line
		row_ptr	= strtok(file[i], "\n");

		//other line
		line	= 0;
		
		while( (row_ptr = strtok(NULL, "\n")) )
		{
			++row;
			++line;
			column	= 0;
		
			//set column
			while( (column_ptr = strsep(&row_ptr, "\t")) )
			{
				++column;

				switch( column ) 
				{
					case	1:	
						if( (D_level[row].index	= strtol(column_ptr, NULL, 10)) != row )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad index(%d)\n", i, line, D_level[row].index);
							return	(-1);
						}
						break;
					case	2:
						if( (D_level[row].hp_max = strtol(column_ptr, NULL, 10)) < 1 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad hp_max(%d)\n", i, line, D_level[row].hp_max);
							return	(-1);
						}
						break;
					case	3:
						if( (D_level[row].ap_max = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad ap_max(%d)\n", i, line, D_level[row].ap_max);
							return	(-1);
						}
						break;
					case	4:
						if( (D_level[row].exp = strtol(column_ptr, NULL, 10)) < 1 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad exp(%d)\n", i, line, D_level[row].exp);
							return	(-1);
						}
						break;
					case	5:
						if( (D_level[row].att1 = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad att1(%d)\n", i, line, D_level[row].att1);
							return	(-1);
						}
						break;
					case	6:
						if( (D_level[row].def1 = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad def1(%d)\n", i, line, D_level[row].def1);
							return	(-1);
						}
						break;
					case	7:
						if( (D_level[row].is_def = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad is_def(%d)\n", i, line, D_level[row].is_def);
							return	(-1);
						}
						break;
					case	8:
						if( (D_level[row].il_def = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad il_def(%d)\n", i, line, D_level[row].il_def);
							return	(-1);
						}
						break;
					case	9:
						if( (D_level[row].i_att = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad i_att(%d)\n", i, line, D_level[row].i_att);
							return	(-1);
						}
						break;
					case	10:
						if( (D_level[row].m_def = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad m_def(%d)\n", i, line, D_level[row].m_def);
							return	(-1);
						}
						break;
					case	11:
						if( (D_level[row].cri = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad cri(%d)\n", i, line, D_level[row].cri);
							return	(-1);
						}
						break;
					case	12:
					case	13:
						break;
					case	14:
						if( (D_level[row].recover_hp = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad recover_hp(%d)\n",
									i, line, D_level[row].recover_hp);
							return	(-1);
						}
						break;
					case	15:
						if( (D_level[row].recover_ap = strtol(column_ptr, NULL, 10)) < 0 )
						{
							U_Printf("[DB]Error file(%d) line(%d) bad recover_ap(%d)\n",
									i, line, D_level[row].recover_ap);
							return	(-1);
						}
						break;
					default	:	
						U_Printf("[DB]Error file(%d) line(%d) bad column(%d)\n", i, line, column);	
						return	(-1);
				}
			}

			//check column
			if( column != 15 )
			{
				U_Printf("[DB]Error file(%d) line(%d) bad column(%d)\n", i, line, column);
				return	(-1);
			}
		
		}//end while
	}//end for

	//free files
	for( i=1; i<=body1.file_cnt; i++ )
		free(file[i]);

	U_Printf("[DB]complete D_max_level(%d)\n", D_max_level);
	return	(0);
}



//______________________________________________________________
//
int	D_GetMax_world(void)
{
	return	(D_max_world);
}


char	*D_GetWorld_name(const int world)
{
	return	(D_world[world].name);
}


int	D_GetWorld_channel_cnt(const int world)
{
	return	(D_world[world].channel_cnt);
}


int	D_GetWorld_channel_user(const int world, const int channel)
{
	return	(D_world[world].channel_user[channel]);
}	


void	D_SetWorld_channel_user(const int world, const int channel, const int user)
{
	D_world[world].channel_user[channel]	= user;
}


char	*D_GetWorld_machine_outer_ip(const int world, const int machine)
{
	return	(D_world[world].machine[machine].outer_ip);
}	

	
char	*D_GetWorld_machine_dbagent_ip(const int world, const int machine)
{
	return	(D_world[world].machine[machine].dbagent_ip);
}


int	D_GetMax_level(void)
{
	return	(D_max_level);
}  


int	D_GetLevel_hp_max(const int level)
{
	return	(D_level[level].hp_max);
}


int	D_GetLevel_ap_max(const int level)
{
	return	(D_level[level].ap_max);
}


	
//______________________________________________________________
//
int	D_GetClient_sec_active(const int fd)
{
	return	(D_client[fd].sec_active);
}


void	D_SetClient_sec_active(const int fd, const int value)
{
	D_client[fd].sec_active		= value;
}


int	D_GetClient_session(const int fd)
{
	return	(D_client[fd].session);
}


void	D_SetClient_session(const int fd, const int value)
{
	D_client[fd].session		= value;
}


int	D_GetClient_billing_idx(const int fd) 
{
	return	(D_client[fd].billing_idx);
}


void	D_SetClient_billing_idx(const int fd, const int value)
{
	D_client[fd].billing_idx	= value;
}


int	D_GetClient_use_billing_idx(const int fd) 
{
	return	(D_client[fd].use_billing_idx);
}


int	D_GetClient_login_dbagent_idx(const int fd) 
{
	return	(D_client[fd].login_dbagent_idx);
}


void	D_SetClient_login_dbagent_idx(const int fd, const int value)
{
	D_client[fd].login_dbagent_idx	= value;
}


int	D_GetClient_use_login_dbagent_idx(const int fd) 
{
	return	(D_client[fd].use_login_dbagent_idx);
}


int	D_GetClient_dbagent_idx(const int fd) 
{
	return	(D_client[fd].dbagent_idx);
}


void	D_SetClient_dbagent_idx(const int fd, const int value)
{
	D_client[fd].dbagent_idx	= value;
}


char	*D_GetClient_ip(const int fd)
{
	return	(D_client[fd].ip);
}


void	D_SetClient_ip(const int fd, const char *string)
{
	strncpy(D_client[fd].ip, string, SIZEOF_IP+1);	
}


char	*D_GetClient_lock_user_id(const int fd)
{
	return	(D_client[fd].lock_user_id);
}


void	D_SetClient_lock_user_id(const int fd, const char *string)
{
	strncpy(D_client[fd].lock_user_id, string, USER_ID_LEN+1);
}


char	*D_GetClient_user_id(const int fd) 
{
	return	(D_client[fd].user_id);
}


void	D_SetClient_user_id(const int fd, const char *string)
{
	strncpy(D_client[fd].user_id, string, USER_ID_LEN+1);
}


int	D_GetClient_load_world_idx(const int fd) 
{
	return	(D_client[fd].load_world_idx);
}


void	D_SetClient_load_world_idx(const int fd, const int value)
{
	D_client[fd].load_world_idx	= value;
}


int	D_GetClient_load_char_cnt(const int fd) 
{
	return	(D_client[fd].load_char_cnt);
}


void	D_SetClient_load_char_cnt(const int fd, const int value)
{
	D_client[fd].load_char_cnt	= value;
}


void	D_SetClient_clear(const int fd)
{
	D_client[fd].sec_active		= 0;
	D_client[fd].billing_idx	= 0;
	D_client[fd].login_dbagent_idx	= 0;
	D_client[fd].dbagent_idx	= 0;
	D_client[fd].lock_user_id[0]	= 0;
	D_client[fd].user_id[0]		= 0;
	D_client[fd].load_world_idx	= 0;
}
