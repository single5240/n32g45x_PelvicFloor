#include "nfc.h"
//#include "delay.h"
//#include "iso14443b.h"
#include <stdio.h>
#include <string.h>
//#include "def.h"


uint8_t PassWd[6]={0xff,0xff,0xff,0xff,0xff,0xff};
uint8_t RWDATA[16]={0x0f,0x1f,0x2f,0x3f,0x4f,0x5f,0x6f,0x7f,0x8f,0x9f,0xaf,0xbf,0xcf,0xdf,0xef,0xff};

uint8_t  IDB[10];//放置B 卡ID
uint8_t  IDA[10];//放置A 卡ID
uint8_t CT[2];//卡类型

/////////////////////////////////////////////////////////////////////
//功    能：验证通讯是否成功
//参数说明：cnt[IN]:读寄存器次数
//返    回：成功返回MI_OK，失败返回MI_ERR
/////////////////////////////////////////////////////////////////////
uint8_t Test_Communication(uint8_t cnt)
{
	static uint8_t i,j;
  SET_NFC_RST;
  Delay1us(200);
	for(i=0;( i<cnt && (j == 0x12 || j == 0x15) ) ;i++)
	{	
	Delay1us(100);	
	j=ReadRawRC(0X37); 
	}
	if( j == 0x12 || j == 0x15 )
		return MI_OK;
	else
		return MI_ERR;
}

void Ws1850s_Init(void)
{
  PcdReset();
  PcdAntennaOff();  
  PcdAntennaOn();
  M500PcdConfigISOTypeA();
}



/////////////////////////////////////////////////////////////////////
//功    能：复位芯片
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
void PcdReset(void)
{

  Delay1us(40);
#if 1
  SET_NFC_RST;
  Delay1us(2);
  CLR_NFC_RST;
  Delay1us(2);
  SET_NFC_RST;
  Delay1us(2);
#endif 
  Delay1us(500); //delay 500us 请客户确认delayus函数延时准确。
  
  WriteRawRC(CommandReg,PCD_RESETPHASE);//软复位
  Delay1us(2);

  WriteRawRC(ModeReg,0x3D); //和Mifare卡通讯，CRC初始值0x6363

  WriteRawRC(TReloadRegL,30);  //重装定时器值高位        
  WriteRawRC(TReloadRegH,0);//重装定时器值低位
  WriteRawRC(TModeReg,0x8D);//跟随协议启动和停止
  WriteRawRC(TPrescalerReg,0x3E);//6.78/3390,15ms产生中断

  WriteRawRC(TxASKReg,0x40);//必须要
 // ClearBitMask(TestPinEnReg,0x80);//off MX and DTRQ out
}

/////////////////////////////////////////////////////////////////////
//开启天线  
//每次启动或关闭天险发射之间应至少有1ms的间隔
/////////////////////////////////////////////////////////////////////
void PcdAntennaOn(void)
{
	WriteRawRC(TxControlReg, ReadRawRC(TxControlReg) | 0x03); //Tx1RFEn=1 Tx2RFEn=1
}

/////////////////////////////////////////////////////////////////////
//关闭天线
/////////////////////////////////////////////////////////////////////
void PcdAntennaOff(void)
{
  WriteRawRC(TxControlReg, ReadRawRC(TxControlReg) & (~0x03));
}


/**
 ****************************************************************
 * @brief pcd_config() 
 *
 * 配置芯片的A/B模式
 *
 * @param: uint8_t type   
 * @return: 
 * @retval: 
 ****************************************************************
 */

void PcdConfig(uint8_t type)
{
	  CLR_NFC_RST;
    Delay10us(1);
	  SET_NFC_RST;
    Delay10us(50);

		if ('A' == type)
		{
				ClearBitMask(Status2Reg, BIT3);
				ClearBitMask(ComIEnReg, BIT7); // 高电平
				WriteRawRC(ModeReg,0x3D);	// 11 // CRC seed:6363
				WriteRawRC(RxSelReg, 0x86);//RxWait
				WriteRawRC(RFCfgReg, 0x58); // 
				WriteRawRC(TxASKReg, 0x40);//15  //typeA
				WriteRawRC(TxModeReg, 0x00);//12 //Tx Framing A
				WriteRawRC(RxModeReg, 0x00);//13 //Rx framing A
				WriteRawRC(0x0C, 0x10);	//^_^
	
			//兼容配置
			{
					uint8_t backup;
					backup = ReadRawRC(0x37);
					WriteRawRC(0x37, 0x00);	
				{
					// 以下寄存器必须按顺序配置
					WriteRawRC(0x37, 0x5E);
					WriteRawRC(0x26, 0x48);
					WriteRawRC(0x17, 0x88);
					//WriteRawRC(0x29, 0x12);//0x0F); //调制指数	
					WriteRawRC(0x35, 0xED);
					WriteRawRC(0x3b, 0xA5);
					WriteRawRC(0x37, 0xAE);
					WriteRawRC(0x3b, 0x72);	
				}
				WriteRawRC(0x27, 0xf0);//RxWait
				WriteRawRC(0x28, 0x3f); // 
				WriteRawRC(0x37, backup);
			}		
		}
		else if ('B' == type)
		{
				WriteRawRC(Status2Reg, 0x00);	//清MFCrypto1On
				ClearBitMask(ComIEnReg, BIT7);// 高电平触发中断
				WriteRawRC(ModeReg, 0x3F);	// CRC seed:FFFF
				WriteRawRC(RxSelReg, 0x85);	//RxWait
				WriteRawRC(RFCfgReg, 0x58);	//	
				//Tx
				WriteRawRC(GsNReg, 0xF8);	//调制系数
				WriteRawRC(CWGsPReg, 0x3F);	// 
				WriteRawRC(ModGsPReg, 0x20);	//调制指数
				WriteRawRC(AutoTestReg, 0x00);
				WriteRawRC(TxASKReg, 0x00);	// typeB
				WriteRawRC(TypeBReg, 0x13);
				WriteRawRC(TxModeReg, 0x83);	//Tx Framing B
				WriteRawRC(RxModeReg, 0x83);	//Rx framing B
				WriteRawRC(BitFramingReg, 0x00);	//TxLastBits=0
	
			//兼容配置
			{
					uint8_t backup;
					backup = ReadRawRC(0x37);
					WriteRawRC(0x37, 0x00);
				{	
						WriteRawRC(0x37, 0x5E);
						WriteRawRC(0x26, 0x48);
						WriteRawRC(0x17, 0x88);
						WriteRawRC(0x29, 0x20);
						WriteRawRC(0x35, 0xED);
						WriteRawRC(0x3b, 0xA5);
						WriteRawRC(0x37, 0xAE);
						WriteRawRC(0x3b, 0x72);
				}
				WriteRawRC(0x37, backup);
			}
		}
		else
		{
//			return USER_ERROR;
		}
		
		PcdAntennaOn();
		Delay1us(1);
		
}	


/////////////////////////////////////////////////////////////////////
//设置PCD定时器
//input:fwi=0~15
/////////////////////////////////////////////////////////////////////
void PcdSetTmo(uint8_t fwi)

{
	WriteRawRC(TPrescalerReg, (TP_FWT_302us) & 0xFF);
	WriteRawRC(TModeReg, BIT7 | (((TP_FWT_302us)>>8) & 0xFF));

	WriteRawRC(TReloadRegL, (1 << fwi)  & 0xFF);
	WriteRawRC(TReloadRegH, ((1 << fwi)  & 0xFF00) >> 8);
}


////////////////////////////////////////////////////////////
void CheckIrq(unsigned int count)
{
#if 1
	static uint8_t waitFor;	
	static uint8_t n;
//	static uint16_t i;	
//count=25;
//	waitFor = 0x00;	
//	i = 500;
	do 
	{
		n = ReadRawRC(ComIrqReg);  //?????????
		count--;
	}
	while ((count!=0) && !(n&0x01) && !(n&waitFor));
#endif	
	
#if 0
//delay1ms(2);
#endif	
	
}	

