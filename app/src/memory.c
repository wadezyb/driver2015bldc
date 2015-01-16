/*
================================================================================
* File Name                 :   memory.c
* Copyright                 :   2011-2013
* 
* CPU                       :   STM32F103
* RTOS                      :   FreeRTOS
*
* Create Date               :   2012/12/1
* Author/Company            :   Zhan Yubo/UESTC
* Abstract Description      :   Configuration and Applications of Memory
*...............................................................................
* Revision History
* NO.     Date      Revised by      Item      Description
*	NO.1		2013/7/16	Zhan Yubo				Note			Added some necessary notes.
*
================================================================================
*/
#include "memory.h"
#include "includes.h"

memoryType memory;

/****
	*	brief: ´æ´¢ ×Ö 32Î»
	*/
void memoryRemberWord( uint32_t Address, uint32_t Data )
{
	FLASH_Unlock();
	FLASH_ProgramWord( Address, (uint32_t)Data);
	FLASH_Lock();	
}


/****	
	*	brief: ´æ´¢ °ë×Ö 16Î»
	*/
void memoryRemberHalfWord( uint32_t Address, uint16_t Data )
{
	FLASH_Unlock();
	FLASH_ProgramHalfWord( Address, (uint16_t)Data);
	FLASH_Lock();
}

/****	
	*	brief: ¶ÁÈ¡ ×Ö
	*/
long memoryReadWord( uint32_t Address )
{
	return *( (uint32_t*)Address );	
}


/****
	*	brief: ²Á³ý ÕûÒ³
	*/
void memoryErasePage( uint32_t Page_Address )
{
	FLASH_Unlock();
	FLASH_ErasePage( Page_Address );
	FLASH_Lock();
}

/* Write all parameters to the flash */
void memoryRemberAll( memoryType* memory )
{
	union
	{
		int temp;
		float tempf;
	}mem;
	
	/* ²Á³ýÕûÒ³ */
	memoryErasePage( START_ADDR );

	/* Ð´Èë±êÖ¾ */
	memoryRemberWord( START_ADDR, 0X01 );
	
	
	
	/* write kcp */
	mem.tempf=memory->PIFF.kp;
	memoryRemberWord( KCP_ADDR, mem.temp );

	/* write kci */
	mem.tempf=memory->PIFF.ki;
	memoryRemberWord( KCI_ADDR, mem.temp );

	/* write kcff */
	mem.tempf=memory->PIFF.kff;
	memoryRemberWord( KCFF_ADDR, mem.temp );

	/* write max t error*/
	mem.tempf=memory->PIFF.maxTError;
	memoryRemberWord( MAXTERROR_ADDR, mem.temp );	
	
	/* write maxpwm */
	mem.tempf=memory->maxpwm;
	memoryRemberWord( MAXPWM_ADDR, mem.temp );	

	/* write focdkcp */
	mem.tempf=memory->FOCDPIFF.kp;
	memoryRemberWord( FOCDKCP_ADDR, mem.temp );

	/* write focdkci */
	mem.tempf=memory->FOCDPIFF.ki;
	memoryRemberWord( FOCDKCI_ADDR, mem.temp );

	/* write focdkcff */
	mem.tempf=memory->FOCDPIFF.kff;
	memoryRemberWord( FOCDKCFF_ADDR, mem.temp );

	/* write focqkcp */
	mem.tempf=memory->FOCQPIFF.kp;
	memoryRemberWord( FOCQKCP_ADDR, mem.temp );

	/* write focqkci */
	mem.tempf=memory->FOCQPIFF.ki;
	memoryRemberWord( FOCQKCI_ADDR, mem.temp );

	/* write focqkcff */
	mem.tempf=memory->FOCQPIFF.kff;
	memoryRemberWord( FOCQKCFF_ADDR, mem.temp );

//----
	/* write Vkcp */
	mem.tempf=memory->VPIFF.kp;
	memoryRemberWord( VKCP_ADDR, mem.temp );

	/* write Vkci */
	mem.tempf=memory->VPIFF.ki;
	memoryRemberWord( VKCI_ADDR, mem.temp );

	/* write Vkcff */
	mem.tempf=memory->VPIFF.kff;
	memoryRemberWord( VKCFF_ADDR, mem.temp );
}

void initAllParameters( memoryType* memory )
{
	union
	{
		int temp;
		float tempf;
	}mem;
	mem.temp = memoryReadWord( KCP_ADDR );
	memory->PIFF.kp = mem.tempf;
	mem.temp = memoryReadWord( KCI_ADDR );
	memory->PIFF.ki =  mem.tempf;
	mem.temp = memoryReadWord( KCFF_ADDR );
	memory->PIFF.kff =  mem.tempf;
	mem.temp = memoryReadWord( MAXTERROR_ADDR );
	memory->PIFF.maxTError = mem.tempf;
	mem.temp = memoryReadWord( MAXPWM_ADDR );
	memory->maxpwm = mem.tempf;	
	/* foc d piff */
	mem.temp = memoryReadWord( FOCDKCP_ADDR );
	memory->FOCDPIFF.kp = mem.tempf;
	mem.temp = memoryReadWord( FOCDKCI_ADDR );
	memory->FOCDPIFF.ki =  mem.tempf;
	mem.temp = memoryReadWord( FOCDKCFF_ADDR );
	memory->FOCDPIFF.kff =  mem.tempf;
	/* foc q piff */
	mem.temp = memoryReadWord( FOCQKCP_ADDR );
	memory->FOCQPIFF.kp = mem.tempf;
	mem.temp = memoryReadWord( FOCQKCI_ADDR );
	memory->FOCQPIFF.ki =  mem.tempf;
	mem.temp = memoryReadWord( FOCQKCFF_ADDR );
	memory->FOCQPIFF.kff =  mem.tempf;	
	
	/* VELOCITY piff */
	mem.temp = memoryReadWord( VKCP_ADDR );
	memory->VPIFF.kp = mem.tempf;
	mem.temp = memoryReadWord( VKCI_ADDR );
	memory->VPIFF.ki =  mem.tempf;
	mem.temp = memoryReadWord( VKCFF_ADDR );
	memory->VPIFF.kff =  mem.tempf;		
}

/****
	*	
	*/
void memoryResetParameter(  )
{
	
}

/*========================== END OF FILE =======================================*/
