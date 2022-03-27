#include "config.h"
#include <string.h>
#include "stm32f0xx_conf.h"
#include "stm32_led.h"
#include "cola_os.h"
#include "cola_device.h"
#include "utils.h"


#define DEF_VAL_NOPULL       0x02
#define DEF_VAL_HIGH         0x01
#define DEF_VAL_LOW          0x00

#define LED_GREEN_Pin        GPIO_Pin_13
#define LED_GREEN_GPIO_Port  GPIOC


#define LED_BLUE_Pin         GPIO_Pin_2
#define LED_BLUE_GPIO_Port   GPIOA

#define LED_RED_Pin          GPIO_Pin_0
#define LED_RED_GPIO_Port    GPIOA

static void GPIO_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->ODR  ^= GPIO_Pin;
}


struct USER_LED
{
    const char   *name;
    GPIO_TypeDef *port;
    uint32_t pin;
    uint8_t def;
    cola_device_t dev;
};

static struct USER_LED led_list[] = 
{
	{"led_green", LED_GREEN_GPIO_Port,LED_GREEN_Pin, DEF_VAL_HIGH,{0}},
    {"led_blue",  LED_BLUE_GPIO_Port ,LED_BLUE_Pin,  DEF_VAL_HIGH,{0}},
    {"led_red",   LED_RED_GPIO_Port,  LED_RED_Pin,   DEF_VAL_HIGH,{0}},
};

static void led_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    int i = 0; 
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;                     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                     
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;         
    for (i = 0; i < array_size(led_list); i++) 
    {
         GPIO_InitStructure.GPIO_Pin =  led_list[i].pin;                            //Òý½Å
         if(led_list[i].def == DEF_VAL_HIGH)
         {
            GPIO_SetBits(led_list[i].port,led_list[i].pin);
         }
         else
         {
            GPIO_ResetBits(led_list[i].port,led_list[i].pin);
         }
         GPIO_Init(led_list[i].port, &GPIO_InitStructure);
         if(led_list[i].def == DEF_VAL_HIGH)
         {
            GPIO_SetBits(led_list[i].port,led_list[i].pin);
         }
         else
         {
            GPIO_ResetBits(led_list[i].port,led_list[i].pin);
         }
    }
}

static int led_ctrl(cola_device_t *dev, int cmd, void *args)
{
    
    int i = 0; 
    for (i = 0; i < array_size(led_list); i++) 
    {
        if (!strcmp(led_list[i].name, dev->name))
        {
            if(LED_TOGGLE == cmd)
            {
                GPIO_Toggle(led_list[i].port,led_list[i].pin);
            }
            else  if(LED_OFF == cmd)
            {
                GPIO_SetBits(led_list[i].port,led_list[i].pin);
            }
            else
            {
                GPIO_ResetBits(led_list[i].port,led_list[i].pin);
            }
            return 1;            
        }
    }
    return 0;
}


static struct cola_device_ops ops =
{
    .control = led_ctrl,
};

static void led_register(void)
{
    int i;
    led_gpio_init();
    for (i = 0; i < array_size(led_list); i++) 
    {
        led_list[i].dev.name = led_list[i].name;
        led_list[i].dev.dops  = &ops;
        cola_device_register(&led_list[i].dev);
    }
}

device_initcall(led_register);
