#include "stm32f10x.h"    //�ٷ���
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "motor.h"
#include "time.h"
#include "bsp_SysTick.h"
#include "./led/bsp_led.h" 
#include "bsp_usart.h"
#include "encoder.h"
#include "control.h"	
#include "pid.h"
#include "pwm.h"

int sign;
//uint32_t time = 0;   //0.02s
 

int main(void)
{
	sign=0;
	SysTick_Init();  //��ʱ������ʼ��
	USART_Config();
	Motor_12_Config(); //298��������ʼ��
	
	Encoder_Init_TIM2();            //=====��ʼ��������1�ӿ�
	Encoder_Init_TIM4();

//  PWM_Init();
	
//  TIM3_Int_Init(500-1,7200-1);
	
  GENERAL_TIM_Init();

	
	PID_Init();
//	Motor_1_PRun();
//  Motor_2_NRun();	
//  TIM_SetCompare1(TIM3,myabs(5000));
//	TIM_SetCompare4(TIM3,myabs(5000));


	while(1)
	{
		/******************************* ����������********************************/
//    Get_Motor_Speed(&leftSpeedNow,&rightSpeedNow);  //===��ȡ����������ʵ�ٶ�
//		printf("leftSpeed=%d,rightSpeed=%d\r\n",leftSpeedNow,rightSpeedNow);
		/******************************** PID����*********************************/
		if(sign == 1)
		{
		  SysTick_Delay_Ms(10);
      pid_control(-7200,-7200);
		}
		else if(sign == 2)
		{		  
			
			SysTick_Delay_Ms(10);
      pid_control(7200,7200);
		}
		
		else if(sign == 3)
		{
		  SysTick_Delay_Ms(10);
      pid_control(7200,-7200);
		}
		else if(sign == 4)
		{	
			SysTick_Delay_Ms(10);
      pid_control(-7200,7200);
		}
		else
		{
//			sign=0;
			Motor_2_STOP();
			Motor_1_STOP();
		}

  }
}


void TIM3_IRQHandler(void)                            //TIM3�ж�
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);   //���TIMx���жϴ�����λ:TIM �ж�Դ 
		
//		Get_Motor_Speed(&leftSpeedNow,&rightSpeedNow);
		 
//	  Motor_1_PRun();
//    Motor_2_NRun();	
		
	}
}


