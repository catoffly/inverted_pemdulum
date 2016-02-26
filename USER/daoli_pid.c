#include "daoli_pid.h"
#include "frequency.h"
#include "usart.h"
#include "sys.h"
#include "stdio.h"

	float   P;  //P����                               
  float   I;  //I����                                 
  float   D;  //D����
	int   value_of_expectation;//
  int   LastEr;  //���һ�ε����                            
  int   PrevEr;  //��һ�ε����                           
  int   diffEr;  //���ı仯��
  int   p_result;
  int   i_result;
  int   d_result;
	int 	total_results; //�ܽ��  ���ֵ
	short		angle,angle_a,output;
/****************************************************************************************************
	*������:pid_int
	*��������:��PID�ṹ����г�ʼ��
	*������void
	*����ֵ��void
	*���ߣ����־�
	*ʱ�䣺2016/1/7
****************************************************************************************************/
void pid_int(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		P=8;//P����                               
		I=0;//I����  
		D=0; //D����  
		LastEr=0;//���һ�ε����  
		PrevEr=2000;//��һ�ε���� 
		diffEr=0;//���ı仯��
		p_result=0;
		i_result=0;
		d_result=0;
		total_results=0;//�ܽ��  ���ֵ
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_6;  //PE5.E6 ���֮ǰ����  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//PE5.E6 ���  
		GPIO_Init(GPIOE, &GPIO_InitStructure);
		GPIO_SetBits(GPIOE, GPIO_Pin_5);
		GPIO_SetBits(GPIOE, GPIO_Pin_6);
}
/****************************************************************************************************
	*������:PID_compute
	*��������:ͨ��PID�㷨���������ֵ
	*������float 
	*����ֵ��u16 encoder_value
	*���ߣ����־�
	*ʱ�䣺2016/1/7
****************************************************************************************************/
float PID_compute(int encoder_value)
{
		static u8 jj=0;
		//�һ�Ѽ�������ʼΪ30000����ת��Ȧ���ﶥ�����28000
		value_of_expectation=28000;
		LastEr=fabs(encoder_value-value_of_expectation);
		if(LastEr>4000)//��ת����һȦ���Ե�ǰ����һȦ���м���
		{
			LastEr=LastEr%4000;
			
		}
	
		if(LastEr<=200)
				a_direction=1;
		if(LastEr>=3800)
		{		
			a_direction=0;
			LastEr=4000-LastEr;
		}
		if(LastEr<=200)
		{		
				jj=0;
			if(LastEr<10)//̫С��������㣬����ϵͳ�𵴡�
			{
				total_results=0;
				return total_results;
			}
			GPIO_ResetBits(GPIOE, GPIO_Pin_6);
				diffEr=LastEr-PrevEr;//����ֵ�ļ���
				p_result=P*LastEr;
				d_result=D*diffEr;
				total_results=p_result+d_result;
				if(v_direction==1)
				{
					GPIO_SetBits(GPIOC, GPIO_Pin_1);
					GPIO_ResetBits(GPIOC, GPIO_Pin_2);
				}
				else
				{			
					GPIO_SetBits(GPIOC, GPIO_Pin_2);
					GPIO_ResetBits(GPIOC, GPIO_Pin_1);
				}
				if(total_results<0)
				{
					total_results=-total_results;
				}
				PrevEr=LastEr;
//				sprintf(buff,"%d",pid_daolibai.LastEr);
//				printf("le=%s\r\n",buff);
//				sprintf(buff,"%d",pid_daolibai.total_results);
//				printf("results=%s\r\n",buff);
				return total_results;
		}
		else//ѭ�����״̬
		{
			GPIO_SetBits(GPIOE, GPIO_Pin_6);
			if(step>40)
				step=40;
			if(jj==0)
			{
				total_results=900;
				//a_direction=1;
				jj=1;
			}
			if(step>=40)
			{
				if(a_direction==1)
					a_direction=0;
				else
					a_direction=1;
			}
			if(v_direction==1)
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_1);
				GPIO_ResetBits(GPIOC, GPIO_Pin_2);
			}
			else
			{			
				GPIO_SetBits(GPIOC, GPIO_Pin_2);
				GPIO_ResetBits(GPIOC, GPIO_Pin_1);
			}
			//total_results=0;
			return total_results;
		}
	
}
/****************************************************************************************************
	*������:fuzzy_control
	*��������:ͨ��ģ���߼�����������
	*������int 
	*����ֵ��u16 encoder_value
	*���ߣ����־�
	*ʱ�䣺2016/1/7
****************************************************************************************************/
int fuzzy_control(int encoder_value)
{
		value_of_expectation=28000;
		LastEr=fabs(encoder_value-value_of_expectation);
		if(LastEr>4000)//��ת����һȦ���Ե�ǰ����һȦ���м���
		{
			LastEr=LastEr%4000;
			
		}
		if(v_direction==1)
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_1);
				GPIO_ResetBits(GPIOC, GPIO_Pin_2);
			}
			else
			{			
				GPIO_SetBits(GPIOC, GPIO_Pin_2);
				GPIO_ResetBits(GPIOC, GPIO_Pin_1);
			}

		if(LastEr<=200|LastEr>=3800)
		{		
				GPIO_ResetBits(GPIOE, GPIO_Pin_6);
				diffEr=LastEr-PrevEr;//����ֵ�ļ���
			/****************************************************************************************************
			*����ģ����
			****************************************************************************************************/
			if(LastEr>80)
				angle=3;
			if(80>LastEr>40)
				angle=2;
			if(40>LastEr>20)
				angle=1;
			if(LastEr<20)
				angle=0;
			if(LastEr>3880)
				angle=-3;
			if(3960>LastEr>3880)
				angle=-2;
			if(3980>LastEr>3960)
				angle=-1;
			if(LastEr>3980)
				angle=0;
			if(diffEr>5)
				angle_a=3;
			if(5>diffEr>2)
				angle_a=2;
			if(2>diffEr>2)
				angle_a=1;
			if(diffEr==0)
				angle_a=0;
			if(-2<diffEr<0)
				angle_a=-1;
			if(-5<diffEr<-2)
				angle_a=-2;
			if(diffEr<-5)
				angle_a=-3;
			/****************************************************************************************************
			*ģ���߼��ж�
			****************************************************************************************************/
			switch(angle)
			{
				case 3:
					switch(angle_a)
					{
						case 3:
							output=2;
						case 2:
							output=3;
						case 1:
							output=3;
						case 0:
							output=3;
						case -1:
							output=3;
						case -2:
							output=3;
						case -3:
							output=3;
					}
					case 2:
					switch(angle_a)
					{
						case 3:
							output=2;
						case 2:
							output=2;
						case 1:
							output=3;
						case 0:
							output=3;
						case -1:
							output=3;
						case -2:
							output=3;
						case -3:
							output=3;
					}
					case 1:
					switch(angle_a)
					{
						case 3:
							output=1;
						case 2:
							output=1;
						case 1:
							output=1;
						case 0:
							output=2;
						case -1:
							output=2;
						case -2:
							output=2;
						case -3:
							output=2;
					}
					case 0:
					switch(angle_a)
					{
						case 3:
							output=-2;
						case 2:
							output=-1;
						case 1:
							output=-1;
						case 0:
							output=0;
						case -1:
							output=1;
						case -2:
							output=1;
						case -3:
							output=2;
					}
					case -1:
					switch(angle_a)
					{
						case 3:
							output=-2;
						case 2:
							output=-2;
						case 1:
							output=-2;
						case 0:
							output=-2;
						case -1:
							output=-1;
						case -2:
							output=-1;
						case -3:
							output=-1;
					}
					case -2:
					switch(angle_a)
					{
						case 3:
							output=-3;
						case 2:
							output=-3;
						case 1:
							output=-3;
						case 0:
							output=-3;
						case -1:
							output=-2;
						case -2:
							output=-2;
						case -3:
							output=-2;
					}
					case -3:
					switch(angle_a)
					{
						case 3:
							output=-3;
						case 2:
							output=-3;
						case 1:
							output=-3;
						case 0:
							output=-3;
						case -1:
							output=-3;
						case -2:
							output=-3;
						case -3:
							output=-2;
					}
			/****************************************************************************************************
			*��ģ����
			****************************************************************************************************/	
			}
			switch(output)
			{
				case 3:
					total_results=600;
				case 2:
					total_results=300;
				case 1:
					total_results=50;
				case 0:
					total_results=0;
				case -1:
					total_results=-50;
				case -2:
					total_results=-300;
				case -3:
					total_results=-600;		
			}
			if(total_results>0)
				a_direction=1;
			if(total_results<0)
				a_direction=0;
			
			
		}
		else//ѭ�����״̬
		{
			GPIO_SetBits(GPIOE, GPIO_Pin_6);
			if(step>40)
				step=40;
			total_results=900;
				//a_direction=1;
			if(step>=40)
			{
				if(a_direction==1)
					a_direction=0;
				else
					a_direction=1;
			}
			if(v_direction==1)
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_1);
				GPIO_ResetBits(GPIOC, GPIO_Pin_2);
			}
			else
			{			
				GPIO_SetBits(GPIOC, GPIO_Pin_2);
				GPIO_ResetBits(GPIOC, GPIO_Pin_1);
			}
			//0total_results=0;
			return total_results;
		}



}
