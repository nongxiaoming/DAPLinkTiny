/********** (C) COPYRIGHT 2021 Fremont Micro Devices (SZ) Corporation *********
* @file      : bsp_systick.c
* @author    : FMD AE
* @brief     : bsp_systick program body
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
#include "bsp_systick.h"

/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void SystickInit(void)
{

    /* Gets the value of SystemCoreClock */
    SystemCoreClockUpdate();

    /* Setup SysTick Timer for 1 msec interrupts.
       ------------------------------------------
      1. The SysTick_Config() function is a CMSIS function which configure:
         - The SysTick Reload register with value passed as function parameter.
         - Configure the SysTick IRQ priority to the lowest value (0x0F).
         - Reset the SysTick Counter register.
         - Configure the SysTick Counter clock source to be Core Clock Source (HCLK).
         - Enable the SysTick Interrupt.
         - Start the SysTick Counter.

      2. You can change the SysTick Clock source to be HCLK_Div8 by calling the
         SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8) just after the
         SysTick_Config() function call. The SysTick_CLKSourceConfig() is defined
         inside the misc.c file.

      3. You can change the SysTick IRQ priority by calling the
         NVIC_SetPriority(SysTick_IRQn,...) just after the SysTick_Config() function
         call. The NVIC_SetPriority() is defined inside the core_cm0.h file.

      4. To adjust the SysTick time base, use the following formula:

           Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)

         - Reload Value is the parameter to be passed for SysTick_Config() function
         - Reload Value should not exceed 0xFFFFFF
     */

    if (SysTick_Config(SystemCoreClock / 1000))
    {
        /* Capture error */
        while (1);
    }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
    TimingDelay = nTime;

    while (TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}