///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
////功    能：和ISO14443卡通讯
////参数说明：Command[IN]:命令字
////          pInData[IN]:发送到卡片的数据
////          InLenByte[IN]:发送数据的字节长度
////          pOutData[OUT]:接收到的卡片返回数据
////          *pOutLenBit[OUT]:返回数据的位长度
///////////////////////////////////////////////////////////////////////
uint8_t PcdComMF522(uint8_t Command,
								uint8_t *pInData,
								uint8_t InLenByte,
								uint8_t *pOutData,
								uint16_t *pOutLenBit,
                unsigned int count)
{
	uint8_t recebyte=0;
	uint8_t val;
	uint8_t err=0;	
	uint8_t irq_inv;
	uint8_t len_rest=0;
	uint8_t len=0;
	uint8_t status;
	uint8_t irqEn   = 0x00;
	uint8_t waitFor = 0x00;
	uint8_t lastBits;
	uint16_t i;

	switch (Command)
	{
	  case PCD_IDLE:
	     irqEn   = 0x00;
	     waitFor = 0x00;
	     break;
	  case PCD_AUTHENT:    
		irqEn = IdleIEn | TimerIEn;
		waitFor = IdleIRq;
		break;
	  case PCD_RECEIVE:
	     irqEn   = RxIEn | IdleIEn;
	     waitFor = RxIRq;
	     recebyte=1;
	     break;
	  case PCD_TRANSMIT:
	     irqEn   = TxIEn | IdleIEn;
	     waitFor = TxIRq;
	     break;
	  case PCD_TRANSCEIVE:   
		 irqEn = RxIEn | IdleIEn | TimerIEn | TxIEn;
	     waitFor = RxIRq;
	     recebyte=1;
	     break;
	  default:
	     Command = MI_UNKNOWN_COMMAND;
	     break;
	}
   
	if (Command != MI_UNKNOWN_COMMAND
		&& (((Command == PCD_TRANSCEIVE || Command == PCD_TRANSMIT) && InLenByte > 0)
		|| (Command != PCD_TRANSCEIVE && Command != PCD_TRANSMIT))
		)
	{		
		WriteRawRC(CommandReg, PCD_IDLE);
		
		irq_inv = ReadRawRC(ComIEnReg) & BIT7;
		WriteRawRC(ComIEnReg, irq_inv |irqEn | BIT0);//使能Timer 定时器中断
		WriteRawRC(ComIrqReg, 0x7F); //Clear INT
		WriteRawRC(DivIrqReg, 0x7F); //Clear INT
		//Flush Fifo
		SetBitMask(FIFOLevelReg, BIT7);
		if (Command == PCD_TRANSCEIVE || Command == PCD_TRANSMIT || Command == PCD_AUTHENT)
		{
			len_rest = InLenByte;
			if (len_rest >= FIFO_SIZE)
			{
				len = FIFO_SIZE;
			}else
			{
				len = len_rest;
			}
			
			for (i = 0; i < len; i++)
			{
				WriteRawRC(FIFODataReg, pInData[i]);
			}
			len_rest -= len;//Rest bytes
			if (len_rest != 0)
			{
				WriteRawRC(ComIrqReg, BIT2); // clear LoAlertIRq
				SetBitMask(ComIEnReg, BIT2);// enable LoAlertIRq
			}

			WriteRawRC(CommandReg, Command);
			if (Command == PCD_TRANSCEIVE)
		    {    
				SetBitMask(BitFramingReg,0x80);  
			}
		
			while (len_rest != 0)
			{
			  	CheckIrq(count);		
				if (len_rest > (FIFO_SIZE - WATER_LEVEL))
				{
					len = FIFO_SIZE - WATER_LEVEL;
				}
				else
				{
					len = len_rest;
				}
				for (i = 0; i < len; i++)
				{
					WriteRawRC(FIFODataReg, pInData[InLenByte - len_rest + i]);
				}

				WriteRawRC(ComIrqReg, BIT2);//在write fifo之后，再清除中断标记才可以
				len_rest -= len;//Rest bytes
				if (len_rest == 0)
				{
					ClearBitMask(ComIEnReg, BIT2);// disable LoAlertIRq
				}	
			}
			//Wait TxIRq
			CheckIrq(count);
                        //while (INT_PIN == 0);
			val = ReadRawRC(ComIrqReg);
			if (val & TxIRq)
			{
				WriteRawRC(ComIrqReg, TxIRq);
			}
		}
		if (PCD_RECEIVE == Command)
		{	
			SetBitMask(ControlReg, BIT6);// TStartNow
		}
	
		len_rest = 0; // bytes received
		WriteRawRC(ComIrqReg, BIT3); // clear HoAlertIRq
		SetBitMask(ComIEnReg, BIT3); // enable HoAlertIRq
	
		//CheckIrq();
	
		while(1)
		{
      CheckIrq(count);
			val = ReadRawRC(ComIrqReg);
			if ((val & BIT3) && !(val & BIT5))
			{
				if (len_rest + FIFO_SIZE - WATER_LEVEL > 255)
				{
					break;
				}
		    for (i = 0; i <FIFO_SIZE - WATER_LEVEL; i++)
		    {
					pOutData[len_rest + i] = ReadRawRC(FIFODataReg);
		    }
				WriteRawRC(ComIrqReg, BIT3);//在read fifo之后，再清除中断标记才可以
				len_rest += FIFO_SIZE - WATER_LEVEL; 
			}
			else
			{
				ClearBitMask(ComIEnReg, BIT3);//disable HoAlertIRq
				break;
			}			
		}

		val = ReadRawRC(ComIrqReg);

		WriteRawRC(ComIrqReg, val);// 清中断
		
		if (val & BIT0)
		{//发生超时
			status = MI_NOTAGERR;
		}
		else
		{
			err = ReadRawRC(ErrorReg);
			
			status = MI_COM_ERR;
			if ((val & waitFor) && (val & irqEn))
			{
				if (!(val & ErrIRq))
				 {//指令执行正确
				    status = MI_OK;

				    if (recebyte)
				    {
						val = 0x7F & ReadRawRC(FIFOLevelReg);
				      	lastBits = ReadRawRC(ControlReg) & 0x07;
						if (len_rest + val > MAX_TRX_BUF_SIZE)
						{//长度过长超出缓存
							status = MI_COM_ERR;
						}
						else
						{	
							if (lastBits && val) //防止spi读错后 val-1成为负值
							{
								*pOutLenBit = (val-1)*8 + lastBits;
							}
							else
							{
								*pOutLenBit = val*8;
							}
							*pOutLenBit += len_rest*8;


							if (val == 0)
							{
								val = 1;
							}
							for (i = 0; i < val; i++)
							{
								pOutData[len_rest + i] = ReadRawRC(FIFODataReg);
							}					
						}
				   }
				 }					
				 else if ((err & CollErr) && (!(ReadRawRC(CollReg) & BIT5)))
				 {//a bit-collision is detected				 	
				    status = MI_COLLERR;
				    if (recebyte)
				    {
								val = 0x7F & ReadRawRC(FIFOLevelReg);
				      	lastBits = ReadRawRC(ControlReg) & 0x07;
						if (len_rest + val > MAX_TRX_BUF_SIZE)
						{//长度过长超出缓存
							;
						}
						else
						{
					     if (lastBits && val) //防止spi读错后 val-1成为负值
					     {
					        *pOutLenBit = (val-1)*8 + lastBits;
					     }
					     else
					     {
					        *pOutLenBit = val*8;
					     }		
							*pOutLenBit += len_rest*8;
					     if (val == 0)
					     {
					        val = 1;
					     }
							for (i = 0; i < val; i++)
					    {
								pOutData[len_rest + i +1] = ReadRawRC(FIFODataReg);				
					    }				
						}
				    }
					pOutData[0] = (ReadRawRC(CollReg) & CollPos);
					if (pOutData[0] == 0)
					{
						pOutData[0] = 32;
					}
				
					pOutData[0]--;// 与之前版本有点映射区别，为了不改变上层代码，这里直接减一；

				}
				else if ((err & CollErr) && (ReadRawRC(CollReg) & BIT5))
				{
					;		
				}
				//else if (err & (CrcErr | ParityErr | ProtocolErr))
				else if (err & (ProtocolErr))
				{
					status = MI_FRAMINGERR;				
				}
				else if ((err & (CrcErr | ParityErr)) && !(err &ProtocolErr))
				{
					//EMV  parity err EMV 307.2.3.4		
					val = 0x7F & ReadRawRC(FIFOLevelReg);
			      	lastBits = ReadRawRC(ControlReg) & 0x07;
					if (len_rest + val > MAX_TRX_BUF_SIZE)
					{//长度过长超出缓存
						status = MI_COM_ERR;
					}
					else
					{
				        if (lastBits && val)
				        {
				           *pOutLenBit = (val-1)*8 + lastBits;
				        }
				        else
				        {
				           *pOutLenBit = val*8;
				        }
						*pOutLenBit += len_rest*8;
					}
					status = MI_INTEGRITY_ERR;
				}				
				else
				{
					status = MI_INTEGRITY_ERR;
				}
			}
			else
			{   
				status = MI_COM_ERR;
			}
		}	
 		SetBitMask(ControlReg, BIT7);// TStopNow =1,必要的；
		WriteRawRC(ComIrqReg, 0x7F);// 清中断0
		WriteRawRC(DivIrqReg, 0x7F);// 清中断1
		ClearBitMask(ComIEnReg, 0x7F);//清中断使能,最高位是控制位
		ClearBitMask(DivIEnReg, 0x7F);//清中断使能,最高位是控制位
		WriteRawRC(CommandReg, PCD_IDLE);
	}
	else
	{
		status = USER_ERROR;
	}
	return status;
}
                      
