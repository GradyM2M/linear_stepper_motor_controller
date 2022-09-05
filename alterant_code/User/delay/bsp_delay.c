#include "bsp_delay.h"


void SysTick_Delay_us(uint32_t us)
{
	uint32_t i;
	
	SysTick_Config(72);
	
	for(i=0; i<us; i++)
	{
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Delay_ms(uint32_t ms)
{
	uint32_t i;
	
	SysTick_Config(72000);
	
	for(i=0; i<ms; i++)
	{
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// 关闭SysTick定时器
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
}


