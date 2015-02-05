/**
  ******************************************************************************
  * @file    cli.c
  * @author  ZHAN Yubo
  * @version V9.0.0
  * @date    03-May-2014
  * @brief   This file is Command Line Interface for STM32F405RGT6
  *
*/
/* INLCUDES */
#include "includes.h"
#include "cli.h"
#include "sci.h"
#include "led.h"
#include "pwm.h"
#include <string.h>
#include "test.h"
#include "canopen.h"
#include "cloop.h"
#include "vloop.h"
#include "ploop.h"
#include "can.h"
#include "interpolation.h"
#include "statemonitor.h"

/* Pivate Variables */
int feedback=FEEDBACK_NONE;//YES NO NONE
int A=0;
/**
  * @brief  Calc the command length.
  * @param  *string : pointer to the string
  * @retval length of the command
  */
int cmdlen(char *string)
{
	int len = 0;
	while( (*string != ' ')&&( *string != '\0' ))
	{
		string++;
		len++;
	}
	return len;	
}

/**
  * @brief  Change the string to a float number
  * @param  *string : pointer to the string
  * @retval  a float number
  */
float string2number( const char *string )
{
	int length = 0;
	long dot = 0;
	int i=0;
	float result=0;
	int k=0;
	length = strlen(string);
	
	for(i=0;i<length;i++)
	{
		if(string[i]=='.')
		{
			dot = length-i;
		}
	}

	for(i=length-1;i>=0;i--)
	{
		k = length-dot-1-i;
		if(k<0)
			k=k+1;
		switch(string[i])
		{
			case '0': result += 0;break;
			case '1': result += 1*pow(10,k);break;
			case '2': result += 2*pow(10,k);break;
			case '3': result += 3*pow(10,k);break;
			case '4': result += 4*pow(10,k);break;
			case '5': result += 5*pow(10,k);break;
			case '6': result += 6*pow(10,k);break;
			case '7': result += 7*pow(10,k);break;
			case '8': result += 8*pow(10,k);break;
			case '9': result += 9*pow(10,k);break;
			case '-': result = result*(-1);break;
			case '.': break;
			default: result = 0;sciSendString("\r\nInvalid Parameter!!",bMessage);return 0;
		}
	}	
	return result;
}
/***
	* check the string are numbers or not
	* if numbers return 1
	* else return 0
*/

/**
  * @brief  check the string are numbers or not
  * @param  *string : pointer to the string
  * @retval if numbers return 1 else return 0
  */
int checkNumbers(const char *string)
{
	int length = 0;
	int i=0;
	length = strlen(string);

	for(i=length-1;i>=0;i--)
	{
		switch(string[i])
		{
			case '0': break;
			case '1': break;
			case '2': break;
			case '3': break;
			case '4': break;
			case '5': break;
			case '6': break;
			case '7':	break;
			case '8': break;
			case '9': break;
			case '-': break;
			case '.': break;
			default: return 0;
		}
	}	
	return 1;
}

void createCommandList(commandListType* commandList)
{
	commandList->head = 0;
	commandList->tail = commandList->head;
}

void addCommandToList(commandType* command,commandListType* commandList)
{
	
}
/**
  * @brief  Find the command in the list
  * @param  None
  * @retval None
  */ 
