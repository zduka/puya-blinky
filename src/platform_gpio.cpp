#include "platform.h"

int main() {
    cpu::initialize();
    gpio::setAsOutput(gpio::PA0);
    gpio::setAsOutput(gpio::PB1);
    gpio::setAsInputPullup(gpio::PB2);

    while (true) {
        if (gpio::read(gpio::PB2)) {
            gpio::write(gpio::PA0, true);
            gpio::write(gpio::PB1, false);
        } else {
            gpio::write(gpio::PA0, false);
            gpio::write(gpio::PB1, true);
        }
    }
}