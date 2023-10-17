/*
	ԭ��TIM2_CH3���������ؼ���������ڡ�������ÿ�μ�����CCR�Ĵ��������������CNT��
		TIM2_CH4�����½��ؼ�������ڵ���һ����������0��������
		���Բ���ֵΪ�ߵ�ƽʱ�䡣
*/
#include "stm32f4xx.h" 
#include "TIM2.h"

void TIM2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2);  //GPIOA0����ΪTIM2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2);  //GPIOA0����ΪTIM2
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		// TIM2_CH1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		// TIM2_CH3	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 65536 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_PWMIConfig(TIM2, &TIM_ICInitStructure);
	// ��ģʽ
  	TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
	
	// ͨ��3
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
 	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	// ͨ��4����������
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
 	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	// �����ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);

	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM2,TIM_FLAG_CC3) != RESET)
	{
		TIM_SetCounter(TIM2, 0);
		TIM_ClearFlag(TIM2, TIM_FLAG_CC3);
	}
}
