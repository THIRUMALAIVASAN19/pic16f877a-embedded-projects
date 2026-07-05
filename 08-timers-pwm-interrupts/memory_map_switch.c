/*
 * File:   7segment.c
 * Author: Admin
 *
 * Created on May 24, 2025, 11:01 AM
 */

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


#define _XTAL_FREQ 6000000
unsigned char value;
unsigned char *IO_reg_b=(unsigned char *)0x86;//1000 0110 TRISB register
unsigned char *data_reg_b=(unsigned char *)0x06;//0000 0110 PORTB register
unsigned char *IO_reg_c=(unsigned char *)0x87;//1000 0111 TRISC register
unsigned char *data_reg_c=(unsigned char *)0x07;//0000 0111 PORTC register
unsigned char *pull_up=(unsigned char *)0x81;// 1000 0001 OPTION_REG register for internal pull up
void main(void) {
   *IO_reg_b=0xF0;//1111 0000 RC0 to RC3 as output RC4 to RC7 as input
   *IO_reg_c=0x00;// 0000 0000 RC0 to RC7 all pin are output
    *data_reg_b=0x00;// 0000 0000 RB0 to RB7 all pin are low
    *data_reg_c=0x00;// 0000 0000 RC0 to RC7 all pin are low
    *pull_up &=0x7F;
    while(1)
    {
        value= *data_reg_b;
        switch(value)
        {
             case 0xE0:
             *data_reg_c=0x02;// 0000 0010 RC1 is high and RC6 is low
             break;
             case 0xD0:
            *data_reg_c=0x40;// 0100 0000 RC1 is low and RC6 is high
             break;
             case 0xB0:
             *data_reg_c=0x42;// 0100 0010 RC1 is high and RC6 is high
             break;
             case 0x70:
            *data_reg_c=0x00;// 0000 0000 RC1 is low and RC6 is low
             break;                  
        }       
    }   
}