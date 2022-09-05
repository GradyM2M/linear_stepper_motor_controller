#include "bsp_stepper.h"
#include "./delay/bsp_delay.h"


void stepper_turn(int tim,float angle,float subdivide,uint8_t dir)
{
	int n,i;
	n=(int)(angle/(1.8/subdivide));
	
	if(dir==CW)   // ˳ʱ��
	{
		MOTOR_DIR(CW); 
	}
	else if(dir==CCW)   // ��ʱ��
	{
		MOTOR_DIR(CCW);
	}
	
	// ��ʹ��
	MOTOR_EN(ON);    
	
	// ģ�ⷽ��
	for(i=0;i<n;i++)
	{
		MOTOR_PUL(OFF);
		SysTick_Delay_us(tim/2);
		MOTOR_PUL(ON);
		SysTick_Delay_us(tim/2);
	}
	// ��ʹ��
	MOTOR_EN(OFF);
}

void stepper_Init()
{
	// ����һ��GPIO_InitTypeDef���͵Ľṹ��
	GPIO_InitTypeDef GPIO_InitStruce;
	
	// �������GPIO������ʱ��
	RCC_APB2PeriphClockCmd( MOTOR_DIR_GPIO_CLK | MOTOR_PUL_GPIO_CLK | MOTOR_EN_GPIO_CLK, ENABLE);
	
	// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_InitStruce.GPIO_Pin = MOTOR_DIR_GPIO_PIN;
	
	// ��������Ϊ�������
	GPIO_InitStruce.GPIO_Mode = GPIO_Mode_Out_PP;
	
	// ��������Ϊ����
	GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
	
	// �������ų�ʼ��
	GPIO_Init(MOTOR_DIR_GPIO_PORT, &GPIO_InitStruce);
	
	// �������ų�ʼ��
	GPIO_InitStruce.GPIO_Pin = MOTOR_PUL_GPIO_PIN;
	GPIO_Init(MOTOR_PUL_GPIO_PORT, &GPIO_InitStruce);
	
	// ʹ�����ų�ʼ��
	GPIO_InitStruce.GPIO_Pin = MOTOR_EN_GPIO_PIN;
	GPIO_Init(MOTOR_EN_GPIO_PORT, &GPIO_InitStruce);
	
	// �ص�ʹ��
	MOTOR_EN(OFF);
}