///////////////////////////////////////////////////////////////////////
////功    能：读A卡，并返回卡号
//  参    数：uid  寻感应区内所有符合14443A标准的卡(0x52)/
//								  寻未进入休眠状态的卡(0x26)
///////////////////////////////////////////////////////////////////////
uint8_t ComReqA_UID(uint8_t *uid)
{

		uint8_t i;
		PcdConfig('A');
		Delay1ms(1);
	if(PcdRequest(PICC_REQALL,CT)!=MI_OK)
			return MI_ERR;
	if(PcdAnticoll(IDA)!=MI_OK)       //一次防冲突及选卡
			return MI_ERR;
    	 for(i=0;i<4;i++)
	    {uid[i]=IDA[i];}
	if(PcdSelect(IDA)!=MI_OK)
			return MI_ERR;
 return MI_OK;
}

uint8_t ComReqA_Block(uint8_t rw,uint8_t Block)
{
	uint8_t status;
		if(PcdAuthState(PICC_AUTHENT1A,Block,PassWd,IDA)!=MI_OK)
			return MI_ERR;
	switch(rw)
	{
		case 0: status = PcdRead(Block,RWDATA);break;
		case 1: status = PcdWrite(Block,RWDATA);break;	
		default: break;
	}

	return status;
}

uint8_t ComReqA(uint8_t rw,uint8_t Block)
{
		uint8_t status = MI_OK;
		PcdConfig('A');
	
	if(PcdRequest(PICC_REQALL,CT)!=MI_OK)///获得卡片类型
//	if(PcdFastSearchCard()!=MI_OK)
			return MI_ERR;
	if(PcdAnticoll(IDA)!=MI_OK)       //一次防冲突及选卡，获得卡片序列号
			return MI_ERR;
	
	
#if 1  //操作扇区
	if(PcdSelect(IDA)!=MI_OK)///选定获得的卡片
			return MI_ERR;
	if(PcdAuthState(PICC_AUTHENT1A,Block,PassWd,IDA)!=MI_OK)///验证密钥，密码，
			return MI_AUTHERR;
	switch(rw)
	{
		case 0: status = PcdRead(Block,RWDATA);break;
		case 1: status = PcdWrite(Block,RWDATA);break;	
		default: break;
	}
#endif
	return status;
}
///////////////////////////////////////////////////////////////////////
////功    能：读B卡，并返回卡号
//  参    数：pcmd  寻感应区内所有符合14443b标准的卡(0x08)/
///////////////////////////////////////////////////////////////////////
uint8_t ComReqB(void)
{
	
	uint8_t  status = MI_ERR;
	uint8_t  i;
	uint8_t  cnt;
	uint8_t  ATQB[16];
	 
	PcdConfig('B');

	cnt = 3;//应用中 可以使用轮询N次	
	while(cnt--)
	{
		status = PcdRequestB(0x08, 0, 0, ATQB);
		
		if(status == MI_COLLERR)	// 有冲突，超过一张卡
		{
			if((status = PcdRequestB(0x08, 0, 2, ATQB)) != MI_OK)
			{
			   	for (i = 1; i < 4; i++)
			   	{
			    	if((status = PcdSlotMarker(i, ATQB)) == MI_OK) 
						{
								break;
			    	}
			  	}
					if (status == MI_OK)
					{
							break;
					}
			}
			else
			{
					break;
			}
		}
	}

	if (status == MI_OK)
	{
		//typeB 106默认速率
		status = PcdAttriB(&ATQB[1], 0, ATQB[10]&0x0f, PICC_CID, ATQB);

		if (status == MI_OK)
		{						
			ATQB[0] = 0x50;//恢复默认值
			status =	GetIdcardNum(IDB);//获取卡号
		}
	}
	
	return status;
}


