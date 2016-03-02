#ifndef __DAOLI_PID_H
#define	__DAOLI_PID_H
#include "sys.h"



//extern PIDStru pid_daolibai;
float PID_compute(int encoder_value);
int fuzzy_control(int encoder_value);
void pid_int(void);
extern float   	P;  //P参数                               
extern float   	I;  //I参数                                 
extern float  	D;  //D参数
extern int   		value_of_expectation;//
extern int   		LastEr;  //最近一次的误差                            
extern int   		PrevEr;  //上一次的误差                           
extern int   		diffEr;  //误差的变化量
extern int   		p_result;
extern int   		i_result;
extern int   		d_result;
extern int 			total_results; //总结果  输出值





#endif
