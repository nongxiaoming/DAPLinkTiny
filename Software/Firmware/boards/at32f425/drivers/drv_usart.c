/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-16     shelton      first version
 * 2022-11-10     shelton      support uart dma
 * 2023-01-31     shelton      add support f421/f425
 * 2023-04-08     shelton      add support f423
 */

#include "drv_common.h"
#include "drv_usart.h"
#include "drv_config.h"

#ifdef RT_USING_SERIAL
#if !defined(BSP_USING_UART1) && !defined(BSP_USING_UART2) && \
    !defined(BSP_USING_UART3) && !defined(BSP_USING_UART4) && \
    !defined(BSP_USING_UART5) && !defined(BSP_USING_UART6) && \
    !defined(BSP_USING_UART7) && !defined(BSP_USING_UART8)
    #error "Please define at least one BSP_USING_UARTx"
#endif

enum {
#ifdef BSP_USING_UART1
    UART1_INDEX,
#endif
#ifdef BSP_USING_UART2
    UART2_INDEX,
#endif
#ifdef BSP_USING_UART3
    UART3_INDEX,
#endif
#ifdef BSP_USING_UART4
    UART4_INDEX,
#endif
#ifdef BSP_USING_UART5
    UART5_INDEX,
#endif
#ifdef BSP_USING_UART6
    UART6_INDEX,
#endif
#ifdef BSP_USING_UART7
    UART7_INDEX,
#endif
#ifdef BSP_USING_UART8
    UART8_INDEX,
#endif
};

static struct at32_uart uart_config[] = {
#ifdef BSP_USING_UART1
    UART1_CONFIG,
#endif
#ifdef BSP_USING_UART2
    UART2_CONFIG,
#endif
#ifdef BSP_USING_UART3
    UART3_CONFIG,
#endif
#ifdef BSP_USING_UART4
    UART4_CONFIG,
#endif
#ifdef BSP_USING_UART5
    UART5_CONFIG,
#endif
#ifdef BSP_USING_UART6
    UART6_CONFIG,
#endif
#ifdef BSP_USING_UART7
    UART7_CONFIG,
#endif
#ifdef BSP_USING_UART8
    UART8_CONFIG,
#endif
};

#ifdef RT_SERIAL_USING_DMA
static void at32_dma_config(struct rt_serial_device *serial, rt_ubase_t flag);
#endif

static rt_err_t at32_configure(struct rt_serial_device *serial,
    struct serial_configure *cfg) {
    usart_data_bit_num_type data_bit;
    usart_stop_bit_num_type stop_bit;
    usart_parity_selection_type parity_mode;
    usart_hardware_flow_control_type flow_control;

    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);

    struct at32_uart *instance = rt_container_of(serial, struct at32_uart, serial);
    RT_ASSERT(instance != RT_NULL);

    at32_msp_usart_init((void *)instance->uart_x);

    usart_receiver_enable(instance->uart_x, TRUE);
    usart_transmitter_enable(instance->uart_x, TRUE);

    switch (cfg->data_bits) {
    case DATA_BITS_8:
        data_bit = USART_DATA_8BITS;
        break;
    case DATA_BITS_9:
        data_bit = USART_DATA_9BITS;
        break;
    default:
        data_bit = USART_DATA_8BITS;
        break;
    }

    switch (cfg->stop_bits) {
    case STOP_BITS_1:
        stop_bit = USART_STOP_1_BIT;
        break;
    case STOP_BITS_2:
        stop_bit = USART_STOP_2_BIT;
        break;
    default:
        stop_bit = USART_STOP_1_BIT;
        break;
    }

    switch (cfg->parity) {
    case PARITY_NONE:
        parity_mode = USART_PARITY_NONE;
        break;
    case PARITY_ODD:
        parity_mode = USART_PARITY_ODD;
        break;
    case PARITY_EVEN:
        parity_mode = USART_PARITY_EVEN;
        break;
    default:
        parity_mode = USART_PARITY_NONE;
        break;
    }

    switch (cfg->flowcontrol) {
    case RT_SERIAL_FLOWCONTROL_NONE:
        flow_control = USART_HARDWARE_FLOW_NONE;
        break;
    case RT_SERIAL_FLOWCONTROL_CTSRTS:
        flow_control = USART_HARDWARE_FLOW_RTS_CTS;
        break;
    default:
        flow_control = USART_HARDWARE_FLOW_NONE;
        break;
    }

