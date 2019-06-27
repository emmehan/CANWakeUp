# CAN WakeUp

The intendation of this project is to create a device that will act as a Wake-Up source for CAN devices.
A microcontroller ([STM32F103](https://www.st.com/en/microcontrollers/stm32f103c8.html)) sends a cyclic CAN messages using an external CAN transceiver ([TJA1050](https://www.nxp.com/docs/en/data-sheet/TJA1050.pdf)).

This project includes the EAGLE schematic and layout files among the source code for the microcontroller.

# Project structure

## Hardware
@TODO

## Software
Big shout-out to Patrik Jaworski for his [blog post](http://regalis.com.pl/en/arm-cortex-stm32-gnulinux/) about the initial bring up of the STM32Fx.
It really helped me to get back on track after a longer absence from the STM32 device family.

The software is based on the [STM32CubeF1 MCu Package ](https://www.st.com/en/embedded-software/stm32cubef1.html) from ST Microelectronis.
The application software is based on the ARM CMSIS library for the STM32F103C8T6 device.

It is developed on Linux (Arch-Linux, Kernel 4.19.2) using the GNU ARM toolchain(gcc-arm-none-eabi-*).

---

### Build

To build the firmware run:

    make MAIN=FW_NAME
in the root directory.

The `MAIN` parameter is optional and used to define the firmware name.

It defaults to `CANWakeUp`.

---

### Flash

To flash the firmare to the controller run:

    openocd -f interface/stlink-v2-1.cfg -f target/stm32f1x.cfg -c "program ./build/$FW_NAME.hex  verify reset exit"
while substitute `$FW_NAME` to the correct firmware filename.

There are preconfigured tasks for Visual Studio Code available for Building, Cleaning, Flashing and Debugging (using the [Cortex-Debug](https://marcelball.ca/projects/cortex-debug) extension by marus25).

---

### Software Documentation

To build the documentation run (requires [Doxygen](http://doxygen.nl/)):

    make doc

The documentation will then be available in `./doc/html/index.html`.

