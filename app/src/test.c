/**
  ******************************************************************************
  * @file    test.c
  * @author  ZHAN Yubo
  * @version V9.0.0
  * @date    03-May-2014
  * @brief   This file is for Testing
  *
*/

#include "includes.h"
#include "test.h"
#include "main.h"
//#include "hall.h"
#include "pwm.h"
#include "sci.h"
#include "LED.h"
#include "cli.h"
#include "can.h"
//#include "ds18b20.h"
#include "encoder.h"
#include "canopen.h"
#include "foc.h"

float w=0.001;

/*
* Task of LED Testing
*/
//void testLEDTask( void *pvParameters )
//{	
//	int last_led_delay=ledDelay;
//	int last_led_colour=ledColour;
//	int last_led_state=ledState;
//	sciSendString("\r\nStarting LED testing!",bMessage);
//	sciSendString("\r\n-> LED testing!",bMessage);

////	sciSendString(pcTaskGetTaskName(ledTaskHandle),bMessage);
////	if(strcmp(pcTaskGetTaskName(ledTaskHandle),"test")==0)
////	{
////		xTaskCreate( ledTask, "LED", configMINIMAL_STACK_SIZE, NULL, 1, &ledTaskHandle );
////	}
//	ledState=IDLE;
//	ledDelay=50;
//	sciSendString("\r\n--> Flashing Frequency is 10Hz",bMessage);
//	ledColour=TEST;
//	sciSendString("\r\n--> Is TEST LED Flashing?(yes or no)",bMessage);
//	while(feedback==FEEDBACK_NONE)//wait for feedback
//	{
//		vTaskDelay(50);
//	}
//	if(feedback==FEEDBACK_YES)
//	{
//		sciSendString("\r\n--> TEST LED is OK",bMessage);
//		feedback=FEEDBACK_NONE;
//	}
//	else
//	{
//		sciSendString("\r\n--> TEST LED is NOT WORKING",bMessage);
//		feedback=FEEDBACK_NONE;
//	}
//	
//	ledColour=RED;
//	sciSendString("\r\n--> Is RED LED Flashing?(yes or no)",bMessage);
//	while(feedback==FEEDBACK_NONE)//wait for feedback
//	{
//		vTaskDelay(50);
//	}
//	if(feedback==FEEDBACK_YES)
//	{
//		sciSendString("\r\n--> RED LED is OK",bMessage);
//		feedback=FEEDBACK_NONE;
//	}
//	else
//	{
//		sciSendString("\r\n--> RED LED is NOT WORKING",bMessage);
//		feedback=FEEDBACK_NONE;
//	}
//	
//	ledColour=GREEN;
//	sciSendString("\r\n--> Is green LED Flashing?(yes or no)",bMessage);
//	while(feedback==FEEDBACK_NONE)//wait for feedback
//	{
//		vTaskDelay(50);
//	}
//	if(feedback==FEEDBACK_YES)
//	{
//		sciSendString("\r\n--> GREEN LED is OK",bMessage);
//		feedback=FEEDBACK_NONE;
//	}
//	else
//	{
//		sciSendString("\r\n--> GREEN LED is NOT WORKING",bMessage);
//		feedback=FEEDBACK_NONE;
//	}
//	//vTaskDelete(ledTaskHandle);
//	LEDALLOFF();
//	sciSendString("\r\nEnd of LED testing!",bMessage);
//	ledDelay=last_led_delay;//恢复之前的LED状态
//	ledState=last_led_state;
//	ledColour=last_led_colour;
//	for(;;)
//	{
//		vTaskDelay(500);
//		vTaskDelete(NULL);
//	}
//}
///* Hall Sensor Test */
//void testHALLTask( void *pvParameters )
//{
//	char stringBuf[20];
//	char hall_temp[17];
//	int i=0;
//	sciSendString("\r\nStarting HALL testing!",bMessage);
//	sciSendString("\r\n-> HALL testing!",bMessage);
//	sciSendString("\r\n-> Please run Clock Wise!",bMessage);
//	for(i=0;i<16;i++)
//	{
//			hall_temp[i] = hallValue;
//			while(hall_temp[i] == hallValue)
//			{
//				vTaskDelay(1);
//			}
//			hall_temp[i]=*int2String(hall_temp[i],stringBuf);
//			if(i!=15)
//			{
//				hall_temp[i+1]='-';
//				hall_temp[i+2]='>';
//				i+=2;
//			}
//			else
//				hall_temp[16]='\0';
//	}
//	sciSendString("\r\n-> HallValue is:",bMessage);
//	sciSendString(hall_temp,bMessage);
//	vTaskDelete(NULL);
//}
//void testPWM1Task( void *pvParameters )
//{
//	sciSendString("\r\nStarting PWM testing!",bMessage);
//	ControlWord.Param= 0x02;//进入测试模式
//	TIM_SetCompare1(TIM1,T_PWM/5);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
//	TIM_SetCompare2(TIM1,T_PWM/5);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
//	TIM_SetCompare3(TIM1,T_PWM/5);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);	
//	sciSendString("\r\nPWM1 on delay 10s ",bMessage);
//	vTaskDelay(100000);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
//	TIM_SetCompare1(TIM1,0);
//	sciSendString("\r\nEnd of pwm1 test",bMessage);
//	ControlWord.Param= 0x00;//进入测试模式
//	vTaskDelete(NULL);
//}
//void testPWM2Task( void *pvParameters )
//{
//	sciSendString("\r\nStarting PWM2 testing!",bMessage);
//	ControlWord.Param= 0x02;//进入测试模式
//	sciSendString("\r\nPWM2 on delay 10s ",bMessage);
//	vTaskDelay(100);
//	PWMAC(TIM1,T_PWM/2);
//	vTaskDelay(3000);
//	PWM1_SET(TIM1,0);
//	PWM3_SET(TIM1,0);
//	vTaskDelay(1000);
//	PWMCA(TIM1,T_PWM/2);
//	vTaskDelay(3000);	
//	sciSendString("\r\nEnd of pwm2 test",bMessage);
//	ControlWord.Param= 0x00;//进入测试模式
//	vTaskDelete(NULL);
//}
//void testPWM3Task( void *pvParameters )
//{
//	sciSendString("\r\nStarting PWM3 testing!",bMessage);
//	TIM_SetCompare3(TIM1,T_PWM/10);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);
//	sciSendString("\r\nPWM5 on delay 10s ",bMessage);
//	vTaskDelay(10000);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);
//	TIM_SetCompare3(TIM1,0);
//	sciSendString("\r\nEnd of pwm3 test",bMessage);
//	vTaskDelete(NULL);
//}
///* PWM Test */
//void testPWMTask( void *pvParameters )
//{
//	char stringBuf[20];
//	int counter=0;
//	
//	int timedelay=5;
//	int timeout=100/timedelay;
//	int hallBuf[20];
//	int hallAB[2];
////	int hallAC[2];
////	int hallBC[2];
////	int hallBA[2];
////	int hallCA[2];
////	int hallCB[2];
//	sciSendString("\r\nStarting PWM testing!",bMessage);
//	sciSendString("\r\nChannel1 pwm output(yes or no)",bMessage);
//	//TIM_SetAutoreload(TIM1,T_PWM*10-1);//f = 50k
////A->B
//	TIM_SetCompare1(TIM1,T_PWM/10);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
//	TIM_SetCompare2(TIM1,0);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
//	TIM_SetCompare3(TIM1,0);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);
//	//vTaskDelay(200);
//	while(feedback==FEEDBACK_NONE)//wait for feedback
//	{
//		vTaskDelay(timedelay);
//		
//		hallBuf[counter]=hallValue;
//		counter++;

