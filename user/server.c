#include "server.h"
/*
** ʱ��ģ��
**
*/
void Clk_Init()         //��ʼ��ʱ��
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
** ����ģ��
**
*/

void Gpio_Key_Init(bool bInterrupt)
{
    if(bInterrupt)
    {
        GPIO_Init(GPIOC,GPIO_PIN_1,GPIO_MODE_IN_PU_IT);                 //�ڶ��������ǰ�����Ҫ�ⲿ���ж�
        GPIO_Init(GPIOC,GPIO_PIN_2,GPIO_MODE_IN_PU_IT);
        EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_FALL_LOW);//�ⲿ�жϴ�����ʽ
    }
    else
    { 
        GPIO_Init(GPIOC,GPIO_PIN_1,GPIO_MODE_IN_PU_NO_IT);  //�ڶ��������ǰ���ɨ�費��Ҫ�ⲿ���ж�
        GPIO_Init(GPIOC,GPIO_PIN_2,GPIO_MODE_IN_PU_NO_IT);
    }
}

//��ѯ��ʽ������
bool KeyScanProc(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin) //�����жϵļ���ɨ��
{
    if(GPIO_ReadInputPin(GPIOx,GPIO_Pin) == Key_Down)           //��⵽��������
    {
        Delay(10000);                                          //���ȥ������
        if(GPIO_ReadInputPin(GPIOx,GPIO_Pin) == Key_Down)      //������İ�����
        {
          while(GPIO_ReadInputPin(GPIOx,GPIO_Pin) == Key_Down); //�ȴ���������
                return true;
        }
        else
            return false;
    }
    return false;
}

/*
** Uart1ģ��
**
*/

void InitUart1(uint32_t Rate,bool bInterrupt)
{
    UART1_DeInit();
    UART1_Init(Rate,UART1_WORDLENGTH_8D,UART1_STOPBITS_1,UART1_PARITY_NO,UART1_SYNCMODE_CLOCK_DISABLE,UART1_MODE_TXRX_ENABLE); //��ʼ������1
    if(bInterrupt)
        UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);          //���óɽ����ж�
    UART1_Cmd(ENABLE);                                  //ʹ�ܴ���
}

void Uart1SendData(uint8_t *buf,uint16_t len)
{
   uint16_t i;
   for(i = 0; i < len; ++i)
   {   
      UART1_SendData8(buf[i]);
      while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);//��������Ϊ��
   }
   while((UART1_GetFlagStatus(UART1_FLAG_TC)==RESET));
}

uint8_t Uart1RecvData()
{
    uint8_t Recvbuf;
    while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET);//�������ݼĴ����ǿ�
    Recvbuf = UART1_ReceiveData8();
    return  Recvbuf;
}


/*
**  Ledģ��
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
     GPIO_WriteHigh(GPIOD,GPIO_PIN_2);//����͵�ƽ
     //GPIO_WriteHigh(GPIOD,GPIO_PIN_3);//����͵�ƽ
}

void Gpio_Led_Close()
{
     GPIO_WriteLow(GPIOD,GPIO_PIN_2);//����ߵ�ƽ
     //GPIO_WriteLow(GPIOD,GPIO_PIN_3);//����ߵ�ƽ
}


