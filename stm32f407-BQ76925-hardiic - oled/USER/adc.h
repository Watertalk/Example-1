#ifndef __ADC_H
#define __ADC_H	
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ADC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
 							  

//ADC1
//PC5 --channel 15  VCOUT
//PB0 --channel 8   VIOUT
//#define 	VTB     Get_Adc_Average(ADC_Channel_0,10)							 

			 
#define   VCOUT 	Get_Adc_Average(ADC_Channel_15,8)	 							 
#define   VIOUT 	Get_Adc_Average(ADC_Channel_8,8)								 
			 				 
void  Adc_dma_Init(void); 				//ADCͨ����ʼ��
u16 Get_Adc(u8 ADC_Channe1);				//���ĳ��ͨ��ֵ 
u16 Get_Adc_Average(u8 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ  






#endif 















