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
#define _XTAL_FREQ 4000000 // Define crystal frequency for delay functions

unsigned char count=0;// Counter for Timer1 interrupt occurrences

void timer1_init(void); // Function prototype for Timer1 initialization
void main()
{
    timer1_init();// // Initialize Timer1 and related settings
    while(1)
    {
        
        PORTB=0xFF;// 1111 1111 all pins of PORTB is high
        __delay_ms(500);
        PORTB=0x00;// 0000 0000 all pins of PORTB is low
        __delay_ms(500);
    
    }

}
void timer1_init(void)
{
    // pin configuration
    TRISB=0x00; //0000 0000 Set PORTB as output
    TRISC=0x00;//0000 0000 Set PORTC as output
    PORTB=0x00;//0000 0000 Initialize PORTB to low
    PORTC=0x00; //0000 0000 Initialize PORTC to low
    
    //timer 1 configuration
    TMR1=34002;// preload the value
    T1CON &= ~(1 << 1);// internal clock (Fosc/4)
    T1CON &= (00 << 4);//pre-scaler 1:1
    T1CON |= (1 << 0); //enable timer1
    
    //interrupt configuration
    PIE1 |=(1 << 0);//TMR1 Overflow Interrupt Enable bit
    PIR1 &=~(1 << 0);// Clear Timer1 interrupt flag (TMR1IF)
    INTCON |=(3 << 6);// Enable Global Interrupt (GIE) and Peripheral Interrupt (PEIE)
    
}

void __interrupt() _ISR(void)
{
    if(PIR1 & 0x01) 
    {
        count++;
        if(count == 31 )
        {
        PORTC = ~ PORTC;// Toggle PORTC
        count=0;// reset count
       
        }
         TMR1=34002;// preload the value
         PIR1 &=~(1 << 0);// Clear Timer1 interrupt flag
  
    }

}

