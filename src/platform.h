#pragma once

#include <py32f0xx_ll_bus.h>
#include <py32f0xx_ll_gpio.h>

/** PY32F030 Platform Implementation
 */


class cpu {
public:

    /** Enables the */
    static void initialize() {
        /* Enable HSI */
        LL_RCC_HSI_Enable();
        while(LL_RCC_HSI_IsReady() != 1)
        {
        }

        /* Set AHB prescaler */
        LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

        /* Configure HSISYS as system clock source */
        LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSISYS);
        while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSISYS)
        {
        }

        /* Set APB1 prescaler */
        LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
        LL_Init1msTick(8000000);

        /* Update system clock global variable SystemCoreClock (can also be updated by calling SystemCoreClockUpdate function) */
        LL_SetSystemCoreClock(8000000);

    }

    static void delayUs(uint16_t value) {
//        _delay_us(value);
    }

    static void delayMs(uint16_t value) {
/*
        while (value-- != 0) {
            // make sure we reset the wdt before cycles for more robustness
            __asm__ __volatile__ ("wdr"::);
            _delay_ms(1);
        }
*/
    }

    static void sleep() {
/*
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        sleep_cpu();
*/
    }

    static void reset() {
        // reset the device by writing to the software reset register
    }

    static void wdtReset() __attribute__((always_inline)) {
//        __asm__ __volatile__ ("wdr"::);
    }

    static void cli() __attribute__((always_inline)) { 
//        __asm__ __volatile__ ("cli"::);
    }

    static void sei()  __attribute__((always_inline)){ 
//        __asm__ __volatile__ ("sei"::);
    }

    static void nop()  __attribute__((always_inline)) { __NOP(); }

//    #include "../common/cpu_common.h"

};


class gpio {
public:

  enum class Pin {
        PF0 = 0x00, 
        PF1,
        PF2,
        PF3,
        PF4,
        PA0 = 0x10,
        PA1, 
        PA2, 
        PA3,
        PA4,
        PA5,
        PA6,
        PA7,
        PA8,
        PA9,
        PA10,
        PA11,
        PA12,
        PA13,
        PA14,
        PA15,
        PB0 = 0x20,
        PB1,
        PB2,
        PB3,
        PB4,
        PB5,
        PB6,
        PB7,
        PB8,
        Unused = 0xff
    };

    static constexpr Pin PF0 = Pin::PF0;
    static constexpr Pin PF1 = Pin::PF1;
    static constexpr Pin PF2 = Pin::PF2;
    static constexpr Pin PF3 = Pin::PF3;
    static constexpr Pin PF4 = Pin::PF4;
    static constexpr Pin PA0 = Pin::PA0;
    static constexpr Pin PA1 = Pin::PA1;
    static constexpr Pin PA2 = Pin::PA2;
    static constexpr Pin PA3 = Pin::PA3;
    static constexpr Pin PA4 = Pin::PA4;
    static constexpr Pin PA5 = Pin::PA5;
    static constexpr Pin PA6 = Pin::PA6;
    static constexpr Pin PA7 = Pin::PA7;
    static constexpr Pin PA8 = Pin::PA8;
    static constexpr Pin PA9 = Pin::PA9;
    static constexpr Pin PA10 = Pin::PA10;
    static constexpr Pin PA11 = Pin::PA11;
    static constexpr Pin PA12 = Pin::PA12;
    static constexpr Pin PA13 = Pin::PA13;
    static constexpr Pin PA14 = Pin::PA14;
    static constexpr Pin PA15 = Pin::PA15;
    static constexpr Pin PB0 = Pin::PB0;
    static constexpr Pin PB1 = Pin::PB1;
    static constexpr Pin PB2 = Pin::PB2;
    static constexpr Pin PB3 = Pin::PB3;
    static constexpr Pin PB4 = Pin::PB4;
    static constexpr Pin PB5 = Pin::PB5;
    static constexpr Pin PB6 = Pin::PB6;
    static constexpr Pin PB7 = Pin::PB7;
    static constexpr Pin PB8 = Pin::PB8;
    static constexpr Pin UNUSED = Pin::Unused;

    #define GPIO_BANK_ID(PIN) (static_cast<unsigned>(PIN) >> 4)
    #define GPIO_BANK(PIN) GPIO_BANK_ID(PIN) == 0 ? GPIOA : GPIO_BANK_ID(PIN) == 1 ? GPIOB : GPIO_BANK_ID(PIN) == 2 ? GPIOF : nullptr
    #define GPIO_PIN(PIN) (1U << (static_cast<unsigned>(PIN)))

    static void initialize() {
        LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
        LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
        LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
    }

    static void setAsOutput(Pin pin) {
        LL_GPIO_SetPinMode(GPIO_BANK(pin), GPIO_PIN(pin), LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinOutputType(GPIO_BANK(pin), GPIO_PIN(pin), LL_GPIO_OUTPUT_PUSHPULL);
        LL_GPIO_SetPinSpeed(GPIO_BANK(pin), GPIO_PIN(pin), LL_GPIO_SPEED_FREQ_LOW);
    }

    static void setAsInput(Pin pin) {
        LL_GPIO_SetPinMode(GPIO_BANK(pin), GPIO_PIN(pin), LL_GPIO_MODE_INPUT);
        LL_GPIO_SetPinPull(GPIO_BANK(pin), GPIO_PIN(pin), LL_GPIO_PULL_NO);
    }

    static void setAsInputPullup(Pin pin) {
        LL_GPIO_SetPinMode(GPIO_BANK(pin), GPIO_PIN(pin), LL_GPIO_MODE_INPUT);
        LL_GPIO_SetPinPull(GPIO_BANK(pin), GPIO_PIN(pin), LL_GPIO_PULL_UP);
    }

    static void setAsInputPulldown(Pin pin) {
        LL_GPIO_SetPinMode(GPIO_BANK(pin), GPIO_PIN(pin), LL_GPIO_MODE_INPUT);
        LL_GPIO_SetPinPull(GPIO_BANK(pin), GPIO_PIN(pin), LL_GPIO_PULL_DOWN);
    }

    static void write(Pin pin, bool value) {
        if (value)
            LL_GPIO_SetOutputPin(GPIO_BANK(pin), GPIO_PIN(pin));
        else
            LL_GPIO_ResetOutputPin(GPIO_BANK(pin), GPIO_PIN(pin));
    }

    static bool read(Pin pin) {
        return LL_GPIO_IsInputPinSet(GPIO_BANK(pin), GPIO_PIN(pin));
    }

    //#include "../common/gpio_common.h"

}; 