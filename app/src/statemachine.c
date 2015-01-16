#include "statemachine.h"
#include "includes.h"
#include "canopen.h"
#include "memory.h"
#include "pwm.h"
#include "test.h"
#include "led.h"
#include "dma.h"
#include "can.h"

int current_overtime=0;
int lowvoltage_time=0;

void CheckFault( void )
{
	if( V_BUS < 450 )//600 2300
	{
		lowvoltage_time++;
	}
	else
	{
		if(lowvoltage_time>0)
			lowvoltage_time--;
	}
	switch((ControlWord.Param)&CONTROLWORD_SWITCH_ON)
	{
		case CONTROLWORD_SWITCH_ON:
			//ledColour = GREEN;
			ledState = ON;
			break;
		case 0:
			//ledColour = GREEN;
			ledState = IDLE;
			break;
		default:
			break;
	}
	if( abs(adc) > MaxCurrent.Param )
	{
		current_overtime++;
	}
	else
	{
		if(current_overtime>0)
			current_overtime--;
	}
	if((current_overtime > 100)||(lowvoltage_time > 100))
	{
		ledColour = RED;
		ledState = IDLE;
		ledDelay = 100;
		Switchoff2();	
	}	
}
void DriveInitialize( void )
{
	MotorType.Param=MOTORTYPE_BDCMOTOR;
	ModesofOperation.Param=HOMING_MODE;
}
void DriveSelftest( void )
{

}

void ActivateCommunication( void )
{

}

void RunStateMachine( void )
{
	int state = Checkstatus(StatusWord.Param);
	
	switch( state )
	{
		case STATE_START:
			//Event: reset
			//Action: The drive self-tests and/or initialized successfully
			DriveInitialize();
			DriveSelftest();
			//Event: A fault has accured in the drive.
			//Action: Execute appropriate fault reaction.		
			break;
		case STATE_NOT_READY_TO_SWITCH_ON:
			//Event: The drive has self-tested and/or initialized successfully
			//Action: Activate communication
			ActivateCommunication();
			//Event: A fault has accured in the drive.
			//Action: Execute appropriate fault reaction.			
			
			break;
		case STATE_SWITCH_ON_DISABLED:
			//Event: 'Shutdown' command received from host
			//Action: None
		
			//Event: A fault has accured in the drive.
			//Action: Execute appropriate fault reaction.	
			break;
		case STATE_READY_TO_SWITCH_ON:
			//Event: 'Switch On' Command received from host
			//Action: The power section is switched on if it is not already switched on.
		
			//Event: 'Quick Stop' and 'Disable Voltage' command received from host
			//Action: None.
		
			//Event: A fault has accured in the drive.
			//Action: Execute appropriate fault reaction.			
			break;
		case STATE_SWITCHED_ON:
			//Event: 'Enable Operation' command received frim host.
			//Action: The drive operation will be disabled.
			
			//Event: 'Shutdown' command received from host.
			//Action: The power section is switched off.
			
			//Event: 'Disable Voltage' or 'Quick Stop' command received from host.
			//Action: The power section is switched off immediately, and the motor is free to rotate if unbraked. 
		
			//Event: A fault has accured in the drive.
			//Action: Execute appropriate fault reaction.		
			break;		
		case STATE_OPERATION_ENABLED:
			//Event: 'Disable Operation' command received from host.
			//Action: The drive operation will be disabled.
		
			//Event: 'Shutdown' command received from host.
			//Action: The power section is switched off immediately, and the motor is free to rotate if unbraked. 
		
			//Event: 'Disable Voltage' command received from host.
			//Action: The power section is switched off immediately, and the motor is free to rotate if unbraked. 
			
			//Event:	'Quick Stop' command received from host.
			//Action:	The quick stop function is executed.
			
			//Event: A fault has accured in the drive.
			//Action: Execute appropriate fault reaction.
			break;
		case STATE_QUICK_STOP_ACTIVE:
			//Event: 'Quick Stop' is completed or 'Disable Voltage' command received from host.
			//Action: The Power section if switched off.
		
			//Event: 'Enable Operation' command received from host.
			//Action: The drive function is enabled.
		
			//Event: A fault has accured in the drive.
			//Action: Execute appropriate fault reaction.		
			break;
		case STATE_FAULT_REACTION_ACTIVE:
			//Event: The fault reaction is completed.
			//Action: The drive fuction is disabled. The power section may be switched off.

			//Event: A fault has accured in the drive.
			//Action: Execute appropriate fault reaction.			
			break;
		case STATE_FAULT:
			//Event: 'Fault Reset' command received from host.
			//Action: A reset of the fault condition is carried out if no fault exists currently on the drive.
			//				Afer leaving the state Fault the Bit 'Fault Reset' of the controlword has to be cleared by the host.
		
			//Event: A fault has accured in the drive.
			//Action: Execute appropriate fault reaction.	
			break;
		default: 
			break;
	}
	
}

