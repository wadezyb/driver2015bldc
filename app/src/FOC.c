/**
  ******************************************************************************
  * @file    FOC.c
  * @author  ZHAN Yubo
  * @version V9.0.0
  * @date    03-May-2014
  * @brief   This file is Field Orentied Control Algorithm for motor control
  *
*/

/* INCLUDES */
#include "foc.h"
#include "includes.h"
#include "pwm.h"
#include <math.h>
#include "encoder.h"

/**
  * @brief  Initialize the FOC parameters
  * @param  None
  * @retval None
  */
void FOC_Init(FOC_Obj* FOC)
{
	FOC->Alpha = 0;
	FOC->Ia = 0;
	FOC->Ib = 0;
	FOC->Id = 0;
	FOC->Iq = 0;
	FOC->PWM1 = 0;
	FOC->PWM2 = 0;
	FOC->PWM3 = 0;
	FOC->Sector = 0;
	FOC->T0 = 0;
	FOC->T1 = 0;
	FOC->T2 = 0;
	FOC->T3 = 0;
	FOC->Theta = 0;
	FOC->Va = 0;
	FOC->Vb = 0;
	FOC->Vd = 0;
	FOC->Vq = 0;
	FOC->Vref = 0;
	FOC->Vu = 0;
	FOC->Vw = 0;
	FOC->Vv = 0;
}

/**
  * @brief  Space Vector PWM
  * @param  None
  * @retval None
  */
