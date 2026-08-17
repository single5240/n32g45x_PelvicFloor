
/**
  ******************************************************************************
  * @author  全威智能科技
  * @version V1.2
  * @date    2021-01-19
  * @brief   init配置H文件
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#ifndef __HF68XX_H__
#define __HF68XX_H__


/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
//#include "ddl.h"
//#include "init.h"
#include "init.h"
/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
#define SOUND_DATA_ON 			GPIO_WriteBit(SOUND_DDD_PORT,SOUND_DDD_PIN,Bit_SET)
#define SOUND_DATA_OFF			GPIO_WriteBit(SOUND_DDD_PORT,SOUND_DDD_PIN,Bit_RESET)
#define SOUND_RST_ON				GPIO_WriteBit(SOUND_RST_PORT,SOUND_RST_PIN,Bit_SET)
#define SOUND_RST_OFF				GPIO_WriteBit(SOUND_RST_PORT,SOUND_RST_PIN,Bit_RESET)
#define STU_SOUND_BUSY			GPIO_ReadInputDataBit(SOUND_BUSY_PORT,SOUND_BUSY_PIN)

void SglSound(uint8_t ss);
void MultSounds(uint8_t ss1, uint8_t ss2, uint8_t ss3, uint8_t ss4, uint8_t ss5, uint8_t ss6);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
