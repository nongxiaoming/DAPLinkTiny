/**
 * @file    gpio.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "at32f4xx.h"
#include "DAP_config.h"
#include "gpio.h"
#include "daplink.h"
#include "util.h"

//static TIM_HandleTypeDef timer;

static void busy_wait(uint32_t cycles)
{
    volatile uint32_t i;
    i = cycles;

    while (i > 0) {
        i--;
    }
}

static uint32_t tim1_clk_div(uint32_t apb2clkdiv)
{
    switch (apb2clkdiv) {
        case RCC_CFGR_PPRE2_DIV2:
            return 1;
        case RCC_CFGR_PPRE2_DIV4:
            return 2;
        case RCC_CFGR_PPRE2_DIV8:
            return 4;
        default: // RCC_CFGR_PPRE2_DIV1
            return 1;
    }
}

static void output_clock_enable(void)
{
    HAL_StatusTypeDef ret;
    RCC_ClkInitTypeDef clk_init;
    TIM_OC_InitTypeDef pwm_config;
    uint32_t unused;
    uint32_t period;
    uint32_t source_clock;

    HAL_RCC_GetClockConfig(&clk_init, &unused);

    /* Compute the period value to have TIMx counter clock equal to 8000000 Hz */
    source_clock = SystemCoreClock / tim1_clk_div(clk_init.APB2CLKDivider);
    period = (uint32_t)(source_clock / 8000000) - 1;

    /* Set TIMx instance */
    timer.Instance = TIM1;

    timer.Init.Period            = period;
    timer.Init.Prescaler         = 0;
    timer.Init.ClockDivision     = 0;
    timer.Init.CounterMode       = TIM_COUNTERMODE_UP;
    timer.Init.RepetitionCounter = 0;//period / 2;

    __HAL_RCC_TIM1_CLK_ENABLE();

    ret = HAL_TIM_PWM_DeInit(&timer);
    if (ret != HAL_OK) {
        util_assert(0);
        return;
    }

    ret = HAL_TIM_PWM_Init(&timer);
    if (ret != HAL_OK) {
        util_assert(0);
        return;
    }

    pwm_config.OCMode = TIM_OCMODE_PWM2;
    pwm_config.Pulse = 0; // TODO - make sure this isn't used
    pwm_config.OCPolarity = TIM_OCPOLARITY_HIGH;
    pwm_config.OCNPolarity = TIM_OCPOLARITY_HIGH;
    pwm_config.OCFastMode = TIM_OCFAST_DISABLE;
    pwm_config.OCIdleState = TIM_OCIDLESTATE_RESET;
    pwm_config.OCNIdleState = TIM_OCIDLESTATE_RESET;
    ret = HAL_TIM_PWM_ConfigChannel(&timer, &pwm_config, TIM_CHANNEL_1);
    if (ret != HAL_OK) {
        util_assert(0);
        return;
    }

    __HAL_TIM_SET_COMPARE(&timer, TIM_CHANNEL_1, period / 2);
    ret = HAL_TIM_PWM_Start(&timer, TIM_CHANNEL_1);
    if (ret != HAL_OK) {
        util_assert(0);
        return;
    }

    return;
}

