#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "main.h"
#include "stm32f4xx.h"


typedef struct
{
//	PIFFTypeDef PIFF;
//	PIFFTypeDef FOCDPIFF;
//	PIFFTypeDef FOCQPIFF;
//	PIFFTypeDef VPIFF;
	int maxpwm;
}memoryType;

extern memoryType memory;


/* FLASK PAGE127 地址 *///中容量最后一页
#define PAGE127_ADDR 0X0801FC00

/* 存储起始地址 */
#define START_ADDR PAGE127_ADDR

/* Current Loop Parameters */
/* address for kcp */
#define KCP_ADDR (START_ADDR+4*1)

/* address for kci */
#define KCI_ADDR ( START_ADDR+4*2 )

/* address for kcff */
#define KCFF_ADDR ( START_ADDR+4*3 )

/* address for maxTorqeError */
#define MAXTERROR_ADDR (( START_ADDR+4*4 ))

/* address for maxTorqeError */
#define MAXPWM_ADDR (( START_ADDR+4*5 ))

/* FOC D Current Loop Parameters */
/* address for kcp */
#define FOCDKCP_ADDR (START_ADDR+4*6)

/* address for kci */
#define FOCDKCI_ADDR ( START_ADDR+4*7 )

/* address for kcff */
#define FOCDKCFF_ADDR ( START_ADDR+4*8 )

/* FOC Q Current Loop Parameters */
/* address for kcp */
#define FOCQKCP_ADDR (START_ADDR+4*9)

/* address for kci */
#define FOCQKCI_ADDR ( START_ADDR+4*10 )

/* address for kcff */
#define FOCQKCFF_ADDR ( START_ADDR+4*11 )

//---
/* Velocity Loop Parameters */
/* address for kcp */
#define VKCP_ADDR (START_ADDR+4*12)

/* address for kci */
#define VKCI_ADDR ( START_ADDR+4*13 )

/* address for kcff */
#define VKCFF_ADDR ( START_ADDR+4*14 )

void memoryRemberWord( uint32_t Address, uint32_t Data );
void memoryRemberHalfWord( uint32_t Address, uint16_t Data );
long memoryReadWord( uint32_t Address );
void memoryErasePage( uint32_t Page_Address );
void memoryRemberAll( memoryType* memory );
void initAllParameters( memoryType* memory );
#endif


