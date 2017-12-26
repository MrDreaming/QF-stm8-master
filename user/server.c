#include "server.h"

/* Initial clock */
void Clk_Init() {
    CLK_DeInit();
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

/* a very simple delay whitch not exactitude */
void Delay(uint32_t time){
    while(time != 0)
      --time;
}

/* 
Initial key whitch using gpio 
bInterrupt : true=interrupt mode, false=asking by looper
*/
void Gpio_Key_Init(bool bInterrupt)
{
    if(bInterrupt)
    {
        GPIO_Init(GPIOC,GPIO_PIN_1,GPIO_MODE_IN_PU_IT);  
        GPIO_Init(GPIOC,GPIO_PIN_2,GPIO_MODE_IN_PU_IT);
        EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_FALL_LOW);
    }
    else
    { 
        GPIO_Init(GPIOC,GPIO_PIN_1,GPIO_MODE_IN_PU_NO_IT);
        GPIO_Init(GPIOC,GPIO_PIN_2,GPIO_MODE_IN_PU_NO_IT);
    }
}

/* read the keyboard by loop asking */
bool KeyScanProc(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin) {
    if(GPIO_ReadInputPin(GPIOx,GPIO_Pin) == Key_Down) {
        Delay(10000); 
        if(GPIO_ReadInputPin(GPIOx,GPIO_Pin) == Key_Down){
          while(GPIO_ReadInputPin(GPIOx,GPIO_Pin) == Key_Down);
                return true;
        }else{
            return false;
        }
    }
    return false;
}

/* Uart1 mode */
void InitUart1(uint32_t Rate,bool bInterrupt) {
    UART1_DeInit();
    UART1_Init(
        Rate,
        UART1_WORDLENGTH_8D,
        UART1_STOPBITS_1,
        UART1_PARITY_NO,
        UART1_SYNCMODE_CLOCK_DISABLE,
        UART1_MODE_TXRX_ENABLE); //initial com-01
    if(bInterrupt){
        UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE); //configure receive by interrupt
    }
    UART1_Cmd(ENABLE);       //set com to work
}

void Uart1SendData(uint8_t *buf,uint16_t len){
   uint16_t i;
   for(i = 0; i < len; ++i) {   
      UART1_SendData8(buf[i]);
      //send-data-register shouldnot be empty
      while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
   }
   while((UART1_GetFlagStatus(UART1_FLAG_TC)==RESET));
}

uint8_t Uart1RecvData(){
    uint8_t Recvbuf;
    //receive-data-register shouldnot be empty
    while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET);
    Recvbuf = UART1_ReceiveData8();
    return  Recvbuf;
}

/* led mode */
void Gpio_Led_Init(){
     GPIO_Init(GPIOD,GPIO_PIN_2,GPIO_MODE_OUT_PP_HIGH_FAST);
     Gpio_Led_Close();
}

void Gpio_Led_Reverse(){
     GPIO_WriteReverse(GPIOD,GPIO_PIN_2);
}

void Gpio_Led_Open(){
     // write 1 to open
     GPIO_WriteHigh(GPIOD,GPIO_PIN_2);
}

void Gpio_Led_Close(){
      // write 0 to close
     GPIO_WriteLow(GPIOD,GPIO_PIN_2);
}


