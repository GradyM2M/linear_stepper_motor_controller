#include "bsp_delay.h"


void SysTick_Delay_us(uint32_t us)
{
	uint32_t i;
	
	SysTick_Config(72);
	
	for(i=0; i<us; i++)
	{
		// ����������ֵ��С��0��ʱ��CRTL�Ĵ�����λ16����1
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
		// ����������ֵ��С��0��ʱ��CRTL�Ĵ�����λ16����1
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// �ر�SysTick��ʱ��
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
}


