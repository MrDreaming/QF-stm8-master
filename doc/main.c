/************************************************

You should disable interrupt  

set false : 

Gpio_Key_Init(false)

************************************************/

#include "stm8s.h"
#include "stm8s_clk.h"
#include "intrinsics.h"
#include "led.h"
#include "tim1.h"
#include "server.h"
#include <string.h>
#include "userinputer.h"
#define HEARD_TICK 10000

#define KEY_CALL  GPIO_PIN_2
#define KEY_ALARM GPIO_PIN_1

#define KEY_DISALARM GPIO_PIN_2
#define KEY_DOOR01 GPIO_PIN_4
#define KEY_DOOR02 GPIO_PIN_5

#define KEY_MONITOR GPIO_PIN_6
#define KEY_UNBIND GPIO_PIN_7


static uint8_t Tx_Callbtn_down[] =    {0xBA, 0x11, 0x01, 0x01, 0x13, 0xAB};
static uint8_t Tx_Callbtn_up[] =     {0xBA, 0x11, 0x01, 0x02, 0x14, 0xAB};
static uint8_t Tx_Alarmbtn_down[] =     {0xBA, 0x12, 0x01, 0x01, 0x14, 0xAB};
static uint8_t Tx_Alarmbtn_up[] =      {0xBA, 0x12, 0x01, 0x02, 0x15, 0xAB};


static uint8_t Tx_DoorSensor01_down[] = {0xBA, 0x13, 0x02, 0x01, 0x01, 0x17, 0xAB};
static uint8_t Tx_DoorSensor01_up[] =   {0xBA, 0x13, 0x02, 0x01, 0x02, 0x18, 0xAB};
static uint8_t Tx_DoorSensor02_down[] = {0xBA, 0x13, 0x02, 0x02, 0x01, 0x18, 0xAB};
static uint8_t Tx_DoorSensor02_up[] =   {0xBA, 0x13, 0x02, 0x02, 0x02, 0x19, 0xAB};

static uint8_t Tx_MonitorCheck_down[] =       {0xBA, 0x14, 0x01, 0x01, 0x16, 0xAB};
static uint8_t Tx_MonitorCheck_up[] =       {0xBA, 0x14, 0x01, 0x02, 0x17, 0xAB};
static uint8_t Tx_Unbind_down[] =            {0xBA, 0x15, 0x01, 0x01, 0x17, 0xAB};
static uint8_t Tx_Unbind_up[] =            {0xBA, 0x15, 0x01, 0x02, 0x18, 0xAB};

static uint8_t Tx_RemoveGuard_down[] =       {0xBA, 0x16, 0x01, 0x01, 0x18, 0xAB};
static uint8_t Tx_RemoveGuard_up[] =       {0xBA, 0x16, 0x01, 0x02, 0x19, 0xAB};

static uint8_t Tx_DisAlarm_down[] =       {0xBA, 0x17, 0x01, 0x01, 0x19, 0xAB};
static uint8_t Tx_DisAlarm_up[] =       {0xBA, 0x17, 0x01, 0x02, 0x20, 0xAB};

static uint8_t TxReboot_Buffer[] = "rebooting...";
static uint8_t HEARD_TICKER[] = {0xCA, 0x10, 0x00, 0x10, 0xAC}; 
static uint32_t g_heardtickCount;
static uint32_t g_ClockTicker;

static uint8_t countCallDown;
static uint8_t countAlarmDown;

static uint8_t countDoor01Down;
static uint8_t countDoor02Down;
static uint8_t countDisAlarmDown;
static uint8_t countUnbindDown;
static uint8_t countMonitorDown;

static bool isWatchDogStart;

void testDelay();
void testComSendRev();

int main(void)
{
  g_heardtickCount = 0;
  g_ClockTicker = 1; // cannot be 0
  countCallDown = 0;
  countAlarmDown = 0;
  isWatchDogStart = false;
  GPIO_Init(GPIOA,GPIO_PIN_3,GPIO_MODE_OUT_PP_HIGH_FAST); //reboot PA3
  testDelay();
  InitUserInputer();
  Gpio_Key_Init(false);
  testComSendRev();
}


void initClockInterrupt(){
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  Tim1_Init();
  __enable_interrupt(); 
}

void onRecUartData(uint8_t *recvBufs, uint8_t len){
  uint8_t i;
  for(i=0; i<len; i++){
    if(HEARD_TICKER[i] != recvBufs[i]){
      break;
    }   
  }
  if(i>=len){
    g_heardtickCount = 0;
    isWatchDogStart = true;
    //Uart1SendData(HEARD_TICKER, countof(HEARD_TICKER));
  }else{
    //Uart1SendData(HEARD_TICKER, countof(HEARD_TICKER));
  }
}

void testComSendRev(){
  initClockInterrupt(); 
  //CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  InitUart1((u32)9600, FALSE);
  
  uint8_t  recBuf;
  uint8_t i;
  uint8_t recvBufs[32];
  memset(recvBufs, 0, sizeof(recvBufs));
  while (1)
  {
    recBuf = Uart1RecvData();
    if(recBuf == 0xCA){
      i = 0;
      recvBufs[i] = recBuf;
      do{
        i++;
        recBuf = Uart1RecvData();
        recvBufs[i] = recBuf;
      }while(recBuf != 0xAC && i < 32);
      onRecUartData(recvBufs, i);
    }
  }
}

