#include "nfc.h"
#include "spi_nfc.h"


#if 1
//------------------------------------------
// 读SPI数据 
//------------------------------------------
uint8_t SPIReadByte(void)
{
  static uint8_t SPICount;                                       // Counter used to clock out the data
  static uint8_t SPIData;                  

  SPIData = 0;
  for (SPICount = 0; SPICount < 8; SPICount++)                  // Prepare to clock in the data to be read
  {
    SPIData <<=1;                                               // Rotate the data
    SET_SPI_CK;
    if(STU_SPI_MISO)
    {
      SPIData|=0x01;
    }                                         
 		CLR_SPI_CK;
  }                                                             // and loop back
  return SPIData;                              // Finally return the read data
} 

//------------------------------------------
// 写SPI数据 
//------------------------------------------
void SPIWriteByte(uint8_t SPIData)
{
  static uint8_t SPICount;                                       // Counter used to clock out the data

  for(SPICount = 0; SPICount < 8; SPICount++)
  {
    if (SPIData & 0x80)
    {
      SET_SPI_MOSI;
    }
    else
    {
      CLR_SPI_MOSI;
    } 
 
    SET_SPI_CK;   
    CLR_SPI_CK;

    
    SPIData <<= 1;
  }          	
} 
#endif



/////////////////////////////////////////////////////////////////////
//功    能：读寄存器
//参数说明：Address[IN]:寄存器地址
//返    回：读出的值
/////////////////////////////////////////////////////////////////////
uint8_t ReadRawRC(uint8_t Address)
{
  static uint8_t ucAddr;
  static uint8_t ucResult;
	
	ucResult=0;
	
  CLR_SPI_CS;
  ucAddr = ((Address<<1)&0x7E)|0x80;

  SPIWriteByte(ucAddr);
  ucResult=SPIReadByte();
  SET_SPI_CS;
  return ucResult;
}

/////////////////////////////////////////////////////////////////////
//功    能：写寄存器
//参数说明：Address[IN]:寄存器地址
//          value[IN]:写入的值
/////////////////////////////////////////////////////////////////////
void WriteRawRC(uint8_t Address, uint8_t value)
{  
  static uint8_t ucAddr;

  CLR_SPI_CS;
  ucAddr = ((Address<<1)&0x7E);

  SPIWriteByte(ucAddr);
  SPIWriteByte(value);
  SET_SPI_CS;
}

/////////////////////////////////////////////////////////////////////
//功    能：置寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:置位值
/////////////////////////////////////////////////////////////////////
void SetBitMask(uint8_t reg,uint8_t mask)  
{
  static uint8_t tmp;
	
	tmp = 0x0;
	
  tmp = ReadRawRC(reg);
  WriteRawRC(reg,tmp | mask);  // set bit mask
}

/////////////////////////////////////////////////////////////////////
//功    能：清寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:清位值
/////////////////////////////////////////////////////////////////////
void ClearBitMask(uint8_t reg,uint8_t mask)  
{
  static uint8_t tmp;
	
	tmp = 0x0;
  tmp = ReadRawRC(reg);
  WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 

