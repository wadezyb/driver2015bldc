/**
  ******************************************************************************
  * @file    voltage.c
  * @author  ZHAN Yubo
  * @version V1.0.0
  * @CreateedDate    21-January-2015
  * @brief   This file is for the Bus Voltage Measurement. To convert the ADC value
	to a value with a unit of Volt.
	
	* @LastDate
	* @LastComment
  *
*/

#include "voltage.h"
#include "includes.h"

#define K_VOLTAGE (0.0169189453125)
float voltage = 0;

float getVoltage( int adc )
{
	return (adc*K_VOLTAGE);
}

void voltageTestTask( void *pvParameters )
{
	for(;;)
	{
		vTaskDelay(100);
		voltage = getVoltage(ADC1->JDR4);
	}
}

