/**
  ******************************************************************************
  * @file    ploop.c
  * @author  ZHAN Yubo
  * @version V9.0.0
  * @date    03-May-2014
  * @brief   This file is for Position Loop Control
  *
*/
#include "ploop.h"
#include "includes.h"
#include "pid.h"
#include "vloop.h"
#include "encoder.h"
#include "canopen.h"
#include "can.h"
#include "sci.h"
#include "interpolation.h"

#define Pi (3.1415926)
#define L0 (150.0)
#define L1 (225.0)
#define L2 (125.0)
#define h   (15.0/2/Pi)// mm/rad
#define h3 (4096*0.0032552/2.0/Pi)// mm/rad
#define MaxV (50.0)  // pulse/ms



pLoopObj Position;
int sendFlag = 1;

//Control Parameters
int Joint1Angle=0;//degree
int Joint2Angle=0;//degree
int JointH=100;//30-150cm
int JointAngle=0;//-360 - +360 degree

int Pp[1000];
int Pv[1000];
int Pe[1000];
int Po[1000];
int Pn;
int pTuningStartFlag;
TrajectoryObj tra;

void ploopTuning( void )
{
	
	if( Pn < 1000 )
	{
		Pp[Pn] = Position.targetPosition;	
		Pv[Pn] = Position.targetVelocity;
		Pe[Pn] = Encoder.Value;
		Po[Pn] = Encoder.V;
		Pn++;
	}
	else
	{
		pTuningStartFlag=0;
	}
}

void ploopInit( void )
{
	Position.pPD.kp = 50;
	Position.pPD.ki = 0;
	Position.pPD.kd = 20;
	Position.pPD.scale = 100;
	Position.pPD.output_max = 100;
	Position.pPD.error_sum_max = Position.pPD.output_max*Position.pPD.scale;
		
//	// for axis 4
//	Position.pPD.kp = 5;
//	Position.pPD.ki = 0;
//	Position.pPD.kd = 1;
//	Position.pPD.scale = 100;
//	Position.pPD.output_max = 100;
//	Position.pPD.error_sum_max = Position.pPD.output_max*Position.pPD.scale;
}

void ploopControl( void )
{
	// Read Position Value
	Position.P = Encoder.Value;
	// Calc the position error.
	Position.pPD.error = Position.targetPosition - Position.P;
	// Calc the d_error
	Position.pPD.d_error = Position.pPD.error - Position.pPD.last_error;
	// Remember error to last error
	Position.pPD.last_error = Position.pPD.error;
	// Calc the sum of error
	Position.pPD.error_sum += Position.pPD.ki*Position.pPD.error;
	// error sum limite
	if( Position.pPD.error_sum > Position.pPD.error_sum_max )
		Position.pPD.error_sum = Position.pPD.error_sum_max;
	else if( Position.pPD.error_sum < -Position.pPD.error_sum_max )
		Position.pPD.error_sum = -Position.pPD.error_sum_max;
	// Calc the PD output
	Position.pPD.output = (Position.pPD.kp*Position.pPD.error + Position.pPD.error_sum + Position.pPD.kd*Position.pPD.d_error )/Position.pPD.scale+ Position.targetVelocity;//;
	// output limitation
	if( Position.pPD.output > Position.pPD.output_max )
		Position.pPD.output = Position.pPD.output_max;
	else if( Position.pPD.output < -Position.pPD.output_max )
		Position.pPD.output = -Position.pPD.output_max;
	
	// apply the output to vloop
	Velocity.targetVelocity = Position.pPD.output;
	if( pTuningStartFlag == 1 )
		ploopTuning();	
	
}


void ploopTask( void *pvParameters )
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 1;	//1KHz
	ploopInit();
	//vTaskSuspend(NULL);
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
		if( (ControlWord.Param&CONTROLWORD_SWITCH_ON) == CONTROLWORD_SWITCH_ON)
		{
			ploopControl();
		}
	}
}


