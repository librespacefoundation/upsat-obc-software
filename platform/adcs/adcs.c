#include "adcs.h"

#undef __FILE_ID__
#define __FILE_ID__ 666

const uint8_t services_verification_ADCS_TC[MAX_SERVICES][MAX_SUBTYPES] = { 
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

struct _adcs_data adcs_data;


SAT_returnState route_pkt(tc_tm_pkt *pkt) {

    SAT_returnState res;
    TC_TM_app_id id;

    if(!C_ASSERT(pkt != NULL && pkt->data != NULL) == true)                         { verification_app(pkt); free_pkt(pkt); return SATR_ERROR; }
    if(!C_ASSERT(pkt->type == TC || pkt->type == TM) == true)                       { verification_app(pkt); free_pkt(pkt); return SATR_ERROR; }
    if(!C_ASSERT(pkt->app_id < LAST_APP_ID && pkt->dest_id < LAST_APP_ID) == true)  { verification_app(pkt); free_pkt(pkt); return SATR_ERROR; }

    if(pkt->type == TC)         { id = pkt->app_id; } 
    else if(pkt->type == TM)    { id = pkt->dest_id; }

    if(id == ADCS_APP_ID && pkt->ser_type == TC_HOUSEKEEPING_SERVICE) {
        //C_ASSERT(pkt->ser_subtype == 21 || pkt->ser_subtype == 23) { free_pkt(pkt); return SATR_ERROR; }
        res = hk_app(pkt);
    } else if(id == ADCS_APP_ID && pkt->ser_type == TC_FUNCTION_MANAGEMENT_SERVICE) {
        res = function_management_app(pkt);
    } else if(id == ADCS_APP_ID && pkt->ser_type == TC_TEST_SERVICE) {
        //C_ASSERT(pkt->ser_subtype == 1 || pkt->ser_subtype == 2 || pkt->ser_subtype == 9 || pkt->ser_subtype == 11 || pkt->ser_subtype == 12 || pkt->ser_subtype == 13) { free_pkt(pkt); return SATR_ERROR; }
        res = test_app(pkt);
    } 
    else if(id == EPS_APP_ID)      { export_pkt(OBC_APP_ID, pkt); }
    else if(id == ADCS_APP_ID)     { export_pkt(OBC_APP_ID, pkt); }
    else if(id == COMMS_APP_ID)    { export_pkt(OBC_APP_ID, pkt); }
    else if(id == IAC_APP_ID)      { export_pkt(OBC_APP_ID, pkt); }
    else if(id == GND_APP_ID)      { export_pkt(OBC_APP_ID, pkt); }
    else if(id == DBG_APP_ID)      { export_pkt(OBC_APP_ID, pkt); }

    verification_app(pkt);
    free_pkt(pkt);
    return SATR_OK;
}

SAT_returnState event_log(uint8_t *buf, const uint16_t size) {
    return SATR_OK;
}

SAT_returnState adcs_data_INIT() {

    sys_data.boot_counter = 0; //should get it from eeprom
    return SATR_OK;
}
