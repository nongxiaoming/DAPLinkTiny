#include "usbd_core.h"
#include "usbd_cdc.h"
#include "DAP_config.h"
#include "DAP.h"

#include <rtthread.h>
#include <drivers/serial_v2.h>

#include <rtdbg.h>

#define DAP_IN_EP  0x81
#define DAP_OUT_EP 0x01

#define CDC_IN_EP  0x82
#define CDC_OUT_EP 0x03
#define CDC_INT_EP 0x84

#define MSC_IN_EP  0x86
#define MSC_OUT_EP 0x07

#define USBD_VID           0x0D28
#define USBD_PID           0x0204
#define USBD_MAX_POWER     500
#define USBD_LANGID_STRING 1033

#define CMSIS_DAP_INTERFACE_SIZE (9 + 7 + 7)
#ifndef CONFIG_CHERRYDAP_USE_MSC
#define USB_CONFIG_SIZE (9 + CMSIS_DAP_INTERFACE_SIZE + CDC_ACM_DESCRIPTOR_LEN)
#define INTF_NUM        3
#else
#define USB_CONFIG_SIZE (9 + CMSIS_DAP_INTERFACE_SIZE + CDC_ACM_DESCRIPTOR_LEN + MSC_DESCRIPTOR_LEN)
#define INTF_NUM        4
#endif

#ifdef CONFIG_USB_HS
#if DAP_PACKET_SIZE != 512
#error "DAP_PACKET_SIZE must be 512 in hs"
#endif
#else
#if DAP_PACKET_SIZE != 64
#error "DAP_PACKET_SIZE must be 64 in fs"
#endif
#endif

#define USBD_WINUSB_VENDOR_CODE 0x20

#define USBD_WEBUSB_ENABLE 0

#define USBD_BULK_ENABLE   1
#define USBD_WINUSB_ENABLE 1

/* WinUSB Microsoft OS 2.0 descriptor sizes */
#define WINUSB_DESCRIPTOR_SET_HEADER_SIZE  10
#define WINUSB_FUNCTION_SUBSET_HEADER_SIZE 8
#define WINUSB_FEATURE_COMPATIBLE_ID_SIZE  20

#define FUNCTION_SUBSET_LEN                160
#define DEVICE_INTERFACE_GUIDS_FEATURE_LEN 132

#define USBD_WINUSB_DESC_SET_LEN (WINUSB_DESCRIPTOR_SET_HEADER_SIZE + USBD_WEBUSB_ENABLE * FUNCTION_SUBSET_LEN + USBD_BULK_ENABLE * FUNCTION_SUBSET_LEN)

