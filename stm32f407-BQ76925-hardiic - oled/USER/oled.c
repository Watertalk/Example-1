#include "oled.h"
#include "stm32f4xx_gpio.h"
#include "oledfont.h"
#include "delay.h"
#include "myiic.h"
/////����
//CS  PD4
//RES  PD2
//DC   PD3
//D0   CLK PD0 
//D1  DIN     PD1   
///////

void OLED_GPIO_Init(void)
{
	GPIO_Init_Pins(GPIOD,GPIO_Pin_0,GPIO_Mode_OUT);
	GPIO_Init_Pins(GPIOD,GPIO_Pin_1,GPIO_Mode_OUT);
	GPIO_Init_Pins(GPIOD,GPIO_Pin_3,GPIO_Mode_OUT);
	GPIO_Init_Pins(GPIOD,GPIO_Pin_2,GPIO_Mode_OUT);
	GPIO_Init_Pins(GPIOD,GPIO_Pin_4,GPIO_Mode_OUT);
}

void XINGC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);	
	
}


/**
  * @brief  ��SSD1106д��һ���ֽ�  ��Ҫ���ڼĴ���������
  *
  * @param  dat:Ҫд�������/����
  * @param  cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
  *     @arg 
  * @retval None 
  */
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   IIC_Send_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
   IIC_Send_Byte(0x40);			//write data
	IIC_Wait_Ack();	
   IIC_Send_Byte(IIC_Data);
	IIC_Wait_Ack();	
   IIC_Stop();
}
void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   IIC_Send_Byte(0x78);            //Slave address,SA0=0
	if(IIC_Wait_Ack())
	{
		while(1);
	}
   IIC_Send_Byte(0x00);			//write command
	IIC_Wait_Ack();	
   IIC_Send_Byte(IIC_Command); 
	IIC_Wait_Ack();	
   IIC_Stop();
}
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)  
{	
	uint8_t i;			 
	
	if(cmd)
	  Write_IIC_Data(dat);
	else
		 Write_IIC_Command(dat); 
} 

/**
  * @brief
  *
  * @param
  *     @arg 
  * @retval None
  */
void OLED_Set_Pos(uint8_t x, uint8_t y) 
{ 
	OLED_WR_Byte(0xb0+(y & 0x07),OLED_CMD);/* set page start address */
	OLED_WR_Byte(x & 0x0f,OLED_CMD); /* set page start address */
	OLED_WR_Byte(((x & 0xf0)>>4)|0x10,OLED_CMD); /* set higher nibble of the column address */
}   	  
/**
  * @brief  ����OLED��ʾ    
  *
  * @param
  *
  * @retval None
  */
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

/**
  * @brief  �ر�OLED��ʾ    
  *
  * @param
  *
  * @retval None
  */    
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
	

}		   		

/**
  * @brief ��������,������,������Ļ�Ǻ�ɫ��,��û����һ��
  *
  * @param  None
  * 
  * @retval None
  */
void OLED_Clear(void)  
{  
	uint8_t page,x;	    
	for(page=0; page<PAGE; page++)  
	{  
		OLED_WR_Byte (0xb0 + page,OLED_CMD);    				//����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      							//������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      							//������ʾλ�á��иߵ�ַ   
		
		for(x=0; x<X_WIDTH; x++){
			OLED_WR_Byte(0,OLED_DATA); 
		}
	}

}

/**
  * @brief  ��ָ��λ����ʾһ���ַ�,���������ַ�
  *
  * @param  x:0~127 y:0~63
  *     @arg mode:0,������ʾ;1,������ʾ			size:ѡ������ 16/12 
  * @retval None
  */
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr)
{      	
	uint8_t c=0,i=0;	
	c=chr-' ';																	//�õ�ƫ�ƺ��ֵ			
	
	if(x > X_WIDTH - 1)
	{
		x = 0;
		y += 2;
	}
	if(SIZE == 16)
	{
		OLED_Set_Pos(x,y);	
		
		for(i=0;i<8;i++){   //8��һ������
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
		}
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++){
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
		}
	}
	else	if(SIZE==8)
		{
			OLED_Set_Pos(x,y);	
			for(i=0;i<6;i++){   
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
			}
		}
	else 
	{	
		OLED_Set_Pos(x,y+1);	
		for(i=0;i<6;i++){
			OLED_WR_Byte(F6x8[c][i],OLED_DATA);
		}		
	}
}

/**
  * @brief  ����m��n�η�
  *
  * @param  ������ָ��
  *     
  * @retval �ݵ�ֵ
  */
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)
	{
		result *= m;    
	}
	return result;
}				  
	
