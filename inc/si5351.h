#ifndef _SI5351_H_
#define _SI5351_H_
//#include "n32g030.h"
#include "main.h"
//#include "init.h"
#include <stdio.h>
#include <stdint.h>   												  	  

#define SI5351_ADDR				0xc0

#define SCL_SI_ON 								GPIO_WriteBit(SCL_SI_PORT,SCL_SI_PIN,Bit_SET)
#define SCL_SI_OFF 								GPIO_WriteBit(SCL_SI_PORT,SCL_SI_PIN,Bit_RESET)

#define SDA_SI_ON 								GPIO_WriteBit(SDA_SI_PORT,SDA_SI_PIN,Bit_SET)
#define SDA_SI_OFF 								GPIO_WriteBit(SDA_SI_PORT,SDA_SI_PIN,Bit_RESET)

#define READ_SDA_SI								GPIO_ReadInputDataBit(SDA_SI_PORT,SDA_SI_PIN)

#define SI_CLK0_CONTROL	16			// Register definitions
#define SI_CLK1_CONTROL	17
#define SI_CLK2_CONTROL	18
#define SI_SYNTH_PLL_A	26
#define SI_SYNTH_PLL_B	34
#define SI_SYNTH_MS_0		42
#define SI_SYNTH_MS_1		50
#define SI_SYNTH_MS_2		58
#define SI_PLL_RESET		177

#define SI_R_DIV_1		0x00			// R-division ratio definitions
#define SI_R_DIV_2		0b00010000
#define SI_R_DIV_4		0b00100000
#define SI_R_DIV_8		0b00110000
#define SI_R_DIV_16		0b01000000
#define SI_R_DIV_32		0b01010000
#define SI_R_DIV_64		0b01100000
#define SI_R_DIV_128		0b01110000

#define SI_CLK_SRC_PLL_A	0x00
#define SI_CLK_SRC_PLL_B	0b00100000
#define XTAL_FREQ	25000000			// Crystal frequency


void SI5351_IIC_Start(void);
void SI5351_IIC_Stop(void);
uint8_t SI5351_IIC_Wait_Ack(void);
void SI5351_IIC_SendACK(uint8_t i);
uint8_t SI5351_Write_Byte(uint8_t addr,uint8_t reg,uint8_t data);
uint8_t SI5351_Write_Reg(uint8_t reg,uint8_t data) ;
uint8_t SI5351_Read_Byte(uint8_t addr,uint8_t reg);
void SI5351_IIC_Send_Byte(uint8_t shu);
uint8_t SI5351_IIC_Read_Byte(void);
void setupPLL(uint8_t pll, uint8_t mult, uint32_t num, uint32_t denom);
void setupMultisynth(uint8_t synth,uint32_t divider,uint8_t rDiv);
void SI5351ASetFrequency(uint32_t frequency , uint8_t Chanal );

#endif
