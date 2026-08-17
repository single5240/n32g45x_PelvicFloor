#ifndef _NFC_H_
#define _NFC_H_

//#include "N76E003.h"
//#include "def.h"
//#include "delay.h"
#include "spi_nfc.h"
//#include "iso14443b.h"
#include "main.h"
	
#define READ   0
#define WRITE  1


#define PICC_CID 0x00 // 0~14 随意指定
#define COM_PKT_CMD_REQB          0x30
#define HEAD 0x68

#define WATER_LEVEL	16 // 
#define FIFO_SIZE	64
#define FSD 256 //Frame Size for proximity coupling Device


#define	READ_REG_CTRL	0x80
#define	TP_FWT_302us	2048
#define TP_dFWT	192 

#define MAX_RX_REQ_WAIT_MS	5000 // 命令等待超时时间100ms

#define BIT7  0X80
#define BIT6  0X40
#define BIT5  0X20
#define BIT4  0X10
#define BIT3  0X08
#define BIT2  0X04
#define BIT1  0X02
#define BIT0  0X01



/*
 * DEFINES Registers bits
 ****************************************************************
 */
#define TxIEn 		BIT6
#define RxIEn 		BIT5
#define IdleIEn		BIT4
#define ErrIEn		BIT1
#define TimerIEn	BIT0
#define TxIRq 		BIT6
#define RxIRq 		BIT5
#define IdleIRq		BIT4
#define ErrIRq		BIT1
#define TimerIRq	BIT0

#define CollErr		BIT3
#define CrcErr		BIT2
#define ParityErr	BIT1
#define ProtocolErr BIT0

#define CollPos		(BIT0|BIT1|BIT2|BIT3|BIT4)

#define RxAlign		(BIT4|BIT5|BIT6)
#define TxLastBits	(BIT0|BIT1|BIT2)

/** 
 * Mifare Error Codes
 * Each function returns a status value, which corresponds to 
 * the mifare error
 * codes. 
 ****************************************************************
 */ 
#define MI_OK														0
#define MI_ERR              					 	2
#define MI_CHK_OK												0
#define MI_CRC_ZERO										 	0

#define MI_CRC_NOTZERO									 1

#define MI_NOTAGERR											(2)
#define MI_CHK_FAILED                   (2)
#define MI_CRCERR												(2)
#define MI_CHK_COMPERR									(2)
#define MI_EMPTY												(3)
#define MI_AUTHERR											(4)
#define MI_PARITYERR										(5)
#define MI_CODEERR											(6)
#define MI_SERNRERR											(8)
#define MI_KEYERR												(9)
#define MI_NOTAUTHERR                   (10)
#define MI_BITCOUNTERR                  (11)
#define MI_BYTECOUNTERR									(12)
#define MI_IDLE													(13)
#define MI_TRANSERR											(14)
#define MI_WRITEERR											(15)
#define MI_INCRERR											(16)
#define MI_DECRERR											(17)
#define MI_READERR											(18)
#define MI_OVFLERR											(19)
#define MI_POLLING											(20)
#define MI_FRAMINGERR                   (21)
#define MI_ACCESSERR                    (22)
#define MI_UNKNOWN_COMMAND							(23)
#define MI_COLLERR											(24)
#define MI_RESETERR											(25)
#define MI_INITERR											(25)
#define MI_INTERFACEERR                 (26)
#define MI_ACCESSTIMEOUT                (27)
#define MI_NOBITWISEANTICOLL						(28)
#define MI_QUIT													(30)
#define MI_INTEGRITY_ERR								(35) //完整性错误(crc/parity/protocol)
#define MI_RECBUF_OVERFLOW              (50) 
#define MI_SENDBYTENR                   (51)	
#define MI_SENDBUF_OVERFLOW             (53)
#define MI_BAUDRATE_NOT_SUPPORTED       (54)
#define MI_SAME_BAUDRATE_REQUIRED       (55)
#define MI_WRONG_PARAMETER_VALUE        (60)
#define MI_BREAK												(99)
#define MI_NY_IMPLEMENTED								(100)
#define MI_NO_MFRC											(101)
#define MI_MFRC_NOTAUTH									(102)
#define MI_WRONG_DES_MODE								(103)
#define MI_HOST_AUTH_FAILED							(104)
#define MI_WRONG_LOAD_MODE							(106)
#define MI_WRONG_DESKEY									(107)
#define MI_MKLOAD_FAILED								(108)
#define MI_FIFOERR											(109)
#define MI_WRONG_ADDR										(110)
#define MI_DESKEYLOAD_FAILED						(111)
#define MI_WRONG_SEL_CNT								(114)
#define MI_WRONG_TEST_MODE							(117)
#define MI_TEST_FAILED									(118)
#define MI_TOC_ERROR										(119)
#define MI_COMM_ABORT										(120)
#define MI_INVALID_BASE									(121)
#define MI_MFRC_RESET										(122)
#define MI_WRONG_VALUE									(123)
#define MI_VALERR												(124)
#define MI_COM_ERR                    	(125)
#define PROTOCOL_ERR										(126)

