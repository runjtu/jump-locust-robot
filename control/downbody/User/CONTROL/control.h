#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "stm32f10x.h"

extern int Voltage;                 //��ص�ѹ������صı���
extern float yaw;                   //ת��������
extern float yaw_acc_error;         //yaw�ۻ����
void pid_control(int leftSpeedSet,int rightSpeedSet);

#endif
