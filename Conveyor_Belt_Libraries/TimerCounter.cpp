#include "TimerCounter.h"

volatile char cnt;
volatile float w[1];              // 각속도
volatile float w_goal[1];
extern volatile int encoder[1];
extern char StatusPacket[20];
char temp1[10];

PID controllerParameter[1] = {{.Kp=1.0, .Ki=40.0}};         // 모터의 제어기 parameter 변수


/* OCR1A, OCR1B 값을 0으로 설정하면 duty 비 0%, OCR1A, OCR1B 값을 TOP으로 설정하면 duty 비 100%, OCR1A, OCR1B의 초기값은 0 */
ISR (TIMER2_OVF_vect)       // 1ms마다 인터럽트 발생
{
	cnt++;                 
	
	if (cnt == 10)          // 제어 주기 = 0.01초
	{
		w[RIGHT] = angularVelocity(encoder[RIGHT]);
		OCR1A = pidControll(w[RIGHT], w_goal[RIGHT], &controllerParameter[RIGHT]);
	
		resetCount();       // Encoder 초기화
		cnt = 0;
	}
	
	TCNT2 = 0x06;           // Timer0 초기화
}


void timer1Init (void)
{
	TCCR1A = (1<<COM1A1) | (0<<COM1A0) | (1<<COM1B1) | (0<<COM1B0) | (1<<WGM11) | (0<<WGM10);         // PC PWM mode, TOP=ICR1
	TCCR1B = (1<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);                             // 분주비 1
    ICR1 = TOP;
}

void timer2Init (void)
{
	TCCR2 = (0<<WGM21) | (0<<WGM20) | (0<<CS22)| (1<<CS21)| (1<<CS20);      // Normal mode, 분주비 64 설정      ==> 16000000/64 = 250000  ==> 4us 마다 TCNT0 1증가
	TIMSK |= (1<<TOIE2);                                                    // Timer2 OVF interrupt 허용
	TCNT2 = 0x06;
}

static float angularVelocity (int EncoderCount)                 
{
	float v = 0.0f;
	v = (float)(EncoderCount*RADPERPULSE)/(SAMPLINGTIME);
	
	return v;
}

static int pidControll (float w_present, float w_goal, PID *motor)
{
	float err = w_goal-w_present;
	float propotional = motor->Kp * err;
	
	motor->integrator += motor->Ki * ((0.5 * (err + motor->preError)) - motor->antiWindup) * SAMPLINGTIME;       // 적분항, 사다리꼴 수치 적분 사용
	motor->preError = err;
	
	motor->output = propotional + motor->integrator;
	
	//dtostrf(motor->output, 4, 2, temp1);
	//sprintf(StatusPacket, "%s\n", temp1);
	//sprintf(StatusPacket, "Enc : %d\n", encoder[RIGHT]);
	//Tx_MCUtoPC(StatusPacket);
	//reset_arr(StatusPacket, strlen(StatusPacket));
	
	/* Anti-windup */
	if (motor->output > MAX)
	{
		motor->antiWindup = motor->output - MAX;
		motor->output = MAX;
	}
	else if (motor->output < -MAX)
	{
		motor->antiWindup = motor->output + MAX;
		motor->output = -MAX;
	}
	else
	{
		motor->antiWindup = 0;
	}
	
	/* motor의 순간적인 방향 전환 방지 */
    if ((motor->output*w_goal) < 0)               
	{
		if (w_goal > 0)
		    motor->output=1;
		else
		    motor->output=-1;
	}
	
	/* 제어기 출력에 따른 모터 방향 전환 */    
	             
	if (motor->output < 0)                                  // 제어기 출력이 음의 값이라면 방향 전환 후 출력의 양의 값 입력
	{
		if (motor == &controllerParameter[RIGHT])
		    rightMotorBackward();
		
		motor->output = -motor->output;
	}
	else
	{
		if(motor == &controllerParameter[RIGHT])
		    rightMotorForward();
	}

	if (w_goal==0 && w_present==0)
	    motor->integrator = 0;
		
		
    //dtostrf(motor->output, 4, 2, temp1);
    //sprintf(StatusPacket, "%s\n", temp1);
    //Tx_MCUtoPC(StatusPacket);
    //reset_arr(StatusPacket, strlen(StatusPacket));
	
	return (motor->output/24.0f)*TOP;    // 제어기 출력 반환
}