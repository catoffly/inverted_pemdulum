#ifndef __FREQUENCY_H
#define __FREQUENCY_H	
#include "stm32f10x.h"
#include "math.h"
#include "time.h"
extern u32 Period;
extern int f0;//ǰһ�����ٶ������Ƶ��//����Ƶ200������һȦ��������Ϊ1/250��Ƶ����5000������һȦ
extern int f1;//��ǰ���Ƶ��
extern int f2;//xxƵ��
extern int t_n;//��n����ʱ��
extern u16  acceleration;
extern int zhongjian1,zhongjian2;//�м����
extern short step;//����
extern u8  meibu;
extern u8 a_direction,v_direction;
extern float  acce;
extern float acceleration_last;
u8  freq (float acceleration);
void direction_control_int(void);

#endif


