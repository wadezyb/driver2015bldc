/*
================================================================================
* File Name                 :    led.h
* Copyright                 :    2011-2013
* Module Name               :
* 
* CPU                       :    STM32F103VCT6(Cotex M3)
* RTOS                      :    None
*
* Create Date               :    2011/9/30
* Author/Company            :    战玉博/UESTC
* Abstract Description      :
*...............................................................................
* Revision History
* NO.     Date      Revised by      Item      Description
*
================================================================================
*/

/*******************************************************************************
*			Multi-Include-Prevent Section
*******************************************************************************/
#ifndef  _LED_H_
#define  _LED_H_


/*******************************************************************************
* Include File Section
*******************************************************************************/
#include "includes.h"

/*******************************************************************************
* Macro Define Section
*******************************************************************************/

/* led port */
#define LED_PORT0 GPIOC
#define LED_PORT1 GPIOC
#define LED_PORT2 GPIOD

/* led pin */
#define LED_PIN0 GPIO_Pin_9
#define LED_PIN1 GPIO_Pin_12
#define LED_PIN2 GPIO_Pin_2

/* led action */
#define LedON() GPIO_WriteBit(LED_PORT0,LED_PIN0,Bit_SET)
#define LedOFF() GPIO_WriteBit(LED_PORT0,LED_PIN0,Bit_RESET)

/* led colour */
#define LED_COLOR_RED 	0
#define LED_COLOR_GREEN 1
#define LED_COLOR_TEST 	2

/* led state */
#define LED_STATE_FLASHING 	0
#define LED_STATE_ON 				1
#define LED_STATE_OFF 			2
#define LED_STATE_IDLE 			3

typedef struct
{
	int Color;
	int State;
	int Delay;
}ledObj;

/* 全局变量 */
extern ledObj LED;
/*******************************************************************************
*	Prototype Declare Section
*******************************************************************************/
void LED_Configuration(void);
void Flash_Once(void);
void delay_ms(unsigned int x);
void ledTask( void *pvParameters );
void LEDALLOFF(void);
#endif

/*==========================END OF FILE=======================================*/

