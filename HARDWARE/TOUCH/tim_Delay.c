#include "tim_Delay.h"

/**
 * 简述：TIM6 初始化
 * 参数：psc    预分频值
 *       period 周期
 * 返回值：无
 */
void tim6_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseInitStructure.TIM_Prescaler         = 84-1; 
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);
	TIM_Cmd(TIM6, DISABLE);
}

/**
 * 简述：TIM6 按周期延时微妙
 * 参数：period 周期（微妙）
 * 返回值：无
 */
void tim6_delay_us(uint16_t period)
{
	TIM_Cmd(TIM6, DISABLE);   			// 停止定时器
	TIM_SetAutoreload(TIM6, period);  	// 设置周期
	TIM_Cmd(TIM6, ENABLE);    			// 打开定时器
	while (RESET == TIM_GetFlagStatus(TIM6, TIM_FLAG_Update));  // 等待到期（更新/溢出）
	TIM_Cmd(TIM6, DISABLE);   			// 停止定时器
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);  // 清空更新标志
}

void tim6_delay_ms(uint32_t ms)
{
	while(ms--)
	{
		tim6_delay_us(1000);
	}
}