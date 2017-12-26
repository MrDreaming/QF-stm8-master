#include "server.h"
/*
** 时间模块
**
*/
void Clk_Init()         //初始化时钟
{
    CLK_DeInit();
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

void Delay(uint32_t time)
{
    while(time != 0)
      --time;
}


/*
** 按键模块
**
*/

void Gpio_Key_Init(bool bInterrupt)
{
    if(bInterrupt)
    {
        GPIO_Init(GPIOC,GPIO_PIN_1,GPIO_MODE_IN_PU_IT);                 //第二个参数是按键需要外部的中断
        GPIO_Init(GPIOC,GPIO_PIN_2,GPIO_MODE_IN_PU_IT);
        EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_FALL_LOW);//外部中断触发方式
    }
    else
    { 
        GPIO_Init(GPIOC,GPIO_PIN_1,GPIO_MODE_IN_PU_NO_IT);  //第二个参数是按键扫描不需要外部的中断
        GPIO_Init(GPIOC,GPIO_PIN_2,GPIO_MODE_IN_PU_NO_IT);
    }
}

//轮询方式读键盘
bool KeyScanProc(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin) //不带中断的键盘扫描
{
    if(GPIO_ReadInputPin(GPIOx,GPIO_Pin) == Key_Down)           //检测到按键按下
    {
        Delay(10000);                                          //软件去除抖动
        if(GPIO_ReadInputPin(GPIOx,GPIO_Pin) == Key_Down)      //按键真的按下了
        {
          while(GPIO_ReadInputPin(GPIOx,GPIO_Pin) == Key_Down); //等待按键弹起
                return true;
        }
        else
            return false;
    }
    return false;
}

/*
** Uart1模块
**
*/

void InitUart1(uint32_t Rate,bool bInterrupt)
{
    UART1_DeInit();
    UART1_Init(Rate,UART1_WORDLENGTH_8D,UART1_STOPBITS_1,UART1_PARITY_NO,UART1_SYNCMODE_CLOCK_DISABLE,UART1_MODE_TXRX_ENABLE); //初始化串口1
    if(bInterrupt)
        UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);          //配置成接收中断
    UART1_Cmd(ENABLE);                                  //使能串口
}

void Uart1SendData(uint8_t *buf,uint16_t len)
{
   uint16_t i;
   for(i = 0; i < len; ++i)
   {   
      UART1_SendData8(buf[i]);
      while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);//发送数据为空
   }
   while((UART1_GetFlagStatus(UART1_FLAG_TC)==RESET));
}

uint8_t Uart1RecvData()
{
    uint8_t Recvbuf;
    while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET);//接收数据寄存器非空
    Recvbuf = UART1_ReceiveData8();
    return  Recvbuf;
}


/*
**  Led模块
**
*/

void Gpio_Led_Init()
{
     GPIO_Init(GPIOD,GPIO_PIN_2,GPIO_MODE_OUT_PP_HIGH_FAST);
     //GPIO_Init(GPIOD,GPIO_PIN_3,GPIO_MODE_OUT_PP_HIGH_FAST);
     Gpio_Led_Close();
}

void Gpio_Led_Reverse()
{
     GPIO_WriteReverse(GPIOD,GPIO_PIN_2);
     //GPIO_WriteReverse(GPIOD,GPIO_PIN_3);
}

void Gpio_Led_Open()
{
     GPIO_WriteHigh(GPIOD,GPIO_PIN_2);//输入低电平
     //GPIO_WriteHigh(GPIOD,GPIO_PIN_3);//输入低电平
}

void Gpio_Led_Close()
{
     GPIO_WriteLow(GPIOD,GPIO_PIN_2);//输入高电平
     //GPIO_WriteLow(GPIOD,GPIO_PIN_3);//输入高电平
}


