#include "config.h"
#include "gd32e23x.h"
#include "gd32_adc.h"
#include "cola_os.h"
#include "cola_device.h"
#include "cola_fifo.h"
#include "string.h"

#ifdef USING_ADC


static cola_device_t adc_dev;
uint16_t adc_value[5];

static int adc_read(cola_device_t *dev, int pos, void *buffer, int size)
{
    memcpy(buffer, adc_value, sizeof(adc_value));
    return size;
}

/*!
    \brief      configure the GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_gpio_config(void)
{
  /* config the GPIO as analog mode */
  gpio_mode_set(ADC_GPIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_GPIO_PIN_3);
  gpio_mode_set(ADC_GPIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_GPIO_PIN_4);
  gpio_mode_set(ADC_GPIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_GPIO_PIN_5);
  gpio_mode_set(ADC_GPIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_GPIO_PIN_6);
  gpio_mode_set(ADC_GPIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_GPIO_PIN_7);
}

/*!
    \brief      configure the different system clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_rcu_config(void)
{
  /* enable GPIOA clock */
  rcu_periph_clock_enable(RCU_GPIOA);
  /* enable ADC clock */
  rcu_periph_clock_enable(RCU_ADC);
  /* enable DMA clock */
  rcu_periph_clock_enable(RCU_DMA);
  /* config ADC clock */
  rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6);//
}

/*!
    \brief      configure the DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(void)
{
  /* ADC_DMA_channel configuration */
  dma_parameter_struct dma_data_parameter;

  /* ADC DMA_channel configuration */
  dma_deinit(DMA_CH0);

  /* initialize DMA single data mode */
  dma_data_parameter.periph_addr  = (uint32_t)(&ADC_RDATA);
  dma_data_parameter.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
  dma_data_parameter.memory_addr  = (uint32_t)(&adc_value);
  dma_data_parameter.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
  dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
  dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;
  dma_data_parameter.direction    = DMA_PERIPHERAL_TO_MEMORY;
  dma_data_parameter.number       = 5U;
  dma_data_parameter.priority     = DMA_PRIORITY_HIGH;
  dma_init(DMA_CH0, &dma_data_parameter);

  dma_circulation_enable(DMA_CH0);

  /* enable DMA channel */
  dma_channel_enable(DMA_CH0);
}

/*!
    \brief      configure the ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_config(void)
{
  adc_special_function_config(ADC_CONTINUOUS_MODE, ENABLE);//todo:开机连续转换跟扫描？
  adc_special_function_config(ADC_SCAN_MODE, ENABLE);
  /* ADC trigger config */
  adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE);
  /* ADC data alignment config */
  adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
  /* ADC分辨率 12B */
  adc_resolution_config(ADC_RESOLUTION_12B);
  /* ADC channel length config */
  adc_channel_length_config(ADC_REGULAR_CHANNEL, 5U);

  /* ADC regular channel config */
  adc_regular_channel_config(0U, BOARD_ADC_CHANNEL_3, ADC_SAMPLETIME_55POINT5);
  adc_regular_channel_config(1U, BOARD_ADC_CHANNEL_4, ADC_SAMPLETIME_55POINT5);
  adc_regular_channel_config(2U, BOARD_ADC_CHANNEL_5, ADC_SAMPLETIME_55POINT5);
  adc_regular_channel_config(3U, BOARD_ADC_CHANNEL_6, ADC_SAMPLETIME_55POINT5);
  adc_regular_channel_config(4U, BOARD_ADC_CHANNEL_7, ADC_SAMPLETIME_55POINT5);
  adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE);

  /* enable ADC interface */
  adc_enable();
  //delay_1ms(1U);//todo
  /* ADC calibration and reset calibration */
  adc_calibration_enable();

  /* ADC DMA function enable */
  adc_dma_mode_enable();
  /* ADC software trigger enable */
  adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
}

void adc_init(void)
{
    adc_rcu_config();
    adc_gpio_config();
    dma_config();
    adc_config();
}


static struct cola_device_ops adc_ops =
{
    // .write  = uart0_write,
    .read   = adc_read,
    // .config = uart0_config,
};


void board_setup_adc(void)
{
#ifdef USING_ADC
    adc_init();
    adc_dev.name = "adc";
    adc_dev.dops = &adc_ops;
    cola_device_register(&adc_dev);
#endif

}
device_initcall(board_setup_adc);

#endif
