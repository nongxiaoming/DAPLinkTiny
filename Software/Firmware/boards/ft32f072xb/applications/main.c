/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-02     FMD-AE       first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/**
  * @brief USB_PDCTRL
  */
#define	USB_PDCTRL_FLAG 		((uint8_t)(0x03))	

#define	USB_PDCTRL_PD_DISABLE	((uint8_t)(0x00))
#define	USB_PDCTRL_PD_ENABLE	((uint8_t)(0x02))

#define	USB_PDCTRL_PU_DISABLE	((uint8_t)(0x00))
#define	USB_PDCTRL_PU_ENABLE	((uint8_t)(0x01))

#define	IS_USB_PDCTRL(PDCTRL)	(((PDCTRL) == USB_PDCTRL_PD_DISABLE) || \
								((PDCTRL) == USB_PDCTRL_PD_ENABLE) || \
								((PDCTRL) == USB_PDCTRL_PU_DISABLE) || \
								((PDCTRL) == USB_PDCTRL_PU_ENABLE))
                                
/* defined the LED2 pin: PB0 */
#define LED2_PIN    GET_PIN(B, 3)

extern void cdc_acm_hid_msc_descriptor_init(uint8_t busid, uintptr_t reg_base);

void USB_PDCTRLConfig(uint8_t PDCT)
{
    uint8_t temp = 0;

    assert_param(IS_USB_PDCTRL(PDCT));

    temp = USB->PDCTRL;
    temp &= ~USB_PDCTRL_FLAG;
    temp |= PDCT;
    USB->PDCTRL = temp;

}


void usb_dc_low_level_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_USBCLKConfig(RCC_USBCLK_HSI48);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);

    USB_PDCTRLConfig(USB_PDCTRL_PU_ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USB_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
extern void USBD_IRQHandler(uint8_t busid);

void USB_IRQHandler(void)
{
    USBD_IRQHandler(0);
}    

void cdc_acm_data_send_with_dtr_test(uint8_t busid);

int main(void)
{
    /* set LED0 pin mode to output */
    rt_pin_mode(LED2_PIN, PIN_MODE_OUTPUT);
    cdc_acm_hid_msc_descriptor_init(0,USB_BASE);
    while (1)
    {
        cdc_acm_data_send_with_dtr_test(0);
        rt_pin_write(LED2_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED2_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}
