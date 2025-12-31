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
 Requirement

Switch pressed ? LED ON

Press again ? LED OFF

If LED stays ON for 30 seconds, turn it OFF automatically
 */
void main(void) {
     TRISB0=1;// switch
    TRISB1=0;// LED or LIGHT
    
    unsigned char LED_state=0;
    unsigned int Timer=0;
 while(1){
     // switch read
    if(RB0==1){
        __delay_ms(50);// DEBOUNCING
        
        if(RB0==1){
            LED_state=!LED_state;// TOGGLE
            RB1=LED_state;// LIGHT ON 
            
            Timer=0;// TIMER RESET
            while(RB0==1);
        }
    }
    
    // Delay time
     __delay_ms(100);
     
    if(LED_state==1)
    {     
        Timer++;   
    if(Timer <=300)
    {
        LED_state=0;
        RB1=LED_state;
        
    }
    }
 
}
}
