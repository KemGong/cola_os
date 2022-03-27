#include "config.h"
#include "stm32f0xx_conf.h"
#include "stm32_output.h"
#include "cola_os.h"
#include "cola_device.h"
#include "utils.h"
#include <string.h>


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
	{"reset",       SX1276_RESET_GPIO_Port,SX1276_RESET_Pin,DEF_VAL_HIGH,0},
};



void output_gpio_init(void)
{
    int i = 0; 
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;                     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                     
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;                  
    for (i = 0; i < array_size(out_list); i++) 
    {
         GPIO_InitStructure.GPIO_Pin =  out_list[i].pin;                            //Òý½Å
         if(out_list[i].def == DEF_VAL_HIGH)
         {
            GPIO_SetBits(out_list[i].port,out_list[i].pin);
         }
         else
         {
            GPIO_ResetBits(out_list[i].port,out_list[i].pin);
         }
         GPIO_Init(out_list[i].port, &GPIO_InitStructure);
         if(out_list[i].def == DEF_VAL_HIGH)
         {
            GPIO_SetBits(out_list[i].port,out_list[i].pin);
         }
         else
         {
            GPIO_ResetBits(out_list[i].port,out_list[i].pin);
         }
    }
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
                GPIO_ResetBits(out_list[i].port,out_list[i].pin);
            }
            else
            {
                GPIO_SetBits(out_list[i].port,out_list[i].pin);
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