#ifdef RT_SERIAL_USING_DMA
    if (!(serial->parent.open_flag & RT_DEVICE_OFLAG_OPEN)) {
        instance->last_index = 0;
    }
#endif

    usart_hardware_flow_control_set(instance->uart_x, flow_control);
    usart_parity_selection_config(instance->uart_x, parity_mode);
    usart_init(instance->uart_x, cfg->baud_rate, data_bit, stop_bit);
    usart_enable(instance->uart_x, TRUE);

    return RT_EOK;
}

static rt_err_t at32_control(struct rt_serial_device *serial, int cmd, void *arg) {
    struct at32_uart *instance;

#ifdef RT_SERIAL_USING_DMA
    rt_ubase_t ctrl_arg = (rt_ubase_t)arg;
#endif
    RT_ASSERT(serial != RT_NULL);

    instance = rt_container_of(serial, struct at32_uart, serial);
    RT_ASSERT(instance != RT_NULL);

    switch (cmd) {
    case RT_DEVICE_CTRL_CLR_INT:
        nvic_irq_disable(instance->irqn);
        usart_interrupt_enable(instance->uart_x, USART_RDBF_INT, FALSE);

#ifdef RT_SERIAL_USING_DMA
        /* disable DMA */
        if (ctrl_arg == RT_DEVICE_FLAG_DMA_RX)
        {
            nvic_irq_disable(instance->dma_rx->dma_irqn);
            dma_reset(instance->dma_rx->dma_channel);
        }
        else if(ctrl_arg == RT_DEVICE_FLAG_DMA_TX)
        {
            nvic_irq_disable(instance->dma_tx->dma_irqn);
            dma_reset(instance->dma_tx->dma_channel);
        }
#endif
        break;
    case RT_DEVICE_CTRL_SET_INT:
        nvic_irq_enable(instance->irqn, 1, 0);
        usart_interrupt_enable(instance->uart_x, USART_RDBF_INT, TRUE);
        break;
#ifdef RT_SERIAL_USING_DMA
    case RT_DEVICE_CTRL_CONFIG:
        at32_dma_config(serial, ctrl_arg);
        break;
#endif
    }

    return RT_EOK;
}

static int at32_putc(struct rt_serial_device *serial, char ch) {
    struct at32_uart *instance;

    RT_ASSERT(serial != RT_NULL);

    instance = rt_container_of(serial, struct at32_uart, serial);
    RT_ASSERT(instance != RT_NULL);

    usart_data_transmit(instance->uart_x, (uint8_t)ch);
    while (usart_flag_get(instance->uart_x, USART_TDC_FLAG) == RESET);

    return 1;
}

static int at32_getc(struct rt_serial_device *serial) {
    int ch;
    struct at32_uart *instance;

    RT_ASSERT(serial != RT_NULL);

    instance = rt_container_of(serial, struct at32_uart, serial);
    RT_ASSERT(instance != RT_NULL);

    ch = -1;
    if (usart_flag_get(instance->uart_x, USART_RDBF_FLAG) != RESET) {
        ch = usart_data_receive(instance->uart_x) & 0xff;
    }

    return ch;
}

