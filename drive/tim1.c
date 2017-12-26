#include "tim1.h"
static  u32 TimingDelay; 

void Tim1_Init(void){
  TIM1_TimeBaseInit(16,TIM1_COUNTERMODE_UP,1000,0);//16��Ƶ�����ϼ������������˴����жϣ������
//�� 1MS �����жϣ��������Ӧ�ü��� 1000 �Σ�1MHZ/1000=1KHZ�����þ��� 1ms��
  TIM1_ARRPreloadConfig(ENABLE);//ʹ���Զ���װ
  TIM1_ITConfig(TIM1_IT_UPDATE , ENABLE);//���ݸ����ж�
  TIM1_Cmd(ENABLE);//����ʱ��
}

void TimingDelay_Decrement(void){
  if (TimingDelay != 0x00) { 
	TimingDelay--;
  }
}

void delay_ms( u32 nTime){
  TimingDelay = nTime;
  while(TimingDelay != 0);
}
