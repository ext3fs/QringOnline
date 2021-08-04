
#include	"global.h"
#include	"db.h"
#include	"util.h"
#include 	"sp.h"

struct path
{
	int		pathtotal;
	int		pathx[MAX_PATH_CNT];
	int		pathz[MAX_PATH_CNT];

	unsigned short 	lenMap[MAX_X_TILE][MAX_Z_TILE];

	int 		pcnt;
	int		plistx[MAX_PATH_CNT];
	int		plistz[MAX_PATH_CNT];
	int		llist [MAX_PATH_CNT];

	int		history_cnt;
	int		history_x[MAX_HISTORY_CNT];
	int		history_z[MAX_HISTORY_CNT];
	double		s, e, delay;
	struct timeval	t;
};




// --------------------------------------------------------------------
// inner valiable 
static const int 	SP_add_x[8] = {  0, -1,  0,  0,  1,  1,  0,  0 };
static const int	SP_add_z[8] = { -1,  0,  1,  1,  0,  0, -1, -1 };

static struct path	*SP_path;





// --------------------------------------------------------------------
// inner function
static inline int	SP_rotateRight	( int *, int *, int * );
static inline int 	SP_freeLine	( const int, const int, const int, const int, const int );






int	SP_init( void )
{
	int	i;

	if( (SP_path = calloc((G_proc_thr_cnt + 1), sizeof(struct path))) == NULL )
	{
		dprintf(0, "[ SP ]Error SP_init() calloc() : %s \n", strerror(errno) );
		return (-1);
	}

	for( i=1; i<=G_proc_thr_cnt; i++ )
		memset( SP_path[i].lenMap, 0xff, sizeof(SP_path[i].lenMap) );

	return (0);
}


int	SP_round( double val, double mid ) 
{
	if (fmod(val, 1.0) >= mid) val += (1.0 - mid);
	return((int)val);
} 


int 	SP_to_x( const int tidx )
{
	return (SP_path[tidx].pathx[SP_path[tidx].pathtotal-2]);
}


int 	SP_to_z( const int tidx )
{
	return (SP_path[tidx].pathz[SP_path[tidx].pathtotal-2]);
}


double	SP_delay( const int tidx )
{
	return (SP_path[tidx].delay);
}


int	SP_history_cnt( const int tidx )
{
	return (SP_path[tidx].history_cnt);
}


static inline int	SP_rotateRight( int *p_dir, int *p_nx, int *p_nz ) 
{
	if( *p_dir < 0 || *p_dir > 7 )
		*p_dir = 0;

	*p_nx += SP_add_x[*p_dir];
	*p_nz += SP_add_z[*p_dir];
	*p_dir+= 1;

	if( (*p_nx < 1) || (*p_nx > (MAX_X_TILE - 2)) || (*p_nz < 1) || (*p_nz > (MAX_Z_TILE - 2)) )
		return (-1);

	return (0);
} 


static inline int 	SP_freeLine( const int tidx, const int spx, const int spz, const int epx, const int epz ) 
{ 
	int 	i, dx, dz, steps;
	double 	x, z, ssx, ssz;


	steps = 0;

	dx = (spx - epx);
	dz = (spz - epz);

	x = (double)spx;
	z = (double)spz;

	ssx = 0;
	ssz = 0;

	if( abs(dx) > abs(dz) )
	{
		if( dx > 0 ) 
			ssx = -1;
		else 
			ssx = 1;

		if( dx == 0 ) 
			ssz = 0;
		else 
			ssz = dz / (dx*ssx);

		steps = abs(dx);
	}
	else 
	{
		if( dz > 0 ) 
			ssz = -1;
		else 
			ssz = 1;

		if( dz == 0 ) 
			ssx = 0;
		else 
			ssx = dx / (dz*ssz);

		steps = abs(dz);
	}

	for( i=0; i<=steps; i++ )
	{
		if( SP_path[tidx].lenMap[SP_round(x, 0.5)][SP_round(z, 0.5)] == 0xffff )
			return (0);

		x += ssx;
		z += ssz;
	}

	return(1);
} 