#ifdef RT_SERIAL_USING_DMA
static void _uart_dma_receive(struct at32_uart *instance, rt_uint8_t *buffer, rt_uint32_t size)
{
    dma_channel_type* dma_channel = instance->dma_rx->dma_channel;

    dma_channel->dtcnt = size;
    dma_channel->paddr = (rt_uint32_t)&(instance->uart_x->dt);
    dma_channel->maddr = (rt_uint32_t)buffer;
    /* enable usart interrupt */
    usart_interrupt_enable(instance->uart_x, USART_PERR_INT, TRUE);
    usart_interrupt_enable(instance->uart_x, USART_IDLE_INT, TRUE);
    /* enable transmit complete interrupt */
    dma_interrupt_enable(dma_channel, DMA_FDT_INT, TRUE);
    /* enable dma receive */
    usart_dma_receiver_enable(instance->uart_x, TRUE);

    /* enable dma channel */
    dma_channel_enable(dma_channel, TRUE);
}

static void _uart_dma_transmit(struct at32_uart *instance, rt_uint8_t *buffer, rt_uint32_t size)
{
    /* wait before transfer complete */
    while(instance->dma_tx->dma_done == RT_FALSE);

    dma_channel_type *dma_channel = instance->dma_tx->dma_channel;

    dma_channel->dtcnt = size;
    dma_channel->paddr = (rt_uint32_t)&(instance->uart_x->dt);
    dma_channel->maddr = (rt_uint32_t)buffer;

    /* enable transmit complete interrupt */
    dma_interrupt_enable(dma_channel, DMA_FDT_INT, TRUE);
    /* enable dma transmit */
    usart_dma_transmitter_enable(instance->uart_x, TRUE);

    /* mark dma flag */
    instance->dma_tx->dma_done = RT_FALSE;
    /* enable dma channel */
    dma_channel_enable(dma_channel, TRUE);
}

static void at32_dma_config(struct rt_serial_device *serial, rt_ubase_t flag)
{
    dma_init_type dma_init_struct;
    dma_channel_type *dma_channel = NULL;
    struct rt_serial_rx_fifo *rx_fifo;
    struct at32_uart *instance;
    struct dma_config *dma_config;

    RT_ASSERT(serial != RT_NULL);

    instance = rt_container_of(serial, struct at32_uart, serial);
    RT_ASSERT(instance != RT_NULL);

    RT_ASSERT(flag == RT_DEVICE_FLAG_DMA_TX || flag == RT_DEVICE_FLAG_DMA_RX);

    if (RT_DEVICE_FLAG_DMA_RX == flag)
    {
        dma_channel = instance->dma_rx->dma_channel;
        dma_config = instance->dma_rx;
    }
    else /* RT_DEVICE_FLAG_DMA_TX == flag */
    {
        dma_channel = instance->dma_tx->dma_channel;
        dma_config = instance->dma_tx;
    }

    crm_periph_clock_enable(dma_config->dma_clock, TRUE);
    dma_default_para_init(&dma_init_struct);
    dma_init_struct.peripheral_inc_enable = FALSE;
    dma_init_struct.memory_inc_enable = TRUE;
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
    dma_init_struct.priority = DMA_PRIORITY_MEDIUM;

    if (RT_DEVICE_FLAG_DMA_RX == flag)
    {
        dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
        dma_init_struct.loop_mode_enable = TRUE;
    }
    else if (RT_DEVICE_FLAG_DMA_TX == flag)
    {
        dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
        dma_init_struct.loop_mode_enable = FALSE;
    }

    dma_reset(dma_channel);
    dma_init(dma_channel, &dma_init_struct);
#if defined (SOC_SERIES_AT32F425)
    dma_flexible_config(dma_config->dma_x, dma_config->flex_channel, \
                       (dma_flexible_request_type)dma_config->request_id);
#endif
#if defined (SOC_SERIES_AT32F435) || defined (SOC_SERIES_AT32F437) || \
    defined (SOC_SERIES_AT32F423)
    dmamux_enable(dma_config->dma_x, TRUE);
    dmamux_init(dma_config->dmamux_channel, (dmamux_requst_id_sel_type)dma_config->request_id);
#endif
    /* enable interrupt */
    if (flag == RT_DEVICE_FLAG_DMA_RX)
    {
        rx_fifo = (struct rt_serial_rx_fifo *)serial->serial_rx;
        /* start dma transfer */
        _uart_dma_receive(instance, rx_fifo->buffer, serial->config.bufsz);
    }

    /* dma irq should set in dma tx mode */
    nvic_irq_enable(dma_config->dma_irqn, 0, 0);
    nvic_irq_enable(instance->irqn, 1, 0);
}

