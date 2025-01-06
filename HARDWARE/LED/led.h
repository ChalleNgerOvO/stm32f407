#ifndef _LED_H
#define _LED_H

#include "stm32f4xx.h"

#define LED0_ON()  do { GPIO_ResetBits(GPIOF, GPIO_Pin_9); } while (0)
#define LED0_OFF() do { GPIO_SetBits(GPIOF, GPIO_Pin_9); } while (0)

#define LED1_ON()  do { GPIO_ResetBits(GPIOF, GPIO_Pin_10); } while (0)
#define LED1_OFF() do { GPIO_SetBits(GPIOF, GPIO_Pin_10); } while (0)

void led_init(void);

#endif
