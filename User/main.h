#ifndef __MAIN_H
#define __MAIN_H

#define REMOTE_PWM_MIN		900
#define REMOTE_PWM_MID		1500
#define REMOTE_PWM_MAX		2100


typedef struct{
	uint8_t RemoteModeEN;
	uint8_t AutoModeEN;
} ControllerInit;

#endif
