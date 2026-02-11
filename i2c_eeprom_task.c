#include <xc.h>
#include "i2c_eeprom_task.h"

#define _XTAL_FREQ 16000000            // Define system clock frequency
#define EEPROM_Address_R  0xA1         // EEPROM I2C read address
#define EEPROM_Address_W  0xA0         // EEPROM I2C write address


//               I2C Master Routines


// Initialize I2C module in master mode
void I2C_Master_INTIALIZATION(const unsigned long baud)
{
    SSPCON = 0b00101000;                                // Enable MSSP module in I2C Master mode
    SSPCON2 = 0;                                        // Clear control bits
    SSPADD = (_XTAL_FREQ / (4 * baud)) - 1;             // Set clock frequency
    SSPSTAT = 0;                                        // Clear status register
    TRISC3 = 1;                                         // Set SCL (RC3) as input
    TRISC4 = 1;                                         // Set SDA (RC4) as input
}

// Wait until I2C bus is idle
void I2C_Master_Wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));        // Wait until transmission is complete
}

// Send I2C START condition
void I2C_Master_Start()
{
    I2C_Master_Wait();
    SEN = 1;                                            // Initiate start condition
}

// Send I2C REPEATED START condition
void I2C_Master_RepeatedStart()
{
    I2C_Master_Wait();
    RSEN = 1;                                           // Initiate repeated start condition
}

// Send I2C STOP condition
void I2C_Master_Stop()
{
    I2C_Master_Wait();
    PEN = 1;                                            // Initiate stop condition
}

// Write a byte to I2C bus
unsigned char I2C_Master_Write(unsigned char data)
{
    I2C_Master_Wait();
    SSPBUF = data;                                     // Load data into buffer
    while (!SSPIF);                                    // Wait for transmission to complete
    SSPIF = 0;                                         // Clear interrupt flag
    return ACKSTAT;                                    // Return ACK status
}

// Read a byte from I2C bus
unsigned char I2C_Read_Byte(void)
{
    I2C_Master_Wait();
    RCEN = 1;                                          // Enable reception
    while (!SSPIF);                                    // Wait for reception complete
    SSPIF = 0;                                         // Clear interrupt flag
    I2C_Master_Wait();
    return SSPBUF;                                     // Return received byte
}

// Send ACK to slave
void I2C_ACK(void)
{
    ACKDT = 0;                                         // 0 = ACK
    I2C_Master_Wait();
    ACKEN = 1;                                         // Enable ACK
}

// Send NACK to slave
void I2C_NACK(void)
{
    ACKDT = 1;                                         // 1 = NACK
    I2C_Master_Wait();
    ACKEN = 1;                                         // Enable NACK
}

//======================================================
//              EEPROM I2C Routines
//======================================================

// Write one byte to EEPROM
void EEPROM_Write(unsigned int add, unsigned char data)
{
    I2C_Master_Start();                                // Start condition

    while (I2C_Master_Write(EEPROM_Address_W))         // Wait until EEPROM is ready
        I2C_Master_RepeatedStart();

    I2C_Master_Write(add >> 8);                        // Send high byte of address
    I2C_Master_Write((unsigned char)add);              // Send low byte of address
    I2C_Master_Write(data);                            // Write data byte
    I2C_Master_Stop();                                 // Stop condition
    __delay_ms(10);                                    // Write cycle delay
}

// Write multiple bytes (page write) to EEPROM
void EEPROM_Write_Page(unsigned int add, unsigned char* data, unsigned char len)
{
    I2C_Master_Start();                                // Start condition

    while (I2C_Master_Write(EEPROM_Address_W))         // Wait until EEPROM is ready
        I2C_Master_RepeatedStart();

    I2C_Master_Write(add >> 8);                        // Send high byte of address
    I2C_Master_Write((unsigned char)add);              // Send low byte of address
    for (unsigned int I = 0; I < len; I++)
    {
        I2C_Master_Write(data[I]);                     // Write each byte
    }
    I2C_Master_Stop();                                 // Stop condition
    __delay_ms(10);                                    // Write cycle delay
}

// Read one byte from EEPROM
unsigned char EEPROM_Read(unsigned int add)
{
    unsigned char Data;
    I2C_Master_Start();                                // Start condition

    while (I2C_Master_Write(EEPROM_Address_W))         // Wait until EEPROM is ready
        I2C_Master_RepeatedStart();

    I2C_Master_Write(add >> 8);                        // Send high byte of address
    I2C_Master_Write((unsigned char)add);              // Send low byte of address
    I2C_Master_RepeatedStart();                        // Restart for read
    I2C_Master_Write(EEPROM_Address_R);                // Send read command
    Data = I2C_Read_Byte();                            // Read data byte
    I2C_NACK();                                        // Send NACK
    I2C_Master_Stop();                                 // Stop condition

    return Data;                                       // Return received data
}

// Read multiple bytes from EEPROM (sequential read)
void EEPROM_Read_Page(unsigned int add, unsigned char* data, unsigned int len)
{
    I2C_Master_Start();                                // Start condition

    while (I2C_Master_Write(EEPROM_Address_W))         // Wait until EEPROM is ready
        I2C_Master_RepeatedStart();

    I2C_Master_Write(add >> 8);                        // Send high byte of address
    I2C_Master_Write((unsigned char)add);              // Send low byte of address
    I2C_Master_RepeatedStart();                        // Restart for read
    I2C_Master_Write(EEPROM_Address_R);                // Send read command
    for (unsigned int I = 0; I < len; I++)
    {
        data[I] = I2C_Read_Byte();                     // Read byte and store in buffer
        I2C_ACK();                                     // Send ACK after each byte
    }
    I2C_Master_Stop();                                 // Stop condition
}