__ALIGN_BEGIN const uint8_t USBD_WinUSBDescriptorSetDescriptor[] = {
    WBVAL(WINUSB_DESCRIPTOR_SET_HEADER_SIZE), /* wLength */
    WBVAL(WINUSB_SET_HEADER_DESCRIPTOR_TYPE), /* wDescriptorType */
    0x00, 0x00, 0x03, 0x06, /* >= Win 8.1 */  /* dwWindowsVersion*/
    WBVAL(USBD_WINUSB_DESC_SET_LEN),          /* wDescriptorSetTotalLength */
#if (USBD_WEBUSB_ENABLE)
    WBVAL(WINUSB_FUNCTION_SUBSET_HEADER_SIZE), // wLength
    WBVAL(WINUSB_SUBSET_HEADER_FUNCTION_TYPE), // wDescriptorType
    0,                                         // bFirstInterface USBD_WINUSB_IF_NUM
    0,                                         // bReserved
    WBVAL(FUNCTION_SUBSET_LEN),                // wSubsetLength
    WBVAL(WINUSB_FEATURE_COMPATIBLE_ID_SIZE),  // wLength
    WBVAL(WINUSB_FEATURE_COMPATIBLE_ID_TYPE),  // wDescriptorType
    'W', 'I', 'N', 'U', 'S', 'B', 0, 0,        // CompatibleId
    0, 0, 0, 0, 0, 0, 0, 0,                    // SubCompatibleId
    WBVAL(DEVICE_INTERFACE_GUIDS_FEATURE_LEN), // wLength
    WBVAL(WINUSB_FEATURE_REG_PROPERTY_TYPE),   // wDescriptorType
    WBVAL(WINUSB_PROP_DATA_TYPE_REG_MULTI_SZ), // wPropertyDataType
    WBVAL(42),                                 // wPropertyNameLength
    'D', 0, 'e', 0, 'v', 0, 'i', 0, 'c', 0, 'e', 0,
    'I', 0, 'n', 0, 't', 0, 'e', 0, 'r', 0, 'f', 0, 'a', 0, 'c', 0, 'e', 0,
    'G', 0, 'U', 0, 'I', 0, 'D', 0, 's', 0, 0, 0,
    WBVAL(80), // wPropertyDataLength
    '{', 0,
    '9', 0, '2', 0, 'C', 0, 'E', 0, '6', 0, '4', 0, '6', 0, '2', 0, '-', 0,
    '9', 0, 'C', 0, '7', 0, '7', 0, '-', 0,
    '4', 0, '6', 0, 'F', 0, 'E', 0, '-', 0,
    '9', 0, '3', 0, '3', 0, 'B', 0, '-',
    0, '3', 0, '1', 0, 'C', 0, 'B', 0, '9', 0, 'C', 0, '5', 0, 'A', 0, 'A', 0, '3', 0, 'B', 0, '9', 0,
    '}', 0, 0, 0, 0, 0,
#endif
#if USBD_BULK_ENABLE
    WBVAL(WINUSB_FUNCTION_SUBSET_HEADER_SIZE), /* wLength */
    WBVAL(WINUSB_SUBSET_HEADER_FUNCTION_TYPE), /* wDescriptorType */
    0,                                         /* bFirstInterface USBD_BULK_IF_NUM*/
    0,                                         /* bReserved */
    WBVAL(FUNCTION_SUBSET_LEN),                /* wSubsetLength */
    WBVAL(WINUSB_FEATURE_COMPATIBLE_ID_SIZE),  /* wLength */
    WBVAL(WINUSB_FEATURE_COMPATIBLE_ID_TYPE),  /* wDescriptorType */
    'W', 'I', 'N', 'U', 'S', 'B', 0, 0,        /* CompatibleId*/
    0, 0, 0, 0, 0, 0, 0, 0,                    /* SubCompatibleId*/
    WBVAL(DEVICE_INTERFACE_GUIDS_FEATURE_LEN), /* wLength */
    WBVAL(WINUSB_FEATURE_REG_PROPERTY_TYPE),   /* wDescriptorType */
    WBVAL(WINUSB_PROP_DATA_TYPE_REG_MULTI_SZ), /* wPropertyDataType */
    WBVAL(42),                                 /* wPropertyNameLength */
    'D', 0, 'e', 0, 'v', 0, 'i', 0, 'c', 0, 'e', 0,
    'I', 0, 'n', 0, 't', 0, 'e', 0, 'r', 0, 'f', 0, 'a', 0, 'c', 0, 'e', 0,
    'G', 0, 'U', 0, 'I', 0, 'D', 0, 's', 0, 0, 0,
    WBVAL(80), /* wPropertyDataLength */
    '{', 0,
    'C', 0, 'D', 0, 'B', 0, '3', 0, 'B', 0, '5', 0, 'A', 0, 'D', 0, '-', 0,
    '2', 0, '9', 0, '3', 0, 'B', 0, '-', 0,
    '4', 0, '6', 0, '6', 0, '3', 0, '-', 0,
    'A', 0, 'A', 0, '3', 0, '6', 0, '-',
    0, '1', 0, 'A', 0, 'A', 0, 'E', 0, '4', 0, '6', 0, '4', 0, '6', 0, '3', 0, '7', 0, '7', 0, '6', 0,
    '}', 0, 0, 0, 0, 0
#endif
};

