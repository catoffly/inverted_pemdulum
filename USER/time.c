/*----------------------------------------------------------------------------
* Version            : V2.0.1
* Date               : 08/01/2013
* Description        : IIC basic function
********************************************************************************
********************************************************************************
*******************************aircraft****************************************/
#include "stm32f10x.h"  
#include "stm32f10x_tim.h"  
#include "system_stm32f10x.h"
#include "time.h"
#include "sys.h"
#include "frequency.h"
// #include "init.h"
#include "daoli_pid.h"
#include "frequency.h"
#include "usart.h"
#include "stdio.h"

u8  TIM5CH1_CAPTURE_STA=0,TIM2CH1_CAPTURE_STA=0,TIM3CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM5CH1_CAPTURE_VAL,TIM2CH1_CAPTURE_VAL,TIM3CH1_CAPTURE_VAL;	//���벶��ֵ




extern void control(void);
extern void control_test(void);

uint64_t System_uTime = 0;	//ϵͳʱ��um
uint64_t System_mTime = 0;	//ϵͳʱ��ms
	 
struct 
{
    volatile uint32_t msPeriod;    // �����ڵ�ʱ�䣬ms��
    uint32_t ticksPerUs;  // ÿus���ڵ�tick����
    uint32_t ticksPerMs;  // ÿms���ڵ�tick����
    uint32_t msPerPeriod; // ÿ���ڵ�ms����
}time;

// ��ʼ��ʱ�䡣
void delay_init(void)
{
    time.msPeriod = 0;
    time.ticksPerUs = SystemCoreClock / 1e6;
    time.ticksPerMs = SystemCoreClock / 1e3;
    time.msPerPeriod = 10;
    SysTick_Config(SystemCoreClock/(1000/time.msPerPeriod));
}

// SysTick�жϡ�
void SysTick_Handler(void)
{
    time.msPeriod += time.msPerPeriod;
	System_mTime +=10;
}

// ��ȡ��ǰʱ�䣬us��
uint64_t time_nowUs(void)
{
    return time.msPeriod * (uint64_t)1000 + (SysTick->LOAD - SysTick->VAL) / time.ticksPerUs;
}

// ��ȡ��ǰʱ�䣬ms��
uint32_t time_nowMs(void)
{
    return time.msPeriod + (SysTick->LOAD - SysTick->VAL) / time.ticksPerMs;
}

// ��ʱdelay us��delay>=4ʱ��׼ȷ��
void delay_us(uint32_t delay)
{
    uint64_t target = time_nowUs() + delay - 2;
    while(time_nowUs() <= target)
        ; // ��ѭ����
}

// ��ʱdelay ms��
void delay_ms(uint32_t delay)
{
    delay_us(delay * 1000);


}




void TIM3_Cap_Init(void)
{	 		
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 49; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =719; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update|TIM_IT_Trigger);
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);			
}

	void TIM3_IRQHandler(void)
{
	u16 pulse;
	pulse=TIM8->CNT;
	if(pulse>58000)
		TIM8->CNT =pulse-28000;
	if(pulse<6000)
		TIM8->CNT =pulse+24000;
	acce=fuzzy_control(pulse);
	
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update|TIM_IT_Trigger);	
} 

/************************************************************************************************
	* ��������TIM1_Configuration
	* �������ã�TIM1�ⲿ�������ģʽ��ʼ��
	* ��������
	* ��汾  ��ST3.5.0
*************************************************************************************************/
void TIM1_Configuration(void)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
	 
		//����GPIO
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
		//����AFIO
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		//����TIM1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	 TIM_DeInit(TIM1);
   TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 
	 TIM_TimeBaseStructure.TIM_Prescaler = 0x00; 
	 TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	 TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); // 
	 TIM_ETRClockMode1Config(TIM1, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x00); 
	 TIM_SetCounter(TIM1, 0); 
	 TIM_Cmd(TIM1, ENABLE); 
	

}
/************************************************************************************************
	* ��������TIM8_Configuration
	* �������ã�TIM8�ⲿ�������ģʽ��ʼ��
	* ��������
	* ��汾  ��ST3.5.0
*************************************************************************************************/
void TIM8_Configuration(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  //PC6 A? PC7 B?
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC,&GPIO_InitStructure);
 
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling
  TIM_TimeBaseStructure.TIM_Period = 0XFFFE; 
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
 
  TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12,
                             TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 6;//ICx_FILTER;
  TIM_ICInit(TIM8, &TIM_ICInitStructure);
 
 // Clear all pending interrupts
  TIM_ClearFlag(TIM8, TIM_FLAG_Update);
  TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM8->CNT = 30000;
 
  TIM_Cmd(TIM8, ENABLE);
	

}


