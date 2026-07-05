# PIC16F877A Embedded Firmware Projects

A collection of bare-metal C firmware modules for the **PIC16F877A** 8-bit microcontroller, demonstrating register-level peripheral control across GPIO, ADC, Timers, PWM, UART, I²C, and interrupt-driven design.

Each project is self-contained with its own source files and a short README covering the hardware setup and what it demonstrates.

---

## Project Index

| # | Project | Peripherals / Concepts | Folder |
|---|---------|------------------------|--------|
| 01 | I²C EEPROM Read/Write | I²C master, external EEPROM, page read/write | [`01-i2c-eeprom`](./01-i2c-eeprom) |
| 02 | LCD & 7-Segment Display | Parallel LCD driver, multiplexed 7-segment display | [`02-lcd-7segment-display`](./02-lcd-7segment-display) |
| 03 | LED Control & Smart Corridor Light | GPIO, ADC-based ambient light sensing | [`03-led-control-smart-corridor`](./03-led-control-smart-corridor) |
| 04 | Password-Protected Access Control | Keypad/switch input, state machine, GPIO output | [`04-password-protected-access`](./04-password-protected-access) |
| 05 | ADC Sensing: Battery & Gas (MQ) | ADC sampling, task-based polling, sensor scaling | [`05-adc-battery-gas-sensing`](./05-adc-battery-gas-sensing) |
| 06 | Real-Time Clock (RTC) | I²C RTC driver, timekeeping | [`06-rtc-clock`](./06-rtc-clock) |
| 07 | UART Communication | USART TX/RX, interrupt-driven serial | [`07-uart-communication`](./07-uart-communication) |
| 08 | Timers, PWM & Interrupts | Timer configuration, PWM generation, ISR handling | [`08-timers-pwm-interrupts`](./08-timers-pwm-interrupts) |

---

## Hardware & Tools

- **MCU:** PIC16F877A (40-pin DIP), 16 MHz crystal, HS oscillator mode
- **IDE:** [MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide)
- **Compiler:** Microchip XC8
- **Simulation:** Proteus Design Suite (optional, for pre-hardware verification)
- **Programmer:** PICkit 3/4

## Building & Flashing

1. Open the desired project folder in MPLAB X IDE as a standalone project, or create a new PIC16F877A project and add the folder's `.c`/`.h` files.
2. Set the device to **PIC16F877A** and the compiler toolchain to **XC8**.
3. Build (`Build Project`) and program via PICkit.
4. Configuration bits (`FOSC`, `WDTE`, `PWRTE`, etc.) are set via `#pragma config` at the top of each project's main file — adjust `_XTAL_FREQ` if your oscillator differs from 16 MHz.

## Repository Structure

```
pic16f877a-embedded-projects/
├── README.md
├── LICENSE
├── 01-i2c-eeprom/
├── 02-lcd-7segment-display/
├── 03-led-control-smart-corridor/
├── 04-password-protected-access/
├── 05-adc-battery-gas-sensing/
├── 06-rtc-clock/
├── 07-uart-communication/
├── 08-timers-pwm-interrupts/
└── docs/
    └── schematics, pinout diagrams, demo images
```

## License

This project is licensed under the MIT License — see [LICENSE](./LICENSE) for details.