void DriveTask( void *pvParameters )
{
	memory.maxpwm = T_PWM-10;
	memory.VPIFF.kp = 20;
	memory.VPIFF.ki = 2;
	memory.VPIFF.kff = 0;
	memory.VPIFF.maxTError = 700;
	memory.VPIFF.TError = 700;
	memory.PIFF.maxTError = 700;
	//memory.PIFF.TError = 700;
//	memoryRemberAll(&memory);
	memory.PIFF.kp = 1;
	memory.PIFF.ki = 0;
	memory.PIFF.kff = 0;
	//vTaskDelay(1000);
	//DriveInitialize();
//	MotorType.Param=MOTORTYPE_BDCMOTOR;
//	ModesofOperation.Param=PROFILE_VELOCITY_MODE;	
//	Switchon2();
	//TargetTorque.Param = -36;
//	TargetVelocity.Param = -50;
	//pwm = 400;
	//TargetTorque.Param = -50;
	
	//xTaskCreate( testCANTask, "testCANTask", configMINIMAL_STACK_SIZE*2, NULL, 2, NULL );
	//vTaskDelete(NULL);
	MaxCurrent.Param = 250;
	vTaskDelay(200);
	for(;;)
	{
		CheckFault();
//		RunStateMachine();
//		vTaskDelay(1);
		vTaskDelay(10);
//		TargetVelocity.Param = -100;
//		//TargetTorque.Param = 30;
//		vTaskDelay(2000);
//		TargetVelocity.Param = 100;
//		//TargetTorque.Param = -30;
		
	}
}

void DriveTask2( void *pvParameters )
{
	memory.PIFF.kp = 1.5;
	memory.PIFF.ki = 0.2;
	memory.PIFF.kff = 1;
	//vTaskDelay(1000);
	//DriveInitialize();
	MotorType.Param=MOTORTYPE_PMSMOTOR;
	ModesofOperation.Param=TORQUE_PROFILE_MODE;
	vTaskDelay(3000);
	//Switchon2();
	//TargetTorque.Param = -36;
	pwm = T_PWM/2;
	for(;;)
	{
		CheckFault();
//		RunStateMachine();
//		vTaskDelay(1);
		vTaskDelay(500);
//		TargetVelocity.Param = -200;
//		//TargetTorque.Param = 30;
//		vTaskDelay(5000);
//		TargetVelocity.Param = 200;
//		//TargetTorque.Param = -30;
		
	}	
}
/* 
**** Position Loop
*/
void DrivePos( void *pvParameters )
{
	
	for(;;)
	{
		vTaskDelay(10);
		CAN_send(0x00,POSITION_ACTUAL_VALUE_INDEX,0,ActualPosition.Param);//Send Position Information
		if((ControlWord.Param&CONTROLWORD_CLEAR_POSITION)==CONTROLWORD_CLEAR_POSITION)// if Clear Position Command is received
		{
			ControlWord.Param&=~CONTROLWORD_CLEAR_POSITION;
			ActualPosition.Param = 0;
			TargetPosition.Param = 0;
		}
		if(abs(ActualPosition.Param-TargetPosition.Param)> 1000)
		{
			if(ActualPosition.Param>TargetPosition.Param)
			{
				TargetVelocity.Param = -20;
			}
			else
			{
				TargetVelocity.Param = 20;
			}
		}
		else
		{
//			if(ActualPosition.Param>TargetPosition.Param)
//			{
//				//TargetVelocity.Param = -10;
//				TargetVelocity.Param = -(TargetPosition.Param-ActualPosition.Param)*(TargetPosition.Param-ActualPosition.Param)/25000;
//			}
//			else
//			{
//				//TargetVelocity.Param = 10;
//				TargetVelocity.Param = (TargetPosition.Param-ActualPosition.Param)*(TargetPosition.Param-ActualPosition.Param)/25000;
//			}
			TargetVelocity.Param = (TargetPosition.Param-ActualPosition.Param)/50;
			
		}
	}	
}
