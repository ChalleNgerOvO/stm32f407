#ifndef _TEST_TASK_H
#define _TEST_TASK_H

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
void create_start_task(void);
void create_lvgl_dome_task(void);
#endif