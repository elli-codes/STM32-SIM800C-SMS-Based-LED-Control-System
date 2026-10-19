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

  /* Private includes ----------------------------------------------------------*/
  /* USER CODE BEGIN Includes */
  #include <string.h>
  #include <stdio.h>
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
  UART_HandleTypeDef huart1;

  /* USER CODE BEGIN PV */
  char data[200];
  int flag,flag_cmgf,flag_cmti,flag_ready,sms_sent;
  char *s;
  int p;
  char atCmd[50]; 
  char *m;
 
  char original_data[32];
  char number[14];
  char original_num[14]="+989226490826";
  char reg[50]; 
  /* USER CODE END PV */

  /* Private function prototypes -----------------------------------------------*/
  void SystemClock_Config(void);
  static void MX_GPIO_Init(void);
  static void MX_USART1_UART_Init(void);
  /* USER CODE BEGIN PFP */

  /* USER CODE END PFP */

  /* Private user code ---------------------------------------------------------*/
  /* USER CODE BEGIN 0 */
  void sendcmgd(void){
   HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CMGD=1,4\r\n", 13, 100);
    
  }

  void sendat(void){
   HAL_UART_Transmit(&huart1, (uint8_t *)"AT\r\n", 4, 100);
   
  }
  void sendcmgf(void){
   HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CMGF=1\r\n", 11, 100);
   
  }
  void sendcnmi(void){
   HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CNMI=2,1,0,0,0\r\n", 19, 100);
   
  }
  void sim_init(void){
        sendcmgd();
        
        sendat();
        memset(data, 0, sizeof(data)); 
        HAL_UART_Receive(&huart1, (uint8_t *)data, sizeof(data), 100); 
        if (strstr(data, "OK") != 0) { 
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
            flag=1;
        }
        
        
        
        if (flag==1){
          flag=0;
          sendcmgf();
          memset(data, 0, sizeof(data));
          HAL_UART_Receive(&huart1, (uint8_t *)data, sizeof(data), 100); 
        if (strstr(data, "OK") != 0) 
        { 
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
            flag_cmgf=1;
          }
        }
        
        
         if (flag_cmgf==1){
          flag_cmgf=0;
           sendcnmi();
          memset(data, 0, sizeof(data));
          HAL_UART_Receive(&huart1, (uint8_t *)data, sizeof(data), 100); 
        if (strstr(data, "OK") != 0) { 
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
            flag_cmti=1;
            flag_ready=1;
        }
        
         }
      }
  void send_ready_sms(void)
  {
      
      const char *cmgf = "AT+CMGF=1\r\n";
      HAL_UART_Transmit(&huart1, (uint8_t *)cmgf, strlen(cmgf), 100);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);//OK
      
      memset(data, 0, sizeof(data));
     

      HAL_UART_Receive(&huart1, (uint8_t *)data, sizeof(data) - 1, 100);
      
      const char *cmgs = "AT+CMGS=\"+989226490826\"\r\n";
      HAL_UART_Transmit(&huart1, (uint8_t *)cmgs, strlen(cmgs), 100);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
     
      memset(data, 0, sizeof(data));


      HAL_UART_Receive(&huart1, (uint8_t *)data, sizeof(data) - 1, 100);
     
       if (strchr(data, '>') == NULL)
         {
           send_ready_sms(); 
         }
         
      
      const char *msg = "READY";
      HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);

      char ctrlZ = 0x1A;
      HAL_UART_Transmit(&huart1, (uint8_t *)&ctrlZ, 1, 100);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
      
      
     
     memset(data, 0, sizeof(data));
     HAL_UART_Receive(&huart1, (uint8_t *)data, sizeof(data) - 1, 100);
     if (strstr(data, "OK") != NULL)
      {
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
      
      } 
      sms_sent = 1;
     }
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
    MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */
    
   
    
    
    
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
      if (flag_ready == 0)
      {
        sim_init();
      }
      else
      {
        if (sms_sent == 0)
        {
          send_ready_sms();
        }

        if (sms_sent == 1)
        {
          memset(data, 0, sizeof(data));
          HAL_UART_Receive(&huart1, (uint8_t *)data, sizeof(data) - 1, 50);

          if (strstr(data, "+CMTI:") != NULL)
          {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
            HAL_Delay(10);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

            s = strstr(data, "+CMTI:");   // +CMTI: "SM",1

            int i = 12;                   // after comma

            if (s[i] >= '0' && s[i] <= '9')
            {
              p = (s[i] - '0');
            }
            if (s[i+1] >= '0' && s[i+1] <= '9')
            {
              p = ((s[i] - '0') * 10) + (s[i+1] - '0');
            }

            number[0] = '\0';
            original_data[0] = '\0';
            sprintf(atCmd, "AT+CMGR=%i\r\n", p);
            HAL_UART_Transmit(&huart1, (uint8_t *)atCmd, strlen(atCmd), 50);
            memset(data, 0, sizeof(data));
            HAL_UART_Receive(&huart1, (uint8_t *)data, sizeof(data) - 1, 300);

            if (strstr(data, "+CMGR:") != NULL)
            {
              HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
              HAL_Delay(10);
              HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);

              m = strstr(data, "+CMGR:");
              char *p, *p1, *start, *end;

              p     = strchr(m, '"');
              p1    = strchr(p + 1, '"');
              start = strchr(p1 + 1, '"');
              end   = strchr(start + 1, '"');

              strncpy(number, start + 1, end - start - 1);
              number[end - start - 1] = '\0';

              int n = 0;
              while (n < strlen(m) - 1)
              {
                if (m[n] == '\r' && m[n+1] == '\n')
                {
                  strncpy(original_data, &m[n+2], sizeof(original_data) - 1);
                  original_data[sizeof(original_data) - 1] = '\0';
                  break;
                }
                else
                {
                  n = n + 1;
                }
              }
            }

            char *ok_pos = strstr(original_data, "\r\nOK\r\n");
            if (ok_pos != NULL)
            {
              *ok_pos = '\0';
            }

            if (number[0]==original_num[0]   && number[1]==original_num[1]   &&
                number[2]==original_num[2]   && number[3]==original_num[3]   &&
                number[4]==original_num[4]   && number[5]==original_num[5]   &&
                number[6]==original_num[6]   && number[7]==original_num[7]   &&
                number[8]==original_num[8]   && number[9]==original_num[9]   &&
                number[10]==original_num[10] && number[11]==original_num[11] &&
                number[12]==original_num[12])
            {
              if (strstr(original_data, "on"))
              {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
              }
              if (strstr(original_data, "off"))
              {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
              }
            }
            else if (number[0] != '\0')
            {
              int t = 0;

              sprintf(reg, "AT+CMGS=\"%s\"\r\n", number);
              HAL_UART_Transmit(&huart1, (uint8_t *)reg, strlen(reg), 100);
              memset(data, 0, sizeof(data));
              HAL_UART_Receive(&huart1, (uint8_t *)data, sizeof(data), 500);

              while (strchr(data, '>') == NULL && t++ < 5)
              {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);

                HAL_UART_Transmit(&huart1, (uint8_t *)"\x1B", 1, 100);   // ESC
                HAL_Delay(1000);

                HAL_UART_Transmit(&huart1, (uint8_t *)reg, strlen(reg), 100);
                memset(data, 0, sizeof(data));
                HAL_UART_Receive(&huart1, (uint8_t *)data, sizeof(data), 500);
              }

              if (strchr(data, '>') != NULL)
              {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);

                const char *msg = "SORRY ,YOU ARE NOT REGISTERED!";
                HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
                HAL_UART_Transmit(&huart1, (uint8_t *)"\x1A", 1, 100);  // Ctrl+Z

                memset(data, 0, sizeof(data));
                HAL_UART_Receive(&huart1, (uint8_t *)data, sizeof(data), 5000);
              }
              else
              {
                HAL_UART_Transmit(&huart1, (uint8_t *)"\x1B", 1, 100);  // ???? PB14 ???? ???????
              }
            }   //  else if
          }     //  if (+CMTI)
        }       // if (sms_sent == 1)
      }         //  else (flag_ready)

      /* USER CODE END WHILE */

      /* USER CODE BEGIN 3 */
    }           //  while(1)
    /* USER CODE END 3 */
  }             //  main

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
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
      Error_Handler();
    }
  }

  /**
    * @brief USART1 Initialization Function
    * @param None
    * @retval None
    */
  static void MX_USART1_UART_Init(void)
  {

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 9600;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
      Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */

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
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                            |GPIO_PIN_9, GPIO_PIN_RESET);

    /*Configure GPIO pins : PB12 PB13 PB14 PB15
                             PB9 */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                            |GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USER CODE BEGIN MX_GPIO_Init_2 */

    /* USER CODE END MX_GPIO_Init_2 */
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
