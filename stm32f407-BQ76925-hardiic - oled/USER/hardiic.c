#include "delay.h"
#include "stm32f4xx.h"
#include "hardiic.h"

void HardI2cInit(void)
{
	  I2C_InitTypeDef I2C_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
	 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_I2C2);
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_I2C2);
	
    /*STM32F407芯片的硬件I2C: PB10 -- SCL; PB11 -- SDA */
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	  I2C_DeInit(I2C2);	
	  I2C_InitStructure.I2C_Mode=I2C_Mode_I2C; 
	  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	  I2C_InitStructure.I2C_Ack=I2C_Ack_Enable;
	  I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
	  I2C_InitStructure.I2C_ClockSpeed=400000; 
	 
	  I2C_Cmd(I2C2, ENABLE);
	  I2C_Init(I2C2, &I2C_InitStructure);
	 
	  I2C_AcknowledgeConfig(I2C2,ENABLE);   //在接收到一个数据后给一个应答
		   
}


void I2C_write_onebyte(u16 WriteAddr,u8 DataToWrite)
{
	 u16  address;
	 address=((0x04<<3)+WriteAddr)<<1;
	
	 I2C_AcknowledgeConfig(I2C2,ENABLE);//循环读写不可少
	
	 while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY)); 
	
	 I2C_GenerateSTART(I2C2, ENABLE);
	 delay_ms(2);
	 /*      EV5          */
	 while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/
   I2C_Send7bitAddress(I2C2,address,I2C_Direction_Transmitter); 
	
	/*     EV6   卡在这里是因为上面的从机地址没写对  */
  while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C2,DataToWrite);
	
	/*        EV8_2   !!     */
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(I2C2,ENABLE);
}


u8 I2C_read_onebyte(u16 ReadAddr)
{
	  u8 temp=0;
	 while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
		
   I2C_GenerateSTART(I2C2, ENABLE);
	
	 //EV5事件
   while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));
   I2C_Send7bitAddress(I2C2,((0x04<<3)+ReadAddr)<<1, I2C_Direction_Receiver); 
	
	 //EV6 EV8_1事件（该事件判断同时判断了EV8_1事件）   
	 while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));	 
			 
  //EV7事件
	 while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED));

	 temp= I2C_ReceiveData(I2C2);		 

	 I2C_NACKPositionConfig(I2C2,DISABLE);   //失能ACK

	 I2C_GenerateSTOP(I2C2,ENABLE);  
	        
		return temp;
}

