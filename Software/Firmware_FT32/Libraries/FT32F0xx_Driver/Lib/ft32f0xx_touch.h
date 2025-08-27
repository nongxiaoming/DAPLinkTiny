/**
  *********************************************************************************
  * @file    	    ft32f0xx_touch.h
  * @author  	    FMD AE
  * @brief   		FT32f0xx_touch Header File. 	
  * @version 	    V1.0.1           
  * @data		    2021-11-4
  *********************************************************************************
  * @attention
  * COPYRIGHT (C) 2021 Fremont Micro Devices (SZ) Corporation All rights reserved.
  *    This software is provided by the copyright holders and contributors,and the
  *software is believed to be accurate and reliable. However, Fremont Micro Devices
  *(SZ) Corporation assumes no responsibility for the consequences of use of such
  *software or for any infringement of patents of other rights of third parties,
  *which may result from its use. No license is granted by implication or otherwise
  *under any patent rights of Fremont Micro Devices (SZ) Corporation.
  *  ******************************************************************************
  */

#ifndef     __FT32F0XX_TOUCH_H
#define     __FT32F0XX_TOUCH_H

/* Includes ---------------------------------------------------------------------*/
#include  "ft32f0xx.h"
#include  "ft32f072x8.h"

/* Public Constant --------------------------------------------------------------*/

/* Public typedef ---------------------------------------------------------------*/
typedef struct
{
	uint8_t    MeantimeMaxKeyNum;					//最大允许同时按下的按键个数
	uint8_t    Multiple;							//按键的放大倍数
    uint8_t    SingleFilter;					    //单键滤波次数
    uint8_t    StrongFilter;					    //最强键滤波次数
	uint8_t    BaselineUpSpeed;						//基线向上更新速度
	uint8_t    BaselineDownSpeed;					//基线向下更新速度
	uint8_t    BaselineEqualSpeed;				    //基线快速更新速度
	uint8_t    SignalNoiseRatio;					//输入信噪比
	uint8_t    BaselineEqualCondition;				//基线快速更新条件
}TSC_ParaDef;

/* Public define ----------------------------------------------------------------*/

/* Public variables -------------------------------------------------------------*/
extern uint8_t ucFrequNum;

/* Public function prototypes----------------------------------------------------*/
void TSC_EnableKey(uint8_t KeyNum,uint8_t KeySensitivity);
void TSC_Start(void);
void TSC_DataProcessing(void);
uint8_t  TSC_GetStrongest(void);
uint32_t TSC_GetSingle(void);
void TSC_Reset(void);
uint8_t TSC_GetDelta(uint8_t KeyNum);
void Set_Baseline(uint8_t KeyNum,uint16_t BaselineData);
uint16_t TSC_GetPrevData(uint8_t KeyNum);
void TSC_Init(TSC_ParaDef *TSC_ParaStruct);
void Set_KeyFilter(uint8_t SingleFilter,uint8_t StrongFilter);
void Set_BaselineSpeed(uint8_t BaselineUpSpeed,uint8_t BaselineDownSpeed,uint8_t BaselineEqualSpeed);
void Set_SignalNoiseRatio(uint8_t iSignalNoiseRatio);
void Set_Frequ(uint8_t Frequ1,uint8_t Frequ2,uint8_t Frequ3);
uint8_t Get_Frequ(uint8_t FrequNum);
#endif

/************************* (C) COPYRIGHT FMD *****END OF FILE**********************/