////		sciSendString("\r\nHallValue=",bMessage);
////		sciSendString(int2String(hallValue,stringBuf),bMessage);
//		if(counter>timeout)
//		{
//			counter=0;
//			hallAB[0]=FindMost(hallBuf);
//			hallAB[1]=FindSecondMost(hallBuf);
//			sciSendString("\r\nHallValueAB=",bMessage);
//			sciSendString(int2String(hallAB[0],stringBuf),bMessage);			
//			sciSendString(int2String(hallAB[1],stringBuf),bMessage);
//			break;
//		}
//	}
//	if(feedback==FEEDBACK_YES)
//	{
//		sciSendString("\r\n--> PWMA->B is OK",bMessage);
//		feedback=FEEDBACK_NONE;
//	}
//	else
//	{
//		sciSendString("\r\n--> PWMA->B is NOT WORKING",bMessage);
//		feedback=FEEDBACK_NONE;
//	}
////A->C
//	TIM_SetCompare1(TIM1,T_PWM/10);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
//	TIM_SetCompare2(TIM1,0);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
//	TIM_SetCompare3(TIM1,0);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);
//	while(feedback==FEEDBACK_NONE)//wait for feedback
//	{
//		vTaskDelay(timedelay);
//		hallBuf[counter]=hallValue;
//		counter++;		
//		
////		sciSendString("\r\nHallValue=",bMessage);
////		sciSendString(int2String(hallValue,stringBuf),bMessage);
//		if(counter>timeout)
//		{
//			counter=0;
//			hallAB[0]=FindMost(hallBuf);
//			hallAB[1]=FindSecondMost(hallBuf);
//			sciSendString("\r\nHallValueAB=",bMessage);
//			sciSendString(int2String(hallAB[0],stringBuf),bMessage);			
//			sciSendString(int2String(hallAB[1],stringBuf),bMessage);			
//			break;
//		}		
//	}
//	if(feedback==FEEDBACK_YES)
//	{
//		sciSendString("\r\n--> PWMA->C is OK",bMessage);
//		feedback=FEEDBACK_NONE;
//	}
//	else
//	{
//		sciSendString("\r\n--> PWMA->C is NOT WORKING",bMessage);
//		feedback=FEEDBACK_NONE;
//	}	
////B->C
//	TIM_SetCompare1(TIM1,0);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
//	TIM_SetCompare2(TIM1,T_PWM/10);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
//	TIM_SetCompare3(TIM1,0);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);
//	while(feedback==FEEDBACK_NONE)//wait for feedback
//	{
//		vTaskDelay(timedelay);
//		hallBuf[counter]=hallValue;
//		counter++;
////		sciSendString("\r\nHallValue=",bMessage);
////		sciSendString(int2String(hallValue,stringBuf),bMessage);
//		if(counter>timeout)
//		{
//			counter=0;
//			hallAB[0]=FindMost(hallBuf);
//			hallAB[1]=FindSecondMost(hallBuf);
//			sciSendString("\r\nHallValueAB=",bMessage);
//			sciSendString(int2String(hallAB[0],stringBuf),bMessage);			
//			sciSendString(int2String(hallAB[1],stringBuf),bMessage);			
//			break;
//		}		
//	}
//	if(feedback==FEEDBACK_YES)
//	{
//		sciSendString("\r\n--> PWMB->C is OK",bMessage);
//		feedback=FEEDBACK_NONE;
//	}
//	else
//	{
//		sciSendString("\r\n--> PWMB->C is NOT WORKING",bMessage);
//		feedback=FEEDBACK_NONE;
//	}
////B->A	
//	TIM_SetCompare1(TIM1,0);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
//	TIM_SetCompare2(TIM1,T_PWM/10);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
//	TIM_SetCompare3(TIM1,0);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);
//	while(feedback==FEEDBACK_NONE)//wait for feedback
//	{
//		vTaskDelay(timedelay);
//		hallBuf[counter]=hallValue;
//		counter++;
////		sciSendString("\r\nHallValue=",bMessage);
////		sciSendString(int2String(hallValue,stringBuf),bMessage);
//		if(counter>timeout)
//		{
//			counter=0;
//			hallAB[0]=FindMost(hallBuf);
//			hallAB[1]=FindSecondMost(hallBuf);
//			sciSendString("\r\nHallValueAB=",bMessage);
//			sciSendString(int2String(hallAB[0],stringBuf),bMessage);			
//			sciSendString(int2String(hallAB[1],stringBuf),bMessage);			
//			break;
//		}		
//	}
//	if(feedback==FEEDBACK_YES)
//	{
//		sciSendString("\r\n--> PWMB->A is OK",bMessage);
//		feedback=FEEDBACK_NONE;
//	}
//	else
//	{
//		sciSendString("\r\n--> PWMB->A is NOT WORKING",bMessage);
//		feedback=FEEDBACK_NONE;
//	}	
////C->A
//	TIM_SetCompare1(TIM1,0);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
//	TIM_SetCompare2(TIM1,0);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
//	TIM_SetCompare3(TIM1,T_PWM/10);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);
//	while(feedback==FEEDBACK_NONE)//wait for feedback
//	{
//		vTaskDelay(timedelay);
//		hallBuf[counter]=hallValue;
//		counter++;
////		sciSendString("\r\nHallValue=",bMessage);
////		sciSendString(int2String(hallValue,stringBuf),bMessage);
//		if(counter>timeout)
//		{
//			counter=0;
//			hallAB[0]=FindMost(hallBuf);
//			hallAB[1]=FindSecondMost(hallBuf);
//			sciSendString("\r\nHallValueAB=",bMessage);
//			sciSendString(int2String(hallAB[0],stringBuf),bMessage);			
//			sciSendString(int2String(hallAB[1],stringBuf),bMessage);			
//			break;
//		}		
//	}
//	if(feedback==FEEDBACK_YES)
//	{
//		sciSendString("\r\n--> PWMC->A is OK",bMessage);
//		feedback=FEEDBACK_NONE;
//	}
//	else
//	{
//		sciSendString("\r\n--> PWMC->A is NOT WORKING",bMessage);
//		feedback=FEEDBACK_NONE;
//	}



