#ifndef _VLOOP_H_
#define _VLOOP_H_
#include "pid.h"


typedef struct
{
	int Hz;
	int error;
	int kp;
	int ki;
	int scale;
	int sum_error;
	int sum_error_max;
	int output_max;
	int output;
}vLoopPIObj;

typedef struct
{
	vLoopPIObj vPI;
	int V;
	int targetVelocity;
}vLoopObj;

extern vLoopObj Velocity;
extern int V[1000];
extern int Vo[1000];
extern int Vn;
extern int vloopTuningStartFlag;
extern int Vt;

void vPI_Init( void );
void vloopControl( void );
#endif
