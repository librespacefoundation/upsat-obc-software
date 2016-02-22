#include <stdio.h>

#include "tc_tm.h"

#define TEST_ARRAY 20

int main() {

	uint8_t in[TEST_ARRAY];
	
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
	struct tc_tm_pkt res, out;
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

	unpack_pkt(in, &out, size);

    if (res.type == out.type) {
        printf("type ok\n");
    } else {
        printf("type error\n");
    }
    
    if (res.app_id == out.app_id) {
        printf("app_id ok\n");
    } else {
        printf("app_id error\n");
    }
    
    if (res.seq_flags == out.seq_flags) {
        printf("seq_flags ok\n");
    } else {
        printf("seq_flags error\n");
    }

    if (res.seq_count == out.seq_count) {
        printf("seq_count ok\n");
    } else {
        printf("seq_count error\n");
    }
    
    if (res.ack == out.ack) {
        printf("ack ok\n");
    } else {
        printf("ack error\n");
    }
    
    if (res.ser_type == out.ser_type) {
        printf("ser_type ok\n");
    } else {
        printf("ser_type error\n");
    }
    
    if (res.ser_subtype == out.ser_subtype) {
        printf("ser_subtype ok\n");
    } else {
        printf("ser_subtype error\n");
    }
    
    if (res.dest_id == out.dest_id) {
        printf("dest_id ok\n");
    } else {
        printf("dest_id error\n");
    }
    
	return 0;
}