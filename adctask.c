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
void lcdnumber_rawconvert(unsigned int i); // RAW DATA DISPLAY FUNCTION
void lcdnumber_calibconvert(unsigned int i); // CALIBRATED DATA TO 0-135 DISPLAY FUNCTION
void lcd_command(unsigned char);
void lcd_data(unsigned char);
unsigned int adc_volt,adc_raw;
double  adc_calibdata;
unsigned int adc_high,adc_low;
unsigned char s1[9]="CHANNEL1:";
unsigned char s2[9]="CHANNEL4:";

void main()
{
    adc_lcd_init();
    while(1){
        
        // CHANNEL-1 CONFIGURATION//
        
        ADCON0=0X89; // ENABLING CHANNEL-1 1000 1001
         __delay_ms(20); // ACQUISTION TIME
        ADCON0 |=0X04; // SET GO DONE BIT TO 1
        while(ADCON0 &0X04); //CONTINUES UNTILL CONVERSION FINISHES
        adc_high=ADRESH;
        adc_low=ADRESL;
        adc_raw=(adc_high<<8)+adc_low;
        lcd_command(0x80); //FOR DISPLAYING IN 1ST ROW
        for(int i=0;i<9;i++){
            lcd_data(s1[i]);
        }
        lcdnumber_rawconvert(adc_raw);
        
        // CHANNEL-4 CONFIGURATION//
  
        ADCON0=0XA1; //ENABLING CHANNEL-4 1010 0001
         __delay_ms(40); //ACQUISTION TIME
        ADCON0 |=0X04; // SET GO DONE BIT TO 1
        while(ADCON0 &0X04); //CONTINUES UNTILL CONVERSION FINISHES
        adc_high=ADRESH;
        adc_low=ADRESL;
        adc_volt=(adc_high<<8)+adc_low;
        adc_calibdata=((double)adc_volt*338.0)/1023.0;
        lcd_command(0xC0); //FOR DISPLAYING IN 2ND ROW
        for(int i=0;i<9;i++){
            lcd_data(s2[i]);
        }
        lcdnumber_calibconvert((unsigned int)adc_calibdata);
    }
}

void lcdnumber_rawconvert(unsigned int i)
{
  
    int j = 0, s;
    unsigned int m = i;
    unsigned char k[5];
    while (m != 0)
    {
        s = m % 10;
        k[j] = s;
        j++;
        m /= 10;
    }
    if (j == 0)
    {
        k[0] = 0;
        j = 1;
    }
    lcd_command(0x89); // Set LCD cursor to print after Channel1:
    // Cells with spaces for alignment
    if (j == 4) {
        lcd_data(0x30 + k[3]);
        lcd_data(0x30 + k[2]);
        lcd_data(0x30 + k[1]);
        lcd_data(0x30 + k[0]);
    }
    else if (j == 3) {
        lcd_data(' ');
        lcd_data(0x30 + k[2]);
        lcd_data(0x30 + k[1]);
        lcd_data(0x30 + k[0]);
    }
    else if (j == 2) {
        lcd_data(' ');
        lcd_data(' ');
        lcd_data(0x30 + k[1]);
        lcd_data(0x30 + k[0]);
    }
    else if (j == 1) {
        lcd_data(' ');
        lcd_data(' ');
        lcd_data(' ');
        lcd_data(0x30 + k[0]);
    }
    lcd_data(0x76); // TO DISPLAY 'V'    
}

void lcdnumber_calibconvert(unsigned int i)
{
   
    int j = 0, s;
    unsigned int m = i;
    unsigned char k[4]; 
    while (m != 0)
    {
        s = m % 10;
        k[j] = s;
        j++;
        m /= 10;
    }
    if (j == 0)
    {
        k[0] = 0;
        j = 1;
    }
    lcd_command(0xC9);  // To display in 9th cell of 2nd row
    if (j == 3)
    {
        lcd_data(0x30 + k[2]);
        lcd_data(0x30 + k[1]);
        lcd_data(0x30 + k[0]);
    }
    else if (j == 2)
    {
        lcd_data(' ');
        lcd_data(0x30 + k[1]);
        lcd_data(0x30 + k[0]);
    }
    else if (j == 1)
    {
        lcd_data(' ');
        lcd_data(' ');
        lcd_data(0x30 + k[0]);
    }

    lcd_data('v'); // Display 'v'

}

void adc_lcd_init(){
    TRISC=0x00;
    TRISD=0x00;
    lcd_command(0x30); // LCD INITIALIZATION
    __delay_ms(100);
    lcd_command(0x30); // LCD INITIALIZATION
    __delay_ms(100);
    lcd_command(0x30); // LCD INITIALIZATION
    __delay_ms(100);
    lcd_command(0x38); // SETTING THE N,F,DL BITS TO 1 (0011 1000)
    __delay_ms(100);
    lcd_command(0x0c); // SETTING D BIT TO 1 AND C BIT 0 (0000 1100)
    __delay_ms(100);
    lcd_command(0x01); // SETTING 0000 0001 TO CLEAR DISPLAY
    __delay_ms(100);
    
    //ADC INITIALIZATION //
   
    ADCON1=0X82; // MAKING AN1 AND AN4 AS ANALOG PIN 1000 0010
   
}
void lcd_data(unsigned char i){
    PORTC|=0X08; // ENABLE RS PIN FOR PASSING DATA TO LCD
    PORTD=i; // I VALUE STORED IN PORT D REGISTER
    PORTC|=0X01; // ENABLE ON
    __delay_ms(50);
    PORTC&=~0X01; // ENABLE OFF
}
void lcd_command(unsigned char i){
    PORTC&=~0X08; // ENABLE RS PIN FOR PASSING COMMAND TO LCD
    PORTD=i;// I VALUE STORED IN PORT D REGISTER
    PORTC|=0X01;// ENABLE ON
    __delay_ms(50);
    PORTC&=~0X01;// ENABLE OFF
}
