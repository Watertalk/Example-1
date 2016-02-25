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
/*********************初始化*************************/

	 delay_init(168);    //初始化延时函数  168 
   BQ76925Init();
	 Adc_dma_Init();     //ADC初始化
   OLED_Init();		   //初始化OLED
	
	 //电流检测 SENSEN
	 //延时大约2s   电流检测引脚需要一定时间才能达到稳定值

	while(1)
	{
//		/******************电压检测*****************/
		//用OLED显示检测到的每一片电压 
       OLED_SHOW_VOLTAGE();
	} 	    
}


