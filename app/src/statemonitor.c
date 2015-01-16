/**
  ******************************************************************************
  * @file    statemonitor.c
  * @author  ZHAN Yubo
  * @version V9.0.0
  * @date    03-May-2014
  * @brief   This file is for State Monitor
  *
*/
#include "statemonitor.h"
#include "includes.h"
#include "main.h"
#include "canopen.h"
#include "calibration.h"
#include "pwm.h"
#include "led.h"
#include "ploop.h"
#include "encoder.h"
#include "interpolation.h"

stateObj state;

/*
	* Check Faults
	* If there are faults return 1  If not return 0
*/
int checkFaults( void )
{
	// UnderVoltage Fault
	if( (StatusWord.Param&STATUSWORD_BIT4)== STATUSWORD_BIT4 )
	{
		return 1;
	}
	return 0;
}
/*
	* Check if it is ready to switch on
	* 1st check faults
	* 2nd check mode of peration
	* Return 1 if Ready
	* return 0 if not ready
*/
int checkReady( void )
{
	if( checkFaults() )
	{
		return 0;
	}
	else if( ModesofOperation.Param == NO_MODE )
	{
		return 0;
	}
	return 1;
}


void changeStateToRunning( void )
{
	LED.Color = LED_COLOR_GREEN;
	LED.State = LED_STATE_FLASHING;
	LED.Delay = 100;
	Position.targetPosition = Encoder.Value;
	PWMON();
	state.State = STATE_RUNNING;
}

void changeStateToError( void )
{
	LED.Color = LED_COLOR_RED;
	LED.State = LED_STATE_FLASHING;
	LED.Delay = 100;
	PWMOFF();
}

void changeStateToStop()
{
	LED.Color = LED_COLOR_RED;
	LED.State = LED_STATE_ON;
	PWMOFF();
}

void changeStateToReady( void )
{
	LED.Color = LED_COLOR_GREEN;
	LED.State = LED_STATE_FLASHING;
	LED.Delay = 1000;
	PWMOFF();
	state.State = STATE_READY;
}

void changeStateToStarting( void )
{
	LED.Color = LED_COLOR_RED;
	LED.State = LED_STATE_FLASHING;
	LED.Delay = 1000;
	PWMOFF();	
	state.State = STATE_STARTING;
}