static rt_ssize_t at32_dma_transmit(struct rt_serial_device *serial, rt_uint8_t *buf, rt_size_t size, int direction)
{
    struct at32_uart *instance;
    RT_ASSERT(serial != RT_NULL);

    instance = rt_container_of(serial, struct at32_uart, serial);
    RT_ASSERT(instance != RT_NULL);
    RT_ASSERT(buf != RT_NULL);

    if (size == 0)
    {
        return 0;
    }

    if (RT_SERIAL_DMA_TX == direction)
    {
        _uart_dma_transmit(instance, buf, size);
    }

    return size;
}
#endif

static const struct rt_uart_ops at32_uart_ops = {
    at32_configure,
    at32_control,
    at32_putc,
    at32_getc,
#ifdef RT_SERIAL_USING_DMA
    at32_dma_transmit,
#endif
};

#ifdef RT_SERIAL_USING_DMA
void dma_rx_isr(struct rt_serial_device *serial)
{
    volatile rt_uint32_t reg_sts = 0, index = 0;
    rt_size_t recv_total_index, recv_len;
    rt_base_t level;
    struct at32_uart *instance;
    RT_ASSERT(serial != RT_NULL);

    instance = rt_container_of(serial, struct at32_uart, serial);
    RT_ASSERT(instance != RT_NULL);

    reg_sts = instance->dma_rx->dma_x->sts;
    index = instance->dma_rx->channel_index;

    if (((reg_sts & (DMA_FDT_FLAG << (4 * (index - 1)))) != RESET) ||
        ((reg_sts & (DMA_HDT_FLAG << (4 * (index - 1)))) != RESET))
    {
        /* clear dma flag */
        instance->dma_rx->dma_x->clr |= (rt_uint32_t)(DMA_FDT_FLAG << (4 * (index - 1))) | (DMA_HDT_FLAG << (4 * (index - 1)));

        level = rt_hw_interrupt_disable();
        recv_total_index = serial->config.bufsz - dma_data_number_get(instance->dma_rx->dma_channel);
        if (recv_total_index == 0)
        {
            recv_len = serial->config.bufsz - instance->last_index;
        }
        else
        {
            recv_len = recv_total_index - instance->last_index;
        }
        instance->last_index = recv_total_index;
        rt_hw_interrupt_enable(level);

        if (recv_len)
        {
            rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_DMADONE | (recv_len << 8));
        }
    }
}

void dma_tx_isr(struct rt_serial_device *serial)
{
    volatile rt_uint32_t reg_sts = 0, index = 0;
    rt_size_t trans_total_index;
    rt_base_t level;
    struct at32_uart *instance;
    RT_ASSERT(serial != RT_NULL);

    instance = rt_container_of(serial, struct at32_uart, serial);
    RT_ASSERT(instance != RT_NULL);

    reg_sts = instance->dma_tx->dma_x->sts;
    index = instance->dma_tx->channel_index;

    if ((reg_sts & (DMA_FDT_FLAG << (4 * (index - 1)))) != RESET)
    {
        /* mark dma flag */
        instance->dma_tx->dma_done = RT_TRUE;
        /* clear dma flag */
        instance->dma_tx->dma_x->clr |= (rt_uint32_t)(DMA_FDT_FLAG << (4 * (index - 1)));
        /* disable dma tx channel */
        dma_channel_enable(instance->dma_tx->dma_channel, FALSE);

        level = rt_hw_interrupt_disable();
        trans_total_index = dma_data_number_get(instance->dma_tx->dma_channel);
        rt_hw_interrupt_enable(level);

        if (trans_total_index == 0)
        {
            rt_hw_serial_isr(serial, RT_SERIAL_EVENT_TX_DMADONE);
        }
    }
}
#endif

