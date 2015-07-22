/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：main.c
 * 描述    ：     
 * 实验平台：
 * 库版本  ：ST3.5.0
**********************************************************************************/
#include "stm32f10x.h"
#include "led.h"
#include "clock.h"
#include "print.h"
#include "usart.h"
#include <string.h>
void Delay(__IO u32 nCount);
extern uint32_t setTime[6];
uint32_t receive[20];
uint32_t setPwm = 0;
/***************************************************/
void IwdgConfig()
{
    RCC_LSICmd(ENABLE);                              //打开LSI
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    IWDG_SetPrescaler(IWDG_Prescaler_32);

    IWDG_SetReload(4000);      //80ms ,max 0xFFF  0~4095 

    IWDG_ReloadCounter();

    IWDG_Enable();
}
void Init()
{
	SysTickConfig();			//时钟配置
	setTime[5]+=2;
	IwdgConfig();					//看门狗初始化
	LED_GPIO_Config();		   /* LED 端口初始化 */
	USART2_Config();
}
void Accept_to_complete(uint32_t *hour,uint32_t *min)
{
	uint32_t one=0,two=0,three=0,four=0,temp=0;
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
			设置pwm，改变灯的亮度
			***/
			break;
		case '3':
			one = (receive[2]-'0')*10+(receive[3]-'0');	//一键闹醒
			break;
		case '4':
			break;
		default :
			printf("0202"); 
		break;		
	}
}
/***************************************************/

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */

int main(void)
{	
	uint32_t setHour=0,setMinute=0;
	Init();
	while (1)
	{

		LED1( ON );			  // 亮
		Delay(500000);
		LED1( OFF );		  // 灭
		IWDG_ReloadCounter();
		printf("setHour = %d\n",setHour);
		printf("setTime[4] = %d\n",setTime[4]);
		if(receive[19]==1)
		{
			Accept_to_complete(&setHour,&setMinute);
			memset(receive,0,sizeof(receive));	
		}	     
	}
}

void Delay(__IO u32 nCount)	 //简单的延时函数
{
	int i;
	for(; nCount != 0; nCount--)
	{
		for(i = 0;i<10;i++)
		;
	}
} 


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
