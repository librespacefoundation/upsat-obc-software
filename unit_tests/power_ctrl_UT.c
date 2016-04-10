#include <stdio.h>

#include "../power_ctrl.h"
#include "../tc_tm.h"

#define TEST_ARRAY 20

int main() {

	/*unpack_pkt UT*/
	struct tc_tm_pkt res, in;
	uint16_t size;

	size = 16;
	/*TC*/
	in.type = TC;
	in.app_id = 0x01;

	in.seq_flags = 3;
	in.seq_count = 0xB9;

    /*data_hdr+data-1*/
	in.len = 5+5-1;

	in.ack = 0;

	in.ser_type = 8;

	in.ser_subtype = 1;
	
	in.dest_id = 0xC9;

	in.data[0] = SD1;
	in.data[4] = RESET;

    power_control_app(&in);
	
    return 0;
}