////C->B
//	TIM_SetCompare1(TIM1,0);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
//	TIM_SetCompare2(TIM1,0);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
//	TIM_SetCompare3(TIM1,T_PWM/10);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);
//	while(feedback==FEEDBACK_NONE)//wait for feedback
//	{
//		vTaskDelay(timedelay);
//		hallBuf[counter]=hallValue;
//		counter++;
////		sciSendString("\r\nHallValue=",bMessage);
////		sciSendString(int2String(hallValue,stringBuf),bMessage);
//		if(counter>timeout)
//		{
//			counter=0;
//			hallAB[0]=FindMost(hallBuf);
//			hallAB[1]=FindSecondMost(hallBuf);
//			sciSendString("\r\nHallValueAB=",bMessage);
//			sciSendString(int2String(hallAB[0],stringBuf),bMessage);			
//			sciSendString(int2String(hallAB[1],stringBuf),bMessage);			
//			break;
//		}		
//	}
//	if(feedback==FEEDBACK_YES)
//	{
//		sciSendString("\r\n--> PWMC->B is OK",bMessage);
//		feedback=FEEDBACK_NONE;
//	}
//	else
//	{
//		sciSendString("\r\n--> PWMC->B is NOT WORKING",bMessage);
//		feedback=FEEDBACK_NONE;
//	}

