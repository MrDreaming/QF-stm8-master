/******************** (C) COPYRIGHT 2012 ������ ********************
 * �ļ���  ��led.c
 * ����    ��         
 * ʵ��ƽ̨�����stm8������
 * ����    ��LED��˸
 * ����    �����
 * ����    ��qfv5.taobao.com



**********************************************************************/

#include "led.h"

void LED_Init(void)
{
   GPIO_Init(LED1_PORT,LED1_PIN,GPIO_MODE_OUT_PP_HIGH_FAST );//����LED�Ĺܽŵ�ģʽ
   GPIO_Init(LED2_PORT,LED2_PIN,GPIO_MODE_OUT_PP_HIGH_FAST );
   GPIO_Init(LED3_PORT,LED3_PIN,GPIO_MODE_OUT_PP_HIGH_FAST );
}

void LED1_Open(void)
{
GPIO_WriteLow(LED1_PORT,LED1_PIN);
}
void LED1_Close(void)
{
 GPIO_WriteHigh(LED1_PORT,LED1_PIN);
}

void LED2_Open(void)
{
GPIO_WriteLow(LED2_PORT,LED2_PIN);
}
void LED2_Close(void)
{
 GPIO_WriteHigh(LED2_PORT,LED2_PIN);
}

void LED3_Open(void)
{
GPIO_WriteLow(LED3_PORT,LED3_PIN);
}
void LED3_Close(void)
{
 GPIO_WriteHigh(LED3_PORT,LED3_PIN);
}
void LED1_Toggle(void)
{
GPIO_WriteReverse(LED1_PORT,LED1_PIN);
}
void LED2_Toggle(void)
{
GPIO_WriteReverse(LED2_PORT,LED2_PIN);
}
void LED3_Toggle(void)
{
GPIO_WriteReverse(LED3_PORT,LED3_PIN);
}
