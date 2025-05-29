/*
 * lcd_dog_avr128_driver.c
 *
 * Created: 3/29/2022 1:59:11 PM
 *  Author: ioniz
 */ 

#include "lcd_dog_avr128_driver.h"

static inline void set_RS()
{
	RS_PORT.OUT |= RS_PIN_bm;	// sets RS pin specified by RS_PORT and RS_PIN_bm macros
}

static inline void clear_RS()
{
	RS_PORT.OUT &= ~RS_PIN_bm;	// clears RS pin specified by RS_PORT and RS_PIN_bm macros
}

void lcd_spi_transmit_CMD(unsigned char cmd)
{
	//while(!(SPI0_INTFLAGS & SPI_IF_bm));	// wait for tx to be ready to transmit data
	clear_RS();								// clear RS => select command register on LCD_DOG
	VPORTF_OUT &= ~(PIN2_bm);
	SPI0_DATA = cmd;						// write command value to SPI0_DATA, which transmits the command
	while(!(SPI0_INTFLAGS & SPI_IF_bm));	// wait for tx to be ready to transmit data
	VPORTF_OUT |= PIN2_bm;
}

void lcd_spi_transmit_DATA(unsigned char cmd)
{
	//while(!(SPI0_INTFLAGS & SPI_IF_bm));	// wait for tx to be ready to transmit data
	set_RS();								// set RS => select data register on LCD_DOG
	VPORTF_OUT &= ~(PIN2_bm);
	SPI0_DATA = cmd;						// write command value to SPI0_DATA, which transmits the command
	while(!(SPI0_INTFLAGS & SPI_IF_bm));	// wait for tx to be ready to transmit data
	VPORTF_OUT |= PIN2_bm;
}

void init_spi_lcd()
{	/*
	 * PA4 - MOSI
	 * PA5 - MISO
	 * PA6 - SCK
	 * PA7 - /SS
	 */
	VPORTA_DIR |= PIN4_bm | PIN6_bm | PIN7_bm;	// set MOSI, SCK, and /SS directions to output
	VPORTF_DIR |= PIN2_bm;
	VPORTA_DIR &= ~(PIN5_bm);					// set MISO to input
	RS_PORT.DIR |= RS_PIN_bm;					// set RS pin to output
	
	/* initialize SPI0 module */
	SPI0_CTRLB = SPI_SSD_bm | SPI_MODE_3_gc;
	SPI0_CTRLA = SPI_MASTER_bm | SPI_PRESC_DIV16_gc | SPI_ENABLE_bm;
}

void init_lcd_dog()
{
	init_spi_lcd();
	
	_delay_ms(40);
	
	lcd_spi_transmit_CMD(0x39);
	_delay_us(30);
	
	lcd_spi_transmit_CMD(0x39);
	_delay_us(30);
	
	lcd_spi_transmit_CMD(0x1E);
	_delay_us(30);
	
	lcd_spi_transmit_CMD(0x55);
	_delay_us(30);
	
	lcd_spi_transmit_CMD(0x6C);
	_delay_ms(40);
	
	lcd_spi_transmit_CMD(0x7F);
	_delay_us(30);
	
	lcd_spi_transmit_CMD(0x0C);
	_delay_us(30);
	
	lcd_spi_transmit_CMD(0x01);
	_delay_us(30);
	
	lcd_spi_transmit_CMD(0x06);
	_delay_us(30);
}

// Updates the LCD display lines 1, 2, and 3, using the
// contents of dsp_buff_1, dsp_buff_2, and dsp_buff_3, respectively.
void update_lcd_dog(void) {

	init_spi_lcd();		//init SPI port for LCD.

	// send line 1 to the LCD module.
	lcd_spi_transmit_CMD(0x80);	//init DDRAM addr-ctr
	_delay_us(40);
	for (int i = 0; i < 16; i++) {
		lcd_spi_transmit_DATA(display_buffer[0][i]);
		_delay_us(30);
	}
	
	// send line 2 to the LCD module.
	lcd_spi_transmit_CMD(0x90);	//init DDRAM addr-ctr
	_delay_us(30);
	for (int i = 0; i < 16; i++) {
		lcd_spi_transmit_DATA(display_buffer[1][i]);
		_delay_us(30);
	}
	
	// send line 3 to the LCD module.
	lcd_spi_transmit_CMD(0xA0);	//init DDRAM addr-ctr
	_delay_us(30);
	for (int i = 0; i < 16; i++) {
		lcd_spi_transmit_DATA(display_buffer[2][i]);
		_delay_us(30);
	}
}