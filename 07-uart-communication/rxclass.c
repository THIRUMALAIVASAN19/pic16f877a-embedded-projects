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





//----------------------- RX CODE -----------------------------
#include <xc.h>
#define _XTAL_FREQ 6000000
void lcd_init();// function to initialize 
unsigned char rx_reg,i,value;// global variable
void lcd_data(unsigned char i);// function to initialize 
void lcd_command(unsigned char i);// function to initialize 
void uart_init();// function to initialize 
unsigned char a[10]={"RX DATA= "}; 
unsigned char b[10]={"TX DATA= "};
void main() {
    uart_init();// initialized UART
    lcd_init();// initialized LCD
    lcd_command(0x80);
    for(int j=0;j<10;j++)
    {
    lcd_data(a[j]);
    }
    lcd_command(0xC0);
    for(int k=0;k<10;k++)
    {
    lcd_data(b[k]);
    }
    while(1)
    {
        if(PIR1& 0x20)// check if UART receiver interrupt flag is set
        {
        rx_reg=RCREG;// read the received data
        // switch  case
        switch(rx_reg)
        {
            case 'A':
                TXREG='a';// transmitted data a
                __delay_ms(100);
            break;
            case 'B':
                TXREG='b';// transmitted data b
                __delay_ms(100);
            break;
            case 'C':
                TXREG='c';// transmitted data c 
                __delay_ms(100);
            break;
            case 'D':
                TXREG='d';// transmitted data d
                __delay_ms(100);
            break;
        }
        // display the data to transmitted and received 
        lcd_command(0x88);
        lcd_data(rx_reg);
        value=TXREG;
        lcd_command(0xC8);
        lcd_data(value);                
        }
    }
  
}
void uart_init()
{
    // pin configuration
    TRISD=0x00;// PORTD as output
    PORTD=0x00;// initialize RD0 to RD7 is low
    // UART configuration
    TRISC=0xC0;//1100 0000 RC(TX) =1 RC7(RX)=1
    TXSTA=0x20;// enable tranmission (TXREN =1),Asynchronous mode
    SPBRG=0x09;// baud rate setting for 9600 @6MHz
    RCSTA=0x90;// enable serial port and continuous receive  
}
void lcd_command(unsigned char i)
{
    PORTB&=~0x08;// RS=1(command mode)
    PORTD=i;// data
    PORTB|=0x01;// EN=1
    PORTB&=~0x01;// EN =0
    __delay_ms(100);
}
void lcd_data(unsigned char i)
{
    PORTB|=0x08;// RS =1 (data mode)
    PORTD=i;// data 
    PORTB|=0x01;// EN=1
    PORTB&=~0x01;// EN=0
    __delay_ms(100); 
}
void lcd_init()
{
   
    TRISB=0x00;// PORTB as output
    PORTD=0x00;// initialize RB0 to RB7 is low
    //LCD configuration
    lcd_command(0x30);
    __delay_ms(100);
    lcd_command(0x30);
    __delay_ms(100);
    lcd_command(0x30);
    __delay_ms(100);
    lcd_command(0x38);
    __delay_ms(100);
    lcd_command(0x06);// increment(left to right)
    __delay_ms(100);
    lcd_command(0x0C);// display on ,cursor off
    __delay_ms(100);
    lcd_command(0x01);// clear the display
    __delay_ms(100);   
}