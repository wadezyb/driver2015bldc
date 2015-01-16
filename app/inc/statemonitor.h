#ifndef _STATEMONITOR_H_
#define _STATEMONITOR_H_

#define STATE_RUNNING 	(1)
#define STATE_STOP			(2)
#define STATE_READY			(3)
#define STATE_ERROR			(4)
#define STATE_STARTING 	(5)

typedef struct
{
	int State;
	int lastState;
	int Mode;
	int Error;
}stateObj;

extern stateObj state;
void driveStateManageTask( void *pvParameters );

#endif
