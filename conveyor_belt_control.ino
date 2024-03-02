#include "CONVEYOR.h"

extern volatile int encoder[1];
extern volatile float w[1];
extern volatile float w_goal[1];
extern char InstrPacket[20];
extern char StatusPacket[20];

String inst;
char instr[30];
void printAngularVelocity();

void setup ()
{
  //Serial.begin(19200);
  UART0_init();
  setInterrupt();
  setPort();
  timer1Init();
  timer2Init();
  resetCount();
  reset_arr(StatusPacket, strlen(StatusPacket));
  reset_arr(InstrPacket, strlen(InstrPacket));
  reset_arr(instr, strlen(instr));
  sei();
}

void loop ()
{
  if (InstrPacket[0] != '\0')
  {
      delay(100);
      if (readInt(InstrPacket))
      {
          sprintf(instr, "w_goal : %d\n", (int)w_goal[RIGHT]);
          Tx_MCUtoPC(instr);
          delay(100);
          reset_arr(instr, strlen(instr));
          reset_arr(InstrPacket, strlen(InstrPacket));
          reset_Rx0Buff();
          while(InstrPacket[0] == '\0')
          {
            printAngularVelocity();
            delay(10);
          }
      }
      else
      {
          sprintf(instr, "fail\n");
          Tx_MCUtoPC(instr);
          reset_arr(instr, strlen(instr));
          reset_arr(InstrPacket, strlen(InstrPacket));
          reset_Rx0Buff();
      }
  }
}


void printAngularVelocity ()
{
  char temp1[10], temp2[10];
  dtostrf(w[RIGHT], 4, 2, temp1);
  dtostrf(w_goal[RIGHT], 4, 2, temp2);
  sprintf(instr, "w : %s, w_goal : %s\n", temp1, temp2);
  Tx_MCUtoPC(instr);
  reset_arr(instr, strlen(instr));
}
