#include "control.h"	
#include "pid.h"
#include "motor.h"
#include "encoder.h"
#include "bsp_usart.h"

extern int motorLeft;
extern int motorRight;         

/**
  * ��������: PID���ں�����5ms��ʱ���ã��ٶȻ���������������
  * �������: leftSpeedSet���������Ŀ���ٶ�
	*           rightSpeedSet�����Ҳ���Ŀ���ٶ�
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void pid_control(int leftpeedSet,int rightpeedSet) 
{
	Get_Motor_Speed(&leftSpeedNow,&rightSpeedNow);  //��ȡ����������ʵ�ٶ�
	printf("leftSpeed=%d,rightSpeed=%d\r\n",leftSpeedNow,rightSpeedNow);
//	printf("%d\r\n",rightSpeedNow);
	pid_Task_Letf.speedSet  = leftpeedSet;	        //�趨�����Ŀ���ٶ�
	pid_Task_Right.speedSet = rightpeedSet;        //�趨�Ҳ���Ŀ���ٶ�
	pid_Task_Letf.speedNow  = leftSpeedNow;	        //��¼�������ǰʵ���ٶ�
	pid_Task_Right.speedNow = rightSpeedNow;        //��¼�Ҳ�����ǰʵ���ٶ�
	
	Pid_Ctrl(&motorLeft,&motorRight);               //ִ��PID����
	Set_Pwm(motorLeft,motorRight);		              //��ֵ��PWM�Ĵ���
//	printf("motorLeft=%d��motorRight=%d\r\n",motorLeft,motorRight);
} 








