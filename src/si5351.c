#include "si5351.h"

void SI5351_IIC_Start(void)
{
	SDA_SI_Out();     //sda线输出
	SDA_SI_ON;	
	Delay1us(1);	
	SCL_SI_ON;
	Delay1us(5);
 	SDA_SI_OFF;//START:when CLK is high,DATA change form high to low 
	Delay1us(5);
	SCL_SI_OFF;//钳住I2C总线，准备发送或接收数据 
}
void SI5351_IIC_Stop(void)
{
	SDA_SI_Out();//sda线输出
	SCL_SI_OFF;
	Delay1us(1);
	SDA_SI_OFF;//STOP:when CLK is high DATA change form low to high
 	Delay1us(5);
	SCL_SI_ON; 
	Delay1us(1);
	SDA_SI_ON;//发送I2C总线结束信号
	Delay1us(5);
//	SCL1_OFF;//钳住I2C总线，准备发送或接收数据 
}
uint8_t SI5351_IIC_Wait_Ack(void)//返回值为1接收失败；返回值为0接收成功
{
	uint8_t ucErrTime=0;
	SDA_SI_In();      //SDA设置为输入  
	Delay1us(1);	   
	SCL_SI_ON;
	Delay1us(1);	 
	while(READ_SDA_SI)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			SI5351_IIC_Stop();
			return 1;
		}
	}
	SCL_SI_OFF;//时钟输出0
//	Delay1us(1);	
	return 0;  
} 
//**************************************
//IIC发送应答信号
//入口参数:ack (0:ACK 1:NAK)
//**************************************
void SI5351_IIC_SendACK(uint8_t i)
{
  if(1==i)SDA_SI_ON;                  //写应答信号
  else SDA_SI_OFF;
  SCL_SI_ON;                    //拉高时钟线
  Delay1us(1);                 //延时
  SCL_SI_OFF ;                  //拉低时钟线
  Delay1us(1);    
} 
void SI5351_IIC_Send_Byte(uint8_t shu)
{
	uint8_t i;
		
	SDA_SI_Out();
	SCL_SI_OFF;
//	Delay1us(1);
  for(i=0;i<8;i++)
  {
		if(shu&0x80)
		{
      SDA_SI_ON;
		}
		else
		{
			SDA_SI_OFF;
		}
		shu = shu<<1;
		Delay1us(3);	
		SCL_SI_ON;
		Delay1us(3);
		SCL_SI_OFF;	
		Delay1us(3);		
	}
}
uint8_t SI5351_IIC_Read_Byte(void)
{
		uint8_t c;
		uint8_t i;
		c = 0x00;
		SDA_SI_In();
//		Delay1us(1);
		for(i=0;i<8;i++)
		{
			SCL_SI_OFF;
			Delay1us(3);
			SCL_SI_ON;
			Delay1us(1);
			c = c<<1;
			if(READ_SDA_SI)
			{
				c = c + 0x01;
			}
			Delay1us(1);
			
		}
		return c;
}
uint8_t SI5351_Write_Byte(uint8_t addr,uint8_t reg,uint8_t data) 				 
{ 
  SI5351_IIC_Start(); 
	SI5351_IIC_Send_Byte(addr|0);//发送器件地址+写命令	
	if(SI5351_IIC_Wait_Ack())	//等待应答
	{
		SI5351_IIC_Stop();		 
		return 1;		
	}
	SI5351_IIC_Send_Byte(reg);//发送寄存器地址
	if(SI5351_IIC_Wait_Ack())	//等待ACK
	{
		SI5351_IIC_Stop();	 
		return 1;		 
	}
	SI5351_IIC_Send_Byte(data);//发送数据
	if(SI5351_IIC_Wait_Ack())	//等待ACK
	{
		SI5351_IIC_Stop();	 
		return 1;		 
	}		 
  SI5351_IIC_Stop();	 
	return 0;
}
uint8_t SI5351_Write_Reg(uint8_t reg,uint8_t data) 				 
{ 
  SI5351_IIC_Start(); 
	SI5351_IIC_Send_Byte(SI5351_ADDR|0);//发送器件地址+写命令	
	if(SI5351_IIC_Wait_Ack())	//等待应答
	{
		SI5351_IIC_Stop();		 
		return 1;		
	}
	SI5351_IIC_Send_Byte(reg);//发送寄存器地址
	if(SI5351_IIC_Wait_Ack())	//等待ACK
	{
		SI5351_IIC_Stop();	 
		return 1;		 
	}
	SI5351_IIC_Send_Byte(data);//发送数据
	if(SI5351_IIC_Wait_Ack())	//等待ACK
	{
		SI5351_IIC_Stop();	 
		return 1;		 
	}		 
  SI5351_IIC_Stop();	 
	return 0;
}
uint8_t SI5351_Read_Byte(uint8_t addr,uint8_t reg)
{
	uint8_t res;
	SI5351_IIC_Start(); 
	SI5351_IIC_Send_Byte(addr|0);//发送器件地址+写命令	
	if(SI5351_IIC_Wait_Ack())	//等待应答
	{
		SI5351_IIC_Stop();		 
		return 1;		
	}
	SI5351_IIC_Send_Byte(reg);//发送寄存器地址
	if(SI5351_IIC_Wait_Ack())	//等待ACK
	{
		SI5351_IIC_Stop();	 
		return 1;		 
	}
  SI5351_IIC_Start(); 
	SI5351_IIC_Send_Byte(addr|1);//发送器件地址+读命令	
	if(SI5351_IIC_Wait_Ack())	//等待应答
	{
		SI5351_IIC_Stop();		 
		return 1;		
	}
	res=SI5351_IIC_Read_Byte();//读取数据,发送nACK 
	SI5351_IIC_SendACK(1);
  SI5351_IIC_Stop();			//产生一个停止条件 
	return res;		
}
void setupPLL(uint8_t pll, uint8_t mult, uint32_t num, uint32_t denom)
{
  uint32_t P1;					// PLL config register P1
  uint32_t P2;					// PLL config register P2
  uint32_t P3;					// PLL config register P3

  P1 = (uint32_t)(128 * ((float)num / (float)denom));
  P1 = (uint32_t)(128 * (uint32_t)(mult) + P1 - 512);
  P2 = (uint32_t)(128 * ((float)num / (float)denom));
  P2 = (uint32_t)(128 * num - denom * P2);
  P3 = denom;

  SI5351_Write_Reg(pll + 0, (P3 & 0x0000FF00) >> 8);
  SI5351_Write_Reg(pll + 1, (P3 & 0x000000FF));
  SI5351_Write_Reg(pll + 2, (P1 & 0x00030000) >> 16);
  SI5351_Write_Reg(pll + 3, (P1 & 0x0000FF00) >> 8);
  SI5351_Write_Reg(pll + 4, (P1 & 0x000000FF));
  SI5351_Write_Reg(pll + 5, ((P3 & 0x000F0000) >> 12) | ((P2 & 0x000F0000) >> 16));
  SI5351_Write_Reg(pll + 6, (P2 & 0x0000FF00) >> 8);
  SI5351_Write_Reg(pll + 7, (P2 & 0x000000FF));
}
void setupMultisynth(uint8_t synth,uint32_t divider,uint8_t rDiv)
{
  uint32_t P1;					// Synth config register P1
  uint32_t P2;					// Synth config register P2
  uint32_t P3;					// Synth config register P3

  P1 = 128 * divider - 512;
  P2 = 0;							// P2 = 0, P3 = 1 forces an integer value for the divider
  P3 = 1;

  SI5351_Write_Reg(synth + 0,   (P3 & 0x0000FF00) >> 8);
  SI5351_Write_Reg(synth + 1,   (P3 & 0x000000FF));
  SI5351_Write_Reg(synth + 2,   ((P1 & 0x00030000) >> 16) | rDiv);
  SI5351_Write_Reg(synth + 3,   (P1 & 0x0000FF00) >> 8);
  SI5351_Write_Reg(synth + 4,   (P1 & 0x000000FF));
  SI5351_Write_Reg(synth + 5,   ((P3 & 0x000F0000) >> 12) | ((P2 & 0x000F0000) >> 16));
  SI5351_Write_Reg(synth + 6,   (P2 & 0x0000FF00) >> 8);
  SI5351_Write_Reg(synth + 7,   (P2 & 0x000000FF));
}


