/*
 * File:   LCD_CLASS.c
 * Author: Admin
 *
 * Created on January 2, 2026, 8:10 PM
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

#include <xc.h>
#define _XTAL_FREQ 16000000

void init(void);
void lcd_command(unsigned char i);
void lcd_data(unsigned char i);
unsigned char i;
char arr[10]={"thirumalai"};
unsigned char a;
void main(void) {
    
    init();
    a=0x80;
    for(int i=0;i<10;i++){
        lcd_command(a++);
        lcd_data(arr[i]);
    }
    while(1);
}


void init(void){
    TRISC=0x00;
    PORTC=0x00;
    TRISD=0x00;
    PORTD=0x00;
    
    lcd_command(0x30);
    __delay_ms(100);
    lcd_command(0x30);
    __delay_ms(100);
    lcd_command(0x30);
    __delay_ms(100);
    lcd_command(0x38);
    __delay_ms(100);
    lcd_command(0x0C);
    __delay_ms(100);
    lcd_command(0x01);
    __delay_ms(100);
    
    
}
void lcd_command(unsigned char i){
    RD3=0;
    PORTC=i;
    RD1=1;
    __delay_ms(100);
    RD1=0;
}
void lcd_data(unsigned char i){
    RD3=1;
    PORTC=i;
    RD1=1;
    __delay_ms(100);
    RD1=0;
}