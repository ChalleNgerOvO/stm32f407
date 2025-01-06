#ifndef __TIM6_H
#define __TIM6_H

#include "stm32f4xx.h"
#include "stdio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#define DELAY_MS(ms) tim6_delay_ms(ms)

void tim6_init(void);
void tim6_delay_us(uint16_t period);
void tim6_delay_ms(uint32_t ms);

#endif