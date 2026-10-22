#include "platform.h"
#include <py32f0xx_ll_spi.h>

/** Neopixels (WS2812B) driven by SPI1 MOSI on PA2.

    The SPI clock is not used at all, we only abuse the MOSI line to generate the neopixel waveform. With the CPU (and APB) running at 24MHz and SPI prescaler of 8, the SPI clock is 3MHz, i.e. 333ns per SPI bit. Each neopixel bit is then encoded as 4 SPI bits (1.33us, ~750kHz, close enough to 800kHz):

        0 -> 1000 (T0H = 333ns, T0L = 1000ns)
        1 -> 1100 (T1H = 667ns, T1L =  667ns)

    So every neopixel byte becomes 32 SPI bits, which we send as two 16bit SPI frames. Since every encoded bit ends with 0, MOSI stays low when the transfer is over, which is what the reset (latch) pulse needs.

    The SPI has a TX FIFO and a 16bit frame takes 5.33us (128 CPU cycles) to shift out, so polling is plenty fast. Interrupts during the update could stretch the low part of a bit and if long enough would be seen as reset by the neopixels, so keep them short or disabled during the update.
 */

static constexpr uint32_t NUM_PIXELS = 5;

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

static Color pixels[NUM_PIXELS];

/** Expands 4 neopixel bits (upper nibble first) into a single 16bit SPI frame.
 */
static uint16_t encodeNibble(uint8_t nibble) {
    uint16_t result = 0;
    for (int i = 3; i >= 0; --i) {
        result <<= 4;
        result |= (nibble & (1 << i)) ? 0b1100 : 0b1000;
    }
    return result;
}

static void sendByte(uint8_t value) {
    uint16_t hi = encodeNibble(value >> 4);
    uint16_t lo = encodeNibble(value & 0xf);
    while (LL_SPI_IsActiveFlag_TXE(SPI1) == 0);
    LL_SPI_TransmitData16(SPI1, hi);
    while (LL_SPI_IsActiveFlag_TXE(SPI1) == 0);
    LL_SPI_TransmitData16(SPI1, lo);
}

static void initializeNeopixels() {
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SPI1);

    // PA2 is SPI1_MOSI on AF0 (datasheet, section 3.1)
    LL_GPIO_InitTypeDef gpioCfg = {};
    gpioCfg.Pin = LL_GPIO_PIN_2;
    gpioCfg.Mode = LL_GPIO_MODE_ALTERNATE;
    gpioCfg.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    gpioCfg.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpioCfg.Pull = LL_GPIO_PULL_DOWN;
    gpioCfg.Alternate = LL_GPIO_AF0_SPI1;
    LL_GPIO_Init(GPIOA, & gpioCfg);

    // master, transmit only (half duplex TX so that we do not have to care about RX overruns), 16bit frames, 24MHz / 8 = 3MHz
    LL_SPI_InitTypeDef spiCfg = {};
    spiCfg.TransferDirection = LL_SPI_HALF_DUPLEX_TX;
    spiCfg.Mode = LL_SPI_MODE_MASTER;
    spiCfg.DataWidth = LL_SPI_DATAWIDTH_16BIT;
    spiCfg.ClockPolarity = LL_SPI_POLARITY_LOW;
    spiCfg.ClockPhase = LL_SPI_PHASE_1EDGE;
    spiCfg.NSS = LL_SPI_NSS_SOFT;
    spiCfg.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV8;
    spiCfg.BitOrder = LL_SPI_MSB_FIRST;
    LL_SPI_Init(SPI1, & spiCfg);
    LL_SPI_Enable(SPI1);
}

/** Sends the pixel buffer to the neopixels. Note that neopixels expect the colors in GRB order.
 */
static void updateNeopixels() {
    for (uint32_t i = 0; i < NUM_PIXELS; ++i) {
        sendByte(pixels[i].g);
        sendByte(pixels[i].r);
        sendByte(pixels[i].b);
    }
    // wait for the last frame to be shifted out
    while (LL_SPI_GetTxFIFOLevel(SPI1) != LL_SPI_TX_FIFO_EMPTY);
    while (LL_SPI_IsActiveFlag_BSY(SPI1));
    // reset (latch) pulse, newer WS2812B need > 280us of low
    cpu::delayMs(1);
}

int main() {
    // 24MHz is needed for the 3MHz SPI clock
    cpu::initialize(cpu::ClockSpeed::MHz24);
    initializeNeopixels();

    uint32_t step = 0;
    while (true) {
        // a single dim (to save eyes & power) dot of changing color running across the strip
        for (uint32_t i = 0; i < NUM_PIXELS; ++i)
            pixels[i] = Color{0, 0, 0};
        uint32_t pos = step % NUM_PIXELS;
        switch ((step / NUM_PIXELS) % 3) {
            case 0: pixels[pos] = Color{32, 0, 0}; break;
            case 1: pixels[pos] = Color{0, 32, 0}; break;
            case 2: pixels[pos] = Color{0, 0, 32}; break;
        }
        updateNeopixels();
        ++step;
        cpu::delayMs(200);
    }
}
