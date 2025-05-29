/*
 * SCD41.c
 *
 * Created: 4/20/2022 3:33:13 PM
 *  Author: ioniz
 */ 

#include <avr/io.h>
#include "SCD41.h"

const uint8_t SCD41_I2C_ADDR = 0x62;

void SCD41_I2C0_init()
{
	// Init TWI0 Module
	TWI0_MCTRLA = TWI_ENABLE_bm; // enable TWI0 as master I2C device
	TWI0_MBAUD = 0;
	TWI0_MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

void SCD41_send_I2C_command_sequence(uint16_t cmd)
{
	while ((TWI0_MSTATUS & TWI_BUSSTATE_gm) != TWI_BUSSTATE_IDLE_gc); // wait for bus to become idle
	TWI0_MADDR = (SCD41_I2C_ADDR << 1);	// write opcode
	while (!(TWI0_MSTATUS & TWI_WIF_bm)); // wait for previous write operation to complete
	TWI0_MDATA = (uint8_t)(cmd >> 8);	// write MSB of command
	while (!(TWI0_MSTATUS & TWI_WIF_bm)); // wait for previous write operation to complete
	TWI0_MDATA = (uint8_t)cmd;	// write LSB of command
	while (!(TWI0_MSTATUS & TWI_WIF_bm)); // wait for previous write operation to complete
	TWI0_MCTRLB = TWI_MCMD_STOP_gc;	// generate stop condition
}

uint16_t SCD41_read_I2C_sequence(uint16_t cmd)
{
	uint16_t read_result = 0x0000;
	uint8_t checksum = 0x00;
	
	while ((TWI0_MSTATUS & TWI_BUSSTATE_gm) != TWI_BUSSTATE_IDLE_gc); // wait for bus to become idle
	TWI0_MADDR = (SCD41_I2C_ADDR << 1);	// write opcode
	while (!(TWI0_MSTATUS & TWI_WIF_bm)); // wait for previous write operation to complete
	TWI0_MDATA = (uint8_t)(cmd >> 8);	// write MSB of command
	while (!(TWI0_MSTATUS & TWI_WIF_bm)); // wait for previous write operation to complete
	TWI0_MDATA = (uint8_t)cmd;	// write LSB of command
	while (!(TWI0_MSTATUS & TWI_WIF_bm)); // wait for previous write operation to complete
	
	TWI0_MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_REPSTART_gc;	// configure MCTRLB to send an ack and generate a repeated start
	TWI0_MADDR = (SCD41_I2C_ADDR << 1) | 0x01;	// read opcode
	while (!(TWI0_MSTATUS & TWI_RIF_bm)); // wait for read result
	read_result = TWI0_MDATA << 8;	// read MSB of result
	TWI0_MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;	// configure TWI0 to receive another byte and send an ACK
 	while (!(TWI0_MSTATUS & TWI_RIF_bm)); // wait for read result
	read_result |= TWI0_MDATA;		// read LSB of result
	TWI0_MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;	// configure TWI0 to receive another byte and send a NACK
	while (!(TWI0_MSTATUS & TWI_RIF_bm)); // wait for read result
	checksum = TWI0_MDATA;			// read checksum
	TWI0_MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_STOP_gc; // generate stop condition
	
	return read_result;
}

struct measurement SCD41_read_measurement()
{
	struct measurement x;
	uint8_t checksum = 0x00;
	
	// WRITE OPCODE AND COMMAND
	while ((TWI0_MSTATUS & TWI_BUSSTATE_gm) != TWI_BUSSTATE_IDLE_gc); // wait for bus to become idle
	TWI0_MADDR = (SCD41_I2C_ADDR << 1);		// write opcode
	while (!(TWI0_MSTATUS & TWI_WIF_bm));	// wait for previous write operation to complete
	TWI0_MDATA = 0xEC;						// write MSB of command
	while (!(TWI0_MSTATUS & TWI_WIF_bm));	// wait for previous write operation to complete
	TWI0_MDATA = 0x05;						// write LSB of command
	while (!(TWI0_MSTATUS & TWI_WIF_bm));	// wait for previous write operation to complete
	
	// CONFIGURE TWI0 TO SEND ACK AND GENERATE A REPEATED START
	TWI0_MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_REPSTART_gc;	// configure MCTRLB to send an ack and generate a repeated start
	TWI0_MADDR = (SCD41_I2C_ADDR << 1) | 0x01;				// read opcode
	
	// READ CO2
	TWI0_MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;	// configure TWI0 to receive another byte and send an ACK
	while (!(TWI0_MSTATUS & TWI_RIF_bm));	// wait for read result
	x.co2 = TWI0_MDATA << 8;				// read MSB of result
	TWI0_MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;	// configure TWI0 to receive another byte and send an ACK
	while (!(TWI0_MSTATUS & TWI_RIF_bm));	// wait for read result
	x.co2 |= TWI0_MDATA;					// read LSB of result
	TWI0_MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;	// configure TWI0 to receive another byte and send an ACK
	while (!(TWI0_MSTATUS & TWI_RIF_bm));	// wait for read result
	checksum = TWI0_MDATA;					// read checksum
	
	// READ TEMP
	TWI0_MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;	// configure TWI0 to receive another byte and send an ACK
	while (!(TWI0_MSTATUS & TWI_RIF_bm));	// wait for read result
	x.temp = TWI0_MDATA << 8;				// read MSB of result
	TWI0_MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;	// configure TWI0 to receive another byte and send an ACK
	while (!(TWI0_MSTATUS & TWI_RIF_bm));	// wait for read result
	x.temp |= TWI0_MDATA;					// read LSB of result
	TWI0_MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;	// configure TWI0 to receive another byte and send an ACK
	while (!(TWI0_MSTATUS & TWI_RIF_bm));	// wait for read result
	checksum = TWI0_MDATA;					// read checksum
	
	// READ HUMIDITY
	TWI0_MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;	// configure TWI0 to receive another byte and send an ACK
	while (!(TWI0_MSTATUS & TWI_RIF_bm));	 // wait for read result
	x.hum = TWI0_MDATA << 8;				// read MSB of result
	TWI0_MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;	// configure TWI0 to receive another byte and send an ACK
	while (!(TWI0_MSTATUS & TWI_RIF_bm));	// wait for read result
	x.hum |= TWI0_MDATA;					// read LSB of result
	TWI0_MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;	// configure TWI0 to receive another byte and send a AACK
	while (!(TWI0_MSTATUS & TWI_RIF_bm));	// wait for read result
	checksum = TWI0_MDATA;					// read checksum
	
	TWI0_MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc; // generate stop condition
	
	return x;
}