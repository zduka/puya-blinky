#include "py32f030x8.h"

#include "py32f0xx_ll_bus.h"
#include "py32f0xx_ll_gpio.h"

#include "py32f0xx_ll_rcc.h"
#include "py32f0xx_ll_utils.h"
#include "py32f0xx_ll_rtc.h"
#include "py32f0xx_ll_pwr.h"
#include "py32f0xx_ll_exti.h"

static void delay(volatile uint32_t t) {
    while (t--) __NOP();
}

int main() {

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
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1);

    while (1)
    {
        LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_0);
        LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_1);
        delay(100000);
    }
#ifdef HAHA
    // older CMSIS alone version of the same code with the LL driver
    
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

