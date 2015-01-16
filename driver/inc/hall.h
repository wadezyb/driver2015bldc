#ifndef _HALL_H_
#define _HALL_H_

/*
* 霍尔传感器接口：
* HALL1->PA0
* HALL2->PA1
* HALL3->PA2
*/
#define RCC_APB2Periph_GPIOHALL RCC_APB2Periph_GPIOA
#define GPIOHALL GPIOA
#define GPIO_Pin_HALL1 GPIO_Pin_0
#define GPIO_Pin_HALL2 GPIO_Pin_1
#define GPIO_Pin_HALL3 GPIO_Pin_2

extern int speed;
extern int hallValue;
void HALL_Configuration(void);
void hallTask( void *pvParameters );
#endif

