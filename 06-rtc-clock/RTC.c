/*
 * File:   RTC.c
 * Author: Admin
 *
 * Created on July 25, 2025, 10:20 PM
 */

// PIC16F877A Configuration Bits


// Configuration for oscillator and basic hardware features
#pragma config FOSC = HS        // High-Speed Oscillator
#pragma config WDTE = OFF       // Watchdog Timer disabled
#pragma config PWRTE = OFF      // Power-up Timer disabled
#pragma config BOREN = OFF      // Brown-out Reset disabled
#pragma config LVP = OFF        // Low-voltage programming disabled (RB3 used as digital I/O)
#pragma config CPD = OFF        // Data EEPROM Code Protection off
#pragma config WRT = OFF        // Flash memory write protection off
#pragma config CP = OFF         // Code protection off



#include <xc.h>

#define RS RC2// LCD RS pin connected to RC2
#define EN RC1// LCD enable pin connected to RC1
#define _XTAL_FREQ 20000000// 20MHz crystal frequency

// function
void init(void);
void i2c_init(const unsigned long);
void i2c_start();
int i2c_read(int);
void i2c_wait();
void i2c_stop();
int i2c_write(unsigned char);
void LCD_Position(unsigned char);
void LCD_Input(unsigned char);
int bcd_2_dec(int);
int dec_2_bcd(int);
void set_time_date(void);
void update(void);

// variable
unsigned char msg1[5]={"TIME:"};
unsigned char msg2[5]={"DATE:"};
int i,j,k,l;
unsigned char sec=30;
unsigned char min=59;
unsigned char hour=11;
unsigned char date=21;
unsigned char month=07;
unsigned int year=25;
unsigned char am_pm,raw;
unsigned char sec1,sec2,min1,min2,hour1,hour2,date1,date2,month1,month2,year1,year2;

void main()
{
    init();
    i2c_init(100);
    
    set_time_date();// Set RTC to 11:59:30 PM, 21-07-2025
    
    while(1)
    {
        update();
         // Extract digits
        sec1=sec/10;
        sec2=sec%10;
        min1=min/10;
        min2=min%10;
        if(hour==0){
            hour =12;
            am_pm=0;//am
        }else if(hour ==12){
            am_pm=1;
        }else if(hour>12){
             hour-=12;
             am_pm=1;
        }else{
            am_pm=0;
        }
        hour1=hour/10;
        hour2=hour%10;
        date1=date/10;
        date2=date%10;
        month1=month/10;
        month2=month%10;
        year1=year/10;
        year2=year%10;
        
        // Display TIME
        LCD_Position(0x80);
        for(i=0;i<5;i++)
            LCD_Input(msg1[i]);
        LCD_Input(hour1+'0');
        LCD_Input(hour2+'0');
        LCD_Input(0x2D);
        LCD_Input(min1+'0');
        LCD_Input(min2+'0');
        LCD_Input(0x2D);
        LCD_Input(sec1+'0');
        LCD_Input(sec2+'0');
        LCD_Input(' ');
        if(am_pm)
        {
        LCD_Input('A');
        LCD_Input('M');
        }
        else{
        LCD_Input('P');
        LCD_Input('m');
        }
        // Display DATE
        LCD_Position(0xC0);
        for(i=0;i<5;i++)
            LCD_Input(msg2[i]);
        LCD_Input(date1+'0');
        LCD_Input(date2+'0');
        LCD_Input(0x2D);
        LCD_Input(month1+'0');
        LCD_Input(month2+'0'); 
        LCD_Input(0x2D);
        LCD_Input(year1+'0');
        LCD_Input(year2+'0');
        
        __delay_ms(500);   
    }
}

//================  LCD Initialization ================
void init(void)
{
    TRISC=0x18;// SDA, SCL as input
    TRISD=0x00;// 0000 0000 set PORTD as output
    PORTD=0x00;// 0000 0000 set as low
  
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
void LCD_Position(unsigned char i)
{
    RS=0;// reset
    PORTD=i;       // data
    EN=1;// enable
    EN=0;
    __delay_ms(100);
}
void LCD_Input(unsigned char i)
{
    RS=1;// reset
    PORTD=i;       // data
    EN=1;// enable
    EN=0;
    __delay_ms(100);
}

//================ I2C ================
void i2c_init(const unsigned long feq_k)
{
    SSPCON=0x28;
    SSPSTAT=0x00;
    SSPCON2=0x00;
    SSPADD= (_XTAL_FREQ/(4*feq_k*100))-1;
    
}
void i2c_wait()
{
    while(SSPCON2 & 0x1F || SSPSTAT & 0x04);// wait for bus idle
    
}
void i2c_stop()
{
    i2c_wait();  
    PEN=1;
}
void i2c_start()
{
    i2c_wait();
    SEN=1;            
}
int i2c_read(int ack)
{
    int value;
    i2c_wait();
    RCEN=1;
    i2c_wait();
    value=SSPBUF;
    i2c_wait();
    ACKDT=(ack)?0:1;
    ACKEN=1;
    return value;
}
int i2c_write(unsigned char temp)
{
    i2c_wait();
    SSPBUF=temp;
}

//================ RTC BCD ===================
int bcd_2_dec(int temp)
{
    return ((temp>>4)*10)+(temp&0x0F);
}
int dec_2_bcd(int temp)
{
    return ((temp/10)<<4)+(temp%10);
}
void set_time_date(void)
{
    
    i2c_start();
    i2c_write(0xD0);// RTC write address
    i2c_write(0x00);// Start at seconds register
    i2c_write(dec_2_bcd(sec));// seconds
    i2c_write(dec_2_bcd(min));// minutes
    unsigned char hour_val = dec_2_bcd(11);
    hour_val |= 0b01000000; // 12-hour mode
    hour_val |= 0b00100000; // PM bit
    i2c_write(hour_val);    // hours
    i2c_write(1);// skip the days
    i2c_write(dec_2_bcd(date));// date
    i2c_write(dec_2_bcd(month));// month
    i2c_write(dec_2_bcd(year));//year
    i2c_stop();
    
    
}
//================ Update RTC =================
void update()
{
    i2c_start();
    i2c_write(0xD0);
    i2c_write(0x00);
    i2c_stop();
    i2c_start();
    i2c_write(0xD1);
    sec=(bcd_2_dec(i2c_read(1)));// Start from seconds
    min=(bcd_2_dec(i2c_read(1)));
     
   unsigned char raw_hour = i2c_read(1);
    am_pm = (raw_hour & 0x20) >> 5;       // bit 5 = PM
    hour  = bcd_2_dec(raw_hour & 0x1F);   // bits 4-0 = hour
    i2c_read(1); // skip day
    date=(bcd_2_dec(i2c_read(1)));
    month=(bcd_2_dec(i2c_read(1)));
    year=(bcd_2_dec(i2c_read(1)));
    i2c_stop();
    i2c_start();
    i2c_write(0xD1);
    i2c_read(1);
    i2c_stop();
}
            


    
    
    