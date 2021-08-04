/*____________________________________________________________

	Project name : Qring online
	File name    : wrap.h
	Description  : wrap function
	Date         : February 14 2005 
_____________________________________________________________*/


#ifdef	G
#undef	G
#endif

#ifdef	_WRAP_H_
#define	G
#else
#define	G	extern
#endif

typedef	void	Sigfunc(int);    

G	pid_t 		W_Fork(void);
G	Sigfunc 	*W_Signal(int, Sigfunc);
G	void 		W_Sigaction(int ,const struct sigaction * ,struct sigaction *);
G	void		W_Pthread_mutex_init(pthread_mutex_t *, pthread_mutexattr_t *);
G	void		W_Pthread_cond_init(pthread_cond_t *, pthread_condattr_t *);
G	void 		W_Pthread_sigmask(int ,const sigset_t * ,sigset_t *);
G	void		W_Pthread_create(pthread_t *, const pthread_attr_t *, void * (*)(void *), void *);
G	void		W_Pthread_join(pthread_t, void **);
G	int		W_Open(const char *, int, mode_t);
G	void		W_Dup2(int, int);

#undef G
#undef _WRAP_H_
