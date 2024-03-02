#include "USART.h"

char StatusPacket[20];              // Dynamixel이 송신하는 데이터를 수신하기 위한 배열
char InstrPacket[20];               // PC가 송신하는 데이터를 수신하기 위한 배열
volatile int i,j,k;                                   // 배열 초기화를 위한 변수
int len;                                     // 배열의 길이

void UART0_init (void)           // ATmeag128과 PC는 USART0 사용
{
	UCSR0B = 0b10011000;          // 수신완료 인터럽트 개별허용, RxDn, TxDn 핀을 직렬 데이터 송,수신 단자로 동작하도록 설정
	UCSR0C = 0b00000110;          // 비동기 모드, 패티티 비트 사용X, STOP 비트 1개, 전송 데이터 길이 8비트 설정
	UBRR0H = 0;
	UBRR0L = 51;                  // BAUD rate : 19200bps 설정
}

ISR (USART0_RX_vect)                             // PC가 송신하는 DATA를 수신하기 위한 인터럽트
{
	len = strlen(InstrPacket);                   // 배열의 길이
	
	if (j == 0)                                  // 문자열을 수신하기전에 문자열을 저장할 배열 초기화
	{
		for (i=0; i<len; i++)
		{
			InstrPacket[i] = '\0';
		}
	}
	InstrPacket[j++] = UDR0;                     // 수신 배열에 데이터 저장
	delay(1);
}

void Tx_MCUtoPC (char* Instrpacket)        // ATmega128이 DATA(Status Packet)를 PC로 송신 (문자열)
{
	//strcat (InstrPacket,"\n");
	int len = strlen(Instrpacket);                  // 송신할 문자열 길이
	int i=0;
	
	for (i = 0; i < len; i++)                      // 문자열 길이 만큼 반복
	{
		if (Instrpacket[i] == '\0')                // 문자열의 끝까지 오면 종료
		break;
		while (!(UCSR0A & 0x20));                  // UDRE0 = 1이면 송신 데이터를 받을 준비가 되고 while문을 빠져나감
		UDR0 = Instrpacket[i];                     // 문자 송신
		delay(1);
	}
}

void reset_Rx0Buff (void)
{
	j = 0;
}