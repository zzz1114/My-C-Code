#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);

u8 GetRandom(u8 min, u8 max);   //得到一定范围的随机数
u8 GetDirection(void);
#endif























