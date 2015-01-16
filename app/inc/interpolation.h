#ifndef _INTERPOLATION_H_
#define _INTERPOLATION_H_

#define SBUF_LENGTH (10)// To Send Messages buf length

typedef struct
{
	int Position;
	int Velocity;
}TrajectoryObj;//Trajectory Obj

typedef struct
{
	int index;
	int index_max;
	TrajectoryObj Trajectory[SBUF_LENGTH];
}STQueueObj;//Sending Trajectory Queue Obj

typedef struct
{
	float a;
	float b;
	float c;
	float d;
}CubicSplineInterpolationObj;

extern TrajectoryObj Tra;

void cubicSlpineInterpolation( int p0, int v0, int p1, int v1, int n );
void interpolationTask( void *pvParameters );
void trajectoryGenTask( void *pvParameters );

#endif
