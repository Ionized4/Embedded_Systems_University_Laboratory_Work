/*
 * SCD41.c
 *
 * Created: 4/20/2022 3:31:05 PM
 *  Author: ioniz
 */ 

const uint8_t SCD41_I2C_ADDR;

/* structure used to store all readings from a single measurement */
struct measurement {
	uint16_t co2;
	uint16_t temp;
	uint16_t hum;
};

/*
 * sends a command sequence as specified by the SCD4X data sheet
 * cmd - 16 bit command to send to the SCD4X
 */
void SCD41_send_I2C_command_sequence(uint16_t cmd);

/*
 * performs a I2C read sequence as specified by the SCD4X data sheet
 * cmd - 16 bit command to send to the SCD4X
 * returns the 16 bit value returned from the SCD4X
 */
uint16_t SCD41_read_I2C_sequence(uint16_t cmd);

/*
 * reads a measurement from the SCD4X
 * reads a 16 bit CO2, 16 bit temp, and 16 bit relative humidity value
 * returns all the data in a struct measurement
 */
struct measurement SCD41_read_measurement();

/* configures the TWI0 module to communicate with the SCD41 */
void SCD41_I2C0_init();

/* sends the command to the SCD4X to start periodic measurement */
static inline void SCD41_start_periodic_measurement()
{
	SCD41_send_I2C_command_sequence(0x21B1);
}

/* sends the command to the SCD4X to stop periodic measurement */
static inline void SCD41_stop_periodic_measurement()
{
	SCD41_send_I2C_command_sequence(0x3F86);
}

/* sends the command to the SCD4X to read a value that indicates whether data is ready */
/* if the last 11 bits of the return result are 0, the result is not ready */
static inline uint16_t SCD41_get_data_ready()
{
	return SCD41_read_I2C_sequence(0xE4B8);
}