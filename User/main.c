/*
Program: 无人艇下位机
History:
	2023/10/9	无人艇下位机v1	Shane	1th release
*/
#include "stm32f4xx.h" 
#include "LED.h" 
#include "Delay.h" 
#include "USART1.h"
#include "SWIC.h"
#include "Servo.h"
#include "Motor.h"
#include "Pushrod.h"
#include "main.h"

void test(void)
{
//	printf("OServo: %u, Servo: %u, OMotorY:%u,X: %u,  MotorL:%u, OP: %u, P:%u, C:%u, P:%u\r\n",\
//		Servo_GetRemoteOriPWM(), Servo_GetRemotePWM(), MotorY_GetRemoteOriPWM(), MotorX_GetRemoteOriPWM(), MotorLeft_GetRemotePWM(),\
//		Pushrod_GetRemoteOriPWM(), Pushrod_GetRemotePWM(), Conveyor_GetPWM(), SWIC4_GetRemoteOriPWM());
	printf("%d, %u, %u\r\n", SWIC_GetRemoteState(SW_REMOTE_EN), MotorLeft_GetRemotePWM(), MotorRight_GetRemotePWM());
}

//void Controller_(void)
//{
//	if (Controller.AutoModeEN != RESET)
//	{
//		Controller.RemoteModeEN = RESET;
//	}
//}

void test2(void)
{
//	MotorLeft_SetPWM(MotorLeft_GetRemotePWM());
//	MotorRight_SetPWM(7000);
}

int main(void)
{
	// define Struct
	MotorDataInit LMainMotor, RMainMotor;
	ServoDataInit LServo, RServo;
	PushrodDataInit LPushrod, RPushrod;
	PumpDataInit Pump;
	ConveyorDataInit Conveyor;
	ControllerInit Controller;
	
	// test
	LED_Init();
	Motor_Init();
	Servo_Init();
	Pushrod_Init();
	SWIC_Init();
	USART1_Config();
	while(1)
	{
		test2();
		test();
		Controller.RemoteModeEN = SWIC_GetRemoteState(SW_REMOTE_EN);
		Controller.AutoModeEN = SWIC_GetRemoteState(SW_AUTO);
		//remote mode
		if (UP == Controller.RemoteModeEN)
		{
			/* start work */
			Motor_Start();
			Servo_Start();
			Pushrod_Start();
			Pump_Start();
			Conveyor_Start();
			
			LMainMotor.PWM = MotorLeft_GetRemotePWM();
			RMainMotor.PWM = MotorRight_GetRemotePWM();
			LServo.PWM = Servo_GetRemotePWM();
			RServo.PWM = Servo_GetRemotePWM();
			LPushrod.PWM = Pushrod_GetRemotePWM();
			RPushrod.PWM = Pushrod_GetRemotePWM();
			Pump.PWM = 0;
			Conveyor.PWM = Conveyor_GetPWM();
		}
		/* auto mode ********************************************************************/
		else if (Controller.AutoModeEN != RESET && Controller.RemoteModeEN != RESET)
		{
			;
		}
		else	// reset
		{
			LMainMotor.PWM = 0;
			RMainMotor.PWM = 0;
			LServo.PWM = 0;
			RServo.PWM = 0;
			LPushrod.PWM = 0;
			RPushrod.PWM = 0;
			Pump.PWM = 0;
			Conveyor.PWM = 0;
			
			Motor_Stop();
			Servo_Reset();
			Pushrod_Stop();
			Pump_Stop();
			Conveyor_Stop();
		}
	// set pwm
//	MotorLeft_SetPWM(LMainMotor.PWM);
	MotorRight_SetPWM(RMainMotor.PWM);
	ServoLeft_SetPWM(LServo.PWM);
	ServoRight_SetPWM(3000 - RServo.PWM);
	PushrodLeft_SetPWM(LPushrod.PWM);
	PushrodRight_SetPWM(RPushrod.PWM);
	Pump_SetPWM(Pump.PWM);			// 待测试
	Conveyor_SetPWM(Conveyor.PWM);
	}
}
