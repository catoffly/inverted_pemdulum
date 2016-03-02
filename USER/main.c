
#include "sys.h"
#include "time.h"
#include  <stdio.h>
#include  <string.h>
#include "math.h"
#include "frequency.h"
#include "daoli_pid.h"
#include "usart.h"

#define ULTRA_PORT			GPIOB
#define ULTRA_CLK       RCC_APB2Periph_GPIOB
#define ULTRA_TRIG			GPIO_Pin_5

#define TRIG_Send  	PBout(5)
u32 juli=0,juli1=0,juli2=0;
u16 cishu=0,cishu1=0,sycishu=0;//次数，剩余次数
u8 left_state=0,right_state=0,state=0;
char buff[4]={0,0,0,0};
/****************************************************************************************************
//=======================================液晶屏数据线接线==========================================//
//DB0       接PD14 
//DB1       接PD15 
//DB2       接PD0 
//DB3       接PD1 
//DB4~DB12  依次接PE7~PE15
//DB13      接PD8 
//DB14      接PD9
//DB15      接PD10  
//=======================================液晶屏控制线接线==========================================//
//LCD_CS	接PG12	//片选信号
//LCD_RS	接PG0	//寄存器/数据选择信号
//LCD_WR	接PD5	//写信号
//LCD_RD	接PD4	//读信号
//LCD_RST	接PC5	//复位信号
//LCD_LED	接PB0	//背光控制信号(高电平点亮)
//=========================================触摸屏触接线=========================================//
//不使用触摸或者模块本身不带触摸，则可不连接
//MO(MISO)	接PF8	//SPI总线输出
//MI(MOSI)	接PF9	//SPI总线输入
//PEN		接PF10	//触摸屏中断信号
//TCS		接PB2	//触摸IC片选
//CLK		接PB1	//SPI总线时钟
//=========================================已使用GPIO、模块=========================================//

**************************************************************************************************/

//不分频200个脉冲一圈，现在设为1/250分频，即5000个脉冲一圈


int main(void)
{	
	char buff[4];
  SystemInit();//初始化RCC 设置系统主频为72MHZ
	//delay_init();     //延时初始化
	pid_int();
	direction_control_int();
	TIM8_Configuration();
	TIM3_Cap_Init();
	TIM4_PWM_Init(7199,1200);
	//设置电机的初始pwn波 1、2是控制右侧的电机，3、4控制左侧的电机。1、3是前进,2、4是后退。
	TIM_SetCompare1(TIM4,0);
	uart_init(9600);
  while(1)
  {	
		if(meibu==1)
		{
			freq (acce);
		}
		//sprintf(buff,"%d",step);
		//printf("字符串输出 d4=%s\r\n",buff);
		}
 }
 


