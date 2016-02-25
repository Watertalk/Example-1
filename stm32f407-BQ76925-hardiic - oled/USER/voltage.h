#ifndef _VOLTAGE_H
#define _VOLTAGE_H
#include "sys.h"
#define   Full_Scale  4096  //2^12	
#define   R_current  1     //À©´ó1000±¶

#define REF_SEL    (I2C_read_onebyte(0x04)&0x01)
#define I_Gain      (I2C_read_onebyte(0x03)&0x01)
double GET_VCN(u8 n);
double GET_Current();
void CorrInit();
void OLED_SHOW_VOLTAGE(void);
#endif