//	GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);
//	vTaskDelete(NULL);
//}


//int FindMost(int number[20])
//{
//	int i=0;
//	int a=-1;
//	int countera=0;
//	int b=-1;
//	int counterb=0;
//	int c=-1;
//	int counterc=0;
//	a=number[0];
//	for(i=0;i<20;i++)
//	{
//		if((a!=number[i])&&(b==-1))
//		{
//			b=number[i];
//		}
//		if((a!=number[i])&&(b!=-1))
//		{
//			c=number[i];
//		}
//		if(a==number[i])
//		{
//			countera++;
//		}
//		else if(b==number[i])
//		{
//			counterb++;
//		}
//		else if(c==number[i])
//		{
//			counterc++;
//		}
//	}
//		if(countera>=counterb)
//		{
//			if(countera>=counterc)
//			{
//				return a;
//			}
//			else
//			{
//				return c;
//			}
//		}
//		else
//		{
//			if(counterb>=counterc)
//			{
//				return b;
//			}
//			else
//			{
//				return c;
//			}
//		}	
//}

//int FindSecondMost(int number[20])
//{
//	int i=0;
//	int a=-1;
//	int countera=0;
//	int b=-1;
//	int counterb=0;
//	int c=-1;
//	int counterc=0;
//	a=number[0];
//	for(i=0;i<20;i++)
//	{
//		if((a!=number[i])&&(b==-1))
//		{
//			b=number[i];
//		}
//		if((a!=number[i])&&(b!=-1))
//		{
//			c=number[i];
//		}
//		if(a==number[i])
//		{
//			countera++;
//		}
//		else if(b==number[i])
//		{
//			counterb++;
//		}
//		else if(c==number[i])
//		{
//			counterc++;
//		}
//	}	
//		
//		if(countera>=counterb)
//		{
//			if(countera>=counterc)
//			{
//				if(counterb>=counterc)
//				{
//					return b;
//				}
//				else
//				{
//					return c;
//				}
//			}
//			else
//			{
//				return a;
//			}
//		}
//		else
//		{
//			if(counterb>=counterc)
//			{
//				if(countera>=counterc)
//				{
//					return a;
//				}
//				else
//				{
//					return c;
//				}
//			}
//			else
//			{
//				return b;
//			}
//		}	
//}

