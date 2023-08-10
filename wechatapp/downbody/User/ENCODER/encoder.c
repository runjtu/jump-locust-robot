#include "encoder.h"
#include "bsp_usart.h"

__IO int16_t EncoderLeft_overflow_cnt = 0;       /* ��ʱ���������(��) */
__IO int16_t EncoderRight_overflow_cnt = 0;      /* ��ʱ���������(��) */

/**************************************************************************
�������ܣ���TIM2��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    //ʹ�ܶ�ʱ��2��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //ʹ��PA�˿�ʱ��

	/* ���ñ�����A�ࡢB����������ģʽ�� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	  //�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);					        //�����趨������ʼ��GPIOA

	/* ʱ���ṹ���ʼ�� */
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;        //��ʱ�����ڣ�TIM_Period+1���趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;                    //����Ԥ��Ƶ���������Ƶ�ʣ�72MHz/(ENCODER_TIM_Prescaler+1)/(ENCODER_TIM_Period+1)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);               //��ʼ����ʱ��
	
	/* ��ʱ��������ģʽ���� */
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3�������ؼ���
  TIM_ICStructInit(&TIM_ICInitStructure);         //����ȱʡֵ
	TIM_ICInitStructure.TIM_ICFilter = 6;           //ѡ������Ƚ��˲��� 
  TIM_ICInit(TIM2, &TIM_ICInitStructure);         //��TIM_ICInitStructure�е�ָ��������ʼ��TIM2
	
	/* ������NVIC���� */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           //�����ж�Դ
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //������ռ���ȼ�Ϊ1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //���������ȼ�Ϊ2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);

  /* ������й����ж� */
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);                  //���TIM2�ĸ��±�־λ
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
//	//Reset counter
//	TIM_SetCounter(TIM2,0);
	//===============================================
	TIM2->CNT = 0;  //����������ֵ��ʼ��
	//===============================================
	TIM_Cmd(TIM2, ENABLE); 
}

/**************************************************************************
�������ܣ���TIM4��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM4(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);    //ʹ�ܶ�ʱ��2��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //ʹ��PA�˿�ʱ��

	/* ���ñ�����A�ࡢB����������ģʽ�� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	  //�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);					        //�����趨������ʼ��GPIOA

	/* ʱ���ṹ���ʼ�� */
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;        //��ʱ�����ڣ�TIM_Period+1���趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;                    //����Ԥ��Ƶ���������Ƶ�ʣ�72MHz/(ENCODER_TIM_Prescaler+1)/(ENCODER_TIM_Period+1)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);               //��ʼ����ʱ��
	
	/* ��ʱ��������ģʽ���� */
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3�������ؼ���
  TIM_ICStructInit(&TIM_ICInitStructure);         //����ȱʡֵ
	TIM_ICInitStructure.TIM_ICFilter = 6;           //ѡ������Ƚ��˲��� 
  TIM_ICInit(TIM4, &TIM_ICInitStructure);         //��TIM_ICInitStructure�е�ָ��������ʼ��TIM4
	
	/* ������NVIC���� */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;           //�����ж�Դ
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //������ռ���ȼ�Ϊ1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //���������ȼ�Ϊ3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);

  /* ������й����ж� */
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);                  //���TIM4�ĸ��±�־λ
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
//	//Reset counter
//	TIM_SetCounter(TIM4,0);
	//===============================================
	TIM4->CNT = 0;  //����������ֵ��ʼ��
	//===============================================
	TIM_Cmd(TIM4, ENABLE); 
}

//��ʱ��2�жϷ�����
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)   //����ж�
	{
		if (!(TIM2->CR1 & TIM_CR1_DIR))
		{
			--EncoderRight_overflow_cnt;    //����������ֵ[����]���
		}
		else
		{
			++EncoderRight_overflow_cnt;    //����������ֵ[����]���
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     //����жϱ�־λ
	}
}

