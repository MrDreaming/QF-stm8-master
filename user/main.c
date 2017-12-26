#include "stm8s.h"
#include "stm8s_clk.h"
#include "intrinsics.h"
#include "led.h"
#include "tim1.h"
#include "server.h"

void testDelay();

int main(void){
  testDelay();
}

void testDelay(){
   CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  /*!<Set High speed internal clock */
   LED_Init();
   Tim1_Init();
   LED1_Open(); 
   
   Gpio_Led_Init();
   __enable_interrupt(); 
  while (1){
    Gpio_Led_Reverse();
    delay_ms(500);
  }
}