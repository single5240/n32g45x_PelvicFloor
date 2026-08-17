#include "AppHandle.h"



extern uint8_t Formula;/////处方0，1,2
extern uint8_t Minute;
extern uint8_t Second;
extern uint8_t WorkType;////0:治疗模式，1：检测模式
extern uint8_t Pwr1;/////0-60档强度
extern uint8_t Pwr2;
extern uint8_t Time_SetFlag;

extern uint16_t MotoFirst_Flag;//首次充气标志位
extern uint16_t MotoRun_Cnt;//电机启动标志位
extern uint16_t MotoStop_Cnt;//电磁阀开启标志位
extern uint8_t  BLE_Pressure;


extern uint8_t		Key_SelectA;
extern uint8_t    Moto_StateFlag;
extern uint8_t    Bat_Value;
extern uint8_t  	BEL_SendData[10];
extern uint8_t    BEL_SendBeat[13];
extern uint8_t    BEL_ReadData[6];


/**  
 * @brief 数据帧打包函数
 * @param cmd 命令输入，data 数据输入
 * 
 */
void Bt_Send(uint8_t cmd, uint8_t data)
{
	BEL_SendData[0] = 0x5a;/*帧头0x5a 0xa5*/
	BEL_SendData[1] = 0xa5;
	BEL_SendData[2] = 0x01;/*数据长度*/
	BEL_SendData[3] = cmd; /*返回命令*/
  BEL_SendData[4] = data;/*返回数据*/  
  BEL_SendData[5] = BLE_Checksum(BEL_SendData,6);/*数据校验*/
}


/**  
 * @brief 数据包校验位计算函数
 * @param data 数组地址
 * 
 */
uint8_t BLE_Checksum(uint8_t *data,uint8_t size) 
{
    uint16_t sum = 0;
    uint8_t *end = data + size-1; 
    
    for (uint8_t *ptr = data; ptr < end; ptr++) {
        sum += *ptr;  /*累加计算*/
    }
    return (uint8_t)(sum & 0xFF);  /*保留后八位*/
}


/**  
 * @brief 串口发送数据帧
 * @param USARTx串口选择，frame 数据包地址，size 数据包大小
 * 
 */
void Send_UART_Frame(USART_Module* USARTx, uint8_t *frame, uint16_t size) 
{
    for(uint16_t i = 0; i < size; i++) {

        USART_SendData(USARTx, frame[i]);
        /*等待发送完成*/ 
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TXDE) == RESET);
       
    }

}


/**  
 * @brief 数组清空函数
 * @param arr[]目标数组，size数据大小
 * 
 */
// 清空 uint8_t 数组的函数
void Clear_Array(uint8_t arr[], int size) 
{
    memset(arr, 0, size * sizeof(uint8_t));
}



/**  
 * @brief 接收数据处理函数
 * @param cmd 命令处理，ctr数据控制
 * 
 */
