#include "test_task.h"

#define START_TASK_PRIO 1         // 任务优先级
#define START_STK_SIZE  (1024*6)       // 任务栈深
TaskHandle_t StartTask_Handler;   // 任务句柄
void start_task(void *arg);       // 任务函数

#define LV_DEMO_TASK_PRIO 3
#define LV_DEMO_STK_SIZE 1024
TaskHandle_t LV_DEMOTASK_Handler;


void start_task(void *arg)
{
	taskENTER_CRITICAL();
	create_lvgl_dome_task();
	vTaskDelete(StartTask_Handler); 
	taskEXIT_CRITICAL();   
	
}

void lv_demo_task(void *arg)
{
	lv_obj_t* switch_obj = lv_switch_create(lv_scr_act());
	lv_obj_set_size(switch_obj, 220, 120);
	lv_obj_align(switch_obj, LV_ALIGN_CENTER, 0, 0);
	while (1) {
		lv_timer_handler();
		vTaskDelay(10);
	}
}

void create_lvgl_dome_task(void)
{
	xTaskCreate(lv_demo_task, "lv_demo task", LV_DEMO_STK_SIZE, NULL, LV_DEMO_TASK_PRIO, &LV_DEMOTASK_Handler);
}

void create_start_task(void)
{
	xTaskCreate(start_task, "start task", START_STK_SIZE, NULL, START_TASK_PRIO, &StartTask_Handler);
}
