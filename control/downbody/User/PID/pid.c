#include "pid.h"
#include "bsp_usart.h"

struct pid_uint pid_Task_Letf;
struct pid_uint pid_Task_Right;

/****************************************************************************
*�������ƣ�PID_Init(void)
*�������ܣ���ʼ��PID�ṹ�����
****************************************************************************/
void PID_Init(void)
{
//����1024ԭ�������ָ��������㣬ȫ�����������㣬����PID�����������ٶȻ����
/***********************�����ٶ�pid****************************/
	pid_Task_Letf.Kp = 4;//1024 * 0.5;//0.4
 	pid_Task_Letf.Ki = 0.44;//1024 * 0;	
	pid_Task_Letf.Kd = 0;//1024 * 1.949; 
	pid_Task_Letf.Ur = 7000;//1024*7000;
	pid_Task_Letf.Adjust   = 0;
	pid_Task_Letf.En       = 1;
	pid_Task_Letf.speedSet = 0;
	pid_Task_Letf.speedNow = 0;
	reset_Uk(&pid_Task_Letf);		
/***********************�����ٶ�pid****************************/
	pid_Task_Right.Kp = 5;//1024*0.6;//0.2����0.5,1.949
 	pid_Task_Right.Ki = 0.5;	//��ʹ�û���
	pid_Task_Right.Kd = 0;//1024*2.2; 
	pid_Task_Right.Ur = 7000;//1024*7000;
	pid_Task_Right.Adjust   = 0;
	pid_Task_Right.En       = 1;
	pid_Task_Right.speedSet = 0;
	pid_Task_Right.speedNow = 0;
	reset_Uk(&pid_Task_Right);
}

/***********************************************************************************************
 �� �� ����void reset_Uk(PID_Uint *p)
 ��    �ܣ���ʼ��U_kk,ekk,ekkk
 ˵    �����ڳ�ʼ��ʱ���ã��ı�PID����ʱ�п�����Ҫ����
 ��ڲ�����PID��Ԫ�Ĳ����ṹ�� ��ַ
************************************************************************************************/
void reset_Uk(struct pid_uint *p)
{
	p->U_kk=0;
	p->ekk=0;
	p->ekkk=0;
}

/***********************************************************************************************
 �� �� ����s32 PID_commen(int set,int jiance,PID_Uint *p)
 ��    �ܣ�PID���㺯��
 ˵    ���������ⵥ��PID�Ŀ�����
 ��ڲ���������ֵ��ʵ��ֵ��PID��Ԫ�ṹ��
 �� �� ֵ��PID������
************************************************************************************************/
s32 PID_common(int set,int jiance,struct pid_uint *p)
{
	int ek=0,U_k=0;

	//ek=jiance - set;   
  ek = set - jiance; 	
	
	U_k=p->U_kk + p->Kp*(ek - p->ekk) + p->Ki*ek + p->Kd*(ek - 2*p->ekk + p->ekkk);
	
	p->U_kk=U_k;
  p->ekkk=p->ekk;
	p->ekk=ek;
	
	if(U_k>(p->Ur))		                                    
    U_k=p->Ur;
	if(U_k<-(p->Ur))
		U_k=-(p->Ur);

	return U_k; 
}

/***********************************************************************************
** �������� ��void Pid_Which(struct pid_uint *pl, struct pid_uint *pr)
** �������� ��pidѡ����	      
***********************************************************************************/
void Pid_Which(struct pid_uint *pl, struct pid_uint *pr)
{
	/**********************�����ٶ�pid*************************/
	if(pl->En == 1)
	{							
//    printf("pls=%d,pln=%d\r\n",pl->speedSet, pl->speedNow);		
		pl->Adjust = PID_common(pl->speedSet, pl->speedNow, pl);	
//    printf("pr=%d,px=%d\r\n",pr->speedSet, pr->speedNow);			
	}	
	else
	{
		pl->Adjust = 0;
		reset_Uk(pl);
		pl->En = 1; 
	}
	/***********************�����ٶ�pid*************************/
//	 printf("En=%d\r\n",pr->En);	
	if(pr->En == 1)
	{
//		printf("prs=%d,prn=%d\r\n",pr->speedSet, pr->speedNow);		
		pr->Adjust = PID_common(pr->speedSet, pr->speedNow, pr);	
//    printf("pr=%d,px=%d\r\n",pr->speedSet, pr->speedNow);			
	}	
	else
	{
		pr->Adjust = 0;
		reset_Uk(pr);
		pr->En = 1; 
	}
}

/*******************************************************************************
 * ��������Pid_Ctrl(int *leftMotor,int  *rightMotor)
 * ����  ��Pid����
 *******************************************************************************/
void Pid_Ctrl(int *leftMotor,int  *rightMotor)
{
	Pid_Which(&pid_Task_Letf, &pid_Task_Right);
//  printf("pla=%d,pra=%d\r\n",pid_Task_Letf.Adjust, pid_Task_Right.Adjust);	
//	*leftMotor  += pid_Task_Letf.Adjust;
//	*rightMotor += pid_Task_Right.Adjust;
	*leftMotor  = pid_Task_Letf.Adjust;
	*rightMotor = pid_Task_Right.Adjust;
}


