#include "stm32f10x.h"    //�ٷ���
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "bsp_GeneralTim.h" 
#include "motor.h"
#include "time.h"
#include "bsp_SysTick.h"
#include "./led/bsp_led.h" 
#include "bsp_usart.h"
#include "bsp_adc.h"

uint32_t time = 0;   //0.02s
extern __IO uint16_t ADC_ConvertedValue;
int sign;
// quanju���������ڱ���ת�������ĵ�ѹֵ 	 
extern float ADC_ConvertedValueLocal;  

int main(void)
{
	Motor_12_Config(); //298��������ʼ��
	SysTick_Init();
  //��ʱ������ʼ��
	GENERAL_TIM_Init();
	USART_Config();
	GENERAL_TIM4_Init();
	ADCx_Init();
	usart3_init();
	sign=0;
	
	
 
	while(1)
	{
		        if(sign==1)
						{
		        Motor_2_NRun();
						}//����
       		  else if(sign==2)
						{
						Motor_2_STOP();
						}
						else
            {
							Motor_1_STOP();
							Motor_2_STOP();
						}	         
  }
}

