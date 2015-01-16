#ifndef _DS18B20_H_
#define _DS18B20_H_



#ifndef _DS18B20_LOCAL
#define DS18B20_EXTERN    extern
#else
#define DS18B20_EXTERN
#endif

#define DS18B20_BIT        GPIO_Pin_11
#define DS18B20_PORT        GPIOA

#define READ_DS18B20()        GPIO_ReadInputDataBit(DS18B20_PORT, DS18B20_BIT)
//需要修改相关值
#define DIR_1WIRE_IN()        {DS18B20_PORT->CRH &= 0xFFFF0FFF;DS18B20_PORT->CRH |= 0x00004000;}
#define DIR_1WIRE_OUT()       {DS18B20_PORT->CRH &= 0xFFFF0FFF;DS18B20_PORT->CRH |= 0x00003000;}
#define CLR_OP_1WIRE()        GPIO_ResetBits(DS18B20_PORT, DS18B20_BIT)
#define SET_OP_1WIRE()        GPIO_SetBits(DS18B20_PORT, DS18B20_BIT)
#define CHECK_IP_1WIRE()    GPIO_ReadInputDataBit(DS18B20_PORT, DS18B20_BIT)
extern  char Temperature[5];
DS18B20_EXTERN void DS18B20_init(void);
DS18B20_EXTERN void DS18B20_Convert(void);
DS18B20_EXTERN unsigned int  DS18B20_Read(void);
DS18B20_EXTERN void delay_nus(unsigned int nCount);



#endif


