/********** (C) COPYRIGHT 2021 Fremont Micro Devices (SZ) Corporation *********
* @file      : main.c
* @author    : FMD AE
* @brief     : main program body
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
#include "main.h"

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_ft32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_ft32f0xx.c file
     */

    /* Initialize LEDs mounted on Starter board */
    LedConfig(LED1);
    LedConfig(LED2);

    /* Turn on LED1 and LED3 */
    LedOn(LED1);
    LedOn(LED2);

    /* Initialize SysTick Timer*/
    SystickInit();

    while (1)
    {
        /* Toggle LED2 and LED4 */
        LedToggle(LED1);
        LedToggle(LED2);

        /* Insert 50 ms delay */
        Delay(50);

        /* Toggle LED1 and LED3 */
        LedToggle(LED1);
        LedToggle(LED2);

        /* Insert 100 ms delay */
        Delay(100);
    }
}

/************************ (C) COPYRIGHT Fremont Micro Devices *****END OF FILE****/
