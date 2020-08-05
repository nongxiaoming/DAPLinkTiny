/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-01-06     xiaonong     first version for stm32h7
 */

#include <rtthread.h>

#ifdef BSP_USING_USBD
#include <rtdevice.h>
#include "board.h"
#include <string.h>
#include "drv_usbd.h"

static struct udcd at_udc;
static struct ep_id _ep_pool[] =
{
    {0x0,  USB_EP_ATTR_CONTROL,     USB_DIR_INOUT,  64, ID_ASSIGNED  },
    {0x1,  USB_EP_ATTR_BULK,        USB_DIR_IN,     64, ID_UNASSIGNED},
    {0x1,  USB_EP_ATTR_BULK,        USB_DIR_OUT,    64, ID_UNASSIGNED},
    {0x2,  USB_EP_ATTR_INT,         USB_DIR_IN,     64, ID_UNASSIGNED},
    {0x2,  USB_EP_ATTR_INT,         USB_DIR_OUT,    64, ID_UNASSIGNED},
    {0x3,  USB_EP_ATTR_BULK,        USB_DIR_IN,     64, ID_UNASSIGNED},
    {0x3,  USB_EP_ATTR_BULK,        USB_DIR_OUT,    64, ID_UNASSIGNED},
    {0xFF, USB_EP_ATTR_TYPE_MASK,   USB_DIR_MASK,   0,  ID_ASSIGNED  },
};

void USBD_IRQ_HANDLER(void)
{
    rt_interrupt_enter();
    HAL_PCD_IRQHandler(&_stm_pcd);
    /* leave interrupt */
    rt_interrupt_leave();
}

void HAL_PCD_ResetCallback(PCD_HandleTypeDef *pcd)
{
    /* open ep0 OUT and IN */
    HAL_PCD_EP_Open(pcd, 0x00, 0x40, EP_TYPE_CTRL);
    HAL_PCD_EP_Open(pcd, 0x80, 0x40, EP_TYPE_CTRL);
    rt_usbd_reset_handler(&at_udc);
}

void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)
{
    rt_usbd_ep0_setup_handler(&at_udc, (struct urequest *)hpcd->Setup);
}

void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    if (epnum == 0)
    {
        rt_usbd_ep0_in_handler(&at_udc);
    }
    else
    {
        rt_usbd_ep_in_handler(&at_udc, 0x80 | epnum, hpcd->IN_ep[epnum].xfer_count);
    }
}

void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
{
    rt_usbd_connect_handler(&at_udc);
}

void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
{
    rt_usbd_sof_handler(&at_udc);
}

void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
{
    rt_usbd_disconnect_handler(&_stm_udc);
}

void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    if (epnum != 0)
    {
        rt_usbd_ep_out_handler(&at_udc, epnum, hpcd->OUT_ep[epnum].xfer_count);
    }
    else
    {
        rt_usbd_ep0_out_handler(&at_udc, hpcd->OUT_ep[0].xfer_count);
    }
}

void HAL_PCDEx_SetConnectionState(PCD_HandleTypeDef *hpcd, uint8_t state)
{
    if (state == 1)
    {
#if defined(SOC_SERIES_STM32F1)
    rt_pin_mode(BSP_USB_CONNECT_PIN,PIN_MODE_OUTPUT);
    rt_pin_write(BSP_USB_CONNECT_PIN, BSP_USB_PULL_UP_STATUS);
#endif
    }
    else
    {
#if defined(SOC_SERIES_STM32F1)
    rt_pin_mode(BSP_USB_CONNECT_PIN,PIN_MODE_OUTPUT);
    rt_pin_write(BSP_USB_CONNECT_PIN, !BSP_USB_PULL_UP_STATUS);
#endif
    }
}

static rt_err_t _ep_set_stall(rt_uint8_t address)
{
    HAL_PCD_EP_SetStall(&at_udc, address);
    return RT_EOK;
}

static rt_err_t _ep_clear_stall(rt_uint8_t address)
{
    HAL_PCD_EP_ClrStall(&at_udc, address);
    return RT_EOK;
}

static rt_err_t _set_address(rt_uint8_t address)
{
    HAL_PCD_SetAddress(&at_udc, address);
    return RT_EOK;
}

static rt_err_t _set_config(rt_uint8_t address)
{
    return RT_EOK;
}

static rt_err_t _ep_enable(uep_t ep)
{
    RT_ASSERT(ep != RT_NULL);
    RT_ASSERT(ep->ep_desc != RT_NULL);
    HAL_PCD_EP_Open(&at_udc, ep->ep_desc->bEndpointAddress,
                    ep->ep_desc->wMaxPacketSize, ep->ep_desc->bmAttributes);
    return RT_EOK;
}

static rt_err_t _ep_disable(uep_t ep)
{
    RT_ASSERT(ep != RT_NULL);
    RT_ASSERT(ep->ep_desc != RT_NULL);
    HAL_PCD_EP_Close(&at_udc, ep->ep_desc->bEndpointAddress);
    return RT_EOK;
}

static rt_size_t _ep_read(rt_uint8_t address, void *buffer)
{
    rt_size_t size = 0;
    RT_ASSERT(buffer != RT_NULL);
    return size;
}

static rt_size_t _ep_read_prepare(rt_uint8_t address, void *buffer, rt_size_t size)
{
    HAL_PCD_EP_Receive(&at_udc, address, buffer, size);
    return size;
}

static rt_size_t _ep_write(rt_uint8_t address, void *buffer, rt_size_t size)
{
    HAL_PCD_EP_Transmit(&at_udc, address, buffer, size);
    return size;
}