static void usart_isr(struct rt_serial_device *serial) {
    struct at32_uart *instance;
#ifdef RT_SERIAL_USING_DMA
    rt_size_t recv_total_index, recv_len;
    rt_base_t level;
#endif
    RT_ASSERT(serial != RT_NULL);

    instance = rt_container_of(serial, struct at32_uart, serial);
    RT_ASSERT(instance != RT_NULL);

    if (usart_flag_get(instance->uart_x, USART_RDBF_FLAG) != RESET) {
        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_IND);
    }
#ifdef RT_SERIAL_USING_DMA
    else if (usart_flag_get(instance->uart_x, USART_IDLEF_FLAG) != RESET)
    {
        /* clear idle flag */
        usart_data_receive(instance->uart_x);

        level = rt_hw_interrupt_disable();
        recv_total_index = serial->config.bufsz - dma_data_number_get(instance->dma_rx->dma_channel);
        recv_len = recv_total_index - instance->last_index;
        instance->last_index = recv_total_index;
        rt_hw_interrupt_enable(level);

        if (recv_len)
        {
            rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_DMADONE | (recv_len << 8));
        }
    }
#endif
    else
    {
        if (usart_flag_get(instance->uart_x, USART_CTSCF_FLAG) != RESET) {
            usart_flag_clear(instance->uart_x, USART_CTSCF_FLAG);
        }

        if (usart_flag_get(instance->uart_x, USART_BFF_FLAG) != RESET) {
            usart_flag_clear(instance->uart_x, USART_BFF_FLAG);
        }

        if (usart_flag_get(instance->uart_x, USART_TDC_FLAG) != RESET) {
            usart_flag_clear(instance->uart_x, USART_TDC_FLAG);
        }
    }
}