#define USBD_NUM_DEV_CAPABILITIES (USBD_WEBUSB_ENABLE + USBD_WINUSB_ENABLE)

#define USBD_WEBUSB_DESC_LEN 24
#define USBD_WINUSB_DESC_LEN 28

#define USBD_BOS_WTOTALLENGTH (0x05 +                                      \
                               USBD_WEBUSB_DESC_LEN * USBD_WEBUSB_ENABLE + \
                               USBD_WINUSB_DESC_LEN * USBD_WINUSB_ENABLE)

__ALIGN_BEGIN const uint8_t USBD_BinaryObjectStoreDescriptor[] = {
    0x05,                         /* bLength */
    0x0f,                         /* bDescriptorType */
    WBVAL(USBD_BOS_WTOTALLENGTH), /* wTotalLength */
    USBD_NUM_DEV_CAPABILITIES,    /* bNumDeviceCaps */
#if (USBD_WEBUSB_ENABLE)
    USBD_WEBUSB_DESC_LEN,           /* bLength */
    0x10,                           /* bDescriptorType */
    USB_DEVICE_CAPABILITY_PLATFORM, /* bDevCapabilityType */
    0x00,                           /* bReserved */
    0x38, 0xB6, 0x08, 0x34,         /* PlatformCapabilityUUID */
    0xA9, 0x09, 0xA0, 0x47,
    0x8B, 0xFD, 0xA0, 0x76,
    0x88, 0x15, 0xB6, 0x65,
    WBVAL(0x0100), /* 1.00 */ /* bcdVersion */
    USBD_WINUSB_VENDOR_CODE,  /* bVendorCode */
    0,                        /* iLandingPage */
#endif
#if (USBD_WINUSB_ENABLE)
    USBD_WINUSB_DESC_LEN,           /* bLength */
    0x10,                           /* bDescriptorType */
    USB_DEVICE_CAPABILITY_PLATFORM, /* bDevCapabilityType */
    0x00,                           /* bReserved */
    0xDF, 0x60, 0xDD, 0xD8,         /* PlatformCapabilityUUID */
    0x89, 0x45, 0xC7, 0x4C,
    0x9C, 0xD2, 0x65, 0x9D,
    0x9E, 0x64, 0x8A, 0x9F,
    0x00, 0x00, 0x03, 0x06, /* >= Win 8.1 */ /* dwWindowsVersion*/
    WBVAL(USBD_WINUSB_DESC_SET_LEN),         /* wDescriptorSetTotalLength */
    USBD_WINUSB_VENDOR_CODE,                 /* bVendorCode */
    0,                                       /* bAltEnumCode */
#endif
};

