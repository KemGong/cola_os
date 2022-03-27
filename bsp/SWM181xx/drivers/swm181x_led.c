#include <string.h>
#include "config.h"
#include "SWM181.h"
#include "cola_device.h"
#include "cola_init.h"
#include "utils.h"

#define DEF_VAL_NOPULL       0x02
#define DEF_VAL_HIGH         0x01
#define DEF_VAL_LOW          0x00

#define LED_GREEN_Pin        PIN5
#define LED_GREEN_GPIO_Port  GPIOA



static void GPIO_Toggle(GPIO_TypeDef * GPIOx, uint32_t GPIO_Pin)
{
	GPIO_InvBit(GPIOx, GPIO_Pin);
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

};

static void led_gpio_init(void)
{
    int i = 0; 
    
    for (i = 0; i < array_size(led_list); i++) 
    {
		GPIO_Init(led_list[i].port, led_list[i].pin, 1, 0, 0, 0);
		if(led_list[i].def == DEF_VAL_HIGH)
		{
			GPIO_SetBits(led_list[i].port,led_list[i].pin,1);
		}
		else
		{
			GPIO_ClrBits(led_list[i].port,led_list[i].pin,1);
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
                GPIO_SetBits(led_list[i].port,led_list[i].pin,1);
            }
            else
            {
                GPIO_ClrBits(led_list[i].port,led_list[i].pin,1);
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

void board_setup_led(void)
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

device_initcall(board_setup_led);
