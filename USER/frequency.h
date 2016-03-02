#ifndef __FREQUENCY_H
#define __FREQUENCY_H	
#include "stm32f10x.h"
#include "math.h"
#include "time.h"
extern u32 Period;
extern int f0;//前一个加速度输出的频率//不分频200个脉冲一圈，现在设为1/250分频，即5000个脉冲一圈
extern int f1;//当前输出频率
extern int f2;//xx频率
extern int t_n;//第n步的时间
extern u16  acceleration;
extern int zhongjian1,zhongjian2;//中间变量
extern short step;//步数
extern u8  meibu;
extern u8 a_direction,v_direction;
extern float  acce;
extern float acceleration_last;
u8  freq (float acceleration);
void direction_control_int(void);

#endif


