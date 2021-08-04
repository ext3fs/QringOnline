
#ifndef __ASTAR_H__
#define __ASTAR_H__

#define	MAX_NODE_CNT		1024
#define	MAX_STACK_CNT		1024

#define	TILESIZE		1


#ifdef __cplusplus
extern "C" {
#endif

int	AS_Init			( void );
int	AS_Find_Path		( const int map_num, const int sx, const int sy, const int dx, const int dy );

#ifdef __cplusplus
}
#endif



#endif


