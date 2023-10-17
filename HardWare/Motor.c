#include "stm32f4xx.h"                  // Device header
#include "TIM1.h"
#include "TIM2.h"
#include "Motor.h"
#include "main.h"

void Motor_Init(void)
{
	TIM1_Init();
	TIM2_Init();
}

uint32_t MotorY_GetRemoteOriPWM(void)
{
	// rising edge / period
	return (TIM_GetCapture2(TIM2) + 1 );
}

uint32_t MotorX_GetRemoteOriPWM(void)
{
	// rising edge / period
	return (TIM_GetCapture4(TIM2) + 1);
}

/**
  * @brief  
  * @param  
  * @retval LPWM :the range of LPWM is 1.6~2.2ms if mainmotor go,
			and 0.8ms~1.4ms if mainmotor back.
			the range of high level of remote is 1ms~2ms.
  */
uint32_t MotorLeft_GetRemotePWM(void)
{
	int32_t LPWM = MotorY_GetRemoteOriPWM();
	int32_t PWM_Down = 0;
	
	/* input data error */
	if (MotorY_GetRemoteOriPWM() < (REMOTE_PWM_MIN - 50) || MotorY_GetRemoteOriPWM() > (REMOTE_PWM_MAX + 50))
		return 0;
//	
	/* if boat turn right */
	if ((PWM_Down = REMOTE_PWM_MID  - MotorX_GetRemoteOriPWM()) < 0)
		PWM_Down = 0;
	
	if ((int32_t)(MotorY_GetRemoteOriPWM() - REMOTE_PWM_MID) >= 0)	/* if boat advance */
	{
		LPWM -= PWM_Down / 2;
	}
	else if ((int32_t)(MotorY_GetRemoteOriPWM() - REMOTE_PWM_MID) < 0)		/* boat backward */
	{
		LPWM += PWM_Down / 2;
	}
	else
		LPWM = 0;

	return LPWM;
}

/**
  * @brief  
  * @param  
  * @retval RPWM :the range of LPWM is 1.6~2.2ms if mainmotor go,
			and 0.8ms~1.4ms if mainmotor back.
			the range of high level of remote is 1ms~2ms.
  */
uint32_t MotorRight_GetRemotePWM(void)
{
	int32_t RPWM = MotorY_GetRemoteOriPWM();
	int32_t PWM_Down = 0;
	
	/* input data error */
	if (MotorY_GetRemoteOriPWM() < (REMOTE_PWM_MIN - 50) || MotorY_GetRemoteOriPWM() > (REMOTE_PWM_MAX + 50))
		return 0;
	
	/* if boat turn left */
	if ((PWM_Down = MotorX_GetRemoteOriPWM() - REMOTE_PWM_MID) < 0)	
		PWM_Down = 0;

	if ((int32_t)(MotorY_GetRemoteOriPWM() - REMOTE_PWM_MID) >= 0)	/* if boat advance */
	{
		RPWM -= PWM_Down / 2;
		
	}
	else if ((int32_t)(MotorY_GetRemoteOriPWM() - REMOTE_PWM_MID) < 0)		/* boat backward */
	{
		RPWM += PWM_Down / 2;
	}
	else
		RPWM = 0;

	return RPWM;
}

void MotorLeft_SetPWM(int32_t PWM)
{
	/* input data error */
	if (PWM < 750 || PWM > 2250)
		PWM = 0;

	TIM_SetCompare3(TIM1, PWM);
}

void MotorRight_SetPWM(int32_t PWM)
{
	/* input data error */
	if (PWM < 750 || PWM > 2250)
		PWM = 0;

	TIM_SetCompare4(TIM1, PWM);
}

void Motor_Start(void)
{
	TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
	TIM_CCxCmd(TIM1, TIM_Channel_4, TIM_CCx_Enable);
}

void Motor_Stop(void)
{
	MotorLeft_SetPWM(0);
	MotorRight_SetPWM(0);
	TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);
	TIM_CCxCmd(TIM1, TIM_Channel_4, TIM_CCx_Disable);
}
