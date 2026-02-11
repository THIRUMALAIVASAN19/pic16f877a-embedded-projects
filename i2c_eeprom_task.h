#ifndef __I2C_EEPROM_TASK_H
#define __I2C_EEPROM_TASK_H

#define __XTAL_FREQ 16000000  // Define system clock frequency (used for delays)

//==========================================
//         Function Prototypes
//==========================================

//-----------[ I2C Routines ]-----------

// Initialize I2C master with given baud rate (e.g., 100000 for 100kHz)
void I2C_Master_INTIALIZATION(const unsigned long baud);

// Wait until I2C bus is free
void I2C_Master_Wait();

// Send START condition
void I2C_Master_Start();

// Send REPEATED START condition
void I2C_Master_RepeatedStart();

// Send STOP condition
void I2C_Master_Stop();

// Send ACK to the slave
void I2C_ACK();

// Send NACK to the slave
void I2C_NACK();

// Write one byte to the I2C bus
unsigned char I2C_Master_Write(unsigned char data);

// Read one byte from the I2C bus
unsigned char I2C_Read_Byte(void);


//-----------[ EEPROM Routines ]-----------

// Write one byte to EEPROM at specified address
void EEPROM_Write(unsigned int add, unsigned char data);

// Write multiple bytes (page write) to EEPROM
void EEPROM_Write_Page(unsigned int add, unsigned char* data, unsigned char len);

// Read one byte from EEPROM at specified address
unsigned char EEPROM_Read(unsigned int add);

// Read multiple bytes (page read) from EEPROM
void EEPROM_Read_Page(unsigned int add, unsigned char* data, unsigned int len);

#endif