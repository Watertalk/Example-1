#ifndef __OLED_H_
#define __OLED_H_

#include "stm32f4xx_gpio.h"
#include "stm32f4xx.h"
#include "stdint.h"
#include "stdlib.h"	


//-----------------OLED端口定义----------------  					   
#define OLED_CS_Clr()  		GPIO_ResetBits(GPIOD,GPIO_Pin_4)//CS
#define OLED_CS_Set()  		GPIO_SetBits(GPIOD,GPIO_Pin_4)

#define OLED_RST_Clr() 		GPIO_ResetBits(GPIOD,GPIO_Pin_2)//RES
#define OLED_RST_Set() 		GPIO_SetBits(GPIOD,GPIO_Pin_2)

#define OLED_DC_Clr() 		GPIO_ResetBits(GPIOD,GPIO_Pin_3)//DC命令线
#define OLED_DC_Set() 		GPIO_SetBits(GPIOD,GPIO_Pin_3)

#define OLED_SCLK_Clr() 	GPIO_ResetBits(GPIOD,GPIO_Pin_0)//CLK
#define OLED_SCLK_Set() 	GPIO_SetBits(GPIOD,GPIO_Pin_0)

#define OLED_SDIN_Clr() 	GPIO_ResetBits(GPIOD,GPIO_Pin_1)//DIN
#define OLED_SDIN_Set() 	GPIO_SetBits(GPIOD,GPIO_Pin_1)

#define keyway            GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)





#define SIZE							8 //   8/16可选，8小1页一组数  16大2页一组数
#if SIZE==8
#define SIZEGAP           14
#else
#define SIZEGAP           18
#endif
#define XLevelL						0x00
#define XLevelH						0x10
#define Max_Column				128
#define Max_Row						64
#define	Brightness				0xCF 
#define X_WIDTH 					128
#define Y_WIDTH 					64	
#define PAGE							8

#define OLED_CMD  				0					//写命令
#define OLED_DATA 				1					//写数据




void OLED_WR_Byte(uint8_t dat,uint8_t cmd);
void OLED_Set_Pos(uint8_t x, uint8_t y) ;
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(void) ;
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);
u32 oled_pow(u8 m,u8 n);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len);
void OLED_ShowString(uint8_t x,uint8_t page,uint8_t *chr);
void OLED_ShowCHinese(uint8_t x,uint8_t page,uint8_t no);
void OLED_FloatNumber(uint8_t x,uint8_t y, double num);
void OLED_Init(void);
void XINGC_Init(void);


void OLED_GPIO_Init(void);
void GPIO_Init_Pins(GPIO_TypeDef * GPIOx,
										uint16_t GPIO_Pin,
										GPIOMode_TypeDef GPIO_Mode);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Command(unsigned char IIC_Command);

#endif