void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��4ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��.��������
	
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC1
	
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
//	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC2
//	
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
//	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM5 OC3
//	
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
//	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM5 OC4

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR3�ϵ�Ԥװ�ؼĴ���
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
//	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
//	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
 
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM3
// 	TIM_SetCompare1(TIM4,0);
// 	TIM_SetCompare2(TIM4,0);
// 	TIM_SetCompare3(TIM4,0);
// 	TIM_SetCompare4(TIM4,0);//��ֹ�������ת��


}
void TIM4_IRQHandler(void)
{
	static u16 n=0;
	if (TIM_GetITStatus(TIM4,TIM_IT_CC1 ) != RESET)
	{n++;
	if(n==25)
	{
		n=0;
		if(!(a_direction^v_direction))//��ͬ���٣��෴���١�
		{step++;}
		else
		{step--;}
		meibu=1;
//		sprintf(buff,"%d",step);
//		printf("�ַ������ d4=%s\r\n",buff);
	}
	}


	TIM_ClearITPendingBit(TIM4, TIM_IT_Update|TIM_IT_CC1); //����жϱ�־λ
}
void TIM2_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM2_ICInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ��TIM5ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ; //PA0 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);						 //PA0 ����
	
	//��ʼ����ʱ��5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
		//��ʼ��TIM5���벶�����
		TIM2_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC2S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM2_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
   	TIM_Cmd(TIM2,ENABLE ); 	//ʹ�ܶ�ʱ��5
   


}


 
//��ʱ��2�жϷ������	 
void TIM2_IRQHandler(void)
{ 

 	if((TIM2CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM2CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM2CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM2CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM2CH1_CAPTURE_VAL=0XFFFF;
				}else TIM2CH1_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)//����2���������¼�
		{	
			if(TIM2CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM2CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				TIM2CH1_CAPTURE_VAL=TIM_GetCapture2(TIM2);
		   		TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC2P=0 ����Ϊ�����ز���
			}else  								//��δ��ʼ,��һ�β���������
			{
				TIM2CH1_CAPTURE_STA=0;			//���
				TIM2CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM2,0);
				TIM2CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
		   		TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC2P=1 ����Ϊ�½��ز���
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC2|TIM_IT_Update); //����жϱ�־λ
 
}

//��ʱ��5ͨ��1���벶������



// void TIM5_Cap_Init(u16 arr,u16 psc)
// {	 
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//    	NVIC_InitTypeDef NVIC_InitStructure;
// 	TIM_ICInitTypeDef  TIM5_ICInitStructure;
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//ʹ��TIM5ʱ��
//  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
// 	
// 	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 ���֮ǰ����  
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ; //PA0 ����  
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);
// 	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						 //PA0 ����
// 	
// 	//��ʼ����ʱ��5 TIM5	 
// 	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
// 	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
// 	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
// 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
// 	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
//   
// 	//��ʼ��TIM5���벶�����
// 	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
//   	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
//   	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
//   	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
//   	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
//   	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
// 	
// 	//�жϷ����ʼ��
// 	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5�ж�
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
// 	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
// 	
// 	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
// 	
//    	TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5
//    


// }


//  
// //��ʱ��5�жϷ������	 
// void TIM5_IRQHandler(void)
// { 

//  	if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
// 	{	  
// 		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
// 		 
// 		{	    
// 			if(TIM5CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
// 			{
// 				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
// 				{
// 					TIM5CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
// 					TIM5CH1_CAPTURE_VAL=0XFFFF;
// 				}else TIM5CH1_CAPTURE_STA++;
// 			}	 
// 		}
// 	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//����1���������¼�
// 		{	
// 			if(TIM5CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
// 			{	  			
// 				TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
// 				TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
// 		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
// 			}else  								//��δ��ʼ,��һ�β���������
// 			{
// 				TIM5CH1_CAPTURE_STA=0;			//���
// 				TIM5CH1_CAPTURE_VAL=0;
// 	 			TIM_SetCounter(TIM5,0);
// 				TIM5CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
// 		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
// 			}		    
// 		}			     	    					   
//  	}
//  
//     TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
//  
// }




