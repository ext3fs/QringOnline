/*____________________________________________________________

	Project name : Qring Online
	File name    : crypt.h
	Description  : crypt module
	Date         : February 14 2005
_____________________________________________________________*/


#ifdef	G 
#undef	G 
#endif

#ifdef	_CRYPT_H_
#define	G
#else
#define	G	extern 
#endif


//G	void	C_Billing_encrypt(unsigned char *, unsigned char *, int);
//G	void	C_Billing_decrypt(unsigned char *, unsigned char *, int);
G	void	C_Game_encrypt(unsigned char *, unsigned char *, int);
G	void	C_Game_decrypt(unsigned char *, unsigned char *, int);

#undef	G
#undef	_CRYPT_H_