#ifdef BSP_USING_UART1
void UART1_IRQHandler(void) {
    rt_interrupt_enter();

    usart_isr(&uart_config[UART1_INDEX].serial);

    rt_interrupt_leave();
}
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART1_RX_USING_DMA)
void UART1_RX_DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    dma_rx_isr(&uart_config[UART1_INDEX].serial);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART1_RX_USING_DMA) */
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART1_TX_USING_DMA)
void UART1_TX_DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    dma_tx_isr(&uart_config[UART1_INDEX].serial);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART1_TX_USING_DMA) */
#endif
#ifdef BSP_USING_UART2
void UART2_IRQHandler(void) {
    rt_interrupt_enter();

    usart_isr(&uart_config[UART2_INDEX].serial);

    rt_interrupt_leave();
}
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART2_RX_USING_DMA)
void UART2_RX_DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    dma_rx_isr(&uart_config[UART2_INDEX].serial);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART2_RX_USING_DMA) */
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART2_TX_USING_DMA)
void UART2_TX_DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    dma_tx_isr(&uart_config[UART2_INDEX].serial);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART2_TX_USING_DMA) */
#endif
#ifdef BSP_USING_UART3
void UART3_IRQHandler(void) {
    rt_interrupt_enter();

    usart_isr(&uart_config[UART3_INDEX].serial);

    rt_interrupt_leave();
}
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART3_RX_USING_DMA)
void UART3_RX_DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    dma_rx_isr(&uart_config[UART3_INDEX].serial);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART3_RX_USING_DMA) */
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART3_TX_USING_DMA)
void UART3_TX_DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    dma_tx_isr(&uart_config[UART3_INDEX].serial);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART3_TX_USING_DMA) */
#endif
#ifdef BSP_USING_UART4
void UART4_IRQHandler(void) {
    rt_interrupt_enter();

    usart_isr(&uart_config[UART4_INDEX].serial);

    rt_interrupt_leave();
}
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART4_RX_USING_DMA)
void UART4_RX_DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    dma_rx_isr(&uart_config[UART4_INDEX].serial);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART4_RX_USING_DMA) */
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART4_TX_USING_DMA)
void UART4_TX_DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    dma_tx_isr(&uart_config[UART4_INDEX].serial);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART14_TX_USING_DMA) */
#endif
#ifdef BSP_USING_UART5
void UART5_IRQHandler(void) {
    rt_interrupt_enter();

    usart_isr(&uart_config[UART5_INDEX].serial);

    rt_interrupt_leave();
}
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART5_RX_USING_DMA)
void UART5_RX_DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    dma_rx_isr(&uart_config[UART5_INDEX].serial);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART5_RX_USING_DMA) */
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART5_TX_USING_DMA)
void UART5_TX_DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    dma_tx_isr(&uart_config[UART5_INDEX].serial);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART5_TX_USING_DMA) */
#endif
#ifdef BSP_USING_UART6
void UART6_IRQHandler(void) {
    rt_interrupt_enter();

    usart_isr(&uart_config[UART6_INDEX].serial);

    rt_interrupt_leave();
}
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART6_RX_USING_DMA)
void UART6_RX_DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    dma_rx_isr(&uart_config[UART6_INDEX].serial);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART6_RX_USING_DMA) */
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART6_TX_USING_DMA)
void UART6_TX_DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    dma_tx_isr(&uart_config[UART6_INDEX].serial);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART6_TX_USING_DMA) */
#endif
#ifdef BSP_USING_UART7
void UART7_IRQHandler(void) {
    rt_interrupt_enter();

    usart_isr(&uart_config[UART7_INDEX].serial);

    rt_interrupt_leave();
}
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART7_RX_USING_DMA)
void UART7_RX_DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    dma_rx_isr(&uart_config[UART7_INDEX].serial);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART7_RX_USING_DMA) */
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART7_TX_USING_DMA)
void UART7_TX_DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    dma_tx_isr(&uart_config[UART7_INDEX].serial);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART7_TX_USING_DMA) */
#endif
#ifdef BSP_USING_UART8
void UART8_IRQHandler(void) {
    rt_interrupt_enter();

    usart_isr(&uart_config[UART8_INDEX].serial);

    rt_interrupt_leave();
}
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART8_RX_USING_DMA)
void UART8_RX_DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    dma_rx_isr(&uart_config[UART8_INDEX].serial);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART8_RX_USING_DMA) */
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART8_TX_USING_DMA)
void UART8_TX_DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    dma_tx_isr(&uart_config[UART8_INDEX].serial);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* defined(RT_SERIAL_USING_DMA) && defined(BSP_UART8_TX_USING_DMA) */
#endif

#if defined (SOC_SERIES_AT32F421)
void UART1_TX_RX_DMA_IRQHandler(void)
{
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART1_TX_USING_DMA)
    UART1_TX_DMA_IRQHandler();
#endif

#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART1_RX_USING_DMA)
    UART1_RX_DMA_IRQHandler();
#endif
}

void UART2_TX_RX_DMA_IRQHandler(void)
{
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART2_TX_USING_DMA)
    UART2_TX_DMA_IRQHandler();
#endif

#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART2_RX_USING_DMA)
    UART2_RX_DMA_IRQHandler();
#endif
}
#endif

#if defined (SOC_SERIES_AT32F425)
#if defined(BSP_USING_UART3) || defined(BSP_USING_UART4)
void USART4_3_IRQHandler(void)
{
#if defined(BSP_USING_UART3)
  UART3_IRQHandler();
#endif
#if defined(BSP_USING_UART4)
  UART4_IRQHandler();
#endif
}
#endif

void UART1_TX_RX_DMA_IRQHandler(void)
{
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART1_TX_USING_DMA)
    UART1_TX_DMA_IRQHandler();
#endif

#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART1_RX_USING_DMA)
    UART1_RX_DMA_IRQHandler();
#endif
}

void UART3_2_TX_RX_DMA_IRQHandler(void)
{
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART2_TX_USING_DMA)
    UART2_TX_DMA_IRQHandler();
