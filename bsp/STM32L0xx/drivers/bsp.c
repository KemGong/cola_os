#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "bsp.h"
#include "stm32l0xx.h"
#include "stm32l0xx_hal.h"
#include "cola_init.h"
#include "cola_os.h"
#include "cola_device.h"

//ÊÊÅä
void Error_Handler(void)
{
    //os_log("Error_Handler\n");
}
void HardFault_Handler(void)
{
    //os_log("HardFault_Handler\n");
}
void bsp_rcc(void)
{
    HAL_Init();

    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Configure the main internal regulator output voltage
    */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
    RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
    Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
    Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_RTC;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
    Error_Handler();
    }
}
pure_initcall(bsp_rcc);
void SysTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */
    cola_timer_ticker();
    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    /* USER CODE BEGIN SysTick_IRQn 1 */

    /* USER CODE END SysTick_IRQn 1 */
}
void cola_show_version(void)
{    
    os_log("\n \\ | /\n");    
    os_log("- cola -     Ver:%d.%d.%d\n",COLA_VERSION, COLA_SUBVERSION, COLA_REVISION);    
    os_log(" / | \\ \n");    
}

void bsp_init(void)
{
    do_init_call();
    
}
void bsp_app_init(void)
{
    do_app_init_call();
    cola_show_version();
}

void bsp_restart(void)
{
     NVIC_SystemReset();
}
uint32_t bsp_get_radom(uint32_t range_min,uint32_t range_max)
{
    uint32_t seed = jiffies;
    if(range_max==range_min)
    {
        return range_min;
    }
    if(range_min > range_max)
    {
        return 0;
    }
    srand((unsigned)seed);
    return rand() % (range_max-range_min) + range_min;
}

void bsp_mdelay(unsigned long ms)
{
    ms *= 32000;
    for(; ms != 0; ms--);
}
