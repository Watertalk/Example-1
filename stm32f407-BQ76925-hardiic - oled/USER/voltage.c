#include "voltage.h"
#include "sys.h"
#include "stm32BQ76925.h" 
#include "adc.h"
#include "oled.h"
#include "delay.h"
#include "hardiic.h"

__IO signed char offset_corr[7]={0};
__IO signed char gain_corr[7]={0};
extern vu32  vref_corr;
extern __IO u16 adc_sensen;


//将只读寄存器中的增益抵消取出来 
void CorrInit()   
{
	u8 i;
	u8 temp_corr;
	for(i=0;i<7;i++)
	{
		temp_corr=I2C_read_onebyte(VREF_CAL|i);
		offset_corr[i]=temp_corr>>4;
		gain_corr[i]=temp_corr&0x0F;
	}
	
	temp_corr=I2C_read_onebyte(VREF_CAL_EXT);
	offset_corr[0] |=(((temp_corr&0x06)<<3)^0x20)-0x20;
	gain_corr[0]   |=(((temp_corr&0x01)<<4)^0x10)-0x10;
	
	temp_corr=I2C_read_onebyte(VC_CAL_EXT_1);
	offset_corr[1] |=(((temp_corr&0x80)>>3)^0x10)-0x10;
	gain_corr[1]   |=(((temp_corr&0x40)>>2)^0x10)-0x10;
	
	offset_corr[2] |=(((temp_corr&0x20)>>1)^0x10)-0x10;
	gain_corr[2]   |=((temp_corr&0x10)^0x10)-0x10;
	
	temp_corr=I2C_read_onebyte(VC_CAL_EXT_2);
	offset_corr[3] |=(((temp_corr&0x80)>>3)^0x10)-0x10;
	gain_corr[3]   |=(((temp_corr&0x40)>>2)^0x10)-0x10;
	
	offset_corr[4] |=(((temp_corr&0x20)>>1)^0x10)-0x10;
	gain_corr[4]   |=((temp_corr&0x10)^0x10)-0x10;
	
	offset_corr[5] |=(((temp_corr&0x08)<<1)^0x10)-0x10;
	gain_corr[5]   |=(((temp_corr&0x04)<<2)^0x10)-0x10;

  offset_corr[6] |=(((temp_corr&0x02)<<3)^0x10)-0x10;
	gain_corr[6]   |=(((temp_corr&0x01)<<4)^0x10)-0x10;
  
}



double GET_Current(void)
{
	
	double current=0;
  long long int temp=0;
  static unsigned int adc=0;
  u8  VIOUT_G=0;
	
	adc=VIOUT;//获取SENSEP 的测量值
	
	//放大倍数
	if(I_Gain)  
		 VIOUT_G=8; 
	else  
		 VIOUT_G=4;

	//SENSEN==SENSEP 时，表示没有电流  计算补偿
	if(adc>adc_sensen)
	{
		temp=adc-adc_sensen; 
		temp=temp/VIOUT_G;
	}
	else if(adc==adc_sensen)
	       temp=adc-adc_sensen;
		   else
			{
		   temp=adc_sensen-adc;				
		   temp=temp/VIOUT_G;
			
	     }

	temp=temp*vref_corr;
	current=(double)temp/Full_Scale/1e3; 
  current=current/0.001;
	return current;
}

//////////////////
//读取每一片电池电压
//向BQ76925 写入 读取某片电池电压
//延长32ms 确保ＡＤＣ通道读取正确值
////////////////

double GET_VCN(u8 n)
{
	double vcn=0;
	double G_vout=0;
	long long int temp_V=0;
	double VREF;
	__IO u8 m_data=0;
	
	m_data=0x10|(n-1);
	//HardI2cInit();
	I2C_write_onebyte(0x01,m_data); 
	delay_ms(32);

   //基准电压
	if(REF_SEL)  
	{
		G_vout=0.6;
		VREF=3;
	}
	else    
	{
		G_vout=0.3;
    VREF=1.5;
	}
	
    //计算电压值
	
    temp_V=(VCOUT*vref_corr+Full_Scale*offset_corr[n]);
	  temp_V=temp_V*(1000+gain_corr[n]);
	  vcn=((temp_V/4096)/1e6);
		vcn=vcn/G_vout;

	
	return vcn;
}
	  
//用ＯＬＥＤ显示电池电压
void OLED_SHOW_VOLTAGE(void)
{
    double m_vout_cn[6]={0.0};
		double m_vout_all[6]={0.0};

	   u8 i=0;
	/*******************获取各个电压****************************/	
	for(i=0;i<6;i++)
	{
		m_vout_cn[i]=GET_VCN(i+1);
	
		if(i==0)
		   m_vout_all[i]=m_vout_cn[i];
		else
			m_vout_all[i]=m_vout_all[i-1]+m_vout_cn[1];
	}
	for(i=0;i<6;i++)
	{
			OLED_FloatNumber(0,i+2,m_vout_cn[i]);
		  OLED_FloatNumber(60,i+2,m_vout_all[i]);
	}
//		 m_vout_cn[0]=GET_VCN(1); //获取第一片电压值
//      OLED_FloatNumber(0,i+2,m_vout_cn[0]);
//	
//     m_vout_cn[1]=GET_VCN(2);
//	   OLED_FloatNumber(0,3,m_vout_cn[1]);
//	
//		 m_vout_cn[2]=GET_VCN(3);
//	   OLED_FloatNumber(0,4,m_vout_cn[2]);
//	
//		 m_vout_cn[3]=GET_VCN(4);
//	   OLED_FloatNumber(0,5,m_vout_cn[3]);
//		
//		 m_vout_cn[4]=GET_VCN(5);
//	   OLED_FloatNumber(0,6,m_vout_cn[4]);

//		 m_vout_cn[5]=GET_VCN(6);
//		 OLED_FloatNumber(0,7,m_vout_cn[5]);
//		
//	/*******************计算各个电压和****************************/	
//	   m_vout_all[0]=m_vout_cn[0];
//		 m_vout_all[1]=m_vout_all[0]+m_vout_cn[1];
//		 m_vout_all[2]=m_vout_all[1]+m_vout_cn[2];
//		 m_vout_all[3]=m_vout_all[2]+m_vout_cn[3];
//		 m_vout_all[4]=m_vout_all[3]+m_vout_cn[4];
//		 m_vout_all[5]=m_vout_all[4]+m_vout_cn[5];
//	/*******************OLED显示各个电压和****************************/
//		
//			OLED_FloatNumber(60,2,m_vout_all[0]);
//			OLED_FloatNumber(60,3,m_vout_all[1]);
//			OLED_FloatNumber(60,4,m_vout_all[2]);
//			OLED_FloatNumber(60,5,m_vout_all[3]);
//		  OLED_FloatNumber(60,6,m_vout_all[4]);
//	    OLED_FloatNumber(60,7,m_vout_all[5]);
}