///用户使用错误
#define USER_ERROR	      						  (127)
#define MAX_TRX_BUF_SIZE								 255

#define UID_4 4
#define UID_7 7
#define FSDI 8 //Frame Size for proximity coupling Device, in EMV test. 身份证必须FSDI = 8





//WS1850命令字
#define PCD_IDLE                0x00               //取消当前命令
#define PCD_AUTHENT             0x0E               //验证密钥
#define PCD_RECEIVE             0x08               //接收数据
#define PCD_TRANSMIT            0x04               //发送数据
#define PCD_TRANSCEIVE          0x0C               //发送并接收数据
#define PCD_RESETPHASE          0x0F               //复位
#define PCD_CALCCRC             0x03               //CRC计算

//Mifare_One卡片命令字
#define PICC_REQIDL             0x26               //寻天线区内未进入休眠状态
#define PICC_REQALL             0x52               //寻天线区内全部卡
#define PICC_ANTICOLL1          0x93               //防冲撞
#define PICC_ANTICOLL2          0x95               //防冲撞
#define PICC_AUTHENT1A          0x60               //验证A密钥
#define PICC_AUTHENT1B          0x61               //验证B密钥
#define PICC_READ               0x30               //读块
#define PICC_WRITE              0xA0               //写块
#define PICC_DECREMENT          0xC0               //扣款
#define PICC_INCREMENT          0xC1               //充值
#define PICC_RESTORE            0xC2               //调块数据到缓冲区
#define PICC_TRANSFER           0xB0               //保存缓冲区中数据
#define PICC_HALT               0x50               //休眠



// FIFO长度定义
//#define DEF_FIFO_LENGTH         64                 //FIFO size=64byte
#define MAXRLEN  									18

//寄存器定义
// PAGE 0
#define 		RFU00                   0x00    
#define 		CommandReg              0x01    
#define 		ComIEnReg               0x02    
#define 		DivIEnReg               0x03    
#define 		ComIrqReg               0x04    
#define 		DivIrqReg               0x05
#define 		ErrorReg                0x06    
#define 		Status1Reg              0x07    
#define 		Status2Reg              0x08    
#define 		FIFODataReg             0x09
#define 		FIFOLevelReg            0x0A
#define 		WaterLevelReg           0x0B
#define 		ControlReg              0x0C
#define 		BitFramingReg           0x0D
#define 		CollReg                 0x0E
#define 		RFU0F                   0x0F
// PAGE 1     
#define     RFU10                 0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxASKReg              0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     RFU1A                 0x1A
#define     RFU1B                 0x1B
#define     MfTxReg	          		0x1C
#define     MfRxReg               0x1D
#define     TypeBReg              0x1E
#define     SerialSpeedReg        0x1F
// PAGE 2    
#define 		RFU20                   0x20  
#define 		CRCResultRegM           0x21
#define 		CRCResultRegL           0x22
#define 		RFU23                   0x23
#define 		ModWidthReg             0x24
#define 		RFU25                   0x25
#define 		RFCfgReg                0x26
#define 		GsNReg                  0x27
#define 		CWGsPReg              	0x28
#define 		ModGsPReg             	0x29
#define 		TModeReg                0x2A
#define 		TPrescalerReg           0x2B
#define 		TReloadRegH             0x2C
#define 		TReloadRegL             0x2D
#define 		TCounterValueRegH       0x2E
#define 		TCounterValueRegL       0x2F
// PAGE 		3      
#define 		RFU30                   0x30
#define 		TestSel1Reg             0x31
#define 		TestSel2Reg             0x32
#define 		TestPinEnReg            0x33
#define 		TestPinValueReg         0x34
#define 		TestBusReg              0x35
#define 		AutoTestReg             0x36
#define 		VersionReg              0x37
#define 		AnalogTestReg           0x38
#define 		TestDAC1Reg             0x39  
#define 		TestDAC2Reg             0x3A   
#define 		TestADCReg              0x3B   
#define 		RFU3C                   0x3C   
#define 		RFU3D                   0x3D   
#define 		RFU3E                   0x3E   
#define 		RFU3F                   0x3F


