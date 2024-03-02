#ifndef USART_H_
#define USART_H_

#include "CONVEYOR.h"

void UART0_init (void);
void Tx_MCUtoPC (char *InstrPacket);
void reset_Rx0Buff (void);

#endif /* USART_H_ */