//		if( ControlWord.Param != ControlWord.LastParam )
//		{
//			ControlWord.LastParam = ControlWord.Param;
//			if( (ControlWord.Param & CONTROLWORD_UPDATE_POSITION) == CONTROLWORD_UPDATE_POSITION )
//			{
//				EncCalibrate();
//				ControlWord.Param &= ~CONTROLWORD_UPDATE_POSITION;
//				ControlWord.LastParam = ControlWord.Param;
//			}
//			if( (ControlWord.Param & CONTROLWORD_SWITCH_ON) == CONTROLWORD_SWITCH_ON )
//			{
//				PWMON();
//			}
//			else
//			{
//				PWMOFF();
//			}
//		}
void modeManage( void )
{
	if( ModesofOperation.Param != ModesofOperation.LastParam )
	{
		switch( ModesofOperation.Param )
		{
			// default mode is No Mode
			case NO_MODE:
				if( ModesofOperation.LastParam == NO_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == PROFILE_VELOCITY_MODE )
				{
					vTaskDelete(ploopTaskHandle);
				}
				else if( ModesofOperation.LastParam == VELOCITY_MODE )
				{
					vTaskDelete(ploopTaskHandle);
				}
				else if( ModesofOperation.LastParam == PROFILE_VELOCITY_MODE )
				{
					vTaskDelete(ploopTaskHandle);
				}
				else if( ModesofOperation.LastParam == TORQUE_PROFILE_MODE )
				{
					vTaskDelete(ploopTaskHandle);
				}
				else if( ModesofOperation.LastParam == HOMING_MODE )
				{
					vTaskDelete(ploopTaskHandle);
				}
				else if( ModesofOperation.LastParam == INTERPOLATED_POSITION_MODE )
				{
					vTaskDelete(ploopTaskHandle);
				}				
				break;
			case PROFILE_POSITION_MODE:
				// if Last Mode is one of follows:
				//	1. No Mode
				//	2. Velocity Mode
				//	3. Torque Mode
				//	then create Position Loop Control Task				
				if( ModesofOperation.LastParam == NO_MODE )
				{
					xTaskCreate( ploopTask, "ploop", configMINIMAL_STACK_SIZE*2, NULL, 3, &ploopTaskHandle );
				}
				else if( ModesofOperation.LastParam == PROFILE_POSITION_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == VELOCITY_MODE )
				{
					xTaskCreate( ploopTask, "ploop", configMINIMAL_STACK_SIZE*2, NULL, 3, &ploopTaskHandle );
				}
				else if( ModesofOperation.LastParam == PROFILE_VELOCITY_MODE )
				{
					xTaskCreate( ploopTask, "ploop", configMINIMAL_STACK_SIZE*2, NULL, 3, &ploopTaskHandle );
				}
				else if( ModesofOperation.LastParam == TORQUE_PROFILE_MODE )
				{
					xTaskCreate( ploopTask, "ploop", configMINIMAL_STACK_SIZE*2, NULL, 3, &ploopTaskHandle );
				}
				else if( ModesofOperation.LastParam == HOMING_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == INTERPOLATED_POSITION_MODE )
				{
					//Delete Interpolation Task
					vTaskDelete(interpolationTaskHandle);
				}				
				break;
				
			case VELOCITY_MODE:
				if( ModesofOperation.LastParam == NO_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == PROFILE_POSITION_MODE )
				{
					vTaskDelete(ploopTaskHandle);
				}
				else if( ModesofOperation.LastParam == VELOCITY_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == PROFILE_VELOCITY_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == TORQUE_PROFILE_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == HOMING_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == INTERPOLATED_POSITION_MODE )
				{
					vTaskDelete(ploopTaskHandle);
					vTaskDelete(interpolationTaskHandle);
				}
				break;
			case PROFILE_VELOCITY_MODE:
				if( ModesofOperation.LastParam == NO_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == PROFILE_POSITION_MODE )
				{
					vTaskDelete(ploopTaskHandle);
				}
				else if( ModesofOperation.LastParam == VELOCITY_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == PROFILE_VELOCITY_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == TORQUE_PROFILE_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == HOMING_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == INTERPOLATED_POSITION_MODE )
				{
					vTaskDelete(ploopTaskHandle);
					vTaskDelete(interpolationTaskHandle);					
				}				
				break;
			case TORQUE_PROFILE_MODE:
				if( ModesofOperation.LastParam == NO_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == PROFILE_POSITION_MODE )
				{
					vTaskDelete(ploopTaskHandle);
				}
				else if( ModesofOperation.LastParam == VELOCITY_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == PROFILE_VELOCITY_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == TORQUE_PROFILE_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == HOMING_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == INTERPOLATED_POSITION_MODE )
				{
					vTaskDelete( &interpolationTaskHandle );
					vTaskDelete( &ploopTaskHandle );
				}
				break;
			case HOMING_MODE:
				if( ModesofOperation.LastParam == NO_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == PROFILE_POSITION_MODE )
				{
					vTaskDelete( &ploopTaskHandle );
				}
				else if( ModesofOperation.LastParam == VELOCITY_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == PROFILE_VELOCITY_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == TORQUE_PROFILE_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == HOMING_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == INTERPOLATED_POSITION_MODE )
				{
					vTaskDelete( &interpolationTaskHandle );
					vTaskDelete( &ploopTaskHandle );				
				}
				break;
			case INTERPOLATED_POSITION_MODE:
				if( ModesofOperation.LastParam == NO_MODE )
				{
					xTaskCreate( ploopTask, "ploop", configMINIMAL_STACK_SIZE*2, NULL, 3, &ploopTaskHandle );
					xTaskCreate( interpolationTask, "interpolationTask", configMINIMAL_STACK_SIZE*2, NULL, 4, &interpolationTaskHandle );
				}
				else if( ModesofOperation.LastParam == PROFILE_POSITION_MODE )
				{
					xTaskCreate( interpolationTask, "interpolationTask", configMINIMAL_STACK_SIZE*2, NULL, 4, &interpolationTaskHandle );
				}
				else if( ModesofOperation.LastParam == VELOCITY_MODE )
				{
					xTaskCreate( ploopTask, "ploop", configMINIMAL_STACK_SIZE*2, NULL, 3, &ploopTaskHandle );
					xTaskCreate( interpolationTask, "interpolationTask", configMINIMAL_STACK_SIZE*2, NULL, 4, &interpolationTaskHandle );
				}
				else if( ModesofOperation.LastParam == PROFILE_VELOCITY_MODE )
				{
					xTaskCreate( ploopTask, "ploop", configMINIMAL_STACK_SIZE*2, NULL, 3, &ploopTaskHandle );
					xTaskCreate( interpolationTask, "interpolationTask", configMINIMAL_STACK_SIZE*2, NULL, 4, &interpolationTaskHandle );
				}
				else if( ModesofOperation.LastParam == TORQUE_PROFILE_MODE )
				{
					xTaskCreate( ploopTask, "ploop", configMINIMAL_STACK_SIZE*2, NULL, 3, &ploopTaskHandle );
					xTaskCreate( interpolationTask, "interpolationTask", configMINIMAL_STACK_SIZE*2, NULL, 4, &interpolationTaskHandle );
				}
				else if( ModesofOperation.LastParam == HOMING_MODE )
				{
					// Reserved
				}
				else if( ModesofOperation.LastParam == INTERPOLATED_POSITION_MODE )
				{
					// Reserved
				}
				break;
			default:
				ModesofOperation.Param = NO_MODE;
				break;
		}
		ModesofOperation.LastParam = ModesofOperation.Param;
	}
}
void errorManage( void )
{

}
void stateManageInit( void )
{
	changeStateToStarting();
}
/*
	* Manage Status
	* According to the ControlWord and Some Sensor Value
	* Change the StatusWord
*/
void statusManage( void )
{
	// Operation Mode
	int temp = ((ModesofOperation.Param)&0x0f)<<12;
	StatusWord.Param |= 	temp;
	StatusWord.Param &= 	temp;
	
	// Switch on
	if((ControlWord.Param&CONTROLWORD_SWITCH_ON) == CONTROLWORD_SWITCH_ON )
	{
		StatusWord.Param |= STATUSWORD_BIT1;
	}
	else
	{
		StatusWord.Param &= ~STATUSWORD_BIT1;
	}
	
	// Votagle Encable
	if( ADC1->JDR4 <= 1000 )
	{
		StatusWord.Param |= STATUSWORD_BIT4;
	}
	else
	{
		StatusWord.Param &=~ STATUSWORD_BIT4;
	}
	// Fault Dectection
	if( checkFaults() )
	{
		StatusWord.Param |= STATUSWORD_BIT3;
	}
	else
	{
		StatusWord.Param &=~ STATUSWORD_BIT3;
	}
	// Ready to switch on
	if( checkReady() )
	{
		StatusWord.Param |= STATUSWORD_READY_BIT;
	}
	else
	{
		StatusWord.Param &=~ STATUSWORD_READY_BIT;
	}
}
/*
	* Manage Drive State
	* 
*/
void stateManage( void )
{
		switch( state.State )
		{
			// Starting Mode: 
			// if 1. Mode is set
			//		2. Motortype is set
			//  	3. Errors are cleared
			// 	next state go to STATE_READY
			// eles stay at Current Mode
			case STATE_STARTING:
				if( checkReady() )
					changeStateToReady();
				break;
			case STATE_READY:
				if( (StatusWord.Param&STATUSWORD_SWITCHON_BIT)==STATUSWORD_SWITCHON_BIT  )
					changeStateToRunning();
				if( (StatusWord.Param&STATUSWORD_FAULT_BIT)==STATUSWORD_FAULT_BIT  )
					changeStateToError();
				break;
			case STATE_STOP:
				if( (ControlWord.Param&CONTROLWORD_SWITCH_ON) == CONTROLWORD_SWITCH_ON  )
					changeStateToRunning();
				break;
			case STATE_ERROR:
					if( (StatusWord.Param&STATUSWORD_READY_BIT)==STATUSWORD_READY_BIT )
					{
						changeStateToReady();
					}				
				break;
			case STATE_RUNNING:
				if( (ControlWord.Param&CONTROLWORD_SWITCH_ON) != CONTROLWORD_SWITCH_ON )
				{
					if( (StatusWord.Param&STATUSWORD_READY_BIT)==STATUSWORD_READY_BIT )
					{
						changeStateToReady();
					}
					else if((StatusWord.Param&STATUSWORD_FAULT_BIT)==STATUSWORD_FAULT_BIT)
					{
						changeStateToError();
					}
					//else if()
				}
				break;			
			default:
				changeStateToError();
				break;
		}
		state.lastState = state.State;
}

void driveStateManageTask( void *pvParameters )
{
	TickType_t xLastWakeTime;
	stateManageInit();
	
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		vTaskDelayUntil( &xLastWakeTime, 10 );
		// This task runs at 100Hz
		modeManage();
		errorManage();
		stateManage();
		statusManage();
	}
}

/*========================== END OF FILE =======================================*/

