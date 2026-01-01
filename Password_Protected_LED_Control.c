/*
 * File:   LED.c
 * Author: Admin
 *
 * Created on December 30, 2025, 2:30 PM
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

/*
    Application problem

Design a secure LED control system:

A switch is used to enter a password sequence (e.g., 1-0-1)

If the correct sequence is entered → LED turns ON

Wrong sequence → LED blinks 3 times as error indication

👉 Real-world use: Door lock / device authorization indicator
*/
void blink_once()
{
    RB1 = 1;
    __delay_ms(150);
    RB1 = 0;
    __delay_ms(150);
}

int main(){
    
    
    TRISB=0x01;
    PORTB=0x00;
    
    
    unsigned char input[3];
    unsigned char index=0;
    unsigned char t=0;
    unsigned char det=0; 
   
    
    
    for(int i=0;i<3;i++){
        while(t<200){
        if(RB0==1){
            __delay_ms(50);
            
            if(RB0==1){
                input[index]=1;
                blink_once();
                det=1;              
                while(RB0==1);
                break;
            }
           
        }
        
       __delay_ms(10);
       t++;
        }
       if(det==0){
           input[index]=0;
           blink_once();
       }
       index++;
       det=0;
       t=0;
     
    }    
        
    if(input[0]==1 && input[1]==0 && input[2]==1){
        RB1=1;
    } 
    else{
        
        for(int j=0;j<3;j++){
        RB1=1;
        __delay_ms(500);
        RB1=0;
        __delay_ms(500);
    }
    }
    }



