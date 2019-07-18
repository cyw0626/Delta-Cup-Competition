#include "servor.h"
#include "usart.h"
#include "delay.h"
#include "common.h"

u8 count1;    //每个变量用做8路舵机先后赋值控制
#define MAXPWM 2505   //舵机最大PWM控制2.5ms宏定义
extern uint16 CPWM[9];
extern uint8 flag_vpwm;
void Servor_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable , ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB,ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/************************GPIO电平反转函数*****************************************/ 
void Flip_GPIO_One(void)
{
  switch(count1)    //将20ms的舵机控制周期分成8份，每2.5ms控制一个舵机运转
  {  								//每个定时器控制8路舵机运转，3个定时器控制24路舵机运转     
    case 1: 
      TIM2->ARR=CPWM[1];        //将第一个舵机脉冲宽度值赋值给定时器2
      GPIO_SetBits(GPIOB,GPIO_Pin_15);      //同时拉高控制舵机1的引脚的电平
      flag_vpwm=1;	
      break;
    case 2:
      TIM2->ARR=MAXPWM-CPWM[1]; 		 //将2.5ms减去PWM脉宽值得数据赋值定时器2
      GPIO_ResetBits(GPIOB,GPIO_Pin_15);    //同时拉低控制舵机1引脚的电平
      flag_vpwm=1;	
      break;          //控制舵机1的引脚在剩下20ms-CPM[0]时间内将一直保持低电平，舵机1按照CPWM值转转动
    case 3: 
      TIM2->ARR=CPWM[2];        
      GPIO_SetBits(GPIOA,GPIO_Pin_8);      
      flag_vpwm=1;	
      break;
    case 4:
      TIM2->ARR=MAXPWM-CPWM[2]; 		
      GPIO_ResetBits(GPIOA,GPIO_Pin_8);    
      flag_vpwm=1;	
      break;  
    ...
    default:break;
  }
}
/************************舵机控制函数*********************************************/
void Servo1(void)
{
  count1++; 
  Flip_GPIO_One();    //反转IO电平
}