const uint8_t cmsisdap_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_1, 0xEF, 0x02, 0x01, USBD_VID, USBD_PID, 0x0100, 0x01),
    /* Configuration 0 */
    USB_CONFIG_DESCRIPTOR_INIT(USB_CONFIG_SIZE, INTF_NUM, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
    /* Interface 0 */
    USB_INTERFACE_DESCRIPTOR_INIT(0x00, 0x00, 0x02, 0xFF, 0x00, 0x00, 0x02),
    /* Endpoint OUT 2 */
    USB_ENDPOINT_DESCRIPTOR_INIT(DAP_OUT_EP, USB_ENDPOINT_TYPE_BULK, DAP_PACKET_SIZE, 0x00),
    /* Endpoint IN 1 */
    USB_ENDPOINT_DESCRIPTOR_INIT(DAP_IN_EP, USB_ENDPOINT_TYPE_BULK, DAP_PACKET_SIZE, 0x00),
    CDC_ACM_DESCRIPTOR_INIT(0x01, CDC_INT_EP, CDC_OUT_EP, CDC_IN_EP, DAP_PACKET_SIZE, 0x00),
#ifdef CONFIG_CHERRYDAP_USE_MSC
    MSC_DESCRIPTOR_INIT(0x03, MSC_OUT_EP, MSC_IN_EP, DAP_PACKET_SIZE, 0x00),
#endif
    /* String 0 (LANGID) */
    USB_LANGID_INIT(USBD_LANGID_STRING),
    /* String 1 (Manufacturer) */
    0x14,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'C', 0x00,                  /* wcChar0 */
    'h', 0x00,                  /* wcChar1 */
    'e', 0x00,                  /* wcChar2 */
    'r', 0x00,                  /* wcChar3 */
    'r', 0x00,                  /* wcChar4 */
    'y', 0x00,                  /* wcChar5 */
    'U', 0x00,                  /* wcChar6 */
    'S', 0x00,                  /* wcChar7 */
    'B', 0x00,                  /* wcChar8 */
    /* String 2 (Product) */
    0x28,                       // bLength
    USB_DESCRIPTOR_TYPE_STRING, // bDescriptorType
    'C', 0x00,                  // wcChar0
    'h', 0x00,                  // wcChar1
    'e', 0x00,                  // wcChar2
    'r', 0x00,                  // wcChar3
    'r', 0x00,                  // wcChar4
    'y', 0x00,                  // wcChar5
    'U', 0x00,                  // wcChar6
    'S', 0x00,                  // wcChar7
    'B', 0x00,                  // wcChar8
    ' ', 0x00,                  // wcChar9
    'C', 0x00,                  // wcChar10
    'M', 0x00,                  // wcChar11
    'S', 0x00,                  // wcChar12
    'I', 0x00,                  // wcChar13
    'S', 0x00,                  // wcChar14
    '-', 0x00,                  // wcChar15
    'D', 0x00,                  // wcChar16
    'A', 0x00,                  // wcChar17
    'P', 0x00,                  // wcChar18
    /* String 3 (Serial Number) */
    0x1A,                       // bLength
    USB_DESCRIPTOR_TYPE_STRING, // bDescriptorType
    '0', 0,                     // wcChar0
    '1', 0,                     // wcChar1
    '2', 0,                     // wcChar2
    '3', 0,                     // wcChar3
    '4', 0,                     // wcChar4
    '5', 0,                     // wcChar5
    'A', 0,                     // wcChar6
    'B', 0,                     // wcChar7
    'C', 0,                     // wcChar8
    'D', 0,                     // wcChar9
    'E', 0,                     // wcChar10
    'F', 0,                     // wcChar11
#ifdef CONFIG_USB_HS
    /* Device Qualifier */
    0x0a,
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    0x10,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x01,
    0x00,
#endif
    /* End */
    0x00
};

static volatile uint16_t USB_RequestIndexI; // Request  Index In
static volatile uint16_t USB_RequestIndexO; // Request  Index Out
static volatile uint16_t USB_RequestCountI; // Request  Count In
static volatile uint16_t USB_RequestCountO; // Request  Count Out
static volatile uint8_t USB_RequestIdle;    // Request  Idle  Flag

static volatile uint16_t USB_ResponseIndexI; // Response Index In
static volatile uint16_t USB_ResponseIndexO; // Response Index Out
static volatile uint16_t USB_ResponseCountI; // Response Count In
static volatile uint16_t USB_ResponseCountO; // Response Count Out
static volatile uint8_t USB_ResponseIdle;    // Response Idle  Flag

static USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t USB_Request[DAP_PACKET_COUNT][DAP_PACKET_SIZE];  // Request  Buffer
static USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t USB_Response[DAP_PACKET_COUNT][DAP_PACKET_SIZE]; // Response Buffer
static uint16_t USB_RespSize[DAP_PACKET_COUNT];                                                        // Response Size

volatile uint8_t config_uart = 0;
volatile uint8_t config_uart_transfer = 0;


static USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t usb_tmpbuffer[DAP_PACKET_SIZE];

static volatile uint8_t usbrx_idle_flag = 0;
static volatile uint8_t usbtx_idle_flag = 0;
static volatile uint8_t uarttx_idle_flag = 0;
struct rx_msg
{
    rt_device_t dev;
    rt_size_t size;
};

