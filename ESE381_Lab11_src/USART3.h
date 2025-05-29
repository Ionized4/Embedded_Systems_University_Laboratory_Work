/*
 * USART3.h
 *
 * Created: 4/27/2022 3:57:41 PM
 *  Author: ioniz
 */ 

#include <avr/io.h>
#ifndef F_CPU
#define F_CPU 4000000
#endif
#define BAUD_RATE(RATE) ((64.0 * F_CPU) / (16.0 * RATE))

void USART3_asynch_init(void);
void USART3_send_str(char *str);