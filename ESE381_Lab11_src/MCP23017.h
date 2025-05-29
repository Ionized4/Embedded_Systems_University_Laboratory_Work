#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 4000000
#endif
#define MCP23017_BANK_bm (0x01 << 7)
#define MCP23017_I2C_ADDR 0x20

#include <util/delay.h>

/*
 * Initializes TWI0 module to communicate with the MCP23107
 * Writes data to control registers to configure the 8 bit ports
 */
void MCP23017_I2C0_init();

/*
 * control - address of the I2C address
 * slave_reg_address - number representing the register of the MCP23017 to write the data to
 * data - data to write
 */
void MCP23017_I2C_write(uint8_t control, uint8_t slave_reg_address, uint8_t data);

/*
 * control - address of the I2C address
 * slave_reg_address - number representing the register of the MCP23017 to read data from
 * returns the data read from slave_reg_address
 */
uint8_t MCP23017_I2C_read(uint8_t control, uint8_t slave_reg_address);

// structure to hold all the control register addresses for the MCP23017
struct MCP23017_register_set {
	uint8_t IODIRA;
	uint8_t IODIRB;
	uint8_t IPOLA;
	uint8_t IPOLB;
	uint8_t GPINTENA;
	uint8_t GPINTENB;
	uint8_t DEFVALA;
	uint8_t DEFVALB;
	uint8_t INTCONA;
	uint8_t INTCONB;
	uint8_t IOCON;		// uses the IOCON in the A port
	uint8_t GPPUA;
	uint8_t GPPUB;
	uint8_t INTFA;
	uint8_t INTFB;
	uint8_t INTCAPA;
	uint8_t INTCAPB;
	uint8_t GPIOA;
	uint8_t GPIOB;
	uint8_t OLATA;
	uint8_t OLATB;
};

// 8 bit register addresses
const struct MCP23017_register_set MCP23017_8bit_registers;

// 16 bit register addresses
const struct MCP23017_register_set MCP23017_16bit_registers;