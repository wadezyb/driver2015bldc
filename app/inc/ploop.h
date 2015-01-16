#ifndef _PLOOP_H_
#define _PLOOP_H_
#include "pid.h"
typedef struct
{
	int error;
	int last_error;
	int kp;
	int ki;
	int kd;
	int scale;
	int d_error;
	int d_error_max;
	int error_sum;
	int error_sum_max;
	int output_max;
	int output;
}pLoopPDObj;

typedef struct
{
	pLoopPDObj pPD;
	int targetPosition;
	int targetVelocity;
	int P;
}pLoopObj;

extern pLoopObj Position;

extern int Pp[];
extern int Pv[];
extern int Pe[];
extern int Po[];
extern int Pn;
extern int pTuningStartFlag;

void ploopTask( void *pvParameters );
void sendPositionTask( void *pvParameters );
void MotionPlanningTask( void *pvParameters );
#endif
