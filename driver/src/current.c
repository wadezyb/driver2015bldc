/*
================================================================================
* File Name                 :   current.c
* Copyright                 :   2011-2013
* 
* CPU                       :   STM32F103
* RTOS                      :   FreeRTOS
*
* Create Date               :   2012/12/1
* Author/Company            :   Zhan Yubo/UESTC
* Abstract Description      :   Configuration and Applications of Current Loop.
*...............................................................................
* Revision History
* NO.     Date      Revised by      Item      Description
*	NO.1		2013/7/16	Zhan Yubo				Note			Added some necessary notes.
*
================================================================================
*/

#include "includes.h"
#include "current.h"
#include "stm32_dsp.h"
#include "ad.h"
#include "pwm.h"
#include "memory.h"
#include "canopen.h"

int pwm = 0;

void PWM(int hallValue, int pwm)
{
	if (pwm>=0)
	{
		if(MotorType.Param==MOTORTYPE_BDCMOTOR)//直流有刷电机
		{
				TIM_SetCompare1(TIM1,pwm);
				GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
				TIM_SetCompare3(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);			
		}
		else if(MotorType.Param==MOTORTYPE_TPMBLMOTOR)//直流无刷电机带霍尔
		{
			switch(hallValue)
			{
				case 6://A->B
				TIM_SetCompare1(TIM1,pwm);
				GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
				TIM_SetCompare2(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
				TIM_SetCompare3(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);				
					break;
				case 4://A->C
				TIM_SetCompare1(TIM1,pwm);
				GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
				TIM_SetCompare2(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
				TIM_SetCompare3(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);				
					break;
				case 5://B->C
				TIM_SetCompare1(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
				TIM_SetCompare2(TIM1,pwm);
				GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
				TIM_SetCompare3(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);				
					break;
				case 1://B->A
				TIM_SetCompare1(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
				TIM_SetCompare2(TIM1,pwm);
				GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
				TIM_SetCompare3(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);
					break;
				case 3://C->A
				TIM_SetCompare1(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
				TIM_SetCompare2(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
				TIM_SetCompare3(TIM1,pwm);
				GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);				
					break;
				case 2://C->B
					TIM_SetCompare1(TIM1,0);
					GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
					TIM_SetCompare2(TIM1,0);
					GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
					TIM_SetCompare3(TIM1,pwm);
					GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);				
					break;
				default:
					GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
					GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
					GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);				
					break;
			}
		}
		else if(MotorType.Param == MOTORTYPE_SPMBLMOTOR)
		{
			
		}
	}	
	else
	{	
		pwm = -pwm;
		if(MotorType.Param==MOTORTYPE_BDCMOTOR)//直流有刷电机
		{
				TIM_SetCompare1(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
				TIM_SetCompare3(TIM1,pwm);
				GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);			
		}
		else if(MotorType.Param==MOTORTYPE_TPMBLMOTOR)//直流无刷电机带霍尔
		{
			switch(hallValue)
			{
				case 1://A->B
				TIM_SetCompare1(TIM1,pwm);
				GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
				TIM_SetCompare2(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
				TIM_SetCompare3(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);				
					break;
				case 3://A->C
				TIM_SetCompare1(TIM1,pwm);
				GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
				TIM_SetCompare2(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
				TIM_SetCompare3(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);				
					break;
				case 2://B->C
				TIM_SetCompare1(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
				TIM_SetCompare2(TIM1,pwm);
				GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
				TIM_SetCompare3(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);				
					break;
				case 6://B->A
				TIM_SetCompare1(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
				TIM_SetCompare2(TIM1,pwm);
				GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
				TIM_SetCompare3(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);
					break;
				case 4://C->A
				TIM_SetCompare1(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
				TIM_SetCompare2(TIM1,0);
				GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
				TIM_SetCompare3(TIM1,pwm);
				GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);				
					break;
				case 5://C->B
					TIM_SetCompare1(TIM1,0);
					GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
					TIM_SetCompare2(TIM1,0);
					GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
					TIM_SetCompare3(TIM1,pwm);
					GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);				
					break;
				default:
					GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
					GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
					GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);				
					break;
			}		
		}	
	}
}

/*========================== END OF FILE =======================================*/
