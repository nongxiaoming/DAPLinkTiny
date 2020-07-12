/**
 * @file    uart.c
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

#include "string.h"

#include "at32f4xx.h"
#include "uart.h"
#include "gpio.h"
#include "util.h"
#include "circ_buf.h"
#include "IO_Config.h"

// For usart
#define CDC_UART                     USART1
#define CDC_UART_ENABLE()            RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1, ENABLE)
#define CDC_UART_DISABLE()           RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1, DISABLE)
#define CDC_UART_IRQn                USART1_IRQn
#define CDC_UART_IRQn_Handler        USART1_IRQHandler

#define UART_PINS_PORT_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA, ENABLE)
#define UART_PINS_PORT_DISABLE()     RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA, DISABLE)

#define UART_TX_PORT                 GPIOA
#define UART_TX_PIN                  GPIO_Pins_9

#define UART_RX_PORT                 GPIOA
#define UART_RX_PIN                  GPIO_Pins_10


#define RX_OVRF_MSG         "<DAPLink:Overflow>\n"
#define RX_OVRF_MSG_SIZE    (sizeof(RX_OVRF_MSG) - 1)
#define BUFFER_SIZE         (512)

circ_buf_t write_buffer;
uint8_t write_buffer_data[BUFFER_SIZE];
circ_buf_t read_buffer;
uint8_t read_buffer_data[BUFFER_SIZE];

static UART_Configuration configuration = {
    .Baudrate = 9600,
    .DataBits = UART_DATA_BITS_8,
    .Parity = UART_PARITY_NONE,
    .StopBits = UART_STOP_BITS_1,
    .FlowControl = UART_FLOW_CONTROL_NONE,
};

extern uint32_t SystemCoreClock;



static void clear_buffers(void)
{
    circ_buf_init(&write_buffer, write_buffer_data, sizeof(write_buffer_data));
    circ_buf_init(&read_buffer, read_buffer_data, sizeof(read_buffer_data));
}

int32_t uart_initialize(void)
{
    GPIO_InitType GPIO_InitStructure;

	  USART_INTConfig(CDC_UART, USART_INT_TDE | USART_INT_RDNE, DISABLE);
    clear_buffers();

    CDC_UART_ENABLE();
    UART_PINS_PORT_ENABLE();

  /* Configure the CDC UART TX pin */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pins = UART_TX_PIN; 
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_Init(UART_TX_PORT, &GPIO_InitStructure);
  /* Configure the CDC UART RX pin */
  GPIO_InitStructure.GPIO_Pins = UART_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(UART_RX_PORT, &GPIO_InitStructure);


    NVIC_EnableIRQ(CDC_UART_IRQn);

    return 1;
}

int32_t uart_uninitialize(void)
{
    USART_INTConfig(CDC_UART, USART_INT_TDE | USART_INT_RDNE, DISABLE);
    clear_buffers();
    return 1;
}

int32_t uart_reset(void)
{
    USART_INTConfig(CDC_UART, USART_INT_TDE | USART_INT_RDNE, DISABLE);
    clear_buffers();
    USART_INTConfig(CDC_UART, USART_INT_RDNE, ENABLE);
    return 1;
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    USART_InitType USART_InitStructure;

  /*Configure UART param*/
  USART_StructInit(&USART_InitStructure);
  
    // parity
    configuration.Parity = config->Parity;
    if(config->Parity == UART_PARITY_ODD) {
        USART_InitStructure.USART_Parity = USART_Parity_Odd;
    } else if(config->Parity == UART_PARITY_EVEN) {
        USART_InitStructure.USART_Parity = USART_Parity_Even;
    } else if(config->Parity == UART_PARITY_NONE) {
        USART_InitStructure.USART_Parity = USART_Parity_No;
    } else {   //Other not support
        USART_InitStructure.USART_Parity = USART_Parity_No;
        configuration.Parity = UART_PARITY_NONE;
    }
  
    // stop bits
    configuration.StopBits = config->StopBits;
    if(config->StopBits == UART_STOP_BITS_2) {
        USART_InitStructure.USART_StopBits = USART_StopBits_2;
    } else if(config->StopBits == UART_STOP_BITS_1_5) {
        USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
        configuration.StopBits = UART_STOP_BITS_2;
    } else if(config->StopBits == UART_STOP_BITS_1) {
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
    } else {
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        configuration.StopBits = UART_STOP_BITS_1;
    }

    //Only 8 bit support
    configuration.DataBits = UART_DATA_BITS_8;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;

    // No flow control
    configuration.FlowControl = UART_FLOW_CONTROL_NONE;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    
    // Specified baudrate
    configuration.Baudrate = config->Baudrate;
    USART_InitStructure.USART_BaudRate = config->Baudrate;

    // TX and RX
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
    
    // Disable uart and tx/rx interrupt
    USART_INTConfig(CDC_UART, USART_INT_RDNE | USART_INT_TDE, DISABLE);

    clear_buffers();

   USART_Init(CDC_UART, &USART_InitStructure); 
   USART_INTConfig(CDC_UART, USART_INT_RDNE, ENABLE);
   USART_Cmd(CDC_UART, ENABLE);

    return 1;
}

int32_t uart_get_configuration(UART_Configuration *config)
{
    config->Baudrate = configuration.Baudrate;
    config->DataBits = configuration.DataBits;
    config->Parity   = configuration.Parity;
    config->StopBits = configuration.StopBits;
    config->FlowControl = UART_FLOW_CONTROL_NONE;

    return 1;
}

int32_t uart_write_free(void)
{
    return circ_buf_count_free(&write_buffer);
}

int32_t uart_write_data(uint8_t *data, uint16_t size)
{
    uint32_t cnt = circ_buf_write(&write_buffer, data, size);
    USART_INTConfig(CDC_UART, USART_INT_TDE, ENABLE);

    return cnt;
}

int32_t uart_read_data(uint8_t *data, uint16_t size)
{
    return circ_buf_read(&read_buffer, data, size);
}

void CDC_UART_IRQn_Handler(void)
{
    if(USART_GetITStatus(USART2, USART_INT_RDNE) != RESET)
		{
  			uint8_t dat = USART_ReceiveData(USART2);
        uint32_t free = circ_buf_count_free(&read_buffer);
        if (free > RX_OVRF_MSG_SIZE) {
            circ_buf_push(&read_buffer, dat);
        } else if (RX_OVRF_MSG_SIZE == free) {
            circ_buf_write(&read_buffer, (uint8_t*)RX_OVRF_MSG, RX_OVRF_MSG_SIZE);
        } else {
            // Drop character
        }
    }

    if(USART_GetITStatus(USART2, USART_INT_TDE) != RESET)
		{
        if (circ_buf_count_used(&write_buffer) > 0) {
            USART_SendData(CDC_UART,circ_buf_pop(&write_buffer));
        } else {
            USART_INTConfig(CDC_UART, USART_INT_TDE, DISABLE);
        }
    }
}
