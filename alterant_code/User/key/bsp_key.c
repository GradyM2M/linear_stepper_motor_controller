#include "bsp_key.h"

void KEY_GPIO_Config(void)
{
	// 定义一个GPIO_InitTypeDef类型的结构体
	GPIO_InitTypeDef GPIO_InitStruce;
	
	// 开启相关GPIO的外设时钟
	RCC_APB2PeriphClockCmd( KEY1_GPIO_CLK | KEY2_GPIO_CLK, ENABLE);
	
	// 选择要控制的GPIO引脚
	GPIO_InitStruce.GPIO_Pin = KEY1_GPIO_PIN;
	
	// 设置引脚为悬空输入模式
	GPIO_InitStruce.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	// 初始化GPIO
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruce);
	
	
	// key2
	GPIO_InitStruce.GPIO_Pin = KEY2_GPIO_PIN;
	
	GPIO_InitStruce.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruce);
	
}

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	//检查是否有按键按下
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON){
		//等待按键释放
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);
		return KEY_ON;
	}else
	return KEY_OFF;
	
}

