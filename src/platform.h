#pragma once

#include <py32f0xx_ll_bus.h>
#include <py32f0xx_ll_gpio.h>
#include <py32f0xx_ll_rcc.h>
#include <py32f0xx_ll_usart.h>
#include <py32f0xx_ll_utils.h>

/** PY32F030 Platform Implementation
 */


class cpu {
public:

    /** Enables the */
    static void initialize() {

        // Enable HSI (cannot fail)
        LL_RCC_HSI_Enable();
        while(LL_RCC_HSI_IsReady() != 1);

        // Set AHB prescaler
        LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

        // Configure HSISYS as system clock source (cannot fail)
        LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSISYS);
        while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSISYS);

        // Set APB1 prescaler
        LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
        LL_Init1msTick(8000000);

        // Update system clock global variable SystemCoreClock (can also be updated by calling SystemCoreClockUpdate function)
        LL_SetSystemCoreClock(8000000);

        // enable GPIO clocks for all ports
        LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
        LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
        LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);

    }

    static void delayUs(uint16_t value) {
//        _delay_us(value);
    }

    static void delayMs(uint16_t value) {
        LL_mDelay(value);
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

    static constexpr GPIO_TypeDef * pinBank(Pin pin) {
        switch (static_cast<unsigned>(pin) >> 4) {
            case 0: return GPIOF;
            case 1: return GPIOA;
            case 2: return GPIOB;
            default: return nullptr;
        }
    }

    static constexpr uint32_t pinMask(Pin pin) {
        return 1U << (static_cast<unsigned>(pin) & 0xf);
    }

    static void setAsOutput(Pin pin) {
        LL_GPIO_SetPinMode(pinBank(pin), pinMask(pin), LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinOutputType(pinBank(pin), pinMask(pin), LL_GPIO_OUTPUT_PUSHPULL);
        LL_GPIO_SetPinSpeed(pinBank(pin), pinMask(pin), LL_GPIO_SPEED_FREQ_LOW);
    }

    static void setAsInput(Pin pin) {
        LL_GPIO_SetPinMode(pinBank(pin), pinMask(pin), LL_GPIO_MODE_INPUT);
        LL_GPIO_SetPinPull(pinBank(pin), pinMask(pin), LL_GPIO_PULL_NO);
    }

    static void setAsInputPullup(Pin pin) {
        LL_GPIO_SetPinMode(pinBank(pin), pinMask(pin), LL_GPIO_MODE_INPUT);
        LL_GPIO_SetPinPull(pinBank(pin), pinMask(pin), LL_GPIO_PULL_UP);
    }

    static void setAsInputPulldown(Pin pin) {
        LL_GPIO_SetPinMode(pinBank(pin), pinMask(pin), LL_GPIO_MODE_INPUT);
        LL_GPIO_SetPinPull(pinBank(pin), pinMask(pin), LL_GPIO_PULL_DOWN);
    }

    static void write(Pin pin, bool value) {
        if (value)
            LL_GPIO_SetOutputPin(pinBank(pin), pinMask(pin));
        else
            LL_GPIO_ResetOutputPin(pinBank(pin), pinMask(pin));
    }

    static bool read(Pin pin) {
        return LL_GPIO_IsInputPinSet(pinBank(pin), pinMask(pin));
    }

    //#include "../common/gpio_common.h"

}; 


/** Serial (USART)
 
    A very simple serial TX driver mostly for debugging purposes. Although the chip has two USARTs, only the USART1 is supported by the driver so far.
 */

class serial {
public:

    /** Returns the alternate function id for the given pin. 
     
        Puya uses weird alternate function numbers where different alternate functions have different numbers for different pins. Those are from the Puya datasheet, sections 3.1, 3.2 and 3.3. 
     */
    static constexpr unsigned txPinAlternateFunction(gpio::Pin pin) {
        switch (pin) {
            case gpio::PF1:
                return LL_GPIO_AF8_USART1;
            case gpio::PF3:
                return LL_GPIO_AF0_USART1;
            case gpio::PA2:
                return LL_GPIO_AF1_USART1;
            case gpio::PA7:
                return LL_GPIO_AF8_USART1;
            case gpio::PA9:
                return LL_GPIO_AF1_USART1;
            case gpio::PA10:
                return LL_GPIO_AF8_USART1;
            case gpio::PA14: // careful this is SWDCLK
                return LL_GPIO_AF1_USART1;
            case gpio::PB6:
                return LL_GPIO_AF0_USART1;
            case gpio::PB8:
                return LL_GPIO_AF8_USART1;
            default:
                return 0xff;
        }
    }

    static constexpr unsigned isValidTxPin(gpio::Pin pin) {
        return (txPinAlternateFunction(pin) != 0xff);
    }

    static void initializeTx(uint32_t speed, gpio::Pin txPin) {
        // ASSERT(isValidTxPin(txPin));
        // enable USART1 clock
        LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1);

        // GPIO configuration 
        LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = gpio::pinMask(txPin);
        GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
        GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
        GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
        GPIO_InitStruct.Alternate = txPinAlternateFunction(txPin);
        LL_GPIO_Init(gpio::pinBank(txPin), & GPIO_InitStruct);

        // USART configuration
        LL_USART_InitTypeDef USART_InitStruct = {0};
        USART_InitStruct.BaudRate = speed;
        USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
        USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
        USART_InitStruct.Parity = LL_USART_PARITY_NONE;
        USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX;
        USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
        USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
        LL_USART_Init(USART1, & USART_InitStruct);
        // Configure as full duplex asynchronous mode
        LL_USART_ConfigAsyncMode(USART1);
        // Enable UART module
        LL_USART_Enable(USART1);
    }

    static void waitForTx() {
        while(LL_USART_IsActiveFlag_TXE(USART1) != 1);
    }

    static void write(char c) {
        waitForTx();
        LL_USART_TransmitData8(USART1, c);
    }

    // TODO this is to be removed as the platform library itself and serial writer provide this
    static void write(char const * s) {
        while(*s) {
            write(*s++);
        }
    }

}; // serial