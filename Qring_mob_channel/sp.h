

#ifndef __SHORTEST_PATH_H__
#define __SHORTEST_PATH_H__



#ifdef __cplusplus
extern "C" {
#endif

int	SP_init				( void );
int 	SP_to_x				( const int );
int 	SP_to_z				( const int );
double	SP_delay			( const int );
int	SP_history_cnt			( const int );
void	SP_GetPath_movevalue		( const int, const int, const float, float *, float *, int * );
int	SP_Find_Path			( const int, const int, const int, const int, const int, const int, const int, const int );

int	SP_round			( double, double );

#ifdef __cplusplus
}
#endif



#endif


