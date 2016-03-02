
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
u16 cishu=0,cishu1=0,sycishu=0;//������ʣ�����
u8 left_state=0,right_state=0,state=0;
char buff[4]={0,0,0,0};
/****************************************************************************************************
//=======================================Һ���������߽���==========================================//
//DB0       ��PD14 
//DB1       ��PD15 
//DB2       ��PD0 
//DB3       ��PD1 
//DB4~DB12  ���ν�PE7~PE15
//DB13      ��PD8 
//DB14      ��PD9
//DB15      ��PD10  
//=======================================Һ���������߽���==========================================//
//LCD_CS	��PG12	//Ƭѡ�ź�
//LCD_RS	��PG0	//�Ĵ���/����ѡ���ź�
//LCD_WR	��PD5	//д�ź�
//LCD_RD	��PD4	//���ź�
//LCD_RST	��PC5	//��λ�ź�
//LCD_LED	��PB0	//��������ź�(�ߵ�ƽ����)
//=========================================������������=========================================//
//��ʹ�ô�������ģ�鱾������������ɲ�����
//MO(MISO)	��PF8	//SPI�������
//MI(MOSI)	��PF9	//SPI��������
//PEN		��PF10	//�������ж��ź�
//TCS		��PB2	//����ICƬѡ
//CLK		��PB1	//SPI����ʱ��
//=========================================��ʹ��GPIO��ģ��=========================================//

**************************************************************************************************/

//����Ƶ200������һȦ��������Ϊ1/250��Ƶ����5000������һȦ


int main(void)
{	
	char buff[4];
  SystemInit();//��ʼ��RCC ����ϵͳ��ƵΪ72MHZ
	//delay_init();     //��ʱ��ʼ��
	pid_int();
	direction_control_int();
	TIM8_Configuration();
	TIM3_Cap_Init();
	TIM4_PWM_Init(7199,1200);
	//���õ���ĳ�ʼpwn�� 1��2�ǿ����Ҳ�ĵ����3��4�������ĵ����1��3��ǰ��,2��4�Ǻ��ˡ�
	TIM_SetCompare1(TIM4,0);
	uart_init(9600);
  while(1)
  {	
		if(meibu==1)
		{
			freq (acce);
		}
		//sprintf(buff,"%d",step);
		//printf("�ַ������ d4=%s\r\n",buff);
		}
 }
 


