#include "config.h"
#include "stm32l0xx_hal.h"
#include "stm32_output.h"
#include "cola_os.h"
#include "cola_device.h"
#include "utils.h"
#include <string.h>
#include "gpio.h"


#ifdef USING_OUTPUT 


#define DEF_VAL_HIGH    0x01
#define DEF_VAL_LOW     0x00
#define SX1276_RESET_Pin         GPIO_Pin_8
#define SX1276_RESET_GPIO_Port   GPIOB


struct USER_OUTPUT
{
    const char   *name;
    GPIO_TypeDef *port;
    uint32_t pin;
    uint8_t def;
    cola_device_t dev;
};


static struct USER_OUTPUT out_list[] = 
{
	{"reset", RESET_GPIO_Port,RESET_Pin,DEF_VAL_HIGH,0},
};

static void output_gpio_init(void)
{
    
}

int output_ctrl(cola_device_t *dev, int cmd, void *args)
{
    
   int i = 0; 
    for (i = 0; i < array_size(out_list); i++) 
    {
        if (!strcmp(out_list[i].name, dev->name))
        {
            if(STATE_OUTPUT_LOW == cmd)
            {
                HAL_GPIO_WritePin(out_list[i].port, out_list[i].pin, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(out_list[i].port, out_list[i].pin, GPIO_PIN_SET);
            }
            return 1;            
        }
    }
    return 0; 
}


static const struct cola_device_ops output_ops =
{
    .control = output_ctrl,
};


void board_setup_output(void)
{
    int i = 0; 
    output_gpio_init();
    for (i = 0; i < array_size(out_list); i++) 
    {
        out_list[i].dev.name = out_list[i].name;
        out_list[i].dev.dops  = &output_ops;
        cola_device_register(&out_list[i].dev);
    }
}

device_initcall(board_setup_output);

#endif

