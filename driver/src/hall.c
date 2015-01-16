/*
================================================================================
* File Name                 :   hall.c
* Copyright                 :   2011-2013
* 
* CPU                       :   STM32F103
* RTOS                      :   FreeRTOS
*
* Create Date               :   2012/12/1
* Author/Company            :   Zhan Yubo/UESTC
* Abstract Description      :   Configuration and Applications of hall sensor.
*...............................................................................
* Revision History
* NO.     Date      Revised by      Item      Description
*	NO.1		2013/7/16	Zhan Yubo				Note			Added some necessary notes.
*
================================================================================
*/

#include"includes.h"
#include"hall.h"
#include"pwm.h"


int hallValue;
int speed = 0;
int hallBuf[6]={3,2,6,4,5,1};


void HALL_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  /*PA-LED*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOHALL,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_HALL1+GPIO_Pin_HALL2+GPIO_Pin_HALL3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOHALL,&GPIO_InitStructure);
}
/*
* 霍尔传感器换向任务
*/
void hallTask( void *pvParameters )
{
	HALL_Configuration();
	for(;;)
	{
		vTaskDelay(1);
		hallValue = (GPIOHALL->IDR&(GPIO_Pin_HALL1+GPIO_Pin_HALL2+GPIO_Pin_HALL3));
//		if(speed<0)
//			switch(hallValue)
//			{
//				case (hallBuf[0])://B->A
//					TIM_SetCompare1(TIM1,0);
//					TIM_SetCompare2(TIM1,-speed);//	
//					TIM_SetCompare3(TIM1,0);//			
//					PWMA_EN();
//					PWMB_EN();
//					PWMC_DIS();
//					break;
//				case hallBuf[1]://B->C
//					TIM_SetCompare1(TIM1,0);
//					TIM_SetCompare2(TIM1,-speed);//	
//					TIM_SetCompare3(TIM1,0);//			
//					PWMA_DIS();
//					PWMB_EN();
//					PWMC_EN();
//					break;
//				case hallBuf[2]://A->C
//					TIM_SetCompare1(TIM1,-speed);
//					TIM_SetCompare2(TIM1,0);//	
//					TIM_SetCompare3(TIM1,0);//			
//					PWMA_EN();
//					PWMB_DIS();
//					PWMC_EN();
//					break;
//				case hallBuf[3]://A->B
//					TIM_SetCompare1(TIM1,-speed);
//					TIM_SetCompare2(TIM1,0);//	
//					TIM_SetCompare3(TIM1,0);//			
//					PWMA_EN();
//					PWMB_EN();
//					PWMC_DIS();
//					break;
//				case hallBuf[4]://C->B
//					TIM_SetCompare1(TIM1,0);
//					TIM_SetCompare2(TIM1,0);//	
//					TIM_SetCompare3(TIM1,-speed);//			
//					PWMA_DIS();
//					PWMB_EN();
//					PWMC_EN();		
//					break;
//				case hallBuf[5]://C->A
//					TIM_SetCompare1(TIM1,0);
//					TIM_SetCompare2(TIM1,0);//	
//					TIM_SetCompare3(TIM1,-speed);//			
//					PWMA_EN();
//					PWMB_DIS();
//					PWMC_EN();
//								
//					break;
//				default: 
//					TIM_SetCompare1(TIM1,0);
//					TIM_SetCompare2(TIM1,0);//	
//					TIM_SetCompare3(TIM1,0);//			
//					PWMA_DIS();
//					PWMB_DIS();
//					PWMC_DIS();					
//				break;
//			}
//		else
//			switch(hallValue)
//			{
//				case hallBuf[0]://A->B
//					TIM_SetCompare1(TIM1,speed);
//					TIM_SetCompare2(TIM1,0);//	
//					TIM_SetCompare3(TIM1,0);//			
//					PWMA_EN();
//					PWMB_EN();
//					PWMC_DIS();
//					break;
//				case hallBuf[1]://C->B
//					TIM_SetCompare1(TIM1,0);
//					TIM_SetCompare2(TIM1,0);//	
//					TIM_SetCompare3(TIM1,speed);//			
//					PWMA_DIS();
//					PWMB_EN();
//					PWMC_EN();
//					break;
//				case hallBuf[2]://C->A
//					TIM_SetCompare1(TIM1,0);
//					TIM_SetCompare2(TIM1,0);//	
//					TIM_SetCompare3(TIM1,speed);//			
//					PWMA_EN();
//					PWMB_DIS();
//					PWMC_EN();
//					break;
//				case hallBuf[3]://B->A
//					TIM_SetCompare1(TIM1,0);
//					TIM_SetCompare2(TIM1,speed);//	
//					TIM_SetCompare3(TIM1,0);//			
//					PWMA_EN();
//					PWMB_EN();
//					PWMC_DIS();
//					break;
//				case hallBuf[4]://B->C
//					TIM_SetCompare1(TIM1,0);
//					TIM_SetCompare2(TIM1,speed);//	
//					TIM_SetCompare3(TIM1,0);//			
//					PWMA_DIS();
//					PWMB_EN();
//					PWMC_EN();		
//					break;
//				case hallBuf[5]://A->C
//					TIM_SetCompare1(TIM1,speed);
//					TIM_SetCompare2(TIM1,0);//	
//					TIM_SetCompare3(TIM1,0);//			
//					PWMA_EN();
//					PWMB_DIS();
//					PWMC_EN();
//								
//					break;
//				default: 
//					TIM_SetCompare1(TIM1,0);
//					TIM_SetCompare2(TIM1,0);//	
//					TIM_SetCompare3(TIM1,0);//			
//					PWMA_DIS();
//					PWMB_DIS();
//					PWMC_DIS();						
//				break;
//			}	
	}
}
/*========================== END OF FILE =======================================*/
