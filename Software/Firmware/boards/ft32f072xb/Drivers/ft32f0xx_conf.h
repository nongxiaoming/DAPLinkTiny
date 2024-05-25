/**
  ******************************************************************************
  * @file    ft32f0xx_conf.h
  * @author  FMD-AE
  * @version V1.0.0
  * @date    2021-8-2
  * @brief   Library configuration file.
  ******************************************************************************
*/

#ifndef __FT32F030XX_CONF_H
#define __FT32F030XX_CONF_H

#ifdef _RTE_
#include "RTE_Components.h"             /* Component selection */
#endif

#ifdef __cplusplus
 extern "C" {
#endif

#include "ft32f0xx_adc.h"

#include "ft32f0xx_comp.h"

#include "ft32f0xx_crc.h"

#include "ft32f0xx_crs.h"

#include "ft32f0xx_dac.h"

#include "ft32f0xx_dma.h"

#include "ft32f0xx_debug.h"

#include "ft32f0xx_exti.h"

#include "ft32f0xx_flash.h"

#include "ft32f0xx_gpio.h"

#include "ft32f0xx_i2c.h"

#include "ft32f0xx_iwdg.h"

#include "ft32f0xx_misc.h"

#include "ft32f0xx_opa.h"

#include "ft32f0xx_pwr.h"

#include "ft32f0xx_rcc.h"

#include "ft32f0xx_rtc.h"

#include "ft32f0xx_spi.h"

#include "ft32f0xx_syscfg.h"

#include "ft32f0xx_tim.h"

#include "ft32f0xx_usart.h"

#include "ft32f0xx_wwdg.h"



#include "stdio.h"

#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports
  *         the name of the source file and the source line number of the call
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __FT32F030X8_CONF_H */

/************************ (C) COPYRIGHT FMD *****END OF FILE****/