static rt_device_t serial;

char mq_msg_pool[256];
static struct rt_messagequeue rx_mq;

static struct rt_semaphore cdc_in_ep_sem;

void usbd_event_handler(uint8_t event)
{
    switch (event) {
        case USBD_EVENT_RESET:
            usbrx_idle_flag = 0;
            usbtx_idle_flag = 0;
            uarttx_idle_flag = 0;
            config_uart_transfer = 0;
            break;
        case USBD_EVENT_CONNECTED:
            break;
        case USBD_EVENT_DISCONNECTED:
            break;
        case USBD_EVENT_RESUME:
            break;
        case USBD_EVENT_SUSPEND:
            break;
        case USBD_EVENT_CONFIGURED:
            /* setup first out ep read transfer */
            USB_RequestIdle = 0U;

            usbd_ep_start_read(DAP_OUT_EP, USB_Request[0], DAP_PACKET_SIZE);
            usbd_ep_start_read(CDC_OUT_EP, usb_tmpbuffer, DAP_PACKET_SIZE);
            break;
        case USBD_EVENT_SET_REMOTE_WAKEUP:
            break;
        case USBD_EVENT_CLR_REMOTE_WAKEUP:
            break;

        default:
            break;
    }
}

void dap_out_callback(uint8_t ep, uint32_t nbytes)
{
    if (USB_Request[USB_RequestIndexI][0] == ID_DAP_TransferAbort) {
        DAP_TransferAbort = 1U;
    } else {
        USB_RequestIndexI++;
        if (USB_RequestIndexI == DAP_PACKET_COUNT) {
            USB_RequestIndexI = 0U;
        }
        USB_RequestCountI++;
    }

    // Start reception of next request packet
    if ((uint16_t)(USB_RequestCountI - USB_RequestCountO) != DAP_PACKET_COUNT) {
        usbd_ep_start_read(DAP_OUT_EP, USB_Request[USB_RequestIndexI], DAP_PACKET_SIZE);
    } else {
        USB_RequestIdle = 1U;
    }
}

void dap_in_callback(uint8_t ep, uint32_t nbytes)
{
    if (USB_ResponseCountI != USB_ResponseCountO) {
        // Load data from response buffer to be sent back
        usbd_ep_start_write(DAP_IN_EP, USB_Response[USB_ResponseIndexO], USB_RespSize[USB_ResponseIndexO]);
        USB_ResponseIndexO++;
        if (USB_ResponseIndexO == DAP_PACKET_COUNT) {
            USB_ResponseIndexO = 0U;
        }
        USB_ResponseCountO++;
    } else {
        USB_ResponseIdle = 1U;
    }
}

void usbd_cdc_acm_bulk_out(uint8_t ep, uint32_t nbytes)
{
//    rt_ringbuffer_put(&g_usbrx, usb_tmpbuffer, nbytes);
//    if ((g_usbrx.buffer_size - rt_ringbuffer_data_len(&g_usbrx)) >= DAP_PACKET_SIZE) {
//        usbd_ep_start_read(CDC_OUT_EP, usb_tmpbuffer, DAP_PACKET_SIZE);
//    } else {
//        usbrx_idle_flag = 1;
//    }
}

void usbd_cdc_acm_bulk_in(uint8_t ep, uint32_t nbytes)
{
//    uint32_t size;
//    uint8_t *buffer;

//    chry_ringbuffer_linear_read_done(&g_uartrx, nbytes);
//    if ((nbytes % DAP_PACKET_SIZE) == 0 && nbytes) {
//        /* send zlp */
//        usbd_ep_start_write(CDC_IN_EP, NULL, 0);
//    } else {
//        if (chry_ringbuffer_get_used(&g_uartrx)) {
//            buffer = chry_ringbuffer_linear_read_setup(&g_uartrx, &size);
//            usbd_ep_start_write(CDC_IN_EP, buffer, size);
//        } else {
//            usbtx_idle_flag = 1;
//        }
//    }
}

