
/*
<오른쪽 모터>
encoder의 채널 A ==> PD3 ==> INT3
encoder의 채널 B ==> PD1 ==> INT1
전진 : 채널 B가 상승 에지일 때, 채널 A는 HIGH. OR 채널 B가 하강 에지일 때, 채널 A는 LOW
*/

#include "Encoder.h"

volatile int encoder[1];

ISR (INT0_vect)                           // 왼쪽 모터 Encoder 채널 B 인터럽트
{
	if (EICRA & (1<<ISC00))               // 상승 에지에서 발생한 인터럽트의 경우
	{
		if (PIND & (1<<PIND2))            // 채널 A가 HIGH일 때.    ==> 채널 B가 상승 에지일 때, 채널 A가 HIGH이면 전진
		{
			encoder[RIGHT]++;
		}
		else                              // 채널 A가 LOW일 때     ==> 채널 B가 상승 에지일 때, 채널 A가 LOW이면 후진
		{
			encoder[RIGHT]--;
		}
	}
	else                                  // 하강 에지에서 발생한 인터럽트의 경우
	{
		if (PIND & (1<<PIND2))            // 채널 A가 HIGH일 때.    ==> 채널 B가 하강 에지일 때, 채널 A가 HIGH이면 후진
		{
			encoder[RIGHT]--;
		}
		else                              // 채널 A가 LOW일 때     ==> 채널 B가 하강 에지일 때, 채널 A가 LOW이면 전진
		{
			encoder[RIGHT]++;
		}
	}
	EICRA ^= (1<<ISC00);                  // 채널 B 기준으로 상승, 하강 에지에서 encoder pulse를 count해야 하므로 매번 에지의 방향을 반대로 바꿔줘야됨
}

void setInterrupt (void)
{
	EIMSK = (1<<INT0);        // INT0 허용
	EICRA = (1<<ISC21) | (1<<ISC20) | (1<<ISC01) | (1<<ISC00);      // INT0, INT2 인터럽트 트리거 방식을 상승 에지로 설정
}

void resetCount (void)
{
	encoder[RIGHT] = 0;
}