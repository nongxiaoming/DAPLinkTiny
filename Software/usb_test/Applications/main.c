#include "tasks.h"
#include <RTL.h>
#include <rl_usb.h>
#include "led.h"

OS_TID ledTask;

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{    
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_AFIO, ENABLE);

  /* Enable USART3 Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_USART3, ENABLE);  
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitType GPIO_InitStructure;

  /* Enable the USART3 Pins Software Remapping */
  GPIO_PinsRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
  GPIO_StructInit(&GPIO_InitStructure);
  /* Configure USART3 Rx as input floating */
  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

  /* Configure USART3 Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_7;
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
}
void USART_Configuration(void)
{
	USART_InitType USART_InitStructure;
  /* USART3 configuration ------------------------------------------------------*/
  /* USART3 configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - Even parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Configure USART3 */
  USART_Init(USART3, &USART_InitStructure);

  /* Enable the USART3 */
  USART_Cmd(USART3, ENABLE);

}
void uart_send(uint8_t *data,uint16_t len)
{
	  while(len--)
		{
    /* Send one byte  to USART3 */
    USART_SendData(USART3, *data);
    data++;
    /* Loop until USART3 DR register is empty */ 
    while(USART_GetFlagStatus(USART3, USART_FLAG_TDE) == RESET)
    {
    }
	}
}
__task void led_task(void)
{
    while(1)
    {
      LED_Toggle(LED_RED);
			os_dly_wait(100);
    }
}

__task void main_task(void)
{
	usbd_init();
	usbd_connect(__TRUE);

	LED_Init();
  RCC_Configuration();
	GPIO_Configuration();
	USART_Configuration();
    ledTask = os_tsk_create(led_task, 5);
	
	
    while(1)
    {
		os_dly_wait(200);
    LED_Toggle(LED_BLUE);
		uart_send("hello uart3\r\n",13);
    }
}


int main (void)
{
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    os_sys_init(main_task);

    return 0;
}
