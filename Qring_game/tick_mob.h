/*____________________________________________________________

	Project name : Qring Online
	File name    : tick_mob.h
	Description  : tick mob module
	Date         : February 14 2005
_____________________________________________________________*/


#ifdef	G 
#undef	G 
#endif

#ifdef	_TICK_MOB_H_
#define	G
#else
#define	G	extern 
#endif

G	void *TM_Tick_mob_main(void *);

#undef	G
#undef	_TICK_MOB_H_