void gpio_init(void)
{
    GPIO_InitType GPIO_InitStructure;
    // enable clock to ports
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOD, ENABLE);
    // Enable USB connect pin
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO, ENABLE);
    // Disable JTAG to free pins for other uses
    // Note - SWD is still enabled
     GPIO_PinsRemapConfig(AFIO_MAP7_SWJTAG_0010,ENABLE);

    USB_CONNECT_PORT_ENABLE();
    USB_CONNECT_OFF();
    GPIO_InitStructure.Pin = USB_CONNECT_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(USB_CONNECT_PORT, &GPIO_InitStructure);
    // configure LEDs
    HAL_GPIO_WritePin(RUNNING_LED_PORT, RUNNING_LED_PIN, GPIO_PIN_SET);
    GPIO_InitStructure.Pin = RUNNING_LED_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(RUNNING_LED_PORT, &GPIO_InitStructure);

    HAL_GPIO_WritePin(CONNECTED_LED_PORT, CONNECTED_LED_PIN, GPIO_PIN_SET);
    GPIO_InitStructure.Pin = CONNECTED_LED_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(CONNECTED_LED_PORT, &GPIO_InitStructure);

    HAL_GPIO_WritePin(PIN_CDC_LED_PORT, PIN_CDC_LED, GPIO_PIN_SET);
    GPIO_InitStructure.Pin = PIN_CDC_LED;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(PIN_CDC_LED_PORT, &GPIO_InitStructure);

    HAL_GPIO_WritePin(PIN_MSC_LED_PORT, PIN_MSC_LED, GPIO_PIN_SET);
    GPIO_InitStructure.Pin = PIN_MSC_LED;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(PIN_MSC_LED_PORT, &GPIO_InitStructure);

    // reset button configured as gpio open drain output with a pullup
    HAL_GPIO_WritePin(nRESET_PIN_PORT, nRESET_PIN, GPIO_PIN_SET);
    GPIO_InitStructure.Pin = nRESET_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(nRESET_PIN_PORT, &GPIO_InitStructure);

    // Turn on power to the board. When the target is unpowered
    // it holds the reset line low.
    HAL_GPIO_WritePin(POWER_EN_PIN_PORT, POWER_EN_PIN, GPIO_PIN_RESET);
    GPIO_InitStructure.Pin = POWER_EN_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(POWER_EN_PIN_PORT, &GPIO_InitStructure);

    // Setup the 8MHz MCO
    GPIO_InitStructure.Pin = GPIO_PIN_8;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
    output_clock_enable();

    //PA13 Output
		GPIO_InitStructure.Pin = GPIO_PIN_13;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		//PA14 Output
		GPIO_InitStructure.Pin = GPIO_PIN_14;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
		
    // Let the voltage rails stabilize.  This is especailly important
    // during software resets, since the target's 3.3v rail can take
    // 20-50ms to drain.  During this time the target could be driving
    // the reset pin low, causing the bootloader to think the reset
    // button is pressed.
    // Note: With optimization set to -O2 the value 1000000 delays for ~85ms
    busy_wait(1000000);
}

void gpio_set_hid_led(gpio_led_state_t state)
{
    // LED is active low
    GPIO_WriteBit(PIN_HID_LED_PORT, PIN_HID_LED, state ? Bit_RESET : Bit_SET);
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
    // LED is active low
    GPIO_WriteBit(PIN_CDC_LED_PORT, PIN_CDC_LED, state ? Bit_RESET : Bit_SET);
}

void gpio_set_msc_led(gpio_led_state_t state)
{
    // LED is active low
    GPIO_WriteBit(PIN_MSC_LED_PORT, PIN_MSC_LED, state ? Bit_RESET : Bit_SET);
}

uint8_t gpio_get_reset_btn_no_fwrd(void)
{
//	  uint8_t i = 20;
//	  uint32_t n = 0;
//	  GPIO_PinState state = GPIO_PIN_RESET;
//	  while(i--)
//		{
//			state = (state==GPIO_PIN_RESET)?GPIO_PIN_SET:GPIO_PIN_RESET;
//			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_13,state);
//			for(n = 0; n < 5000;n++)
//       {
//			 
//			 }
//       if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_14)!=state)
//			 {
		   return 0;
//			 }
//		}
//    return 1;
}

uint8_t gpio_get_reset_btn_fwrd(void)
{
    return 0;
}


uint8_t GPIOGetButtonState(void)
{
    return 0;
}

void target_forward_reset(bool assert_reset)
{
    // Do nothing - reset is forwarded in gpio_get_sw_reset
}

void gpio_set_board_power(bool powerEnabled)
{
}
