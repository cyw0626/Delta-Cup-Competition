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

