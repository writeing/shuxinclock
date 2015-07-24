#ifndef _DS_H_
#define _DS_H_
#include "stm32f10x.h"
#define DS(i) if(i==1)\
					GPIO_SetBits(GPIOB,GPIO_Pin_13);	\
				else\
					{			\
						if(i==0)			\
							GPIO_ResetBits(GPIOB,GPIO_Pin_13);		\
					}
#define rDS GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_13)


void DS18_config(void );
int tmp(void);
void tmpchange(void );
#endif

