#ifndef _REMOTE_H
#define _REMOTE_H

#include "sys.h"

//遥感模块，定义x轴模数-PA5，y轴模数-PA6，微动按钮-PA4(上拉）

#define SW_PEWSS 0    //按下后为0

void AdcInit(void);
void DMAInit(void);
u16* PS2_Scan(void);

#endif
