
// PIC16F877A Configuration Bits


// Configuration for oscillator and basic hardware features
#pragma config FOSC = HS        // High-Speed Oscillator
#pragma config WDTE = OFF       // Watchdog Timer disabled
#pragma config PWRTE = OFF      // Power-up Timer disabled
#pragma config BOREN = OFF      // Brown-out Reset disabled
#pragma config LVP = OFF        // Low-voltage programming disabled (RB3 used as digital I/O)
#pragma config CPD = OFF        // Data EEPROM Code Protection off
#pragma config WRT = OFF        // Flash memory write protection off
#pragma config CP = OFF         // Code protection off


//        Include Files


#include <xc.h>                     // XC8 compiler header for PIC
#include "i2c_eeprom_task.h"       // Custom header file for I2C & EEPROM routines

#define _XTAL_FREQ 16000000        // Define system clock frequency for delay functions


//         Main Function


void main(void) {

    // Initialize I2C Master with 100kHz clock
    I2C_Master_INTIALIZATION(100000);
    
    // Write characters to EEPROM at different addresses
    EEPROM_Write(0x0023, 'A');     // Write character 'A' to EEPROM address 0x0023
    EEPROM_Write(0x0028, 'B');     // Write character 'B' to EEPROM address 0x0028
    EEPROM_Write(0x0036, 'C');     // Write character 'C' to EEPROM address 0x0036
    __delay_ms(10);                // Delay to allow EEPROM write to complete
    
    TRISD = 0x00;                  // Set PORTD as output to display data on LEDs or other output device
    
    // Read back values from EEPROM and display on PORTD
    PORTD = EEPROM_Read(0x0023);   // Read from 0x0023, should show 'A'
    __delay_ms(2000);              // Wait 2 seconds
    
    PORTD = EEPROM_Read(0x0028);   // Read from 0x0028, should show 'B'
    __delay_ms(2000);              // Wait 2 seconds
    
    PORTD = EEPROM_Read(0x0036);   // Read from 0x0036, should show 'C'
    
    while(1)
    {
        // Infinite loop to keep microcontroller running
    }

    return;
}