int	SP_Find_Path( const int tidx, const int channel, const int mob_id, const int map, const int x1, const int z1
			, const int x2, const int z2 ) 
{
	int 		i, reached, dir, short_dir;
	int	 	bestx, bestz, beststep;
	int 		lspx, lspz, nx, nz, px, pz, dx, dz, sx, sz;


	gettimeofday( &SP_path[tidx].t, NULL );
	SP_path[tidx].delay		= 0.0f;
	SP_path[tidx].s 		= ((double)SP_path[tidx].t.tv_sec * 1000000) + ((double)SP_path[tidx].t.tv_usec);

	reached				= 0;
	SP_path[tidx].pathtotal		= 0;
	SP_path[tidx].history_cnt	= 0;

	if( (x1 == x2) && (z1 == z2) ) 
		return (0);

	if( D_ChkMob_map_collission(channel, mob_id, x2, z2) )
		return (0);	

	memset( SP_path[tidx].lenMap	, 0xff, sizeof(SP_path[tidx].lenMap) );

	// set first node
	SP_path[tidx].lenMap[x1][z1]	= 1;
	SP_path[tidx].plistx[0]		= x1;
	SP_path[tidx].plistz[0]		= z1;
	dx 				= (x1 - x2);
	dz 				= (z1 - z2); 
	SP_path[tidx].llist[0]		= (dx * dx) + (dz * dz);
	SP_path[tidx].pcnt		= 1;


	do
	{
		short_dir = 0;
		for( i=0; i < SP_path[tidx].pcnt; i++ ) 
		{
			if( SP_path[tidx].llist[i] < SP_path[tidx].llist[short_dir] )
				short_dir = i;
		}

		px = SP_path[tidx].plistx[short_dir];
		pz = SP_path[tidx].plistz[short_dir];
		SP_path[tidx].plistx[short_dir] = SP_path[tidx].plistx[SP_path[tidx].pcnt - 1];
		SP_path[tidx].plistz[short_dir] = SP_path[tidx].plistz[SP_path[tidx].pcnt - 1];
		SP_path[tidx].llist [short_dir] = SP_path[tidx].llist [SP_path[tidx].pcnt - 1];
		SP_path[tidx].pcnt --;

		nx = px;
		nz = pz;
		dir = 0;

		for( i=0; i<8; i++ ) 
		{
			if( SP_rotateRight(&dir, &nx, &nz) )
				continue;

			if( SP_path[tidx].lenMap[nx][nz] == 0xffff && !D_ChkMob_map_collission(channel, mob_id, nx, nz)
				&& !D_ChkMob_over_area_persuit(channel, mob_id, nx, nz) )
			{
				SP_path[tidx].lenMap[nx][nz]	= SP_path[tidx].lenMap[px][pz] + 1;

				SP_path[tidx].plistx[SP_path[tidx].pcnt]	= nx;
				SP_path[tidx].plistz[SP_path[tidx].pcnt]	= nz;

				dx	= (nx - x2);
				dz	= (nz - z2);
				SP_path[tidx].llist[SP_path[tidx].pcnt]	= (dx * dx) + (dz * dz);
				SP_path[tidx].pcnt ++;
			}
	
			if( (nx == x2) && (nz == z2) ) 
			{
				reached = 1;
				break;
			}
		}

		if( (SP_path[tidx].pcnt + 8) > (MAX_PATH_CNT - 1) )
		{
			dprintf(channel, "[ SP ]Error SP_Find_Path() max_path_cnt mob_id(%d), map(%d), s(%d, %d), e(%d, %d) \n"
				, mob_id, map, x1, z1, x2, z2 );
			break;
		}

	} while ( (reached == 0) && (SP_path[tidx].pcnt != 0) );



	if ( reached ) 
	{

		SP_path[tidx].pathx[0] 	= x2;
		SP_path[tidx].pathz[0] 	= z2;
		SP_path[tidx].pathtotal ++;

		lspx = px = x2;
		lspz = pz = z2;
		bestx = bestz = 0;
		sx = sz = 0;

		while ( px != x1 || pz != z1 )
		{

			nx = px;
			nz = pz;

			dir = 0;
			beststep = 0xffff;

			for( i=0; i<8; i++ ) 
			{
				if( SP_rotateRight(&dir, &nx, &nz) )
					continue;

				if( SP_path[tidx].lenMap[nx][nz] < beststep )
				{
					beststep = SP_path[tidx].lenMap[nx][nz];
					sx = nx;
					sz = nz;
				}
			}

			px = sx;
			pz = sz;

			SP_path[tidx].history_x[SP_path[tidx].history_cnt]	= px;
			SP_path[tidx].history_z[SP_path[tidx].history_cnt]	= pz;
			SP_path[tidx].history_cnt ++;

			if( SP_path[tidx].history_cnt > (MAX_HISTORY_CNT - 1) )
			{
				dprintf(channel, "[ SP ]Error SP_Find_Path() max_history mob_id(%d), map(%d), s(%d, %d), e(%d, %d) \n"
					, mob_id, map, x1, z1, x2, z2 );

				//for(i=0; i<SP_path[tidx].history_cnt; i++ )
				//	dprintf(channel, "history(%d, %d) \n"
				//		, SP_path[tidx].history_x[i], SP_path[tidx].history_z[i] );

				SP_path[tidx].pathtotal = 0;
				return (0);
			}

			if( SP_freeLine(tidx, lspx, lspz, px, pz) ) 
			{
				bestx = px;
				bestz = pz;
			}

			if( px == x1 && pz == z1 ) 
			{
				SP_path[tidx].pathx[SP_path[tidx].pathtotal] = px = bestx;
				SP_path[tidx].pathz[SP_path[tidx].pathtotal] = pz = bestz;
				SP_path[tidx].pathtotal ++;
				lspx = px;
				lspz = pz;  
			}

		} // while;

	} // if (reached);


	gettimeofday( &SP_path[tidx].t, NULL );
	SP_path[tidx].e 	= ((double)SP_path[tidx].t.tv_sec * 1000000) + ((double)SP_path[tidx].t.tv_usec);
	SP_path[tidx].delay	= (SP_path[tidx].e - SP_path[tidx].s);

	return (SP_path[tidx].pathtotal);

} /* SP_GetPath_fine */
