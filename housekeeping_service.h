#include <stdint.h>

struct _obc_status {
	uint8_t mode;
	uint8_t batt_curr;
	uint8_t batt_volt;
	uint8_t bus_3v3_curr;
	uint8_t bus_5v_curr;
	uint8_t temp_eps;
	uint8_t temp_batt;
	uint8_t temp_comms;

}obc_status;