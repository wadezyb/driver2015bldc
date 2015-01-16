#ifndef _CLOOP_H_
#define _CLOOP_H_

#define Tz (T_PWM-100)
typedef struct
{
	int error;
	int kp;
	int ki;
	int scale;
	int sum_error;
	int sum_error_max;
	int output_max;
	int output_min;
	int output;
}cLoopPIObj;

typedef struct
{
	cLoopPIObj dPI;
	cLoopPIObj qPI;
	int Theta;// rotor angle for 0 to 3600(0-2PI)
	int targetCurrent; //mA
	int Id_exp;
	int Iq_exp;
	int IA;
	int IB;
	int IA_Offset;
	int IB_Offset;
	int IAlpha;
	int IBeta;
	int Id;
	int Iq;
	int Vd;
	int Vq;
	int V_Alpha;
	int V_Beta;
	int V_total;
	float V_theta;
	
}cLoopObj;



typedef struct
{
	int Vref;
	int Sector;
	int Alpha;//转子的位置角度
	float Theta;//电压向量的的角度
	int T1;
	int T2;
	int T0;
	int PWM1;
	int PWM2;
	int PWM3;
	int Vdc;
//	int Dir;
//	
//	int tempA;
//	float tempB;
}SVPWMObj;

extern cLoopObj Current;
//extern int Cd[1000];
extern int Cq[1000];
extern int Cn;
extern int tuningStartFlag;
extern int Cti;

float SIN( int pos );
float COS( int pos );
void currentLoopInit( void );
void currentLoop( void );
void test_cloopTask( void *pvParameters );

#endif
