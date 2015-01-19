#ifndef _ENCODER_H_
#define _ENCODER_H_

#define ENCODER_COUNTER TIM3->CNT

#define INC_ENC_REVOLUTION (int)(500*4) // 增量编码器分辨率
#define ABS_ENC_REVOLUTION (int)(500*4)//绝对值编码器分辨率
#define MOTOR_POLE (4)
#define ELEC_REVOLUTION_INC (INC_ENC_REVOLUTION/MOTOR_POLE)
#define ELEC_REVOLUTION_ABS (ABS_ENC_REVOLUTION/MOTOR_POLE)

#define K (1.38) 	//绝对编码器（4096每圈）和增量编码器分辨率之比
#define D (int)(4096/K)			//增量编码器的分辨率

/*编码器读数结构体*/
typedef struct _encoderReadType
{
	int V; //速度
	int DP;	//位置变化量
	int LastV;
	int LastP;//上一次的位置
	int CNT32;//32bits soft encoder counter
	int Multi;
	int Single;
	int Value;
	int Delta;
}encoderReadType;

typedef struct
{
	char flag;
	char buf[15];
	int counter;
	int num;
	int value;
	int multi;
	int single;
	int sign;
	
}absObj;

///*三个编码器的读取数据的结构体*/
extern encoderReadType Encoder;
extern absObj absEnc;

#define MOTOR_POSITION (int)(Encoder.S+Encoder.CNT32)
void Encoder_Configuration(void);
void USART4_Configuration( void );
void encoderTask ( void *pvParameters );
#endif
