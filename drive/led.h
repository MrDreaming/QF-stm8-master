/******************** (C) COPYRIGHT 2012 ������  ***************************
 * �ļ���  ��led.h
 * ����    ��         
 * ʵ��ƽ̨�����stm8������
 * ����    ��LED��˸
 * ����    �����
 * ����    ��qfv5.taobao.com


****************************************************************************************/

#ifndef __LED_H
#define __LED_H
#include "stm8s_gpio.h"


#define LED1_PIN        GPIO_PIN_5 
#define LED2_PIN        GPIO_PIN_6 
#define LED3_PIN        GPIO_PIN_7 
#define LED1_PORT       GPIOF
#define LED2_PORT       GPIOF
#define LED3_PORT       GPIOF
void LED_Init(void);
void LED1_Open(void);
void LED1_Close(void);
void LED1_Toggle(void);
void LED2_Open(void);
void LED2_Close(void);
void LED2_Toggle(void);
void LED3_Open(void);
void LED3_Close(void);
void LED3_Toggle(void);

#endif

