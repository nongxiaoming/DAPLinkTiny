/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "at32f4xx.h" 
#include "gpio.h"

#define LED0 0
#define LED1 1
#define LED0_PIN 0
#define LED1_PIN 1

#define led_hw_on(led)  //GPIOE->BSRRH =0x01<<(LED0_PIN+led)
#define led_hw_off(led) //GPIOE->BSRRL =0x01<<(LED0_PIN+led)

void gpio_init(void) {
  /* Enable clock for  GPIOE */
//  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN  ;

///* Pins in Push-pull output mode, 50 MHz Fast Speed with Pull-up resistors  */
//  GPIOE->MODER   &= ~(0x0fUL << 2*LED0_PIN);
//  GPIOE->MODER   |=  ((0x01UL << 2*LED0_PIN) | (0x01UL << 2*LED1_PIN));
//  GPIOE->OTYPER  &= ~(3UL <<   LED0_PIN) ;
//  GPIOE->OSPEEDR &= ~(0x0fUL << 2*LED0_PIN);
//  GPIOE->OSPEEDR |=  ((0x02UL << 2*LED0_PIN) | (0x02UL << 2*LED1_PIN));
//  GPIOE->PUPDR   &= ~(0x0fUL << 2*LED0_PIN);
//  GPIOE->PUPDR   |=  ((0x01UL << 2*LED0_PIN) | (0x01UL << 2*LED1_PIN));  
//  led_hw_off(LED0); // LED off
//  led_hw_off(LED1); // LED off	
//	/* configure the PE14 as input */
//	GPIOE->MODER &= ~(3UL << 2*14);
//	GPIOE->MODER   |=  (0x01UL << 2*14);
//	GPIOE->OTYPER  &= ~(1UL <<   14) ;
//  GPIOE->OSPEEDR &= ~(0x03UL << 2*14);
//  GPIOE->OSPEEDR |=  (0x02UL << 2*14);
//  GPIOE->PUPDR   &= ~(0x03UL << 2*14);
//	GPIOE->PUPDR   |= ~(0x01UL << 2*14);
//  GPIOE->ODR  |=  (0x01UL << 14);
}

void gpio_set_dap_led(uint8_t state) {
    if (state) {
        led_hw_on(LED0); // LED on
    } else {
       led_hw_off(LED0); // LED off
    }
}

void gpio_set_msd_led(uint8_t state) {
     if (state) {
        led_hw_on(LED1); // LED on
    } else {
       led_hw_off(LED1); // LED off
    }
}

void gpio_set_cdc_led(uint8_t state) {
     if (state) {
        led_hw_on(LED1); // LED on
    } else {
       led_hw_off(LED1); // LED off
    }
}
uint16_t gpio_get_pin_loader_state(void) {
    return 0;//( GPIOE->IDR & (1UL << 14));
}


