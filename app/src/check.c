/*
================================================================================
* File Name                 :   check.c
* Copyright                 :   2011-2013
* 
* CPU                       :   STM32F103
* RTOS                      :   FreeRTOS
*
* Create Date               :   2012/12/1
* Author/Company            :   Zhan Yubo/UESTC
* Abstract Description      :   Applications of selfchecking.
*...............................................................................
* Revision History
* NO.     Date      Revised by      Item      Description
*	NO.1		2013/7/16	Zhan Yubo				Note			Added some necessary notes.
*
================================================================================
*/

#include"includes.h"
#include"check.h"
#include"hall.h"
#include"pwm.h"

/*
* Selfchecking Task
*/
void checkTask( void *pvParameters )
{
	vTaskDelay(500);
	PWMA_EN();
	PWMB_DIS();
	PWMC_DIS();
	TIM_SetAutoreload(TIM1,30000-1);//f = 2k
	TIM_SetCompare1(TIM1,1000);//
	TIM_SetCompare2(TIM1,0);
	TIM_SetCompare3(TIM1,0);//
	vTaskDelay(1000);
	PWMA_DIS();
	PWMB_EN();
	PWMC_DIS();
	TIM_SetAutoreload(TIM1,40000-1);//f = 2k
	TIM_SetCompare1(TIM1,0);//
	TIM_SetCompare2(TIM1,1000);
	TIM_SetCompare3(TIM1,0);//	
	vTaskDelay(1000);
	PWMA_DIS();
	PWMB_DIS();
	PWMC_EN();
	TIM_SetAutoreload(TIM1,50000-1);//f = 2k
	TIM_SetCompare1(TIM1,0);//
	TIM_SetCompare2(TIM1,0);
	TIM_SetCompare3(TIM1,1000);//
	vTaskDelay(1000);
	
	PWMA_EN();
	PWMB_EN();
	PWMC_DIS();
	TIM_SetAutoreload(TIM1,36000-1);//f = 2k
	TIM_SetCompare1(TIM1,1000);//
	TIM_SetCompare2(TIM1,0);
	TIM_SetCompare3(TIM1,0);//	
	vTaskDelay(200);
	TIM_SetAutoreload(TIM1,36000-1);//f = 2k
	TIM_SetCompare1(TIM1,0);//
	TIM_SetCompare2(TIM1,1000);
	TIM_SetCompare3(TIM1,0);//	
	vTaskDelay(200);
	
	PWMA_DIS();
	PWMB_EN();
	PWMC_EN();
	
	TIM_SetAutoreload(TIM1,50000-1);//f =1k
	TIM_SetCompare1(TIM1,0);//
	TIM_SetCompare2(TIM1,1000);
	TIM_SetCompare3(TIM1,0);//	
	vTaskDelay(200);
	TIM_SetAutoreload(TIM1,50000-1);//f =1k
	TIM_SetCompare1(TIM1,0);//
	TIM_SetCompare2(TIM1,0);
	TIM_SetCompare3(TIM1,1000);//	
	vTaskDelay(200);
	
	PWMA_EN();
	PWMB_DIS();
	PWMC_EN();
		
	TIM_SetAutoreload(TIM1,60000-1);//f =1k	
	TIM_SetCompare1(TIM1,0);//
	TIM_SetCompare2(TIM1,0);
	TIM_SetCompare3(TIM1,1000);//	
	vTaskDelay(200);
	TIM_SetAutoreload(TIM1,60000-1);//f =1k	
	TIM_SetCompare1(TIM1,1000);//
	TIM_SetCompare2(TIM1,0);
	TIM_SetCompare3(TIM1,0);//	
	vTaskDelay(200);

	PWMA_DIS();
	PWMB_DIS();
	PWMC_DIS();
	
	TIM_SetCompare1(TIM1,0);//
	TIM_SetCompare2(TIM1,0);
	TIM_SetCompare3(TIM1,0);//	
	TIM_SetAutoreload(TIM1,1440-1);//f = 40k
	//xTaskCreate( hallTask, "hall", configMINIMAL_STACK_SIZE, NULL, 4, NULL );
	vTaskDelete(NULL);
}


/*========================== END OF FILE =======================================*/
