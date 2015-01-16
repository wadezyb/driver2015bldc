/**
  ******************************************************************************
  * @file    canopen.c
  * @author  ZHAN Yubo
  * @version V9.0.0
  * @date    03-May-2014
  * @brief   This file is CANOPEN
  *
*/

/* INCLUDES */
#include "canopen.h"
#include "includes.h"
#include "can.h"
#include "ad.h"
#include "pwm.h"
#include "encoder.h"
#include "foc.h"


/* Public Variables */
COBJ ControlWord={CONTROLWORD_INDEX,0,0};
COBJ StatusWord={STATUSWORD_INDEX,0,0};
COBJ TargetTorque={TARGETTORQUE_INDEX,0,0};
COBJ TargetVelocity={TARGET_VELOCITY_INDEX,0,0};
COBJ ModesofOperation={MODES_OF_OPERATION_INDEX,0,0,NO_MODE,NO_MODE};
COBJ ModesofOperationDisplay={MODES_OF_OPERATION_DISPLAY_INDEX,0,0,NO_MODE,NO_MODE};
COBJ MotorType={MOTORTYPE_INDEX,0,0};
COBJ CurrentLoopKcp={CURRENTLOOPPID_INDEX,CURRENTLOOPPID_KCP_SUBINDEX,1};
COBJ CurrentLoopKci={CURRENTLOOPPID_INDEX,CURRENTLOOPPID_KCI_SUBINDEX,0};
COBJ CurrentLoopKcff={CURRENTLOOPPID_INDEX,CURRENTLOOPPID_KCFF_SUBINDEX,0};
COBJ MaxCurrent={MAXCURRENT_INDEX,0,0};
COBJ TargetPosition={TARGET_POSITION_INDEX,0,0};
COBJ ActualPosition={POSITION_ACTUAL_VALUE_INDEX,0,0};

DEVICEOBJ Joint1 ={CAN_NODE1};
DEVICEOBJ Joint2 ={CAN_NODE2};
DEVICEOBJ Joint3 ={CAN_NODE3};
DEVICEOBJ Joint4 ={CAN_NODE4};


/* Device control commands */
//Shutdown
void Shutdown( void )
{
	
}
//Switch on
void Switchon( void )
{
	
}
//Switch on2
void Switchon2( void )
{
	ControlWord.Param |= CONTROLWORD_SWITCH_ON;
}
void Switchoff2( void )
{
	ControlWord.Param &=~ CONTROLWORD_SWITCH_ON;
}
//Disable voltage
void Disablevoltage( void )
{
}
//Quick stop
void Quickstop( void )
{
}
//Disable operation
void Disableoperation( void )
{
}
//Enable operation
void Enableoperation( void )
{
}
//Fault reset
void Faultreset( void )
{
}



/* Check status */
/* Return the state */
int Checkstatus( int status )
{
	int state=STATE_START;//
	if((status&0x004f)==0x0000)//XXXX XXXX X1XX 0000
	{
		state=STATE_NOT_READY_TO_SWITCH_ON;
	}
	else if((status&0x004f)==0x0040)
	{
		state=STATE_SWITCH_ON_DISABLED;
	}
	else if((status&0x006f)==0x0021)
	{
		state=STATE_READY_TO_SWITCH_ON;
	}
	else if((status&0x006f)==0x0023)
	{
		state=STATE_SWITCHED_ON;
	}
	else if((status&0x006f)==0x0027)
	{
		state=STATE_OPERATION_ENABLED;
	}
	else if((status&0x006f)==0x0007)
	{
		state=STATE_QUICK_STOP_ACTIVE;
	}
	else if((status&0x004f)==0x004F)
	{
		state=STATE_FAULT_REACTION_ACTIVE;
	}
	else if((status&0x004f)==0x0048)
	{
		state=STATE_FAULT;
	}
	
	return state;
}

void RunControlCommand( void )
{

}

/* Modes of Operation */
//mode can be found from MODES_OF_OPERATION_INDEX
void SetOperationMode( int mode )
{
	ModesofOperation.Param = mode;
	ModesofOperationDisplay.Param = mode;
}

/* Set Motor Type */
//Motor type can be found from MOTORTYPE_INDEX
void SetMotorType( int type )
{
	MotorType.Param = type;
}

void canopenDataSyncTask( void *pvParameters )
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 10;
	vTaskDelay(3000);
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
		
		ActualPosition.Param = Encoder.Value;
		CAN_send(CAN_NODE_MASTER,POSITION_ACTUAL_VALUE_INDEX,0,ActualPosition.Param);
		ActualPosition.LastParam = ActualPosition.Param;
		CAN_send(CAN_NODE_MASTER,STATUSWORD_INDEX,0,StatusWord.Param);
	}
}

/*========================== END OF FILE =======================================*/
