#include "tim_Delay.h"

/**
 * ������TIM6 ��ʼ��
 * ������psc    Ԥ��Ƶֵ
 *       period ����
 * ����ֵ����
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
 * ������TIM6 ��������ʱ΢��
 * ������period ���ڣ�΢�
 * ����ֵ����
 */
void tim6_delay_us(uint16_t period)
{
	TIM_Cmd(TIM6, DISABLE);   			// ֹͣ��ʱ��
	TIM_SetAutoreload(TIM6, period);  	// ��������
	TIM_Cmd(TIM6, ENABLE);    			// �򿪶�ʱ��
	while (RESET == TIM_GetFlagStatus(TIM6, TIM_FLAG_Update));  // �ȴ����ڣ�����/�����
	TIM_Cmd(TIM6, DISABLE);   			// ֹͣ��ʱ��
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);  // ��ո��±�־
}

void tim6_delay_ms(uint32_t ms)
{
	while(ms--)
	{
		tim6_delay_us(1000);
	}
}