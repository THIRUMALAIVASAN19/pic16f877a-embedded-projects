/*
 * File:   led18.c
 * Author: Admin
 *
 * Created on November 18, 2025
 */

// PIC18F45K20 Configuration Bit Settings
#pragma config FOSC = INTIO67  // Internal oscillator, RA6/7 as port pins
#pragma config WDTEN = OFF     // Watchdog Timer off
#pragma config LVP = OFF       // Low-voltage programming off

#include <xc.h>

#define _XTAL_FREQ 16000000UL

void main(void) {
    // Set internal oscillator to 16 MHz (INTIO67 requires OSCCON)
    OSCCON = 0x70;  // IRCF2:0 = 111 -> 16 MHz, SCS bits select internal oscillator

    // Disable analog functions on PORTA and PORTB so digital I/O works
    ANSEL = 0x00;
    ANSELH = 0x00;

    TRISA = 0x00;     // Make PORTA output
    PORTA = 0x00;     // Clear PORTA

    while(1){
        PORTA = 0x01;     // Turn ON RA0
        __delay_ms(1000);

        PORTA = 0x00;     // Turn OFF RA0
        __delay_ms(1000);
    }
}