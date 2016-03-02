#ifndef __DAOLI_PID_H
#define	__DAOLI_PID_H
#include "sys.h"



//extern PIDStru pid_daolibai;
float PID_compute(int encoder_value);
int fuzzy_control(int encoder_value);
void pid_int(void);
extern float   	P;  //P����                               
extern float   	I;  //I����                                 
extern float  	D;  //D����
extern int   		value_of_expectation;//
extern int   		LastEr;  //���һ�ε����                            
extern int   		PrevEr;  //��һ�ε����                           
extern int   		diffEr;  //���ı仯��
extern int   		p_result;
extern int   		i_result;
extern int   		d_result;
extern int 			total_results; //�ܽ��  ���ֵ





#endif
