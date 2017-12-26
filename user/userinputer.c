#include "server.h"
#include "queue.h"
#include "useruart.h"

#include "tim1.h" // zzy 2017-12-25

static uint8_t Tx_Buffer1[] = "buffer1 ";
static uint8_t Tx_Buffer2[] = "buffer2 ";

//按键中断函数服务函数
#pragma vector = 7                                                 //PC1
__interrupt void EXIT_PORTC_IRQHander(void){
  if(GPIO_ReadInputPin(GPIOC,GPIO_PIN_1) == Key_Down)
  {
      while(GPIO_ReadInputPin(GPIOC,GPIO_PIN_1) == Key_Down);
      Uart1SendData( Tx_Buffer1, countof(Tx_Buffer1)-1);
      __enable_interrupt(); 
      delay_ms(1000);
  }else  if(GPIO_ReadInputPin(GPIOC,GPIO_PIN_2) == Key_Down)  {
      while(GPIO_ReadInputPin(GPIOC,GPIO_PIN_2) == Key_Down);
      Uart1SendData( Tx_Buffer2, countof(Tx_Buffer2)-1);
      __enable_interrupt(); 
      delay_ms(1000);
  }
}

static uint8_t Recvbuf;
static uint8_t Sendbuf[16];
static uint16_t SendCount;

//串口中断服务函数
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18){
   GPIO_WriteReverse(GPIOD,GPIO_PIN_2);
   GPIO_WriteReverse(GPIOD,GPIO_PIN_3);
   
   if(UART1_GetITStatus(UART1_IT_RXNE) == SET)     //先读UART_SR,再读UART_DR,就能清空RXNE
   {  
       Recvbuf = UART1_ReceiveData8();
       InsertMsgQueue(Recvbuf);                     //将收到的内容插入接收队列中
   }
   
   if(Recvbuf == 0x0D) {
      if ( (SendCount = GetMsgQueue(Sendbuf) )> 0)
          Uart1SendData(Sendbuf,SendCount);
   }
}

