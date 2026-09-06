#include "py32f030x8.h"

static void delay(volatile uint32_t t)
{
    while (t--) __NOP();
}

int main(void)
{
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
}
