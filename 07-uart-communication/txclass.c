
// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
//------------------------- TX CODE ------------------------------
#include <xc.h>
#define _XTAL_FREQ 6000000
unsigned char value,rx_reg;// global variable
void uart_init();// function to initialize 
void main() {
    uart_init();// initialized UART
    while(1)
    {
        value = PORTB;
        switch(value)
        {
            case 0xE0: TXREG='A';// transmitted data A
                       __delay_ms(100);
            break;
            case 0xD0: TXREG='B';// transmitted data B
                       __delay_ms(100);
            break;
            case 0xB0: TXREG='C';// transmitted data C
                       __delay_ms(100);
            break;
            case 0x70: TXREG='D';// transmitted data D
                       __delay_ms(100);
            break;
        }    
    } 
}
void uart_init()
{
    // pin configuration
    TRISB=0xF0;// PORTB as output
    PORTB=0x00;// initialize RB0 to RB7 is low
    
    OPTION_REG&=0x7F;// internal pull up on
    // UART configuration
    TRISC=0xC0;//1100 0000 RC(TX) =1 RC7(RX)=1
    TXSTA=0x20;// enable tranmission (TXREN =1),Asynchronous mode
    SPBRG=0x09;// baud rate setting for 9600 @6MHz
    RCSTA=0x90;// enable serial port and continuous receive 
    
}


  