//��ʱ��4�жϷ�����
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)   //����ж�
	{
		if (!(TIM4->CR1 & TIM_CR1_DIR))
		{
			--EncoderLeft_overflow_cnt;    //����������ֵ[����]���
		}
		else
		{
			++EncoderLeft_overflow_cnt;    //����������ֵ[����]���
		}
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     //����жϱ�־λ
	}
}

/**************************************************************************
�������ܣ���ȡ�����������ֵ
��ڲ�����TIM_TypeDef * TIMx
����  ֵ����
**************************************************************************/
u16 getTIMx_DetaCnt(TIM_TypeDef * TIMx)
{
	u16 cnt;
	cnt = TIMx->CNT-0;
//	printf("c=%d\r\n",cnt);
	TIMx->CNT = 0;
	return cnt;
}

/**************************************************************************
�������ܣ���ȡ�����������ۼ�ֵ
��ڲ�����TIM_TypeDef * TIMx
����  ֵ����
**************************************************************************/
int32_t getTIMx_Cnt(TIM_TypeDef * TIMx)
{
	int32_t cnt;
	if(TIMx == TIM2)  //�Ҳ������
	{
	  cnt = EncoderRight_overflow_cnt * (1 + TIMx->ARR) + TIMx->CNT;
	}
	if(TIMx == TIM4)  //��������
	{
		cnt = EncoderLeft_overflow_cnt * (1 + TIMx->ARR) + TIMx->CNT;
	}
	return cnt;
}

/**************************************************************************
�������ܣ�������������
��ڲ�����int *leftSpeed,int *rightSpeed
����  ֵ����
		//�������ҳ������ٶȣ������ٶ�Ϊ��ֵ �������ٶ�Ϊ��ֵ���ٶ�Ϊ����1000֮����ٶ� mm/s
		//һ��ʱ���ڵı������仯ֵ*ת���ʣ�ת��Ϊֱ���ϵľ���m��*200s��5ms����һ�Σ� �õ� m/s *1000ת��Ϊint����

		һȦ����������
			��7
			�ң�7
		���Ӱ뾶��0.0015
		�����ܳ���2*pi*r
		һ������ľ��룺
			��0.001345714m
			�ң�0.001345714m
		�ٶȷֱ��ʣ�
			��0.0120m/s 12.0mm/s
			�ң�0.0181m/s 18.1mm/s
**************************************************************************/
static int32_t leftWheelEncoderNow    = 0;
static int32_t rightWheelEncoderNow   = 0;
static int32_t leftWheelEncoderLast   = 0;
static int32_t rightWheelEncoderLast  = 0;	
void Get_Motor_Speed(int *leftSpeed,int *rightSpeed)
{
	//��¼�������ұ���������
//	leftWheelEncoderNow += getTIMx_DetaCnt(TIM4);
//	rightWheelEncoderNow+= getTIMx_DetaCnt(TIM2);
	leftWheelEncoderNow = getTIMx_Cnt(TIM4);
	rightWheelEncoderNow = getTIMx_Cnt(TIM2);
//	printf("left=%d,right=%d\r\n",leftWheelEncoderNow,rightWheelEncoderNow);
		
	//5ms����    	   	
//	*leftSpeed   = leftWheelEncoderNow / 7 * 20;  
//	*rightSpeed  = rightWheelEncoderNow / 7 * 20;	
	*leftSpeed   = (leftWheelEncoderNow - leftWheelEncoderLast)*100;    //���������ת�٣�ת/min
	*rightSpeed  = (rightWheelEncoderNow - rightWheelEncoderLast)*100;  //�����Ҳ���ת�٣�ת/min
//  printf("leftSpeed=%d,rightSpeed=%d\r\n",*leftSpeed,*rightSpeed);

	//��¼�ϴα���������
	leftWheelEncoderLast  = leftWheelEncoderNow;
	rightWheelEncoderLast = rightWheelEncoderNow;                   
}


