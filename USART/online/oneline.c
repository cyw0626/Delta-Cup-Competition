#include "oneline.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"

extern uint16 UartRec[9]; //上位机字符串解析放在这个数组里
extern uint16 CPWM[9];
extern char uart1_buf[255];
uint8 pwm_num;    //PWM序号
/***************************************************************************************************************
函数名：RecStr_to_pwm()
功能描述：把解析之后的上位机指令-滚动条实时角度变化值送入缓存
输入参数：无，由全局数组变量UartRec[17]传递
备注：因为实时性好，不要求插补，直接放入pwm[]
****************************************************************************************************************/	
void RecStr_to_pwm(void)
{
  CPWM[pwm_num]=UartRec[pwm_num];
}
/***************************************************************************************************************
函数名：
功能描述：把上位机字符串处理
备注：通过把数据处理从串口中断中独立出来，减小串口中断时间，消除定时器中断的影响
****************************************************************************************************************/
void DealRec(void)
{
  Oneline(uart1_buf);
}
/***************************************************************************************************************
函数名：Parse_String_Cmd()
功能描述：解析串口接收到的命令字符串
输入参数：传递接收到串口数据
备注：
****************************************************************************************************************/
int Parse_String_Cmd(char *str)
{
  char *p=NULL;
  p=strchr(str,'#');    //在str中搜索#的位置
  if(p==NULL)
  {
    return 1;
  }
  p++;
  if ((*(p) >= '0' && *(p) <= '9' && *(p+1) == 'P')|| 
      ((*(p)>='0' && *(p) <= '9')&& (*(p+1) == '0'&&*(p+1) <= '9') &&*(p+2) == 'P'))      //如果当前数字p以#开始，说明可执行直接返回
  {
    return 0；
  }
  return 0；
}
/***************************************************************************************************************
函数名：OneLine()
功能描述：把串口接收到的上位机编辑运行数据字符串解析出来，放到UartRec[33]
输入参数：*str.串口接受字符的存放地址
返回值：无
备注：原来单个舵机使用OneMotor()处理的。现在合并为一个函数处理，当T<100时直接放入pwm[],不进行插补
****************************************************************************************************************/
void OneLine(char *str)
{
  uint8 motor_num=0;      //舵机号
  uint16 motor_jidu=0;    //舵机脉宽值
  uint16 motor_time=0;    //执行时间
  uint8 num_now=0;      //编号解析中间变量
  uint8 jidu_now=0;     //脉宽解析中间变量
  uint8 time_now=0;     //执行时间解析中间变量
  uint8 flag_num=0;     //标记出现过#
  uint8 flag_jidu=0;    //标记出现过P
  uint8 flag_time=0;    //标记出现过T
  uint16 i=0;
  if (Parse_String_Cmd(str))      //是否出现过#，空的直接返回
    return ;
  UART_PutStr(USART3,str);      //串口2发送出去这个命令
  while( str[i]!='\n' && i < 255)
  {
    if(flag_num==1)     //出现过#
    {
      if(str[i]!='P')     //如果当前字符不是P
      {
        num_now=ASC_To_Valu(str[i]);      //把当前字符转化成数字的值
        motor_num=motor_num*10+num_now;
      }
      else        //当前字符是P
        flag_num=0;
    }
    if(flag_jidu==1)        //出现过P
    {
      if((str[i]!='T')&(str[i]!='#'))       //当前字符是出现p之后的非#非T的字符
      {
        jidu_now=ASC_To_Valu(str[i]);         //把当前数字字符转化成数字的值
        motor_jidu=motor_jidu*10+jidu_now;
      }
      else          //当前字符是#或者T，角度结束
      {
        flag_jidu=0;
        if(motor_jidu>2500)
        motor_jidu=2500;
        UartRec[motor_num]=motor_jidu;
        pwm_num=motor_num;
        motor_jidu=0;
        motor_num=0;
      }
    }
    if(flag_time==1)        //出现了T
    {
      time_now=ASC_To_Valu(str[i]);       //把当前数字字符转化成数字的值
      motor_time=motor_time*10+time_now;
      UartRec[0]=motor_time;      //执行时间放在[0]位置
      if(str[i+1]=='\r')
      {
        RrcStr_to_pwm();        //把指令的有效值送到相应位置
      }
    }
    if(str[i]=='#')
      flag_num=1;
    if(str[i]=='P')
      flag_jidu=1;
    if(str[i]=='T')
      flag_time=1;
    i++;
  }
}
