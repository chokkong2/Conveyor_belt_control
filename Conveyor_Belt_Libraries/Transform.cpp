#include "Transform.h"

extern char StatusPacket[20];
extern volatile float w_goal[1];

int readInt (char *InstructionPacket)
{
	int i = 0;
	
	/* packet 형식 : +6, (0번째 요소=모터 부호, 1번째 요소=모터 각속도) */
	if (!(InstructionPacket[0]==43 || InstructionPacket[0]==45) || InstructionPacket[1] > 55)
	{
		reset_arr(StatusPacket, strlen(StatusPacket));
		sprintf(StatusPacket, "Fail\n");
		Tx_MCUtoPC(StatusPacket);
		return 0;
	}
	
	if (InstructionPacket[1] == 48)
	{
	    w_goal[RIGHT] = 0.0;
	}
	else if ((InstructionPacket[0] == '-') && InstructionPacket[1] != 48)
	{
	    w_goal[RIGHT] = -(float)(InstructionPacket[1]-48);
	}
	else
	{
	    w_goal[RIGHT] = (float)(InstructionPacket[1]-48);
	}
	
	return 1;
}

void reset_arr(char *instr, int len)              // 문자열을 초기화하기 위한 함수
{
	int i;
	
	for (i=0; i<len; i++)
	{
		instr[i] = '\0';
	}
}
