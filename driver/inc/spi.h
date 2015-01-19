#ifndef _SPI_H_
#define _SPI_H_

#define SPI_CS_LOW() 	GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_RESET)
#define SPI_CS_HIGH()	GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_SET)

void SPI1_Init( void );
int SPI_SEND( int data );
#endif
