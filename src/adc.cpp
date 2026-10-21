#include "platform.h"
#include <py32f0xx_ll_adc.h>

static void writeNumber(uint32_t value) {
    char buf[11];
    char * s = buf + sizeof(buf) - 1;
    *s = 0;
    do {
        *--s = '0' + (value % 10);
        value /= 10;
    } while (value != 0);
    serial::write(s);
}

int main() {
    cpu::initialize();
    serial::initializeTx(9600, gpio::PA2);
    serial::write("adc init...\r\n");

    // PB0 is ADC_IN8, set it to analog mode
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);

    // enable ADC clock & basic configuration (all of this must be done while the ADC is disabled)
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_ADC1);
    LL_ADC_SetClock(ADC1, LL_ADC_CLOCK_SYNC_PCLK_DIV4);
    LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_8B);
    LL_ADC_SetDataAlignment(ADC1, LL_ADC_DATA_ALIGN_RIGHT);
    LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_239CYCLES_5);
    // single conversion of channel 0, triggered by software
    LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);
    LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
    LL_ADC_REG_SetSequencerChannels(ADC1, LL_ADC_CHANNEL_8); // PB0

    // calibrate, then wait a bit before enabling the ADC
    LL_ADC_StartCalibration(ADC1);
    while (LL_ADC_IsCalibrationOnGoing(ADC1));
    cpu::delayMs(1);

    while (true) {
        // when single conversion ends, the ADC is turned off, so we have to turn it on every time again and wait a short while for it to stabilize
        LL_ADC_Enable(ADC1);
        cpu::delayUs(100);
        LL_ADC_REG_StartConversion(ADC1);
        while (LL_ADC_IsActiveFlag_EOC(ADC1) == 0);
        uint16_t value = LL_ADC_REG_ReadConversionData12(ADC1);
        LL_ADC_ClearFlag_EOC(ADC1);
        serial::write("  adc: ");
        writeNumber(value);
        serial::write("\r\n");
        cpu::delayMs(50);
    }
}