//#if 1
////V2.0

//#endif



uint8_t SPIReadByte(void);
void SPIWriteByte(uint8_t SPIData);
uint8_t ReadRawRC(uint8_t Address);
void WriteRawRC(uint8_t Address, uint8_t value);
void SetBitMask(uint8_t reg,uint8_t mask);  
void ClearBitMask(uint8_t reg,uint8_t mask);

uint8_t Test_Communication(uint8_t cnt);
void Ws1850s_Init(void);
void PcdReset(void);
void PcdAntennaOn(void);
void PcdAntennaOff(void);
void PcdConfig(uint8_t type);
void PcdSetTmo(uint8_t fwi);
void CheckIrq(unsigned int count);
uint8_t PcdComMF522(uint8_t Command,uint8_t *pInData,uint8_t InLenByte,uint8_t *pOutData,uint16_t *pOutLenBit,unsigned int coun);
uint8_t ComReqA_UID(uint8_t *uid);
uint8_t ComReqA_Block(uint8_t rw,uint8_t Block);
uint8_t ComReqA(uint8_t rw,uint8_t Block);
uint8_t ComReqB(void);
uint8_t PcdRequest(uint8_t req_code,uint8_t *pTagType);
uint8_t PcdFastSearchCard(void);
uint8_t PcdAnticoll(uint8_t *pSnr);
uint8_t PcdSelect(uint8_t *pSnr);
uint8_t PcdAuthState(uint8_t auth_mode,uint8_t addr,uint8_t *pKey,uint8_t *pSnr);
uint8_t PcdRead(uint8_t addr,uint8_t *pData);
uint8_t PcdWrite(uint8_t addr,uint8_t *pData);
void CalulateCRC(uint8_t *pIndata,uint8_t len,uint8_t *pOutData);
void M500PcdConfigISOTypeA(void);
void pcd_lpcd_start(uint8_t delta,uint8_t swingscnt);
void pcd_lpcd_end(void);
void pcd_lpcd_application(void);
uint8_t IC_ver(void);

uint8_t PcdRequestB(uint8_t req_code, uint8_t AFI, uint8_t N, uint8_t *ATQB);
uint8_t PcdSlotMarker(uint8_t N, uint8_t *ATQB);
uint8_t PcdAttriB(uint8_t *PUPI, uint8_t dsi_dri, uint8_t pro_type, uint8_t CID, uint8_t *answer);
uint8_t GetIdcardNum(uint8_t *pid);
uint8_t PcdHaltB(uint8_t *PUPI);
uint8_t SelectSr(uint8_t *chip_id);
uint8_t ReadSr176(uint8_t addr, uint8_t *readdata);
uint8_t WriteSr176(uint8_t addr, uint8_t *writedata);
uint8_t ProtectSr176(uint8_t lockreg);
uint8_t CompletionSr(void);


#endif

