#include "led.h"


void LED_Init ( void )
{
    GPIO_InitType GPIO_InitStructure;

    RCC_APB2PeriphClockCmd ( LED1_RCC | LED2_RCC | LED3_RCC, ENABLE );

    /*Configure the LED pin as ouput push-pull*/
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pins = LED1_PIN;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP; 		 
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;	

    GPIO_Init ( LED1_PORT, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pins = LED2_PIN;
    GPIO_Init ( LED2_PORT, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pins = LED3_PIN;
    GPIO_Init ( LED3_PORT, &GPIO_InitStructure );

    LED_Off ( LED_ALL );
}


void LED_On ( uint16_t leds )
{

    if ( leds & LED_R )
    {
        GPIO_ResetBits ( LED1_PORT, LED1_PIN );
    }
    if ( leds & LED_G )
    {
        GPIO_ResetBits ( LED2_PORT, LED2_PIN );
    }
    if ( leds & LED_B )
    {
        GPIO_ResetBits ( LED3_PORT, LED3_PIN );
    }
}

void LED_Off ( uint16_t leds )
{
    if ( leds & LED_R )
    {
        GPIO_SetBits ( LED1_PORT, LED1_PIN );
    }
    if ( leds & LED_G )
    {
        GPIO_SetBits ( LED2_PORT, LED2_PIN );
    }
    if ( leds & LED_B )
    {
        GPIO_SetBits ( LED3_PORT, LED3_PIN );
    }
}

void LED_Toggle ( uint16_t leds )
{
    if ( leds & LED_R )
    {
        LED1_PORT->OPTDT ^= LED1_PIN;
    }
    if ( leds & LED_G )
    {
        LED2_PORT->OPTDT ^= LED2_PIN;
    }
    if ( leds & LED_B )
    {
        LED3_PORT->OPTDT ^= LED3_PIN;
    }
}
