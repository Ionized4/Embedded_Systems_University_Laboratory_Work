/*
 * lcd_dog_avr128_driver.h
 *
 * Created: 3/29/2022 2:00:36 PM
 *  Author: ioniz
 */ 

#ifndef F_CPU
#define F_CPU 4000000
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// overkill on macros but w/e
// allows you to reroute RS by just changing macros
// macros can be predefined before this header is included, otherwise they default to use the pins specified in this laboratory
#ifndef RS_PORT
#define RS_PORT		VPORTC
#endif
#ifndef RS_PIN_bm
#define RS_PIN_bm	PIN0_bm
#endif

char display_buffer[3][17];

void init_lcd_dog();
void update_lcd_dog();