void sendPositionTask( void *pvParameters )
{
	TickType_t xLastWakeTime;
//	char stringBuf[20];
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		vTaskDelayUntil( &xLastWakeTime, 100 );
		//vTaskDelay(1000);
		#if (CAN_NODE != CAN_NODE5)
		{
//			ActualPosition.Param = Encoder.Value;
//			CAN_send(0x05,POSITION_ACTUAL_VALUE_INDEX,0,ActualPosition.Param);
		}
		#else
		{
			if(sendFlag ==1)
			{
				sendFlag = 0;
				sciSendString("A1 ",bMessage);
				sciSendString(int2String(Joint1.ActualPosition.Param,stringBuf),bMessage);
				sciSendString("\r\n",bMessage);
				vTaskDelay(5);
				sciSendString("A2 ",bMessage);
				sciSendString(int2String(Joint2.ActualPosition.Param,stringBuf),bMessage);
				sciSendString("\r\n",bMessage);
				vTaskDelay(5);
				sciSendString("A3 ",bMessage);
				sciSendString(int2String(Joint3.ActualPosition.Param,stringBuf),bMessage);
				sciSendString("\r\n",bMessage);
				vTaskDelay(5);
				sciSendString("A4 ",bMessage);
				sciSendString(int2String(Joint4.ActualPosition.Param,stringBuf),bMessage);
				sciSendString("\r\n",bMessage);
				vTaskDelay(5);
				sciSendString("A5 ",bMessage);
				sciSendString(int2String(-Encoder.Value,stringBuf),bMessage);
				sciSendString("\r\n",bMessage);
				vTaskDelay(5);			
			}

		}
		#endif
		
	}
}

