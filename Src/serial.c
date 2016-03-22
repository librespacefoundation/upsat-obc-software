#include "../Inc/serial.h"

SAT_returnState serial_app(UART_HandleTypeDef huart, uint8_t tx_flag) {
	uint8_t buf_in[TEST_ARRAY], buf_out[TEST_ARRAY];
	struct tc_tm_pkt pkt_in, pkt_out;
	uint8_t buf_frm_in[TEST_ARRAY], buf_frm_out[TEST_ARRAY];
	uint8_t c_in, c_out, res_in, res2_in, res_out;
	uint16_t size_out, cnt_out, size_in, cnt_in;

    HAL_StatusTypeDef res_uart;
        
	if(tx_flag == 1) {
		
		uint8_t i;
		pack_pkt( buf_out, &pkt_out, &size_out);
		do {
			res_out = HLDLC_frame( &c_out, buf_out, &cnt_out, size_out);
			buf_frm_out[i++] = c_out;	
		} while( res_out != SATR_EOT || res_out != SATR_ERROR);

		HAL_UART_Transmit(&huart, buf_frm_out, i, 10);
	}

	res_uart = HAL_UART_Receive(&huart, &c_in, 1, 10);
	if( res_uart == HAL_OK ) {
		res_in = HLDLC_deframe(buf_in, &cnt_in, c_in, &size_in);
		if(res_in == SATR_EOT) {
			res2_in = unpack_pkt(buf_in, &pkt_in, size_in);
			if(res2_in == SATR_OK) {
				res2_in=route_pkt(&pkt_in);
			} else {
				verify_pkt(&pkt_in, TC_ACK_ACC, res2_in);
			}
		}
	}
};