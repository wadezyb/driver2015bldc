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

