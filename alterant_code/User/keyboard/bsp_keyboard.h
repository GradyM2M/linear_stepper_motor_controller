#ifndef   __BSP_KEYBOARD_H
#define   __BSP_KEYBOARD_H
 

#include "stm32f10x.h"

//行输入端口定义
#define X1_GPIO_CLK     RCC_APB2Periph_GPIOC
#define X1_GPIO_PORT    GPIOC 
#define X1_GPIO_PIN     GPIO_Pin_5

#define X2_GPIO_CLK     RCC_APB2Periph_GPIOC
#define X2_GPIO_PORT    GPIOC 
#define X2_GPIO_PIN     GPIO_Pin_7

#define X3_GPIO_CLK     RCC_APB2Periph_GPIOC
#define X3_GPIO_PORT    GPIOC 
#define X3_GPIO_PIN     GPIO_Pin_4

#define X4_GPIO_CLK     RCC_APB2Periph_GPIOC
#define X4_GPIO_PORT    GPIOC 
#define X4_GPIO_PIN     GPIO_Pin_6

//列输入端口定义
#define Y1_GPIO_CLK     RCC_APB2Periph_GPIOE
#define Y1_GPIO_PORT    GPIOE 
#define Y1_GPIO_PIN     GPIO_Pin_12

#define Y2_GPIO_CLK     RCC_APB2Periph_GPIOE
#define Y2_GPIO_PORT    GPIOE 
#define Y2_GPIO_PIN     GPIO_Pin_15

#define Y3_GPIO_CLK     RCC_APB2Periph_GPIOE
#define Y3_GPIO_PORT    GPIOE 
#define Y3_GPIO_PIN     GPIO_Pin_13

#define Y4_GPIO_CLK     RCC_APB2Periph_GPIOE
#define Y4_GPIO_PORT    GPIOE 
#define Y4_GPIO_PIN     GPIO_Pin_14

//#define uchar unsigned char
	


void Keyboard_Init(void);
int Keyboard_Scan(void);
//uint8_t Keyboard_Test(void);
int* Keyboard_Test(void);
//uint8_t Keyboard_input(void);
//uint16_t Keyboard_Input(int a);
//static uint16_t MYS(int a);

#endif /*__BSP_KEYBOARD_H */
