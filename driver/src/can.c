/**
  ******************************************************************************
  * @file    can.c
  * @author  ZHAN Yubo
  * @version V9.0.0
  * @date    03-May-2014
  * @brief   This file is can driver for STM32F405RGT6
  *
*/

/* INCLUDES */
#include "can.h"
#include "includes.h"
#include "led.h"
#include "canopen.h"
#include "encoder.h"
#include "cloop.h"
#include "vloop.h"
#include "ploop.h"
#include "interpolation.h"

/* Private Define */
#define CAN_MESSAGE_MAXBUF (20)
int device_id = CAN_NODE;

/* Public Variables */
CanTxMsg TxMessage;
CanRxMsg RxMessage;


/* Private Variables */
typedef struct
{
	int head;
	int tail;
	CanTxMsg TxMessage[CAN_MESSAGE_MAXBUF];
}CANMessageQueueOBJ;

CANMessageQueueOBJ CANMessageQueue;

/**
  * @brief  Initializes the CANMessage Queue.
  * @param  CANMessageQueue
  * @retval None
  */
void CANMessageQueueInit(CANMessageQueueOBJ* CANMessageQueue)
{
	CANMessageQueue->head = 0;
	CANMessageQueue->tail = 0;
}

/**
  * @brief  Configure the TIM4 as CAN message timer
  * @param  None
  * @retval None
  */
void TIM4_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);	        //使能TIM4的时钟

	TIM_TimeBaseStructure.TIM_Period = 1680; 						//500us
	TIM_TimeBaseStructure.TIM_Prescaler =15-1; 	                    //分频系数    计数频率=CLK/(Prescaler+1)  即1秒能计数CLK/(Prescaler+1)
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	                //采样分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; 		
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 

	TIM_ClearFlag(TIM4, TIM_FLAG_Update); 	                        //清中断，以免一启用中断后立即产生中断 
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);                      //使能TIM4中断源 
	TIM_Cmd(TIM4, ENABLE); 		                                    //TIM4总开关：开启 
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_SetPriority(TIM4_IRQn,15);
}
/**
  * @brief  Initializes the CAN.
  * @param  None
  * @retval None
  */
void CAN_Configuration(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;		
	GPIO_InitTypeDef 	   GPIO_InitStructure;
	NVIC_InitTypeDef   		NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	/*时钟使能*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	 //  使能CAN控制器系统外设	
	/*配置端口PB8,PB9*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_CAN1);

	/* Configurate CAN */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	
	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=ENABLE;
	CAN_InitStructure.CAN_AWUM=ENABLE;
	CAN_InitStructure.CAN_NART=ENABLE;	   	
	CAN_InitStructure.CAN_RFLM=DISABLE;		
	CAN_InitStructure.CAN_TXFP=ENABLE;		
//	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
 	CAN_InitStructure.CAN_Mode=	CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler=4;
	CAN_Init(CAN1, &CAN_InitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber=1;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=CAN_NODE<<5;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=CAN_NODE<<5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_SetPriority(CAN1_RX0_IRQn,15);
	
	CANMessageQueueInit(&CANMessageQueue);

	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);	
	TIM4_Configuration();
}


/**
  * @brief  Check the message queue and send CAN messages.
  * @param  None
  * @retval None
  */
void CANSendCmd(CANMessageQueueOBJ* CANMessageQueue)
{
	if(CANMessageQueue->head != CANMessageQueue->tail)//如果消息队列里有数据
	{
		CANMessageQueue->head++;
		if(CANMessageQueue->head >= CAN_MESSAGE_MAXBUF)//如果溢出了 则归零
			CANMessageQueue->head = 0;
		CAN_Transmit(CAN1, &CANMessageQueue->TxMessage[CANMessageQueue->head]);//将消息放入CAN FIFO
	}
}

/**
  * @brief  Send CAN message to the queue
  * @param  None
  * @retval None
  */
