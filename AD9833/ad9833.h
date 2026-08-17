/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
**
 * ****************************************************************************/

/**
 * @file delay.h
<<<<<<< .mine
 * @author Nations 
=======
 * @author Nations
>>>>>>> .r78072
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#ifndef __AD9833_H__
#define __AD9833_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g45x.h"
#include "main.h"
#include "init.h"

#define TRI_WAVE 	0  		//输出三角波
#define SIN_WAVE 	1		//输出正弦波
#define SQU_WAVE 	2		//输出方波

//#define SDATA_ON 							GPIO_WriteBit(SDATA_PORT,SDATA_PIN,Bit_SET)
//#define SDATA_OFF 						GPIO_WriteBit(SDATA_PORT,SDATA_PIN,Bit_RESET)

//#define SCLK_ON 							GPIO_WriteBit(SCLK_PORT,SCLK_PIN,Bit_SET)
//#define SCLK_OFF 							GPIO_WriteBit(SCLK_PORT,SCLK_PIN,Bit_RESET)

//#define SCS_ON 								GPIO_WriteBit(SCS_PORT,SCS_PIN,Bit_SET)
//#define SCS_OFF 							GPIO_WriteBit(SCS_PORT,SCS_PIN,Bit_RESET)

static void ad9833_Delay(void);	
void Write_ad9833(uint16_t d);
void AD9833_WaveSeting(double Freq,uint16_t Freq_SFR,uint16_t WaveMode,uint16_t Phase);


#ifdef __cplusplus
}
#endif

#endif /* __AD9833_H__ */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */



