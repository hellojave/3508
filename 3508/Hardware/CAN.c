#include "stm32f10x.h"
#include "CAN.h"
static void CAN_GPIO_Config(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;   	

  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(CAN_GPIO_CLK|RCC_APB2Periph_AFIO,ENABLE);

	 /* Configure CAN TX pins */
  GPIO_InitStructure.GPIO_Pin = CAN_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		         // 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(CAN_TX_GPIO_PORT, &GPIO_InitStructure);
	
	/* Configure CAN RX  pins */
  GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	             // 上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(CAN_RX_GPIO_PORT, &GPIO_InitStructure);
}

/*
 * 函数名：CAN_NVIC_Config
 * 描述  ：CAN的NVIC 配置,第1优先级组，0，0优先级
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */



static void CAN_Mode_Config(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	/************************CAN通信参数设置**********************************/
	/* Enable CAN clock */
  RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);

	/*CAN寄存器初始化*/
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);

	/*CAN单元初始化*/
	CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  ENABLE时间触发通信模式使能
	CAN_InitStructure.CAN_ABOM=DISABLE;			   //MCR-ABOM  自动离线管理 
	CAN_InitStructure.CAN_AWUM=DISABLE;			   //MCR-AWUM  使用自动唤醒模式
	CAN_InitStructure.CAN_NART=ENABLE;			   //MCR-NART  禁止报文自动重传	  DISABLE-自动重传
	CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  接收FIFO 锁定模式  DISABLE-溢出时新报文会覆盖原有报文  
	CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  发送FIFO优先级 DISABLE-优先级取决于报文标示符 
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  
//	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;		   
//	 
//	
//	CAN_InitStructure.CAN_BS1=CAN_BS1_10tq;		   
//	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;		   	
//	
//	
//	CAN_InitStructure.CAN_Prescaler =3;		 
	CAN_InitStructure.CAN_SJW=CAN_SJW_2tq;
	 
	
	CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;
	
	
	CAN_InitStructure.CAN_Prescaler =4;	
	CAN_Init(CAN1, &CAN_InitStructure);
}

/*
 * 函数名：CAN_Filter_Config
 * 描述  ：CAN的过滤器 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_Filter_Config(void)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/*CAN筛选器初始化*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;						//筛选器组0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//工作在掩码模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//筛选器位宽为单个32位。
	/* 使能筛选器，按照标志的内容进行比对筛选，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 */

	CAN_FilterInitStructure.CAN_FilterIdHigh= 0x0000;		//要筛选的ID高位 
	CAN_FilterInitStructure.CAN_FilterIdLow= 0x0000; //要筛选的ID低位 
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;			//筛选器高16位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0x0000;			//筛选器低16位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//筛选器被关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//使能筛选器
	CAN_FilterInit(&CAN_FilterInitStructure);
	/*CAN通信中断使能*/
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

static void CAN_NVIC_Config(void)
{
   	NVIC_InitTypeDef NVIC_InitStructure;
		/* Configure one bit for preemption priority */
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	 	/*中断设置*/
	  NVIC_InitStructure.NVIC_IRQChannel = CAN_RX_IRQ;	   //CAN1 RX0中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		   //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			   //子优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*
 * 函数名：CAN_Config
 * 描述  ：完整配置CAN的功能
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void CAN_Config(void)
{
  CAN_GPIO_Config();
  CAN_Mode_Config();
  CAN_Filter_Config(); 
  CAN_NVIC_Config();
  
}


/**
  * @brief  初始化 Rx Message数据结构体
  * @param  RxMessage: 指向要初始化的数据结构体
  * @retval None
  */

void RECIVE_CAN(CanRxMsg *RxMessage)
{
	/*读取报文*/

	
  uint8_t ubCounter = 0;


	/*把接收结构体清零*/
  RxMessage->StdId = 0x203;
  //RxMessage->ExtId = 0x203;
  RxMessage->IDE = CAN_ID_STD;
  RxMessage->DLC = 0x08;
  RxMessage->FMI = 0;
  for (ubCounter = 0; ubCounter < 8; ubCounter++)
  {
    RxMessage->Data[ubCounter] = 0;
  }
	
}




/*
 * 函数名：CAN_SetMsg
 * 描述  ：CAN通信报文内容设置,设置一个数据内容为0-7的数据包
 * 输入  ：发送报文结构体
 * 输出  : 无
 * 调用  ：外部调用CanTxMsg TxMessage;
 */	 
 CanTxMsg TxMessage;

void SEND_CAN(int16_t iql)//CAN发送函数
{
	uint16_t box;
	TxMessage.StdId = 0x200;
//	TxMessage->ExtId = 0x200;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.DLC = 0x08;	
	
//	TxMessage.Data[0] = iql >> 8;
//	TxMessage.Data[1] = iql;
//	TxMessage.Data[2] = iql >> 8;
//	TxMessage.Data[3] = iql;
	TxMessage.Data[4] = (int8_t)(iql >> 8);
	TxMessage.Data[5] = (int8_t)(iql&0x00ff);
//	TxMessage.Data[6] = iql >> 8;
//	TxMessage.Data[7] = iql;
	
	CAN_Transmit(CAN1,&TxMessage);
	box = CAN_Transmit(CAN1,&TxMessage);

	while(CAN_TransmitStatus(CAN1,box) == CAN_TxStatus_Failed);
		
}

/**************************END OF FILE************************************/
