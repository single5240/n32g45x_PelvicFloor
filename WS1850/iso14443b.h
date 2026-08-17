/**
 ****************************************************************
 * @file iso14443b.h
 *
 * @brief 
 *
 * @author 
 *
 *
 ****************************************************************
 */
//#include "N76E003.h"
#include "main.h"
 
#ifndef ISO_14443B_H
#define ISO_14443B_H
/////////////////////////////////////////////////////////////////////
//ISO14443B COMMAND
///////////////////////////////////////////////////////////////////// 
#define	ISO14443B_ANTICOLLISION                  0x05
#define	ISO14443B_ATTRIB                         0x1D
#define	ISO14443B_HLTB                           0x50

#define FSDI 8 //Frame Size for proximity coupling Device, in EMV test. 身份证必须FSDI = 8

/////////////////////////////////////////////////////////////////////
//函数原型
/////////////////////////////////////////////////////////////////////
uint8_t PcdRequestB(uint8_t req_code, uint8_t AFI, uint8_t N, uint8_t *ATQB);
uint8_t PcdSlotMarker(uint8_t N, uint8_t *ATQB);
uint8_t PcdAttriB(uint8_t *PUPI, uint8_t dsi_dri, uint8_t pro_type,uint8_t CID, uint8_t *answer);
uint8_t GetIdcardNum(uint8_t *ATQB);

#endif
