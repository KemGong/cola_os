#include "config.h"
#include "stm32l0xx_hal.h"
#include "stm32_led.h"
#include "gpio.h"
#include "cola_os.h"
#include "cola_device.h"
#include <string.h>
#include "utils.h"

#define DEF_VAL_NOPULL       0x02
#define DEF_VAL_HIGH         0x01
#define DEF_VAL_LOW          0x00


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
    MX_GPIO_Init();
    
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
                HAL_GPIO_TogglePin(led_list[i].port,led_list[i].pin);
            }
            else  if(LED_OFF == cmd)
            {
                HAL_GPIO_WritePin(led_list[i].port, led_list[i].pin, GPIO_PIN_SET);
            }
            else
            {
                HAL_GPIO_WritePin(led_list[i].port, led_list[i].pin, GPIO_PIN_RESET);
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
