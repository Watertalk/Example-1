#ifndef STM32BQ76925_H
#define STM32BQ76925_H
#include "stm32f4xx.h"


//#define CHECKIIC  
//#define IIC   1
#define TESTSEND 1

#define Write_IIC 0x00
#define Read_IIC  0x01

/*****
SCL PB10
SDA PB11
VCOUT PC5
VIOUT PB0
ALERT PB1
****/
#define I2C_GROUP_ADDR    0x04
/******ºê¶¨ÒåÖ»¶Á¼Ä´æÆ÷*****/

#define VREF_CAL 0x10
#define VC1_CAL  0x11
#define VC2_CAL  0x12
#define VC3_CAL  0x13
#define VC4_CAL  0x14
#define VC5_CAL  0x15
#define VC6_CAL  0x16
#define VC_CAL_EXT_1 0x17
#define VC_CAL_EXT_2 0x18
#define VREF_CAL_EXT 0x1B

void BQ76925Init(void);


void SHOWFIVE(void);
void SHOWANOTHERFIVE(void);


#endif

