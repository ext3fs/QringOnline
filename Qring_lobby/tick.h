/*____________________________________________________________

	Project name : Qring Online
	File name    : tick.h
	Description  : tick module
	Date         : February 14 2005
_____________________________________________________________*/


#ifdef	G 
#undef	G 
#endif

#ifdef	_TICK_H_
#define	G
#else
#define	G	extern 
#endif

G	void *T_Tick_main(void *);

#undef	G
#undef	_TICK_H_

