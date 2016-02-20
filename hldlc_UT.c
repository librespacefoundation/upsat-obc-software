#define TEST_ARRAY 20

#include <stdio.h>

#include "hldlc.h"

int main() {

	uint8_t in[TEST_ARRAY], out[TEST_ARRAY*2], res[TEST_ARRAY*2];
	uint16_t cnt_in, cnt_out, size, res_size;
	uint8_t c;

 	printf("Starting Unit tests\n");


 	in[0] = 0x12;
 	in[1] = 0x13; 
 	in[2] = 0x14; 
 	in[3] = 0x15; 
 	in[4] = 0x16; 
 	in[5] = 0x17; 
 	in[6] = 0x18; 
	in[7] = 0x19; 
 	in[8] = 0x22; 
 	in[9] = 0x23;

	res[0] = 0x7E;
 	res[1] = 0x12; 
 	res[2] = 0x13; 
 	res[3] = 0x14; 
 	res[4] = 0x15; 
 	res[5] = 0x16; 
 	res[6] = 0x17; 
 	res[7] = 0x18; 
	res[8] = 0x19; 
 	res[9] = 0x22; 
 	res[10] = 0x23;
 	res[11] = 0x7E;

 	cnt_in = 0;
 	cnt_out = 0;
 	size = 10;
 	res_size = 12;
    uint8_t check;
    
    printf("%d\n", in[0]);
    
 	do {
 		check = HLDLC_frame( &c, in, &cnt_in, size);
 		out[cnt_out++] = c;
 	} while( check != R_EOT);


 	for(int i =0; i < res_size; i++ ) {
 		if( out[i] == res[i] ) {
 			continue;
 		} else {
 			printf("error\n");
 			break;
 		}
 	}
    return 0;

}