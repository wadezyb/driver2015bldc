/**
  ******************************************************************************
  * @file    calibration.c
  * @author  ZHAN Yubo
  * @version V9.0.0
  * @date    03-May-2014
  * @brief   This file is calibration for STM32F405RGT6
  *
*/

#include "includes.h"
#include "calibration.h"
#include "encoder.h"
#include "foc.h"
#include "pwm.h"
#include "ad.h"
#include "canopen.h"
#include "ploop.h"
#include "cloop.h"

/**
  * @brief  Calibrate the encoder
  * @param  None
  * @retval None
  */
void EncCalibrate( void )
{
	TIM3->CNT = ((4096 - absEnc.single))*INC_ENC_REVOLUTION/4096;
	vTaskDelay(100);
	if( absEnc.value > 0 )
		Encoder.Multi = absEnc.value/INC_ENC_REVOLUTION;
	else
		Encoder.Multi = absEnc.value/INC_ENC_REVOLUTION - 1;
	Position.targetPosition = Encoder.Multi*INC_ENC_REVOLUTION + Encoder.Single;
}
/**
  * @brief  Calibrate the FOC
  * @param  None
  * @retval None
  */
void FOC_Calibrate( void )
{
	EncCalibrate();
	PWMOFF();
}
/**
  * @brief  Calibration Task
  * @param  None
  * @retval None
  */
void calibrationTask( void *pvParameters )
{
	vTaskDelay(3000);
	FOC_Calibrate();
	CurrentOffsetCalibration();
	StatusWord.Param |= STATUSWORD_BIT0;
	for(;;)
	{
		vTaskDelete(NULL);
	}
}

/**
  * @brief  Current Sampling Offset Calibration 
  * @param  None
  * @retval None
  */
void CurrentOffsetCalibration( void )
{
	int i=0;
	int sum1 = 0;
	int sum2 = 0;
	int n = 20;
	for(i=0;i<n;i++)
	{
		sum1 += ADC1->JDR2;
		sum2 += ADC1->JDR1;
		vTaskDelay(1);
	}
	Current.IA_Offset = sum1/n;
	Current.IB_Offset = sum2/n;
	vTaskDelete(NULL);
}


/*========================== END OF FILE =======================================*/