void findCommandInList(char* string)
{

	commandType command;
	int i=0;
	char stringBuf[100];
	
	/* calc the length of the cmd */
	command.NameLength = cmdlen(string);

	/* calc the length of the whole string */
	command.StringLength = strlen(string);

	/* calc the length of parameters */
	if(command.StringLength-command.NameLength>0)
		command.ParameterLength = command.StringLength-command.NameLength-1;
	else
		command.ParameterLength = 0;

	/* copy the cmd string from the whole string */
	for(i=0;i<command.NameLength;i++)
	{
		command.Name[i]=string[i];
	}
	for(i=i;i<MaxCmdLength;i++)
	{
		command.Name[i]='\0';
	}

	/* if there is parameter, copy it to parameter string */
	if(command.ParameterLength!=0)
	{
		for(i=command.NameLength+1;i<command.StringLength;i++)
		{
			command.Parameter[i-command.NameLength-1]=string[i];
		}
		for(i=i;i-command.NameLength-1<MaxParameterLength;i++)
		{
			command.Parameter[i-command.NameLength-1]='\0';
		}
	}
	else
	for(i=0;i<MaxParameterLength;i++)
	{
		command.Parameter[i]='\0';
	}
	
	/* if the Parameters are numbers, chage them to a float number*/
	if(checkNumbers(command.Parameter))
	{
		command.parameter = string2number(command.Parameter);
	}
//	/* DISPLAY */
//		if(strcmp(command.Name,"display")==0)
//	{
//		if(command.ParameterLength!=0)
//		{
//			if(command.parameter)
//			{
//				xTaskCreate( displayTask, "display", configMINIMAL_STACK_SIZE, NULL, 5, &displayTaskHandle );
//			}
//			else
//			{
//				vTaskDelete(displayTaskHandle);
//			}
//		}		
//		sciSendString("\r\n",bMessage);
//		sciSendString(command.Name,bMessage);
//		sciSendString(" ",bMessage);
//		sciSendString(int2String(command.parameter,stringBuf),bMessage);		
//	}	
	/* search the command */
	if(strcmp(command.Name,"yes")==0)
	{
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		feedback = FEEDBACK_YES;
	}
	else if(strcmp(command.Name,"no")==0)
	{
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		feedback = FEEDBACK_NO;
	}
//	/* comand for FLASH */
//	else if(strcmp(command.Name,"save")==0)
//	{
//		sciSendString("\r\n",bMessage);
//		sciSendString(command.Name,bMessage);
//		memoryRemberAll(&memory);
//	}
//	else if(strcmp(command.Name,"config")==0)
//	{
//		initAllParameters(&memory);
//		sciSendString("\r\n",bMessage);
//		sciSendString(command.Name,bMessage);
//	}
	
//	/* pwm */
//	else if(strcmp(command.Name,"pwm")==0)
//	{
//		if(command.ParameterLength!=0)
//		{
//			pwm = command.parameter;					
//		}
//		sciSendString("\r\n",bMessage);
//		sciSendString(command.Name,bMessage);
//		sciSendString(" ",bMessage);
//		sciSendString(float2String(pwm,3,stringBuf),bMessage);		
//	}	
//	else if(strcmp(command.Name,"v")==0)
//	{
//		if(command.ParameterLength!=0)
//		{
//			speed = command.parameter;					
//		}
//		sciSendString("\r\n",bMessage);
//		sciSendString(command.Name,bMessage);
//		sciSendString(" ",bMessage);
//		sciSendString(float2String(speed,3,stringBuf),bMessage);							
//	}
//	else if(strcmp(command.Name,"check")==0)
//	{
//		xTaskCreate( checkTask, "HALL", configMINIMAL_STACK_SIZE, NULL, 4, NULL );
//		sciSendString("\r\nChecking...",bMessage);
//	}
	#if (CAN_NODE != CAN_NODE_MASTER)
	else if(strcmp(command.Name,"MO")==0)
	{
		if(command.ParameterLength!=0)
		{
			if( command.parameter == 1 )
			{
				ControlWord.Param|=CONTROLWORD_SWITCH_ON;
				PWMON();
				sciSendString("Run\r\n",bMessage);
			}
			else if( command.parameter == 0 )
			{
				ControlWord.Param&=~CONTROLWORD_SWITCH_ON;
				PWMOFF();
				sciSendString("\r\nStop",bMessage);				
			}			
		}
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		if( (ControlWord.Param&CONTROLWORD_SWITCH_ON) == CONTROLWORD_SWITCH_ON )
			sciSendString(" 1\r\n",bMessage);		
		else
			sciSendString(" 0\r\n",bMessage);		
		
	}
	else if(strcmp(command.Name,"k")==0)
	{
		PWMOFF();
		ControlWord.Param&=~CONTROLWORD_SWITCH_ON;
		sciSendString("\r\nEmergencyStop",bMessage);
	}
	else if(strcmp(command.Name,"cmdI")==0)
	{
		if(command.ParameterLength!=0)
		{
			Current.targetCurrent = command.parameter;					
		}
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		sciSendString(float2String(Current.targetCurrent,3,stringBuf),bMessage);
	}
	else if(strcmp(command.Name,"kcp")==0)
	{
		if(command.ParameterLength!=0)
		{
			Current.dPI.kp= command.parameter;					
			Current.qPI.kp= command.parameter;					
		}
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		sciSendString(float2String(Current.dPI.kp,3,stringBuf),bMessage);
	}	
	else if(strcmp(command.Name,"kci")==0)
	{
		if(command.ParameterLength!=0)
		{
			Current.dPI.ki= command.parameter;					
			Current.qPI.ki= command.parameter;					
		}
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		sciSendString(float2String(Current.dPI.ki,3,stringBuf),bMessage);
	}
	else if(strcmp(command.Name,"TCI")==0)
	{
		if(command.ParameterLength!=0)
		{
			Cti = command.parameter;
		}
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		sciSendString(float2String(Cti,3,stringBuf),bMessage);
	}	
	else if(strcmp(command.Name,"tC")==0)
	{
		if(command.ParameterLength!=0)
		{
			tuningStartFlag=1;
			Cn = 0;
			PWMON();
			ControlWord.Param|=CONTROLWORD_SWITCH_ON;			
		}
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		sciSendString(float2String(tuningStartFlag,3,stringBuf),bMessage);
		vTaskDelay(100);
		PWMOFF();
		ControlWord.Param&=~CONTROLWORD_SWITCH_ON;
		sciSendString("\r\n",bMessage);
		sciSendString("Id:\r\n",bMessage);
//		Cn=0;
//		while( Cn < 1000 )
//		{
//			sciSendString(int2String(Cd[Cn],stringBuf),bMessage);
//			sciSendString(",",bMessage);
//			Cn++;
//			vTaskDelay(5);
//		}
		Cn=0;
		sciSendString("\r\n",bMessage);
		sciSendString("Iq:\r\n",bMessage);		
		while( Cn < 200 )
		{
			sciSendString(int2String(Cq[Cn],stringBuf),bMessage);
			sciSendString(",",bMessage);
			Cn++;
			vTaskDelay(2);
		}
		Cn=0;
	}	
/*---------------------------------------------------------------------------*/
	else if(strcmp(command.Name,"cmdV")==0)
	{
		if(command.ParameterLength!=0)
		{
			Velocity.targetVelocity= command.parameter;					
		}
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		sciSendString(float2String(Velocity.targetVelocity,3,stringBuf),bMessage);
	}
	else if(strcmp(command.Name,"kvp")==0)
	{
		if(command.ParameterLength!=0)
		{
			Velocity.vPI.kp= command.parameter;					
		}
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		sciSendString(float2String(Velocity.vPI.kp,3,stringBuf),bMessage);
	}	
	else if(strcmp(command.Name,"kvi")==0)
	{
		if(command.ParameterLength!=0)
		{
			Velocity.vPI.ki= command.parameter;					
		}
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		sciSendString(float2String(Velocity.vPI.ki,3,stringBuf),bMessage);
	}	
//	else if(strcmp(command.Name,"kvd")==0)
//	{
//		if(command.ParameterLength!=0)
//		{
//			Velocity.vPI.kd= command.parameter;					
//		}
//		sciSendString("\r\n",bMessage);
//		sciSendString(command.Name,bMessage);
//		sciSendString(" ",bMessage);
//		sciSendString(float2String(Velocity.vPI.kd,3,stringBuf),bMessage);
//	}
	else if(strcmp(command.Name,"TV")==0)
	{
		if(command.ParameterLength!=0)
		{
			Vt = command.parameter;
		}
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		sciSendString(float2String(Vt,3,stringBuf),bMessage);
	}	
	else if(strcmp(command.Name,"tV")==0)
	{
		if(command.ParameterLength!=0)
		{
			vloopTuningStartFlag=1;
			Vn = 0;
			PWMON();
			ControlWord.Param|=CONTROLWORD_SWITCH_ON;			
		}
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		sciSendString(float2String(vloopTuningStartFlag,3,stringBuf),bMessage);
		vTaskDelay(2000);
		PWMOFF();
		ControlWord.Param&=~CONTROLWORD_SWITCH_ON;
		sciSendString("\r\n",bMessage);
		sciSendString("\r\n",bMessage);
		Vn=0;
		sciSendString("\r\n",bMessage);
		sciSendString("V:\r\n",bMessage);		
		while( Vn < 1000 )
		{
			sciSendString(int2String(V[Vn],stringBuf),bMessage);
			sciSendString(",",bMessage);
			Vn++;
			vTaskDelay(1);
		}
		Vn=0;
//		sciSendString("\r\n",bMessage);
//		sciSendString("Vo:\r\n",bMessage);		
//		while( Vn < 1000 )
//		{
//			sciSendString(int2String(Vo[Vn],stringBuf),bMessage);
//			sciSendString(",",bMessage);
//			Vn++;
//			vTaskDelay(1);
//		}
//		Vn=0;		
	}	
/*---------------------------------------------------------------------------*/	

/*------------------------------------------------------------------------------*/
	else if(strcmp(command.Name,"cmdP")==0)
	{
		if(command.ParameterLength!=0)
		{
			Position.targetPosition = command.parameter;					
		}
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		sciSendString(int2String(Position.targetPosition,stringBuf),bMessage);
	}
	else if(strcmp(command.Name,"kpp")==0)
	{
		if(command.ParameterLength!=0)
		{
			Position.pPD.kp = command.parameter;					
		}
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		sciSendString(int2String(Position.pPD.kp,stringBuf),bMessage);
	}
	else if(strcmp(command.Name,"kpi")==0)
	{
		if(command.ParameterLength!=0)
		{
			Position.pPD.ki = command.parameter;					
		}
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		sciSendString(int2String(Position.pPD.ki,stringBuf),bMessage);
	}		
	else if(strcmp(command.Name,"kpd")==0)
	{
		if(command.ParameterLength!=0)
		{
			Position.pPD.kd = command.parameter;					
		}
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		sciSendString(int2String(Position.pPD.kd,stringBuf),bMessage);
	}	
	else if(strcmp(command.Name,"tP")==0)
	{
		if(command.ParameterLength!=0)
		{
			pTuningStartFlag=1;
			Pn = 0;
			PWMON();
			ControlWord.Param|=CONTROLWORD_SWITCH_ON;
		}
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		sciSendString(float2String(pTuningStartFlag,3,stringBuf),bMessage);
		vTaskDelay(2000);
		PWMOFF();
		ControlWord.Param&=~CONTROLWORD_SWITCH_ON;
		sciSendString("\r\n",bMessage);
		sciSendString("\r\n",bMessage);
		
		Pn=0;
		sciSendString("\r\n",bMessage);
		sciSendString("Pp:\r\n",bMessage);		
		while( Pn < 1000 )
		{
			sciSendString(int2String(Pp[Pn],stringBuf),bMessage);
			sciSendString(",",bMessage);
			Pn++;
			vTaskDelay(1);
		}
		
		Pn=0;
		sciSendString("\r\n",bMessage);
		sciSendString("Pv:\r\n",bMessage);		
		while( Pn < 1000 )
		{
			sciSendString(int2String(Pv[Pn],stringBuf),bMessage);
			sciSendString(",",bMessage);
			Pn++;
			vTaskDelay(1);
		}
		Pn=0;		
		
		sciSendString("\r\n",bMessage);
		sciSendString("Prealp:\r\n",bMessage);		
		while( Pn < 1000 )
		{
			sciSendString(int2String(Pe[Pn],stringBuf),bMessage);
			sciSendString(",",bMessage);
			Pn++;
			vTaskDelay(1);
		}
		Pn=0;	
		sciSendString("\r\n",bMessage);
		sciSendString("Prealv:\r\n",bMessage);		
		while( Pn < 1000 )
		{
			sciSendString(int2String(Po[Pn],stringBuf),bMessage);
			sciSendString(",",bMessage);
			Pn++;
			vTaskDelay(1);
		}
		Pn=0;			
	}		
	else if(strcmp(command.Name,"TP")==0)
	{
		if(command.ParameterLength!=0)
		{
			Tra.Position = command.parameter;					
		}
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		sciSendString(int2String(Tra.Position,stringBuf),bMessage);
	}	
/*------------------------------------------------------------------------------*/
	
/*------------------------------------------------------------------------------*/
	
	#endif
//	#if (CAN_NODE == CAN_NODE_MASTER)
//	else if(strcmp(command.Name,"update")==0)
//	{
//		ControlWord.Param |= CONTROLWORD_UPDATE_POSITION;
//		CAN_send(CAN_NODE1,CONTROLWORD_INDEX,0,ControlWord.Param);
//		CAN_send(CAN_NODE2,CONTROLWORD_INDEX,0,ControlWord.Param);
//		CAN_send(CAN_NODE3,CONTROLWORD_INDEX,0,ControlWord.Param);
//		CAN_send(CAN_NODE4,CONTROLWORD_INDEX,0,ControlWord.Param);
//		ControlWord.Param &= ~CONTROLWORD_UPDATE_POSITION;
//		sciSendString("\r\n",bMessage);
//		sciSendString(command.Name,bMessage);
//	}
//	else if(strcmp(command.Name,"runAll")==0)
//	{
//		Joint1.ControlWord.Param|=CONTROLWORD_SWITCH_ON;
//		Joint2.ControlWord.Param|=CONTROLWORD_SWITCH_ON;
//		Joint3.ControlWord.Param|=CONTROLWORD_SWITCH_ON;
//		Joint4.ControlWord.Param|=CONTROLWORD_SWITCH_ON;
//		sciSendString("\r\nAllJ Running...",bMessage);
//	}
//		else if(strcmp(command.Name,"kAll")==0)
//	{
//		Joint1.ControlWord.Param&=~CONTROLWORD_SWITCH_ON;
//		Joint2.ControlWord.Param&=~CONTROLWORD_SWITCH_ON;
//		Joint3.ControlWord.Param&=~CONTROLWORD_SWITCH_ON;
//		Joint4.ControlWord.Param&=~CONTROLWORD_SWITCH_ON;
//		sciSendString("\r\nAllJ Stop",bMessage);
//	}
//	else if(strcmp(command.Name,"runJ1")==0)
//	{
//		Joint1.ControlWord.Param|=CONTROLWORD_SWITCH_ON;
//		sciSendString("\r\nJ1 Running...",bMessage);
//	}
//	else if(strcmp(command.Name,"kJ1")==0)
//	{
//		Joint1.ControlWord.Param&=~CONTROLWORD_SWITCH_ON;
//		sciSendString("\r\nJ1 Stop...",bMessage);
//	}	
//	else if(strcmp(command.Name,"J1")==0)
//	{
//		if(command.ParameterLength!=0)
//		{
//			Joint1.TargetPosition.Param = command.parameter;					
//		}
//		//CAN_send(CAN_NODE1,POSITION_DEMAND_VALUE_INDEX,0,Joint1.TargetPosition.Param);
//		sciSendString("\r\n",bMessage);
//		sciSendString(command.Name,bMessage);
//		sciSendString(" ",bMessage);
//		sciSendString(int2String(Joint1.ActualPosition.Param,stringBuf),bMessage);
//	}
//	else if(strcmp(command.Name,"J1M")==0)
//	{
//		if(command.ParameterLength!=0)
//		{
//			Joint1.ModesofOperation.Param = command.parameter;					
//		}
//		sciSendString("\r\n",bMessage);
//		sciSendString(command.Name,bMessage);
//		sciSendString(" ",bMessage);
//		sciSendString(int2String(Joint1.ModesofOperation.Param,stringBuf),bMessage);
//	}	
//	else if(strcmp(command.Name,"runJ2")==0)
//	{
//		Joint2.ControlWord.Param|=CONTROLWORD_SWITCH_ON;
//		sciSendString("\r\nJ2 Running...",bMessage);
//	}
//	else if(strcmp(command.Name,"kJ2")==0)
//	{
//		Joint2.ControlWord.Param&=~CONTROLWORD_SWITCH_ON;
//		sciSendString("\r\nJ2 Stop...",bMessage);
//	}		
//	else if(strcmp(command.Name,"J2")==0)
//	{
//		if(command.ParameterLength!=0)
//		{
//			Joint2.TargetPosition.Param = command.parameter;					
//		}
//		//CAN_send(CAN_NODE2,POSITION_DEMAND_VALUE_INDEX,0,Joint2.TargetPosition.Param);
//		sciSendString("\r\n",bMessage);
//		sciSendString(command.Name,bMessage);
//		sciSendString(" ",bMessage);
//		sciSendString(int2String(Joint2.ActualPosition.Param,stringBuf),bMessage);
//	}
//	else if(strcmp(command.Name,"J2M")==0)
//	{
//		if(command.ParameterLength!=0)
//		{
//			Joint2.ModesofOperation.Param = command.parameter;					
//		}
//		sciSendString("\r\n",bMessage);
//		sciSendString(command.Name,bMessage);
//		sciSendString(" ",bMessage);
//		sciSendString(int2String(Joint2.ModesofOperation.Param,stringBuf),bMessage);
//	}		
//	else if(strcmp(command.Name,"runJ3")==0)
//	{
//		Joint3.ControlWord.Param|=CONTROLWORD_SWITCH_ON;
//		sciSendString("\r\nJ3 Running...",bMessage);
//	}
//	else if(strcmp(command.Name,"kJ3")==0)
//	{
//		Joint3.ControlWord.Param&=~CONTROLWORD_SWITCH_ON;
//		sciSendString("\r\nJ3 Stop...",bMessage);
//	}		
//	else if(strcmp(command.Name,"J3")==0)
//	{
//		if(command.ParameterLength!=0)
//		{
//			Joint3.TargetPosition.Param = command.parameter;
//		}
//		//CAN_send(CAN_NODE3,POSITION_DEMAND_VALUE_INDEX,0,Joint3.TargetPosition.Param);
//		sciSendString("\r\n",bMessage);
//		sciSendString(command.Name,bMessage);
//		sciSendString(" ",bMessage);
//		sciSendString(int2String(Joint3.ActualPosition.Param,stringBuf),bMessage);
//	}
//	else if(strcmp(command.Name,"J3M")==0)
//	{
//		if(command.ParameterLength!=0)
//		{
//			Joint3.ModesofOperation.Param = command.parameter;					
//		}
//		sciSendString("\r\n",bMessage);
//		sciSendString(command.Name,bMessage);
//		sciSendString(" ",bMessage);
//		sciSendString(int2String(Joint3.ModesofOperation.Param,stringBuf),bMessage);
//	}		
//	else if(strcmp(command.Name,"runJ4")==0)
//	{
//		Joint4.ControlWord.Param|=CONTROLWORD_SWITCH_ON;
//		sciSendString("\r\nJ4 Running...",bMessage);
//	}
//	else if(strcmp(command.Name,"kJ4")==0)
//	{
//		Joint4.ControlWord.Param&=~CONTROLWORD_SWITCH_ON;
//		sciSendString("\r\nJ4 Stop...",bMessage);
//	}		
//	else if(strcmp(command.Name,"J4")==0)
//	{
//		if(command.ParameterLength!=0)
//		{
//			Joint4.TargetPosition.Param = command.parameter;					
//		}
//		//CAN_send(CAN_NODE4,POSITION_DEMAND_VALUE_INDEX,0,Joint4.TargetPosition.Param);
//		sciSendString("\r\n",bMessage);
//		sciSendString(command.Name,bMessage);
//		sciSendString(" ",bMessage);
//		sciSendString(int2String(Joint4.ActualPosition.Param,stringBuf),bMessage);
//	}
//	else if(strcmp(command.Name,"J4M")==0)
//	{
//		if(command.ParameterLength!=0)
//		{
//			Joint4.ModesofOperation.Param = command.parameter;					
//		}
//		sciSendString("\r\n",bMessage);
//		sciSendString(command.Name,bMessage);
//		sciSendString(" ",bMessage);
//		sciSendString(int2String(Joint4.ModesofOperation.Param,stringBuf),bMessage);
//	}		
//	#endif
//	/* TEST */
//	else if(strcmp(command.Name,"testled")==0)
//	{
//		xTaskCreate( testLEDTask, "testled", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
//		//sciSendString("\r\nTesting...",bMessage);
//	}
//	else if(strcmp(command.Name,"testhall")==0)
//	{
//		xTaskCreate( testHALLTask, "testHALL", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
//		//sciSendString("\r\nTesting...",bMessage);
//	}	
//	else if(strcmp(command.Name,"testpwm")==0)
//	{
//		xTaskCreate( testPWMTask, "testPWM", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
//		//sciSendString("\r\nTesting...",bMessage);
//	}
//	else if(strcmp(command.Name,"testpwm1")==0)
//	{
//		xTaskCreate( testPWM1Task, "testPWM1", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
//		//sciSendString("\r\nTesting...",bMessage);
//	}	
//	else if(strcmp(command.Name,"testpwm2")==0)
//	{
//		xTaskCreate( testPWM2Task, "testPWM2", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
//		//sciSendString("\r\nTesting...",bMessage);
//	}	
//	else if(strcmp(command.Name,"testpwm3")==0)
//	{
//		xTaskCreate( testPWM3Task, "testPWM3", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
//		//sciSendString("\r\nTesting...",bMessage);
//	}		
	else if(strcmp(command.Name,"testcan")==0)
	{
		//xTaskCreate( testCANTask, "testCAN", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
		//sciSendString("\r\nTesting...",bMessage);
	}
//	else if(strcmp(command.Name,"testtemp")==0)
//	{
//		xTaskCreate( testTEMPTask, "testTEMP", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
//		sciSendString("\r\nTesting DS18B20",bMessage);
//	}
//	else if(strcmp(command.Name,"testenc")==0)
//	{
//		xTaskCreate( testENCODERTask, "testENCODER", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
//		sciSendString("\r\nTesting Encoder",bMessage);
//	}
//	else if(strcmp(command.Name,"testsvpwm")==0)
//	{
//		xTaskCreate( testSVPWMTask, "testSVPWM", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
//		sciSendString("\r\nTesting SVPWM",bMessage);
//	}	
//	else if(strcmp(command.Name,"hallValue")==0)
//	{
//		if(command.ParameterLength!=0)
//		{
//			hallValue = command.parameter;					
//		}
//		sciSendString("\r\n",bMessage);
//		sciSendString(command.Name,bMessage);
//		sciSendString(" ",bMessage);
//		sciSendString(float2String(hallValue,3,stringBuf),bMessage);	
//	}	
	else if(strcmp(command.Name,"UM")==0)
	{
		if( state.State == STATE_RUNNING  )
		{
			sciSendString("Cannot chage Unit Mode when Motor is running\r\n",bMessage);
		}
		else
		{
			if(command.ParameterLength!=0)
			{
				ModesofOperation.Param = command.parameter;					
			}
			sciSendString("\r\n",bMessage);
			sciSendString(command.Name,bMessage);
			sciSendString(" ",bMessage);
			sciSendString(float2String(ModesofOperation.Param,3,stringBuf),bMessage);	
			sciSendString(" ",bMessage);
			switch((ModesofOperation.Param))
			{
				case PROFILE_POSITION_MODE:
					sciSendString("PROFILE_POSITION_MODE\r\n",bMessage);
					break;
				case VELOCITY_MODE:
					sciSendString("VELOCITY_MODE\r\n",bMessage);
					break;
				case PROFILE_VELOCITY_MODE:
					sciSendString("PROFILE_VELOCITY_MODE\r\n",bMessage);
					break;
				case TORQUE_PROFILE_MODE:
					sciSendString("TORQUE_PROFILE_MODE\r\n",bMessage);
					break;
				case HOMING_MODE:
					sciSendString("HOMING_MODE\r\n",bMessage);
					break;
				case INTERPOLATED_POSITION_MODE:
					sciSendString("INTERPOLATED_POSITION_MODE\r\n",bMessage);
					break;
				case PROFILE_PWM_MODE:
					sciSendString("PROFILE_PWM_MODE\r\n",bMessage);
					break;
				case NO_MODE:
					sciSendString("NO_MODE\r\n",bMessage);
					break;
				default:
					sciSendString("\r\n",bMessage);
					sciSendString("-1 NO_MODE\r\n",bMessage);
					sciSendString("1 PROFILE_POSITION_MODE\r\n",bMessage);
					sciSendString("2 VELOCITY_MODE\r\n",bMessage);
					sciSendString("3 PROFILE_VELOCITY_MODE\r\n",bMessage);
					sciSendString("4 TORQUE_PROFILE_MODE\r\n",bMessage);
					sciSendString("6 HOMING_MODE\r\n",bMessage);
					sciSendString("7 INTERPOLATED_POSITION_MODE\r\n",bMessage);
					sciSendString("0 LIST MODES\r\n",bMessage);
					break;
			}		
		}

	}
//	else if(strcmp(command.Name,"motortype")==0)
//	{
//		if(command.ParameterLength!=0)
//		{
//			MotorType.Param = command.parameter;					
//		}
//		sciSendString("\r\n",bMessage);
//		sciSendString(command.Name,bMessage);
//		sciSendString(" ",bMessage);
//		sciSendString(float2String(MotorType.Param,3,stringBuf),bMessage);	
//	}		
	else if(strcmp(command.Name,"A")==0)
	{
		if(command.ParameterLength!=0)
		{
			A = command.parameter;					
		}
		sciSendString("\r\n",bMessage);
		sciSendString(command.Name,bMessage);
		sciSendString(" ",bMessage);
		sciSendString(float2String(A,3,stringBuf),bMessage);	
	}


	/* INFO */
	else if(strcmp(command.Name,"hwinfo")==0)
	{
		sciSendString("\r\n",bMessage);
		sciSendString("Hardware Version: ",bMessage);
		sciSendString(float2String(HW_VERSION,1,stringBuf),bMessage);					
	}
	else if(strcmp(command.Name,"swreset")==0)
	{
		sciSendString("\r\n",bMessage);
		sciSendString("System Restarting...",bMessage);		
	}

	
	else if(strcmp(command.Name,"help")==0)
	{
		sciSendString("\r\nrun: turn on\r\n",bMessage);
		sciSendString("k  : turn off\r\n",bMessage);
		sciSendString("cmdI: Command Current\r\n",bMessage);
		
	}			
	/* OTHERS */
	else
	{
		sciSendString("\r\n",bMessage);
		sciSendString("Unknown Command ",bMessage);
	}	
	
	
}



/*========================== END OF FILE =======================================*/
