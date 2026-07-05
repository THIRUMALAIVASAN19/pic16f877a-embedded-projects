#include <xc.h>
#include "I2C_EEPROM.h"

#define _XTAL_FREQ 16000000
#define EEPROM_Address_R  0xA1  // EEPROM Read Address (with R/W bit = 1)
#define EEPROM_Address_W  0xA0  // EEPROM Write Address (with R/W bit = 0)

//--------------------------------------------------
//              I2C Master Routines
//--------------------------------------------------

// Initialize I2C in Master Mode with desired baud rate
void I2C_Master_Init(const unsigned long baud)
{
    SSPCON = 0b00101000;       // Configure MSSP module in I2C Master mode
    SSPCON2 = 0;               // Clear control bits
    SSPADD = (_XTAL_FREQ/(4*baud))-1; // Set Baud rate
    SSPSTAT = 0;               // Clear status register
    TRISC3 = 1;                // Set SCL (RC3) as input
    TRISC4 = 1;                // Set SDA (RC4) as input
}

// Wait until I2C bus is idle
void I2C_Master_Wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); // Wait till idle
}

// Send Start Condition
void I2C_Master_Start()
{
    I2C_Master_Wait();
    SEN = 1;
}

// Send Repeated Start Condition
void I2C_Master_RepeatedStart()
{
    I2C_Master_Wait();
    RSEN = 1;
}

// Send Stop Condition
void I2C_Master_Stop()
{
    I2C_Master_Wait();
    PEN = 1;
}

// Write one byte on I2C bus
unsigned char I2C_Master_Write(unsigned char data)
{
    I2C_Master_Wait();
    SSPBUF = data;               // Load data into buffer
    while(!SSPIF);               // Wait for transmission to complete
    SSPIF = 0;                   // Clear interrupt flag
    return ACKSTAT;              // Return ACK status (0 = ACK, 1 = NACK)
}

// Read one byte from I2C bus
unsigned char I2C_Read_Byte(void)
{
    I2C_Master_Wait();
    RCEN = 1;                    // Enable receive mode
    while(!SSPIF);               // Wait for reception complete
    SSPIF = 0;                   // Clear interrupt flag
    I2C_Master_Wait();
    return SSPBUF;              // Return received byte
}

// Send ACK
void I2C_ACK(void)
{
    ACKDT = 0;                   // ACK
    I2C_Master_Wait();
    ACKEN = 1;                   // Enable ACK
}

// Send NACK
void I2C_NACK(void)
{
    ACKDT = 1;                   // NACK
    I2C_Master_Wait();
    ACKEN = 1;                   // Enable NACK
}

//--------------------------------------------------
//              EEPROM Routines
//--------------------------------------------------

// Write one byte to EEPROM
void EEPROM_Write(unsigned int add, unsigned char data)
{
    I2C_Master_Start();

    while(I2C_Master_Write(EEPROM_Address_W))   // Send control byte until EEPROM ready
        I2C_Master_RepeatedStart();

    I2C_Master_Write(add >> 8);                  // Send high byte of address
    I2C_Master_Write((unsigned char)add);        // Send low byte of address
    I2C_Master_Write(data);                      // Send data byte
    I2C_Master_Stop();
    __delay_ms(10);                              // EEPROM write delay
}

// Write multiple bytes (page write) to EEPROM
void EEPROM_Write_Page(unsigned int add, unsigned char* data, unsigned char len)
{
    I2C_Master_Start();

    while(I2C_Master_Write(EEPROM_Address_W))   // Wait until EEPROM is idle
        I2C_Master_RepeatedStart();

    I2C_Master_Write(add >> 8);                  // Send high byte of address
    I2C_Master_Write((unsigned char)add);        // Send low byte of address
    for(unsigned int i = 0; i < len; i++)
        I2C_Master_Write(data[i]);               // Write each byte
    I2C_Master_Stop();
    __delay_ms(10);                              // EEPROM write delay
}

// Read one byte from EEPROM
unsigned char EEPROM_Read(unsigned int add)
{
    unsigned char Data;
    I2C_Master_Start();

    while(I2C_Master_Write(EEPROM_Address_W))   // Wait until EEPROM is idle
        I2C_Master_RepeatedStart();

    I2C_Master_Write(add >> 8);                  // Send high byte of address
    I2C_Master_Write((unsigned char)add);        // Send low byte of address

    I2C_Master_RepeatedStart();                  // Restart for read operation
    I2C_Master_Write(EEPROM_Address_R);          // Send EEPROM read command
    Data = I2C_Read_Byte();                      // Read the byte
    I2C_NACK();                                  // Send NACK to end read
    I2C_Master_Stop();

    return Data;                                 // Return received data
}

// Read multiple bytes (page read) from EEPROM
void EEPROM_Read_Page(unsigned int add, unsigned char* data, unsigned int len)
{
    I2C_Master_Start();

    while(I2C_Master_Write(EEPROM_Address_W))   // Wait until EEPROM is idle
        I2C_Master_RepeatedStart();

    I2C_Master_Write(add >> 8);                  // Send high byte of address
    I2C_Master_Write((unsigned char)add);        // Send low byte of address

    I2C_Master_RepeatedStart();
    I2C_Master_Write(EEPROM_Address_R);          // Send EEPROM read command
    for(unsigned int i = 0; i < len; i++)
    {
        data[i] = I2C_Read_Byte();               // Read each byte
        I2C_ACK();                               // Send ACK after each byte
    }
    I2C_Master_Stop();                           // Stop I2C communication
}