/*____________________________________________________________

	Project name : Qring online
	File name    : db.c
	Description  : data structure
	Date         : February 14 2005
_____________________________________________________________*/


#include	"global.h"
#include	"util.h"
#include	"mk_packet.h"
#define	_DB_H_
#include	"db.h"

struct  dbagent
{
	char                    ip[SIZEOF_IP + 1];
	int			port;
};

struct	friend
{
	char			char_name[CHAR_NAME_LEN + 1];
};

struct	client
{
	int			sec_active;	
	int			session;			

	char			char_name[CHAR_NAME_LEN + 1];	

	struct friend		friend_list[MAX_FRIEND_CNT];
	int			friend_cnt;
};


static	int			D_set_myinfo(const int);
static  int     		D_set_db(void);


static  struct  dbagent		D_dbagent;
static	struct	client		D_client[MAX_FD];



//________________________________________________________________
//
int     D_SetDb_init(void)
{
        int                             sock_fd, n;
        struct  sockaddr_in             addr;
        char                            buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_PACKET_BODY];
        struct  body_SW_INIT_RESULT     *body;


	//set db_info
	if( D_set_db() )
	{
                U_Printf("[DB]Error D_set_db() \n");
                return  (-1);
	}


        //connect script
        if( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
        {
                U_Printf("[DB]Error socket() : %s\n", strerror(errno));
                return  (-1);
        }

        bzero(&addr, sizeof(addr));
        addr.sin_family         = AF_INET;
        addr.sin_port           = htons(SCRIPT_PORT);

        if( inet_pton(AF_INET, G_Script_ip, &addr.sin_addr) <= 0 )
        {
                U_Printf("[DB]Error inet_pton(%s)\n", G_Script_ip);
                return  (-1);
        }

        if( U_Nonb_connect(sock_fd, (struct sockaddr *)&addr, 3) )
        {
                U_Printf("[DB]Error U_Nonb_connect()\n");
                return  (-1);
        }


        //send  WS_INIT
        n       = MK_GetPacket_WS_INIT(buf);

        if( U_Nonb_send(sock_fd, buf, n, 3) )
        {
                U_Printf("[DB]Error U_Nonb_send()\n");
                return  (-1);
        }

        //recv SW_INIT_RESULT
        if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL +  sizeof(struct body_SW_INIT_RESULT), 10) )
        {
                U_Printf("[DB]Error U_Nonb_recv\n");
                return  (-1);
        }

        //check result_code
        body    = (struct body_SW_INIT_RESULT *)(buf + SIZEOF_HEADER_SPECIAL);

        if( body->result_code )
        {
                U_Printf("[DB]Error bad result_code(%d)\n", body->result_code);
                return  (-1);
        }

        //set G_Game, G_World
        if( D_set_myinfo(sock_fd) )
        {
                U_Printf("[DB]Error D_set_myinfo()\n");
                return  (-1);
        }

        close(sock_fd);
        return  (0);
}


static  int     D_set_myinfo(const int sock_fd)
{
        char                            buf[SIZEOF_HEADER_SPECIAL + SIZEOF_MAX_PACKET_BODY];
        int                             n;
        struct  body_SW_MYINFO_RESULT   body;


        //send WS_MYINFO
        n       = MK_GetPacket_WS_MYINFO(buf);

        if( U_Nonb_send(sock_fd, buf, n, 3) )
        {
                U_Printf("[DB]Error U_Nonb_send()\n");
                return  (-1);
        }

        //recv header
        if( U_Nonb_recv(sock_fd, buf, SIZEOF_HEADER_SPECIAL, 3) )
        {
                U_Printf("[DB]Error U_Nonb_recv()\n");
                return  (-1);
        }

        //recv body
        if( U_Nonb_recv(sock_fd, (char *)&body, sizeof(struct body_SW_MYINFO_RESULT), 3) )
        {
                U_Printf("[DB]Error U_Nonb_recv()\n");
                return  (-1);
        }

        //check G_Game, G_World
        if( (body.game < 1) || (body.game > 999) || (body.world < 1) || (body.world > 999) )
        {
                U_Printf("[DB]Error bad game(%d), world(%d)\n", body.game, body.world);
                return  (-1);
        }

        //set G_Game, G_World 
        G_Game          = body.game;
        G_World         = body.world;

        U_Printf("[DB]complete G_Game(%d), G_World(%d)\n", G_Game, G_World);
        return  (0);
}


static  int     D_set_db(void)
{
}




//______________________________________________________________
//
int	D_GetClient_sec_active(const unsigned short fd)
{
	return	(D_client[fd].sec_active);
}

void	D_SetClient_sec_active(const unsigned short fd, const int value)
{
	D_client[fd].sec_active		= value;
}

int	D_GetClient_session(const unsigned short fd)
{
	return	(D_client[fd].session);
}

void	D_SetClient_session(const unsigned short fd, const int value)
{
	D_client[fd].session		= value;
}

char    *D_GetClient_char_name(const unsigned short fd)
{
        return  (D_client[fd].char_name);
}

void    D_SetClient_char_name(const unsigned short fd, const char *string)
{
        strncpy(D_client[fd].char_name, string, USER_ID_LEN);
}

void	D_SetClient_clear(const unsigned short fd)
{
	D_client[fd].sec_active		= 0;
	D_client[fd].char_name[0]	= 0;

	D_client[fd].friend_cnt		= 0;
}