void testDelay(){
   //CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
   initClockInterrupt(); 
  /*!<Set High speed internal clock */
   LED_Init();
   Tim1_Init();
   Gpio_Led_Init();
   //LED2_Open(); 
   //LED1_Open();
   //LED2_Close();
   //GPIO_WriteLow(GPIOD,GPIO_PIN_2);
   //GPIO_WriteHigh(GPIOD,GPIO_PIN_2);
   __enable_interrupt(); 
}

void onTimeTickerPerMs(){
  g_heardtickCount++;
  if(isWatchDogStart && (g_heardtickCount > HEARD_TICK)){
    Uart1SendData(HEARD_TICKER, countof(HEARD_TICKER));
    g_heardtickCount = 0;
    //Uart1SendData(TxReboot_Buffer, countof(TxReboot_Buffer)-1);
    // to reboot PA 3
    isWatchDogStart = false; // when app wakeup watch-dog
    GPIO_WriteReverse(GPIOA,GPIO_PIN_3);// reboot cpu
    //delay_ms(500);
    for(uint32_t i =0; i<65534; i++);// delay_ms() is failed !
    GPIO_WriteReverse(GPIOA,GPIO_PIN_3);
    
  }
  g_ClockTicker++;
  if(g_ClockTicker > 500){
    g_ClockTicker = 1;
    //Gpio_Led_Reverse();
  }
  if(g_ClockTicker == 100
     || g_ClockTicker == 200
     || g_ClockTicker == 300
     || g_ClockTicker == 400
     || g_ClockTicker == 500 ){
      if(GPIO_ReadInputPin(GPIOC,KEY_CALL) == Key_Down){
        if(countCallDown == 0){
          Uart1SendData(Tx_Callbtn_down, countof(Tx_Callbtn_down));
        }
        countCallDown++;
      }else if(GPIO_ReadInputPin(GPIOC,KEY_CALL) == Key_Up && countCallDown > 0){
        Uart1SendData(Tx_Callbtn_up, countof(Tx_Callbtn_up));
        countCallDown = 0;
      }
      
      if(GPIO_ReadInputPin(GPIOC,KEY_ALARM) == Key_Down){
        if(countAlarmDown == 0){
          Uart1SendData(Tx_Alarmbtn_down, countof(Tx_Alarmbtn_down));
        }
        countAlarmDown++;
      }else if(GPIO_ReadInputPin(GPIOC,KEY_ALARM) == Key_Up && countAlarmDown > 0){
        Uart1SendData(Tx_Alarmbtn_up, countof(Tx_Alarmbtn_up));
        countAlarmDown = 0;
      }
      
      if(GPIO_ReadInputPin(GPIOB,KEY_DISALARM) == Key_Down){
        if(countDisAlarmDown == 0){
          Uart1SendData(Tx_DisAlarm_down, countof(Tx_DisAlarm_down));
        }
        countDisAlarmDown++;
      }else if(GPIO_ReadInputPin(GPIOB,KEY_DISALARM) == Key_Up && countDisAlarmDown > 0){
        Uart1SendData(Tx_DisAlarm_up, countof(Tx_DisAlarm_up));
        countDisAlarmDown = 0;
      }
      
      if(GPIO_ReadInputPin(GPIOB,KEY_DOOR01) == Key_Up){
        if(countDoor01Down == 0){
          Uart1SendData(Tx_DoorSensor01_down, countof(Tx_DoorSensor01_down));
        }
        countDoor01Down++;
      }else if(GPIO_ReadInputPin(GPIOB,KEY_DOOR01) == Key_Down && countDoor01Down > 0){
        Uart1SendData(Tx_DoorSensor01_up, countof(Tx_DoorSensor01_up));
        countDoor01Down = 0;
      }
      
      if(GPIO_ReadInputPin(GPIOB,KEY_DOOR02) == Key_Up){
        if(countDoor02Down == 0){
          Uart1SendData(Tx_DoorSensor02_down, countof(Tx_DoorSensor02_down));
        }
        countDoor02Down++;
      }else if(GPIO_ReadInputPin(GPIOB,KEY_DOOR02) == Key_Down && countDoor02Down > 0){
        Uart1SendData(Tx_DoorSensor02_up, countof(Tx_DoorSensor02_up));
        countDoor02Down = 0;
      }
      
      if(GPIO_ReadInputPin(GPIOB,KEY_MONITOR) == Key_Down){
        if(countMonitorDown == 0){
          Uart1SendData(Tx_MonitorCheck_down, countof(Tx_MonitorCheck_down));
        }
        countMonitorDown++;
      }else if(GPIO_ReadInputPin(GPIOB,KEY_MONITOR) == Key_Up && countMonitorDown > 0){
        Uart1SendData(Tx_MonitorCheck_up, countof(Tx_MonitorCheck_up));
        countMonitorDown = 0;
      }
      
      if(GPIO_ReadInputPin(GPIOB,KEY_UNBIND) == Key_Down){
        if(countUnbindDown == 0){
          Uart1SendData(Tx_Unbind_down, countof(Tx_Unbind_down));
        }
        countUnbindDown++;
      }else if(GPIO_ReadInputPin(GPIOB,KEY_UNBIND) == Key_Up && countUnbindDown > 0){
        Uart1SendData(Tx_Unbind_up, countof(Tx_Unbind_up));
        countUnbindDown = 0;
      }
      
   }
}
