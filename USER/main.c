/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��main.c
 * ����    ��     
 * ʵ��ƽ̨��
 * ��汾  ��ST3.5.0
**********************************************************************************/
#include "stm32f10x.h"
#include "led.h"
#include "clock.h"
#include "print.h"
#include "usart.h"
#include "pwm_output.h"
#include "ds18b20.h"
#include <string.h>


void Delay(__IO u32 nCount);
extern uint32_t setTime[6];
uint32_t receive[20];
uint32_t setPwm = 0;
extern u16 CCR4_Val;
uint32_t vTemp=0; 
/***************************************************/
void IwdgConfig()
{
    RCC_LSICmd(ENABLE);                              //��LSI
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    IWDG_SetPrescaler(IWDG_Prescaler_32);

    IWDG_SetReload(4000);      //80ms ,max 0xFFF  0~4095 

    IWDG_ReloadCounter();

    IWDG_Enable();
}
void Init()
{
	SysTickConfig();			//ʱ������
	setTime[5]+=2;
	IwdgConfig();					//���Ź���ʼ��	
	USART2_Config();
	TIM3_PWM_Init();
	LED_GPIO_Config();		   /* LED �˿ڳ�ʼ�� */
	HOT_config();
	DS18_config();				//�¶ȴ�������ʼ�� IO��
}
void Accept_to_complete(uint32_t *hour,uint32_t *min)
{
	uint32_t one=0,two=0,three=0,four=0;
	switch(receive[1])
	{
		case '1':
			one = (receive[2]-'0')*10+(receive[3]-'0');
			two = (receive[4]-'0')*10+(receive[5]-'0');
			three = (receive[7]-'0')*10+(receive[6]-'0');
			four = (receive[8]-'0')*10+(receive[9]-'0');
			*hour = one;
			*min = two;
			setTime[3] = three;
			setTime[4] = four;
			USART2_SendStringData("0101");
			break;
		case '2':
			one = (receive[2]-'0')*10+(receive[3]-'0');		
			setPwm = one;
			/***
			����pwm���ı�Ƶ�����
			***/
			CCR4_Val= one*100;
			TIM3_Mode_Config();
			printf("0101");
			break;
		case '3':
			one = (receive[2]-'0')*10+(receive[3]-'0');	//һ������
			if(one==1)
			{	
				BEEP(1);
				Delay(500000);
				BEEP(0);
			}
			printf("0101");
			break;
		case '4':
			printf("03%03d55aa",vTemp/1000);
			break;
		default :
			printf("0202"); 
		break;		
	}
}
void clock(int sec)
{
	while(1)
	{
		BEEP(1);
		Delay(100000);
		BEEP(0);
		Delay(100000);	
		if(setTime[5]==sec)
			break;
	}
	
}
void isTimeOut(int hour,int min)
{
	if(hour==setTime[3]&&min==setTime[4])
	{
		setTime[5]=0;
		clock(20);
	}	
}
/***************************************************/

/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 */

int main(void)
{	
	uint32_t setHour=0,setMinute=0;
	//uint32_t v;
	Init();
	while (1)
	{

		LED1( ON );			  // ��
		Delay(500000);
		LED1( OFF );		  // ��	
		Delay(500000);																										 
		if(receive[19]==1)
		{
			Accept_to_complete(&setHour,&setMinute);
			memset(receive,0,sizeof(receive));	
		}			     
		IWDG_ReloadCounter();
		
		tmpchange();
		vTemp = tmp();
		printf("the temp =%d",vTemp/1000);
		isTimeOut(setHour,setMinute);
	}
}

void Delay(__IO u32 nCount)	 //�򵥵���ʱ����
{
	int i;
	for(; nCount != 0; nCount--)
	{
		for(i = 0;i<10;i++)
		;
	}
} 


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
