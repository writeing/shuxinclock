#include "ds18b20.h"
#include <math.h>

#define uchar unsigned char
#define uint unsigned int 
/**//*****************************DS18B20*******************************/

void DS18_config()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启GPIOC的外设时钟*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 

	/*选择要控制的GPIOB引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	

	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
	
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  

	/*调用库函数，初始化GPIOB*/
  	GPIO_Init(GPIOB, &GPIO_InitStructure);		
}

#define uchar unsigned char
#define uint unsigned int
//uint temp;             // variable of temperature
           // sign of the result positive or negative
//sbit dula=P2^6;
//sbit wela=P2^7;
/*unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
unsigned char code table1[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,
                        0x87,0xff,0xef};
*/
void delay(uint count)      //delay
{
  uint i;
  while(count)
  {
    i=20000;
    while(i>0)
    i--;
    count--;
  }
}



void dsreset()       //send reset and initialization command
{
  uint i;
  DS(0);
  i=103;
  while(i>0)i--;
  DS(1);
  i=4;
  while(i>0)i--;
}



uint8_t tmpreadbit()       //read a bit
{
   uint i;
   uint8_t dat;
   DS(0);i++;          //i++ for delay
   DS(1);i++;i++;
   dat=rDS;
   i=8;while(i>0)i--;
   return (dat);
}

uchar tmpread()   //read a byte date
{
  uchar i,j,dat;
  dat=0;
  for(i=1;i<=8;i++)
  {
    j=tmpreadbit();
    dat=(j<<7)|(dat>>1);   
  }
  return(dat);
}

void tmpwritebyte(uchar dat)   //write a byte to ds18b20
{
  uint i;
  uchar j;
  uint8_t testb;
  for(j=1;j<=8;j++)
  {
    testb=dat&0x01;
    dat=dat>>1;
    if(testb)     //write 1
    {
      DS(0);
      i++;
			i++;
      DS(1);
      i=8;
			while(i>0)
				i--;
    }
    else
    {
      DS(0);       //write 0
      i=8;
			while(i>0)
				i--;
      DS(1);
      i++;
			i++;
    }

  }
}

void tmpchange()  //DS18B20 begin change
{
  dsreset();
  delay(1);
  tmpwritebyte(0xcc);  // address all drivers on bus
  tmpwritebyte(0x44);  //  initiates a single temperature conversion
}

int tmp()               //get the temperature
{
  float tt;
  int temp;
  uchar a,b;
  dsreset();
  delay(1);
  tmpwritebyte(0xcc);
  tmpwritebyte(0xbe);
  a=tmpread(); 
  b=tmpread();
  temp=b;
  temp<<=8;             //two byte  compose a int variable
  temp=temp|a;
  tt=temp*0.0625;
  temp=tt*10+0.5;
  return temp;
}

void readrom()          //read the serial
{
 //uchar sn1,sn2;
  dsreset();
  delay(1);
  tmpwritebyte(0x33);
}
