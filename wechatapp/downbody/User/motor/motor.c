#include "motor.h"  //����ledͷ�ļ�
#include "stm32f10x.h"  //����STM32�ٷ���
#include "stm32f10x_rcc.h"  //����STM32��RCCʱ�ӿ�
#include "pid.h"
#include "encoder.h"//��ʱ��
#include "bsp_usart.h"

int motorLeft     = 0;
int motorRight    = 0;         

//����1000֮����ٶ�ʵʱֵ
int leftSpeedNow=0; 
int rightSpeedNow=0; 

//����1000֮����ٶ��趨ֵ
int leftSpeedSet  = 0; 
int rightSpeedSet = 0; 
 
void Motor_12_Config(void)  //�����ʼ������
{
	GPIO_InitTypeDef GPIO_InitStructure;   //����GPIO_InitTypeDef�ṹ��
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);  //��������ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5; //����IN����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //ͨ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��GPIOA�����Ų���,д��
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5); //������������
	
}


void Motor_1_STOP(void)
{
	IN1(Low);
	IN2(Low);
}
 
void Motor_1_PRun(void)
{
	IN1(Low);
	IN2(High);
}
 
void Motor_1_NRun(void)
{
	IN1(High);
	IN2(Low);
}
 
void Motor_2_STOP(void)
{
	IN3(High);
	IN4(High);
}
 
void Motor_2_PRun(void)
{
	IN3(Low);
	IN4(High);
}
 
void Motor_2_NRun(void)
{
	IN3(High);
	IN4(Low);
}


void Set_Pwm(int motorLeft,int motorRight)
{
	if(motorLeft<0) Motor_1_PRun();
	
	else Motor_1_NRun();
//	printf("PWM_left_abs=%d\r\n",myabs(motorLeft));
	TIM_SetCompare1(TIM3,myabs(motorLeft));
	
	if(motorRight>0) Motor_2_PRun();
	
	else Motor_2_NRun();
//  printf("PWM_right_abs=%d\r\n",myabs(motorRight));
	TIM_SetCompare4(TIM3,myabs(motorRight));
}


int myabs(int a)
{ 		   
	int temp;
	if(a<0)  
	  temp=-a;  
	else 
	  temp=a;
	return temp;
}




