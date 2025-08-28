/********** (C) COPYRIGHT 2021 Fremont Micro Devices (SZ) Corporation *********
* @file      : bsp_led.c
* @author    : FMD AE
* @brief     : bsp_led program body
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

/* Includes ------------------------------------------------------------------*/
#include "drv_led.h"

const pin_config_t  PIN_CONFIG[LED_NUM] =
{
    { LED1_CLK, LED1_GPIOX, LED1_GPIO_Pin },
    { LED2_CLK, LED2_GPIOX, LED2_GPIO_Pin },
};

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured.
  *          This parameter can be one of following parameters:
  *            @arg LED1
  *            @arg LED2
  * @retval None
  */
void LedConfig(Led_TypeDef Led)
{
    /* Enable the GPIO_LED Clock */
    RCC_AHBPeriphClockCmd(PIN_CONFIG[Led].clk, ENABLE);

    GPIO_InitTypeDef    GPIO_InitStructure;

    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Pin = PIN_CONFIG[Led].pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(PIN_CONFIG[Led].port, &GPIO_InitStructure);

    /* Turn off GPIO_LED*/
    LedOff(Led);

}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on.
  *          This parameter can be one of following parameters:
  *            @arg LED1
  *            @arg LED2
  * @retval None
  */
void LedOn(Led_TypeDef Led)
{
   GPIO_WriteBit(PIN_CONFIG[Led].port, PIN_CONFIG[Led].pin, Bit_RESET);
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off.
  *          This parameter can be one of following parameters:
  *            @arg LED1
  *            @arg LED2
  * @retval None
  */
void LedOff(Led_TypeDef Led)
{
   GPIO_WriteBit(PIN_CONFIG[Led].port, PIN_CONFIG[Led].pin, Bit_SET);
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled.
  *          This parameter can be one of following parameters:
  *            @arg LED1
  *            @arg LED2
  * @retval None
  */
void LedToggle(Led_TypeDef Led)
{
    if (GPIO_ReadOutputDataBit(PIN_CONFIG[Led].port, PIN_CONFIG[Led].pin) == Bit_RESET)
    {
        GPIO_WriteBit(PIN_CONFIG[Led].port, PIN_CONFIG[Led].pin, Bit_SET);
    }
    else
    {
        GPIO_WriteBit(PIN_CONFIG[Led].port, PIN_CONFIG[Led].pin, Bit_RESET);
    }
}