static struct usbd_endpoint dap_out_ep = {
    .ep_addr = DAP_OUT_EP,
    .ep_cb = dap_out_callback
};

static struct usbd_endpoint dap_in_ep = {
    .ep_addr = DAP_IN_EP,
    .ep_cb = dap_in_callback
};

static struct usbd_endpoint cdc_out_ep = {
    .ep_addr = CDC_OUT_EP,
    .ep_cb = usbd_cdc_acm_bulk_out
};

static struct usbd_endpoint cdc_in_ep = {
    .ep_addr = CDC_IN_EP,
    .ep_cb = usbd_cdc_acm_bulk_in
};

struct usbd_interface dap_intf;
struct usbd_interface intf1;
struct usbd_interface intf2;
struct usbd_interface intf3;

static void chry_dap_state_init(void)
{
    // Initialize variables
    USB_RequestIndexI = 0U;
    USB_RequestIndexO = 0U;
    USB_RequestCountI = 0U;
    USB_RequestCountO = 0U;
    USB_RequestIdle = 1U;
    USB_ResponseIndexI = 0U;
    USB_ResponseIndexO = 0U;
    USB_ResponseCountI = 0U;
    USB_ResponseCountO = 0U;
    USB_ResponseIdle = 1U;
}

struct usb_msosv2_descriptor msosv2_desc = {
    .vendor_code = USBD_WINUSB_VENDOR_CODE,
    .compat_id = USBD_WinUSBDescriptorSetDescriptor,
    .compat_id_len = USBD_WINUSB_DESC_SET_LEN,
};

struct usb_bos_descriptor bos_desc = {
    .string = USBD_BinaryObjectStoreDescriptor,
    .string_len = USBD_BOS_WTOTALLENGTH
};

void daplink_init(void)
{

    DAP_Setup();

    chry_dap_state_init();

    usbd_desc_register(cmsisdap_descriptor);
    usbd_bos_desc_register(&bos_desc);
    usbd_msosv2_desc_register(&msosv2_desc);

    /*!< winusb */
    usbd_add_interface(&dap_intf);
    usbd_add_endpoint(&dap_out_ep);
    usbd_add_endpoint(&dap_in_ep);

    /*!< cdc acm */
    usbd_add_interface(usbd_cdc_acm_init_intf(&intf1));
    usbd_add_interface(usbd_cdc_acm_init_intf(&intf2));
    usbd_add_endpoint(&cdc_out_ep);
    usbd_add_endpoint(&cdc_in_ep);

#ifdef CONFIG_CHERRYDAP_USE_MSC
    usbd_add_interface(usbd_msc_init_intf(&intf3, MSC_OUT_EP, MSC_IN_EP));
#endif
    usbd_initialize();
}

