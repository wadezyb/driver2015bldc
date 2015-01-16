#ifndef _FOC_H_
#define _FOC_H_

typedef struct
{
	int Sector;
	int Alpha;//转子的位置角度
	int Theta;//电压向量的的角度
	float Iq;
	float Id;
	float Ia;
	float Ib;
	int T1;
	int T2;
	int T3;
	int T0;
	int PWM1;
	int PWM2;
	int PWM3;
	int Vd;
	int Vq;
	int Vu;
	int Vv;
	int Vw;
	int Va;
	int Vb;
	int Vref;
	int Dir;
}FOC_Obj;

#define Tz (T_PWM-100)

#ifdef USE_FOC

#define Iu adc2
#define Iv adc1

#define FOC_MAX_VREF T_PWM
#define PI (3.1415926)
extern FOC_Obj FOC;

float Sin( int pos );
float Cos( int pos );
void SVPWM( FOC_Obj* FOC );
void FOCPWM( FOC_Obj* FOC);
void FOC_Init(FOC_Obj* FOC);
#endif
void InitDQPID( void );
#endif


