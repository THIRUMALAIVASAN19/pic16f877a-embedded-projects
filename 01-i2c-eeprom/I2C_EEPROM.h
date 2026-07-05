
#ifndef __I2C_EEPROM_H
#define __I2C_EEPROM_H

#define __XTAL_FREQ 16000000  // Set crystal frequency (used for delay)


//     I2C Function Prototypes


void I2C_Master_Init(const unsigned long baud);   // Initialize I2C with given baud rate
void I2C_Master_Wait();                           // Wait until I2C bus is free
void I2C_Master_Start();                          // Send I2C start condition
void I2C_Master_RepeatedStart();                  // Send I2C repeated start
void I2C_Master_Stop();                           // Send I2C stop condition
void I2C_ACK();                                   // Send ACK to slave
void I2C_NACK();                                  // Send NACK to slave
unsigned char I2C_Master_Write(unsigned char data); // Write byte to I2C
unsigned char I2C_Read_Byte(void);                // Read byte from I2C


//    EEPROM Function Prototypes


void EEPROM_Write(unsigned int add, unsigned char data);                      // Write one byte to EEPROM
void EEPROM_Write_Page(unsigned int add, unsigned char* data, unsigned char len); // Write multiple bytes (page)
unsigned char EEPROM_Read(unsigned int add);                                  // Read one byte from EEPROM
void EEPROM_Read_Page(unsigned int add, unsigned char* data, unsigned int len);   // Read multiple bytes

#endif