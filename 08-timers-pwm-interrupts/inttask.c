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
    TRISB=0x01;// 0000 0001 RB0  is input and RB1 to RB7 is output
    PORTB=0x00;// 0000 0000 RB0 to RB7 is low 
    TRISC=0x00;// 0000 0000 RC0 to RC7 is output
    PORTC=0x00;// 0000 0000 RC0 to RC7 is low 
    TRISD=0x00;// 0000 0000 RC0 to RC7 is output
    PORTD=0x00;// 0000 0000 RD0 to RD7 is low 
    OPTION_REG&=0xBF;// 1100 1111 INTERNAL PULL UP IS DISABLE AND INTEDG =0 so it is falling edge  
    INTCON &= ~(0x02);//0000 0010 INTF  is cleared
    INTCON |=0x90;//1100 0000 INTE   and GIE are set as one 
    
     while(1)
    {
        PORTC=0xFF;// 0000 0000 RD0 is high
        __delay_ms(500);
        PORTC=0x00;// 0000 0000 RD0 is low 
        __delay_ms(500);
    }
}
void __interrupt() _ISR(void)
{
    if(INTCON & 0x02)
    {
        value=PORTB;// read the PORTB 
        if(value == 0x00)
        {
            PORTD= ~PORTD;
            
        }
        INTCON &= ~(0x02);// 0000 0001  RBIF is cleared
    }
    
}































