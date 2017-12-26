#ifndef __SERVER_H
#define __SERVER_H
#include "stm8s.h"
#include "stm8s_clk.h"
#include "stm8s_gpio.h"
#include "stm8s_uart1.h"
#include "stdbool.h"

#define Key_Up          1
#define Key_Down        0

#define countof(a) (sizeof(a) / sizeof(*(a))) //zzy 2017-12-25

void Delay(uint32_t time);
void Clk_Init();

void InitUart1(uint32_t Rate,bool bInterrupt);
void Uart1SendData(uint8_t *buf,uint16_t len);
uint8_t Uart1RecvData();

void Gpio_Key_Init(bool bInterrupt);
bool KeyScanProc(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin); //不带中断的键盘扫描

void Gpio_Led_Init();
void Gpio_Led_Open();
void Gpio_Led_Close();
void Gpio_Led_Reverse();
#endif
