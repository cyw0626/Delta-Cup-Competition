#include "sys.h"
#include "usart.h"
#include "common.h"

extern unsigned char flag_uart_rev;
extern unsigned char flag_uart_rev;
extern char uart1_buf[50];
extern char uart2_buf[50];

/**************************************************************************
支持printf函数，不需要选择usb MicroLIB
**************************************************************************/
#if 1
#pragma import(__use_no_semihosting)
struct __FILE
{
  int handle;
};
FILE __stdout;
_sys_exit(int x)
{
  x = x;
}
int fputc(int ch, FILE *f)
{
  while((USART1->SR&0X40)==0);
    USART1->DR = (u8) ch;
  return ch;
}
#endif 
/**************************************************************************
函数功能：串口1、2中断向量管理
入口参数：无
返回值：无
**************************************************************************/
void Uart1_NVIC_Init(void)    //Usart1 NVIC配置
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;   //抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //子通道0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	    //IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);     //根据指定的参数初始化NVIC寄存器
}
void Uart3_NVIC_Init(void)    //Usart2 NVIC配置
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;   //抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;    //子通道2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	    //IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);     //根据指定的参数初始化NVIC寄存器
}
/**************************************************************************
函数功能：串口1、2 IO相关初始化
入口参数：无
返回值：无
**************************************************************************/
void Uart1_Gpio_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
}
void Uart3_Gpio_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
}
/**************************************************************************
函数功能：初始化选择对应串口
入口参数：uart_num 串口号选择范围1-2
返回值：无
**************************************************************************/
void Uart_Init(u16 uart_num)
{
  if(uart_num==1)
  {
    Uart1_NVIC_Init();   //串口1中断向量管理初始化
    Uart1_Gpio_Config();    //串口1对应IO初始化
  }
  else if(uart_num==3)
  {
    Uart3_NVIC_Init();
    Uart3_Gpio_Config();
  }
}
/**************************************************************************
函数功能：串口初始化相关设置，波特率设置
入口参数：USART_TypeDef* TUSARTx:可选择对应串口 bound:波特率选择
返回值：无
**************************************************************************/
void USART_Config(USART_TypeDef* TUSARTx,u32 bound)
{
  USART_InitTypeDef USART_InitStructure;    //USART初始化设置
  
  USART_InitStructure.USART_BaudRate = bound;   //串口波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //字长为8位数据格式
  USART_InitStructure.USART_StopBits = USART_StopBits_1;    //一个停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;     //无奇偶校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;     //无硬件数据流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;       //收发模式
  
  USART_Init(TUSARTx, &USART_InitStructure);      //初始化串口
  USART_ITConfig(TUSARTx, USART_IT_RXNE, ENABLE);       //开启串口接受中断
  USART_Cmd(TUSARTx, ENABLE);       //使能串口
}
void USART3_Config(u32 bound)
{
  USART_InitTypeDef USART_InitStructure;
  
  USART_InitStructure.USART_BaudRate = bound;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStructure);
  
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
  USART_HalfDuplexCmd(USART3,ENABLE);
  USART_Cmd(USART3, ENABLE); 
}
/**************************************************************************
函数功能：串口发送一个字符、字符串
入口参数：USART_TypeDef* TUSARTx:可选择对应串口 data:要发送的字符 *str:要发送的字符串
返回值：无
**************************************************************************/
void UART_PutChar(USART_TypeDef* USARTx, uint8_t Data)
{
  USART_SendData(USARTx, Data); 
  while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET){}
}
void UART_PutStr (USART_TypeDef* USARTx, char *str)
{
  unsigned int i = 0;
  while(*str != '\0' && i < 500)
  {
    UART_PutChar(USARTx, *str++);
    i++;
  }
}
/**************************************************************************
函数功能：将一个无符号整型数转换成对应字符串，在通过串口1发送出去
入口参数：uint16 data
返回值：无
**************************************************************************/
void UART_Put_Num(uint16  dat)
{
  char temp[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  u32tostr(dat,temp);
  UART_PutStr(USART1,temp); 
}
void UART_Put_Inf(char *inf,uint16 dat)
{
  UART_PutStr (USART1,inf); 
  UART_Put_Num(dat);
  UART_PutStr (USART1,"\n");  
}
/**************************************************************************
函数功能：串口1中断入口函数
入口参数：无
返回值：无
**************************************************************************/
void USART1_IRQHandler(void)
{
  static unsigned int i=0;
  u8 res;
  if(USART_GetITStatus(USART,USART_IT_RXNE)!= RESET)
  {
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    Res =USART_ReceiveData(USART1);	
    if(!flag_uart1_rev)
    {
      uart1_buf[i]=Res;
      i++;
      if(uart1_buf[i-1]=='\n'||i>255)
      {
        uart1_buf[i] = 0;
        flag_uart1_rev=1;
        i=0;
      }
    }
  }
}
void USART3_IRQHandler(void)
{
  static char num=0;
  unsigned char ch=0;
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  
  { 
    USART_ClearITPendingBit(USART3,USART_IT_RXNE);
    ch=USART_ReceiveData(USART3);	
    if(!flag_uart2_rev)
    {
      if(ch=='#')
      {
        num=0;
        uart2_buf[num]=ch;
      }
      else if(uart2_buf[num-1]=='\r')
      {
        uart2_buf[num]=0;
        flag_uart2_rev=1;
        num=0;
			}
      uart2_buf[num]=ch;
      num++;
    }
	}
}
