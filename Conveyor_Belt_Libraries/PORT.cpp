#include "PORT.h"

void setPort ()
{
	DDRD = 0x00;                         // 외부 인터럽트 PIN 입력 설정
	DDRA |= (1<<DDA1);                   // motor direction 설정
	DDRB |= (1<<DDB5);                   // motor PWM PIN 출력 설정
}

void rightMotorForward (void)
{
	PORTA &= ~(1<<PORTA1);
}

void rightMotorBackward (void)
{
	PORTA |= (1<<PORTA1);
}