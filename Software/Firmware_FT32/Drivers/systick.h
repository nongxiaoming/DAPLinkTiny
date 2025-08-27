/********** (C) COPYRIGHT 2021 Fremont Micro Devices (SZ) Corporation *********
* @file      : bsp_systick.h
* @author    : FMD AE
* @brief     : Header for bsp_systick.c module
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
#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H

/* Includes ------------------------------------------------------------------*/
#include "ft32f0xx.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SystickInit(void);
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

#endif  /* __BSP_SYSTICK_H */

/************************ (C) COPYRIGHT Fremont Micro Devices *****END OF FILE****/




