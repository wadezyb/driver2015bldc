/**
  ******************************************************************************
  * @file    pwm.c
  * @author  ZHAN Yubo
  * @version V9.0.0
  * @date    03-May-2014
  * @brief   This file is pwm driver for STM32F405RGT6
  *
*/

#include "includes.h"
#include "pwm.h"

/**
  * @brief  Configure the pwm 
	* @param  None
  * @retval None
  * @note PWM Interface:
	* PWM1->PA8->TIM1 CH1
	* PWM2->PA9->TIM1 CH2
	* PWM3->PA10->TIM1 CH3
	* ENA->PB14
	* ENB->PB15
	* ENC->PB13
*/
void pwmConfiguration( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA+RCC_AHB1Periph_GPIOB+RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 + GPIO_Pin_9 + GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_TIM1);

	//en pa6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14+GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&GPIO_InitStructure);	
	
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);
	GPIO_WriteBit(GPIOC,GPIO_Pin_6,Bit_RESET);

	TIM_CounterModeConfig(TIM1,TIM_CounterMode_CenterAligned3);
	/*Set pwm mode*/
	TIM_OCInitStructure.TIM_OCMode =  TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);
	TIM_OC3Init(TIM1,&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);

	TIM_SetCompare1(TIM1,0);//
	TIM_SetCompare2(TIM1,0);
	TIM_SetCompare3(TIM1,0);//
	TIM_SetCompare4(TIM1, 1000);//
	
	TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_OC4Ref);
	
	TIM_SetAutoreload(TIM1,T_PWM-1);//f = 20k
	
	TIM_CCxCmd(TIM1,TIM_Channel_1,TIM_CCx_Enable);
	TIM_CCxCmd(TIM1,TIM_Channel_2,TIM_CCx_Enable);
	TIM_CCxCmd(TIM1,TIM_Channel_3,TIM_CCx_Enable);
	TIM_CCxCmd(TIM1,TIM_Channel_4,TIM_CCx_Enable);

	TIM_CtrlPWMOutputs( TIM1, ENABLE );
	TIM_Cmd(TIM1,ENABLE);
}

///**
//  * @brief  TIM1_CC_IRQHandler
//	* @param  None
//  * @retval None
//  */
//void TIM1_CC_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM1,TIM_IT_CC4) == SET)
//	{
//		TIM_ClearITPendingBit(TIM1,TIM_IT_CC4);
//		TIM_ClearFlag(TIM1,TIM_FLAG_CC4);
//		TIM1->SR = 0;	
//	}
//}
/*
****	Test PWM hardware.
*/
void testPWMTask( void *pvParameters )
{
	int i = 1000;
	int pwm = 500;
	//test pwm1
	vTaskDelay(1000);
	for(;;)
	{


//		PWMA_EN();
//		PWM1_SET(TIM1,pwm);
//		PWM2_SET(TIM1,0);
//		PWM3_SET(TIM1,0);
//		vTaskDelay(1000);
//		PWMA_DIS();
//		
//		PWMB_EN();
//		PWM1_SET(TIM1,0);
//		PWM2_SET(TIM1,pwm);
//		PWM3_SET(TIM1,0);
//		vTaskDelay(1000);
//		PWMB_DIS();
//		
//		PWMC_EN();
//		PWM1_SET(TIM1,0);
//		PWM2_SET(TIM1,0);
//		PWM3_SET(TIM1,pwm);
//		vTaskDelay(1000);
//		PWMC_DIS();
		//		PWMOFF();

		//		vTaskDelay(500);
		PWMAB(TIM1,pwm);
		vTaskDelay(i);
		PWMAC(TIM1,pwm);
		vTaskDelay(i);
		PWMBC(TIM1,pwm);
		vTaskDelay(i);
		PWMBA(TIM1,pwm);
		vTaskDelay(i);
		PWMCA(TIM1,pwm);
		vTaskDelay(i);
		PWMCB(TIM1,pwm);
		vTaskDelay(i);
		
	}

//	//test pwm2
//	for(i=0;i<10;i++)
//	{
//		PWMON();
//		PWM_SET(0,pwm,0);
//		vTaskDelay(1000);
//	}
//	PWMOFF();
//	vTaskDelay(5000);
//	//test pwm3
//	for(i=0;i<10;i++)
//	{
//		PWMON();
//		PWM_SET(0,0,pwm);
//		vTaskDelay(1000);
//	}
//	PWMOFF();
//	vTaskDelay(5000);
}

/*========================== END OF FILE =======================================*/
