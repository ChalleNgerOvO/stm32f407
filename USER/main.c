#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "touch.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#include "test_task.h"
#include "task.h"
#include "FreeRTOS.h"
#include "stdio.h"
#include "tim_Delay.h"
void vApplicationTickHook(void) {
    lv_tick_inc(1);
}

int main(void)
{
		tim6_init();
    delay_init(168);
    uart_init(115200);
    TP_Init();
	  tp_dev.init();
    led_init();

    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    create_start_task();
    vTaskStartScheduler();
}
