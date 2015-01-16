/**
  ******************************************************************************
  * @file    ad.c
  * @author  ZHAN Yubo
  * @version V9.0.0
  * @date    03-May-2014
  * @brief   This file is adc driver for STM32F405RGT6
  *
*/

#include "includes.h"
#include "foc.h"
#include "ad.h"
#include "pwm.h"
#include "canopen.h"
#include "cloop.h"
/**
  * @brief  Initializes the ADC
  * @param  None
  * @retval None
	* @Note
			PIN DEFINIITON
			pa3 - adc2 - ADC1->JDR1
			pa4 - adc1 - ADC1->JDR2
			pa5 - adc3 - ADC1->JDR3
			pc4 - adc4 - ADC1->JDR4
*/
void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/*RCC*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA+RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	/*NVIC*/
	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_SetPriority(ADC_IRQn,15);

	/*GPIO-PA3-PA4-PA5*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3+GPIO_Pin_4+GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	/*GPIO-PC4*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	/*Perigh*/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 4;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_ExternalTrigInjectedConvConfig(ADC1,ADC_ExternalTrigInjecConv_T1_CC4);
	
	ADC_ExternalTrigInjectedConvEdgeConfig(ADC1,ADC_ExternalTrigInjecConvEdge_Rising);
	
	ADC_InjectedSequencerLengthConfig(ADC1,4);
	
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_3,1,ADC_SampleTime_3Cycles);
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_4,2,ADC_SampleTime_3Cycles);
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_5,3,ADC_SampleTime_3Cycles);
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_14,4,ADC_SampleTime_3Cycles);
	
	ADC_InjectedDiscModeCmd(ADC1,DISABLE);
	
	ADC_SetInjectedOffset(ADC1,ADC_InjectedChannel_1, 0x000);
	
	ADC_Cmd(ADC1,ENABLE);
	ADC_ITConfig(ADC1,ADC_IT_JEOC,ENABLE);
}

/**
  * @brief  ADC_IRQHandler
  * @param  None
  * @retval None
  */ 
void ADC_IRQHandler(void)
{
	if((ADC1->SR&0X04)==0X04)//IF JEOC
	{
		ADC1->SR &= ~0x04;//ADC_Clear JEOC
		//currentLoop();
		if((ControlWord.Param&CONTROLWORD_SWITCH_ON) == CONTROLWORD_SWITCH_ON)
		{
			if( ModesofOperation.Param != NO_MODE )
				currentLoop();
		}
	}
}
/*========================== END OF FILE =======================================*/
