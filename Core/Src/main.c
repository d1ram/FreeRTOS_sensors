/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// Handlers
#include "btn_handler.h"

// Services
#include "monitor_service.h"
#include "watchdog_service.h"
#include "i2c_bus.h"
#include "bmp280_service.h"
#include "ui_state.h"
#include "fault_diag.h"

// Tasks
#include "watchdog_task.h"
#include "gatekeeper_task.h"
#include "bmp_task.h"
#include "page_task.h"
#include "bmp_print_task.h"
#include "gyro_print_task.h"
#include "gyro_task.h"

// STD
#include <stdio.h>
#include <stdint.h>
#include <string.h>
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

I2C_HandleTypeDef hi2c1;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* USER CODE BEGIN PV */
SemaphoreHandle_t hi2c_mutex;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
void StartDefaultTask(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
/* USER CODE BEGIN 2 */

// For jitter_testing we open PA6 and PA7 for GPIO_OUTPUT.
#ifdef DIAG
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitTypeDef mark = {0};
  mark.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8;
  mark.Mode = GPIO_MODE_OUTPUT_PP;
  mark.Pull = GPIO_NOPULL;
  mark.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &mark);
#endif

  // BMP_PRINT_TASK INIT (data)
  int32_t bmp_p_1 = watchdog_add_task(bmp_print_task_watchdogMIN_TIME_MS, bmp_print_task_watchdogMAX_TIME_MS, "bmpPrintTask1");
  uint32_t bmp_p_g_1 = 0;
  static BmpPrintTaskConfig_t bmp_p_data1 = {
      .function = bmp_service_get_temperature,
      "C"};
  bmp_p_data1.task_id_gatekeeper = bmp_p_g_1;
  bmp_p_data1.watchdog_id = bmp_p_1;

  int32_t bmp_p_2 = watchdog_add_task(bmp_print_task_watchdogMIN_TIME_MS, bmp_print_task_watchdogMAX_TIME_MS, "bmpPrintTask2");
  uint32_t bmp_p_g_2 = 1;
  static BmpPrintTaskConfig_t bmp_p_data2 = {
      .function = bmp_service_get_pressure,
      "hPa"};
  bmp_p_data2.task_id_gatekeeper = bmp_p_g_2;
  bmp_p_data2.watchdog_id = bmp_p_2;

  int32_t bmp_p_3 = watchdog_add_task(bmp_print_task_watchdogMIN_TIME_MS, bmp_print_task_watchdogMAX_TIME_MS, "bmpPrintTask3");
  uint32_t bmp_p_g_3 = 2;
  static BmpPrintTaskConfig_t bmp_p_data3 = {
      .function = bmp_service_get_altitude,
      "m"};
  bmp_p_data3.task_id_gatekeeper = bmp_p_g_3;
  bmp_p_data3.watchdog_id = bmp_p_3;

  // GYRO_PRINT_TASK INIT (data)
  int32_t gyro_p_1 = watchdog_add_task(gyro_print_task_watchdogMIN_TIME_MS, gyro_print_task_watchdogMAX_TIME_MS, "gyroPrintTask1");
  uint32_t gyro_p_g_1 = gatekeeperMAX_TASK_PER_PAGE; // 2nd page
  static GyroPrintTaskConfig_t gyro_p_data1;
  gyro_p_data1.watchdog_id = gyro_p_1;
  gyro_p_data1.task_id_gatekeeper = gyro_p_g_1;
  gyro_p_data1.data_axis = X;

  int32_t gyro_p_2 = watchdog_add_task(gyro_print_task_watchdogMIN_TIME_MS, gyro_print_task_watchdogMAX_TIME_MS, "gyroPrintTask2");
  uint32_t gyro_p_g_2 = gatekeeperMAX_TASK_PER_PAGE + 1;
  static GyroPrintTaskConfig_t gyro_p_data2;
  gyro_p_data2.watchdog_id = gyro_p_2;
  gyro_p_data2.task_id_gatekeeper = gyro_p_g_2;
  gyro_p_data2.data_axis = Y;

  int32_t gyro_p_3 = watchdog_add_task(gyro_print_task_watchdogMIN_TIME_MS, gyro_print_task_watchdogMAX_TIME_MS, "gyroPrintTask3");
  uint32_t gyro_p_g_3 = gatekeeperMAX_TASK_PER_PAGE + 2;
  static GyroPrintTaskConfig_t gyro_p_data3;
  gyro_p_data3.watchdog_id = gyro_p_3;
  gyro_p_data3.task_id_gatekeeper = gyro_p_g_3;
  gyro_p_data3.data_axis = Z;

  // GATEKEEPER_TASK INIT
  int32_t gtk_id = watchdog_add_task(gatekeeper_watchdogMIN_TIME, gatekeeper_watchdogMAX_TIME, "gatekeeperTASK");
  static GatekeeperTaskConfig_t gdata = {
      .task_names = {"Temp", "Pres", "Alti", "\0", "\0", "X", "Y", "Z"}};
  gdata.watchdog_id = gtk_id;

  // BMP_TASK INIT
  int32_t bmp_id = watchdog_add_task(bmp_task_watchdogMIN_TIME_MS, bmp_task_watchdogMAX_TIME_MS, "bmpTASK");
  static BmpTaskConfig_t bdata;
  bdata.watchdog_id = bmp_id;

  // GYRO_TASK INIT
  int32_t gyro_id = watchdog_add_task(gyro_task_watchdogMIN_TIME_MS, gyro_task_watchdogMAX_TIME_MS, "gyroTASK");
  static GyroTaskConfig_t gyro_data;
  gyro_data.watchdog_id = gyro_id;

  // WATCHDOG_TASK INIT
  static WatchdogTaskMapping_t temp_tasks[watchdogMAX_MONITORED_TASKS];
  static WatchdogConfig_t wdata;

  temp_tasks[0].task_watchdog_id = bmp_id;
  temp_tasks[0].task_gatekeeper_id = gatekeeperMAX_TASK_PER_PAGE * 2;

  temp_tasks[1].task_watchdog_id = gyro_id;
  temp_tasks[1].task_gatekeeper_id = (gatekeeperMAX_TASK_PER_PAGE * 2) + 1;

  memcpy(wdata.mon_tasks, temp_tasks, sizeof(temp_tasks));
  wdata.task_count = 2;

  // SERVICES INIT
  watchdog_init();
  fault_diag_init();
  monitor_init(&hi2c1);
  bmp_service_init(&hi2c1);
  gyro_service_init(&hi2c1);

  // TASK CREATION
  xTaskCreate(watchdog_task, "watchdogTASK", 96, &wdata, 10, NULL);     // Watchdog task
  xTaskCreate(bmp_task, "bmpTASK", 160, &bdata, 6, NULL);               // Refresh bmp-data
  xTaskCreate(page_task, "pagehandlerTASK", 96, NULL, 1, NULL);         // Task-Handler for button
  xTaskCreate(gyro_task, "gyroTASK", 160, &gyro_data, 6, NULL);         // Refresh gyro-data
  xTaskCreate(gatekeeper_task, "gatekeeperTASK", 360, &gdata, 2, NULL); // Display gatekeeper

  xTaskCreate(bmp_print_task, "bmpPrintTask1", 224, &bmp_p_data1, 1, NULL);
  xTaskCreate(bmp_print_task, "bmpPrintTask2", 224, &bmp_p_data2, 1, NULL);
  xTaskCreate(bmp_print_task, "bmpPrintTask3", 224, &bmp_p_data3, 1, NULL);

  xTaskCreate(gyro_print_task, "gyroPrintTask1", 224, &gyro_p_data1, 1, NULL);
  xTaskCreate(gyro_print_task, "gyroPrintTask2", 224, &gyro_p_data2, 1, NULL);
  xTaskCreate(gyro_print_task, "gyroPrintTask3", 224, &gyro_p_data3, 1, NULL);

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  i2c_mutex_init();
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  gatekeeper_init_queue();
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Initialize leds */
  BSP_LED_Init(LED2);

  /* Initialize USER push-button, will be used to trigger an interrupt each time it's pressed.*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin | USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
#ifdef USE_FULL_ASSERT
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