#endif

#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART2_RX_USING_DMA)
    UART2_RX_DMA_IRQHandler();
#endif
#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART3_TX_USING_DMA)
    UART3_TX_DMA_IRQHandler();
#endif

#if defined(RT_SERIAL_USING_DMA) && defined(BSP_UART3_RX_USING_DMA)
    UART3_RX_DMA_IRQHandler();
#endif
}
#endif

#if defined (RT_SERIAL_USING_DMA)
static void _dma_base_channel_check(struct at32_uart *instance)
{
    dma_channel_type *rx_channel = instance->dma_rx->dma_channel;
    dma_channel_type *tx_channel = instance->dma_tx->dma_channel;

    instance->dma_rx->dma_done = RT_TRUE;
    instance->dma_rx->dma_x = (dma_type *)((rt_uint32_t)rx_channel & ~0xFF);
    instance->dma_rx->channel_index = ((((rt_uint32_t)rx_channel & 0xFF) - 8) / 0x14) + 1;

    instance->dma_tx->dma_done = RT_TRUE;
    instance->dma_tx->dma_x = (dma_type *)((rt_uint32_t)tx_channel & ~0xFF);
    instance->dma_tx->channel_index = ((((rt_uint32_t)tx_channel & 0xFF) - 8) / 0x14) + 1;
}
#endif

