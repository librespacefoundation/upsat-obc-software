#include <stdio.h>

#include "../route_verification.h"
#include "../tc_tm.h"

#define TEST_ARRAY 20

uint8_t __power_control_app(struct tc_tm_pkt *pkt) {
    printf("Called ok\n");
    return 1;
}

uint8_t __hk_app(struct tc_tm_pkt *pkt) {
    printf("Called error\n");
    return 1;
}

#define power_control_app   __power_control_app
#define hk_app   __hk_app

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

    route_pkt(&in);
	
    return 0;
}