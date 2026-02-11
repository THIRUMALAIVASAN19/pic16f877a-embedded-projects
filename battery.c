/*
 * File:   battery.c
 * Author: Admin
 *
 * Created on June 6, 2025, 5:19 PM
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
void LCD_Input(unsigned char);// data to the program
void init(void);// command to LCD
void LCD_Position(unsigned char);// position of world
void LCD_Output(float);
void keyscane();// recursion function
void condition(void);// condition of battery
unsigned char p,q,value;// variable declaration 
unsigned int r1,r2,r3;// variable declaration
float s,t;// variable declaration
unsigned char arr[15]={"S.NEGHASRI"};// array declaration 
void main() 
{
 init();
 while(1){
     keyscane();     
 }
}
void init(void)
{
    TRISC=0x00;// 0000 0000 set PORTC as output
    TRISD=0x00;// 0000 0000 set PORTD as output
    TRISB=0xF0;// 1111 0000 set RB4 to RB7 as input and RB0 and RB3 as output
    OPTION_REG&=0x7F;// internal pull up resistor
    // lcd configuration
    LCD_Position(0x30);
    __delay_ms(100);
    LCD_Position(0x30);
    __delay_ms(100);
    LCD_Position(0x30);
    __delay_ms(100);
    LCD_Position(0x38);
    __delay_ms(100);
    LCD_Position(0x06);
    __delay_ms(100);
    LCD_Position(0x0C);// display on cursor off
    __delay_ms(100);
    LCD_Position(0x01);// clear display
    __delay_ms(100);
}
void keyscane()
{
    value=PORTB & 0xF0;// 1111 0000 data to switch case  
    switch(value){
      case 0xE0:   // 1110 0000 RB4 is on 
        LCD_Position(0x80);// LCD starting
        for(q=0;q<14;q++)
        {
            LCD_Input(arr[q]);            
        }       
        t=17.5;
        condition();
        break;
        case 0xD0:// 1101 0000 RB5 is on
           t=t+0.1;
           if(t>22.5)
           {
               t=22.5;              
           }
         LCD_Position(0x88);
         LCD_Output(t);
         condition();       
         break;
      case 0xB0:// 1011 0000 RB6 is on
            t=t-0.1;
            if(t<15.5)
            {
                t=15.5;
            }
            LCD_Position(0x88);
            LCD_Output(t);
            condition();
            break;
      case 0x70:// 1000 0000 RB7 is  on
            t=17.5;
            LCD_Position(0x88);
            LCD_Output(t);
            condition();
            break;    
    }    
}
void LCD_Position(unsigned char s)
{
    PORTC&=~0x08;  // 0000 1000 Reset=1 at RC3
    PORTD=s;       // data
    PORTC|=0x01;   //0000 0001 enable=1 at RC0
    PORTC&=~0x01;  //0000 0001 enable=0 at RC0
    __delay_ms(100);  
}
void LCD_Input(unsigned char s)
{
    PORTC|=0x08;   // 0000 1000 Reset=1 at RC3
    PORTD=s;       // data
    PORTC|=0x01;   //0000 0001 enable=1 at RC0
    PORTC&=~0x01;  //0000 0001 enable=0 at RC0
    __delay_ms(100);
}
void LCD_Output(float s)
{
    s=s*10;
    r3=(unsigned char)(s/100);// 1
    r2=(unsigned char)((s-(r3*100))/10);// 155 -(1*100)/10
    r1=(unsigned char)(s-(r3*100)-(r2*10));  
    LCD_Position(0x88);
    LCD_Input(0x30+r3);
    LCD_Position(0x89);
    LCD_Input(0x30+r2);
    LCD_Position(0x8A);
    LCD_Input(0x2E);
    LCD_Position(0x8B);
    LCD_Input(0x30+r1);   
}
void condition(void)
{ 
 unsigned char arr1[15]={"BATT LOW"};// array declaration
 unsigned char arr2[15]={"BATT MEDIUM"};// array declaration
 unsigned char arr3[15]={"BATT HIGH"};// array declaration
    LCD_Position(0xC0);
    if(t<17.6)
       {
        for(p=0;p<15;p++)
        {       
              LCD_Input(arr1[p]); // battery low
        }
        }
    else if(t<20.6)
            {
             for(p=0;p<15;p++)
             {
                LCD_Input(arr2[p]);// battery medium
             }
        }   
        else
            {
             for(p=0;p<15;p++)
             {           
                 LCD_Input(arr3[p]);// battery high
             }
        }
}