static rt_err_t _ep0_send_status(void)
{
    HAL_PCD_EP_Transmit(&at_udc, 0x00, NULL, 0);
    return RT_EOK;
}

static rt_err_t _suspend(void)
{
    return RT_EOK;
}

static rt_err_t _wakeup(void)
{
    return RT_EOK;
}
/**
 * @brief PCD MSP initialization
 * @param[in] hpcd Pointer to a PCD_HandleTypeDef structure
 **/

void HAL_PCD_MspInit(PCD_HandleTypeDef *hpcd)
{
   GPIO_InitTypeDef GPIO_InitStruct;
  if (hpcd->Instance == USB_OTG_FS)
  {
   //Enable GPIO clocks
   __HAL_RCC_GPIOA_CLK_ENABLE();

   //Configure VBUS pin (PA9)
   GPIO_InitStruct.Pin = GPIO_PIN_9;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull = GPIO_PULLDOWN;
   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

   //Configure OTG1_FS_ID (PA10)
   GPIO_InitStruct.Pin = GPIO_PIN_10;
   GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
   GPIO_InitStruct.Pull = GPIO_PULLUP;
   GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_FS;
   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

   //Configure OTG1_FS_DM (PA11) and OTG1_FS_DP (PA12)
   GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
   GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_FS;
   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

   //Enable USB FS clock
   __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

   //Set OTG_FS interrupt priority
   HAL_NVIC_SetPriority(OTG_FS_IRQn, 15, 0);

   //Enable OTG_FS interrupt
   HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
	 
  } 
		 //Enable USB voltage level detector
   LL_PWR_EnableUSBVoltageDetector();
}


/**
 * @brief PCD MSP de-initialization
 * @param[in] hpcd Pointer to a PCD_HandleTypeDef structure
 **/

void HAL_PCD_MspDeInit(PCD_HandleTypeDef *hpcd)
{
  if (hpcd->Instance == USB2_OTG_FS)
  {
    /* Disable USB FS Clocks */
    __HAL_RCC_USB2_OTG_FS_CLK_DISABLE();
  }
}

static rt_err_t _init(rt_device_t device)
{
    PCD_HandleTypeDef *pcd;
    /* Set LL Driver parameters */
    pcd = (PCD_HandleTypeDef *)device->user_data;
    pcd->Instance = USBD_INSTANCE;
    memset(&pcd->Init, 0, sizeof pcd->Init);
    pcd->Init.dev_endpoints = 8;
    pcd->Init.speed = USBD_PCD_SPEED;
    pcd->Init.ep0_mps = DEP0CTL_MPS_64;
#if !defined(SOC_SERIES_STM32F1)
    pcd->Init.phy_itface = USBD_PCD_PHY_MODULE;
#endif
    /* Initialize LL Driver */
    HAL_PCD_Init(pcd);
    /* USB interrupt Init */
    HAL_NVIC_SetPriority(USBD_IRQ_TYPE, 2, 0);
    HAL_NVIC_EnableIRQ(USBD_IRQ_TYPE);
#if !defined(SOC_SERIES_STM32F1)
    HAL_PCDEx_SetRxFiFo(pcd, 0x80);
    HAL_PCDEx_SetTxFiFo(pcd, 0, 0x40);
    HAL_PCDEx_SetTxFiFo(pcd, 1, 0x40);
    HAL_PCDEx_SetTxFiFo(pcd, 2, 0x40);
    HAL_PCDEx_SetTxFiFo(pcd, 3, 0x40);
#else
    HAL_PCDEx_PMAConfig(pcd, 0x00, PCD_SNG_BUF, 0x18);
    HAL_PCDEx_PMAConfig(pcd, 0x80, PCD_SNG_BUF, 0x58);
    HAL_PCDEx_PMAConfig(pcd, 0x81, PCD_SNG_BUF, 0x98);
    HAL_PCDEx_PMAConfig(pcd, 0x01, PCD_SNG_BUF, 0x118);
    HAL_PCDEx_PMAConfig(pcd, 0x82, PCD_SNG_BUF, 0xD8);
    HAL_PCDEx_PMAConfig(pcd, 0x02, PCD_SNG_BUF, 0x158);
    HAL_PCDEx_PMAConfig(pcd, 0x83, PCD_SNG_BUF, 0x198);
#endif
    HAL_PCD_Start(pcd);
    return RT_EOK;
}

const static struct udcd_ops _udc_ops =
{
    _set_address,
    _set_config,
    _ep_set_stall,
    _ep_clear_stall,
    _ep_enable,
    _ep_disable,
    _ep_read_prepare,
    _ep_read,
    _ep_write,
    _ep0_send_status,
    _suspend,
    _wakeup,
};

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops _ops =
{
    _init,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    RT_NULL,
};
#endif

int stm_usbd_register(void)
{
    rt_memset((void *)&at_udc, 0, sizeof(struct udcd));
    at_udc.parent.type = RT_Device_Class_USBDevice;
#ifdef RT_USING_DEVICE_OPS
    at_udc.parent.ops = &_ops;
#else
    at_udc.parent.init = _init;
#endif
    at_udc.parent.user_data = &_stm_pcd;
    at_udc.ops = &_udc_ops;
    /* Register endpoint infomation */
    at_udc.ep_pool = _ep_pool;
    at_udc.ep0.id = &_ep_pool[0];
#ifdef BSP_USBD_SPEED_HS
    at_udc.device_is_hs = RT_TRUE;
#endif
    rt_device_register((rt_device_t)&at_udc, "usbd", 0);
    rt_usb_device_init();
    return RT_EOK;
}
INIT_DEVICE_EXPORT(stm_usbd_register);
#endif
