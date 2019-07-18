/***************************************************************************************************************  
函数名：main()  
功能描述：主函数-通过Usart控制舵机  
****************************************************************************************************************/	
#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"    //包含STM32库系统头文件
#include "usart.h"    //包含串口通信设置头文件
#include "delay.h"    //包含系统定时器库头文件
#include "led.h"
#include "beep.h"
#include "common.h"   //包含字符转换文件
#include "servor.h"   //包含舵机头文件
#include "timer.h"    //包含中断定时器文件
#include "oneline.h"   //包含串口数据处理

uint16 CPWM[9]={1500,1500,1500,1500,1500,1500,1500,1500,1500};
uint16 UartRec[9];  //上位机字符串解析都放在这个数组里
unsigned char flag_uart1_rev=0;   //接受标志位
unsigned char flag_uart2_rev=0;
char uart1_buf[255];    //缓存
char uart2_buf[255];
unsigned char i=0;
uint8 flag_vpm=0;

int main(void)
{
  SysTick_Init();   //系统滴答定时器初始化
  LED_Init();     //LED初始化函数
  Beep_Init();    //蜂鸣器初始化函数
  Beep_Test();    //蜂鸣器测试
  Led_Test();     //LED测试
  Servor_GPIO_Config();   //舵机接口配置
  Timer_Init();   //中断初始化
  Timer_ON();   //开中断
  Uart_Init(1);   //串口1初始化
  Uart_Init(3);   //串口2初始化
  USART3_Config(115200);    //波特率设置
  USART——Config(USART1,115200);   //串口初始化相关设置
  while(1)
  {
    if(falg_uart1_rev=1)    //串口接收一条指令
    {
      DealRec();    //处理串口接收到的数据
      flag_uart1_rev=0;   //标志位清0
    }
  }
}