void CAN_SendToQueue(CanTxMsg TxMessage, CANMessageQueueOBJ* CANMessageQueue)
{
	CANMessageQueue->tail++;
	if(CANMessageQueue->tail >= CAN_MESSAGE_MAXBUF)
		CANMessageQueue->tail = 0;
	if(CANMessageQueue->tail == CANMessageQueue->head)//如果队列满了
	{
		
	}
	else//若队列未满
	{
		CANMessageQueue->TxMessage[CANMessageQueue->tail] = TxMessage;
	}
	
}

/**
  * @brief  Send CAN 
  * @param  None
  * @retval None
  */
void CAN_send(int id,int index,int subindex, int param)		
{
  CanTxMsg TxMessage;
 
  TxMessage.StdId=id;
  TxMessage.RTR=CAN_RTR_DATA;
  TxMessage.IDE=CAN_ID_STD;
  TxMessage.DLC=8;
  TxMessage.Data[0]=device_id;
  TxMessage.Data[1]=index;
  TxMessage.Data[2]=index>>8;
  TxMessage.Data[3]=subindex;
  TxMessage.Data[4]=param;
  TxMessage.Data[5]=param>>8;
  TxMessage.Data[6]=param>>16;
  TxMessage.Data[7]=param>>24;
	CAN_SendToQueue(TxMessage,&CANMessageQueue);
	//CAN_Transmit(CAN1, &TxMessage);
}
/**
  * @brief  Receive the CAN message
  * @param  None
  * @retval None
  */
void ReceiveData(void)
{
		COBJ RX;
		int Index;
//		RxMessage.StdId=0x01;
//    RxMessage.IDE=CAN_ID_STD;
//    RxMessage.DLC=0;
//    RxMessage.Data[0]=0x00;
//    RxMessage.Data[1]=0x00;
    CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
		RX.ID =RxMessage.StdId;         			
	
//		RX.Specifier=RxMessage.Data[0];     

		if( RX.ID == device_id )
		{
			RX.Index = RxMessage.Data[1] + (RxMessage.Data[2]<<8);
			RX.SubIndex= RxMessage.Data[3];       
			RX.Param=RxMessage.Data[4]+(RxMessage.Data[5]<<8)+(RxMessage.Data[6]<<16)+(RxMessage.Data[7]<<24);
		
			if(RxMessage.Data[0] == CAN_NODE_MASTER)
			{
				Index=(RxMessage.Data[1] + (RxMessage.Data[2]<<8));
				switch( Index )
				{
					case CONTROLWORD_INDEX:
						ControlWord.Param = RX.Param;
						break;
					case MODES_OF_OPERATION_INDEX:
						ModesofOperation.Param=RX.Param;
						break;
					case MOTORTYPE_INDEX:
						MotorType.Param = RX.Param;
						break;
					case TARGETTORQUE_INDEX:
						TargetTorque.Param=RX.Param;
						Current.targetCurrent=TargetTorque.Param;
						break;
					case TARGET_VELOCITY_INDEX:
						TargetVelocity.Param=RX.Param;
						Velocity.targetVelocity = TargetVelocity.Param;
						break;
					case TARGET_POSITION_INDEX:
						TargetPosition.Param=RX.Param;
						Tra.Position = TargetPosition.Param;
						break;
					case POSITION_ACTUAL_VALUE_INDEX:
						
						break;
					default: 
						break;
				}
			}
		}
		
}


void TIM4_IRQHandler(void)
{
	if(TIM4->SR & TIM_SR_UIF)
	{
		TIM4->SR &= ~TIM_SR_UIF;
		CANSendCmd(&CANMessageQueue);
	}
}

/**
  * @brief  CAN1_RX0_IRQHandler
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{
	//CAN_ClearITPendingBit();
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0) == SET)
	{
		CAN_ClearFlag(CAN1,CAN_FLAG_RQCP0);
		ReceiveData();
	}
}
/*========================== END OF FILE =======================================*/

