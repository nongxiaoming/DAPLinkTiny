/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-31     shelton      first version
 */

#include <rtthread.h>
#include <board.h>
#include <DAP_config.h>

int main(void)
{
    gpio_init_type gpio_init_struct;
    
    crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
    
    gpio_default_para_init(&gpio_init_struct);

    gpio_init_struct.gpio_pins = SWCLK_PIN;
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
    gpio_init(SWCLK_PORT, &gpio_init_struct);


    gpio_init_struct.gpio_pins = LED_CONNECTED_PIN;
    gpio_init(LED_CONNECTED_PORT, &gpio_init_struct);

    gpio_init_struct.gpio_pins = LED_RUNNING_PIN;
    gpio_init(LED_RUNNING_PORT, &gpio_init_struct);
    
    while(1)
    {
    
        gpio_bits_reset(LED_CONNECTED_PORT, LED_CONNECTED_PIN);
        gpio_bits_set(LED_RUNNING_PORT, LED_RUNNING_PIN);
        rt_thread_mdelay(500);
        
        gpio_bits_set(LED_CONNECTED_PORT, LED_CONNECTED_PIN);
        gpio_bits_reset(LED_RUNNING_PORT, LED_RUNNING_PIN);
        rt_thread_mdelay(500);
    }
    return RT_EOK;
}
