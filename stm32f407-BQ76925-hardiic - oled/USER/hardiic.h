#ifndef HARDIIC_H
#define HARDIIC_H
#include "stm32f4xx.h"

#define SLAVE_Write_Address   0x04
#define SLAVE_Read_Address    0x04
void HardI2cInit(void);
void I2C_write_onebyte(u16 WriteAddr,u8 DataToWrite);
u8 I2C_read_onebyte(u16 ReadAddr);


#endif

