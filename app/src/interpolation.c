#include "interpolation.h"
#include "includes.h"
#include "main.h"
#include "ploop.h"
#include "cloop.h"
#include "encoder.h"
#include "pwm.h"
#include "canopen.h"

TrajectoryObj Tra;
TrajectoryObj LastTra;
STQueueObj STQueue;//Sending Trajectory Queue
CubicSplineInterpolationObj CSI;


void interpolationInit( void )
{	
	STQueue.index_max = SBUF_LENGTH;
	for( STQueue.index=0;STQueue.index< STQueue.index_max;STQueue.index++)
	{
		STQueue.Trajectory[STQueue.index].Position = Encoder.Value;
		STQueue.Trajectory[STQueue.index].Velocity = Encoder.V*10;
	}
	STQueue.index = 0;
	Tra.Position = Encoder.Value;
	Tra.Velocity = 0;
}

void cubicSlpineInterpolation( int p0, int v0, int p1, int v1, int n )
{
	//p(n)=a*n^3+b*n^2+c*n+d;
	//v(n)=p'(n)=3a*n^2+2b*n+c;
	//p(0)=d;
	//v(0)=c;
	float a,b,c,d;
	int k=0;
	d = p0;
	c = v0;
	b = -( 3*d+2*c*n-3*p1+n*v1 )/n/n;
	a = -( -2*d - c*n + 2*p1 -n*v1 )/n/n/n;
	// calc the cubic spline curve
	for( k=0;k<n;k++ )
	{
		STQueue.Trajectory[k].Position = 	a*k*k*k+b*k*k+c*k+d;
		STQueue.Trajectory[k].Velocity = 	3*a*k*k+2*b*k+c;
	}
}


/*
take Trajectory from queue every 1ms and calc the interpolation value at every ms 
*/
void interpolationTask( void *pvParameters )
{
	TickType_t xLastWakeTime;
	interpolationInit();
	//vTaskSuspend(NULL);
	Tra.Position = Encoder.Value;
	LastTra.Position = Tra.Position;
	xLastWakeTime = xTaskGetTickCount();	
	for(;;)
	{
		vTaskDelayUntil( &xLastWakeTime, 1) ;//1000Hz

		if( STQueue.index >= STQueue.index_max )
		{
			STQueue.index=0;
			Tra.Velocity = (Tra.Position - LastTra.Position)/10;
			cubicSlpineInterpolation(LastTra.Position,LastTra.Velocity, Tra.Position, Tra.Velocity, STQueue.index_max);
			LastTra.Velocity = Tra.Velocity;
			LastTra.Position = Tra.Position;
		}
		Position.targetPosition = STQueue.Trajectory[STQueue.index].Position;
		Position.targetVelocity = STQueue.Trajectory[STQueue.index].Velocity;
		STQueue.index++;		
	}
}

//void trajectoryGenTask( void *pvParameters )
//{
//	TickType_t xLastWakeTime;	
//	// Update the Target Position to be Current Position
//	Tra.Position = Encoder.Value;
//	LastTra.Position = Tra.Position;
//	LastLastTra.Position = LastTra.Position;
//	// Create the interpolationTask, running at 1kHz
//	xTaskCreate( interpolationTask, "interpolationTask", configMINIMAL_STACK_SIZE*2, NULL, 4, &interpolationTaskHandle );
//	xLastWakeTime = xTaskGetTickCount();
//	for(;;)
//	{
//		vTaskDelayUntil( &xLastWakeTime, 10) ;//100Hz
//		// Calc the Velocity of the tragectory.
//		LastTra.Velocity = (Tra.Position - LastLastTra.Position)/10/2;	
//		LastTra.Position = Tra.Position;
//		LastLastTra.Position = LastTra.Position;		
//	}	
//}
