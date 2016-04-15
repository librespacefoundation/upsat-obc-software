#include "obc.h"

#undef __FILE_ID__
#define __FILE_ID__ 666

const uint8_t services_verification_OBC_TC[MAX_SERVICES][MAX_SUBTYPES] = { 
/*    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 */
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, /*TC_VERIFICATION_SERVICE*/
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0 }, /*TC_HOUSEKEEPING_SERVICE*/
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, /*TC_FUNCTION_MANAGEMENT_SERVICE*/
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, /*TC_SCHEDULING_SERVICE*/
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 }, /*TC_LARGE_DATA_SERVICE*/
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, /*TC_MASS_STORAGE_SERVICE*/
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, /*TC_TEST_SERVICE*/
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

struct _obc_data obc_data;
struct _sat_status sat_status;

SAT_returnState route_pkt(tc_tm_pkt *pkt) {

    SAT_returnState res;
    TC_TM_app_id id;

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true)                         { verification_app(pkt); free_pkt(pkt); return SATR_ERROR; }
    if(!C_ASSERT(pkt->type == TC || pkt->type == TM) == true)                       { verification_app(pkt); free_pkt(pkt); return SATR_ERROR; }
    if(!C_ASSERT(pkt->app_id < LAST_APP_ID && pkt->dest_id < LAST_APP_ID) == true)  { verification_app(pkt); free_pkt(pkt); return SATR_ERROR; }

    if(pkt->type == TC)         { id = pkt->app_id; } 
    else if(pkt->type == TM)    { id = pkt->dest_id; }

    if(id == SYSTEM_APP_ID && pkt->ser_type == TC_HOUSEKEEPING_SERVICE) {
        //C_ASSERT(pkt->ser_subtype == 21 || pkt->ser_subtype == 23) { free_pkt(pkt); return SATR_ERROR; }
        res = hk_app(pkt);
    } else if(id == SYSTEM_APP_ID && pkt->ser_type == TC_FUNCTION_MANAGEMENT_SERVICE) {
        res = function_management_app(pkt);
    } else if(id == SYSTEM_APP_ID && pkt->ser_type == TC_LARGE_DATA_SERVICE) {
        res = large_data_app(pkt);
    } else if(id == SYSTEM_APP_ID && pkt->ser_type == TC_MASS_STORAGE_SERVICE) {
        //C_ASSERT(pkt->ser_subtype == 1 || pkt->ser_subtype == 2 || pkt->ser_subtype == 9 || pkt->ser_subtype == 11 || pkt->ser_subtype == 12 || pkt->ser_subtype == 13) { free_pkt(pkt); return SATR_ERROR; }
        res = mass_storage_app(pkt);
    } else if(id == SYSTEM_APP_ID && pkt->ser_type == TC_TEST_SERVICE) {
        //C_ASSERT(pkt->ser_subtype == 1 || pkt->ser_subtype == 2 || pkt->ser_subtype == 9 || pkt->ser_subtype == 11 || pkt->ser_subtype == 12 || pkt->ser_subtype == 13) { free_pkt(pkt); return SATR_ERROR; }
        res = test_app(pkt);
    } 
    else if(id == EPS_APP_ID)      { export_pkt(EPS_APP_ID, pkt, &obc_data.eps_uart); }
    else if(id == ADCS_APP_ID)     { export_pkt(ADCS_APP_ID, pkt, &obc_data.adcs_uart); }
    else if(id == COMMS_APP_ID)    { export_pkt(COMMS_APP_ID, pkt, &obc_data.comms_uart); }
    else if(id == IAC_APP_ID)      { export_pkt(DBG_APP_ID, pkt, &obc_data.dbg_uart); }
    else if(id == GND_APP_ID)      { export_pkt(DBG_APP_ID, pkt, &obc_data.dbg_uart); }
    else if(id == DBG_APP_ID)      { export_pkt(DBG_APP_ID, pkt, &obc_data.dbg_uart); }

    verification_app(pkt);
    free_pkt(pkt);
    return SATR_OK;
}

SAT_returnState obc_data_INIT() {

    obc_data.obc_seq_cnt = 0;
    bkup_sram_INIT();
    return SATR_OK;
}


void bkup_sram_INIT() {

    obc_data.log_cnt = HAL_obc_BKPSRAM_BASE();
    obc_data.log_state = HAL_obc_BKPSRAM_BASE() + 1;
    sys_data.boot_counter = HAL_obc_BKPSRAM_BASE() + 2;
    obc_data.file_id = HAL_obc_BKPSRAM_BASE() + 3;
    obc_data.wod_cnt = HAL_obc_BKPSRAM_BASE() + 4;

    obc_data.log = HAL_obc_BKPSRAM_BASE() + 5;

    obc_data.wod_log = HAL_obc_BKPSRAM_BASE() + 5 + (EV_MAX_BUFFER/32);

    //for testing only
    *obc_data.log_cnt = 0;
    *obc_data.wod_cnt = 0;
    
    if(!C_ASSERT(*obc_data.log_cnt < EV_MAX_BUFFER) == true) { *obc_data.log_cnt = 0; }
    if(!C_ASSERT(*obc_data.wod_cnt < EV_MAX_BUFFER) == true) { *obc_data.wod_cnt = 0; }
}

