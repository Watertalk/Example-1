#include "stm32BQ76925.h" 
#include "delay.h" 
#include "myiic.h"
#include "stm32f4xx.h"
#include "lcd.h"
#include "24cxx.h"
#include "oled.h"
#include "voltage.h"
#include "adc.h"
#include "hardiic.h"

extern signed char offset_corr[7];
extern signed char gain_corr[7];
vu32  vref_corr=0;


const __IO u8 Config_reg[][2]={
	{0x03,0x00},//CONFIG_1   ���÷Ŵ��� �Լ�SENSEN/SENSEP respect to VSS
	{0x04,0x01},//CONGIG_2   0x01��׼��ѹ3.0  ��������2   0x00��׼��ѹ1.5 ��������1  REF_SEL 
	{0x05,0x0D},//POWER_CTL  enable Current comparator and amplifier ,Cell amplifier ,Voltage reference
	{0x00,0x00},//STATUS     
  {0x02,0x00},//BAL_CTL
	{0x01,0x10},//CELL_CTL   ���ò���ĳƬ��ص�ѹ
};

void BQ76925Init(void)
{
	u8 i=0;
	double VREF;
	HardI2cInit();

	for(i=0;i<6;i++)
	{
		I2C_write_onebyte(Config_reg[i][0],Config_reg[i][1]);
		delay_ms(32);
	} 

	CorrInit();//��ȡ�Ĵ�����ֵ
	 
	/*********�����׼��ѹ***************/
	 if(REF_SEL)  
			 VREF=3;
	 else      
			 VREF=1.5;
  vref_corr=VREF*(1000+gain_corr[0])+offset_corr[0];
	 
}





