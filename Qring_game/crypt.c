/*____________________________________________________________

	Project name : Qring Online
	File name    : crypt.c
	Description  : crypt module
	Date         : February 14 2005
_____________________________________________________________*/


#include	"global.h"
#define	_CRYPT_H_
#include	"crypt.h"


//static	int	C_billing_c1	= 52845;
//static	int	C_billing_c2	= 22719;
//static	int	C_billing_key	= 72957;

static	int	C_game_c1	= 3452845;
static	int	C_game_c2	= 2822719;
static	int	C_game_key	= 6372957;

/*
//__________________________________________________________
//billing
void	C_Billing_encrypt(unsigned char *sour, unsigned char *dest, int len)
{
	int	i, key;
	

	key	= C_billing_key;	

	for( i=0; i<len; i++ )
	{
		dest[i]	= sour[i]^key>>8;
		key	= (dest[i] + key)*C_billing_c1 + C_billing_c2;
	}
}


void	C_Billing_decrypt(unsigned char *sour, unsigned char *dest, int len)
{
	int			i, key;
	unsigned	char	prev;

	
	key	= C_billing_key;	

	for( i=0; i<len; i++ )
	{
		prev	= sour[i];
		dest[i]	= sour[i]^key>>8;
		key	= (prev + key)*C_billing_c1 + C_billing_c2;
	}
}
*/

//__________________________________________________________
//game
void	C_Game_encrypt(unsigned char *sour, unsigned char *dest, int len)
{
	int	i, key;


	key	= C_game_key;

	for( i=(len/4); i>0; i-- )
	{
		*((int *)dest + i)	= *((int *)sour + i) ^ key;
		key			= (*((int *)dest + i) + key)*C_game_c1 + C_game_c2;
	}

	for( i=(len-len/4*4); i>-1; i-- )
	{
		dest[i]	= sour[i]^key>>8;
		key	= (dest[i] + key)*C_game_c1 + C_game_c2;
	}
}

void	C_Game_decrypt(unsigned char *sour, unsigned char *dest, int len)
{
	int			i, key, prev;
	unsigned	char	char_prev;

	
	key	= C_game_key;

	for( i=(len/4); i>0; i-- )
	{
		prev			= *((int *)sour + i);
		*((int *)dest + i)	= *((int *)sour + i) ^ key;
		key			= (prev + key)*C_game_c1 + C_game_c2;
	}

	for( i=(len-len/4*4); i>-1; i-- )
	{
		char_prev	= sour[i];
		dest[i]		= sour[i]^key>>8;
		key		= (char_prev + key)*C_game_c1 + C_game_c2;
	}
}
