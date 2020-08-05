/*
 * File      : stm32_usbd.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2015, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-01-06     xiaonong         the first version for stm32h7
 */
#ifndef __STM32_USBD_H__
#define __STM32_USBD_H__
#include <rtthread.h>

#ifdef BSP_USBD_TYPE_HS
#define USBD_IRQ_TYPE     OTG_HS_IRQn
#define USBD_IRQ_HANDLER  OTG_HS_IRQHandler
#define USBD_INSTANCE     USB_OTG_HS
#else
#define USBD_IRQ_TYPE     OTG_FS_IRQn
#define USBD_IRQ_HANDLER  OTG_FS_IRQHandler
#define USBD_INSTANCE     USB_OTG_FS
#endif

#ifdef BSP_USBD_SPEED_HS
#define USBD_PCD_SPEED    PCD_SPEED_HIGH
#elif  BSP_USBD_SPEED_HSINFS
#define USBD_PCD_SPEED    PCD_SPEED_HIGH_IN_FULL
#else
#define USBD_PCD_SPEED    PCD_SPEED_FULL
#endif

#ifdef BSP_USBD_PHY_ULPI
#define USBD_PCD_PHY_MODULE    PCD_PHY_ULPI
#elif  BSP_USBD_PHY_UTMI
#define USBD_PCD_PHY_MODULE    PCD_PHY_UTMI
#else
#define USBD_PCD_PHY_MODULE    PCD_PHY_EMBEDDED
#endif

int stm_usbd_register(void);

#endif
