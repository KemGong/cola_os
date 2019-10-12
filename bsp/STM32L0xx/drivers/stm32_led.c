#include "config.h"
#include "stm32l0xx_hal.h"
#include "stm32_led.h"
#include "gpio.h"
#include "cola_os.h"
#include "cola_device.h"

#define LED_GREEN_ON                  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET)
#define LED_GREEN_OFF                 HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET)
#define LED_GREEN_TOGGLE              HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin)



static cola_device_t led_dev;

static void led_gpio_init(void)
{
    MX_GPIO_Init();
}

static int led_ctrl(cola_device_t *dev, int cmd, void *args)
{
    if(LED_TOGGLE == cmd)
    {
        LED_GREEN_TOGGLE;
    }
    else  if(LED_OFF == cmd)
    {
        LED_GREEN_OFF;
    }
    else
    {
        LED_GREEN_ON;
    }
    return 1;
}


static struct cola_device_ops ops =
{
    .control = led_ctrl,
};

static void led_register(void)
{
    led_gpio_init();
    led_dev.dops = &ops;
    led_dev.name = "led";
    cola_device_register(&led_dev);
}

device_initcall(led_register);
