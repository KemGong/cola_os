
#include "stm32f0xx.h"
#include "led.h"



#define PORT_GREEN_LED                 GPIOC                    //端口s
#define PIN_GREENLED                   GPIO_Pin_13              //引脚

/* LED亮、灭、变化 */
#define LED_GREEN_OFF                  (PORT_GREEN_LED->BSRR = PIN_GREENLED)
#define LED_GREEN_ON                   (PORT_GREEN_LED->BRR  = PIN_GREENLED)
#define LED_GREEN_TOGGLE               (PORT_GREEN_LED->ODR ^= PIN_GREENLED)


void led_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = PIN_GREENLED;                             //LED引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                      //输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                  //高速输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                     //推完输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                   //无上下拉(浮空)
    GPIO_Init(PORT_GREEN_LED, &GPIO_InitStructure);
    LED_GREEN_OFF;
}

void led_toggle(void)
{
    LED_GREEN_TOGGLE;
}