void SI5351ASetFrequency(uint32_t frequency , uint8_t Chanal )
{
  uint32_t pllFreq;
  uint32_t xtalFreq = XTAL_FREQ;// Crystal frequency
  uint32_t l;
  float f;
  uint8_t mult;
  uint32_t num;
  uint32_t denom;
  uint32_t divider;

  divider = 900000000 / frequency;// Calculate the division ratio. 900,000,000 is the maximum internal 
                                                                  // PLL frequency: 900MHz
  if (divider % 2) divider--;		// Ensure an even integer division ratio

  pllFreq = divider * frequency;	// Calculate the pllFrequency: the divider * desired output frequency

  mult = pllFreq / xtalFreq;		// Determine the multiplier to get to the required pllFrequency
  l = pllFreq % xtalFreq;			// It has three parts:
  f = l;							// mult is an integer that must be in the range 15..90
  f *= 1048575;					// num and denom are the fractional parts, the numerator and denominator
  f /= xtalFreq;					// each is 20 bits (range 0..1048575)
  num = f;						// the actual multiplier is  mult + num / denom
  denom = 1048575;				// For simplicity we set the denominator to the maximum 1048575
  // Set up PLL A with the calculated multiplication ratio
  setupPLL(SI_SYNTH_PLL_A, mult, num, denom);
                                                                  // Set up MultiSynth divider 0, with the calculated divider. 
                                                                  // The final R division stage can divide by a power of two, from 1..128. 
                                                                  // reprented by constants SI_R_DIV1 to SI_R_DIV128 (see si5351a.h header file)
                                                                  // If you want to output frequencies below 1MHz, you have to use the 
                                                                  // final R division stage
  if( Chanal == 0 ){
		setupMultisynth(SI_SYNTH_MS_0,divider,SI_R_DIV_1);
                                                                  // Reset the PLL. This causes a glitch in the output. For small changes to 
                                                                  // the parameters, you don't need to reset the PLL, and there is no glitch
		SI5351_Write_Reg(SI_PLL_RESET,0xA0);	
																																	// Finally switch on the CLK0 output (0x4F)
                                                                  // and set the MultiSynth0 input to be PLL A
		SI5351_Write_Reg(SI_CLK0_CONTROL, 0x4F|SI_CLK_SRC_PLL_A);
	}
	else if ( Chanal == 1 ){
		setupMultisynth(SI_SYNTH_MS_1,divider,SI_R_DIV_1);
		SI5351_Write_Reg(SI_PLL_RESET,0xA0);	
		SI5351_Write_Reg(SI_CLK1_CONTROL, 0x4F|SI_CLK_SRC_PLL_A);
	}
		else if ( Chanal == 2 ){
		setupMultisynth(SI_SYNTH_MS_2,divider,SI_R_DIV_1);
		SI5351_Write_Reg(SI_PLL_RESET,0xA0);	
		SI5351_Write_Reg(SI_CLK2_CONTROL, 0x4F|SI_CLK_SRC_PLL_A);
		}
}



