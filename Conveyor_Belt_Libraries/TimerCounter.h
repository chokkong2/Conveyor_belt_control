#ifndef TimerCounter_H_
#define TimerCounter_H_

#include "CONVEYOR.h"

typedef struct  
{
	float Kp;
	float Ki;
	float preError;
	float integrator;
	float antiWindup;
	
	float output;
}PID;

static int pidControll (float w_present, float w_goal, PID *motor);
static float angularVelocity (int EncoderCount);
void timer2Init (void);
void timer1Init (void);

#endif /* TimerCounter_H_ */