#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "CAN.h"
#include "pid.h"
__IO uint32_t flag = 0;		 //���ڱ�־�Ƿ���յ����ݣ����жϺ����и�ֵ

CanRxMsg RxMessage;				 //���ջ�����
pid_t pid;
INIT_STATUS init_status=DONE;
uint16_t speed,eletric,c,i=0;

/// ����ȷ����ʱ
static void can_delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
} 

int main(void)
{		

  /*��ʼ��USART1*/
	Serial_Init();
	
	/*��ʼ��can,���жϽ���CAN���ݰ�*/
	CAN_Config();
	RECIVE_CAN(&RxMessage);
	
	/* ��ʼ��pid */
	PID_Struct_Init(&pid,1.5f, 0.1f, 0.0f,2000,2000,init_status);//(,p,i,d,int32_t max_out,int32_t integral_limit,INIT_STATUS init_status)
  printf("ready");

  while(1)
	{
		c++;
    
		if(flag==1)
	{	
			SEND_CAN((int16_t)pid_calc(&pid,speed,1000));
			if(c%10==0)
		{
	   	printf("\r\n���յ�ת���ٶ����ݣ�%d \r\n",speed);
			speed=(RxMessage.Data[2]<<8)|RxMessage.Data[3];
			printf("\r\n���յ�ת�ص������ݣ�%d \r\n",eletric);
			eletric=(RxMessage.Data[4]<<8)+RxMessage.Data[5];
			c=0;
			printf("\r\n����¶�:%d��\r\n",RxMessage.Data[6]);
		}
			
			flag=0;
		}

		
		
	}
}
