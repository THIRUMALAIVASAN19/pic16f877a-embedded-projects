#!/usr/bin/env bash
# Run this from the root of your cloned pic16f877a-embedded-projects repo.
# It creates the folder structure and moves files with git mv so history is preserved.
set -e

mkdir -p 01-i2c-eeprom 02-lcd-7segment-display 03-led-control-smart-corridor \
         04-password-protected-access 05-adc-battery-gas-sensing 06-rtc-clock \
         07-uart-communication 08-timers-pwm-interrupts docs

# 01 - I2C EEPROM
git mv I2C_EEPROM.c I2C_EEPROM.h i2c_eeprom_task.c i2c_eeprom_task.h main.c 01-i2c-eeprom/

# 02 - LCD & 7-Segment
git mv LCD_CLASS.c 7segment.c 02-lcd-7segment-display/

# 03 - LED control / smart corridor
git mv LED_CLASS.c Smart_Corridor_Light_Controller.c pull_down.c pull_up.c 03-led-control-smart-corridor/

# 04 - Password protected access
git mv Password_Protected_LED_Control.c 04-password-protected-access/

# 05 - ADC / battery / gas sensing
git mv adc_class.c adctask.c battery.c mq.c 05-adc-battery-gas-sensing/

# 06 - RTC
git mv RTC.c 06-rtc-clock/

# 07 - UART
git mv rxclass.c txclass.c 07-uart-communication/

# 08 - Timers / PWM / interrupts
git mv timer.c pwm.c intclass.c inttask.c main_task.c memory_map_switch.c 08-timers-pwm-interrupts/

echo "Done. Review with 'git status', then add per-project README.md files before committing."