uint32_t get_new_fileId() {

    (*obc_data.file_id)++;
    if(*obc_data.file_id > MAX_FILE_NUM) {
        *obc_data.file_id = 1;
    }
    return *obc_data.file_id;
}

SAT_returnState event_log(uint8_t *buf, const uint16_t size) {

    union _cnv temp_cnv;
  
    for(uint16_t i = 0; i < size; i++) {
        uint32_t point = ((*obc_data.log_cnt) >> 2);
        temp_cnv.cnv32 = obc_data.log[point];
        temp_cnv.cnv8[(0x00000003 & *obc_data.log_cnt)] = buf[i];
        obc_data.log[point] = temp_cnv.cnv32;
        //obc_data.log[point] &= 0xFFFF
        //obc_data.log[point] |= (buf[i] << ((0x00000003 & *obc_data.log_cnt) * 8));
        (*obc_data.log_cnt)++;
        if(*obc_data.log_cnt >= EV_MAX_BUFFER) { *obc_data.log_cnt = 0; }

        if(*obc_data.log_state == ev_free_1 && *obc_data.log_cnt > (EV_MAX_BUFFER / 2)) { *obc_data.log_state = ev_wr_1; }
        else if(*obc_data.log_state == ev_free_2 && *obc_data.log_cnt < (EV_MAX_BUFFER / 2)) { *obc_data.log_state = ev_wr_2; }
        else if(*obc_data.log_state == ev_wr_1 && *obc_data.log_cnt < (EV_MAX_BUFFER / 2)) { *obc_data.log_state = ev_owr_2; }
        else if(*obc_data.log_state == ev_wr_2 && *obc_data.log_cnt > (EV_MAX_BUFFER / 2)) { *obc_data.log_state = ev_owr_1; }
    }

    return SATR_OK;
}

SAT_returnState event_log_load(uint8_t *buf, const uint16_t pointer, const uint16_t size) {
   for(uint16_t i = 0; i < size; i++) {
        buf[i] = obc_data.log[(pointer + i) >> 2] >> ((0x00000003 & i) * 8);
   }
   return SATR_OK;
}

    uint16_t cnt = 0;
    uint8_t buf[TEST_ARRAY];

SAT_returnState event_log_IDLE() {

    if(*obc_data.log_state == ev_wr_1 || *obc_data.log_state == ev_owr_1) { 
        uint16_t size = (EV_MAX_BUFFER / 2);

        for(uint16_t i = 0; i < size ; i+=4) {
            cnv32_8(obc_data.log[i], &buf[i]);
        }
        mass_storage_storeLogs(EVENT_LOG, buf, &size);

        *obc_data.log_state = ev_free_2;

    } else if(*obc_data.log_state == ev_wr_2 || *obc_data.log_state == ev_owr_2) { 
        uint16_t size = (EV_MAX_BUFFER / 2);

        for(uint16_t i = 0; i < size ; i+=4) {
            cnv32_8(obc_data.log[i + size], &buf[i]);
        }
        mass_storage_storeLogs(EVENT_LOG, buf, &size);

        *obc_data.log_state = ev_free_1;
    }
    
     return SATR_OK;
}

SAT_returnState wod_log() {

//check endianess

    obc_data.wod_log[*obc_data.wod_cnt] = (sat_status.batt_curr << 24) || (sat_status.batt_volt << 16) || (sat_status.bus_3v3_curr << 8) || sat_status.bus_5v_curr; 
     
    (*obc_data.wod_cnt)++;
    if(*obc_data.wod_cnt >= WOD_MAX_BUFFER) { *obc_data.wod_cnt = 0; }

    obc_data.wod_log[*obc_data.wod_cnt] = (sat_status.temp_eps << 16) || (sat_status.temp_batt << 8) || sat_status.temp_comms;

    (*obc_data.wod_cnt)++;
    if(*obc_data.wod_cnt >= WOD_MAX_BUFFER) { *obc_data.wod_cnt = 0; }

    return SATR_OK;
}

SAT_returnState wod_log_load(uint8_t *buf) {

//    union _cnv temp_cnv;

//    temp_cnv.cnv32 = obc_data.wod_log[*obc_data.wod_cnt]; 
//    buf[i] = temp_cnv.cnv8[3];
//    buf[i] = temp_cnv.cnv8[2];
//    buf[i] = temp_cnv.cnv8[1];
//    buf[i] = temp_cnv.cnv8[0];

//    temp_cnv.cnv32 = obc_data.wod_log[*obc_data.wod_cnt]; 
//    buf[i] = temp_cnv.cnv8[2];
//    buf[i] = temp_cnv.cnv8[1];
//    buf[i] = temp_cnv.cnv8[0];
   
//   for(uint16_t i = 0; i < size; i++) {
//        buf[i] = obc_data.log[(pointer + i) >> 2] >> ((0x00000003 & i) * 8);
//   }
   return SATR_OK;
}
