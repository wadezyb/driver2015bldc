#include "as5047d.h"
#include "includes.h"
#include "spi.h"

int MagnetEncValue;
int MagnetEncOffset = 2126;

/* Even Parity */
/*
*** @param: data, 16bit data
*** @retval: data with even parity
*/
int evenParity( int data )
{
	int i;
	int counter = 0;
	// if data is not 16 bit
	if(data&0xffff0000)
	{
		return 0;
	}
	else // if data is a 16 bit data
	{
		for( i=0;i<16;i++ )
		{
			if( data&(0x1<<i) )
			{
				counter++;
			}
		}
		if( (counter%2) )
		{
			return (data|0x8000);
		}
		else
			return data;	
	}
}

/* Send data to AS5047D */
int asSend( int data )
{
	return SPI_SEND(evenParity(data));
}

/* Write data to addr of  AS5047D */
int asWrite( int addr, int data )
{
	// Write Command Frame bit14 should be 0
	asSend(addr&0x3fff);
	// Send the data to the addr and return the value of addr last time.
	return asSend(data&0x3fff);
}

/* Read data from addr of AS5047D */
int asRead( int addr )
{
	// Read Command Frame bit14 should be 1
	asSend(addr|0x4000);
	// Send a NOP command to get the value of addr.
	return asSend(0x0000|0x4000);
}

/* Get the encoder value, resolution is 12-bit */
int getEncoderValue( void )
{
    return (asRead(0X3ffe)&0x3fff)>>2 + MagnetEncOffset;
}


/* as5047d operating task */
void as5047dTask( void *pvParameters )
{
	SPI1_Init();
	vTaskDelay(1000);
	for(;;)
	{
		vTaskDelay(100);
    MagnetEncValue = (asRead(0X3ffe)&0x3fff)>>2;
		//value = evenParity(i);
	}
}

//void SPI2_IRQHandler(void)
//{
//	if( SPI_I2S_GetFlagStatus( SPI2,SPI_I2S_FLAG_TXE ) == SET )
//	{
//		GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);
//	}
//	else if( SPI_I2S_GetFlagStatus( SPI2,SPI_I2S_FLAG_RXNE ) == SET )
//	{
//		value = SPI_I2S_ReceiveData(SPI2);
//	}
//	
//}
