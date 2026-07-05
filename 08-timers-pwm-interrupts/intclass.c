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

#include <xc.h>
#define _XTAL_FREQ 6000000

unsigned char value;
void main()
{
    TRISB=0xF0;// 1111 0000 RB4 to RB7 is input and RB0 to RB3 is output
    PORTB=0x00;// 0000 0000 RB0 to RB7 is low 
    TRISC=0x00;// 0000 0000 RC0 to RC7 is output
    PORTC=0x00;// 0000 0000 RC0 to RC7 is low 
    TRISD=0x00;// 0000 0000 RC0 to RC7 is output
    PORTD=0x00;// 0000 0000 RD0 to RD7 is low 
    OPTION_REG&=0x7F;// 0111 1111 internal pull up activate
    INTCON &= ~(0x01);// 0000 0001  RBIF is cleared
    INTCON |=0x88;//1000 1000 RBIE and GIE are set as one 
    
    while(1)
    {
        PORTD=0xFF;// 0000 0000 RD0 to RD7 is high
        __delay_ms(500);
        PORTD=0x00;// 0000 0000 RD0 to RD7 is low 
        __delay_ms(500);
    }
}


void __interrupt() _ISR(void)
{
    if(INTCON & 0x01)
    {
        value=PORTB;// read the PORTB 
        if(value == 0xE0)
        {
            PORTC= ~PORTC;
        }
        INTCON &= ~(0x01);// 0000 0001  RBIF is cleared
    }
   
}