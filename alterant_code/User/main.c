#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include "./delay/bsp_delay.h"
#include "./key/bsp_key.h"
#include "./stepper/bsp_stepper.h"
#include "bsp_oled.h"
#include "./keyboard/bsp_keyboard.h"


/************************************
        按键表盘为：		1   2   3   12
											4   5   6   13
											7   8   9   14
											10  0   11  15 
				10 -- 正转按键；
				11 -- 反转按键；
				15 -- 确定按键；
//				12 -- 设置单位1对应转角位18°- 0.05mm；
//				13 -- 设置单位1对应转角位36°- 0.1mm；
//				14 -- 设置单位1对应转角位180°- 0.05cm；
************************************/
/*
	SCL-PA5
	SDA-PA7
	RES-PD4
	DC-PD15
	CS-PD1
*/


int main(void)
{

	int key_val=1;            // 0-电机使能
	int dir_val=0;            // 转向 0-正转 1-反转
	int angle_val=0;          // 转角
	Keyboard_Init();          // 按键初始化
	stepper_Init();           // 步进电机初始化
	OLED_Init();              // OLED的初始化
	OLED_ColorTurn(0);        // 0正常显示，1反色显示
	OLED_DisplayTurn(0);      // 0正常显示，1屏幕反转显示
	
	int i=0;
	int a=0;
	int key_num;
	int h_value,l_value,value;

	
	while(1)
	{
		OLED_Refresh();
		//SysTick_Delay_ms(500);
		//OLED_Clear();
		
		// 16*16转 向：
		OLED_ShowChinese(0,0,0,16,1);         // 转
		OLED_ShowChinese(36,0,1,16,1);        // 向
		OLED_ShowString(54,0,":",16,1);       // ：
		// 16*16脉冲数：
		OLED_ShowChinese(0,18,2,16,1);        // 脉
		OLED_ShowChinese(18,18,3,16,1);       // 冲
		OLED_ShowChinese(36,18,4,16,1);       // 数
		OLED_ShowString(54,18,":",16,1);      // ：
		
		GPIO_SetBits(X1_GPIO_PORT,X1_GPIO_PIN);  // X1输出高电平
		GPIO_SetBits(X2_GPIO_PORT,X2_GPIO_PIN);  // X2输出高电平
		GPIO_SetBits(X3_GPIO_PORT,X3_GPIO_PIN);  // X3输出高电平
		GPIO_SetBits(X4_GPIO_PORT,X4_GPIO_PIN);  // X4输出高电平
		int Y1,Y2,Y3,Y4=0;
		Y1=GPIO_ReadInputDataBit(Y1_GPIO_PORT,Y1_GPIO_PIN);  
		Y2=GPIO_ReadInputDataBit(Y2_GPIO_PORT,Y2_GPIO_PIN);
		Y3=GPIO_ReadInputDataBit(Y3_GPIO_PORT,Y3_GPIO_PIN);
		Y4=GPIO_ReadInputDataBit(Y4_GPIO_PORT,Y4_GPIO_PIN);
		key_num=Keyboard_Scan();
		// 检测是否有按键按下，((Y1||Y2||Y3||Y4)==1)则有按键按下
		if((Y1||Y2||Y3||Y4)==1)
		{
			if(key_num<10)
			{
				a=++i%2;
				if(a==1)
				{
					OLED_Refresh();OLED_ShowString(72,18,"_",16,1);
					switch(key_num)
					{
						case 0: OLED_Refresh();OLED_ShowString(72,18,"0",16,1);h_value=0*10;break;
						case 1: OLED_Refresh();OLED_ShowString(72,18,"1",16,1);h_value=1*10;break;
						case 2: OLED_Refresh();OLED_ShowString(72,18,"2",16,1);h_value=2*10;break;
						case 3: OLED_Refresh();OLED_ShowString(72,18,"3",16,1);h_value=3*10;break;
						case 4: OLED_Refresh();OLED_ShowString(72,18,"4",16,1);h_value=4*10;break;
						case 5: OLED_Refresh();OLED_ShowString(72,18,"5",16,1);h_value=5*10;break;
						case 6: OLED_Refresh();OLED_ShowString(72,18,"6",16,1);h_value=6*10;break;
						case 7: OLED_Refresh();OLED_ShowString(72,18,"7",16,1);h_value=7*10;break;
						case 8: OLED_Refresh();OLED_ShowString(72,18,"8",16,1);h_value=8*10;break;
						case 9: OLED_Refresh();OLED_ShowString(72,18,"9",16,1);h_value=9*10;break;;
					}
				}
				if(a==0)
				{
					OLED_Refresh();OLED_ShowString(80,18,"_",16,1);
					switch(key_num)
					{
						case 0: OLED_Refresh();OLED_ShowString(80,18,"0",16,1);l_value=0;break;
						case 1: OLED_Refresh();OLED_ShowString(80,18,"1",16,1);l_value=1;break;
						case 2: OLED_Refresh();OLED_ShowString(80,18,"2",16,1);l_value=2;break;
						case 3: OLED_Refresh();OLED_ShowString(80,18,"3",16,1);l_value=3;break;
						case 4: OLED_Refresh();OLED_ShowString(80,18,"4",16,1);l_value=4;break;
						case 5: OLED_Refresh();OLED_ShowString(80,18,"5",16,1);l_value=5;break;
						case 6: OLED_Refresh();OLED_ShowString(80,18,"6",16,1);l_value=6;break;
						case 7: OLED_Refresh();OLED_ShowString(80,18,"7",16,1);l_value=7;break;
						case 8: OLED_Refresh();OLED_ShowString(80,18,"8",16,1);l_value=8;break;
						case 9: OLED_Refresh();OLED_ShowString(80,18,"9",16,1);l_value=9;break;
					}
				}
			}
		}
		
		if(key_num==10)
		{
			dir_val=ON;
			OLED_Refresh();
			OLED_ShowChinese(63,0,5,16,1);   // 正
			OLED_ShowChinese(81,0,0,16,1);   // 转
			key_val=OFF;
		}
		if(key_num==11)
		{
			dir_val=OFF;
			OLED_Refresh();
			OLED_ShowChinese(63,0,6,16,1);   // 反
			OLED_ShowChinese(81,0,0,16,1);   // 转
			key_val=OFF;
		}
//		if(key_num==12)
//		{
//			c=18;
//		}
//		if(key_num==13)
//		{
//			c=36;
//		}
//		if(key_num==14)
//		{
//			c=180;
//		}
		if(key_num==15)
		{
			key_val=ON;
		}
		value=h_value+l_value;
		angle_val=value*18;
		
		if( key_val == ON )
		{
			stepper_turn(1000,angle_val,32,dir_val);  // 1KHZ
			key_val=OFF;
		}

		
	}



	
}



//int main(void)
//{	
////	NVIC_Configuration();
////	OLED_Init();
////	OLED_Clear();
//	
//	
//	// 0-电机旋转
//	int key_val=0;
//	//转向
//	int dir_val=0;
//	//转角
//	int angle_val=1080;
//	// 按键初始化
//	KEY_GPIO_Config();
//	// 步进电机初始化
//	stepper_Init();
//	// 开启步进电机使能
//  while(1)
//	{
//		
//		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON )
//		{
//			dir_val = OFF;
//			key_val=ON;
//		}
//		if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON )
//		{
//			dir_val = ON;
//			key_val=ON;
//		}
//		
//		if( key_val == ON )
//		{
//			stepper_turn(1000,angle_val,32,dir_val);  // 1KHZ
//			key_val=OFF;
//		}
//		
//	}	
//}




