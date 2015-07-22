#include	"clock.h"
static volatile uint32_t TimingDelay = 0;
uint32_t setTime[6]={0,0,0,0,0,0};
/*
 * 函数名：SysTick_Init
 * 描述  ：启动系统滴答定时器 SysTick
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用 
 */
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
void SysTickConfig(void )
{
	if (SysTick_Config(SystemCoreClock / 1000))	// ST3.5.0库版本
	{ 
		/* Capture error */ 
		while (1)
		{
		}
	}
	//使能定时器
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
	
}
//配置时间
void configTime()
{
	int setMouth[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	if(setTime[5] ==60)		   //满一分钟
	{
		setTime[4]+=setTime[5]/60;	
		setTime[5] = 0;
		if(setTime[4] == 60)	//满一小时
		{
			setTime[3]+=setTime[4]/60;
			setTime[4] = 0;
			if(setTime[3] == 24)		  //满一天
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
				if(setTime[2] == setMouth[setTime[1]]+1) 		//满一个月
				{
					setTime[1]+=setTime[2]/setMouth[setTime[1]];
					setTime[2] = 1;
					if(setTime[1]==13)					   //满一年
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
 * 函数名：TimingDelay_Decrement
 * 描述  ：获取节拍程序
 * 输入  ：无
 * 输出  ：无
 * 调用  ：在 SysTick 中断函数 SysTick_Handler()调用
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