void MotionPlanningTask( void *pvParameters )
{
	TickType_t xLastWakeTime;
	
	double J1Resolution = 0.0010986;// degrees/pulse
	double J2Resolution = 0.0017578;// degrees/pulse
	double J3Resolution = 0.0032552;// mm/pulse
	double J4Resolution = 0.0077097;// degrees/pulse

	double J1Max = 109963.00000;
	double J2Max = 86137.00000;
	double J1Min = -106395.00000;
	double J2Min = -82281.00000;
	double J1Zero = (J1Max+J1Min)/2;
	double J2Zero = (J2Max+J2Min)/2;
	double J3Zero = -5429.00000;
	double J4Zero = -60859.00000;

	double J1Offset = -(J1Max+J1Min)*0.5;
	double J2Offset = -(J2Max+J2Min)*0.5;
	double J3Offset = -J3Zero;
	double J4Offset = -J4Zero;
	
	// Joint Limitation
//	double Joint1Max = 90;//Degree
//	double Joint1Min = -90;//Degree
//	double Joint2Max = 90;//Degree
//	double Joint2Min = -90;//Degree
//	double Joint4Max = 90;//Degree
//	double Joint4Min = -90;//Degree
//	double PezMax = 135; //mm
//	double PezMin = 35;  //mm
	
	double theta1=0;
	double theta2=0;
	double theta3=0;
	double theta4=0;
	
	int completeFlag = 1;
	
	int dTheta[4];	
	int MaxdTheta;
	int i;
	int Pos=0;
	int Error=10;
	
	Joint1.ControlWord.Param &=~ CONTROLWORD_SWITCH_ON;
	Joint2.ControlWord.Param &=~ CONTROLWORD_SWITCH_ON;
	Joint3.ControlWord.Param &=~ CONTROLWORD_SWITCH_ON;
	Joint4.ControlWord.Param &=~ CONTROLWORD_SWITCH_ON;
	
	CAN_send(CAN_NODE1,CONTROLWORD_INDEX,0,Joint1.ControlWord.Param);
	CAN_send(CAN_NODE2,CONTROLWORD_INDEX,0,Joint2.ControlWord.Param);
	CAN_send(CAN_NODE3,CONTROLWORD_INDEX,0,Joint3.ControlWord.Param);
	CAN_send(CAN_NODE4,CONTROLWORD_INDEX,0,Joint4.ControlWord.Param);
	
	Joint1.TargetPosition.Param = J1Zero;
	Joint2.TargetPosition.Param = J2Zero;
	Joint3.TargetPosition.Param = J3Zero;
	Joint4.TargetPosition.Param = J4Zero;
	Joint1.TargetVelocity.Param = MaxV;
	Joint2.TargetVelocity.Param = MaxV;
	Joint3.TargetVelocity.Param = MaxV;
	Joint4.TargetVelocity.Param = MaxV;
	while(
			(abs(Joint1.ActualPosition.Param-Joint1.TargetPosition.Param)<3) && 
		(abs(Joint2.ActualPosition.Param-Joint2.TargetPosition.Param)<3) && 
		(abs(Joint3.ActualPosition.Param-Joint3.TargetPosition.Param)<3) && 
		(Joint4.ActualPosition.Param-Joint4.TargetPosition.Param<3)==0);	
	vTaskDelay(5000);

	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		vTaskDelayUntil( &xLastWakeTime, 10 );
		
		if( completeFlag == 1 )
		{
			completeFlag = 0;
			if(Pos==0)//Point1
			{
				Joint1Angle = -10;
				Joint2Angle = 10;
				JointH = 100;
				JointAngle = 0;
				theta1 = ( Joint1Angle )*Pi/180.0;// random rad
				theta2 = ( Joint2Angle )*Pi/180.0;// random rad
				theta4 = ( JointAngle )*Pi/180.0;// random rad
				theta3 = ((L0-(JointH))+theta4*h)/h3;
			}
			else if( Pos==1 )//Point2
			{
				Joint1Angle = -10;
				Joint2Angle = 10;
				JointH = 40;
				JointAngle = 0;
				theta1 = ( Joint1Angle )*Pi/180.0;// random rad
				theta2 = ( Joint2Angle )*Pi/180.0;// random rad
				theta4 = ( JointAngle )*Pi/180.0;// random rad
				theta3 = ((L0-(JointH))+theta4*h)/h3;
			}
			else if( Pos==2 )//Point3
			{
				Joint1Angle = -10;
				Joint2Angle = 10;
				JointH = 100;
				JointAngle = 0;
				theta1 = ( Joint1Angle )*Pi/180.0;// random rad
				theta2 = ( Joint2Angle )*Pi/180.0;// random rad
				theta4 = ( JointAngle )*Pi/180.0;// random rad
				theta3 = ((L0-(JointH))+theta4*h)/h3;
			}
			else if( Pos==3 )//Point4
			{
				Joint1Angle = 10;
				Joint2Angle = -10;
				JointH = 100;
				JointAngle = 0;
				theta1 = ( Joint1Angle )*Pi/180.0;// random rad
				theta2 = ( Joint2Angle )*Pi/180.0;// random rad
				theta4 = ( JointAngle )*Pi/180.0;// random rad
				theta3 = ((L0-(JointH))+theta4*h)/h3;
			}		
			else if( Pos==4 )//Point5
			{
				Joint1Angle = 10;
				Joint2Angle = -10;
				JointH = 40;
				JointAngle = 0;
				theta1 = ( Joint1Angle )*Pi/180.0;// random rad
				theta2 = ( Joint2Angle )*Pi/180.0;// random rad
				theta4 = ( JointAngle )*Pi/180.0;// random rad
				theta3 = ((L0-(JointH))+theta4*h)/h3;
			}	
			else if( Pos==5 )//Point6
			{
				Joint1Angle = 10;
				Joint2Angle = -10;
				JointH = 100;
				JointAngle = 0;
				theta1 = ( Joint1Angle )*Pi/180.0;// random rad
				theta2 = ( Joint2Angle )*Pi/180.0;// random rad
				theta4 = ( JointAngle )*Pi/180.0;// random rad
				theta3 = ((L0-(JointH))+theta4*h)/h3;
			}				
			Pos++;
			if(Pos==6)
			{
				Pos=0;
			}
			
			Joint1.TargetPosition.Param = (theta1/Pi*180/J1Resolution)-J1Offset;
			Joint2.TargetPosition.Param = (theta2/Pi*180/J2Resolution)-J2Offset;
			Joint3.TargetPosition.Param = (theta3*h3/J3Resolution)-J3Offset;
			Joint4.TargetPosition.Param = (theta4/Pi*180/J4Resolution)-J4Offset;
//			
			dTheta[0] = abs(Joint1.ActualPosition.Param-Joint1.TargetPosition.Param);
			dTheta[1] = abs(Joint2.ActualPosition.Param-Joint2.TargetPosition.Param);
			dTheta[2] = abs(Joint3.ActualPosition.Param-Joint3.TargetPosition.Param);
			dTheta[3] = abs(Joint4.ActualPosition.Param-Joint4.TargetPosition.Param);
			MaxdTheta = dTheta[0];
			for( i = 1; i < 4;i++ )
			{
				if(MaxdTheta < dTheta[i])
				{
					MaxdTheta = dTheta[i];
				}
				if( MaxdTheta == 0 )
				{
					MaxdTheta = 1;
				}
			}			
			
			Joint1.TargetVelocity.Param = (int)(MaxV*dTheta[0]/(float)MaxdTheta)+1;
			Joint2.TargetVelocity.Param = (int)(MaxV*dTheta[1]/(float)MaxdTheta)+1;
			Joint3.TargetVelocity.Param = (int)(MaxV*dTheta[2]/(float)MaxdTheta)+1;
			Joint4.TargetVelocity.Param = (int)(MaxV*dTheta[3]/(float)MaxdTheta)+1;

		}
		else if(
			(abs(Joint1.ActualPosition.Param-Joint1.TargetPosition.Param)<Error) && 
		(abs(Joint2.ActualPosition.Param-Joint2.TargetPosition.Param)<Error) && 
		(abs(Joint3.ActualPosition.Param-Joint3.TargetPosition.Param)<Error) && 
		(Joint4.ActualPosition.Param-Joint4.TargetPosition.Param<Error))
		{
			completeFlag = 1;
			//vTaskDelay(10);
			sendFlag = 1;
			//vTaskDelay(10);
		}
	}		
}
/*========================== END OF FILE =======================================*/

