#include "platform.h"

int main() {
    cpu::initialize();
    serial::initializeTx(9600, gpio::PA2);
    serial::write("Hello, USART, this is rckid!\n\r");
    serial::write("So USART seems to work...\n\r");
    while (true);
}