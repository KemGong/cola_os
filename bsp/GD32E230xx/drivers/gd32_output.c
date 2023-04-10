#include "config.h"
#include "gd32e23x.h"
#include "gd32_output.h"
#include "cola_os.h"
#include "cola_device.h"
#include "utils.h"
#include <string.h>


#ifdef USING_OUTPUT 

#define DEF_VAL_HIGH    0x01
#define DEF_VAL_LOW     0x00
#define OUTPUT_SIGNAL_PORT           GPIOA
#define OUTPUT_SIGNAL_PIN            GPIO_PIN_0




struct USER_OUTPUT
{
    const char   *name;
    uint32_t port;
    uint32_t pin;
    uint8_t def;
    cola_device_t dev;
};


static struct USER_OUTPUT out_list[] = 
{
	{"output", OUTPUT_SIGNAL_PORT, OUTPUT_SIGNAL_PIN, DEF_VAL_LOW,0},
};



void output_gpio_init(void)
{
    int i = 0; 
    /* enable GPIOC clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_mode_set(OUTPUT_SIGNAL_PORT, GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,OUTPUT_SIGNAL_PIN);
    gpio_output_options_set(OUTPUT_SIGNAL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, OUTPUT_SIGNAL_PIN);
    for (i = 0; i < array_size(out_list); i++) 
    {
         if(out_list[i].def == DEF_VAL_HIGH)
         {
            gpio_bit_set(out_list[i].port,out_list[i].pin);
         }
         else
         {
            gpio_bit_reset(out_list[i].port,out_list[i].pin);
         }
    }
}

int output_ctrl(cola_device_t *dev, int cmd, void *args)
{
    int i = 0;
    static uint8_t toggle_cnt = 0;
    toggle_cnt++;
    for (i = 0; i < array_size(out_list); i++) 
    {
        if (!strcmp(out_list[i].name, dev->name))
        {
            if(STATE_OUTPUT_LOW == cmd)
            {
                gpio_bit_reset(out_list[i].port,out_list[i].pin);
            }
            else if(STATE_OUTPUT_HIGH == cmd)
            {
                gpio_bit_set(out_list[i].port,out_list[i].pin);
            }
            else if(STATE_OUTPUT_TOGGLE == cmd)
            {
                if (toggle_cnt%2 == 0)
                {
                    gpio_bit_set(out_list[i].port, out_list[i].pin);
                }
                else
                {
                    gpio_bit_reset(out_list[i].port, out_list[i].pin);
                }
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
