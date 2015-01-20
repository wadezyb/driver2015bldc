#ifndef _AS5047D_H_
#define _AS5047D_H_

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


void spi_init( void );
void as5047dTask( void *pvParameters );
int getEncoderValue( void );
#endif


