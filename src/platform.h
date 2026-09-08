#pragma once

#include <py32f0xx_ll_bus.h>
#include <py32f0xx_ll_gpio.h>

/** PY32F030 Platform Implementation
 */

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

    static constexpr Pin UNUSED = Pin::Unused;

    static void initialize() {
        LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
        LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
        LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
    }

    static void setAsOutput(Pin pin) {

    }

    static void setAsInput(Pin pin) {

    }

    static void setAsInputPullup(Pin pin) {

    }

    static void write(Pin pin, bool value) {

    }

    static bool read(Pin pin) {

    }

    //#include "../common/gpio_common.h"

}; 