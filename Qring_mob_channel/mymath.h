
#ifndef _CMATH_H
#define _CMATH_H


#define		GRAVITY			9.8f
#define		PI			3.1415926535897932384626433832795
#define		SINES_SIZE		1024
#define		SINES_VAL		0x3FF	// SINES_SIZE-1
#define		SINES_VALMUL		0x10000
#define		RADIAN_SIZE		360

int	CreateSines			( void );
void	UnSines				( void );

int	BinarySearch			( long *, const long, const int );
int	SequenceSearch			( long *, const long, const int );
long	ATan				( const float, const float );
int	GetDirection			( const float, const float );
void	GetCirclePos			( const float, const int, float *, float * );

void	PointMove			( const float, const float, const float, float *, float *, float * );
void	StraightMove			( const float, const int, const float, float *, float *, float *, float * );
void	DescendMove			( const float, const float, float *, float *, float *, float * );
void	ParabolaMove			( const float, const int, const float, float *, float *, float *, float * );
void	CircleMove			( const float, const float, float, float *, float *, float *, float * );

float	GetLength			( const float, const float, const float, const float );

int	InCircle			( float, float, const int, const float, const float, float, float );
int	InBox				( const float, const float, const float, const float, const float, const float );

#endif
