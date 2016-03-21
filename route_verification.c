#include "route_verification.h"

OBC_returnStateTypedef route_pkt(tc_tm_pkt *pkt) {

    OBC_returnStateTypedef res;
    uint16_t id;

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true)                         { free_pkt(pkt); return R_ERROR; }
    if(!C_ASSERT(pkt->type == TC || pkt->type == TM) == true)                       { free_pkt(pkt); return R_ERROR; }
    if(!C_ASSERT(pkt->app_id < LAST_APP_ID && pkt->dest_id < LAST_APP_ID) == true)  { free_pkt(pkt); return R_ERROR; }

    if(pkt->type == TC)         { id = pkt->app_id; } 
    else if(pkt->type == TM)    { id = pkt->dest_id; }

    if(id == OBC && pkt->ser_type == TC_HOUSEKEEPING_SERVICE) {
        //C_ASSERT(pkt->ser_subtype == 21 || pkt->ser_subtype == 23) { free_pkt(pkt); return R_ERROR; }
        res = hk_app(pkt);
        verification_service_app(pkt, res);
    } else if(id == OBC && pkt->ser_type == TC_FUNCTION_MANAGEMENT_SERVICE) {
        res = power_control_app(pkt);
    } else if(id == OBC && pkt->ser_type == TC_LARGE_DATA_SERVICE) {
        res = large_data_app(pkt);
    } else if(id == OBC && pkt->ser_type == TC_MASS_STORAGE_SERVICE) {
        //C_ASSERT(pkt->ser_subtype == 1 || pkt->ser_subtype == 2 || pkt->ser_subtype == 9 || pkt->ser_subtype == 11 || pkt->ser_subtype == 12 || pkt->ser_subtype == 13) { free_pkt(pkt); return R_ERROR; }
        res = mass_storage_app(pkt);
    } 
    else if(id == EPS)      { export_eps_pkt(pkt); } 
    else if(id == ADCS)     { export_eps_pkt(pkt); }
    else if(id == COMMS)    { export_eps_pkt(pkt); } 
    else if(id == IAC)      { export_eps_pkt(pkt); } 
    else if(id == GND)      { export_eps_pkt(pkt); } 

    free_pkt(pkt);
    return R_OK;
}

OBC_returnStateTypedef export_eps_pkt(tc_tm_pkt *pkt) {

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true) { return R_ERROR; }

    uint8_t c = 0;
    uint16_t size = 0;
    uint16_t cnt = 0;
    uint16_t cnt_out = 0;
    uint8_t buf[TEST_ARRAY];
    uint8_t buf_out[TEST_ARRAY];
    OBC_returnStateTypedef res;    

    pack_pkt(buf, pkt, &size);
    for(uint16_t i = 0; i < size*2; i++) {
        res = HLDLC_frame(&c, buf, &cnt_out, size);
        if(res == R_EOT || res != R_ERROR) { cnt = i; break; }
        buf_out[i++] = c;   
    }

    if(!C_ASSERT(cnt > 0) == true) { return R_ERROR; }

    HAL_eps_uart_tx(buf_out, cnt);

    return R_OK;
}

OBC_returnStateTypedef import_eps_pkt() {

    tc_tm_pkt *pkt;
    uint8_t c = 0;
    uint16_t size = 0;
    uint16_t cnt = 0;
    uint16_t cnt_out = 0;
    uint8_t buf[TEST_ARRAY];
    uint8_t buf_out[TEST_ARRAY];
    OBC_returnStateTypedef res;    
    OBC_returnStateTypedef res_deframe; 
    OBC_returnStateTypedef res_unpack;
    OBC_returnStateTypedef res_route; 

    res = HAL_eps_uart_rx(&c, 1, 10);
    if( res == R_OK ) {
        res_deframe = HLDLC_deframe(buf, &cnt, c, &size);
        if(res_deframe == R_EOT) {
            
            pkt = get_pkt(NORMAL);
            if(!C_ASSERT(pkt != NULL) == true) { return R_ERROR; }
            res_unpack = unpack_pkt(buf, pkt, size);
            if(res_unpack == R_OK) { res_route = route_pkt(pkt); } 
            else {
                //verify_pkt(&pkt_in, TC_ACK_ACC, res_route);
            }
        }
    }

    return R_OK;
}
