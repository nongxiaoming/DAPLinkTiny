/********** (C) COPYRIGHT 2021 Fremont Micro Devices (SZ) Corporation *********
* @file      : bsp_led.h
* @author    : FMD AE
* @brief     : Header for bsp_led.c module
* @version   : V1.0.0
* @data      : 2021-09-14
******************************************************************************
* @attention
* COPYRIGHT (C) 2021 Fremont Micro Devices (SZ) Corporation All rights reserved.
* This software is provided by the copyright holders and contributors,and the
*   software is believed to be accurate and reliable. However, Fremont Micro
*   Devices (SZ) Corporation assumes no responsibility for the consequences of
*   use of such software or for any infringement of patents of other rights
*   of third parties, which may result from its use. No license is granted by
*   implication or otherwise under any patent rights of Fremont Micro Devices (SZ)
*   Corporation.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_LED_H
#define __BSP_LED_H

/* Includes ------------------------------------------------------------------*/
#include "ft32f0xx.h"


/* Exported types ------------------------------------------------------------*/
typedef struct
{
    uint32_t clk;
    GPIO_TypeDef   *port;
    uint16_t    pin;
} pin_config_t;

typedef enum
{
    LED1 = 0,
    LED2 = 1,
} Led_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* ------------------------------- Configuration ---------------------------- */
#define LED_NUM                         2U

/* Exported macro ------------------------------------------------------------*/
#define LED1_CLK        RCC_AHBENR_GPIOBEN
#define LED2_CLK        RCC_AHBENR_GPIOBEN

#define LED1_GPIOX  GPIOB
#define LED2_GPIOX  GPIOB

#define LED1_GPIO_Pin   GPIO_Pin_3
#define LED2_GPIO_Pin   GPIO_Pin_4

/* Exported functions ------------------------------------------------------- */
void LedConfig(Led_TypeDef Led);
void LedOn(Led_TypeDef Led);
void LedOff(Led_TypeDef Led);
void LedToggle(Led_TypeDef Led);

#endif  /* __BSP_LED_H */

/************************ (C) COPYRIGHT Fremont Micro Devices *****END OF FILE****/