void SVPWM( FOC_Obj* FOC )
{
	int Vdc = Tz;
	FOC->Sector = (int)(FOC->Theta/THETA1)%6+1;//Atan2(FOC->Vb,FOC->Va);
	switch(FOC->Sector)
	{
		case 1:
			FOC->T1 = Tz*FACTOR1*Sin( THETA1-FOC->Theta );
			FOC->T2 = Tz*FACTOR1*Sin( FOC->Theta );
			FOC->T3 = FOC->T1+FOC->T2;
			FOC->T1 = Tz*FOC->T1/FOC->T3*FOC->Vref/Vdc;
			FOC->T2 = Tz*FOC->T2/FOC->T3*FOC->Vref/Vdc;
			FOC->T0 = Tz-FOC->T1-FOC->T2;
			
			FOC->PWM1 = FOC->T1+FOC->T2+FOC->T0/2;
			FOC->PWM2 = FOC->T2+FOC->T0/2;
			FOC->PWM3 = FOC->T0/2;
			break;
		case 2:
			FOC->T1 = Tz*FACTOR1*Sin( 2*THETA1-FOC->Theta );
			FOC->T2 = Tz*FACTOR1*Sin( FOC->Theta-THETA1 );
			FOC->T3 = FOC->T1+FOC->T2;
			FOC->T1 = Tz*FOC->T1/FOC->T3*FOC->Vref/Vdc;
			FOC->T2 = Tz*FOC->T2/FOC->T3*FOC->Vref/Vdc;			
			FOC->T0 = Tz-FOC->T1-FOC->T2;
			
			FOC->PWM1 = FOC->T1+FOC->T0/2;
			FOC->PWM2 = FOC->T1+FOC->T2+FOC->T0/2;
			FOC->PWM3 = FOC->T0/2;			
			break;
		case 3:
			FOC->T1 = Tz*FACTOR1*Sin( 3*THETA1-FOC->Theta );
			FOC->T2 = Tz*FACTOR1*Sin( FOC->Theta-2*THETA1 );
			FOC->T3 = FOC->T1+FOC->T2;
			FOC->T1 = Tz*FOC->T1/FOC->T3*FOC->Vref/Vdc;
			FOC->T2 = Tz*FOC->T2/FOC->T3*FOC->Vref/Vdc;			
			FOC->T0 = Tz-FOC->T1-FOC->T2;			
			
			FOC->PWM1 = FOC->T0/2;
			FOC->PWM2 = FOC->T1+FOC->T2+FOC->T0/2;
			FOC->PWM3 = FOC->T2+FOC->T0/2;			
			break;
		case 4:
			FOC->T1 = Tz*FACTOR1*Sin( 4*THETA1-FOC->Theta );
			FOC->T2 = Tz*FACTOR1*Sin( FOC->Theta-3*THETA1 );
			FOC->T3 = FOC->T1+FOC->T2;
			FOC->T1 = Tz*FOC->T1/FOC->T3*FOC->Vref/Vdc;
			FOC->T2 = Tz*FOC->T2/FOC->T3*FOC->Vref/Vdc;			
			FOC->T0 = Tz-FOC->T1-FOC->T2;				
			
			FOC->PWM1 = FOC->T0/2;
			FOC->PWM2 = FOC->T1+FOC->T0/2;
			FOC->PWM3 = FOC->T1+FOC->T2+FOC->T0/2;			
			break;
		case 5:
			FOC->T1 = Tz*FACTOR1*Sin( 5*THETA1-FOC->Theta );
			FOC->T2 = Tz*FACTOR1*Sin( FOC->Theta-4*THETA1 );
			FOC->T3 = FOC->T1+FOC->T2;
			FOC->T1 = Tz*FOC->T1/FOC->T3*FOC->Vref/Vdc;
			FOC->T2 = Tz*FOC->T2/FOC->T3*FOC->Vref/Vdc;			
			FOC->T0 = Tz-FOC->T1-FOC->T2;				
			
			FOC->PWM1 = FOC->T2+FOC->T0/2;
			FOC->PWM2 = FOC->T0/2;
			FOC->PWM3 = FOC->T1+FOC->T2+FOC->T0/2;			
			break;
		case 6:
			FOC->T1 = Tz*FACTOR1*Sin( 6*THETA1-FOC->Theta );
			FOC->T2 = Tz*FACTOR1*Sin( FOC->Theta-5*THETA1 );
			FOC->T3 = FOC->T1+FOC->T2;
			FOC->T1 = Tz*FOC->T1/FOC->T3*FOC->Vref/Vdc;
			FOC->T2 = Tz*FOC->T2/FOC->T3*FOC->Vref/Vdc;			
			FOC->T0 = Tz-FOC->T1-FOC->T2;			
			
			FOC->PWM1 = FOC->T1+FOC->T2+FOC->T0/2;
			FOC->PWM2 = FOC->T0/2;
			FOC->PWM3 = FOC->T1+FOC->T0/2;			
			break;
		default:
			break;
	}
	//
	if(FOC->PWM1>Tz)
		FOC->PWM1=Tz;
	if(FOC->PWM1<-Tz)
		FOC->PWM1 = -Tz;
	if(FOC->PWM2>Tz)
		FOC->PWM2=Tz;
	if(FOC->PWM2<-Tz)
		FOC->PWM2 = -Tz;
	if(FOC->PWM3>Tz)
		FOC->PWM3=Tz;
	if(FOC->PWM3<-Tz)
		FOC->PWM3 = -Tz;
	//ÉèÖÃÈýÂ·PWM
//	GPIO_WriteBit(GPIOC,GPIO_Pin_6,Bit_SET);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);
	TIM_SetCompare1(TIM1,FOC->PWM1);//
	TIM_SetCompare2(TIM1,FOC->PWM2);//
	TIM_SetCompare3(TIM1,FOC->PWM3);//
}

/**
  * @brief  Calc the 
	*					Vref 		
	*					Alpha
	*					Theta
	*					according to the position
  * @param  None
  * @retval None
  */
void FOCPWM( FOC_Obj* FOC )
{
	FOC->Alpha =3600*(ELEC_REVOLUTION_INC-Encoder.Single%ELEC_REVOLUTION_INC)/(ELEC_REVOLUTION_INC);
	FOC->Theta = FOC->Alpha+(FOC->Dir==1?(900):(2700));
	SVPWM(FOC);
}
#endif

/*========================== END OF FILE =======================================*/

