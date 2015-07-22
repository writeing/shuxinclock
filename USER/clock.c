#include	"clock.h"
static volatile uint32_t TimingDelay = 0;
uint32_t setTime[6]={0,0,0,0,0,0};
/*
 * ��������SysTick_Init
 * ����  ������ϵͳ�δ�ʱ�� SysTick
 * ����  ����
 * ���  ����
 * ����  ���ⲿ���� 
 */
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
void SysTickConfig(void )
{
	if (SysTick_Config(SystemCoreClock / 1000))	// ST3.5.0��汾
	{ 
		/* Capture error */ 
		while (1)
		{
		}
	}
	//ʹ�ܶ�ʱ��
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
	
}
//����ʱ��
void configTime()
{
	int setMouth[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	if(setTime[5] ==60)		   //��һ����
	{
		setTime[4]+=setTime[5]/60;	
		setTime[5] = 0;
		if(setTime[4] == 60)	//��һСʱ
		{
			setTime[3]+=setTime[4]/60;
			setTime[4] = 0;
			if(setTime[3] == 24)		  //��һ��
			{
				setTime[2]+=setTime[3]/24;
				setTime[3] = 0;
				if(setTime[1]==2&&setTime[2]==28)
				{
					if(setTime[0]/400==0&&setTime[0]/4==0&&setTime[0]/100!=0)
					{
						setMouth[2]=29;
					}
					else
					{
						setMouth[2]=28;
					}
				}
				if(setTime[2] == setMouth[setTime[1]]+1) 		//��һ����
				{
					setTime[1]+=setTime[2]/setMouth[setTime[1]];
					setTime[2] = 1;
					if(setTime[1]==13)					   //��һ��
					{
						setTime[0]+=setTime[1]/13;
						setTime[1] = 1;
					}					
				}
			}
		}
	}
}

/*
 * ��������TimingDelay_Decrement
 * ����  ����ȡ���ĳ���
 * ����  ����
 * ���  ����
 * ����  ���� SysTick �жϺ��� SysTick_Handler()����
 */  
void TimingDelay_Decrement(void)
{ 
	if(TimingDelay++==1000)
	{
		setTime[5]++;
		configTime();
		TimingDelay = 0;
	}
	
}