/**
  * @brief  ��ʾ2������
  *
  * @param  x,y :�������	  len :���ֵ�λ��  size:�����С mode:ģʽ	0,���ģʽ;1,����ģʽ num:��ֵ(0~4294967295);
  *     @arg 
  * @retval None
  */
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;				
	
	for(t=0; t<len; t++){ 
		temp=(num/oled_pow(10,len-t-1))%10;//�Ѷ�λ������ֵ�ֳ���  �Ӹߵ���
		
		if(enshow == 0 && t < (len-1))  //�ų����һλ��  ��һλ
		{
			if(temp == 0)
			{
				OLED_ShowChar( x+(SIZEGAP/2)*t, y,'0');//���λΪ0����ʾΪ��
				continue;
			}
			else
			{
				enshow=1; 
			}		 	 
		}
	 	OLED_ShowChar( x+(SIZEGAP/2)*t, y, temp + '0'); 
	}
} 

/**
  * @brief  ��ʾһ���ַ���
  *
  * @param	x px��page���Լ���Ҫ��ʾ���ַ���
  *     
  * @retval None
  */
void OLED_ShowString(uint8_t x,uint8_t page,uint8_t *chr)
{
	uint8_t j=0;
	for(j=0; chr[j] != '\0'; j++)
	{
		OLED_ShowChar(x,page,chr[j]);
		if(SIZE==8)
		{
			x += 6;
			if(x > 122)  //һ�н�����ת��һ��
			{
				x = 0;
				page += 2;
			}
	  }
		else if(SIZE==16)
		{
			x += 8;
			if(x > 120)
			{
				x = 0;
				page += 2;
			}
	  }
		
	}
}

/**
  * @brief  ��ʾһ�����֣���oledfont.h�У�   �˲���û���޸�
  *
  * @param
  *     @arg 
  * @retval None
  */
void OLED_ShowCHinese(uint8_t x,uint8_t page,uint8_t no)
{      			    
	uint8_t t,adder=0;
	OLED_Set_Pos(x,page);	
	
	for(t=0; t<16; t++)
	{
		OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
		adder+=1;
	}	
	
	OLED_Set_Pos(x,page + 1);	
	
	for(t=0; t<16; t++)
	{	
		OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
		adder += 1;
	}					
}

/**
  * @brief  ��ʾһ����������С���������3λ
  *
  * @param  ������꣬���֣���С
  *     @arg 
  * @retval None
  */
void OLED_FloatNumber(uint8_t x,uint8_t y, double num)
{
	u32 integerPart;
	u8 integerPartNum;
	u8  gap=0;
	if(num <0 ){
		num = -num;
		OLED_ShowString(x,y,(uint8_t *)"-");
	}
	
	//����ʾЧ����һ��
	if(SIZE==8)
	{
		x += 7;
		if(x > 121)
		{
			x = 0;
			y += 2;
		}	
	}
	
	if(SIZE==16)
	{
		x += 10;
		if(x > 128)
		{
			x = 0;
			y += 2;
		}	
  }
	
	//������������  ������
	for(integerPartNum = 0,integerPart = (u32)num; integerPart != 0; integerPartNum++){
		integerPart /= 10;
	}
	if(SIZE==8)
		gap=6;
	else if(SIZE==16)
		gap=8;
	if(x > 128 - (integerPartNum+4)*gap){
		x = 0;
		y += 2;
	}

	OLED_ShowNum(x,y,(int32_t)num,integerPartNum);	
	x += integerPartNum*gap;	
	OLED_ShowString(x,y,(uint8_t *)".");
	x += gap;
	OLED_ShowNum(x,y,(int32_t)((num-integerPart)*100000),5);
}

/**
  * @brief  ��ʼ��SSD1306			
  *
  * @param
  *     @arg 
  * @retval None
  */	    
void OLED_Init(void)
{ 	
  
	
  IIC_Init();
					  
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel

	OLED_Clear();
	OLED_Set_Pos(0,0); 	

}  































void GPIO_Init_Pins(GPIO_TypeDef * GPIOx,
										uint16_t GPIO_Pin,
										GPIOMode_TypeDef GPIO_Mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	  /* Enable GPIOx, clock */  
  switch((uint32_t)GPIOx)
  {
		case GPIOA_BASE: 
    {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			break;
    }
		case GPIOB_BASE: 
    {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
			break;
    }
		case GPIOC_BASE: 
    {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
			break;
    }
		case GPIOD_BASE: 
    {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
			break;
    }
		case GPIOE_BASE: 
    {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
			break;
    }
		case GPIOF_BASE: 
    {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
			break;
    }
		case GPIOG_BASE: 
    {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
			break;
    }
		case GPIOH_BASE: 
    {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
			break;
    }
		case GPIOI_BASE: 
    {
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
			break;
    }	
		default: break;
  }
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode;

	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		
	GPIO_Init(GPIOx,&GPIO_InitStructure);	
	GPIO_SetBits(GPIOx,GPIO_Pin);
}









