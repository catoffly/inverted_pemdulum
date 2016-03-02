#include "frequency.h"
#include "math.h"
u32 Period,time_lag,time_lag1;//time_lag time_lag1 �ֱ�Ϊǰһ���ٶȵĵ���ʱ�� �ͺ�һ���ٶȵĵ���ʱ��
int f0=10;//ǰһ�����ٶ������Ƶ��//����Ƶ200������һȦ��������Ϊ1/250��Ƶ����5000������һȦ
int f1;//��ǰ���Ƶ��
int f2;//xxƵ��
int t_n=0,t_n1=0;//��1������n����ʱ��  ��1������n-1����ʱ��
float  acce;//���ٶ�
int zhongjian1,zhongjian2;//�м����
short  step=0;//����
u8  meibu=1;
u8 range1,range2,range3,range4;
u8 a_direction=1,v_direction=1;   //���ٶȵķ���,���������ת�ķ���1Ϊ��0Ϊ��
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
	*������:freq
	*�������ã�����Ӧ���ٶȵ�Ƶ�ʼ������
	*������acceleration ���ٶ�
	*����ֵ����
	*���ߣ����־�
	*ʱ�䣺2015/12/4
****************************************************************************************************/
u8  freq (float acceleration)//ÿ�θı��˼��ٶȣ�Ҫ��f0��Ϊǰһ�����ٶ�,t_n��Period�����㣬step���¼���
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
//		TIM4->ARR=Period;//�ı�Ƶ��
//		TIM4->CCR1=Period/2;//�̶�ռ�ձ�Ϊ50%

			if(m==0)
			{
				m=1;
				if(a_direction==1)
					v_direction=1;
				else
					v_direction=0;
			}
			if((a_direction^v_direction)&&step<1)//�ı��ٶȷ���
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
			if(fabs(acceleration)!=fabs(acceleration_last)&&acceleration_last!=0)//���ٶȱ仯֮�󣬼������һ����ʼ��
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
									if(i==98)//�����Ǹ�ʲô�õİ��������ʱ���һ��
									{
										if(!(a_direction^v_direction))
										{step=i+1;}
										else
										{step=i-1;}

										
									}
								}			
					}			
			}
			
			if(step>98)//���������ᳬ����Χ
				step=98;
			if(step<0)
				step=0;
			time_lag=numerical_table[step]/acceleration;
			Period=time_lag/25;
			Period=Period*60000;
			Period=Period/65536;
			if(Period<5)//������������Ƶ�ʣ����ǽǼ��ٶ�Ϊ12��ʱ�������ٶȡ�
				Period=5;
			TIM4->ARR=Period;//�ı�Ƶ��
			TIM4->CCR1=Period/2;//�̶�ռ�ձ�Ϊ50%
			meibu=0;
			acceleration_last=acceleration;
			

}

/****************************************************************************************************
	*������:direction_control_int
	*�������ã��ѿ��Ʋ����������Ķ˿ڳ�ʼ��
	*������void
	*����ֵ��void
	*���ߣ����־�
	*ʱ�䣺2016/1/7
****************************************************************************************************/
void direction_control_int(void)
{		
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_SetBits(GPIOC, GPIO_Pin_1);
		GPIO_SetBits(GPIOC, GPIO_Pin_2);
}




