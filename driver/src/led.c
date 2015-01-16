/**
  ******************************************************************************
  * @file    led.c
  * @author  ZHAN Yubo
  * @version V9.0.0
  * @date    03-May-2014
  * @brief   This file is led driver for STM32F405RGT6
  *
*/

/* INCLUDES */
#include "led.h"
#include "includes.h"

ledObj LED;

void ledObjInit( void )
{
	LED.Color = LED_COLOR_RED;
	LED.Delay = 100;//ms
	LED.State = LED_STATE_IDLE;
}

/**
  * @brief  A simple delay for testing.
  * @param  x		: x ms to delay
  * @retval None
  */
void delay_ms(unsigned int x)   //READ
{
  int i,j;
  for(i=0;i<x;i++)
  for(j=0;j<13150;j++);
}

/**
  * @brief  A simple flash led for testing.
  * @param  None
  * @retval None
  */
void FLASHING_Once(void)
{
   LedON();
   delay_ms(100);
   LedOFF();
   delay_ms(100);
}

/**
  * @brief  Initializes the Led IO port.
  * @param  None
  * @retval None
  */                                                                                   
void LED_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED_PIN0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(LED_PORT0,&GPIO_InitStructure);

	/*PC-12*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED_PIN1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(LED_PORT1,&GPIO_InitStructure);
	/*PD-2*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED_PIN2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(LED_PORT2,&GPIO_InitStructure);
}


/**
  * @brief  Turn on the Led
	* @param  ledColour	:can be RED GREEN TEST
  * @retval None
  */
void LedOn( int ledColour )
{
	if(ledColour == LED_COLOR_RED)
	{
		GPIO_WriteBit(LED_PORT0,LED_PIN0,Bit_RESET);
		GPIO_WriteBit(LED_PORT1,LED_PIN1,Bit_SET);
		GPIO_WriteBit(LED_PORT2,LED_PIN2,Bit_RESET);	
	}
	else if(ledColour == LED_COLOR_GREEN)
	{
		GPIO_WriteBit(LED_PORT0,LED_PIN0,Bit_RESET);
		GPIO_WriteBit(LED_PORT1,LED_PIN1,Bit_RESET);
		GPIO_WriteBit(LED_PORT2,LED_PIN2,Bit_SET);		
	}
	else if(ledColour == LED_COLOR_TEST)
	{
		GPIO_WriteBit(LED_PORT0,LED_PIN0,Bit_SET);	
		GPIO_WriteBit(LED_PORT1,LED_PIN1,Bit_RESET);
		GPIO_WriteBit(LED_PORT2,LED_PIN2,Bit_RESET);
	}
}
/**
  * @brief  Turn off the Led
	* @param  None
  * @retval None
  */
void LedOff(void)
{
	if((LED.Color == LED_COLOR_RED)||(LED.Color == LED_COLOR_GREEN))
	{
		GPIO_WriteBit(LED_PORT1,LED_PIN1,Bit_RESET);
		GPIO_WriteBit(LED_PORT2,LED_PIN2,Bit_RESET);	
	}
	else if(LED.Color == LED_COLOR_TEST)
	{
		GPIO_WriteBit(LED_PORT0,LED_PIN0,Bit_RESET);		
	}
}

/**
  * @brief  Led Flashing Task
  * @param  None
  * @retval None
  */
void ledTask( void *pvParameters )
{
	/* Configure the IO of LED */
	LED_Configuration();
	ledObjInit();
	for( ;; )
	{
//		LED.Color++;
//		if(LED.Color == 3)
//		{
//			LED.Color = 0;
//		}
		switch( LED.State )
		{
			case LED_STATE_IDLE:
				LedOn(LED.Color);
				vTaskDelay(LED.Delay);
				LedOff();
				vTaskDelay(LED.Delay);	
				break;
			case LED_STATE_FLASHING: 
				LedOn(LED.Color);
				vTaskDelay(LED.Delay);
				LedOff();
				vTaskDelay(LED.Delay);				
				break;
			case LED_STATE_ON: 
				LedOn(LED.Color);
				vTaskDelay(LED.Delay);
				break;
			case LED_STATE_OFF:
				LedOff();
				vTaskDelay(LED.Delay);
				break;
			default :break;
		}				
	}
}

/**
  * @brief  Turn off all Leds
  * @param  None
  * @retval None
  */
void LEDALLOFF(void)
{
		GPIO_WriteBit(LED_PORT1,LED_PIN1,Bit_RESET);
		GPIO_WriteBit(LED_PORT2,LED_PIN2,Bit_RESET);
		GPIO_WriteBit(LED_PORT0,LED_PIN0,Bit_RESET);		
}

/*========================== END OF FILE =======================================*/

