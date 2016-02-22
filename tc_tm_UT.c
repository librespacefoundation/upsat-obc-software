#include <stdio.h>

#include "tc_tm.h"

#define TEST_ARRAY 20

int main() {

	uint8_t in[TEST_ARRAY], out[TEST_ARRAY];
	
	/*checksum UT*/
	uint8_t res_cs;
	in[0] = 0xE1;
	in[1] = 0x00;
	in[2] = 0xCA;
	in[3] = 0xFE;

	res_cs = checkSum( in, 4);

	if(res_cs == 0xFE) {
		printf("Checksum is ok\n");
	} else {
		printf("Checksum error\n");
	}



	/*unpack_pkt UT*/
	struct tc_tm_pkt res;
	uint16_t size;

	size = 13;
	/*TC*/
	in[0] = 0b00011000;
	in[1] = 0xA9;
	res.type = TC;
	res.app_id = 0xA9;

	in[2] = 0b11000000;
	in[3] = 0xB9;
	res.seq_flags = 3;
	res.seq_count = 0xB9;

	in[4] = 5+2;
	in[5] = 0;

	in[6] = 0b00010000;
	res.ack = 0;

	in[7] = 8;
	res.ser_type = 8;

	in[8] = 1;
	res.ser_subtype = 1;
	
	in[9] = 0xC9;
	res.dest_id = 0xC9;

	in[10] = 0x01;
	in[11] = 0x02;
	res.data[0] = 0x01;
	res.data[1] = 0x02;

	in[12] = checkSum( in, size);

	unpack_pkt(in, &res, size);

	return 0;
}