void chry_dap_handle(void)
{
    uint32_t n;

    // Process pending requests
    while (USB_RequestCountI != USB_RequestCountO) {
        // Handle Queue Commands
        n = USB_RequestIndexO;
        while (USB_Request[n][0] == ID_DAP_QueueCommands) {
            USB_Request[n][0] = ID_DAP_ExecuteCommands;
            n++;
            if (n == DAP_PACKET_COUNT) {
                n = 0U;
            }
            if (n == USB_RequestIndexI) {
                // flags = osThreadFlagsWait(0x81U, osFlagsWaitAny, osWaitForever);
                // if (flags & 0x80U) {
                //     break;
                // }
            }
        }

        // Execute DAP Command (process request and prepare response)
        USB_RespSize[USB_ResponseIndexI] =
            (uint16_t)DAP_ExecuteCommand(USB_Request[USB_RequestIndexO], USB_Response[USB_ResponseIndexI]);

        // Update Request Index and Count
        USB_RequestIndexO++;
        if (USB_RequestIndexO == DAP_PACKET_COUNT) {
            USB_RequestIndexO = 0U;
        }
        USB_RequestCountO++;

        if (USB_RequestIdle) {
            if ((uint16_t)(USB_RequestCountI - USB_RequestCountO) != DAP_PACKET_COUNT) {
                USB_RequestIdle = 0U;
                usbd_ep_start_read(DAP_OUT_EP, USB_Request[USB_RequestIndexI], DAP_PACKET_SIZE);
            }
        }

        // Update Response Index and Count
        USB_ResponseIndexI++;
        if (USB_ResponseIndexI == DAP_PACKET_COUNT) {
            USB_ResponseIndexI = 0U;
        }
        USB_ResponseCountI++;

        if (USB_ResponseIdle) {
            if (USB_ResponseCountI != USB_ResponseCountO) {
                // Load data from response buffer to be sent back
                n = USB_ResponseIndexO++;
                if (USB_ResponseIndexO == DAP_PACKET_COUNT) {
                    USB_ResponseIndexO = 0U;
                }
                USB_ResponseCountO++;
                USB_ResponseIdle = 0U;
                usbd_ep_start_write(DAP_IN_EP, USB_Response[n], USB_RespSize[n]);
            }
        }
    }
}


void chry_dap_usb2uart_handle(void)
{

}

/* implment by user */
__WEAK void chry_dap_usb2uart_uart_config_callback(struct cdc_line_coding *line_coding)
{
}

/* called by user */
void chry_dap_usb2uart_uart_send_complete(uint32_t size)
{
//    uint8_t *buffer;

//    chry_ringbuffer_linear_read_done(&g_usbrx, size);

//    if (chry_ringbuffer_get_used(&g_usbrx)) {
//        buffer = chry_ringbuffer_linear_read_setup(&g_usbrx, &size);
//        chry_dap_usb2uart_uart_send_bydma(buffer, size);
//    } else {
//        uarttx_idle_flag = 1;
//    }
}

/* implment by user */
__WEAK void chry_dap_usb2uart_uart_send_bydma(uint8_t *data, uint16_t len)
{
}

#ifdef DAP_USE_MSC
#define BLOCK_SIZE  512
#define BLOCK_COUNT 10

typedef struct
{
    uint8_t BlockSpace[BLOCK_SIZE];
} BLOCK_TYPE;

BLOCK_TYPE mass_block[BLOCK_COUNT];

void usbd_msc_get_cap(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
    *block_num = 1000; //Pretend having so many buffer,not has actually.
    *block_size = BLOCK_SIZE;
}
int usbd_msc_sector_read(uint32_t sector, uint8_t *buffer, uint32_t length)
{
    if (sector < 10)
        memcpy(buffer, mass_block[sector].BlockSpace, length);
    return 0;
}

int usbd_msc_sector_write(uint32_t sector, uint8_t *buffer, uint32_t length)
{
    if (sector < 10)
        memcpy(mass_block[sector].BlockSpace, buffer, length);
    return 0;
}
#endif


/**
  * @brief  usb 48M clock select
  * @param  clk_s:USB_CLK_HICK, USB_CLK_HEXT
  * @retval none
  */
void usb_clock48m_select(usb_clk48_s clk_s)
{
  if(clk_s == USB_CLK_HICK)
  {
    crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_HICK);

    /* enable the acc calibration ready interrupt */
    crm_periph_clock_enable(CRM_ACC_PERIPH_CLOCK, TRUE);

    /* update the c1\c2\c3 value */
    acc_write_c1(7980);
    acc_write_c2(8000);
    acc_write_c3(8020);

    /* open acc calibration */
    acc_calibration_mode_enable(ACC_CAL_HICKTRIM, TRUE);
  }
  else
  {
    crm_pllu_output_set(TRUE);
    /* wait till pll is ready */
    while(crm_flag_get(CRM_PLLU_STABLE_FLAG) != SET)
    {
    }
    crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_PLLU);
  }
}

