#ifndef __DRV_LED_H
#define __DRV_LED_H

#include "at32f4xx.h"


#define LED1_RCC       RCC_APB2PERIPH_GPIOA
#define LED1_PORT      GPIOA
#define LED1_PIN       GPIO_Pins_0

#define LED2_RCC       RCC_APB2PERIPH_GPIOA
#define LED2_PORT      GPIOA
#define LED2_PIN       GPIO_Pins_1

#define LED3_RCC       RCC_APB2PERIPH_GPIOA
#define LED3_PORT      GPIOA
#define LED3_PIN       GPIO_Pins_2

#define LED_RED       0x01
#define LED_BLUE      0x02
#define LED_ALL       0x03

void LED_Init(void);
void LED_On(uint8_t leds);
void LED_Off(uint8_t leds);
void LED_Toggle(uint8_t leds);

#endif
