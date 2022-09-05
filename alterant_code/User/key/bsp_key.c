#include "bsp_key.h"

void KEY_GPIO_Config(void)
{
	// ����һ��GPIO_InitTypeDef���͵Ľṹ��
	GPIO_InitTypeDef GPIO_InitStruce;
	
	// �������GPIO������ʱ��
	RCC_APB2PeriphClockCmd( KEY1_GPIO_CLK | KEY2_GPIO_CLK, ENABLE);
	
	// ѡ��Ҫ���Ƶ�GPIO����
	GPIO_InitStruce.GPIO_Pin = KEY1_GPIO_PIN;
	
	// ��������Ϊ��������ģʽ
	GPIO_InitStruce.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	// ��ʼ��GPIO
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruce);
	
	
	// key2
	GPIO_InitStruce.GPIO_Pin = KEY2_GPIO_PIN;
	
	GPIO_InitStruce.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruce);
	
}

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	//����Ƿ��а�������
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON){
		//�ȴ������ͷ�
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);
		return KEY_ON;
	}else
	return KEY_OFF;
	
}

