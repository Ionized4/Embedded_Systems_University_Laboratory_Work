/*
 * SCD_to_LCD.c
 *
 * Created: 4/20/2022 3:29:38 PM
 * Author : ioniz
 */ 

#include <avr/io.h>
#define F_CPU 4000000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include "SCD41.h"
#include "lcd_dog_avr128_driver.h"
#include "MCP23017.h"
#include "USART3.h"

float temp, hum;
int main(void)
{
    /* Replace with your application code */
	SCD41_I2C0_init();
	_delay_ms(3000);
	SCD41_start_periodic_measurement();
	init_lcd_dog();
	MCP23017_I2C0_init();
    while (1) 
    {
		while (!(SCD41_get_data_ready() & 0x07FF));	// wait for data to become available
		struct measurement reading = SCD41_read_measurement();	// read data
		
		temp = -45.0 + (175.0 * reading.temp) / pow(2, 16);
		hum = (100.0 * reading.hum) / pow(2, 16);
		
		sprintf(display_buffer[0], "CO2: %d", reading.co2);
		sprintf(display_buffer[1], "Temp: %d", (uint16_t)temp);
		sprintf(display_buffer[2], "Hum: %d", (uint16_t)hum);
		update_lcd_dog();
		
		// print the formatted display buffer strings to the USART module
		for (int i = 0; i < 2; i++)
			USART3_send_str(display_buffer[i]);
			
		uint16_t co2_bargraph = 0;
		
		for (int i = 0; i < 10; i++)
			co2_bargraph |= ((reading.co2 > (500 + 100*i)) << i);
			
		MCP23017_I2C_write(MCP23017_I2C_ADDR, MCP23017_16bit_registers.GPIOA, (uint8_t)co2_bargraph);
		MCP23017_I2C_write(MCP23017_I2C_ADDR, MCP23017_16bit_registers.GPIOB, (uint8_t)(co2_bargraph >> 8));
			
		_delay_ms(1000);
	}
}