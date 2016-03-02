#include "frequency.h"
#include "math.h"
u32 Period,time_lag,time_lag1;//time_lag time_lag1 分别为前一加速度的单步时间 和后一加速度的单步时间
int f0=10;//前一个加速度输出的频率//不分频200个脉冲一圈，现在设为1/250分频，即5000个脉冲一圈
int f1;//当前输出频率
int f2;//xx频率
int t_n=0,t_n1=0;//第1步到第n步的时间  第1步到第n-1步的时间
float  acce;//加速度
int zhongjian1,zhongjian2;//中间变量
short  step=0;//步数
u8  meibu=1;
u8 range1,range2,range3,range4;
u8 a_direction=1,v_direction=1;   //加速度的方向,步进电机旋转的方向，1为正0为反
float acceleration_last;
u32 numerical_table[100]={	92682,38390,29458,24834,21879,19780,18190,
										16931,15902,15040,14305,13668,13110,12615,12172,11772,11410,
										11079,10775,10495,10236,9995,9770,9560,9363,9177,9002,8837,
										8681,8532,8391,8257,8129,8007,7890,7778,7671,7568,7469,7374,
										7282,7194,7109,7026,6947,6870,6796,6724,6654,6587,6521,6458,
										6396,6336,6277,6220,6165,6111,6059,6008,5958,5909,5862,5815,
										5770,5726,5683,5640,5599,5559,5519,5480,5443,5405,5369,5333,
										5298,5264,5230,5197,5165,5133,5102,5071,5041,5012,4983,4954,
										4926,4898,4871,4845,4818,4792,4767,4742,4717,4693,4669,4646
									};
/****************************************************************************************************
	*函数名:freq
	*函数作用：将对应加速度的频率计算出来
	*参数：acceleration 加速度
	*返回值：无
	*作者：李林峻
	*时间：2015/12/4
****************************************************************************************************/
u8  freq (float acceleration)//每次改变了加速度，要把f0设为前一个加速度,t_n、Period、清零，step重新计算
{		u8 i=0;
	  static u8 m=0;
//		zhongjian1=f0*f0+(2*step-2)*acceleration;
//		zhongjian1=sqrt(zhongjian1);
//		zhongjian2=f0/acceleration;
//		t_n=zhongjian1-zhongjian2;
//		t_n=t_n-t_n1;
//		f1=f0+t_n*acceleration/2;
//		f0=f1;
//		if(f1>50000)
//			f1=50000;
//		Period=1000000/f1-1;
//		t_n1=t_n;
//		TIM4->ARR=Period;//改变频率
//		TIM4->CCR1=Period/2;//固定占空比为50%

			if(m==0)
			{
				m=1;
				if(a_direction==1)
					v_direction=1;
				else
					v_direction=0;
			}
			if((a_direction^v_direction)&&step<1)//改变速度方向
			{
				if(a_direction==1)
					v_direction=1;
				else
					v_direction=0;
			
			}
			if(acceleration==0)
			{
				acceleration_last=acceleration;
				TIM4->CCR1=0;
				return 0;
			}
			acceleration=sqrt(acceleration);
			if(fabs(acceleration)!=fabs(acceleration_last)&&acceleration_last!=0)//加速度变化之后，计算从哪一步开始走
			{
					if(acceleration>acceleration_last)
					{
								for(i=0;i<step;i++)
								{
									time_lag1=numerical_table[i]/acceleration;
									if(time_lag1<=time_lag)
									{
										
										if(!(a_direction^v_direction))//
										{step=i+1;}
										else
										{step=i-1;}
										break;									
									}							
								}					
					}
					else
					{
						for(i=step;i<99;i++)
								{
									time_lag1=numerical_table[i]/acceleration;
										if(time_lag1<=time_lag)
									{
										if(!(a_direction^v_direction))
										{step=i+1;}
										else
										{step=i-1;}
										break;
										
									}
									if(i==98)//忘了是干什么用的啊，仿真的时候查一下
									{
										if(!(a_direction^v_direction))
										{step=i+1;}
										else
										{step=i-1;}

										
									}
								}			
					}			
			}
			
			if(step>98)//限制数表不会超出范围
				step=98;
			if(step<0)
				step=0;
			time_lag=numerical_table[step]/acceleration;
			Period=time_lag/25;
			Period=Period*60000;
			Period=Period/65536;
			if(Period<5)//限制输出的最高频率，这是角加速度为12的时候的最高速度。
				Period=5;
			TIM4->ARR=Period;//改变频率
			TIM4->CCR1=Period/2;//固定占空比为50%
			meibu=0;
			acceleration_last=acceleration;
			

}

/****************************************************************************************************
	*函数名:direction_control_int
	*函数作用：把控制步进电机方向的端口初始化
	*参数：void
	*返回值：void
	*作者：李林峻
	*时间：2016/1/7
****************************************************************************************************/
void direction_control_int(void)
{		
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_SetBits(GPIOC, GPIO_Pin_1);
		GPIO_SetBits(GPIOC, GPIO_Pin_2);
}




