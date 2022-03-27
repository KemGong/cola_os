#include "config.h"
#include "stm32l0xx_hal.h"
#include "stm32_sleep.h"
#include "cola_os.h"
#include "cola_device.h"
#include "rtc.h"
#include "bsp.h"

static cola_device_t sleep_dev;


void board_sleep_init(void)
{
    MX_RTC_Init();
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    HAL_PWR_DisablePVD();
    HAL_PWREx_EnableUltraLowPower();
    HAL_PWREx_EnableFastWakeUp();
    __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_HSI);

}

extern void bsp_rcc(void);
void board_exit_sleep(void)
{
    if(__HAL_PWR_GET_FLAG(PWR_FLAG_WU) != RESET)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);	
	}
	if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB); 
	}
    bsp_rcc();
    __GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
}


void board_enter_sleep(uint16_t time)
{

    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();

    HAL_DBGMCU_DisableDBGStopMode();

    HAL_DBGMCU_DBG_DisableLowPowerConfig(DBGMCU_SLEEP | DBGMCU_STOP | DBGMCU_STANDBY);
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);	
    HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, time, RTC_WAKEUPCLOCK_RTCCLK_DIV16);

    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
    board_exit_sleep();
      
}
void board_enter_deep_sleep(uint16_t time)
{

	__HAL_RCC_GPIOA_CLK_DISABLE();
	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOH_CLK_DISABLE();

	HAL_DBGMCU_DBG_DisableLowPowerConfig(DBGMCU_SLEEP | DBGMCU_STOP | DBGMCU_STANDBY);
    
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

	HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, time, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
	/* Enter Stop Mode */

	HAL_PWR_EnterSTANDBYMode();
      
}
static int sleep_ctrl(cola_device_t *dev, int cmd, void *args)
{
    int time = (int)args;
     time<<=10;
    if(DEV_STOP == cmd)
    {
       
        board_enter_sleep(time);
    }
    else if(DEV_STANDBY == cmd)
    {
        board_enter_deep_sleep(time);
    }
    return 0;
}
static struct cola_device_ops ops =
{
    .control = sleep_ctrl,
};

void RTC_IRQHandler(void)
{
	/* USER CODE BEGIN RTC_IRQn 0 */

	/* USER CODE END RTC_IRQn 0 */
	HAL_RTCEx_WakeUpTimerIRQHandler(&hrtc);
	/* USER CODE BEGIN RTC_IRQn 1 */

	/* USER CODE END RTC_IRQn 1 */
}

void board_setup_sleep(void)
{
    board_sleep_init();
    sleep_dev.name = "sleep";
    sleep_dev.dops = &ops;
    cola_device_register(&sleep_dev);

}
device_initcall(board_setup_sleep);
