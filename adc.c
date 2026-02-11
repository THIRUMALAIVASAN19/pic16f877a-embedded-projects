/*
 * File:   adc class.c
 * Author: Admin
 *
 * Created on May 2, 2025, 6:25 PM
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
#define _XTAL_FREQ 6000000
void adc_lcd_init();
void lcd_number_convert(unsigned int i);
void lcd_command(unsigned char i);
void lcd_data(unsigned char i);

unsigned long adc_calibrated_data;
unsigned int m,n;
int adc_volt;
unsigned char adc_high,adc_low,b[5];
void main() {
    adc_lcd_init();
    while(1)
    {
    
        ADCON0 |=0x04;// bit manipulation 1000 0001 | 0000 0100 = 1000 0101
        while(ADCON0 & 0x04);// loop until  1000 0001 = 0000 0000
        adc_high=ADRESH;// MSB bit ADRESH register
        adc_low=ADRESL;// LCB bit ADRESL register
        adc_volt=(adc_high << 8)+adc_low;// raw data
        adc_calibrated_data=(adc_volt*338)/1024;// calibrated data
        lcd_command(0x80);
        lcd_number_convert(adc_calibrated_data);// data to LCD
    }
}
void adc_lcd_init()
{
    TRISC=0x00;// 0000 0000 set PORTC as output
    TRISD=0x00;// 0000 0000 set PORTD as output
    PORTD=0x00;
    PORTC=0x00;
    //------------ LCD INTIALIZATION ----------------------
    lcd_command(0x30);// 0011 0000 LCD INTIALIZATION
    __delay_ms(100);// delay
     lcd_command(0x30);//0011 0000 LCD INTIALIZATION
    __delay_ms(100);// delay
     lcd_command(0x30);//0011 0000 LCD INTIALIZATION
    __delay_ms(100);// delay
     lcd_command(0x38);// 0011 1000
    __delay_ms(100);// delay
    
     lcd_command(0x0C);//0000 1100 display on cursor off
    __delay_ms(100);//delay
     lcd_command(0x01);//0000 0001 clear display
    __delay_ms(100);// delay
    
    // ----------- ADC initialization---------------------
    ADCON1=0x8E;// AN0 pin,all pin are digital expect AN0,right justification
    ADCON0=0x81;// 1000 0001 ADC power on ,AN0 Analog channel selection,FOSC/32 
    __delay_us(19);// Acqution time
}
void lcd_command(unsigned char i)
{
    PORTC&=~0x08;// 0000 1000 reset =1 at RC3
    PORTD=i;// data 
    PORTC|=0x01;// 0000 0001 enable = 1 at RC0    
    PORTC&=~0x01;// 0000 0001 enable = 0 at RC0
    __delay_ms(100);// delay
}
void lcd_data(unsigned char i)
{
    PORTC|=0x08;// 0000 1000 reset =1 at RC3
    PORTD=i;// data 
    PORTC|=0x01;// 0000 0001 enable = 1 at RC0    
    PORTC&=~0x01;// 0000 0001 enable = 0 at RC0    
    __delay_ms(100); // delay
}
void lcd_number_convert(unsigned int i )
{
    unsigned char s,a=1;
    m=i;
    while(m!=0)
    {
        s=(m-((m/10)*10));
        b[a]=s;
        a++;
        m=m/10;
    }
    b[a]='\0';
    a--;
    if(i>=100)
    {
        lcd_command(0x80);
        lcd_data(0x30 + b[3]);
        
        lcd_data(0x30 + b[2]);
     
        lcd_data(0x30 + b[1]);
    }
    else if (i>=10)
    {
        lcd_command(0x80);
        lcd_data(0x20);// space
      
        lcd_data(0x30 + b[2]);
        
        lcd_data(0x30 + b[1]);
        
    }
    else
    {
       lcd_command(0x80);
        lcd_data(0x20);// space
        
        lcd_data(0x20);// space
      
        lcd_data(0x30 + b[1]); 
    }
     lcd_command(0x83);
        lcd_data(0x76);// v
}
 