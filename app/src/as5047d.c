#include "as5047d.h"
#include "includes.h"
#include "spi.h"

int value;
/* Note */
/*  
*** 1. Max clock should be less than 10kHz, since minimun clock is 100ns in the datasheet.
*** 2. CHn should be longer than 350ns.
*** 3. SPI Command Frame:
				bit		name	detail
				15		PARC	Parity bit(even) calculated on the commmand frame
				14		R/W		0:write	1:read
				13:0 ADDR	address to read or write
*** 4. SPI Read Data Frame:
				bit		name	detail
				15		PARD	Parity bit(even) for the data frame
				14 		EF		0:No command frame error occurred 	1:Error occurred
				13:0 	DATA	Data
*** 5. SPI Write Data Frame:
				bit		name	detail
				15		PARD	Parity bit(even)
				14 		0			Always low
				13:0 	DATA	Data				
*/

/* Volatile Register Table */
// Each register has a 14-bit address.
#define ADDR_NOP 				0X0000  	// Reading the NOP register is equivalent to a nop instruction for the AS5047D
#define ADDR_ERRFL 			0X0001
#define ADDR_PROG				0X0003
#define ADDR_DIAAGC			0x3FFC		// 
#define ADDR_MAG				0X3FFD		// bit 13:0 CORDIC magnitude information
#define ADDR_ANGLEUNC		0X3FFE		// bit 13:0 Angle information without dynamic angle error compensation
#define ADDR_ANGLECOM		0X3FFF		// bit 13:0 Angle information with dynamic angle error compensation

/* Non-Volatile Register Table */
// A nonvolatile memory( One-Time Programmable ) is used to store the zero position of the magnet and custom settings.
#define ADDR_ZPOSM			0X0016		// bit 7:0 Zero position MSB
#define ADDR_ZPOSL			0X0017		// bit 5:0 Zero position LSB; bit 6 this bit enables the contribution of MAGH; bit 7 this bit enables the contribution of MAGL
#define ADDR_SETTINGS1	0X0018		// Custom setting register 1
#define ADDR_SETTINGS2 	0X0019		// Custom setting register 2
#define ADDR_RED				0X001A		// Redundancy register

/* ERRFL 0X0001 */
// Reading the ERRFL register automatically clears its contents(ERRFL=0X0000)
/*
***	Name		R/W	Bit_Position	Detail
***	PARERR	R		2							Parity error
*** INVCOMM	R		1							Incalid command error: set to 1 by reading ro writing an incalid register address.
*** FRERR		R		0							Framing error: is set to 1 when a non-compliant SPI frame is detected
*/
#define ERRFL_BIT_PARERR 		0X0004
#define ERRFL_BIT_INVCOMM 	0X0002
#define ERRFL_BIT_FRERR 		0X0001

/* PROG 0X0003 */
// The PROG register is used for programming the OTP memory.( See programming the zero position. )
/*
***	Name		R/W	Bit_Position	Detail
***	PROGVER	R		6							Program verify: must be to 1 for verifying the correctness of the OTP programming.
*** PROGOTP	R		3							Start OTP programming cycle.
*** OTPREF	R		2							Refreshed the non-volatile memory content with the OTP programmed content.
*** PROGEN	R		0							Program OTP enable: enables programming the entire OTP memory.
*/



/* DIAAGC 0X3FFC */
// 
/*
***	Name		R/W	Bit_Position	Detail
***	MAGH		R		11						Diagnostics: Magnetic field strength too low; AGC=0XFF
*** MAGL		R		10						Diagnostics: Magnetic field strength too high; AGC=0X00
*** COF			R		9							Diagnostics: CORDIC overflow
*** LF			R		8							Diagnostics: Offset compensation: LF=0->internal offset loops not ready regulated; LF=1->internal offset loop finished
*** AGC			R		7:0						Automatic gain control value.
*/

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


/* as5047d operating task */
void as5047dTask( void *pvParameters )
{
	SPI1_Init();
	vTaskDelay(1000);
	for(;;)
	{
		vTaskDelay(100);
		value = asRead(0X3ffe)&0x3fff;
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
