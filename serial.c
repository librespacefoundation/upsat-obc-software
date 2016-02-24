
#include "tc_tm.h"


uint8_t serial_app() {
	uint8_t buf_in[], buf_out[];
	struct tc_tm_pkt pkt_in, pkt_out;
	uint8_t buf_frm_in[], buf_frm_out[];
	uint8_t c_in, c_out, res_in, res_out;
	uint16_t size_out, cnt_out, size_in, cnt_in;

	if(tx_flag == 1) {
		
		uint8_t i;
		pack_pkt( buf_out, &pkt_out, &size_out);
		do {
			res_out = HLDLC_frame( &c_out, buf_out, &cnt_out, size_out);
			buf_frm_out[i++] = c_out;	
		} while( res_out != R_EOT || res_out != R_ERROR);

		HAL_UART_Transmit(UART_HandleTypeDef *huart, buf_frm_out, i, uint32_t Timeout);
	}

	HAL_UART_Receive(UART_HandleTypeDef *huart, &c_in, 1, uint32_t Timeout);
	if() {
		res_in = HLDLC_deframe( buf_in, &cnt_in, c, &size_in);
		if(res_in == R_EOT) {
			res2_in = unpack_pkt(buf_in, &pkt_in, size_in);
			if(res2_in == R_OK) {
				res2_in=route_pkt(&pkt_in);
			}
			verify_pkt( &pkt_in, ACC, res2_in);
		}
	}
};