void App_Event(uint8_t cmd,uint8_t ctr)
{
		switch(cmd)
		{
			case 0x01:/***设置工作时间 */
				Time_SetFlag = 1;
				switch(ctr)
				{
					case 0x0a:
						Minute=10;
					break;
					case 0x14:
						Minute=20;
					break;
					case 0x1e:
						Minute=30;
					break;
					default:
					break ;
				}
				Bt_Send(0x01, Minute);
				Send_UART_Frame(USART2,BEL_SendData,6);
			break;
			case 0x02:/***设置工作模式 */ 
				Pwr1 = 0;
        Pwr2 = 0;       
				switch(ctr)
        {
          case 0x00:
            WorkType=0;
          break;
          case 0x01:
            WorkType=1;           
          break;
        }
				Bt_Send(0x02, WorkType);
				Send_UART_Frame(USART2,BEL_SendData,6);
			break;
			case 0x03:/***设置通道一强度 */
        Time_SetFlag = 0;
			  Pwr1=ctr;
				Bt_Send(0x03, Pwr1);
				Send_UART_Frame(USART2,BEL_SendData,6);
			break;
			case 0x04:/***设置通道二强度 */
        Time_SetFlag = 0;
			  Pwr2=ctr; 
				Bt_Send(0x04, Pwr2);
				Send_UART_Frame(USART2,BEL_SendData,6);       
			break;
			case 0x05:/***设置治疗模式 */
				Pwr1 = 0;
        Pwr2 = 0;
        switch(ctr)
        {
          case 0x00:
            Formula=0;
          break;
          case 0x01:
            Formula=1;
          break;
          case 0x02:
            Formula=2;
          break;
        }
				Bt_Send(0x05, Formula);
				Send_UART_Frame(USART2,BEL_SendData,6);
			break;
			case 0x06:/***启动充气 */
        Key_SelectA=1;
				SWEN_OFF;/////关闭电磁阀		
				MotoRun_Cnt=1;//启动马达  
				Bt_Send(0x06, Moto_StateFlag);
				Send_UART_Frame(USART2,BEL_SendData,6);
			break;
			case 0x07:/***开始放气 */
				SWEN_ON;///开启电磁阀
				MotoFirst_Flag=1;//重置初次充气标志位
				Key_SelectA=2;
				MotoStop_Cnt=2000;//电磁阀开启2S
				Bt_Send(0x07, Moto_StateFlag);
				Send_UART_Frame(USART2,BEL_SendData,6);
			break;
			case 0x81:/***查询工作剩余时间 */
        Bt_Send(0x81, Minute);
				Send_UART_Frame(USART2,BEL_SendData,10);
			break;
			case 0x82:/***查询工作模式 */
        Bt_Send(0x82, Formula);
				Send_UART_Frame(USART2,BEL_SendData,10);
			break;
			case 0x83:/***查询通道一强度 */
				Bt_Send(0x83, Pwr1);
				Send_UART_Frame(USART2,BEL_SendData,10);
			break;
			case 0x84:/***查询通道二强度 */
				Bt_Send(0x84, Pwr2);
				Send_UART_Frame(USART2,BEL_SendData,10);
			break;
			case 0x85:/***查询治疗模式 */
				Bt_Send(0x85, WorkType);
				Send_UART_Frame(USART2,BEL_SendData,10);
			break;
		//	case 0x91:
		//	break;
		//	case 0x90:
		//	break;
			default :
			break;
   }
   Clear_Array(BEL_SendData, 10);/*清空数组*/

}



/**  
 * @brief 心跳包发送函数
 * @param 
 * 
 */

void Send_BeatPack(void)
{
	  BEL_SendBeat[0]=0x5a;
	  BEL_SendBeat[1]=0xa5;
	  BEL_SendBeat[2]=0x08;
	  BEL_SendBeat[3]=0x92;
		if(Time_SetFlag)
		{
      Second = 0;
			BEL_SendBeat[4]=Minute;/*剩余时间*/
			BEL_SendBeat[5]=0;/*剩余时间*/
		}
		else
		{
			BEL_SendBeat[4]=Minute;/*剩余时间*/
			BEL_SendBeat[5]=Second;/*剩余时间*/
		}
	  BEL_SendBeat[6]=WorkType;/*工作模式*/
	  BEL_SendBeat[7]=Formula;/*治疗模式*/
	  BEL_SendBeat[8]=BLE_Pressure;/*压力值*/
		BEL_SendBeat[9]=Pwr1;
		BEL_SendBeat[10]=Pwr2;
	  BEL_SendBeat[11]=Bat_Value;/*电池电量*/
	  BEL_SendBeat[12]=BLE_Checksum(BEL_SendBeat,11);
   for(uint8_t i=0;i<13;i++)
	 {
		  USART_SendData(USART2, BEL_SendBeat[i]);//发送数据
		  while (USART_GetFlagStatus(USART2, USART_FLAG_TXDE) == RESET);
	 }

}