/* CAN Test */
void testCANTask( void *pvParameters )
{
	int t=0;
	int can_id = 0x01;//
	char stringBuf[100];
	A = 60;
	sciSendString("\r\nTESTING CAN",bMessage);
	SetMotorType(MOTORTYPE_BDCMOTOR);
	SetOperationMode(VELOCITY_MODE);
	Switchon2();
	
	CAN_send(can_id,MODES_OF_OPERATION_INDEX,0,ModesofOperation.Param);
	CAN_send(can_id,MOTORTYPE_INDEX,0,MotorType.Param);
	CAN_send(can_id,CONTROLWORD_INDEX,0,ControlWord.Param);	
	for(;;)
	{
		vTaskDelay(10);
		TargetVelocity.Param = A*sin(w*t);
		t++;
		//CAN_send(can_id,TARGETVELOCITY_INDEX,0,TargetVelocity.Param);
		sciSendString("\r\nTESTING CAN",bMessage);
		sciSendString(float2String(HW_VERSION,1,stringBuf),bMessage);
	}
	

	//vTaskDelete(NULL);
}


///* DS18B20 Test */
//void testTEMPTask( void *pvParameters )
//{
//	int i= 10;
//	DS18B20_init();// IO INIT

//	for(i=10;i>0;i--)
//	{
//		vTaskDelay(500);
//		DS18B20_Convert();
//		DS18B20_Read();
//		sciSendString("\r\nTemperature is ",bMessage);
//		sciSendString(Temperature,bMessage);
//	}
//	sciSendString("\r\nDS18B20 TEST FINISHED",bMessage);
//	vTaskDelete(NULL);
//}

//void testENCODERTask(void * pvParameters)
//{
////	long Position[6];
////	int Rev = 1000*4;
////	long HallPos[6];
//	sciSendString("\r\nStarting PWM testing!",bMessage);
//	ControlWord.Param= 0x02;//进入测试模式
//	PWMAB(TIM1, T_PWM/5);
//	vTaskDelay(2000);
//	Encoder.S=0;
//	Position[0]=Encoder.S;
//	
//	PWMAC(TIM1, T_PWM/5);
//	vTaskDelay(2000);
//	Position[1]=Encoder.S;
//	
//	PWMBC(TIM1, T_PWM/5);
//	vTaskDelay(2000);
//	Position[2]=Encoder.S;
//	
//	PWMBA(TIM1, T_PWM/5);
//	vTaskDelay(2000);
//	Position[3]=Encoder.S;
//	
//	PWMCA(TIM1, T_PWM/5);
//	vTaskDelay(2000);
//	Position[4]=Encoder.S;
//	
//	PWMCB(TIM1, T_PWM/5);
//	vTaskDelay(2000);
//	Position[5]=Encoder.S;	
//	
//	PWMAB(TIM1, T_PWM/5);
//	vTaskDelay(2000);
//	Position[6]=Encoder.S;

//	HallPos[0]=(Position[1]+Position[2])/2;
//	HallPos[1]=(Position[2]+Position[3])/2;
//	HallPos[2]=(Position[3]+Position[4])/2;
//	HallPos[3]=(Position[4]+Position[5])/2;
//	HallPos[4]=(Position[5]+Position[0])/2;
//	HallPos[5]=(Position[0]+Position[1])/2;
//	ControlWord.Param= 0x00;//结束测试模式	
//	sciSendString("\r\nENCODER TEST FINISHED",bMessage);	
//	vTaskDelete(NULL);
//}

//void testSVPWMTask(void * pvParameters)
//{

//	sciSendString("\r\nStarting SVPWM testing!",bMessage);
//	ControlWord.Param= 0x02;//进入测试模式
//	//PWMC_EN();
//	PWMA(TIM1, T_PWM/10);
//	vTaskDelay(1000);
//	
//	Encoder.LastP = 0;
//	TIM3->CNT = 0;
//	Encoder.S=0;
//	ControlWord.Param=0;
//	vTaskDelete(NULL);

//	
//}

//void testSVPWMTask(void * pvParameters)
//{

//	sciSendString("\r\nStarting SVPWM testing!",bMessage);
//	ControlWord.Param= 0x01;//进入测试模式
//	pwm = 1000;

//	for(;;)
//	{
//		vTaskDelay(1);
//		#ifdef USE_FOC
//		FOC.Theta+=50;
//		FOC.Vref = pwm;
//		if(FOC.Theta>4000)
//			FOC.Theta=0;
//		SVPWM(&FOC);
//		#endif
//	}

//	
//}

/*========================== END OF FILE =======================================*/
