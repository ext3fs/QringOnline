/*____________________________________________________________

	Project name : Qring Online
	File name    : wrap.c
	Description  : wrap function
	Date         : February 14 2005 
_____________________________________________________________*/

#include	"global.h"
#include	"util.h"
#define	_WRAP_H_
#include	"wrap.h"



pid_t	W_Fork(void)
{
	pid_t pid;


	if( (pid = fork()) == -1 )
	{
		U_Printf("[WRAP]Error fork() : %s\n", strerror(errno));
		exit(-1);
	}
		
	return(pid);
}


Sigfunc	*W_Signal(int signo, Sigfunc *func) 
{
	Sigfunc *sigfunc;


	if( (sigfunc = signal(signo, func)) == SIG_ERR )
	{
		U_Printf("[WRAP]Error signal() : %s\n", strerror(errno));
		exit(-1);
	}
	
	return(sigfunc);
}


void	W_Sigaction(int signo, const struct sigaction *act, struct sigaction *oact)
{
	if( sigaction(signo, act, oact) == -1 )
	{
		U_Printf("[WRAP]Error sigaction() : %s\n", strerror(errno));
		exit(-1);
	}	
}

void	W_Pthread_mutex_init(pthread_mutex_t *mptr, pthread_mutexattr_t *attr)
{
	int	n;


	if( (n = pthread_mutex_init(mptr, attr)) )
	{
		U_Printf("[WRAP]Error pthread_mutex_init() : %s\n",strerror(n));
		exit(-1);
	}
}


void	W_Pthread_cond_init(pthread_cond_t *cptr, pthread_condattr_t *attr)
{
	int	n;
	
	if( (n = pthread_cond_init(cptr, attr)) )
	{
		U_Printf("[WRAP]Error pthread_cond_init() : %s\n",strerror(n));
		exit(-1);
	}
}


void	W_Pthread_sigmask(int how, const sigset_t *set, sigset_t *oset)
{
	int	n;


	if( (n = pthread_sigmask(how, set, oset)) )
	{
		U_Printf("[WRAP]Error pthread_sigmask() : %s\n", strerror(n));
		exit(-1);
	}
}


void	W_Pthread_create(pthread_t *tid, const pthread_attr_t *attr, void * (*func)(void *), void *arg)
{
	int	n;


	if( (n = pthread_create(tid, attr, func, arg)) )
	{
		U_Printf("[WRAP]Error pthread_create() : %s\n", strerror(n));
		exit(-1);
	}
	
}


void	W_Pthread_join(pthread_t tid, void **status)
{
	int	n;


	if( (n = pthread_join(tid, status)) )
	{
		U_Printf("[WRAP]Error pthread_join() : %s\n", strerror(n));
		exit(-1);
	}
}


int	W_Open(const char *pathname, int oflag, mode_t mode)
{
	int		fd;


	if( (fd = open(pathname, oflag, mode)) == -1)
	{
		U_Printf("[WRAP]Error open() : %s\n", strerror(errno));
		exit(-1);
	}

	return(fd);
}


void	W_Dup2(int fd1, int fd2)
{
	if ( dup2(fd1, fd2) == -1 )
	{
		U_Printf("[WRAP]Error dup2() : %s\n", strerror(errno));
		exit(-1);
	}
}

