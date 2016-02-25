#include "sys.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "stm32BQ76925.h"
#include "oled.h"
#include "stm32f4xx_gpio.h"
#include "delay.h"
#include "adc.h"
#include "voltage.h"
#include "hardiic.h"


extern __IO signed char offset_corr[7];
extern __IO signed char gain_corr[7];
extern vu32  vref_corr;
u16 __IO adc_sensen=0;
u8 i;
double current=0;
double testB=4.0,testC=0;
int main(void)
{ 
    static u8 flag=0;
		GPIO_InitTypeDef GPIO_InitStructure;
/*********************��ʼ��*************************/

	 delay_init(168);    //��ʼ����ʱ����  168 
   BQ76925Init();
	 Adc_dma_Init();     //ADC��ʼ��
   OLED_Init();		   //��ʼ��OLED
	
	 //������� SENSEN
	 //��ʱ��Լ2s   �������������Ҫһ��ʱ����ܴﵽ�ȶ�ֵ

	while(1)
	{
//		/******************��ѹ���*****************/
		//��OLED��ʾ��⵽��ÿһƬ��ѹ 
       OLED_SHOW_VOLTAGE();
	} 	    
}


