/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define MAX_INPUT_NUM  5
// 0.005 mm/STEP, so 1 mm = 200 step
// 步进电机的细分
// 1600 步/每转 = 1/8细分(实测)
#define DISTANCE_2_STPES(i)  ((i) * 8 / 5) //um
#define STPES_2_DISTANCE(i)  ((i) * 5 / 8) //um
// Now the timmer is 50 000 steps/s


int keyPad_lastKey = 0;
int tim_cnt = 0;
int speed = 0; // mm/S, , MAX 10mm/S
int distance = 0; // MAX = 19mm
int distance_steps = 0;
int distance_pulse = 0;
void flash_led()
{
  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}

void show_err(char *str)
{
  //OLED_Clear();
  OLED_Clearrow(3);
  OLED_ShowString(6,0, str, 16);
}

int KeyPad_Scan(void)
{
  int key = 0;
  uint16_t c_tbl[4] = {GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3};
  uint16_t r_tbl[4] = {GPIO_PIN_4,GPIO_PIN_5,GPIO_PIN_6,GPIO_PIN_7};
  for (int c = 0; c < 4; c++) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, c_tbl[c], GPIO_PIN_RESET);
    for (int r = 0; r < 4; r++) {
      if (HAL_GPIO_ReadPin(GPIOA, r_tbl[r]) == GPIO_PIN_RESET){
        HAL_Delay(1);
        if (HAL_GPIO_ReadPin(GPIOA, r_tbl[r]) == GPIO_PIN_RESET){
          key |= 1<<c;					
          key |= 1<<(r+8);
          while(HAL_GPIO_ReadPin(GPIOA, r_tbl[r] == GPIO_PIN_RESET))
            HAL_Delay(5);
          return key;
        }
      } 
    }
  }
  return key;
}

  
uint16_t	KeyPad_WaitForKey(uint32_t  Timeout_ms)
{	
  uint16_t  keyRead;
  while(Timeout_ms==0)
  {
    keyRead = KeyPad_Scan();
		if(keyRead!=0)
		{
			keyPad_lastKey = keyRead;
			return keyRead;	
		}
		HAL_Delay(1);
	}
	uint32_t	StartTime = HAL_GetTick();
	while(HAL_GetTick()-StartTime < Timeout_ms)
	{
		keyRead = KeyPad_Scan();
		if(keyRead!=0)
		{
			keyPad_lastKey = keyRead;
			return keyRead;	
		}
		HAL_Delay(1);	
	}
	keyPad_lastKey=0;
	return 0;
}

char	KeyPad_WaitForKeyGetChar(uint32_t	Timeout_ms)
{
  switch(KeyPad_WaitForKey(Timeout_ms))
  {
    case 0x0000:
      return 0;
		case 0x0101:
			return '1';
		case 0x0201:
			return '2';
		case 0x0401:
			return '3';
		case 0x0801:
			return 'A';
		case 0x0102:
			return '4';
		case 0x0202:
			return '5';
		case 0x0402:
			return '6';
		case 0x0802:
			return 'B';
		case 0x0104:
			return '7';
		case 0x0204:
			return '8';		
		case 0x0404:
			return '9';
		case 0x0804:
			return 'C';
		case 0x0108:
			return '*';				
		case 0x0208:
			return '0';				
		case 0x0408:
			return '#';
		case 0x0808:
			return 'D';
		
		default:
			return 0;		
	}	
}

//0.005 mm/STEP

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) 
{ //72M / (8+1) / (9+1) = 8*10^5Hz
  if(htim->Instance == TIM1){ //50
    if ((distance_pulse > 0) && (speed != 0)) {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        distance_pulse--;
    }
  }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  char c = 0;
  int i = 0;
  int input_val = 0;
  GPIO_PinState is_forward = GPIO_PIN_RESET;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  //初始化oled屏幕
  OLED_Init();
	//开启OLED显示    
	OLED_Display_On();
  OLED_Clear();
  //show_err("Stepmotor");
  //HAL_TIM_Base_Start_IT(&htim1); //10us
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    HAL_Delay(300);
    //flash_led();
    c = KeyPad_WaitForKeyGetChar(20);
    if ((c >= '0') && (c <= '9')) {
      if (i < MAX_INPUT_NUM) {
        input_val *= 10;
        input_val += c - '0';
        i++;
        distance = input_val;
      }
    } else if (c == '#'){ 
      OLED_Clearrow(3);
      OLED_Clearrow(6);
      HAL_TIM_Base_Stop_IT(&htim1);

      distance_steps = DISTANCE_2_STPES(distance); //debug
      distance_pulse = distance_steps * 2;
      i = 0;
      input_val = 0;
      HAL_TIM_Base_Start_IT(&htim1);
    } else if (c == '*'){  //SPEED
      OLED_Clearrow(3);
      OLED_Clearrow(6);
      HAL_TIM_Base_Stop_IT(&htim1);
      speed = (speed + 1) % 11;
      if (speed != 0) {
        htim1.Init.Period = 500/speed/2 - 1; // 499 = 800steps/s
        HAL_TIM_Base_DeInit(&htim1);
        HAL_TIM_Base_Init(&htim1);
        HAL_TIM_Base_Start_IT(&htim1);
      } else {
        HAL_TIM_Base_Stop_IT(&htim1);
      }
    } else if (c == 'C') { 
      HAL_TIM_Base_Stop_IT(&htim1);
      distance = 0;
      distance_steps = 0;
      distance_pulse = 0;
      i = 0;
      input_val = 0;
    } else if (c == 'D') {
      HAL_TIM_Base_Stop_IT(&htim1);
    } else if (c == 'A') {
      HAL_TIM_Base_Start_IT(&htim1);
    } else if (c == 'B') {
      is_forward = !is_forward;
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, is_forward);
      show_err(is_forward ? "Forward" : "Back");
    }
    //
    OLED_ShowNum(0, 3, distance, MAX_INPUT_NUM, 16);
    OLED_ShowNum(48,3, STPES_2_DISTANCE(distance_pulse/2), MAX_INPUT_NUM, 16);
    OLED_ShowString(96,3, "um", 16);
    OLED_ShowNum(6,6, speed, MAX_INPUT_NUM, 16);
    OLED_ShowString(48,6, "mm/s", 16);
    
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
