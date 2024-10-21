/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RE_SW_Pin GPIO_PIN_13
#define RE_SW_GPIO_Port GPIOC
#define RE_CLK_Pin GPIO_PIN_14
#define RE_CLK_GPIO_Port GPIOC
#define RE_DT_Pin GPIO_PIN_15
#define RE_DT_GPIO_Port GPIOC
#define BTN_PRG1_Pin GPIO_PIN_0
#define BTN_PRG1_GPIO_Port GPIOA
#define BTN_STOP_Pin GPIO_PIN_1
#define BTN_STOP_GPIO_Port GPIOA
#define BTN_PRG3_Pin GPIO_PIN_2
#define BTN_PRG3_GPIO_Port GPIOA
#define BTN_PRG2_Pin GPIO_PIN_3
#define BTN_PRG2_GPIO_Port GPIOA
#define BTN_EDIT_Pin GPIO_PIN_4
#define BTN_EDIT_GPIO_Port GPIOA
#define LED_PRG3_Pin GPIO_PIN_5
#define LED_PRG3_GPIO_Port GPIOA
#define LED_PRG2_Pin GPIO_PIN_6
#define LED_PRG2_GPIO_Port GPIOA
#define LED_PRG1_Pin GPIO_PIN_7
#define LED_PRG1_GPIO_Port GPIOA
#define LED_EDIT_Pin GPIO_PIN_0
#define LED_EDIT_GPIO_Port GPIOB
#define NRF_IRQ_Pin GPIO_PIN_1
#define NRF_IRQ_GPIO_Port GPIOB
#define NRF_IRQ_EXTI_IRQn EXTI1_IRQn
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define NRF_CSN_Pin GPIO_PIN_12
#define NRF_CSN_GPIO_Port GPIOB
#define NRF_CE_Pin GPIO_PIN_8
#define NRF_CE_GPIO_Port GPIOA
#define DBG_TX_Pin GPIO_PIN_9
#define DBG_TX_GPIO_Port GPIOA
#define DBG_RX_Pin GPIO_PIN_10
#define DBG_RX_GPIO_Port GPIOA
#define SIG1_Pin GPIO_PIN_15
#define SIG1_GPIO_Port GPIOA
#define SIG2_Pin GPIO_PIN_3
#define SIG2_GPIO_Port GPIOB
#define SIG3_Pin GPIO_PIN_4
#define SIG3_GPIO_Port GPIOB
#define SIG4_Pin GPIO_PIN_5
#define SIG4_GPIO_Port GPIOB
#define FL1_Pin GPIO_PIN_6
#define FL1_GPIO_Port GPIOB
#define FL1_EXTI_IRQn EXTI9_5_IRQn
#define FL2_Pin GPIO_PIN_7
#define FL2_GPIO_Port GPIOB
#define FL2_EXTI_IRQn EXTI9_5_IRQn
#define PS_Pin GPIO_PIN_8
#define PS_GPIO_Port GPIOB
#define PS_EXTI_IRQn EXTI9_5_IRQn
#define OIL_Pin GPIO_PIN_9
#define OIL_GPIO_Port GPIOB
#define OIL_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
