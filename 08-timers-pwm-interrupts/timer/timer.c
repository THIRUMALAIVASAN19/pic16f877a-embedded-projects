#include <xc.h>
#define _XTAL_FREQ 6000000

unsigned char cont=0; // Counter for Timer1 interrupt occurrences
        
void timer_1_init(void);// Function prototype for Timer1 initialization

void main()
{
    timer_1_init();// Initialize Timer1 and related settings
    while(1)
    {
        PORTC=0xFF;// 1111 1111 all pins is high
        __delay_ms(500);
        PORTC=0x00;// 0000 0000 all pins are low
        __delay_ms(500);
    }
}
void timer_1_init(void)
{
    // pin configuration
    TRISC=0x00; //0000 0000 Set PORTC as output
    TRISD=0x00;//0000 0000 Set PORTD as output
    PORTC=0x00;//0000 0000 Initialize PORTC to low
    PORTD=0x00; //0000 0000 Initialize PORTD to low
    // timer 1 register configuration
    T1CON=0x01; // Timer1 ON,  pre-scaler 1:1
    // timer 1 interrupt configuration
    INTCON|=(3<<6); // Enable Global Interrupt (GIE) and Peripheral Interrupt (PEIE)
    PIE1|=(1<<0); // Enable Timer1 interrupt (TMR1IE)
    PIR1&=~(1<<0);// Clear Timer1 interrupt flag (TMR1IF)
}
// Interrupt Service Routine
void __interrupt() _ISR(void)
{
    if(PIR1 & 0x01)// Check if Timer1 overflow interrupt occurred
    {
        cont++; // Increment overflow counter
        if (cont==23)// After 23 overflows (based on timing goal)
        {
            
        
        PORTD = ~PORTD;  // Toggle PORTD
        cont = 0;        // Reset counter
        }
    
        PIR1 &=~(1<<0); // Clear Timer1 interrupt flag
    }
}
  
