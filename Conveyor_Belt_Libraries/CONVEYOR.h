#ifndef CONVEYOR_H_
#define CONVEYOR_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>
#include "Encoder.h"
#include "TimerCounter.h"
#include "USART.h"
#include "PORT.h"
#include "Transform.h"

#define F_CPU 160000000UL
#define RIGHT 0
#define SAMPLINGTIME 0.01f
#define RADPERPULSE 1.59e-3                  // encoder의 1개의 pulse 당 회전각 (단위 : rad/pulse)
#define TOP 750
#define MAX 23.0f

#endif /* CONVEYOR_H_ */