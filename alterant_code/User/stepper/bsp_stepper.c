#include "bsp_stepper.h"
#include "./delay/bsp_delay.h"


void stepper_turn(int tim,float angle,float subdivide,uint8_t dir)
{
	int n,i;
	n=(int)(angle/(1.8/subdivide));
	
	if(dir==CW)   // 顺时针
	{
		MOTOR_DIR(CW); 
	}
	else if(dir==CCW)   // 逆时针
	{
		MOTOR_DIR(CCW);
	}
	
	// 开使能
	MOTOR_EN(ON);    
	
	// 模拟方波
	for(i=0;i<n;i++)
	{
		MOTOR_PUL(OFF);
		SysTick_Delay_us(tim/2);
		MOTOR_PUL(ON);
		SysTick_Delay_us(tim/2);
	}
	// 关使能
	MOTOR_EN(OFF);
}

void stepper_Init()
{
	// 定义一个GPIO_InitTypeDef类型的结构体
	GPIO_InitTypeDef GPIO_InitStruce;
	
	// 开启相关GPIO的外设时钟
	RCC_APB2PeriphClockCmd( MOTOR_DIR_GPIO_CLK | MOTOR_PUL_GPIO_CLK | MOTOR_EN_GPIO_CLK, ENABLE);
	
	// 选择要控制的GPIO引脚
	GPIO_InitStruce.GPIO_Pin = MOTOR_DIR_GPIO_PIN;
	
	// 设置引脚为推免输出
	GPIO_InitStruce.GPIO_Mode = GPIO_Mode_Out_PP;
	
	// 设置引脚为高速
	GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
	
	// 方向引脚初始化
	GPIO_Init(MOTOR_DIR_GPIO_PORT, &GPIO_InitStruce);
	
	// 脉冲引脚初始化
	GPIO_InitStruce.GPIO_Pin = MOTOR_PUL_GPIO_PIN;
	GPIO_Init(MOTOR_PUL_GPIO_PORT, &GPIO_InitStruce);
	
	// 使能引脚初始化
	GPIO_InitStruce.GPIO_Pin = MOTOR_EN_GPIO_PIN;
	GPIO_Init(MOTOR_EN_GPIO_PORT, &GPIO_InitStruce);
	
	// 关掉使能
	MOTOR_EN(OFF);
}

