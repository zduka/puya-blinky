#include "platform.h"


volatile bool second = false;

extern "C" {
void RTC_IRQHandler(void)
    {
        if (LL_RTC_IsActiveFlag_SEC(RTC)) {
            LL_RTC_ClearFlag_SEC(RTC);
            second = true;
        }
    }

}


int main() {
    cpu::initialize();
    serial::initializeTx(9600, gpio::PA2);
    serial::write("init...\r\n");
    rtc::enable();
    serial::write("rtc enabled\r\n");
    rtc::enableSecondInterrupt();


    while (true) {
        // nothing
        if (second) {
            second = false;
            serial::write("  irq\r\n");
        }
    }
}