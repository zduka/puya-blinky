#include "py32f030x8.h"

#include "py32f0xx_ll_bus.h"
#include "py32f0xx_ll_gpio.h"

#include "py32f0xx_ll_rcc.h"
#include "py32f0xx_ll_rtc.h"
#include "py32f0xx_ll_pwr.h"
#include "py32f0xx_ll_exti.h"

static void delay(volatile uint32_t t) {
    while (t--) __NOP();
}

volatile bool state = false;

static void RTC_Init(void) {
    LL_RTC_InitTypeDef rtc_init = {0};
    LL_RTC_TimeTypeDef rtc_time = {0};

    // return;

    /* Enable PWR and backup domain */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
    LL_PWR_EnableBkUpAccess();

    /* Enable external 32.768 kHz oscillator */
    LL_RCC_LSE_Enable();
    while (!LL_RCC_LSE_IsReady());

    //LL_RCC_LSI_Enable();
    //while (!LL_RCC_LSI_IsReady());

    /* Select LSE as RTC clock */
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);

    /* Reset backup domain (recommended when switching clock source) */
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();

    // return;

    /* Enable RTC */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_RTC);
    LL_RCC_EnableRTC();

    // return;

    /* Configure prescalers for 1 Hz */
    rtc_init.AsynchPrescaler = 0x7FFF;   // Puya uses 15-bit async prescaler
    rtc_init.OutPutSource    = LL_RTC_CALIB_OUTPUT_NONE;

    // return;

    LL_RTC_DeInit(RTC);

    return;

    LL_RTC_Init(RTC, &rtc_init);

    return;

    /* Set initial time (optional) */
    rtc_time.Hours   = 0;
    rtc_time.Minutes = 0;
    rtc_time.Seconds = 0;
    LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BIN, &rtc_time);

    return;

    /* Enable second interrupt */
    LL_RTC_EnableIT_SEC(RTC);

    /* Enable EXTI line 19 */
    LL_EXTI_EnableIT(LL_EXTI_LINE_19);

    NVIC_SetPriority(RTC_IRQn, 0);
    NVIC_EnableIRQ(RTC_IRQn);
}

void RTC_IRQHandler(void)
{
    if (LL_RTC_IsActiveFlag_SEC(RTC)) {
        LL_RTC_ClearFlag_SEC(RTC);
        /* your second tick logic */
        LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_1);
    }
}


int main(void)
{

// first an attempt to use the LL driver functions to do what I did with CMSIS below

    /* Enable GPIOA and GPIOB clocks */
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);

    /* Configure PA0 as push‑pull output */
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_0, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_LOW);

    /* Configure PB1 as push‑pull output */
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_1, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_1, LL_GPIO_SPEED_FREQ_LOW);

    // PB1 is off by default, only the RTC interrupt enables it
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_1);

    /* Enable RTC interrupt */
    //LL_RTC_EnableIT_SEC(RTC);
    RTC_Init();

    while (1)
    {
        LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_0);   // PA0 ON
        //LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_1); // PB1 OFF
        delay(200000);

        LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_0); // PA0 OFF
        //LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1);   // PB1 ON
        delay(200000);
    }
#ifdef HAHA
    
    /* Enable GPIOA clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;


    /* Configure PA0 as push‑pull output
       MODER bits: 01 = output */
    GPIOA->MODER &= ~(3U << (0 * 2));
    GPIOA->MODER |=  (1U << (0 * 2));

    /* Configure PB1 as push‑pull output
       MODER bits: 01 = output */
    GPIOB->MODER &= ~(3U << (1 * 2));
    GPIOB->MODER |=  (1U << (1 * 2));



    // and make PB1 always on:

    GPIOB->ODR |=  (1U << 1);   // LED ON

    while (1)
    {
        GPIOA->ODR |=  (1U << 0);   // LED ON
        GPIOB->ODR &= ~(1U << 1);   // LED OFF
        delay(200000);

        GPIOA->ODR &= ~(1U << 0);   // LED OFF
        GPIOB->ODR |=  (1U << 1);   // LED ON
        delay(200000);
    }
#endif
}

