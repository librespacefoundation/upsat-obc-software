#include <stdio.h>

#include "../services/services.h"

#define TEST_ARRAY 1024
#define MIN_PKT_SIZE 5
#define MAX_PKT_SIZE 1024


SAT_returnState checkSum(const uint8_t *data, const uint16_t size, uint8_t *res_crc) {

    for(int i=0; i<=size; i++){
        *res_crc = *res_crc ^ data[i];
    }

    return SATR_OK;
}

int main() {

	uint8_t in[TEST_ARRAY], out2[TEST_ARRAY];
	
	/*checksum UT*/
	uint8_t res_cs;
	in[0] = 0xE1;
	in[1] = 0x00;
	in[2] = 0xCA;
	in[3] = 0xFE;

	checkSum(in, 4, &res_cs);

	if(res_cs == 0xFE) {
		printf("Checksum is ok\n");
	} else {
		printf("Checksum error\n");
	}



	/*unpack_pkt UT*/
	tc_tm_pkt res, out;
	uint16_t size;

//temp pkt for test service.

    size = 16;
    /*TC*/
    in[0] = 0b00011000;
    in[1] = OBC_APP_ID;

    in[2] = 0b11000000;
    in[3] = 0xB9;

    /*data_hdr+crc-1*/
    in[4] = 0;
    in[5] = 5+1-1;

    in[6] = 0b00010000;

    in[7] = TC_TEST_SERVICE;

    in[8] = TC_CT_PERFORM_TEST;
    
    in[9] = GND_APP_ID;
    
    in[10] = 0;
    checkSum(in, 9, &in[11]);

    for(int i = 0; i < 12; i++) {
        printf("%d\n", in[i]);
    }
    printf("test service\n");
//temp pkt for fm service.

    size = 16;
    /*TC*/
    in[0] = 0b00011000;
    in[1] = OBC_APP_ID;

    in[2] = 0b11000000;
    in[3] = 0xB9;

    /*data_hdr+crc-1*/
    in[4] = 0;
    in[5] = 5+1+5-1;

    in[6] = 0b00010000;

    in[7] = TC_FUNCTION_MANAGEMENT_SERVICE;

    in[8] = TC_FM_PERFORM_FUNCTION;
    
    in[9] = GND_APP_ID;
    
    in[10] = P_ON; //led on

    in[11] = 0; //led on
    in[12] = 0; 
    in[13] = 0; 
    in[14] = OBC_SD_DEV_ID; //OBC_SD_DEV_ID
    
    in[15] = 0;
    in[16] = 0;
    checkSum(in, 14, &in[16]);

    for(int i = 0; i < 17; i++) {
        printf("%d\n", in[i]);
    }
    printf("FM led on\n");
//temp pkt for fm service.

    size = 16;
    /*TC*/
    in[0] = 0b00011000;
    in[1] = OBC_APP_ID;

    in[2] = 0b11000000;
    in[3] = 0xB9;

    /*data_hdr+crc-1*/
    in[4] = 0;
    in[5] = 5+1+5-1;

    in[6] = 0b00010000;

    in[7] = TC_FUNCTION_MANAGEMENT_SERVICE;

    in[8] = TC_FM_PERFORM_FUNCTION;
    
    in[9] = GND_APP_ID;
    
    in[10] = P_OFF; //led off

    in[11] = 0; //led 
    in[12] = 0; 
    in[13] = 0; 
    in[14] = OBC_SD_DEV_ID; //OBC_SD_DEV_ID
    
    in[15] = 0;
    in[16] = 0;
    checkSum(in, 14, &in[16]);

    for(int i = 0; i < 17; i++) {
        printf("%d\n", in[i]);
    }
    printf("FM led off\n");

//temp pkt for fm service.

    size = 16;
    /*TC*/
    in[0] = 0b00011000;
    in[1] = OBC_APP_ID;

    in[2] = 0b11000000;
    in[3] = 0xB9;

    /*data_hdr+crc-1*/
    in[4] = 0;
    in[5] = 5+1+5-1;

    in[6] = 0b00010001;

    in[7] = TC_FUNCTION_MANAGEMENT_SERVICE;

    in[8] = TC_FM_PERFORM_FUNCTION;
    
    in[9] = GND_APP_ID;
    
    in[10] = P_ON; //led on

    in[11] = 0; //led on
    in[12] = 0; 
    in[13] = 0; 
    in[14] = OBC_SD_DEV_ID; //OBC_SD_DEV_ID
    
    in[15] = 0;
    in[16] = 0;
    checkSum(in, 14, &in[16]);

    for(int i = 0; i < 17; i++) {
        printf("%d\n", in[i]);
    }
    printf("FM led on with ack\n");

    size = 16;
    /*TC*/
    in[0] = 0b00011000;
    in[1] = OBC_APP_ID;

    in[2] = 0b11000000;
    in[3] = 0xB9;

    /*data_hdr+crc-1*/
    in[4] = 0;
    in[5] = 5+1+5-1;

    in[6] = 0b00010001;

    in[7] = TC_FUNCTION_MANAGEMENT_SERVICE;

    in[8] = TC_FM_PERFORM_FUNCTION;
    
    in[9] = GND_APP_ID;
    
    in[10] = P_OFF; //led off

    in[11] = 0; //led 
    in[12] = 0; 
    in[13] = 0; 
    in[14] = OBC_SD_DEV_ID; //OBC_SD_DEV_ID
    
    in[15] = 0;
    in[16] = 0;
    checkSum(in, 14, &in[16]);

    for(int i = 0; i < 17; i++) {
        printf("%d\n", in[i]);
    }
    printf("FM led off with ack\n");
	// size = 16;
	// /*TC*/
	// in[0] = 0b00011000;
	// in[1] = 0x01;
	// res.type = TC;
	// res.app_id = 0x01;

	// in[2] = 0b11000000;
	// in[3] = 0xB9;
	// res.seq_flags = 3;
	// res.seq_count = 0xB9;

 //    /*data_hdr+data-1*/
	// in[4] = 5+5-1;
	// in[5] = 0;
	// res.len = 5+5-1;

	// in[6] = 0b00010000;
	// res.ack = 0;

	// in[7] = 8;
	// res.ser_type = 8;

	// in[8] = 1;
	// res.ser_subtype = 1;
	
	// in[9] = 0xC9;
	// res.dest_id = 0xC9;

	// in[10] = 0x01;
	// in[11] = 0x02;
 //    in[12] = 0x03;
 //    in[13] = 0x04;
 //    in[14] = 0x05;

	// res.data[0] = 0x01;
	// res.data[1] = 0x02;
 //    res.data[2] = 0x03;
 //    res.data[3] = 0x04;
 //    res.data[4] = 0x05;
    
	//  checkSum( in, size-2, &in[15]);

	// unpack_pkt(in, &out, size);

 //    if (res.type == out.type) {
 //        printf("type ok\n");
 //    } else {
 //        printf("type error\n");
 //    }
    
 //    if (res.app_id == out.app_id) {
 //        printf("app_id ok\n");
 //    } else {
 //        printf("app_id error\n");
 //    }
    
 //    if (res.seq_flags == out.seq_flags) {
 //        printf("seq_flags ok\n");
 //    } else {
 //        printf("seq_flags error\n");
 //    }

 //    if (res.seq_count == out.seq_count) {
 //        printf("seq_count ok\n");
 //    } else {
 //        printf("seq_count error\n");
 //    }
    
 //    if (res.ack == out.ack) {
 //        printf("ack ok\n");
 //    } else {
 //        printf("ack error\n");
 //    }
    
 //    if (res.ser_type == out.ser_type) {
 //        printf("ser_type ok\n");
 //    } else {
 //        printf("ser_type error\n");
 //    }
    
 //    if (res.ser_subtype == out.ser_subtype) {
 //        printf("ser_subtype ok\n");
 //    } else {
 //        printf("ser_subtype error\n");
 //    }
    
 //    if (res.dest_id == out.dest_id) {
 //        printf("dest_id ok\n");
 //    } else {
 //        printf("dest_id error\n");
 //    }
    
 //    for(int i = 0; i < res.len-4; i++) {
 // 		if(out.data[0] != res.data[0]) {
 //            printf("Error in data%d\n", i);
 //        } else {
 //            printf("Ok data %d\n",i);
 //        }
 //    }
    
 //    pack_pkt(out2, &res, &size);
    
 //    for (int i = 0; i < size; i++) {
 //        if(in[i] != out2[i]) {
 //            printf("Error in %d\n", i);
 //        } else {
 //            printf("Ok %d\n",i);
 //        }
 //    }
    
    
	return 0;
}