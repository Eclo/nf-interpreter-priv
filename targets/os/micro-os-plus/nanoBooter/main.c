#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"

#define LD4_Pin GPIO_PIN_12
#define LD4_GPIO_Port GPIOD
#define LD3_Pin GPIO_PIN_13
#define LD3_GPIO_Port GPIOD
#define LD5_Pin GPIO_PIN_14
#define LD5_GPIO_Port GPIOD
#define LD6_Pin GPIO_PIN_15
#define LD6_GPIO_Port GPIOD
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define CLK_IN_Pin GPIO_PIN_10
#define CLK_IN_GPIO_Port GPIOB


void SystemClock_Config(void);
void Error_Handler(void);
void MX_GPIO_Init(void);
void StartDefaultTask(void const * argument);

TIM_HandleTypeDef        htim1; 
uint32_t                 uwIncrementState = 0;

int
cube_mx_init(void);

#define main cube_mx_init

int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  HAL_DBGMCU_EnableDBGSleepMode();
  HAL_DBGMCU_EnableDBGStopMode();
  HAL_DBGMCU_EnableDBGStandbyMode();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  //MX_GPIO_Init();
    return 0;
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);

}

void MX_GPIO_Init(void)
{
//   GPIO_InitTypeDef GPIO_InitStruct;

//   /* GPIO Ports Clock Enable */
//   __HAL_RCC_GPIOD_CLK_ENABLE();

//   /*Configure GPIO pin Output Level */
//   HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin, GPIO_PIN_RESET);

//   /*Configure GPIO pin : BOOT1_Pin */
//   GPIO_InitStruct.Pin = BOOT1_Pin;
//   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//   GPIO_InitStruct.Pull = GPIO_NOPULL;
//   HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

//   /*Configure GPIO pin : CLK_IN_Pin */
//   GPIO_InitStruct.Pin = CLK_IN_Pin;
//   GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//   GPIO_InitStruct.Pull = GPIO_NOPULL;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//   GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
//   HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

//   /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin */
//   GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin;
//   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//   GPIO_InitStruct.Pull = GPIO_NOPULL;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//   HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}


/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
// {
//   if (htim->Instance == TIM1) {
//     HAL_IncTick();
//   }
// }

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
}

void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  os_systick_handler ();
}

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
// void NMI_Handler(void)
// {
// }

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
// void HardFault_Handler(void)
// {
//   /* Go to infinite loop when Hard Fault exception occurs */
//   while (1)
//   {
//   }
// }

/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{

  /* System interrupt init*/
  /* SVC_IRQn interrupt configuration */
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/
  /* MemoryManagement_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
  /* BusFault_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
  /* UsageFault_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
  /* SVCall_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
  /* DebugMonitor_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
  /* PendSV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/**
  * @brief  This function configures the TIM1 as a time base source. 
  *         The time source is configured  to have 1ms time base with a dedicated 
  *         Tick interrupt priority. 
  * @note   This function is called  automatically at the beginning of program after
  *         reset by HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig(). 
  * @param  TickPriority: Tick interrupt priorty.
  * @retval HAL status
  */
// HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
// {
//   RCC_ClkInitTypeDef    clkconfig;
//   uint32_t              uwTimclock = 0;
//   uint32_t              uwPrescalerValue = 0;
//   uint32_t              pFLatency;
  
//   /*Configure the TIM1 IRQ priority */
//   HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, TickPriority ,0); 
  
//   /* Enable the TIM1 global Interrupt */
//   HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn); 
  
//   /* Enable TIM1 clock */
//   __HAL_RCC_TIM1_CLK_ENABLE();
  
//   /* Get clock configuration */
//   HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);
  
//   /* Compute TIM1 clock */
//   uwTimclock = 2*HAL_RCC_GetPCLK2Freq();
   
//   /* Compute the prescaler value to have TIM1 counter clock equal to 1MHz */
//   uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000) - 1);
  
//   /* Initialize TIM1 */
//   htim1.Instance = TIM1;
  
//   /* Initialize TIMx peripheral as follow:
//   + Period = [(TIM1CLK/1000) - 1]. to have a (1/1000) s time base.
//   + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
//   + ClockDivision = 0
//   + Counter direction = Up
//   */
//   htim1.Init.Period = (1000000 / 1000) - 1;
//   htim1.Init.Prescaler = uwPrescalerValue;
//   htim1.Init.ClockDivision = 0;
//   htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
//   if(HAL_TIM_Base_Init(&htim1) == HAL_OK)
//   {
//     /* Start the TIM time Base generation in interrupt mode */
//     return HAL_TIM_Base_Start_IT(&htim1);
//   }
   
//   /* Return function status */
//   return HAL_ERROR;
// }

/**
  * @brief  Suspend Tick increment.
  * @note   Disable the tick increment by disabling TIM1 update interrupt.
  * @param  None
  * @retval None
  */
// void HAL_SuspendTick(void)
// {
//   /* Disable TIM1 update Interrupt */
//   __HAL_TIM_DISABLE_IT(&htim1, TIM_IT_UPDATE);                                                  
// }

/**
  * @brief  Resume Tick increment.
  * @note   Enable the tick increment by Enabling TIM1 update interrupt.
  * @param  None
  * @retval None
  */
// void HAL_ResumeTick(void)
// {
//   /* Enable TIM1 Update interrupt */
//   __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);
// }

// void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
// {
//   HAL_TIM_IRQHandler(&htim1);
// }
