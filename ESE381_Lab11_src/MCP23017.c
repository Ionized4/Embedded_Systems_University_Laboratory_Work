/*
 * MCP23017.c
 *
 * Created: 4/10/2022 1:33:37 PM
 *  Author: ioniz
 */ 

#include "MCP23017.h"

const struct MCP23017_register_set MCP23017_8bit_registers = {
	.IODIRA = 0x00,
	.IODIRB = 0x10,
	.IPOLA = 0x01,
	.IPOLB = 0x11,
	.GPINTENA = 0x02,
	.GPINTENB = 0x12,
	.DEFVALA = 0x03,
	.DEFVALB = 0x13,
	.INTCONA = 0x04,
	.INTCONB = 0x14,
	.IOCON = 0x05,
	.GPPUA = 0x06,
	.GPPUB = 0x16,
	.INTFA = 0x07,
	.INTFB = 0x17,
	.INTCAPA = 0x08,
	.INTCAPB = 0x18,
	.GPIOA = 0x09,
	.GPIOB = 0x19,
	.OLATA = 0x0A,
	.OLATB = 0x1A,
};

const struct MCP23017_register_set MCP23017_16bit_registers = {
	.IODIRA = 0x00,
	.IODIRB = 0x01,
	.IPOLA = 0x02,
	.IPOLB = 0x03,
	.GPINTENA = 0x04,
	.GPINTENB = 0x05,
	.DEFVALA = 0x06,
	.DEFVALB = 0x07,
	.INTCONA = 0x08,
	.INTCONB = 0x09,
	.IOCON = 0x0A,
	.GPPUA = 0x0C,
	.GPPUB = 0x0D,
	.INTFA = 0x0E,
	.INTFB = 0x0F,
	.INTCAPA = 0x10,
	.INTCAPB = 0x11,
	.GPIOA = 0x12,
	.GPIOB = 0x13,
	.OLATA = 0x14,
	.OLATB = 0x15,
};

void I2C0_MCP23017_init()
{
	// Init TWI0 Module
	TWI0_MCTRLA = TWI_ENABLE_bm;	// enable TWI0 as master I2C device
	TWI0_MBAUD = 0;
	TWI0_MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

/*
 * Initializes TWI0 module to communicate with the MCP23107
 * Writes data to control registers to configure the 8 bit ports
 */
void MCP23017_I2C0_init()
{
	// Configure the TWI module for communication with the MCP23017
	I2C0_MCP23017_init();
	
	// Configure MCP Port A
	MCP23017_I2C_write(MCP23017_I2C_ADDR, MCP23017_16bit_registers.IODIRA, 0xFF);	// Configure port A to be input
	MCP23017_I2C_write(MCP23017_I2C_ADDR, MCP23017_16bit_registers.GPPUA, 0xFF);		// Enable pull up resistors on port A
	
	// Configure MCP port B
	MCP23017_I2C_write(MCP23017_I2C_ADDR, MCP23017_16bit_registers.IODIRB, 0xFF);	// Configure port B to be output
	MCP23017_I2C_write(MCP23017_I2C_ADDR, MCP23017_16bit_registers.GPPUB, 0xFF);
}

/*
 * control - address of the I2C address
 * slave_reg_address - number representing the register of the MCP23017 to write the data to
 * data - data to write
 */
void MCP23017_I2C_write(uint8_t control, uint8_t slave_reg_address, uint8_t data)
{
	while ((TWI0_MSTATUS & TWI_BUSSTATE_gm) != TWI_BUSSTATE_IDLE_gc);	// wait for bus to become idle
	TWI0_MADDR = (control << 1);					// write opcode
	while (!(TWI0_MSTATUS & TWI_WIF_bm));			// wait for previous write operation to complete
	TWI0_MDATA = slave_reg_address;					// write slave register address as data
	while (!(TWI0_MSTATUS & TWI_WIF_bm));			// wait for previous write operation to complete
	TWI0_MDATA = data;								// write data to slave register address
	while (!(TWI0_MSTATUS & TWI_WIF_bm));			// wait for previous write operation to complete
	TWI0_MCTRLB = TWI_MCMD_STOP_gc;					// generate stop condition
}

/*
 * control - address of the I2C address
 * slave_reg_address - number representing the register of the MCP23017 to read data from
 * returns the data read from slave_reg_address
 */
uint8_t MCP23017_I2C_read(uint8_t control, uint8_t slave_reg_address)
{
	uint8_t data;
	
	while ((TWI0_MSTATUS & TWI_BUSSTATE_gm) != TWI_BUSSTATE_IDLE_gc);	// wait for bus to become idle
	TWI0_MADDR = (control << 1);								// write opcode
	while (!(TWI0_MSTATUS & TWI_WIF_bm));						// wait for previous write operation to complete
	TWI0_MDATA = slave_reg_address;								// write slave register address as data
	while (!(TWI0_MSTATUS & TWI_WIF_bm));						// wait for previous write operation to complete
	TWI0_MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_REPSTART_gc;	// generate a repeated start condition and configure TWI0_MCTRLB to send a NACK on the read
	TWI0_MADDR = (control << 1) | 0x01;							// write opcode to the data register (unsure if this should be written to MADDR or MDATA, but I assume MDATA cause a repeated start condition was already issued)
	while (!(TWI0_MSTATUS & TWI_RIF_bm));						// wait for previous read operation to complete
	data = TWI0_MDATA;											// read data
	TWI0_MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_STOP_gc;			// generate stop condition and reconfigure TWI0_MCTRLB to send an ACK on a read (although this will not be needed in this laboratory)
	
	return data;
}