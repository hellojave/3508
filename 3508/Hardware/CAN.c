#include "stm32f10x.h"
#include "CAN.h"
static void CAN_GPIO_Config(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;   	

  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(CAN_GPIO_CLK|RCC_APB2Periph_AFIO,ENABLE);

	 /* Configure CAN TX pins */
  GPIO_InitStructure.GPIO_Pin = CAN_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		         // �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(CAN_TX_GPIO_PORT, &GPIO_InitStructure);
	
	/* Configure CAN RX  pins */
  GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	             // ��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(CAN_RX_GPIO_PORT, &GPIO_InitStructure);
}

/*
 * ��������CAN_NVIC_Config
 * ����  ��CAN��NVIC ����,��1���ȼ��飬0��0���ȼ�
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */



static void CAN_Mode_Config(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	/************************CANͨ�Ų�������**********************************/
	/* Enable CAN clock */
  RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);

	/*CAN�Ĵ�����ʼ��*/
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);

	/*CAN��Ԫ��ʼ��*/
	CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  ENABLEʱ�䴥��ͨ��ģʽʹ��
	CAN_InitStructure.CAN_ABOM=DISABLE;			   //MCR-ABOM  �Զ����߹��� 
	CAN_InitStructure.CAN_AWUM=DISABLE;			   //MCR-AWUM  ʹ���Զ�����ģʽ
	CAN_InitStructure.CAN_NART=ENABLE;			   //MCR-NART  ��ֹ�����Զ��ش�	  DISABLE-�Զ��ش�
	CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  ����FIFO ����ģʽ  DISABLE-���ʱ�±��ĻḲ��ԭ�б���  
	CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  ����FIFO���ȼ� DISABLE-���ȼ�ȡ���ڱ��ı�ʾ�� 
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
 * ��������CAN_Filter_Config
 * ����  ��CAN�Ĺ����� ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_Filter_Config(void)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/*CANɸѡ����ʼ��*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;						//ɸѡ����0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//����������ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//ɸѡ��λ��Ϊ����32λ��
	/* ʹ��ɸѡ�������ձ�־�����ݽ��бȶ�ɸѡ����չID�������µľ����������ǵĻ��������FIFO0�� */

	CAN_FilterInitStructure.CAN_FilterIdHigh= 0x0000;		//Ҫɸѡ��ID��λ 
	CAN_FilterInitStructure.CAN_FilterIdLow= 0x0000; //Ҫɸѡ��ID��λ 
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;			//ɸѡ����16λÿλ����ƥ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0x0000;			//ɸѡ����16λÿλ����ƥ��
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//ɸѡ����������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//ʹ��ɸѡ��
	CAN_FilterInit(&CAN_FilterInitStructure);
	/*CANͨ���ж�ʹ��*/
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

static void CAN_NVIC_Config(void)
{
   	NVIC_InitTypeDef NVIC_InitStructure;
		/* Configure one bit for preemption priority */
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	 	/*�ж�����*/
	  NVIC_InitStructure.NVIC_IRQChannel = CAN_RX_IRQ;	   //CAN1 RX0�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		   //��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			   //�����ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*
 * ��������CAN_Config
 * ����  ����������CAN�Ĺ���
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void CAN_Config(void)
{
  CAN_GPIO_Config();
  CAN_Mode_Config();
  CAN_Filter_Config(); 
  CAN_NVIC_Config();
  
}


/**
  * @brief  ��ʼ�� Rx Message���ݽṹ��
  * @param  RxMessage: ָ��Ҫ��ʼ�������ݽṹ��
  * @retval None
  */

void RECIVE_CAN(CanRxMsg *RxMessage)
{
	/*��ȡ����*/

	
  uint8_t ubCounter = 0;


	/*�ѽ��սṹ������*/
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
 * ��������CAN_SetMsg
 * ����  ��CANͨ�ű�����������,����һ����������Ϊ0-7�����ݰ�
 * ����  �����ͱ��Ľṹ��
 * ���  : ��
 * ����  ���ⲿ����CanTxMsg TxMessage;
 */	 
 CanTxMsg TxMessage;

void SEND_CAN(int16_t iql)//CAN���ͺ���
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
