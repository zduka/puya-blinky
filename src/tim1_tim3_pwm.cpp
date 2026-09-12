#include "platform.h"

int main() {
    cpu::initialize();
    timer1::enable();
    gpio::configureAsAlternate(gpio::Pin::PA8, gpio::AlternateFunction::TIM1_CH1);
    gpio::configureAsAlternate(gpio::Pin::PB3, gpio::AlternateFunction::TIM1_CH2);
    gpio::configureAsAlternate(gpio::Pin::PB6, gpio::AlternateFunction::TIM1_CH3);

    gpio::configureAsAlternate(gpio::Pin::PA2, gpio::AlternateFunction::TIM3_CH1);
    gpio::configureAsAlternate(gpio::Pin::PA5, gpio::AlternateFunction::TIM3_CH2);
    gpio::configureAsAlternate(gpio::Pin::PA4, gpio::AlternateFunction::TIM3_CH3);

    timer1::enableChannelPWM(1, 64);
    timer1::enableChannelPWM(2, 128);
    timer1::enableChannelPWM(3, 192);

    timer3::enable(256, 2);

    timer3::enableChannelPWM(1, 64);
    timer3::enableChannelPWM(2, 128);
    timer3::enableChannelPWM(3, 192);

    timer3::disableChannel(1);

    gpio::setAsOutput(gpio::Pin::PA0);
    gpio::write(gpio::Pin::PA0, true);

    while(1) {
        // Do nothing
    }
}