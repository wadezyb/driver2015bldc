#ifndef _PWM_H_
#define _PWM_H_

#define T_PWM (3360/2)//50kHz
//#define T_PWM (5250)//16kHz
//#define T_PWM (8400)//10kHz
//#define T_PWM (4200)//20kHz
/*
* PWM½Ó¿Ú:
* PWM1->PA9->TIM1 CH2
* PWM2->PA10->TIM1 CH3
* PWM3->PA8->TIM1 CH1
* ENA->PB14
* ENB->PB15
* ENC->PB13
*/
#define PWMA_EN() 	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET)
#define PWMA_DIS() 	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET)
#define PWMB_EN() 	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET)
#define PWMB_DIS() 	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET)
#define PWMC_EN() 	GPIO_WriteBit(GPIOC,GPIO_Pin_6,Bit_SET)
#define PWMC_DIS() 	GPIO_WriteBit(GPIOC,GPIO_Pin_6,Bit_RESET)

#define PWM1_SET(TIM1,pwm) TIM_SetCompare1(TIM1,pwm)
#define PWM2_SET(TIM1,pwm) TIM_SetCompare2(TIM1,pwm)
#define PWM3_SET(TIM1,pwm) TIM_SetCompare3(TIM1,pwm)


#define PWMAB(TIM1,pwm) {PWM1_SET(TIM1,pwm);PWM2_SET(TIM1,0);PWM3_SET(TIM1,0);PWMA_EN();PWMB_EN();PWMC_DIS();}
#define PWMBA(TIM1,pwm) {PWM1_SET(TIM1,0);PWM2_SET(TIM1,pwm);PWM3_SET(TIM1,0);PWMA_EN();PWMB_EN();PWMC_DIS();}
#define PWMBC(TIM1,pwm) {PWM1_SET(TIM1,0);PWM2_SET(TIM1,pwm);PWM3_SET(TIM1,0);PWMA_DIS();PWMB_EN();PWMC_EN();}
#define PWMCB(TIM1,pwm) {PWM1_SET(TIM1,0);PWM2_SET(TIM1,0);PWM3_SET(TIM1,pwm);PWMA_DIS();PWMB_EN();PWMC_EN();}
#define PWMAC(TIM1,pwm) {PWM1_SET(TIM1,pwm);PWM2_SET(TIM1,0);PWM3_SET(TIM1,0);PWMA_EN();PWMB_DIS();PWMC_EN();}
#define PWMCA(TIM1,pwm) {PWM1_SET(TIM1,0);PWM2_SET(TIM1,0);PWM3_SET(TIM1,pwm);PWMA_EN();PWMB_DIS();PWMC_EN();}

#define PWMA(TIM1,pwm) {PWM1_SET(TIM1,pwm);PWM2_SET(TIM1,0);PWM3_SET(TIM1,0);PWMA_EN();PWMB_EN();PWMC_EN();}

#define PWMON() {PWMA_EN();PWMB_EN();PWMC_EN();}
#define PWMOFF() {PWMA_DIS();PWMB_DIS();PWMC_DIS();}

#define PWM_SET(pwm1,pwm2,pwm3) {PWM1_SET(TIM1,pwm1);PWM2_SET(TIM1,pwm2);PWM3_SET(TIM1,pwm3);}

void pwmConfiguration( void );
void testPWMTask( void *pvParameters );
#endif
