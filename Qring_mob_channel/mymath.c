
#include "global.h"
#include "mymath.h"


static float	Sin			( const long angle );
static float	Cos			( const long angle );
//static float	Tan			( const long angle );
static int	GetTab2Angle		( const long angle );
static long	GetAngle2Tab		( const int angle );



static long	*SinTab, *CosTab, *TanTab;
static float 	*RadTab;



static float	Sin( const long angle ) 	
{ 
	return (*(SinTab + (angle&SINES_VAL)) / (float)SINES_VALMUL); 
}


static float	Cos( const long angle ) 
{ 
	return (*(CosTab + (angle&SINES_VAL)) / (float)SINES_VALMUL); 
}


//static float	Tan( const long angle ) 
//{ 
//	return (*(TanTab + (angle&SINES_VAL)) / (float)SINES_VALMUL); 
//}


static int	GetTab2Angle( const long angle )	
{ 
	return (angle * 360 / SINES_SIZE); 
}


static long	GetAngle2Tab( const int angle )		
{ 
	return (angle * SINES_SIZE / 360); 
}


int	CreateSines()
{
	int		i;
	double		dAngle;


	if( SinTab != NULL )
		return (-1);
		
	SinTab 	= (long *)calloc(SINES_SIZE, sizeof(long));
	CosTab 	= (long *)calloc(SINES_SIZE, sizeof(long));
	TanTab 	= (long *)calloc(SINES_SIZE, sizeof(long));

	for( i=0; i<SINES_SIZE; i ++ )
	{
		dAngle = (double)(sin((double)(i*2*PI/SINES_SIZE))*SINES_VALMUL);
		*(SinTab + i) = (long)dAngle;

		dAngle = (double)(cos((double)(i*2*PI/SINES_SIZE))*SINES_VALMUL);
		*(CosTab + i) = (long)dAngle;

		dAngle = (double)(tan((double)(i*2*PI/SINES_SIZE))*SINES_VALMUL);
		if(dAngle > 0x7FFFFFFF) { // exception
			dAngle = 0x7FFFFFFF;
		}
		*(TanTab + i) = (long)dAngle;
	}

	RadTab = (float *)calloc(RADIAN_SIZE, sizeof(float));

	for(i=0; i<RADIAN_SIZE; i++)
		*(RadTab + i) = (float)(RADIAN_SIZE-i)*(float)(PI/180.f);

	return (0);
}


void 	UnSines( void )
{
	if( SinTab != NULL ) 
	{
		free(SinTab);
		free(CosTab);
		free(TanTab);
		free(RadTab);
	}
}


int 	BinarySearch( long *array, const long key, const int size )
{
	int iFirst, iLast, iMiddle;

	iMiddle = -1;
	iFirst 	= 0;
	iLast 	= size-1;

	while( iFirst <= iLast )
	{
		iMiddle = (iFirst+iLast) / 2;

		if( array[iMiddle] == key ) 
			return iMiddle;
		if( array[iMiddle] > key )
			iLast 	= iMiddle-1;
		else 
			iFirst 	= iMiddle+1;
	}

	return iMiddle;
}


int 	SequenceSearch( long *array, const long key, const int size )
{
	int i, iIndex;

	iIndex = -1;
	i = 0;
	while( i < size )
	{
		if( array[i] <= key )
			iIndex = i;
		else
			break;
		i++;
	}
	return iIndex;
}


long 	ATan( const float dx, const float dy )
{
	long lSlope = 0, lRevise = 0;

	if( dx == 0 && dy == 0 ) {
		return 0;
	}
	else if( dx > 0 && dy >= 0 ) {	// 0
		lSlope 	= labs((long)(dy*SINES_VALMUL/dx));
		lRevise = 0;
	}
	else if( dx <= 0 && dy > 0 ) {	// 90
		lSlope 	= labs((long)(dx*SINES_VALMUL/dy));
		lRevise = (SINES_SIZE>>2);
	}	
	else if( dx < 0 && dy <= 0 ) {	// 180
		lSlope 	= labs((long)(dy*SINES_VALMUL/dx));
		lRevise = (SINES_SIZE>>1);
	}
	else if( dx >= 0 && dy < 0 ) {	// 270
		lSlope 	= labs((long)(dx*SINES_VALMUL/dy));
		lRevise = SINES_SIZE - (SINES_SIZE>>2);
	}

	return BinarySearch(TanTab, lSlope, (SINES_SIZE>>2)) + lRevise;
}


int 	GetDirection( const float dx, const float dy )
{
	int basis	= 90;

	if( dx == 0 && dy == 0 ) return basis;

	return (GetTab2Angle(ATan(dx, dy))+basis) % 360;
}


