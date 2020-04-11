#ifndef _LED_H
#define _LED_H

#include "sys.h"
              
#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5

#define LED_ON 0
#define LED_OFF 1

void LED_Init(void);

#endif
