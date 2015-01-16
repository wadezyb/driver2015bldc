#ifndef _DMA_H_
#define _DMA_H_

extern short ADC1_Value[5];
extern int adc;
extern int adc1,adc2;
extern long Position[7];
extern int Rev ;
extern long HallPos[7];
void DMA_Configuration(void);
#define V_BUS (ADC1_Value[3])
#endif

