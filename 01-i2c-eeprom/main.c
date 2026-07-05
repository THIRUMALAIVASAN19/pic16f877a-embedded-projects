// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.




#include <xc.h>
#include "I2C_EEPROM.h"
#define _XTAL_FREQ 16000000

void main(void) {

    // Initialize I2C Master with 100kHz clock
    I2C_Master_Init(100000);   

    unsigned int Address = 0x0020;        // EEPROM memory start address
    unsigned char Data = 0x04;            // Data to be written to EEPROM

    // Write three bytes sequentially into EEPROM starting at address 0x0020
    EEPROM_Write(Address++, Data++);     // Write 0x04 at 0x0020, then increment Address and Data
    EEPROM_Write(Address++, Data++);     // Write 0x05 at 0x0021, then increment Address and Data
    EEPROM_Write(Address, Data);         // Write 0x06 at 0x0022

    __delay_ms(10);                      // Wait 10 ms to ensure EEPROM write cycle completes (tWR)

    Address = 0x0020;                    // Reset address to beginning (0x0020) for reading

    TRISD = 0x00;                        // Set PORTD as output to display EEPROM read data

    PORTD = EEPROM_Read(Address++);      // Read data from 0x0020 (should be 0x04) and output to PORTD
    __delay_ms(1000);                    // Wait for 1 second

    PORTD = EEPROM_Read(Address++);      // Read data from 0x0021 (should be 0x05) and output to PORTD
    __delay_ms(1000);                    // Wait for 1 second

    PORTD = EEPROM_Read(Address);        // Read data from 0x0022 (should be 0x06) and output to PORTD

    while(1)
    {
        // Infinite loop to hold the program
    }

    return;
}