void 	GetCirclePos( const float radius, const int angle, float *x, float *y )
{
	long lAngle = GetAngle2Tab(angle);

	*x = (float)(radius*Cos(lAngle));
	*y = (float)(radius*Sin(lAngle));
}


void 	PointMove( const float dx, const float dy, const float movevalue, float *ux, float *uy, float *axis)
{
	long labsx, labsy, lmovevalue;

	labsx = labs((long)(dx*SINES_VALMUL));
	labsy = labs((long)(dy*SINES_VALMUL));
	lmovevalue = (long)(movevalue*SINES_VALMUL);

	*axis = 0.0f;

	if( labsx >= labsy )
	{
		if( dx > 0 )
		{
			*ux = movevalue; *axis = dx;
			*uy = (dy/dx)*(*ux);
		}
		else if( dx < 0 )
		{
			*ux =-movevalue; *axis = dx;
			*uy = (dy/dx)*(*ux);
		}
	}
	else
	{
		if( dy > 0 )
		{
			*uy = movevalue; *axis = dy;
			*ux = (dx/dy)*(*uy);
		}
		else if( dy < 0 )
		{
			*uy =-movevalue; *axis = dy;
			*ux = (dx/dy)*(*uy);
		}
	}

	if( labsx < lmovevalue ) *ux = dx;
	if( labsy < lmovevalue ) *uy = dy;
}


void 	StraightMove( const float speed, const int angle, const float time, float *dx, float *dy, float *ux, float *uy )
{
	float x, y;
	long lAngle;

	if( speed == 0.0f || time == 0.0f ) {
		*dx = *dy = *ux = *uy = 0.0f;
		return;
	}

	x = *dx;
	y = *dy;

	lAngle = GetAngle2Tab(angle);
	*dx = speed*Cos(lAngle)*time;
	*dy = speed*Sin(lAngle)*time;

	*ux = *dx-x;
	*uy = *dy-y;
}


void 	DescendMove( const float speed, const float time, float *dx, float *dy, float *ux, float *uy )
{
	float x, y;

	if( time == 0.0f ) {
		*dx = *dy = *ux = *uy = 0.0f;
		return; 
	}

	x = *dx;
	y = *dy;

	*dx = speed*time;
	*dy = -(0.5f)*GRAVITY*time*time;

	*ux = *dx-x;
	*uy = *dy-y;
}


void 	ParabolaMove( const float speed, const int angle, const float time, float *dx, float *dy, float *ux, float *uy )
{
	float x, y;
	long lAngle;

	if( time == 0.0f ) {
		*dx = *dy = *ux = *uy = 0.0f;
		return; 
	}

	x = *dx;
	y = *dy;

	lAngle = GetAngle2Tab(angle);
	*dx = speed*Cos(lAngle)*time;
	*dy = speed*Sin(lAngle)*time-(0.5f)*GRAVITY*time*time;

	*ux = *dx-x;
	*uy = *dy-y;
}


void 	CircleMove( const float radiusx, const float radiusy, float movevalue, float *dx, float *dy, float *ux, float *uy )
{
	float x, y;

	if( movevalue == 0.0f ) {
		*dx = *dy = *ux = *uy = 0.0f;
		return;
	}

	x = *dx;
	y = *dy;

	if( *dx <= 0 ) {
		movevalue = fabsf(movevalue);
	}
	else if( *dx >= (radiusx*2) ) {
		movevalue = -fabsf(movevalue);
	}

	*dx = x + movevalue;
	*dy = (float)sqrt((double)((radiusx*radiusx*radiusy*radiusy-radiusy*radiusy*
						(*dx-radiusx)*(*dx-radiusx))/(radiusx*radiusx)));

	*ux = *dx-x;
	*uy = *dy-y;
}


float	GetLength( const float x1, const float y1, const float x2, const float y2 )
{
	return sqrtf((double)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
}


int	InCircle( float mx, float my, const int dir, const float rx, const float ry, float px, float py )
{
	float	px1, py1;
	long 	ldir;

	px	= px - mx;
	py	= py - my;
	mx	= 0;
	my	= 0;

	ldir	= GetAngle2Tab(dir);
	px1	= ( (px * Cos(ldir)) - (py * Sin(ldir)) );
	py1	= ( (px * Sin(ldir)) + (py * Cos(ldir)) );
	return ( (((px1-mx)*(px1-mx)/(rx*rx)) + ((py1-my)*(py1-my))/(ry*ry)) <= 1 );
}


int	InBox( const float sx, const float sy, const float w, const float h, const float px, const float py )
{
	return ( sx<px && (sx+w)>px && sy<py && (sy+h)>py );
}