static void at32_uart_get_dma_config(void)
{
#ifdef BSP_USING_UART1
    uart_config[UART1_INDEX].uart_dma_flag = 0;
#ifdef BSP_UART1_RX_USING_DMA
    uart_config[UART1_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_RX;
    static struct dma_config uart1_dma_rx = UART1_RX_DMA_CONFIG;
    uart_config[UART1_INDEX].dma_rx = &uart1_dma_rx;
#endif
#ifdef BSP_UART1_TX_USING_DMA
    uart_config[UART1_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_TX;
    static struct dma_config uart1_dma_tx = UART1_TX_DMA_CONFIG;
    uart_config[UART1_INDEX].dma_tx = &uart1_dma_tx;
#endif
#endif

#ifdef BSP_USING_UART2
    uart_config[UART2_INDEX].uart_dma_flag = 0;
#ifdef BSP_UART2_RX_USING_DMA
    uart_config[UART2_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_RX;
    static struct dma_config uart2_dma_rx = UART2_RX_DMA_CONFIG;
    uart_config[UART2_INDEX].dma_rx = &uart2_dma_rx;
#endif
#ifdef BSP_UART2_TX_USING_DMA
    uart_config[UART2_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_TX;
    static struct dma_config uart2_dma_tx = UART2_TX_DMA_CONFIG;
    uart_config[UART2_INDEX].dma_tx = &uart2_dma_tx;
#endif
#endif

#ifdef BSP_USING_UART3
    uart_config[UART3_INDEX].uart_dma_flag = 0;
#ifdef BSP_UART3_RX_USING_DMA
    uart_config[UART3_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_RX;
    static struct dma_config uart3_dma_rx = UART3_RX_DMA_CONFIG;
    uart_config[UART3_INDEX].dma_rx = &uart3_dma_rx;
#endif
#ifdef BSP_UART3_TX_USING_DMA
    uart_config[UART3_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_TX;
    static struct dma_config uart3_dma_tx = UART3_TX_DMA_CONFIG;
    uart_config[UART3_INDEX].dma_tx = &uart3_dma_tx;
#endif
#endif

#ifdef BSP_USING_UART4
    uart_config[UART4_INDEX].uart_dma_flag = 0;
#ifdef BSP_UART4_RX_USING_DMA
    uart_config[UART4_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_RX;
    static struct dma_config uart4_dma_rx = UART4_RX_DMA_CONFIG;
    uart_config[UART4_INDEX].dma_rx = &uart4_dma_rx;
#endif
#ifdef BSP_UART4_TX_USING_DMA
    uart_config[UART4_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_TX;
    static struct dma_config uart4_dma_tx = UART4_TX_DMA_CONFIG;
    uart_config[UART4_INDEX].dma_tx = &uart4_dma_tx;
#endif
#endif

#ifdef BSP_USING_UART5
    uart_config[UART5_INDEX].uart_dma_flag = 0;
#ifdef BSP_UART5_RX_USING_DMA
    uart_config[UART5_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_RX;
    static struct dma_config uart5_dma_rx = UART5_RX_DMA_CONFIG;
    uart_config[UART5_INDEX].dma_rx = &uart5_dma_rx;
#endif
#ifdef BSP_UART5_TX_USING_DMA
    uart_config[UART5_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_TX;
    static struct dma_config uart5_dma_tx = UART5_TX_DMA_CONFIG;
    uart_config[UART5_INDEX].dma_tx = &uart5_dma_tx;
#endif
#endif

#ifdef BSP_USING_UART6
    uart_config[UART6_INDEX].uart_dma_flag = 0;
#ifdef BSP_UART6_RX_USING_DMA
    uart_config[UART6_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_RX;
    static struct dma_config uart6_dma_rx = UART6_RX_DMA_CONFIG;
    uart_config[UART6_INDEX].dma_rx = &uart6_dma_rx;
#endif
#ifdef BSP_UART6_TX_USING_DMA
    uart_config[UART6_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_TX;
    static struct dma_config uart6_dma_tx = UART6_TX_DMA_CONFIG;
    uart_config[UART6_INDEX].dma_tx = &uart6_dma_tx;
#endif
#endif

#ifdef BSP_USING_UART7
    uart_config[UART7_INDEX].uart_dma_flag = 0;
#ifdef BSP_UART7_RX_USING_DMA
    uart_config[UART7_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_RX;
    static struct dma_config uart7_dma_rx = UART7_RX_DMA_CONFIG;
    uart_config[UART7_INDEX].dma_rx = &uart7_dma_rx;
#endif
#ifdef BSP_UART7_TX_USING_DMA
    uart_config[UART7_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_TX;
    static struct dma_config uart7_dma_tx = UART7_TX_DMA_CONFIG;
    uart_config[UART7_INDEX].dma_tx = &uart7_dma_tx;
#endif
#endif

#ifdef BSP_USING_UART8
    uart_config[UART8_INDEX].uart_dma_flag = 0;
#ifdef BSP_UART8_RX_USING_DMA
    uart_config[UART8_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_RX;
    static struct dma_config uart8_dma_rx = UART8_RX_DMA_CONFIG;
    uart_config[UART8_INDEX].dma_rx = &uart8_dma_rx;
#endif
#ifdef BSP_UART8_TX_USING_DMA
    uart_config[UART8_INDEX].uart_dma_flag |= RT_DEVICE_FLAG_DMA_TX;
    static struct dma_config uart8_dma_tx = UART8_TX_DMA_CONFIG;
    uart_config[UART8_INDEX].dma_tx = &uart8_dma_tx;
#endif
#endif
}

int rt_hw_usart_init(void) {
    rt_size_t obj_num;
    int index;

    obj_num = sizeof(uart_config) / sizeof(struct at32_uart);
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    rt_err_t result = 0;

    at32_uart_get_dma_config();

    for (index = 0; index < obj_num; index++) {
        uart_config[index].serial.ops = &at32_uart_ops;
        uart_config[index].serial.config = config;

#if defined (RT_SERIAL_USING_DMA)
        /* search dma base and channel index */
        _dma_base_channel_check(&uart_config[index]);
#endif
        /* register uart device */
        result = rt_hw_serial_register(&uart_config[index].serial,
                 uart_config[index].name,
                 RT_DEVICE_FLAG_RDWR |
                 RT_DEVICE_FLAG_INT_RX |
                 uart_config[index].uart_dma_flag ,
                 &uart_config[index]);
        RT_ASSERT(result == RT_EOK);
    }

    return result;
}

#endif /* BSP_USING_SERIAL */
