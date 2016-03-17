#include "route_verification.h"

OBC_returnStateTypedef route_pkt(tc_tm_pkt *pkt) {
    uint16_t id;

    if(pkt->type == TC) {
        id = pkt->app_id;
    } else if(pkt->type == TM) {
        id = pkt->dest_id;
    } else {
        return R_ERROR;
    }

    if(id == OBC && pkt->ser_type == TC_HOUSEKEEPING_SERVICE && (pkt->ser_subtype == 21 || pkt->ser_subtype == 23)) {
        hk_app(pkt);
    } else if(id == OBC && pkt->ser_type == TC_FUNCTION_MANAGEMENT_SERVICE && pkt->ser_subtype == 1) {
        uint8_t fid = pkt->data[0];
        if(fid == 1 || fid == 2 || fid == 3 ){
            power_control_app(pkt);
        } else {
            return R_ERROR;
        }
    } else if(id == OBC && pkt->ser_type == TC_LARGE_DATA_SERVICE) {
        large_data_app(pkt);
    } else if(id == OBC && pkt->ser_type == TC_MASS_STORAGE_SERVICE && (pkt->ser_subtype == 1 || pkt->ser_subtype == 2 || pkt->ser_subtype == 9 || pkt->ser_subtype == 11 || pkt->ser_subtype == 12 || pkt->ser_subtype == 13)) {
        mass_storage_app(pkt);
    } else if(id == EPS) {
    } else if(id == ADCS) {

    } else if(id == COMMS) {
    } else if(id == IAC) {

    } else if(id == GND) {
        
    } else {
        return R_ERROR;
    }

    return R_OK;
}

OBC_returnStateTypedef serial_app(UART_HandleTypeDef huart, uint8_t tx_flag) {
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
        } while( res_out != R_EOT || res_out != R_ERROR);

        HAL_UART_Transmit(&huart, buf_frm_out, i, 10);
    }

    res_uart = HAL_UART_Receive(&huart, &c_in, 1, 10);
    if( res_uart == HAL_OK ) {
        res_in = HLDLC_deframe(buf_in, &cnt_in, c_in, &size_in);
        if(res_in == R_EOT) {
            res2_in = unpack_pkt(buf_in, &pkt_in, size_in);
            if(res2_in == R_OK) {
                res2_in=route_pkt(&pkt_in);
            } else {
                verify_pkt(&pkt_in, TC_ACK_ACC, res2_in);
            }
        }
    }
};
