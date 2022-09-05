#ifndef   __BSP_DELAY_H
#define  __BSP_DELAY_H
 

#include "stm32f10x.h"
#include "core_cm3.h"

void SysTick_Delay_us(uint32_t us);
void SysTick_Delay_ms(uint32_t ms);


#endif /*_BSP_DELAY_H */
