#ifndef   __BSP_STEPPER_H
#define  __BSP_STEPPER_H
 

#include "stm32f10x.h"

// Motor 方向
#define MOTOR_DIR_GPIO_PORT      GPIOB
#define MOTOR_DIR_GPIO_CLK       RCC_APB2Periph_GPIOB
#define MOTOR_DIR_GPIO_PIN       GPIO_Pin_12
// Motor 使能
#define MOTOR_EN_GPIO_PORT       GPIOB
#define MOTOR_EN_GPIO_CLK        RCC_APB2Periph_GPIOB
#define MOTOR_EN_GPIO_PIN        GPIO_Pin_6

// Motor 脉冲
#define MOTOR_PUL_GPIO_PORT      GPIOB
#define MOTOR_PUL_GPIO_CLK       RCC_APB2Periph_GPIOB
#define MOTOR_PUL_GPIO_PIN       GPIO_Pin_7


#define ON  0
#define OFF 1

#define CW  1         // 顺时针
#define CCW 0         // 逆时针


#define MOTOR_EN(a)     if(a)  \
															GPIO_SetBits(MOTOR_EN_GPIO_PORT, MOTOR_EN_GPIO_PIN);\
													else     \
															GPIO_ResetBits(MOTOR_EN_GPIO_PORT, MOTOR_EN_GPIO_PIN)

#define MOTOR_PUL(a)    if(a)  \
															GPIO_SetBits(MOTOR_PUL_GPIO_PORT, MOTOR_PUL_GPIO_PIN);\
													else     \
															GPIO_ResetBits(MOTOR_PUL_GPIO_PORT, MOTOR_PUL_GPIO_PIN)

#define MOTOR_DIR(a)    if(a)  \
															GPIO_SetBits(MOTOR_DIR_GPIO_PORT, MOTOR_DIR_GPIO_PIN);\
													else     \
															GPIO_ResetBits(MOTOR_DIR_GPIO_PORT, MOTOR_DIR_GPIO_PIN)
													

void stepper_turn(int tim,float angle,float subdivide,uint8_t dir);
void stepper_Init(void);													
													
													
#endif /*_BSP_STEPPER_H */
