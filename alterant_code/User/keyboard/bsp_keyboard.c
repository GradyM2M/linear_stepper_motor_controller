#include "bsp_keyboard.h"
#include "bsp_delay.h"
#include "bsp_oled.h"

int key_num;
int flag;
int i=0;

// 初始化GPIO
void Keyboard_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(X1_GPIO_CLK|Y1_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin=X1_GPIO_PIN|X2_GPIO_PIN|X3_GPIO_PIN|X4_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(X1_GPIO_PORT, &GPIO_InitStructure);
	/*.....*/
	GPIO_InitStructure.GPIO_Pin=Y1_GPIO_PIN|Y2_GPIO_PIN|Y3_GPIO_PIN|Y4_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(Y1_GPIO_PORT, &GPIO_InitStructure);
}

int Keyboard_Scan(void)
{
	uint16_t KeyboardVal;
	flag = ((GPIO_ReadInputDataBit(Y1_GPIO_PORT,Y1_GPIO_PIN))|(GPIO_ReadInputDataBit(Y2_GPIO_PORT,Y2_GPIO_PIN))|(GPIO_ReadInputDataBit(Y3_GPIO_PORT,Y3_GPIO_PIN))|(GPIO_ReadInputDataBit(Y4_GPIO_PORT,Y4_GPIO_PIN)));
  GPIO_SetBits(X1_GPIO_PORT,X1_GPIO_PIN);  //先让X1输出高电平
  GPIO_SetBits(X2_GPIO_PORT,X2_GPIO_PIN);  //先让X2输出高电平
  GPIO_SetBits(X3_GPIO_PORT,X3_GPIO_PIN);  //先让X3输出高电平
  GPIO_SetBits(X4_GPIO_PORT,X4_GPIO_PIN);  //先让X4输出高电平
	
	/* 若Y1到Y4全为低电平，则没有按键按下 */
	if(flag==0x0000)
		return -1;
	else
	{
		SysTick_Delay_ms(10);
		if(flag==0x0000)
			return -1;
	}
	
	int Y1,Y2,Y3,Y4 = 0;
	/* X1输出高电平，X2-X4输出低电平
		 检测键盘第一行							*/
	GPIO_SetBits(X1_GPIO_PORT,X1_GPIO_PIN);
	GPIO_ResetBits(X2_GPIO_PORT,X2_GPIO_PIN);
	GPIO_ResetBits(X3_GPIO_PORT,X3_GPIO_PIN);
	GPIO_ResetBits(X4_GPIO_PORT,X4_GPIO_PIN);

	Y1=GPIO_ReadInputDataBit(Y1_GPIO_PORT,Y1_GPIO_PIN);
	Y2=GPIO_ReadInputDataBit(Y2_GPIO_PORT,Y2_GPIO_PIN);
  Y3=GPIO_ReadInputDataBit(Y3_GPIO_PORT,Y3_GPIO_PIN);
	Y4=GPIO_ReadInputDataBit(Y4_GPIO_PORT,Y4_GPIO_PIN);
	
	if(Y1==1&&Y2==0&&Y3==0&&Y4==0)
	{
		KeyboardVal=1;
	}         
	if(Y1==0&&Y2==1&&Y3==0&&Y4==0)
            KeyboardVal=2;
	if(Y1==0&&Y2==0&&Y3==1&&Y4==0)
            KeyboardVal=3;
	if(Y1==0&&Y2==0&&Y3==0&&Y4==1)
            KeyboardVal=12;                  
	while(((GPIO_ReadInputDataBit(Y1_GPIO_PORT,Y1_GPIO_PIN))|(GPIO_ReadInputDataBit(Y2_GPIO_PORT,Y2_GPIO_PIN))|(GPIO_ReadInputDataBit(Y3_GPIO_PORT,Y3_GPIO_PIN))|(GPIO_ReadInputDataBit(Y4_GPIO_PORT,Y4_GPIO_PIN)))>0);
	
	
	/* X2输出高电平，X1,X3,X4输出低电平
		 检测键盘第二行							    */
	GPIO_ResetBits(X1_GPIO_PORT,X4_GPIO_PIN);
	GPIO_SetBits(X2_GPIO_PORT,X2_GPIO_PIN);
	GPIO_ResetBits(X3_GPIO_PORT,X3_GPIO_PIN);
	GPIO_ResetBits(X4_GPIO_PORT,X1_GPIO_PIN);

	Y1=GPIO_ReadInputDataBit(Y1_GPIO_PORT,Y1_GPIO_PIN);
	Y2=GPIO_ReadInputDataBit(Y2_GPIO_PORT,Y2_GPIO_PIN);
  Y3=GPIO_ReadInputDataBit(Y3_GPIO_PORT,Y3_GPIO_PIN);
	Y4=GPIO_ReadInputDataBit(Y4_GPIO_PORT,Y4_GPIO_PIN);
	
	if(Y1==1&&Y2==0&&Y3==0&&Y4==0)
            KeyboardVal=4;
	if(Y1==0&&Y2==1&&Y3==0&&Y4==0)
            KeyboardVal=5;
	if(Y1==0&&Y2==0&&Y3==1&&Y4==0)
            KeyboardVal=6;
	if(Y1==0&&Y2==0&&Y3==0&&Y4==1)
            KeyboardVal=13;                
	while(((GPIO_ReadInputDataBit(Y1_GPIO_PORT,Y1_GPIO_PIN))|(GPIO_ReadInputDataBit(Y2_GPIO_PORT,Y2_GPIO_PIN))|(GPIO_ReadInputDataBit(Y3_GPIO_PORT,Y3_GPIO_PIN))|(GPIO_ReadInputDataBit(Y4_GPIO_PORT,Y4_GPIO_PIN)))>0);
	
	/* X3输出高电平，X1,X2,X4输出低电平
		 检测键盘第三行							    */
	GPIO_ResetBits(X1_GPIO_PORT,X4_GPIO_PIN);
	GPIO_ResetBits(X2_GPIO_PORT,X2_GPIO_PIN);
	GPIO_SetBits(X3_GPIO_PORT,X3_GPIO_PIN);
	GPIO_ResetBits(X4_GPIO_PORT,X1_GPIO_PIN);

	Y1=GPIO_ReadInputDataBit(Y1_GPIO_PORT,Y1_GPIO_PIN);
	Y2=GPIO_ReadInputDataBit(Y2_GPIO_PORT,Y2_GPIO_PIN);
  Y3=GPIO_ReadInputDataBit(Y3_GPIO_PORT,Y3_GPIO_PIN);
	Y4=GPIO_ReadInputDataBit(Y4_GPIO_PORT,Y4_GPIO_PIN);
	
	if(Y1==1&&Y2==0&&Y3==0&&Y4==0)
            KeyboardVal=7;
	if(Y1==0&&Y2==1&&Y3==0&&Y4==0)
            KeyboardVal=8;
	if(Y1==0&&Y2==0&&Y3==1&&Y4==0)
            KeyboardVal=9;
	if(Y1==0&&Y2==0&&Y3==0&&Y4==1)
            KeyboardVal=14;                  
	while(((GPIO_ReadInputDataBit(Y1_GPIO_PORT,Y1_GPIO_PIN))|(GPIO_ReadInputDataBit(Y2_GPIO_PORT,Y2_GPIO_PIN))|(GPIO_ReadInputDataBit(Y3_GPIO_PORT,Y3_GPIO_PIN))|(GPIO_ReadInputDataBit(Y4_GPIO_PORT,Y4_GPIO_PIN)))>0);
	
	/* X1-X3输出低电平，X4输出高电平 
		 检测第四行         					 */
	GPIO_ResetBits(X1_GPIO_PORT,X1_GPIO_PIN);
	GPIO_ResetBits(X2_GPIO_PORT,X2_GPIO_PIN);
	GPIO_ResetBits(X3_GPIO_PORT,X3_GPIO_PIN);
	GPIO_SetBits(X4_GPIO_PORT,X4_GPIO_PIN);
	
	Y1=GPIO_ReadInputDataBit(Y1_GPIO_PORT,Y1_GPIO_PIN);
	Y2=GPIO_ReadInputDataBit(Y2_GPIO_PORT,Y2_GPIO_PIN);
  Y3=GPIO_ReadInputDataBit(Y3_GPIO_PORT,Y3_GPIO_PIN);
	Y4=GPIO_ReadInputDataBit(Y4_GPIO_PORT,Y4_GPIO_PIN);
	
	if(Y1==1&&Y2==0&&Y3==0&&Y4==0)
            KeyboardVal=10;                    // 设置键
	if(Y1==0&&Y2==1&&Y3==0&&Y4==0)
	{
		KeyboardVal=0;
	}
	if(Y1==0&&Y2==0&&Y3==1&&Y4==0)
            KeyboardVal=11;
	if(Y1==0&&Y2==0&&Y3==0&&Y4==1)
            KeyboardVal=15;                     // 确认键
	
	// 等待按键释放
	while(((GPIO_ReadInputDataBit(Y1_GPIO_PORT,Y1_GPIO_PIN))|(GPIO_ReadInputDataBit(Y2_GPIO_PORT,Y2_GPIO_PIN))|(GPIO_ReadInputDataBit(Y3_GPIO_PORT,Y3_GPIO_PIN))|(GPIO_ReadInputDataBit(Y4_GPIO_PORT,Y4_GPIO_PIN)))>0);

	return KeyboardVal;

}


/************************************
        按键表盘为：		1  2  3   12 
											4  5  6   13
											7  8  9   14
										 10  0  11  15 
************************************/
