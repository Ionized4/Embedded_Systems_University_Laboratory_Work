/*
 * USART3.c
 *
 * Created: 4/27/2022 3:56:57 PM
 *  Author: ioniz
 */ 

#include "USART3.h"
#include <string.h>

/*
 * configure USART 3 for asynchronous 8N1 serial communication
 * initializes USART3_BAUD based on the BAUD macro for normal speed
 * enables TX
 */
void USART3_asynch_init(void)
{
	VPORTB_DIR = PIN0_bm;
	USART3_CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc | USART_CHSIZE_8BIT_gc;
	USART3_BAUD = BAUD_RATE(9600);
	USART3_CTRLB = USART_TXEN_bm | USART_RXMODE_NORMAL_gc;
}

void USART3_send_char(char c)
{
	USART3_TXDATAL = c;
	while (!(USART3_STATUS & USART_TXCIF_bm));
}

void USART3_send_str(char* str)
{
	int n = strlen(str);
	for (int i = 0; i < n; i++) {
		USART3_send_char(str[i]);
	}
	USART3_send_char(0x13);	// send carriage return
	USART3_send_char(0x10);	// send line feed
}