#include "tasks.h"
#include <RTL.h>
#include <rl_usb.h>
#include "led.h"

OS_TID ledTask;


__task void led_task(void)
{
    while(1)
    {
      LED_Toggle(LED_B);
			os_dly_wait(100);
    }
}

__task void main_task(void)
{
	usbd_init();
	usbd_connect(__TRUE);

	LED_Init();

	
    ledTask = os_tsk_create(led_task, 5);
	
	
    while(1)
    {
		os_dly_wait(200);
    LED_Toggle(LED_R);
    }
}


int main (void)
{
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    os_sys_init(main_task);

    return 0;
}