/////////////////////////////////////////////////////////////////////
//功    能：寻卡
//参数说明: req_code[IN]:寻卡方式
//                0x52 = 寻感应区内所有符合14443A标准的卡
//                0x26 = 寻未进入休眠状态的卡
//          pTagType[OUT]：卡片类型代码
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
uint8_t PcdRequest(uint8_t req_code,uint8_t *pTagType)
{	
   uint8_t status; 
   uint8_t	i;
   uint16_t  unLen;
   uint8_t ucComMF522Buf[MAXRLEN]; 

   ClearBitMask(Status2Reg,0x08);
   WriteRawRC(BitFramingReg,0x07);
   SetBitMask(TxControlReg,0x03);   
	 i=ReadRawRC(0X37);
	if(i == 0x12 || i == 0x15)
	 {
			Delay1us(1);
	 }
	
   ucComMF522Buf[0] = req_code;

   status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen,5);
   if ((status == MI_OK) && (unLen == 0x10))
   {    
       *pTagType     = ucComMF522Buf[0];
       *(pTagType+1) = ucComMF522Buf[1];
   }
   else
   {   status = MI_ERR;   }
   
   return status;
}

uint8_t PcdFastSearchCard(void)
{
  uint8_t irqEn   = 0x77;
  uint8_t errmask = 0x30;
  uint8_t errflag;
  uint16_t times;
#if 1
  SET_NFC_RST;
  Delay1us(5);
  CLR_NFC_RST;//
  Delay1us(2);
  SET_NFC_RST;
  
  Delay1us(2);
 // GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);
  //while(ReadRawRC(0x27) != 0x88);
 #endif  
  Delay1us(500); //delay 500us 
  ClearBitMask (TestPinEnReg,0x9E);
  WriteRawRC(ModeReg,0x3D); //和Mifare卡通讯，CRC初始值0x6363	
  WriteRawRC(TxASKReg,0x40);//100%ASK调制
  WriteRawRC(BitFramingReg,0x07);//
  WriteRawRC(TxControlReg,0x83); //T1，T2发送经过连续调制的载波。出现载波0X83
	//Delay1us(500);
  WriteRawRC(ComIEnReg,irqEn|0x80);//
  WriteRawRC(ComIrqReg,0x14);     //
  WriteRawRC(CommandReg,PCD_IDLE);//
  WriteRawRC(FIFOLevelReg,0x80);  //
  WriteRawRC(FIFODataReg,PICC_REQIDL); //先将数据写入fifo  0X26
  WriteRawRC(CommandReg, PCD_TRANSCEIVE);//执行操作命令写入卡中
  WriteRawRC(BitFramingReg,0x87);//启动数据发送
  times = 25;
  do 
  {
    errflag = ReadRawRC(ComIrqReg);  //判断是否有中断发生
    times--;
  }
  while ((times!=0)&& !(errflag&errmask));  
  if (times==0) 
  {
#if 1   
    CLR_NFC_RST;//硬复位
#else
  //  WriteRawRC(TxControlReg,0x80); //软复位
#endif   
    return MI_ERR;
  }
  
  return MI_OK;
  
}


