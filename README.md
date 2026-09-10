# Puya Blinky Test Project

Debugprobe (1) and pyOCD (2) are used to talk to the devices via the standard SWD wires (GND, SWCLK and SWD) must be connected and the debugprobe when used on rpi pico only supports 3v3. I have repurposed the old Raspberry Pi Pico I have as the debug probe. 

Then installing the pyOCD, on Windows:

    python -m pip install pyocd

Once pyOCD is installed, connect the pico debugprobe and run:

    python -m pyocd list

And you should see the debug probe (CMSIS-DAP) detected, but no target connected. To install the Puya targets, run the following:

    python -m pyocd pack install PY32F030x8

This installs the 64kb version of the chip, which is what I am using. You can verify all works by connecting the SWD header and then running the following:

    python -m pyocd gdbserver --target PY32F030x8

It should start the server without any errors. If anything is *not* working, you should see an error. 

## Installing on Linux (WSL)

    python3 -m venv ~/.python-env
    source ~/.python-env/bin/activate
    pip3 install pyocd
    sudo ~/.python-env/bin/pyocd pack install PY32F030x8

Now to be able to see the USB device, we need to make it available in the WSL, which can be done via [`usbipd-win`](https://github.com/dorssel/usbipd-win). First the device needs to be enabled as shared in administrator mode in windows. This needs to happens only once and persists reboots:

    usbipd list 
    usbipd bind --busid 1-2 # or whatever the bus id actually is

Then in order for the device to appear in WSL, this needs to be executed, can be made by normal user.

    usbipd attach --wsl --busid=1-2 # or whatever

This can be executed from WSL as well, just say:

    usbipd.exe attach --wsl --busid=1-2 # or whatever

Now the device shall be available in WSL.

In order for the USB to serial to work, the `cdc_acm` must be enabled in WSL, which by default it is on. To enable immediately, run:

    sudo modprobe cdc_acm

To enable always, add `cdc_acm` to `/etc/modules`. Feel free to ignore the warning on Ubuntu 24.04, as the file is included to the load dir. Then to inspect, run `picocom`:

    sudo picocom -b 9600 /dev/ttyACM0

(use `C-a` `C-x` to exit)

## Blinky Basic Application

First, the CMSIS must be obtained. However, the CMSIS is for the ARM Keil compiler, which is different from the GCC and so things cannot be used directly. Notably, there is no linker script (instead a scatter file) and the startup assembly routine is in a wrong syntax. You can get it from (3), but for now I have taken it from (4) and specifically (5), where it promised to already have GCC variants. But they were not working, so I started more searches and fooling around. Note that we also need the core cortex-m0plus CMSIS, which you can get from (6).

I've added to the `CMSIS` folder in the repostory the appropriate linker script as well as a startup script for the PY32F030 that I am using. 

> NOTE that those scripts are guesswork, and while they do seem to work (blinky blinks), the 100% trust is not there yet.

I have also added a simple `cmake` configuration that ensures the `arm-eabi-none` cross-compiler is used and configured correctly. To build the target, do the following: 

    mkdir build
    cd build
    cmake ..
    make 

## Flashing

    python -m pyocd flash blinky.elf --target PY32F030x7

## Things to verify

- [x] GPIO write
- [x] UART (TX is enough for debugging purposes)
- [x] RTC from internal source
- [x] RTC from external source
- [ ] debug why rtc from LSE is so flaky
- [ ] PWM (and which timers it can be generated from)
- [ ] ADC (can also try the reverse diode measurements)
- [ ] sleep and wakeup interrupts
- [ ] different clocks & power consumption
- [ ] watchdog timer
- [ ] SPI (?)
- [ ] I2C slave
- [ ] Neopixel RGB
- [ ] platform library integration

## Notes

Right now its a bit flaky and the files are not official from Puya, but from the template (5), including the Puya packs and Core CMSIS info (6). Also the packs contain extra files for other puya chips, which probably is *not* needed and can be removed.

(1) https://github.com/raspberrypi/debugprobe
(2) https://pyocd.io/
(3) https://www.keil.arm.com/packs/py32f0xx_dfp-puya/devices/
(4) https://jaycarlson.net/2023/02/04/the-cheapest-flash-microcontroller-you-can-buy-is-actually-an-arm-cortex-m0/
(5) https://github.com/jaydcarlson/py32-template
(6) https://github.com/ARM-software/CMSIS_6/releases/tag/v6.3.0





