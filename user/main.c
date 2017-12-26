#include "stm8s.h"
#include "stm8s_clk.h"
#include "intrinsics.h"
#include "led.h"
#include "tim1.h"
#include "server.h"
 

//static uint8_t Tx_Buffer[] = "good qfv5";

void testKeyInterrupt();

int main(void)
{
  testKeyInterrupt();
}

void testKeyInterrupt(){
  Clk_Init();
  Gpio_Led_Init();
  __enable_interrupt(); 
  Gpio_Key_Init(true);
  while(1);
}

#pragma vector = 7
__interrupt void EXIT_PORTC_IRQHander(void){
  if(GPIO_ReadInputPin(GPIOC,GPIO_PIN_1) == Key_Down){
      while(GPIO_ReadInputPin(GPIOC,GPIO_PIN_1) == Key_Down);
      Gpio_Led_Open();
      __enable_interrupt(); 
      delay_ms(1000);
  }else if(GPIO_ReadInputPin(GPIOC,GPIO_PIN_2) == Key_Down) {
      while(GPIO_ReadInputPin(GPIOC,GPIO_PIN_2) == Key_Down);
      Gpio_Led_Close();
      __enable_interrupt(); 
      delay_ms(1000);
  }
}