void usb_dc_low_level_init(void)
{
    /* enable otg clock */
    crm_periph_clock_enable(CRM_OTGHS_PERIPH_CLOCK, TRUE);
    usb_clock48m_select(USB_CLK_HEXT);
    nvic_irq_enable(OTGHS_IRQn, 0, 0);
}

static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    struct rx_msg msg;
    rt_err_t result;
    msg.dev = dev;
    msg.size = size;

    result = rt_mq_send(&rx_mq, &msg, sizeof(msg));
    if (result == -RT_EFULL)
    {
        LOG_D("message queue full！\n");
    }
    return result;
}

struct serial_configure config;

void usbd_cdc_acm_get_line_coding(uint8_t intf, struct cdc_line_coding *line_coding)
{
    line_coding->dwDTERate = config.baud_rate;
    line_coding->bDataBits = config.data_bits;
    line_coding->bParityType = config.parity;
    line_coding->bCharFormat = config.stop_bits;
}

void usbd_cdc_acm_set_line_coding(uint8_t intf, struct cdc_line_coding *line_coding)
{
    config.baud_rate = line_coding->dwDTERate;
    config.bit_order = line_coding->bDataBits;
    config.stop_bits = line_coding->bCharFormat;
    config.parity = line_coding->bParityType;
    config.rx_bufsz = BSP_UART2_RX_BUFSIZE;
    config.tx_bufsz = BSP_UART2_TX_BUFSIZE;
    
    rt_device_close(serial);
    rt_device_set_rx_indicate(serial, uart_input);
    rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);
    rt_device_open(serial, RT_DEVICE_FLAG_RX_NON_BLOCKING | RT_SERIAL_TX_NON_BLOCKING);
    config_uart = 1;
    config_uart_transfer = 0;
}

void usb_cdc_thread_entry(void *para)
{
    rt_err_t result;
    uint8_t rx_buffer[BSP_UART2_RX_BUFSIZE];
    uint8_t cdc_buffer[DAP_PACKET_SIZE];
    rt_uint32_t rx_length;
    rt_uint32_t tx_length;
    rt_size_t remain_length;
    struct rx_msg msg;
    while (1)
    {
        result = rt_mq_recv(&rx_mq, &msg, sizeof(msg), RT_WAITING_FOREVER);
        if (result == RT_EOK)
        {
            /* 从串口读取数据 */
            rx_length = rt_device_read(msg.dev, 0, rx_buffer, msg.size);

            remain_length = rx_length;
            rx_length = 0;
            while(remain_length)
            {
                rt_sem_take(&cdc_in_ep_sem, RT_WAITING_FOREVER);
                if (remain_length > DAP_PACKET_SIZE)
                {
                    tx_length = DAP_PACKET_SIZE;
                }
                else
                {
                    tx_length = remain_length;
                }
                usbd_ep_start_write(CDC_IN_EP, rx_buffer + rx_length, tx_length);
                remain_length -= tx_length;
                rx_length += tx_length;
            }
        }
    }
}

void usb_dap_thread_entry(void *para)
{
    rt_err_t result;

    while (1)
    {
      chry_dap_handle();
      rt_thread_delay(1);
    }
}

int app_daplink_init(void)
{
    serial = rt_device_find("uart2");
    rt_sem_init(&cdc_in_ep_sem, "cdc_in_ep_sem", 1, RT_IPC_FLAG_FIFO);

    rt_mq_init(&rx_mq, "rx_mq",
               mq_msg_pool,
               sizeof(struct rx_msg),
               sizeof(mq_msg_pool),
               RT_IPC_FLAG_FIFO);

    DAP_Setup();

    daplink_init();

    rt_thread_startup(
        rt_thread_create("cdc", usb_cdc_thread_entry, RT_NULL, 2048, 20, 20)
    );

    rt_thread_startup(
        rt_thread_create("dap", usb_dap_thread_entry, RT_NULL, 2048, 10, 20)
    );

    return RT_EOK;
}
INIT_APP_EXPORT(app_daplink_init);