/////////////////////////////////////////////////////////////////////
//功    能：防冲撞
//参数说明: pSnr[OUT]:卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////  
uint8_t PcdAnticoll(uint8_t *pSnr)
{
    uint8_t status;
    uint8_t i,snr_check=0;
    uint16_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen,7);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：选定卡片
//参数说明: pSnr[IN]:卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
uint8_t PcdSelect(uint8_t *pSnr)
{
    uint8_t status;
    uint8_t i;
    uint16_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen,15);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：验证卡片密码
//参数说明: auth_mode[IN]: 密码验证模式
//                 0x60 = 验证A密钥
//                 0x61 = 验证B密钥 
//          addr[IN]：块地址
//          pKey[IN]：密码
//          pSnr[IN]：卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////                   
uint8_t PcdAuthState(uint8_t auth_mode,uint8_t addr,uint8_t *pKey,uint8_t *pSnr)
{
    uint8_t status;
    uint16_t  unLen;
    uint8_t i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen,30);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   //status = MI_ERR;   
		    status = MI_AUTHERR;
		}
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：读取M1卡一块数据
//参数说明: addr[IN]：块地址
//          p [OUT]：读出的数据，16字节
//返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////// 
uint8_t PcdRead(uint8_t addr,uint8_t *pData)
{
    uint8_t status;
    uint16_t  unLen;
    uint8_t i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen,20);
    if ((status == MI_OK) && (unLen == 0x90))
    {
        for (i=0; i<16; i++)
        {    *(pData+i) = ucComMF522Buf[i];   }
    }
    else
    {  // status = MI_ERR; 
        status =  MI_READERR;
		}
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：写数据到M1卡一块
//参数说明: addr[IN]：块地址
//          p [IN]：写入的数据，16字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////                 
uint8_t PcdWrite(uint8_t addr,uint8_t *pData)
{
    uint8_t status;
    uint16_t  unLen;
    uint8_t i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
	  ClearBitMask(TxModeReg, BIT7); //????crc
    ClearBitMask(RxModeReg, BIT7); //?????crc
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen,20);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pData+i);   }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen,20);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   //status = MI_ERR; 
            status =MI_WRITEERR;
				}
    }
    
    return status;
}



