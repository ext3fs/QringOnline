/*____________________________________________________________

	Project name : Qring online
	File name    : proc.h
	Description  : proc module
	Date         : February 14 2005
_____________________________________________________________*/

#ifdef	G 
#undef	G 
#endif

#ifdef	_PROC_H_
#define	G
#else
#define	G	extern 
#endif

G	void *P_Proc_main(void *);


#undef	G
#undef	_PROC_H_

