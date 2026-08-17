#include "lcd_init.h"
//#include "delay.h"
//#include "init.h"

/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_WriteByte(uint8_t dat) 
{	
	uint8_t i;
	CS_OFF;
//	Delay1us(1);
	for(i=0;i<8;i++)
	{			  
		SCL_OFF;
//		Delay1us(1);
		if(dat&0x80)
			SDA_ON;
		else
			SDA_OFF;
//		Delay1us(1);
		SCL_ON;
//    Delay1us(1);
		dat<<=1;
	}	
  CS_ON;	
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_Dat8(uint8_t dat)
{
	LCD_WriteByte(dat);
}//

/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_Dat16(uint16_t dat)
{
	LCD_WriteByte((uint8_t)(dat>>8));
	LCD_WriteByte((uint8_t)(dat&0x0ff));
}

// 
/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令 
      返回值：  无
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	DC_OFF;//写命令 
//	Delay1us(1);
	LCD_WriteByte(dat);
	DC_ON;//写数据
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_Dat16(x1);
		LCD_WR_Dat16(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_Dat16(y1);
		LCD_WR_Dat16(y2);
		LCD_WR_REG(0x2c);//储存器写
}

void LCD_Init(void)
{
	
	RES_OFF;//复位
	Delay1ms(100);
	RES_ON;
	Delay1ms(100);
	
	
	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11); ////从休眠中唤醒
	Delay1ms(120); 
	LCD_WR_REG(0x36);////显示方式 
	LCD_WR_Dat8(0x00);////从上到下，从左到右，普通模式，RGB
//	if(USE_HORIZONTAL==0)LCD_WR_Dat8(0x00);
//	else if(USE_HORIZONTAL==1)LCD_WR_Dat8(0xC0);
//	else if(USE_HORIZONTAL==2)LCD_WR_Dat8(0x70);
//	else LCD_WR_Dat8(0xA0);

	LCD_WR_REG(0x3A);///颜色模式
	LCD_WR_Dat8(0x05);////16BIT

	LCD_WR_REG(0xB2);/////Porch Setting
	LCD_WR_Dat8(0x0C);
	LCD_WR_Dat8(0x0C);
	LCD_WR_Dat8(0x00);
	LCD_WR_Dat8(0x33);
	LCD_WR_Dat8(0x33); 

	LCD_WR_REG(0xB7); ////gate control
	LCD_WR_Dat8(0x35);  

	LCD_WR_REG(0xBB);////vcoms Setting
	LCD_WR_Dat8(0x19);

	LCD_WR_REG(0xC0);/////lcm control
	LCD_WR_Dat8(0x2C);

	LCD_WR_REG(0xC2);
	LCD_WR_Dat8(0x01);

	LCD_WR_REG(0xC3);
	LCD_WR_Dat8(0x12);   

	LCD_WR_REG(0xC4);
	LCD_WR_Dat8(0x20);  

	LCD_WR_REG(0xC6); 
	LCD_WR_Dat8(0x0F);    

	LCD_WR_REG(0xD0); 
	LCD_WR_Dat8(0xA4);
	LCD_WR_Dat8(0xA1);

	LCD_WR_REG(0xE0);
	LCD_WR_Dat8(0xD0);
	LCD_WR_Dat8(0x04);
	LCD_WR_Dat8(0x0D);
	LCD_WR_Dat8(0x11);
	LCD_WR_Dat8(0x13);
	LCD_WR_Dat8(0x2B);
	LCD_WR_Dat8(0x3F);
	LCD_WR_Dat8(0x54);
	LCD_WR_Dat8(0x4C);
	LCD_WR_Dat8(0x18);
	LCD_WR_Dat8(0x0D);
	LCD_WR_Dat8(0x0B);
	LCD_WR_Dat8(0x1F);
	LCD_WR_Dat8(0x23);

	LCD_WR_REG(0xE1);
	LCD_WR_Dat8(0xD0);
	LCD_WR_Dat8(0x04);
	LCD_WR_Dat8(0x0C);
	LCD_WR_Dat8(0x11);
	LCD_WR_Dat8(0x13);
	LCD_WR_Dat8(0x2C);
	LCD_WR_Dat8(0x3F);
	LCD_WR_Dat8(0x44);
	LCD_WR_Dat8(0x51);
	LCD_WR_Dat8(0x2F);
	LCD_WR_Dat8(0x1F);
	LCD_WR_Dat8(0x1F);
	LCD_WR_Dat8(0x20);
	LCD_WR_Dat8(0x23);

	LCD_WR_REG(0x21); 

	LCD_WR_REG(0x29); 
} 