/////////////////////////////////////////////////////////////////////
//用MF522计算CRC16函数
/////////////////////////////////////////////////////////////////////
void CalulateCRC(uint8_t *pIndata,uint8_t len,uint8_t *pOutData)
{
    uint8_t i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   WriteRawRC(FIFODataReg, *(pIndata+i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
}

void M500PcdConfigISOTypeA(void)//ISO14443_A//A:NXP,B:MOTO
{
  ClearBitMask(Status2Reg,0x08);//清除寄存器
  WriteRawRC(ModeReg,0x3D);//3F 选择模式
  WriteRawRC(RxSelReg,0x86);//84  内部接收设置
  WriteRawRC(RFCfgReg,0x7F);   //4F  接收增益
  WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
  WriteRawRC(TReloadRegH,0);
  WriteRawRC(TModeReg,0x8D);
  WriteRawRC(TPrescalerReg,0x3E);

  Delay1us(400);
  PcdAntennaOn();
}

void pcd_lpcd_start(uint8_t delta,uint8_t swingscnt)
{
	uint16_t sw;
	sw = swingscnt;
	delta=0X30 + delta;
	
	CLR_NFC_RST;
	Delay1ms(5);
	SET_NFC_RST;
	Delay1ms(5);

	WriteRawRC(0x01,0x0F); 	///WS1850启动唤醒过程
	WriteRawRC(0x14, 0x23);	/// 发送13.56MHz的能量载波信号
	WriteRawRC(0x37, 0x5e);	/// 指示当前芯片版本 
	WriteRawRC(0x3c,delta);//Delta[3:0]///灵敏度？
	//WriteRawRC(0x3d, 0x18);		//休眠时间	200MS
	WriteRawRC(0x3d, 0x0d);	//休眠时间	100MS
	WriteRawRC(0x3e, 0x90|sw);//探测时间
	WriteRawRC(0x37, 0x00);	// 关闭私有寄存器保护开关
	#if 1
	WriteRawRC(0x37, 0x5a);//打开私有寄存器保护开关
	WriteRawRC(0x38, 0x70);//设置LPCD 发射功率
	WriteRawRC(0x39, 0x10);//设置LPCD 发射功率
	WriteRawRC(0x33, 0x20);//调整步长,20,60,A0,E0
	WriteRawRC(0x36, 0x80);
	WriteRawRC(0x37, 0x00);//关闭私有寄存器保护开关
	//WriteRawRC(0x37, 0x5e);
	//WriteRawRC(0x32, 0x80);
	WriteRawRC(0x37, 0x00);//关闭私有寄存器保护开关
#endif	
//	ClearBitMask(0x02, 0x80); //配置IRQ为高电平中断
  SetBitMask(0x02, 0x80);//配置为低电平中断
	WriteRawRC(0x03, 0xa0);	//打开卡探测中断,IRQ 为CMOS 输出	
	WriteRawRC(0x01, 0x10);	//PCD soft powerdown						
}

//******WS1850结束LPCD模式*******************
void pcd_lpcd_end(void)
{
	WriteRawRC(0x01,0x0F); //?????????lpcd
}


//****检测卡片类型以及卡片的信息******************************
//void Card_Check(void)
//{
//		uint8_t i;
//		uint8_t statusA,statusB;
//		uint8_t Block=0x05;
//		statusA = ComReqA(READ,Block);// WRITE READ
//		
////		#if 1 
//  UART0_INIT(9600);	
//	if(statusA != MI_ERR)
//	{
//	           
//							if( statusA ==MI_OK || statusA ==MI_AUTHERR || statusA == MI_READERR)
//							{
//								{	
//									UART0_INIT(9600);	
//								  LED1_ON;
//									BEEP_ON;
//									for(i=0;i<4;i++)
//									{                         
//									Send_Data_To_UART0(IDA[i]);   
//									}	
//									UART0_INIT(9600);
//									for(i=0;i<2;i++)
//									{                                          
//									Send_Data_To_UART0(CT[i]);   
//									}
//									
//									delay1ms(10);
//								}	
//                 								
//		#if 1
//						if( statusA == MI_OK)	
//						{
//							delay1ms(30);	
//							BEEP_ON;
//							LED2_ON;
//							UART0_INIT(9600);
//							Send_Data_To_UART0(Block);
//							for(i=0;i<16;i++)
//							{ 
//									Send_Data_To_UART0(RWDATA[i]);   
//							}	
//							
//							//UART0_INIT(9600);
//						}
//						else
//						{
//							    BEEP_OFF;
//                  LED1_OFF;
//						      LED2_OFF;
//						
//							//Send_Data_To_UART0(Block);
//							
//							
//						}
//		#endif 
//						      UART0_INIT(9600);
//									delay1ms(5);
//									LED1_OFF;
//						      LED2_OFF;
//									BEEP_OFF;
//							}
//				}
//							else 
//							{
//									statusB = ComReqB();
//									if(statusB==MI_OK)
//									{
//											LED2_ON;
//											BEEP_ON;
//										 UART0_INIT(9600);
//											for(i=0;i<10;i++)
//											{ 
//											Send_Data_To_UART0(IDB[i]);   
//											}
//											UART0_INIT(9600);
//											delay1ms(10);
//											LED2_OFF;	
//											BEEP_OFF;
//									}
//							}
//}

//lpcd 应用函数
void pcd_lpcd_application(void)
{
	{     			
//			Card_Check();																		
			CLR_NFC_RST;
			Delay1ms(200);//模拟读卡片后拿开过程				
	}
	CLR_NFC_RST;
	Delay1us(500);
	SET_NFC_RST;
	Delay1us(500);	
}
//****检测芯片版本信息******************************
uint8_t IC_ver(void)
{
	uint8_t status;
	WriteRawRC(0x37,0x55);
	status=ReadRawRC(0x37);
//	  UART0_INIT(9600);	
//    Send_Data_To_UART0(status);
	return status;
}
