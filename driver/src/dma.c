/*
================================================================================
* File Name                 :   dma.c
* Copyright                 :   2011-2013
* 
* CPU                       :   STM32F103
* RTOS                      :   FreeRTOS
*
* Create Date               :   2012/12/1
* Author/Company            :   Zhan Yubo/UESTC
* Abstract Description      :   Configuration and Applications of DMA.
*...............................................................................
* Revision History
* NO.     Date      Revised by      Item      Description
*	NO.1		2013/7/16	Zhan Yubo				Note			Added some necessary notes.
*
================================================================================
*/

#include "dma.h"
#include "includes.h"
//#include "hall.h"
//#include "current.h"
//#include "ad.h"
//#include "pwm.h"
//#include "memory.h"
//#include "canopen.h"
//#include "encoder.h"
//#include "foc.h"
//#include "led.h"
#include <math.h>
short ADC1_Value[5];
int adc1,adc2;
int adc;

//long Position[7];
//	
//long HallPos[7];
#define ADC1_DR_Address    ((uint32_t)(&(ADC1->JDR1)))

/*
DMA2 CHANNEL STREAM 0
*/
void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/*RCC*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE)	;
		
	/*NVIC*/
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_SetPriority(DMA1_Stream2_IRQn,15);
	
	DMA_DeInit(DMA2_Stream0);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;//外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC1_Value;//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//dma传输方向单向
	DMA_InitStructure.DMA_BufferSize = 4;//设置DMA在传输时缓冲区的长度 word
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;//设置DMA的外设递增模式，一个外设
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//设置DMA的内存递增模式，
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据字长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//内存数据字长
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//设置DMA的传输模式：连续不断的循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//设置DMA的优先级别
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;//
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA2_Stream0,ENABLE);
}
void DMA2_Stream0_IRQHandler(void)
{
	
	if(DMA_GetITStatus(DMA2_Stream0,DMA_IT_TCIF0) == SET)
	{
		DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
		adc1 = (int)(2048-ADC1_Value[0]);
		adc2 = (int)(2048-ADC1_Value[1]);
		#ifdef USE_FOC

		#endif
		
//	if(MotorType.Param==MOTORTYPE_BDCMOTOR)
//	{
//		adc=-adc2;
//	}
//	else if(MotorType.Param==MOTORTYPE_TPMBLMOTOR)
//	{
//		switch(hallValue)
//		{
//			case 5:
//					adc=-adc1;
//				break;
//			case 1:
//					adc=-(adc1-adc2)/2;
//				break;
//			case 3:
//					adc=adc2;
//				break;
//			case 2:
//					adc=adc1;
//				break;
//			case 6:
//					adc=(adc1-adc2)/2;
//				break;
//			case 4:
//					adc=-adc2;
//				break;
//			default:break;
//		}

//	}
//	else if(MotorType.Param==MOTORTYPE_PMSMOTOR)
//	{
//#ifdef USE_FOC		
//		FOC.Ia=Iu;
//		FOC.Ib=0.57735*Iu+1.1547*Iv;
//		FOC.Id=FOC.Ia*Cos(FOC.Alpha)+FOC.Ib*Sin(FOC.Alpha);
//		FOC.Iq=-FOC.Ia*Sin(FOC.Alpha)+FOC.Ib*Cos(FOC.Alpha);
//#endif
//	}
	

//	operation_mode=ModesofOperation.Param;
//	if(((ControlWord.Param)&CONTROLWORD_SWITCH_ON) == CONTROLWORD_SWITCH_ON)
//	{
//		ledState = ON;
//		switch(ModesofOperation.Param)
//		{
//			case -1://PWM mode
//				pwm=pwm;
//				//pwm=ADC1_Value[2]/7;
//				break;
//			case PROFILE_POSITION_MODE://Profile Position Mode
//				break;
//			case VELOCITY_MODE://current loop for Velocity Mode
//				memory.PIFF.In = adc;
//				memory.PIFF.Ref = (TargetTorque.Param);
//				DoPIFF(&(memory.PIFF));
//				pwm=memory.PIFF.Output;
//				break;
//			case PROFILE_VELOCITY_MODE:
//				memory.PIFF.In = adc;
//				memory.PIFF.Ref = (TargetTorque.Param);
//				DoPIFF(&(memory.PIFF));
//				pwm=memory.PIFF.Output;
//				break;				
//			case TORQUE_PROFILE_MODE://Torque Profile Mode
//				#ifndef USE_FOC
//				memory.PIFF.In = adc;
//				memory.PIFF.Ref = (TargetTorque.Param);
//				DoPIFF(&(memory.PIFF));
//				pwm=memory.PIFF.Output;
//				#else
//				//FOC.Vd=DoFOCPIFF(FOC.Id,0,&(memory.FOCDPIFF));
//				//FOC.Vq=DoFOCPIFF(FOC.Iq,(TargetTorque.Param),&(memory.FOCQPIFF));
//				#endif
//				break;
//			case HOMING_MODE:
//				break;
//			case INTERPOLATED_POSITION_MODE:
//				break;
//			default:
//				break;
//		}
//		
//	
//		#ifndef USE_FOC
//		if(pwm>memory.maxpwm)
//			pwm=memory.maxpwm;
//		if(pwm<-memory.maxpwm)
//			pwm = -memory.maxpwm;			
//		PWM(hallValue,pwm);
//		#else
//		FOCPWM(&FOC);
//		#endif
//	}
//		else if(((ControlWord.Param)&0x02) ==0x02)//for testing
//		{

//		}
//		else
//		{
//			TIM_SetCompare1(TIM1,0);//
//			TIM_SetCompare2(TIM1,0);//
//			TIM_SetCompare3(TIM1,0);//
//			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
//			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
//			GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);		
//		}	
		



				
	}
}
/*========================== END OF FILE =======================================*/
